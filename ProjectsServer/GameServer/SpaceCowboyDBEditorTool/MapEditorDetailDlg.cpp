// MapEditorDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "MapEditorDetailDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDetailDlg dialog


CMapEditorDetailDlg::CMapEditorDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapEditorDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapEditorDetailDlg)
	m_valueEditMapIndex = _T("");
	m_valueEditRenderMapIndex = _T("0");
	m_valueEditMapName = _T("");
	m_valueEditMapInfluenceType = _T("0");
	m_valueEditCityMapIndex = _T("0");
	m_valueEditCityWarQuestIndex = _T("0");
	m_valueEditCityWarQuestNPCIndex = _T("0");
	m_valueEditCityWarQuestMonsterUID = _T("0");
	m_valueEditMaxUserCount = _T("0");
	m_valueEditMaxGroundHeight = _T("0");
	m_valueEditMinGroundHeight = _T("0");
	m_valueEditWaterHeight = _T("0");
	m_valueEditUserVisibleDistance = _T("0");
	m_valueEditMonsterVisibleDistance = _T("0");
	m_valueEditQuestIndexForWarp = _T("0");
	m_valueEditDefaultMapIndex = _T("0");
	m_valueEditFrontPositionDistance = _T("0");
	m_valueEditAutoRecoveryFlag = _T("0");
	m_valueEditCityWarpTargetPositionX = _T("0");
	m_valueEditCityWarpTargetPositionY = _T("0");
	m_valueEditCityWarpTargetPositionZ = _T("0");
	m_valueEditClientMaxAltitudeHeight = _T("0");
	m_valueEditDayFogColor = _T("0");
	m_valueEditNightFogColor = _T("0");
	m_valueEditDayFogStartDistance = _T("0");
	m_valueEditDayFogEndDistance = _T("0");
	m_valueEditNightFogStartDistance = _T("0");
	m_valueEditNightFogEndDistance = _T("0");
	m_valueEditDaySunDirectionX = _T("0");
	m_valueEditDaySunDirectionY = _T("0");
	m_valueEditDaySunDirectionZ = _T("0");
	m_valueEditNightSunDirectionX = _T("0");
	m_valueEditNightSunDirectionY = _T("0");
	m_valueEditNightSunDirectionZ = _T("0");
	m_valueEditWaterType = _T("0");
	m_valueEditPKZoneFlag = _T("0");
	m_valueEditTileRenderingFlag = _T("0");
	m_valueEditSkyRenderingFlag = _T("0");
	m_valueEditSunRenderingFlag = _T("0");
	m_valueEditFogRenderingFlag = _T("0");
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd(); //메인 다이얼로그 포인터 얻기
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pMapEditorDlg = (CMapEditorDlg*)pParent; // 부모 클래스 포인터 저장
	m_nSelectRow = 0;
}

CMapEditorDetailDlg::~CMapEditorDetailDlg()
{
// 2006-01-23 by cmkwon
// 	util::del(m_pODBCStmt);
}


void CMapEditorDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapEditorDetailDlg)
	DDX_Control(pDX, IDC_EDIT36, m_ctrlEditMapIndex);
	DDX_Control(pDX, IDC_BUTTON_MAPD_SAVE, m_ctrlButtonSave);
	DDX_Control(pDX, IDC_BUTTON_MAPD_MAKE, m_ctrlButtonMake);
	DDX_Text(pDX, IDC_EDIT36, m_valueEditMapIndex);
	DDX_Text(pDX, IDC_EDIT7, m_valueEditRenderMapIndex);
	DDX_Text(pDX, IDC_EDIT1, m_valueEditMapName);
	DDX_Text(pDX, IDC_EDIT94, m_valueEditMapInfluenceType);
	DDX_Text(pDX, IDC_EDIT18, m_valueEditCityMapIndex);
	DDX_Text(pDX, IDC_EDIT2, m_valueEditCityWarQuestIndex);
	DDX_Text(pDX, IDC_EDIT30, m_valueEditCityWarQuestNPCIndex);
	DDX_Text(pDX, IDC_EDIT3, m_valueEditCityWarQuestMonsterUID);
	DDX_Text(pDX, IDC_EDIT31, m_valueEditMaxUserCount);
	DDX_Text(pDX, IDC_EDIT37, m_valueEditMaxGroundHeight);
	DDX_Text(pDX, IDC_EDIT32, m_valueEditMinGroundHeight);
	DDX_Text(pDX, IDC_EDIT4, m_valueEditWaterHeight);
	DDX_Text(pDX, IDC_EDIT33, m_valueEditUserVisibleDistance);
	DDX_Text(pDX, IDC_EDIT5, m_valueEditMonsterVisibleDistance);
	DDX_Text(pDX, IDC_EDIT34, m_valueEditQuestIndexForWarp);
	DDX_Text(pDX, IDC_EDIT6, m_valueEditDefaultMapIndex);
	DDX_Text(pDX, IDC_EDIT87, m_valueEditFrontPositionDistance);
	DDX_Text(pDX, IDC_EDIT35, m_valueEditAutoRecoveryFlag);
	DDX_Text(pDX, IDC_EDIT15, m_valueEditCityWarpTargetPositionX);
	DDX_Text(pDX, IDC_EDIT38, m_valueEditCityWarpTargetPositionY);
	DDX_Text(pDX, IDC_EDIT41, m_valueEditCityWarpTargetPositionZ);
	DDX_Text(pDX, IDC_EDIT39, m_valueEditClientMaxAltitudeHeight);
	DDX_Text(pDX, IDC_EDIT42, m_valueEditDayFogColor);
	DDX_Text(pDX, IDC_EDIT11, m_valueEditNightFogColor);
	DDX_Text(pDX, IDC_EDIT40, m_valueEditDayFogStartDistance);
	DDX_Text(pDX, IDC_EDIT88, m_valueEditDayFogEndDistance);
	DDX_Text(pDX, IDC_EDIT73, m_valueEditNightFogStartDistance);
	DDX_Text(pDX, IDC_EDIT12, m_valueEditNightFogEndDistance);
	DDX_Text(pDX, IDC_EDIT84, m_valueEditDaySunDirectionX);
	DDX_Text(pDX, IDC_EDIT13, m_valueEditDaySunDirectionY);
	DDX_Text(pDX, IDC_EDIT85, m_valueEditDaySunDirectionZ);
	DDX_Text(pDX, IDC_EDIT14, m_valueEditNightSunDirectionX);
	DDX_Text(pDX, IDC_EDIT86, m_valueEditNightSunDirectionY);
	DDX_Text(pDX, IDC_EDIT93, m_valueEditNightSunDirectionZ);
	DDX_Text(pDX, IDC_EDIT89, m_valueEditWaterType);
	DDX_Text(pDX, IDC_EDIT90, m_valueEditPKZoneFlag);
	DDX_Text(pDX, IDC_EDIT25, m_valueEditTileRenderingFlag);
	DDX_Text(pDX, IDC_EDIT91, m_valueEditSkyRenderingFlag);
	DDX_Text(pDX, IDC_EDIT26, m_valueEditSunRenderingFlag);
	DDX_Text(pDX, IDC_EDIT92, m_valueEditFogRenderingFlag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapEditorDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CMapEditorDetailDlg)
	ON_BN_CLICKED(IDC_BUTTON_MAPD_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_MAPD_MAKE, OnButtonMapdMake)
	ON_BN_CLICKED(IDC_BUTTON_MAPD_SAVE, OnButtonMapdSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDetailDlg message handlers

BOOL CMapEditorDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapEditorDetailDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CMapEditorDetailDlg::OnButtonMapdMake() 
{
	// TODO: Add your control notification handler code here
	if(ValidateCheck()== FALSE)
		return;
	ReflectionToGrid(1);
}

void CMapEditorDetailDlg::OnButtonMapdSave() 
{
	// TODO: Add your control notification handler code here
	if(ValidateCheck()== FALSE)
		return;
	m_pMapEditorDlg->m_GridCurrent.DeleteRow(m_nSelectRow);
	ReflectionToGrid(2);
}

BOOL CMapEditorDetailDlg::ValidateCheck()
{
	/*
	m_valueEditMapIndex = _T("");
	m_valueEditRenderMapIndex = _T("0");
	m_valueEditMapName = _T("");
	m_valueEditCityMapIndex = _T("0");
	m_valueEditCityWarQuestIndex = _T("0");
	m_valueEditCityWarQuestNPCIndex = _T("0");
	m_valueEditCityWarQuestMonsterUID = _T("0");
	m_valueEditMaxUserCount = _T("0");
	m_valueEditMaxGroundHeight = _T("0");
	m_valueEditMinGroundHeight = _T("0");
	m_valueEditWaterHeight = _T("0");
	m_valueEditUserVisibleDistance = _T("0");
	m_valueEditMonsterVisibleDistance = _T("0");
	m_valueEditQuestIndexForWarp = _T("0");
	m_valueEditDefaultMapIndex = _T("0");
	m_valueEditFrontPositionDistance = _T("0");
	m_valueEditAutoRecoveryFlag = _T("0");
	m_valueEditCityWarpTargetPositionX = _T("0");
	m_valueEditCityWarpTargetPositionY = _T("0");
	m_valueEditCityWarpTargetPositionZ = _T("0");
	m_valueEditClientMaxAltitudeHeight = _T("0");
	m_valueEditDayFogColor = _T("0");
	m_valueEditNightFogColor = _T("0");
	m_valueEditDayFogStartDistance = _T("0");
	m_valueEditDayFogEndDistance = _T("0");
	m_valueEditNightFogStartDistance = _T("0");
	m_valueEditNightFogEndDistance = _T("0");
	m_valueEditDaySunDirectionX = _T("0");
	m_valueEditDaySunDirectionY = _T("0");
	m_valueEditDaySunDirectionZ = _T("0");
	m_valueEditNightSunDirectionX = _T("0");
	m_valueEditNightSunDirectionY = _T("0");
	m_valueEditNightSunDirectionZ = _T("0");
	m_valueEditWaterType = _T("0");
	m_valueEditPKZoneFlag = _T("0");
	m_valueEditTileRenderingFlag = _T("0");
	m_valueEditSkyRenderingFlag = _T("0");
	m_valueEditSunRenderingFlag = _T("0");
	m_valueEditFogRenderingFlag = _T("0");
	*/
	UpdateData();
	if(m_valueEditMapIndex == "")
	{
		AfxMessageBox("Please Check Map Index Value");
		return FALSE;
	}
	return TRUE;
}

void CMapEditorDetailDlg::ReflectionToGrid(int type)
{
	// 칼럼 만들기
	m_pMapEditorDlg->m_GridUpdate.SetRowCount(m_pMapEditorDlg->m_GridUpdate.GetRowCount()+1);

	//각 콘트롤의 값들을 CWeaponItem의 GridUpdate에 넣는다.
	UpdateData();
	
	int m_nCols= 0;
	CString tempValue;
	int n_tempValue = 0;
	if(type == 1)
		m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, "INSERT");
	else if(type == 2)
		m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, "UPDATE");

	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMapIndex);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditRenderMapIndex);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMapName);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMapInfluenceType);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCityMapIndex);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCityWarQuestIndex);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCityWarQuestNPCIndex);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCityWarQuestMonsterUID);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMaxUserCount);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMaxGroundHeight);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMinGroundHeight);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditWaterHeight);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditUserVisibleDistance);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMonsterVisibleDistance);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditQuestIndexForWarp);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDefaultMapIndex);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditFrontPositionDistance);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAutoRecoveryFlag);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCityWarpTargetPositionX);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCityWarpTargetPositionY);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCityWarpTargetPositionZ);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditClientMaxAltitudeHeight);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDayFogColor);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditNightFogColor);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDayFogStartDistance);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDayFogEndDistance);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditNightFogStartDistance);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditNightFogEndDistance);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDaySunDirectionX);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDaySunDirectionY);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDaySunDirectionZ);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditNightSunDirectionX);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditNightSunDirectionY);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditNightSunDirectionZ);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditWaterType);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditPKZoneFlag);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditTileRenderingFlag);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSkyRenderingFlag);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSunRenderingFlag);
	m_pMapEditorDlg->m_GridUpdate.SetItemText(m_pMapEditorDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditFogRenderingFlag);
	
	m_pMapEditorDlg->m_GridUpdate.Refresh();

	EndDialog(0);
}
