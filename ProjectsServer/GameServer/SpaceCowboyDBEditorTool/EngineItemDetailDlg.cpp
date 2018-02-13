// EngineItemDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "EngineItemDetailDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEngineItemDetailDlg dialog


CEngineItemDetailDlg::CEngineItemDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEngineItemDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEngineItemDetailDlg)
	m_valueEditItemNum = _T("");
	m_valueEditPrice = _T("0");
	m_valueEditCashPrice = _T("0");
	m_valueEditItemName = _T("");
	m_valueEditReqMinLevel = _T("0");
	m_valueEditReqMaxLevel = _T("0");
	m_valueEditAbilityMin = _T("0");
	m_valueEditAbilityMax = _T("0");
	m_valueEditScarcity = _T("1");
	m_valueEditWeight = _T("0");
	m_valueEditRange = _T("0");
	m_valueEditTime = _T("0");
	m_valueEditMinTradeQuantity = _T("1");
	m_valueEditRangeAngle = _T("0");
	m_valueEditBoosterAngle = _T("0");
	m_valueEditReqAttack = _T("0");
	m_valueEditReqFuel = _T("0");
	m_valueEditReqShield = _T("0");
	m_valueEditReqDefense = _T("0");
	m_valueEditReqSoul = _T("0");
	m_valueEditReqDodge = _T("0");
	m_valueEditDescription = _T("");
	m_valueCheckBGEAR = FALSE;
	m_valueCheckAGEAR = FALSE;
	m_valueCheckMGEAR = FALSE;
	m_valueCheckIGEAR = FALSE;
	m_valueCheckAutoPickUp = FALSE;
	m_valueCheckNoTransfer = FALSE;
	m_valueCheckQuestItem = FALSE;
	m_valueCheckTimeLimite = FALSE;
	m_valueCheckCashItem = FALSE;
	m_valueEditSourceIndex = _T("0");
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pEngineItem = (CEngineItemDlg*)pParent;
	m_nSelectRow = 0;
}

CEngineItemDetailDlg::~CEngineItemDetailDlg()
{
}


void CEngineItemDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEngineItemDetailDlg)
	DDX_Control(pDX, IDC_IE_COMBO_RACE2, m_ctrlComboReqRace2);
	DDX_Control(pDX, IDC_IE_COMBO_RACE, m_ctrlComboReqRace);
	DDX_Control(pDX, IDC_IE_COMBO_POSITION, m_ctrlComboPosition);
	DDX_Control(pDX, IDC_IE_COMBO_KIND, m_ctrlComboKind);
	DDX_Control(pDX, IDC_IE_BUTTON_CANCEL, m_ctrlButtonCancel);
	DDX_Control(pDX, IDC_IE_BUTTON_MAKE, m_ctrlButtonMake);
	DDX_Control(pDX, IDC_IE_BUTTON_SAVE, m_ctrlButtonSave);
	DDX_Text(pDX, IDC_IE_EDIT29, m_valueEditItemNum);
	DDX_Text(pDX, IDC_IE_EDIT21, m_valueEditPrice);
	DDX_Text(pDX, IDC_IE_EDIT31, m_valueEditCashPrice);
	DDX_Text(pDX, IDC_IE_EDIT32, m_valueEditItemName);
	DDX_Text(pDX, IDC_IE_EDIT16, m_valueEditReqMinLevel);
	DDX_Text(pDX, IDC_IE_EDIT17, m_valueEditReqMaxLevel);
	DDX_Text(pDX, IDC_IE_EDIT_ATTACKMIN, m_valueEditAbilityMin);
	DDX_Text(pDX, IDC_IE_EDIT_ATTACKMAX, m_valueEditAbilityMax);
	DDX_Text(pDX, IDC_IE_EDIT_SCARCITY, m_valueEditScarcity);
	DDX_Text(pDX, IDC_IE_EDIT14, m_valueEditWeight);
	DDX_Text(pDX, IDC_IE_EDIT10, m_valueEditRange);
	DDX_Text(pDX, IDC_IE_EDIT23, m_valueEditTime);
	DDX_Text(pDX, IDC_IE_EDIT27, m_valueEditMinTradeQuantity);
	DDX_Text(pDX, IDC_IE_EDIT_RANGEANGLE, m_valueEditRangeAngle);
	DDX_Text(pDX, IDC_IE_EDIT_BOOSTERANGLE, m_valueEditBoosterAngle);
	DDX_Text(pDX, IDC_IE_EDIT_REQATTACK, m_valueEditReqAttack);
	DDX_Text(pDX, IDC_IE_EDIT_REQFUEL, m_valueEditReqFuel);
	DDX_Text(pDX, IDC_IE_EDIT_REQSHIELD, m_valueEditReqShield);
	DDX_Text(pDX, IDC_IE_EDIT_REQDEFENSE, m_valueEditReqDefense);
	DDX_Text(pDX, IDC_IE_EDIT_REQSOUL, m_valueEditReqSoul);
	DDX_Text(pDX, IDC_IE_EDIT_REQDODGE, m_valueEditReqDodge);
	DDX_Text(pDX, IDC_IE_EDIT30, m_valueEditDescription);
	DDX_Check(pDX, IDC_IE_CHECK_BGEAR, m_valueCheckBGEAR);
	DDX_Check(pDX, IDC_IE_CHECK_AGEAR, m_valueCheckAGEAR);
	DDX_Check(pDX, IDC_IE_CHECK_MGEAR, m_valueCheckMGEAR);
	DDX_Check(pDX, IDC_IE_CHECK_IGEAR, m_valueCheckIGEAR);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM2, m_valueCheckAutoPickUp);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM3, m_valueCheckNoTransfer);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM4, m_valueCheckQuestItem);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM5, m_valueCheckTimeLimite);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM, m_valueCheckCashItem);
	DDX_Text(pDX, IDC_IE_EDIT39, m_valueEditSourceIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEngineItemDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CEngineItemDetailDlg)
	ON_BN_CLICKED(IDC_IE_BUTTON_MAKE, OnButtonMake)
	ON_BN_CLICKED(IDC_IE_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_IE_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEngineItemDetailDlg message handlers

void CEngineItemDetailDlg::OnButtonMake() 
{
	// TODO: Add your control notification handler code here
//	if(ValidateCheck()== FALSE)
//		return;
	ReflectionToGrid(1);
}

void CEngineItemDetailDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
//	if(ValidateCheck()== FALSE)
//		return;
	m_pEngineItem->m_GridCurrent.DeleteRow(m_nSelectRow);
	ReflectionToGrid(2);
}

void CEngineItemDetailDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

BOOL CEngineItemDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ctrlComboReqRace.InsertString(0,STRCMD_CS_COMMON_RACE_BATTALUS);
	m_ctrlComboReqRace.InsertString(1,STRCMD_CS_COMMON_RACE_DECA);
	m_ctrlComboReqRace.InsertString(2,STRCMD_CS_COMMON_RACE_PHILON);
	m_ctrlComboReqRace.InsertString(3,STRCMD_CS_COMMON_RACE_SHARRINE);
	//콤보 박스 나누기
	m_ctrlComboReqRace2.InsertString(0,STRCMD_CS_COMMON_RACE_OPERATION);
	m_ctrlComboReqRace2.InsertString(1,STRCMD_CS_COMMON_RACE_GAMEMASTER);
	m_ctrlComboReqRace2.InsertString(2,STRCMD_CS_COMMON_RACE_MONITOR);
	m_ctrlComboReqRace2.InsertString(3,STRCMD_CS_COMMON_RACE_GUEST);
	m_ctrlComboReqRace2.InsertString(4,STRCMD_CS_COMMON_RACE_DEMO);

	m_ctrlComboKind.InsertString(0,STRCMD_CS_ITEMKIND_SUPPORT);

	m_ctrlComboPosition.InsertString(0,STRCMD_CS_POS_PROW);
	m_ctrlComboPosition.InsertString(1,STRCMD_CS_POS_PROWIN);
	m_ctrlComboPosition.InsertString(2,STRCMD_CS_POS_PROWOUT);
	m_ctrlComboPosition.InsertString(3,STRCMD_CS_POS_WINGIN);
	m_ctrlComboPosition.InsertString(4,STRCMD_CS_POS_WINGOUT);
	m_ctrlComboPosition.InsertString(5,STRCMD_CS_POS_CENTER);
	m_ctrlComboPosition.InsertString(6,STRCMD_CS_POS_REAR);
	m_ctrlComboPosition.InsertString(7,STRCMD_CS_POS_ACCESSORY_UNLIMITED /*STRCMD_CS_POS_ATTACHMENT*/);
	m_ctrlComboPosition.InsertString(8,STRCMD_CS_POS_ACCESSORY_TIME_LIMIT /*STRCMD_CS_POS_PET*/);	// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
	m_ctrlComboPosition.InsertString(9,STRCMD_CS_POS_PET);	// 2010-06-15 by shcho&hslee 펫시스템 - 팻 추가.
	m_ctrlComboPosition.SetCurSel(6);
	m_ctrlComboPosition.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEngineItemDetailDlg::ReflectionToGrid(int type)
{
	// TODO: Add your control notification handler code here
	// 칼럼 만들기
	m_pEngineItem->m_GridUpdate.SetRowCount(m_pEngineItem->m_GridUpdate.GetRowCount()+1);

	//각 콘트롤의 값들을 CWeaponItem의 GridUpdate에 넣는다.
	UpdateData();
	
	int m_nCols= 0;
	CString tempValue;
	int n_tempValue = 0;
	if(type == 1)
		m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, "INSERT");
	else if(type == 2)
		m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, "UPDATE");
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditItemNum);
	m_ctrlComboKind.GetWindowText(tempValue);
	if(tempValue == STRCMD_CS_ITEMKIND_SUPPORT)
		tempValue.Format("%d", ITEMKIND_SUPPORT);
	
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditItemName);
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAbilityMin);//abilitymin
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAbilityMax);//abilitymax

	m_ctrlComboReqRace.GetWindowText(tempValue);
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
	m_ctrlComboReqRace2.GetWindowText(tempValue);
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
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqAttack);//ReqAttackPart
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqDefense);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqFuel);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqSoul);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqShield);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqDodge);//
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
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);//ReqUnitKind
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqMinLevel);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqMaxLevel);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditWeight);//

	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditRange);//

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
	else if(tempValue == STRCMD_CS_POS_ACCESSORY_UNLIMITED/*STRCMD_CS_POS_ATTACHMENT*/)
		tempValue.Format("%d", POS_ACCESSORY_UNLIMITED /*POS_ATTACHMENT*/);
	else if(tempValue == STRCMD_CS_POS_ACCESSORY_TIME_LIMIT /*STRCMD_CS_POS_PET*/)	// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
		tempValue.Format("%d", POS_ACCESSORY_TIME_LIMIT /*POS_PET*/);	// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
	else if(tempValue == STRCMD_CS_POS_PET )							// 2010-06-15 by shcho&hslee 펫시스템 - 팻 추가.
		tempValue.Format("%d", POS_PET);

	
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);//Position

	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditScarcity);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMinTradeQuantity);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditPrice);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCashPrice);//

	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditTime);//
		
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditRangeAngle);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditBoosterAngle);//

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

	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDescription);//
	m_pEngineItem->m_GridUpdate.SetItemText(m_pEngineItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSourceIndex);//


	m_pEngineItem->m_GridUpdate.Refresh();
	EndDialog(0);
}

BOOL CEngineItemDetailDlg::ValidateCheck()
{
	UpdateData();
	
	if(m_valueEditItemNum == "")
	{
		AfxMessageBox("Please Check ItemNum Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAbilityMin) > 100000 ||
		atoi((char*)(LPCTSTR)m_valueEditAbilityMin) <0)
	{
		AfxMessageBox("Please Check AbilityMin Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAbilityMax) > 100000 ||
		atoi((char*)(LPCTSTR)m_valueEditAbilityMax) <0)
	{
		AfxMessageBox("Please Check AbilityMax Value");
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
	if(atoi((char*)(LPCTSTR)m_valueEditReqMinLevel) > 100 ||
		atoi((char*)(LPCTSTR)m_valueEditReqMinLevel) <0)
	{
		AfxMessageBox("Please Check ReqMinLevel Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReqMaxLevel) > 100 ||
		atoi((char*)(LPCTSTR)m_valueEditReqMaxLevel) <0)
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

	if(atoi((char*)(LPCTSTR)m_valueEditRangeAngle) > 3.14 ||
		atoi((char*)(LPCTSTR)m_valueEditRangeAngle) <0)
	{
		AfxMessageBox("Please Check Weight RangeAngle");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditBoosterAngle) > 3.14 ||
		atoi((char*)(LPCTSTR)m_valueEditBoosterAngle) <0)
	{
		AfxMessageBox("Please Check Weight BoosterAngle");
		return FALSE;
	}
	
	if(atoi((char*)(LPCTSTR)m_valueEditRange) > 9999 ||
		atoi((char*)(LPCTSTR)m_valueEditRange) <0)
	{
		AfxMessageBox("Please Check Range Value");
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
	
	if(atoi((char*)(LPCTSTR)m_valueEditTime) > 99999 ||
		atoi((char*)(LPCTSTR)m_valueEditTime) <0)
	{
		AfxMessageBox("Please Check Range Time");
		return FALSE;
	}


	if(m_valueEditDescription.GetLength()>200)
	{
		AfxMessageBox("Please Check Description String Length");
		return FALSE;
	}

	//콤보박스가 선택된것이 없으면 에러처리
	CString szCheck;
	m_ctrlComboKind.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Kind Value");
		return FALSE;
	}

	m_ctrlComboPosition.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Position Value");
		return FALSE;
	}

	m_ctrlComboReqRace.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check ReqRace Value");
		return FALSE;
	}


	return TRUE;
}