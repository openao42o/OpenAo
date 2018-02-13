// MonsterDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "MonsterDetailDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterDetailDlg dialog


CMonsterDetailDlg::CMonsterDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterDetailDlg)
	m_valueEditLevel = _T("1");
	m_valueEditUniqueNumber = _T("0");
	m_valueEditMonsterName = _T("");
	m_valueEditExperience = _T("0");
	m_valueEditHP = _T("0");
	m_valueEditSpeed = _T("1");
	m_valueEditSizeForServer = _T("1");
	m_valueEditSizeForClient = _T("1");
	m_valueEditDefense = _T("0");
	m_valueEditDefenseProbability = _T("0");
	m_valueEditAlphaBlending = _T("0");
	m_valueEditAttackRange = _T("0");
	m_valueEditHPRecoveryTime = _T("0");
	m_valueEditHPRecoveryValue = _T("0");
	m_valueEditScaleValue = _T("1.0");
	m_valueEditTextureIndex = _T("0");
	m_valueEditSourceIndex = _T("0");
	m_valueEditQuickTurnAngle = _T("30");
	m_valueEditQuickTurnSpeed = _T("1");
	m_valueEditQuickTurnTerm = _T("0");
	m_valueEditMPOptionParam1 = _T("0");
	m_valueEditMPOptionParam2 = _T("0");
	m_valueEditTurnAngle = _T("0.0");
	m_valueEditRange = _T("0");
	m_valueCheckMoveParty = FALSE;
	m_valueCheckPatternMonster = FALSE;
	m_valueCheckBossMonster = FALSE;
	m_valueCheckDeadFall = FALSE;
	m_valueCheckAutoDestroy = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd(); //메인 다이얼로그 포인터 얻기
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pMonsterDlg = (CMonsterDlg*)pParent; // 부모 클래스 포인터 저장
	m_pMonsterAttackDetailDlg = NULL;
}

CMonsterDetailDlg::~CMonsterDetailDlg()
{
// 2006-01-23 by cmkwon
// 	util::del(m_pODBCStmt);
	util::del(m_pMonsterAttackDetailDlg);
}


void CMonsterDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterDetailDlg)
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEditUniqueNumber);
	DDX_Control(pDX, IDC_COMBO12, m_ctrlComboAttackObject);
	DDX_Control(pDX, IDC_COMBO8, m_ctrlComboAttackPattern);
	DDX_Control(pDX, IDC_COMBO10, m_ctrlComboBelligerence);
	DDX_Control(pDX, IDC_COMBO9, m_ctrlComboMovePattern);
	DDX_Control(pDX, IDC_COMBO7, m_ctrlComboMonsterForm);
	DDX_Control(pDX, IDC_COMBO6, m_ctrlComboMonsterItem05);
	DDX_Control(pDX, IDC_COMBO4, m_ctrlComboMonsterItem04);
	DDX_Control(pDX, IDC_COMBO5, m_ctrlComboMonsterItem03);
	DDX_Control(pDX, IDC_COMBO3, m_ctrlComboMonsterItem02);
	DDX_Control(pDX, IDC_COMBO2, m_ctrlComboMonsterItem01);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlComboRace);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_ctrlButtonCancel);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ctrlButtonSave);
	DDX_Control(pDX, IDC_BUTTON_MAKE, m_ctrlButtonMake);
	DDX_Text(pDX, IDC_EDIT3, m_valueEditLevel);
	DDX_Text(pDX, IDC_EDIT1, m_valueEditUniqueNumber);
	DDX_Text(pDX, IDC_EDIT2, m_valueEditMonsterName);
	DDX_Text(pDX, IDC_EDIT4, m_valueEditExperience);
	DDX_Text(pDX, IDC_EDIT5, m_valueEditHP);
	DDX_Text(pDX, IDC_EDIT6, m_valueEditSpeed);
	DDX_Text(pDX, IDC_EDIT8, m_valueEditSizeForServer);
	DDX_Text(pDX, IDC_EDIT9, m_valueEditSizeForClient);
	DDX_Text(pDX, IDC_EDIT11, m_valueEditDefense);
	DDX_Text(pDX, IDC_EDIT12, m_valueEditDefenseProbability);
	DDX_Text(pDX, IDC_EDIT13, m_valueEditAlphaBlending);
	DDX_Text(pDX, IDC_EDIT15, m_valueEditAttackRange);
	DDX_Text(pDX, IDC_EDIT16, m_valueEditHPRecoveryTime);
	DDX_Text(pDX, IDC_EDIT17, m_valueEditHPRecoveryValue);
	DDX_Text(pDX, IDC_EDIT19, m_valueEditScaleValue);
	DDX_Text(pDX, IDC_EDIT20, m_valueEditTextureIndex);
	DDX_Text(pDX, IDC_EDIT21, m_valueEditSourceIndex);
	DDX_Text(pDX, IDC_EDIT22, m_valueEditQuickTurnAngle);
	DDX_Text(pDX, IDC_EDIT23, m_valueEditQuickTurnSpeed);
	DDX_Text(pDX, IDC_EDIT24, m_valueEditQuickTurnTerm);
	DDX_Text(pDX, IDC_EDIT25, m_valueEditMPOptionParam1);
	DDX_Text(pDX, IDC_EDIT26, m_valueEditMPOptionParam2);
	DDX_Text(pDX, IDC_EDIT28, m_valueEditTurnAngle);
	DDX_Text(pDX, IDC_EDIT27, m_valueEditRange);
	DDX_Check(pDX, IDC_CHECK_BIT_MOVE_PARTY, m_valueCheckMoveParty);
	DDX_Check(pDX, IDC_CHECK_BIT_PATTERN_MONSTER, m_valueCheckPatternMonster);
	DDX_Check(pDX, IDC_CHECK_BIT_BOSS_MONSTER, m_valueCheckBossMonster);
	DDX_Check(pDX, IDC_CHECK_BIT_DEAD_FALL, m_valueCheckDeadFall);
	DDX_Check(pDX, IDC_CHECK_BIT_AUTO_DESTROY, m_valueCheckAutoDestroy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterDetailDlg)
	ON_BN_CLICKED(IDC_BUTTON_MAKE, OnButtonMake)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_ATTACKITEM, OnButtonAttackitem)
	ON_BN_CLICKED(IDC_BUTTON_DROPITEM, OnButtonDropitem)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERITEM01, OnButtonMonsteritem01)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERITEM2, OnButtonMonsteritem2)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERITEM3, OnButtonMonsteritem3)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERITEM4, OnButtonMonsteritem4)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERITEM5, OnButtonMonsteritem5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterDetailDlg message handlers

BOOL CMonsterDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// DB 연결 초기화
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		EndDialog(-1);
//		return FALSE;
//	}

	m_ctrlComboRace.InsertString(0,STRCMD_CS_COMMON_RACE_BATTALUS);
	m_ctrlComboRace.InsertString(1,STRCMD_CS_COMMON_RACE_DECA);
	m_ctrlComboRace.InsertString(2,STRCMD_CS_COMMON_RACE_PHILON);
	m_ctrlComboRace.InsertString(3,STRCMD_CS_COMMON_RACE_SHARRINE);

// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	CString szSQLQuery;
// 	
// 	//itemnum이 75로 시작하는 아이템 가져오기
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select itemnum from ti_item where itemnum like '75%%' ");
// 	szSQLQuery.Format(QUERY_080702_0105);
// 
// 	SQLINTEGER cb1;
// 
// 	int tempMonsterItem;
// 	CString szMonsterItem;
// 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempMonsterItem, 0, &cb1);
// 
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (!bRet)
// 	{
// 		// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Error");
// 		return FALSE;
// 	}
// 
// 	// DB에 값이 없을때까지 loop를 돈다
// 	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
// 	{
// 		szMonsterItem.Format("%d", tempMonsterItem);
// 		//select된 값을 넣어준다.
// 		m_ctrlComboMonsterItem01.InsertString(0,szMonsterItem);
// 		m_ctrlComboMonsterItem02.InsertString(0,szMonsterItem);
// 		m_ctrlComboMonsterItem03.InsertString(0,szMonsterItem);
// 		m_ctrlComboMonsterItem04.InsertString(0,szMonsterItem);
// 		m_ctrlComboMonsterItem05.InsertString(0,szMonsterItem);
// 	}
// 
// 	// clean up
// 	m_pODBCStmt->FreeStatement();

	m_ctrlComboMonsterForm.InsertString(0,"FORM_FLYING_RIGHT");
	m_ctrlComboMonsterForm.InsertString(1,"FORM_FLYING_COPTER");
	m_ctrlComboMonsterForm.InsertString(2,"FORM_GROUND_MOVE");
	m_ctrlComboMonsterForm.InsertString(3,"FORM_FLYINGandGROUND_RIGHT");
	m_ctrlComboMonsterForm.InsertString(4,"FORM_FLYINGandGROUND_COPTER");
	m_ctrlComboMonsterForm.InsertString(5,"FORM_SWIMMINGFLYING_RIGHT");
	m_ctrlComboMonsterForm.InsertString(6,"FORM_SWIMMINGFLYING_COPTER");
	m_ctrlComboMonsterForm.InsertString(7,"FORM_OBJECT_STOP");
	m_ctrlComboMonsterForm.InsertString(8,"FORM_OBJECT_PLANE_ROTATE");
	m_ctrlComboMonsterForm.InsertString(9,"FORM_OBJECT_CANNON");

	m_ctrlComboAttackPattern.InsertString(0,"ATTPAT_NORMAL");
	m_ctrlComboAttackPattern.InsertString(1,"ATTPAT_SELFDESTRUCTION");
	m_ctrlComboAttackPattern.InsertString(2,"ATTPAT_RETREAT");
	m_ctrlComboAttackPattern.InsertString(3,"ATTPAT_HITANDRUN");
	m_ctrlComboAttackPattern.InsertString(4,"ATTPAT_ONEATATTCK");
	m_ctrlComboAttackPattern.InsertString(5,"ATTPAT_PK");

	m_ctrlComboMovePattern.InsertString(0,"MOVPAT_STOP");
	m_ctrlComboMovePattern.InsertString(1,"MOVPAT_STRAIGHT");
	m_ctrlComboMovePattern.InsertString(2,"MOVPAT_RIGHTANDLEFT");
	m_ctrlComboMovePattern.InsertString(3,"MOVPAT_8RIGHTANDLEFT");
	m_ctrlComboMovePattern.InsertString(4,"MOVPAT_PATROL");
	m_ctrlComboMovePattern.InsertString(5,"MOVPAT_PARTY");
	m_ctrlComboMovePattern.InsertString(6,"MOVPAT_UPANDDOWN");
	m_ctrlComboMovePattern.InsertString(7,"MOVPAT_OBLIQUE");
	m_ctrlComboMovePattern.InsertString(8,"MOVPAT_OBLIQUEANDCURVE");
	m_ctrlComboMovePattern.InsertString(9,"MOVPAT_TELEPORT");

	m_ctrlComboBelligerence.InsertString(0,"BELL_ATATTACK");
	m_ctrlComboBelligerence.InsertString(1,"BELL_DEFENSE");
	m_ctrlComboBelligerence.InsertString(2,"BELL_RETREAT");
	m_ctrlComboBelligerence.InsertString(3,"BELL_TAGETATATTACK");
	m_ctrlComboBelligerence.InsertString(4,"BELL_NOTATTACK");
	m_ctrlComboBelligerence.InsertString(5,"BELL_INFLUENCE_VCN");
	m_ctrlComboBelligerence.InsertString(6,"BELL_INFLUENCE_ANI");

	m_ctrlComboAttackObject.InsertString(0,"ATTACKOBJ_CLOSERANGE");
	m_ctrlComboAttackObject.InsertString(1,"ATTACKOBJ_FIRSTATTACK");
	m_ctrlComboAttackObject.InsertString(2,"ATTACKOBJ_LOWHP");
	m_ctrlComboAttackObject.InsertString(3,"ATTACKOBJ_HIGHHP");
	m_ctrlComboAttackObject.InsertString(4,"ATTACKOBJ_SAMERACE");
	m_ctrlComboAttackObject.InsertString(5,"ATTACKOBJ_RANDOM");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonsterDetailDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	if(ValidateCheck()== FALSE)
		return;
	m_pMonsterDlg->m_GridCurrent.DeleteRow(m_nSelectRow);
	ReflectionToGrid(2);
}

void CMonsterDetailDlg::OnButtonMake() 
{
	// TODO: Add your control notification handler code here
	if(ValidateCheck()== FALSE)
		return;
	ReflectionToGrid(1);
}

void CMonsterDetailDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CMonsterDetailDlg::ReflectionToGrid(int type)
{
	// 칼럼 만들기
	m_pMonsterDlg->m_GridUpdate.SetRowCount(m_pMonsterDlg->m_GridUpdate.GetRowCount()+1);

	//각 콘트롤의 값들을 CWeaponItem의 GridUpdate에 넣는다.
	UpdateData();
	
	int m_nCols= 0;
	CString tempValue;
	int n_tempValue = 0;
	if(type == 1)
		m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, "INSERT");
	else if(type == 2)
		m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, "UPDATE");
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditUniqueNumber);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMonsterName);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditLevel);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditExperience);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditHP);

	m_ctrlComboRace.GetWindowText(tempValue);
	int tempbit1 = 0;
	
	if(tempValue == STRCMD_CS_COMMON_RACE_BATTALUS)
		tempbit1 = RACE_BATTALUS;
	else if(tempValue == STRCMD_CS_COMMON_RACE_DECA)
		tempbit1 = RACE_DECA;
	else if(tempValue == STRCMD_CS_COMMON_RACE_PHILON)
		tempbit1 = RACE_PHILON;
	else if(tempValue == STRCMD_CS_COMMON_RACE_SHARRINE)
		tempbit1 = RACE_SHARRINE;

	tempValue.Format("%d", tempbit1);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSpeed);

	m_ctrlComboMonsterItem01.GetWindowText(tempValue);
	if(tempValue == "") tempValue = "0";
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	m_ctrlComboMonsterItem02.GetWindowText(tempValue);
	if(tempValue == "") tempValue = "0";
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	m_ctrlComboMonsterItem03.GetWindowText(tempValue);
	if(tempValue == "") tempValue = "0";
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	m_ctrlComboMonsterItem04.GetWindowText(tempValue);
	if(tempValue == "") tempValue = "0";
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	m_ctrlComboMonsterItem05.GetWindowText(tempValue);
	if(tempValue == "") tempValue = "0";
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSizeForServer);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSizeForClient);

	m_ctrlComboMonsterForm.GetWindowText(tempValue);
	if(tempValue == "FORM_FLYING_RIGHT")
		tempValue.Format("%d", FORM_FLYING_RIGHT);
	else if(tempValue == "FORM_FLYING_COPTER")
		tempValue.Format("%d", FORM_FLYING_COPTER);
	else if(tempValue == "FORM_GROUND_MOVE")
		tempValue.Format("%d", FORM_GROUND_MOVE);
	else if(tempValue == "FORM_FLYINGandGROUND_RIGHT")
		tempValue.Format("%d", FORM_FLYINGandGROUND_RIGHT);
	else if(tempValue == "FORM_FLYINGandGROUND_COPTER")
		tempValue.Format("%d", FORM_FLYINGandGROUND_COPTER);
	else if(tempValue == "FORM_SWIMMINGFLYING_RIGHT")
		tempValue.Format("%d", FORM_SWIMMINGFLYING_RIGHT);
	else if(tempValue == "FORM_SWIMMINGFLYING_COPTER")
		tempValue.Format("%d", FORM_SWIMMINGFLYING_COPTER);
	else if(tempValue == "FORM_OBJECT_STOP")
		tempValue.Format("%d", FORM_OBJECT_STOP);
	else if(tempValue == "FORM_OBJECT_PLANE_ROTATE")
		tempValue.Format("%d", FORM_OBJECT_PLANE_ROTATE);
	else if(tempValue == "FORM_OBJECT_PLANE_ROTATE")
		tempValue.Format("%d", FORM_OBJECT_PLANE_ROTATE);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_ctrlComboAttackPattern.GetWindowText(tempValue);
	if(tempValue == "ATTPAT_NORMAL")
		tempValue.Format("%d", ATTPAT_NORMAL);
	else if(tempValue == "ATTPAT_SELFDESTRUCTION")
		tempValue.Format("%d", ATTPAT_SELFDESTRUCTION);
	else if(tempValue == "ATTPAT_RETREAT")
		tempValue.Format("%d", ATTPAT_RETREAT);
	else if(tempValue == "ATTPAT_HITANDRUN")
		tempValue.Format("%d", ATTPAT_HITANDRUN);
	else if(tempValue == "ATTPAT_ONEATATTCK")
		tempValue.Format("%d", ATTPAT_ONEATATTCK);
	else if(tempValue == "ATTPAT_PK")
		tempValue.Format("%d", ATTPAT_PK);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_ctrlComboMovePattern.GetWindowText(tempValue);
	if(tempValue == "MOVPAT_STOP")
		tempValue.Format("%d", MOVPAT_STOP);
	else if(tempValue == "MOVPAT_STRAIGHT")
		tempValue.Format("%d", MOVPAT_STRAIGHT);
	else if(tempValue == "MOVPAT_RIGHTANDLEFT")
		tempValue.Format("%d", MOVPAT_RIGHTANDLEFT);
	else if(tempValue == "MOVPAT_8RIGHTANDLEFT")
		tempValue.Format("%d", MOVPAT_8RIGHTANDLEFT);
	else if(tempValue == "MOVPAT_PATROL")
		tempValue.Format("%d", MOVPAT_PATROL);
	else if(tempValue == "MOVPAT_PARTY")
		tempValue.Format("%d", MOVPAT_PARTY);
	else if(tempValue == "MOVPAT_UPANDDOWN")
		tempValue.Format("%d", MOVPAT_UPANDDOWN);
	else if(tempValue == "MOVPAT_OBLIQUE")
		tempValue.Format("%d", MOVPAT_OBLIQUE);
	else if(tempValue == "MOVPAT_OBLIQUEANDCURVE")
		tempValue.Format("%d", MOVPAT_OBLIQUEANDCURVE);
	else if(tempValue == "MOVPAT_TELEPORT")
		tempValue.Format("%d", MOVPAT_TELEPORT);
	else
		tempValue = "0";
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_ctrlComboBelligerence.GetWindowText(tempValue);
	if(tempValue == "BELL_ATATTACK")
		tempValue.Format("%d", BELL_ATATTACK);
	else if(tempValue == "BELL_DEFENSE")
		tempValue.Format("%d", BELL_DEFENSE);
	else if(tempValue == "BELL_RETREAT")
		tempValue.Format("%d", BELL_RETREAT);
	else if(tempValue == "BELL_TAGETATATTACK")
		tempValue.Format("%d", BELL_TAGETATATTACK);
	else if(tempValue == "BELL_NOTATTACK")
		tempValue.Format("%d", BELL_NOTATTACK);
	else if(tempValue == "BELL_INFLUENCE_VCM")
		tempValue.Format("%d", BELL_INFLUENCE_VCN);
	else if(tempValue == "BELL_INFLUENCE_ANI")
		tempValue.Format("%d", BELL_INFLUENCE_ANI);

	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDefense);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDefenseProbability);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAlphaBlending);
	
	m_ctrlComboAttackObject.GetWindowText(tempValue);
	if(tempValue == "ATTACKOBJ_CLOSERANGE")
		tempValue.Format("%d", ATTACKOBJ_CLOSERANGE);
	else if(tempValue == "ATTACKOBJ_FIRSTATTACK")
		tempValue.Format("%d", ATTACKOBJ_FIRSTATTACK);
	else if(tempValue == "ATTACKOBJ_LOWHP")
		tempValue.Format("%d", ATTACKOBJ_LOWHP);
	else if(tempValue == "ATTACKOBJ_HIGHHP")
		tempValue.Format("%d", ATTACKOBJ_HIGHHP);
	else if(tempValue == "ATTACKOBJ_SAMERACE")
		tempValue.Format("%d", ATTACKOBJ_SAMERACE);
	else if(tempValue == "ATTACKOBJ_RANDOM")
		tempValue.Format("%d", ATTACKOBJ_RANDOM);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAttackRange);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditHPRecoveryTime);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditHPRecoveryValue);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditScaleValue);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditTextureIndex);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSourceIndex);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditQuickTurnAngle);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditQuickTurnSpeed);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditQuickTurnTerm);

	//MPOption
	if(m_valueCheckDeadFall == TRUE)
		n_tempValue += MPOPTION_BIT_DEAD_FALL;
	if(m_valueCheckMoveParty == TRUE)
		n_tempValue += MPOPTION_BIT_MOVE_PARTY;
	if(m_valueCheckPatternMonster == TRUE)
		n_tempValue += MPOPTION_BIT_PATTERN_MONSTER;
	if(m_valueCheckBossMonster == TRUE)
		n_tempValue += MPOPTION_BIT_BOSS_MONSTER;
	if(m_valueCheckAutoDestroy == TRUE)
		n_tempValue += MPOPTION_BIT_AUTO_DESTROY;

	tempValue.Format("%d", n_tempValue);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMPOptionParam1);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMPOptionParam2);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditRange);
	m_pMonsterDlg->m_GridUpdate.SetItemText(m_pMonsterDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditTurnAngle);

	
	m_pMonsterDlg->m_GridUpdate.Refresh();

	EndDialog(0);
}

bool CMonsterDetailDlg::ValidateCheck()
{
	//제약 사항 체크
	UpdateData();
	if(atoi((char*)(LPCTSTR)m_valueEditUniqueNumber) <2000000 || atoi((char*)(LPCTSTR)m_valueEditUniqueNumber) > 2999999)
	{
		AfxMessageBox("Please Check Unique Number Value");
		return FALSE;
	}
	if(m_valueEditMonsterName == "")
	{
		AfxMessageBox("Please Check Monster Name Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditLevel) < 1 || atoi((char*)(LPCTSTR)m_valueEditLevel) > 99)
	{
		AfxMessageBox("Please Check Level Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditExperience) < 0)
	{
		AfxMessageBox("Please Check Experience Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditHP) < 1)
	{
		AfxMessageBox("Please Check HP Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditSpeed) < 1 || atoi((char*)(LPCTSTR)m_valueEditSpeed) > 255)
	{
		AfxMessageBox("Please Check Speed Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditSizeForServer) < 1)
	{
		AfxMessageBox("Please Check Size For Server Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditSizeForClient) < 1)
	{
		AfxMessageBox("Please Check Size For Client Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditDefense) < 0 || atoi((char*)(LPCTSTR)m_valueEditDefense) > 255)
	{
		AfxMessageBox("Please Check Defense Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditDefenseProbability) < 0 || atoi((char*)(LPCTSTR)m_valueEditDefenseProbability) > 255)
	{
		AfxMessageBox("Please Check Defense Probability Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAlphaBlending) < 0 || atoi((char*)(LPCTSTR)m_valueEditAlphaBlending) > 1)
	{
		AfxMessageBox("Please Check Alpha Blending Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAttackRange) < 0)
	{
		AfxMessageBox("Please Check Attack Range Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditHPRecoveryTime) <0 || atoi((char*)(LPCTSTR)m_valueEditHPRecoveryTime) > 30000)
	{
		AfxMessageBox("Please Check HP Recovery Time Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditHPRecoveryValue) < 0 || atoi((char*)(LPCTSTR)m_valueEditHPRecoveryValue) > 30000)
	{
		AfxMessageBox("Please Check HP Recovery Value Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditScaleValue) < 0.1 || atoi((char*)(LPCTSTR)m_valueEditScaleValue) > 100.0)
	{
		AfxMessageBox("Please Check Scale Value Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditTextureIndex) < 0 || atoi((char*)(LPCTSTR)m_valueEditTextureIndex) > 255)
	{
		AfxMessageBox("Please Check Texture Index Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditSourceIndex) < 2000000 || atoi((char*)(LPCTSTR)m_valueEditSourceIndex) > 2999999)
	{
		AfxMessageBox("Please Check Source Index Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditQuickTurnAngle) < 10 || atoi((char*)(LPCTSTR)m_valueEditQuickTurnAngle) > 180)
	{
		AfxMessageBox("Please Check Quick Turn Angle Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditQuickTurnSpeed) < 0 || atoi((char*)(LPCTSTR)m_valueEditQuickTurnSpeed) > 255)
	{
		AfxMessageBox("Please Check Quick Turn Speed Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditQuickTurnTerm) < 0 || atoi((char*)(LPCTSTR)m_valueEditQuickTurnTerm) > 30000)
	{
		AfxMessageBox("Please Check Quick Turn Term Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMPOptionParam1) < 0 || atoi((char*)(LPCTSTR)m_valueEditMPOptionParam1)  > 255)
	{
		AfxMessageBox("Please Check MPOption Param 1 Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMPOptionParam2)  < 0 || atoi((char*)(LPCTSTR)m_valueEditMPOptionParam2) > 255)
	{
		AfxMessageBox("Please Check MPOption Param 2 Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditRange) <0 || atoi((char*)(LPCTSTR)m_valueEditRange) > 30000)
	{
		AfxMessageBox("Please Check Range Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditTurnAngle) < 0 || atoi((char*)(LPCTSTR)m_valueEditTurnAngle) > 3.14)
	{
		AfxMessageBox("Please Check Turn Angle Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditTurnAngle) < 0 )
	{
		AfxMessageBox("Please Check Camera Pattern Value");
		return FALSE;
	}

	//콤보박스가 선택된것이 없으면 에러처리
	CString szCheck;
	m_ctrlComboMonsterItem01.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check MonsterItem01 Value");
		return FALSE;
	}
	
	m_ctrlComboAttackObject.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check AttackObject Value");
		return FALSE;
	}

	m_ctrlComboBelligerence.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Belligernce Value");
		return FALSE;
	}

	m_ctrlComboMonsterForm.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check MonsterForm Value");
		return FALSE;
	}

	m_ctrlComboAttackPattern.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check AttackPattern Value");
		return FALSE;
	}

	// 2005-10-28 by cmkwon
	if(m_valueCheckMoveParty
		&& m_valueCheckAutoDestroy)
	{
		AfxMessageBox("Cannot use both together. Moveparty and Autodestroy. Please Check MPOption !!");
		return FALSE;
	}

	// 2005-10-28 by cmkwon
	if(m_valueCheckMoveParty)
	{
		int nNum = atoi(m_valueEditMPOptionParam2);
		if(nNum < 1)
		{
			AfxMessageBox("MPOptionParam2 will need to be charged by more than 1. Please Check MoveParty from MPOption");
			return FALSE;
		}
	}

	if(m_valueCheckAutoDestroy)
	{
		int nNum = atoi(m_valueEditMPOptionParam1);
		if(nNum < 1)
		{
			AfxMessageBox("MPOptionParam1 will need to be charged by more than 1. Please Check AutoDestroy from MPOption");
			return FALSE;
		}
	}
	return TRUE;
}

void CMonsterDetailDlg::OnButtonAttackitem() 
{
	// TODO: Add your control notification handler code here
	// 몬스터 공격 아이템 연결
	if (m_pMainDlg->m_pMonsterAttackDlg != NULL)
	{
		util::del(m_pMainDlg->m_pMonsterAttackDlg);
	}

	if (m_pMainDlg->m_pMonsterAttackDlg == NULL)
	{
		int tempMonsterItem[ARRAY_SIZE_MONSTER_ITEM]={0,};
		CString tempszMonsterItem;
		m_ctrlComboMonsterItem01.GetWindowText(tempszMonsterItem);
		tempMonsterItem[0]= atoi(tempszMonsterItem);
		m_ctrlComboMonsterItem02.GetWindowText(tempszMonsterItem);
		tempMonsterItem[1]= atoi(tempszMonsterItem);
		m_ctrlComboMonsterItem03.GetWindowText(tempszMonsterItem);
		tempMonsterItem[2]= atoi(tempszMonsterItem);
		m_ctrlComboMonsterItem04.GetWindowText(tempszMonsterItem);
		tempMonsterItem[3]= atoi(tempszMonsterItem);
		m_ctrlComboMonsterItem05.GetWindowText(tempszMonsterItem);
		tempMonsterItem[4]= atoi(tempszMonsterItem);
		m_pMainDlg->m_pMonsterAttackDlg = new CMonsterAttackDlg(MONSTER_ITEMLIST_TYPE_ANYMONSTER, tempMonsterItem, m_pMainDlg);
		m_pMainDlg->m_pMonsterAttackDlg->Create(IDD_DIALOG_MONSTERATTACK, m_pMainDlg);
	}

	m_pMainDlg->m_pMonsterAttackDlg->ShowWindow(SW_SHOW);
}

void CMonsterDetailDlg::OnButtonDropitem() 
{
	// TODO: Add your control notification handler code here
	// 몬스터 드롭 아이템 연결
	if (m_pMainDlg->m_pMonsterDropDlg != NULL)
	{
		util::del(m_pMainDlg->m_pMonsterDropDlg);
	}

	if (m_pMainDlg->m_pMonsterDropDlg == NULL)
	{
		m_pMainDlg->m_pMonsterDropDlg = new CMonsterDropDlg(m_pMainDlg);
		m_pMainDlg->m_pMonsterDropDlg->Create(IDD_DIALOG_MONSTERDROP, m_pMainDlg);
		m_pMainDlg->m_pMonsterDropDlg->GetCurrentList(1, atoi(m_valueEditUniqueNumber));
	}

	m_pMainDlg->m_pMonsterDropDlg->ShowWindow(SW_SHOW);
}

void CMonsterDetailDlg::OnButtonMonsteritem01() 
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	// TODO: Add your control notification handler code here
// 	//ItemNumber가지고 DB에서 select한다.
// 	CString tempMonsterItem;
// 	m_ctrlComboMonsterItem01.GetWindowText(tempMonsterItem);
// 	if(tempMonsterItem== "0")
// 		return;
// 	CString szSQLQuery;
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, CameraPattern, Kind, sourceindex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from ti_item where itemnum like '75%%' and itemnum in (%s)",
// 	//	tempMonsterItem);
// 	szSQLQuery.Format(QUERY_080702_0106, tempMonsterItem);
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
// 	TRACE(szSQLQuery);
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
// 	if (m_pMonsterAttackDetailDlg != NULL)
// 	{
// 		util::del(m_pMonsterAttackDetailDlg);
// 	}
// 
// 	if (m_pMonsterAttackDetailDlg == NULL)
// 	{
// 		m_pMonsterAttackDetailDlg = new CMonsterAttackDetailDlg(this);
// 		m_pMonsterAttackDetailDlg->Create(IDD_DIALOG_MONSTERATTACKDETAIL, this);
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE); //버튼 비활성화
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
// 	}
// 
// 	SetDetailMonsterItem(&st_Item); //자식 다이얼로그 컨트롤에 값 셋팅
// 	m_pMonsterAttackDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDetailDlg::OnButtonMonsteritem2() 
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	// TODO: Add your control notification handler code here
// 	//ItemNumber가지고 DB에서 select한다.
// 	CString tempMonsterItem;
// 	m_ctrlComboMonsterItem02.GetWindowText(tempMonsterItem);
// 	if(tempMonsterItem== "0")
// 		return;
// 	CString szSQLQuery;
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, sourceindex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from ti_item where itemnum like '75%%' and itemnum in (%s)",
// 	//	tempMonsterItem);
// 	szSQLQuery.Format(QUERY_080702_0107, tempMonsterItem);
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
// 	if (m_pMonsterAttackDetailDlg != NULL)
// 	{
// 		util::del(m_pMonsterAttackDetailDlg);
// 	}
// 
// 	if (m_pMonsterAttackDetailDlg == NULL)
// 	{
// 		m_pMonsterAttackDetailDlg = new CMonsterAttackDetailDlg(this);
// 		m_pMonsterAttackDetailDlg->Create(IDD_DIALOG_MONSTERATTACKDETAIL, this);
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE); //버튼 비활성화
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
// 	}
// 
// 	SetDetailMonsterItem(&st_Item); //자식 다이얼로그 컨트롤에 값 셋팅
// 	m_pMonsterAttackDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDetailDlg::OnButtonMonsteritem3() 
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	// TODO: Add your control notification handler code here
// 	//ItemNumber가지고 DB에서 select한다.
// 	CString tempMonsterItem;
// 	m_ctrlComboMonsterItem03.GetWindowText(tempMonsterItem);
// 	if(tempMonsterItem== "0")
// 		return;
// 	CString szSQLQuery;
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, sourceindex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from ti_item where itemnum like '75%%' and itemnum in (%s)",
// 	//	tempMonsterItem);
// 	szSQLQuery.Format(QUERY_080702_0108, tempMonsterItem);
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
// 	if (m_pMonsterAttackDetailDlg != NULL)
// 	{
// 		util::del(m_pMonsterAttackDetailDlg);
// 	}
// 
// 	if (m_pMonsterAttackDetailDlg == NULL)
// 	{
// 		m_pMonsterAttackDetailDlg = new CMonsterAttackDetailDlg(this);
// 		m_pMonsterAttackDetailDlg->Create(IDD_DIALOG_MONSTERATTACKDETAIL, this);
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE); //버튼 비활성화
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
// 	}
// 
// 	SetDetailMonsterItem(&st_Item); //자식 다이얼로그 컨트롤에 값 셋팅
// 	m_pMonsterAttackDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDetailDlg::OnButtonMonsteritem4() 
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	// TODO: Add your control notification handler code here
// 	//ItemNumber가지고 DB에서 select한다.
// 	CString tempMonsterItem;
// 	m_ctrlComboMonsterItem04.GetWindowText(tempMonsterItem);
// 	if(tempMonsterItem== "0")
// 		return;
// 	CString szSQLQuery;
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, sourceindex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from ti_item where itemnum like '75%%' and itemnum in (%s)",
// 	//	tempMonsterItem);
// 	szSQLQuery.Format(QUERY_080702_0109, tempMonsterItem);
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
// 	if (m_pMonsterAttackDetailDlg != NULL)
// 	{
// 		util::del(m_pMonsterAttackDetailDlg);
// 	}
// 
// 	if (m_pMonsterAttackDetailDlg == NULL)
// 	{
// 		m_pMonsterAttackDetailDlg = new CMonsterAttackDetailDlg(this);
// 		m_pMonsterAttackDetailDlg->Create(IDD_DIALOG_MONSTERATTACKDETAIL, this);
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE); //버튼 비활성화
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
// 	}
// 
// 	SetDetailMonsterItem(&st_Item); //자식 다이얼로그 컨트롤에 값 셋팅
// 	m_pMonsterAttackDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDetailDlg::OnButtonMonsteritem5() 
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	// TODO: Add your control notification handler code here
// 	//ItemNumber가지고 DB에서 select한다.
// 	CString tempMonsterItem;
// 	m_ctrlComboMonsterItem05.GetWindowText(tempMonsterItem);
// 	if(tempMonsterItem== "0")
// 		return;
// 	CString szSQLQuery;
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, sourceindex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from ti_item where itemnum like '75%%' and itemnum in (%s)",
// 	//	tempMonsterItem);
// 	szSQLQuery.Format(QUERY_080702_0110, tempMonsterItem);
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
// 	if (m_pMonsterAttackDetailDlg != NULL)
// 	{
// 		util::del(m_pMonsterAttackDetailDlg);
// 	}
// 
// 	if (m_pMonsterAttackDetailDlg == NULL)
// 	{
// 		m_pMonsterAttackDetailDlg = new CMonsterAttackDetailDlg(this);
// 		m_pMonsterAttackDetailDlg->Create(IDD_DIALOG_MONSTERATTACKDETAIL, this);
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE); //버튼 비활성화
// 		m_pMonsterAttackDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
// 	}
// 
// 	SetDetailMonsterItem(&st_Item); //자식 다이얼로그 컨트롤에 값 셋팅
// 	m_pMonsterAttackDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDetailDlg::SetDetailMonsterItem(_TI_ITEM *i_pTi_Item)
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	//선택된 셀을 바탕으로 값을 설정한다.
// 	m_pMonsterAttackDetailDlg->m_valueEditItemNum.Format("%d", i_pTi_Item->ItemNum);
// 	m_pMonsterAttackDetailDlg->m_valueEditItemName.Format("%s", i_pTi_Item->ItemName);
// 	m_pMonsterAttackDetailDlg->m_valueEditAbilityMin.Format("%f", i_pTi_Item->AbilityMin);
// 	m_pMonsterAttackDetailDlg->m_valueEditAbilityMax.Format("%f", i_pTi_Item->AbilityMax);
// 	m_pMonsterAttackDetailDlg->m_ctrlComboRace.SetWindowText(CAtumSJ::GetRaceString(i_pTi_Item->ReqRace));
// 	m_pMonsterAttackDetailDlg->m_valueEditReqMinLevel.Format("%d", i_pTi_Item->ReqMinLevel);
// 	m_pMonsterAttackDetailDlg->m_valueEditHitRate.Format("%d", i_pTi_Item->HitRate);
// 	m_pMonsterAttackDetailDlg->m_valueEditSpeedPenalty.Format("%d", i_pTi_Item->SpeedPenalty);
// 	m_pMonsterAttackDetailDlg->m_valueEditRange.Format("%d", i_pTi_Item->Range);
// 	m_pMonsterAttackDetailDlg->m_valueEditCharging.Format("%d", i_pTi_Item->Charging);
// 	m_pMonsterAttackDetailDlg->m_valueEditRangeAngle.Format("%f", i_pTi_Item->RangeAngle);
// 	m_pMonsterAttackDetailDlg->m_valueEditShotNum.Format("%d", i_pTi_Item->ShotNum);
// 	m_pMonsterAttackDetailDlg->m_valueEditMultiNum.Format("%d", i_pTi_Item->AttackTime);
// 	m_pMonsterAttackDetailDlg->m_valueEditAttackTime.Format("%d", i_pTi_Item->AttackTime);
// 	m_pMonsterAttackDetailDlg->m_valueEditReqSP.Format("%d", i_pTi_Item->ReqSP);
// 	m_pMonsterAttackDetailDlg->m_valueEditSummonMonster.Format("%d", i_pTi_Item->SummonMonster);
// 
// 	int tempValue;
// 	tempValue = i_pTi_Item->OrbitType;
// 	if(tempValue == ORBIT_SINGLE_CROSS_FIRE)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_CROSS_FIRE");
// 	else if(tempValue == ORBIT_SINGLE_FIRE_100)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_100");
// 	else if(tempValue == ORBIT_SINGLE_FIRE_150)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_150");
// 	else if(tempValue == ORBIT_SINGLE_FIRE_200)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_200");
// 	else if(tempValue == ORBIT_SINGLE_FIRE_250)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_250");
// 	else if(tempValue == ORBIT_SINGLE_FIRE_300)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_300");
// 	else if(tempValue == ORBIT_SINGLE_FIRE_320)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_320");
// 	else if(tempValue == ORBIT_SINGLE_FIRE_350)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_350");
// 	else if(tempValue == ORBIT_TWIN_CROSS_FIRE)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_CROSS_FIRE");
// 	else if(tempValue == ORBIT_TWIN_FIRE_100)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_100");
// 	else if(tempValue == ORBIT_TWIN_FIRE_150)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_150");
// 	else if(tempValue == ORBIT_TWIN_FIRE_200)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_200");
// 	else if(tempValue == ORBIT_TWIN_FIRE_250)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_250");
// 	else if(tempValue == ORBIT_TWIN_FIRE_300)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_300");
// 	else if(tempValue == ORBIT_TWIN_FIRE_320)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_320");
// 	else if(tempValue == ORBIT_TWIN_FIRE_350)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_350");
// 	else if(tempValue == ORBIT_CROSS_FIRE)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_CROSS_FIRE");
// 	else if(tempValue == ORBIT_STRAIGHT_ROCKET_250)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_STRAIGHT_ROCKET_250");
// 	else if(tempValue == ORBIT_FALL_ROCKET_250)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_FALL_ROCKET_250");
// 	else if(tempValue == ORBIT_LEFT_ROCKET_200)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_LEFT_ROCKET_200");
// 	else if(tempValue == ORBIT_RIGHT_ROCKET_200)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_RIGHT_ROCKET_200");
// 	else if(tempValue == ORBIT_THREE_ROCKET)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_THREE_ROCKET");
// 	else if(tempValue == ORBIT_CHILD_ROCKET)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_CHILD_ROCKET");
// 	else if(tempValue == ORBIT_STRAIGHT_MISSILE_300)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_STRAIGHT_MISSILE_300");
// 	else if(tempValue == ORBIT_FALL_MISSILE_300)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_FALL_MISSILE_300");
// 	else if(tempValue == ORBIT_LEFT_MISSILE_300)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_LEFT_MISSILE_300");
// 	else if(tempValue == ORBIT_RIGHT_MISSILE_270)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_RIGHT_MISSILE_270");
// 	else if(tempValue == ORBIT_UPDOWN_ROCKET_270)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_UPDOWN_ROCKET_270");
// 	else if(tempValue == ORBIT_UPDOWN_MISSILE_300)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_UPDOWN_MISSILE_300");
// 	else if(tempValue == ORBIT_FLAME_THROWER)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_FLAME_THROWER");
// 	else if(tempValue == ORBIT_BODYSLAM)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_BODYSLAM");
// 	else if(tempValue == ORBIT_MINE_300)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_MINE_300");
// 	else if(tempValue == ORBIT_MINE_400)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_MINE_400");
// 	else if(tempValue == ORBIT_RANGE_ROCKET_300)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_RANGE_ROCKET_300");
// 	else if(tempValue == ORBIT_RANGE_ROCKET_400)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_RANGE_ROCKET_400");
// 	else if(tempValue == ORBIT_FIRE)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_FIRE");
// 	else if(tempValue == ORBIT_OBJBEAM)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_OBJBEAM");
// 	else if(tempValue == ORBIT_STRAIGHTBOOM)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_STRAIGHTBOOM");
// 	else if(tempValue == ORBIT_CATCH)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_CATCH");
// 	else if(tempValue == ORBIT_STICK)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_STICK");
// 	else if(tempValue == ORBIT_ATTACK_ALL)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_ATTACK_ALL");	
// 
// 	tempValue = i_pTi_Item->Kind;
// 	if(tempValue == ITEMKIND_FOR_MON_PRIMARY)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_PRIMARY);
// 	else if(tempValue == ITEMKIND_FOR_MON_GUN)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_GUN);
// 	else if(tempValue == ITEMKIND_FOR_MON_BEAM)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_BEAM);
// 	else if(tempValue == ITEMKIND_FOR_MON_ALLATTACK)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_ALLATTACK);
// 	else if(tempValue == ITEMKIND_FOR_MON_SECONDARY)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_SECONDARY);
// 	else if(tempValue == ITEMKIND_FOR_MON_ROCKET)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_ROCKET);
// 	else if(tempValue == ITEMKIND_FOR_MON_MISSILE)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_MISSILE);
// 	else if(tempValue == ITEMKIND_FOR_MON_BUNDLE)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_BUNDLE);
// 	else if(tempValue == ITEMKIND_FOR_MON_MINE)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_MINE);
// 	else if(tempValue == ITEMKIND_FOR_MON_SHIELD)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_SHIELD);
// 	else if(tempValue == ITEMKIND_FOR_MON_DUMMY)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_DUMMY);
// 	else if(tempValue == ITEMKIND_FOR_MON_FIXER)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_FIXER);
// 	else if(tempValue == ITEMKIND_FOR_MON_DECOY)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_DECOY);
// 	else if(tempValue == ITEMKIND_FOR_MON_FIRE)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_FIRE);
// 	else if(tempValue == ITEMKIND_FOR_MON_OBJBEAM)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_OBJBEAM);
// 	else if(tempValue == ITEMKIND_FOR_MON_STRAIGHTBOOM)
// 		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_STRAIGHTBOOM);
// 
// 	m_pMonsterAttackDetailDlg->m_valueEditSummonMonster.Format("%d",i_pTi_Item->CameraPattern);
// 	m_pMonsterAttackDetailDlg->m_valueEditSourceIndex.Format("%d",i_pTi_Item->SourceIndex);
// 	
// 	m_pMonsterAttackDetailDlg->m_valueEditDestParameter1.Format("%d",i_pTi_Item->DestParameter1);
// 	m_pMonsterAttackDetailDlg->m_valueEditParameterValue1.Format("%d",i_pTi_Item->ParameterValue1);
// 	m_pMonsterAttackDetailDlg->m_valueEditDestParameter2.Format("%d",i_pTi_Item->DestParameter2);
// 	m_pMonsterAttackDetailDlg->m_valueEditParameterValue1.Format("%d",i_pTi_Item->ParameterValue2);
// 	m_pMonsterAttackDetailDlg->m_valueEditDestParameter3.Format("%d",i_pTi_Item->DestParameter3);
// 	m_pMonsterAttackDetailDlg->m_valueEditParameterValue1.Format("%d",i_pTi_Item->ParameterValue3);
// 	m_pMonsterAttackDetailDlg->m_valueEditDestParameter4.Format("%d",i_pTi_Item->DestParameter4);
// 	m_pMonsterAttackDetailDlg->m_valueEditParameterValue1.Format("%d",i_pTi_Item->ParameterValue4);
// 
// 	m_pMonsterAttackDetailDlg->UpdateData(FALSE);
}


