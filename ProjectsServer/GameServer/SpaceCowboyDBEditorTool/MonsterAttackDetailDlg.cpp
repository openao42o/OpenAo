// MonsterAttackDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "MonsterAttackDetailDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttackDetailDlg dialog


CMonsterAttackDetailDlg::CMonsterAttackDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterAttackDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterAttackDetailDlg)
	m_valueEditAbilityMin = _T("0");
	m_valueEditAbilityMax = _T("0");
	m_valueEditReqMinLevel = _T("1");
	m_valueEditHitRate = _T("1");
	m_valueEditSpeedPenalty = _T("100");
	m_valueEditRange = _T("1");
	m_valueEditCharging = _T("0");
	m_valueEditRangeAngle = _T("0.0");
	m_valueEditShotNum = _T("1");
	m_valueEditMultiNum = _T("1");
	m_valueEditAttackTime = _T("0");
	m_valueEditReqSP = _T("0");
	m_valueEditItemName = _T("0");
	m_valueEditSummonMonster = _T("0");
	m_valueEditItemNum = _T("");
	m_valueEditCameraPattern = _T("0");
	m_valueEditSourceIndex = _T("");
	m_valueEditDestParameter1 = _T("0");
	m_valueEditDestParameter2 = _T("0");
	m_valueEditDestParameter3 = _T("0");
	m_valueEditDestParameter4 = _T("0");
	m_valueEditParameterValue1 = _T("0");
	m_valueEditParameterValue2 = _T("0");
	m_valueEditParameterValue3 = _T("0");
	m_valueEditParameterValue4 = _T("0");
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd(); //메인 다이얼로그 클래스 포인터 얻기
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pMonsterAttackDlg = (CMonsterAttackDlg*)pParent; // 부모 클래스 포인터 저장
}


CMonsterAttackDetailDlg::~CMonsterAttackDetailDlg()
{
// 2006-01-23 by cmkwon
// 	util::del(m_pODBCStmt);
}



void CMonsterAttackDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterAttackDetailDlg)
	DDX_Control(pDX, IDC_COMBO3, m_ctrlComboKind);
	DDX_Control(pDX, IDC_COMBO2, m_ctrlComboOrbitType);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlComboRace);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ctrlButtonSave);
	DDX_Control(pDX, IDC_BUTTON_MAKE, m_ctrlButtonMake);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_ctrlButtonCancel);
	DDX_Text(pDX, IDC_EDIT2, m_valueEditAbilityMin);
	DDX_Text(pDX, IDC_EDIT3, m_valueEditAbilityMax);
	DDX_Text(pDX, IDC_EDIT4, m_valueEditReqMinLevel);
	DDX_Text(pDX, IDC_EDIT5, m_valueEditHitRate);
	DDX_Text(pDX, IDC_EDIT6, m_valueEditSpeedPenalty);
	DDX_Text(pDX, IDC_EDIT7, m_valueEditRange);
	DDX_Text(pDX, IDC_EDIT18, m_valueEditCharging);
	DDX_Text(pDX, IDC_EDIT30, m_valueEditRangeAngle);
	DDX_Text(pDX, IDC_EDIT31, m_valueEditShotNum);
	DDX_Text(pDX, IDC_EDIT32, m_valueEditMultiNum);
	DDX_Text(pDX, IDC_EDIT33, m_valueEditAttackTime);
	DDX_Text(pDX, IDC_EDIT34, m_valueEditReqSP);
	DDX_Text(pDX, IDC_EDIT1, m_valueEditItemName);
	DDX_Text(pDX, IDC_EDIT35, m_valueEditSummonMonster);
	DDX_Text(pDX, IDC_EDIT36, m_valueEditItemNum);
	DDX_Text(pDX, IDC_EDIT37, m_valueEditCameraPattern);
	DDX_Text(pDX, IDC_EDIT38, m_valueEditSourceIndex);
	DDX_Text(pDX, IDC_EDIT83, m_valueEditDestParameter1);
	DDX_Text(pDX, IDC_EDIT39, m_valueEditDestParameter2);
	DDX_Text(pDX, IDC_EDIT11, m_valueEditDestParameter3);
	DDX_Text(pDX, IDC_EDIT12, m_valueEditDestParameter4);
	DDX_Text(pDX, IDC_EDIT13, m_valueEditParameterValue1);
	DDX_Text(pDX, IDC_EDIT80, m_valueEditParameterValue2);
	DDX_Text(pDX, IDC_EDIT81, m_valueEditParameterValue3);
	DDX_Text(pDX, IDC_EDIT82, m_valueEditParameterValue4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterAttackDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterAttackDetailDlg)
	ON_BN_CLICKED(IDC_BUTTON_MAKE, OnButtonMake)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttackDetailDlg message handlers

BOOL CMonsterAttackDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
// 2006-01-23 by cmkwon
//	//DB 연결 초기화
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		EndDialog(-1);
//		return FALSE;
//	}
	//ComboBox에 데이터 입력
	m_ctrlComboRace.InsertString(0,STRCMD_CS_COMMON_RACE_BATTALUS);
	m_ctrlComboRace.InsertString(1,STRCMD_CS_COMMON_RACE_DECA);
	m_ctrlComboRace.InsertString(2,STRCMD_CS_COMMON_RACE_PHILON);
	m_ctrlComboRace.InsertString(3,STRCMD_CS_COMMON_RACE_SHARRINE);

	m_ctrlComboOrbitType.InsertString(0,"ORBIT_SINGLE_CROSS_FIRE");
	m_ctrlComboOrbitType.InsertString(1,"ORBIT_SINGLE_FIRE_100");
	m_ctrlComboOrbitType.InsertString(2,"ORBIT_SINGLE_FIRE_150");
	m_ctrlComboOrbitType.InsertString(3,"ORBIT_SINGLE_FIRE_200");
	m_ctrlComboOrbitType.InsertString(4,"ORBIT_SINGLE_FIRE_250");
	m_ctrlComboOrbitType.InsertString(5,"ORBIT_SINGLE_FIRE_300");
	m_ctrlComboOrbitType.InsertString(6,"ORBIT_SINGLE_FIRE_320");
	m_ctrlComboOrbitType.InsertString(7,"ORBIT_SINGLE_FIRE_350");
	m_ctrlComboOrbitType.InsertString(8,"ORBIT_TWIN_CROSS_FIRE");
	m_ctrlComboOrbitType.InsertString(9,"ORBIT_TWIN_FIRE_100");
	m_ctrlComboOrbitType.InsertString(10,"ORBIT_TWIN_FIRE_150");
	m_ctrlComboOrbitType.InsertString(11,"ORBIT_TWIN_FIRE_200");
	m_ctrlComboOrbitType.InsertString(12,"ORBIT_TWIN_FIRE_250");
	m_ctrlComboOrbitType.InsertString(13,"ORBIT_TWIN_FIRE_300");
	m_ctrlComboOrbitType.InsertString(14,"ORBIT_TWIN_FIRE_320");
	m_ctrlComboOrbitType.InsertString(15,"ORBIT_TWIN_FIRE_350");
	m_ctrlComboOrbitType.InsertString(16,"ORBIT_CROSS_FIRE");
	m_ctrlComboOrbitType.InsertString(17,"ORBIT_STRAIGHT_ROCKET_250");
	m_ctrlComboOrbitType.InsertString(18,"ORBIT_FALL_ROCKET_250");
	m_ctrlComboOrbitType.InsertString(19,"ORBIT_LEFT_ROCKET_200");
	m_ctrlComboOrbitType.InsertString(20,"ORBIT_RIGHT_ROCKET_200");
	m_ctrlComboOrbitType.InsertString(21,"ORBIT_THREE_ROCKET");
	m_ctrlComboOrbitType.InsertString(22,"ORBIT_CHILD_ROCKET");
	m_ctrlComboOrbitType.InsertString(23,"ORBIT_STRAIGHT_MISSILE_300");
	m_ctrlComboOrbitType.InsertString(24,"ORBIT_FALL_MISSILE_300");
	m_ctrlComboOrbitType.InsertString(25,"ORBIT_LEFT_MISSILE_300");
	m_ctrlComboOrbitType.InsertString(26,"ORBIT_RIGHT_MISSILE_270");
	m_ctrlComboOrbitType.InsertString(27,"ORBIT_UPDOWN_ROCKET_270");
	m_ctrlComboOrbitType.InsertString(28,"ORBIT_UPDOWN_MISSILE_300");
	m_ctrlComboOrbitType.InsertString(29,"ORBIT_FLAME_THROWER");
	m_ctrlComboOrbitType.InsertString(30,"ORBIT_BODYSLAM");
	m_ctrlComboOrbitType.InsertString(31,"ORBIT_MINE_300");
	m_ctrlComboOrbitType.InsertString(32,"ORBIT_MINE_400");
	m_ctrlComboOrbitType.InsertString(33,"ORBIT_RANGE_ROCKET_300");
	m_ctrlComboOrbitType.InsertString(34,"ORBIT_RANGE_ROCKET_400");
	m_ctrlComboOrbitType.InsertString(35,"ORBIT_FIRE");
	m_ctrlComboOrbitType.InsertString(36,"ORBIT_OBJBEAM");
	m_ctrlComboOrbitType.InsertString(37,"ORBIT_STRAIGHTBOOM");
	m_ctrlComboOrbitType.InsertString(38,"ORBIT_CATCH");
	m_ctrlComboOrbitType.InsertString(39,"ORBIT_STICK");
	m_ctrlComboOrbitType.InsertString(40,"ORBIT_ATTACK_ALL");

	m_ctrlComboKind.InsertString(0,STRCMD_CS_ITEMKIND_FOR_MON_PRIMARY);
	m_ctrlComboKind.InsertString(1,STRCMD_CS_ITEMKIND_FOR_MON_GUN);
	m_ctrlComboKind.InsertString(2,STRCMD_CS_ITEMKIND_FOR_MON_BEAM);
	m_ctrlComboKind.InsertString(3,STRCMD_CS_ITEMKIND_FOR_MON_ALLATTACK);
	m_ctrlComboKind.InsertString(4,STRCMD_CS_ITEMKIND_FOR_MON_SECONDARY);
	m_ctrlComboKind.InsertString(5,STRCMD_CS_ITEMKIND_FOR_MON_ROCKET);
	m_ctrlComboKind.InsertString(6,STRCMD_CS_ITEMKIND_FOR_MON_MISSILE);
	m_ctrlComboKind.InsertString(7,STRCMD_CS_ITEMKIND_FOR_MON_BUNDLE);
	m_ctrlComboKind.InsertString(8,STRCMD_CS_ITEMKIND_FOR_MON_MINE);
	m_ctrlComboKind.InsertString(9,STRCMD_CS_ITEMKIND_FOR_MON_SHIELD);
	m_ctrlComboKind.InsertString(10,STRCMD_CS_ITEMKIND_FOR_MON_DUMMY);
	m_ctrlComboKind.InsertString(11,STRCMD_CS_ITEMKIND_FOR_MON_FIXER);
	m_ctrlComboKind.InsertString(12,STRCMD_CS_ITEMKIND_FOR_MON_DECOY);
	m_ctrlComboKind.InsertString(13,STRCMD_CS_ITEMKIND_FOR_MON_FIRE);
	m_ctrlComboKind.InsertString(14,STRCMD_CS_ITEMKIND_FOR_MON_OBJBEAM);
	m_ctrlComboKind.InsertString(15,STRCMD_CS_ITEMKIND_FOR_MON_STRAIGHTBOOM);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonsterAttackDetailDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	// 제약사항 체크
	if(ValidateCheck()== FALSE)
		return;
	//부모 다이이얼로그의 Current GridCtrl에 선택된 열을 삭제
	m_pMonsterAttackDlg->m_GridCurrent.DeleteRow(m_nSelectRow);
	ReflectionToGrid(2); //부모 다이얼로그의 Update GridCtrl에 저장한 값을 넣는다.
}

void CMonsterAttackDetailDlg::OnButtonMake() 
{
	// TODO: Add your control notification handler code here
	//제약사항 체크
	if(ValidateCheck()== FALSE)
		return;
	ReflectionToGrid(1);//부모 다이얼로그의 Update GridCtrl에 저장한 값을 넣는다.
}

void CMonsterAttackDetailDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

bool CMonsterAttackDetailDlg::ValidateCheck()
{
	//제약사항들 검사
	UpdateData();
	if(m_valueEditItemName == "")
	{
		AfxMessageBox("Please Check Item Name Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAbilityMin) < 0 || atoi((char*)(LPCTSTR)m_valueEditAbilityMin) > 9999)
	{
		AfxMessageBox("Please Check AbilityMin Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAbilityMax) < 0 || atoi((char*)(LPCTSTR)m_valueEditAbilityMax) > 9999)
	{
		AfxMessageBox("Please Check AbilityMax Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReqMinLevel) < 1 || atoi((char*)(LPCTSTR)m_valueEditReqMinLevel) > 100)
	{
		AfxMessageBox("Please Check ReqMinLevel Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditHitRate) < 1 || atoi((char*)(LPCTSTR)m_valueEditHitRate) > 255)
	{
		AfxMessageBox("Please Check HitRate Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditSpeedPenalty) < 0 || atoi((char*)(LPCTSTR)m_valueEditSpeedPenalty) > 100)
	{
		AfxMessageBox("Please Check SpeedPenalty Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditRange) < 0 || atoi((char*)(LPCTSTR)m_valueEditRange) > 2000)
	{
		AfxMessageBox("Please Check Range Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditCharging) < 0 || atoi((char*)(LPCTSTR)m_valueEditCharging) > 100)
	{
		AfxMessageBox("Please Check Charging Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditRangeAngle) < 0|| atoi((char*)(LPCTSTR)m_valueEditRangeAngle) > 3.14)
	{
		AfxMessageBox("Please Check RangeAngle Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditShotNum) < 0 || atoi((char*)(LPCTSTR)m_valueEditShotNum) > 9)
	{
		AfxMessageBox("Please Check ShotNum Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMultiNum) < 0 || atoi((char*)(LPCTSTR)m_valueEditMultiNum) > 50)
	{
		AfxMessageBox("Please Check MultiNum Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAttackTime) < 0 || atoi((char*)(LPCTSTR)m_valueEditAttackTime) > 30000)
	{
		AfxMessageBox("Please Check AttackTime Value");
		return FALSE;
	}
	if(atof((char*)(LPCTSTR)m_valueEditCameraPattern) < 0 || atoi((char*)(LPCTSTR)m_valueEditCameraPattern) > 3.14)
	{
		AfxMessageBox("Please Check CameraPattern Value");
		return FALSE;
	}
	if(atof((char*)(LPCTSTR)m_valueEditSourceIndex) < 0 )
	{
		AfxMessageBox("Please Check SourceIndex Value");
		return FALSE;
	}

	//콤보박스가 선택된것이 없으면 에러처리
	CString szCheck;
	m_ctrlComboOrbitType.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check OrbitType Value");
		return FALSE;
	}
	m_ctrlComboRace.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Race Value");
		return FALSE;
	}
	m_ctrlComboKind.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Kind Value");
		return FALSE;
	}
	return TRUE;
}

void CMonsterAttackDetailDlg::ReflectionToGrid(int type)
{
	// 칼럼 만들기
	m_pMonsterAttackDlg->m_GridUpdate.SetRowCount(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()+1);

	//각 콘트롤의 값들을 부모 다이얼로그의 Update Grid Ctrl에 추가한다.
	UpdateData();
	
	int m_nCols= 0;
	CString tempValue;
	if(type == 1) //INSERT 일때
		m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, "INSERT");
	else if(type == 2) //UPDATE 일때
		m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, "UPDATE");
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditItemNum);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditItemName);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAbilityMin);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAbilityMax);
	
	//콤보박스의 값을 반영 시작
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
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	// 끝

	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqMinLevel);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditHitRate);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSpeedPenalty);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditRange);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCharging);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditRangeAngle);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditShotNum);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMultiNum);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAttackTime);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqSP);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSummonMonster);

	m_ctrlComboOrbitType.GetWindowText(tempValue);
	if(tempValue == "ORBIT_SINGLE_CROSS_FIRE")
		tempValue.Format("%d", ORBIT_SINGLE_CROSS_FIRE);
	else if(tempValue == "ORBIT_SINGLE_FIRE_100")
		tempValue.Format("%d", ORBIT_SINGLE_FIRE_100);
	else if(tempValue == "ORBIT_SINGLE_FIRE_150")
		tempValue.Format("%d", ORBIT_SINGLE_FIRE_150);
	else if(tempValue == "ORBIT_SINGLE_FIRE_200")
		tempValue.Format("%d", ORBIT_SINGLE_FIRE_200);
	else if(tempValue == "ORBIT_SINGLE_FIRE_250")
		tempValue.Format("%d", ORBIT_SINGLE_FIRE_250);
	else if(tempValue == "ORBIT_SINGLE_FIRE_300")
		tempValue.Format("%d", ORBIT_SINGLE_FIRE_300);
	else if(tempValue == "ORBIT_SINGLE_FIRE_320")
		tempValue.Format("%d", ORBIT_SINGLE_FIRE_320);
	else if(tempValue == "ORBIT_SINGLE_FIRE_350")
		tempValue.Format("%d", ORBIT_SINGLE_FIRE_350);
	else if(tempValue == "ORBIT_TWIN_CROSS_FIRE")
		tempValue.Format("%d", ORBIT_TWIN_CROSS_FIRE);
	else if(tempValue == "ORBIT_TWIN_FIRE_100")
		tempValue.Format("%d", ORBIT_TWIN_FIRE_100);
	else if(tempValue == "ORBIT_TWIN_FIRE_150")
		tempValue.Format("%d", ORBIT_TWIN_FIRE_150);
	else if(tempValue == "ORBIT_TWIN_FIRE_200")
		tempValue.Format("%d", ORBIT_TWIN_FIRE_200);
	else if(tempValue == "ORBIT_TWIN_FIRE_250")
		tempValue.Format("%d", ORBIT_TWIN_FIRE_250);
	else if(tempValue == "ORBIT_TWIN_FIRE_300")
		tempValue.Format("%d", ORBIT_TWIN_FIRE_300);
	else if(tempValue == "ORBIT_TWIN_FIRE_320")
		tempValue.Format("%d", ORBIT_TWIN_FIRE_320);
	else if(tempValue == "ORBIT_TWIN_FIRE_350")
		tempValue.Format("%d", ORBIT_TWIN_FIRE_350);
	else if(tempValue == "ORBIT_CROSS_FIRE")
		tempValue.Format("%d", ORBIT_CROSS_FIRE);
	else if(tempValue == "ORBIT_STRAIGHT_ROCKET_250")
		tempValue.Format("%d", ORBIT_STRAIGHT_ROCKET_250);
	else if(tempValue == "ORBIT_FALL_ROCKET_250")
		tempValue.Format("%d", ORBIT_FALL_ROCKET_250);
	else if(tempValue == "ORBIT_LEFT_ROCKET_200")
		tempValue.Format("%d", ORBIT_LEFT_ROCKET_200);
	else if(tempValue == "ORBIT_RIGHT_ROCKET_200")
		tempValue.Format("%d", ORBIT_RIGHT_ROCKET_200);
	else if(tempValue == "ORBIT_THREE_ROCKET")
		tempValue.Format("%d", ORBIT_THREE_ROCKET);
	else if(tempValue == "ORBIT_CHILD_ROCKET")
		tempValue.Format("%d", ORBIT_CHILD_ROCKET);
	else if(tempValue == "ORBIT_STRAIGHT_MISSILE_300")
		tempValue.Format("%d", ORBIT_STRAIGHT_MISSILE_300);
	else if(tempValue == "ORBIT_FALL_MISSILE_300")
		tempValue.Format("%d", ORBIT_FALL_MISSILE_300);
	else if(tempValue == "ORBIT_LEFT_MISSILE_300")
		tempValue.Format("%d", ORBIT_LEFT_MISSILE_300);
	else if(tempValue == "ORBIT_RIGHT_MISSILE_270")
		tempValue.Format("%d", ORBIT_RIGHT_MISSILE_270);
	else if(tempValue == "ORBIT_UPDOWN_ROCKET_270")
		tempValue.Format("%d", ORBIT_UPDOWN_ROCKET_270);
	else if(tempValue == "ORBIT_UPDOWN_MISSILE_300")
		tempValue.Format("%d", ORBIT_UPDOWN_MISSILE_300);
	else if(tempValue == "ORBIT_FLAME_THROWER")
		tempValue.Format("%d", ORBIT_FLAME_THROWER);
	else if(tempValue == "ORBIT_BODYSLAM")
		tempValue.Format("%d", ORBIT_BODYSLAM);
	else if(tempValue == "ORBIT_MINE_300")
		tempValue.Format("%d", ORBIT_MINE_300);
	else if(tempValue == "ORBIT_MINE_400")
		tempValue.Format("%d", ORBIT_MINE_400);
	else if(tempValue == "ORBIT_RANGE_ROCKET_300")
		tempValue.Format("%d", ORBIT_RANGE_ROCKET_300);
	else if(tempValue == "ORBIT_RANGE_ROCKET_400")
		tempValue.Format("%d", ORBIT_RANGE_ROCKET_400);
	else if(tempValue == "ORBIT_FIRE")
		tempValue.Format("%d", ORBIT_FIRE);
	else if(tempValue == "ORBIT_OBJBEAM")
		tempValue.Format("%d", ORBIT_OBJBEAM);
	else if(tempValue == "ORBIT_STRAIGHTBOOM")
		tempValue.Format("%d", ORBIT_STRAIGHTBOOM);
	else if(tempValue == "ORBIT_CATCH")
		tempValue.Format("%d", ORBIT_CATCH);
	else if(tempValue == "ORBIT_STICK")
		tempValue.Format("%d", ORBIT_STICK);
	else if(tempValue == "ORBIT_ATTACK_ALL")
		tempValue.Format("%d", ORBIT_ATTACK_ALL);

	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_ctrlComboKind.GetWindowText(tempValue);
	if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_PRIMARY)
		tempValue.Format("%d", ITEMKIND_FOR_MON_PRIMARY);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_GUN)
		tempValue.Format("%d", ITEMKIND_FOR_MON_GUN);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_BEAM)
		tempValue.Format("%d", ITEMKIND_FOR_MON_BEAM);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_ALLATTACK)
		tempValue.Format("%d", ITEMKIND_FOR_MON_ALLATTACK);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_SECONDARY)
		tempValue.Format("%d", ITEMKIND_FOR_MON_SECONDARY);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_ROCKET)
		tempValue.Format("%d", ITEMKIND_FOR_MON_ROCKET);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_MISSILE)
		tempValue.Format("%d", ITEMKIND_FOR_MON_MISSILE);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_BUNDLE)
		tempValue.Format("%d", ITEMKIND_FOR_MON_BUNDLE);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_MINE)
		tempValue.Format("%d", ITEMKIND_FOR_MON_MINE);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_SHIELD)
		tempValue.Format("%d", ITEMKIND_FOR_MON_SHIELD);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_DUMMY)
		tempValue.Format("%d", ITEMKIND_FOR_MON_DUMMY);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_FIXER)
		tempValue.Format("%d", ITEMKIND_FOR_MON_FIXER);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_DECOY)
		tempValue.Format("%d", ITEMKIND_FOR_MON_DECOY);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_FIRE)
		tempValue.Format("%d", ITEMKIND_FOR_MON_FIRE);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_OBJBEAM)
		tempValue.Format("%d", ITEMKIND_FOR_MON_OBJBEAM);
	else if(tempValue == STRCMD_CS_ITEMKIND_FOR_MON_STRAIGHTBOOM)
		tempValue.Format("%d", ITEMKIND_FOR_MON_STRAIGHTBOOM);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);

	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCameraPattern);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSourceIndex);
	
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDestParameter1);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditParameterValue1);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDestParameter2);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditParameterValue2);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDestParameter3);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditParameterValue3);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDestParameter4);
	m_pMonsterAttackDlg->m_GridUpdate.SetItemText(m_pMonsterAttackDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditParameterValue4);

	m_pMonsterAttackDlg->m_GridUpdate.Refresh();

	EndDialog(0);
}
