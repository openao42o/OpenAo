#if !defined(AFX_MAPEDITORDETAILDLG_H__A3AD5B5F_756F_41CE_AAFF_1F862D8C801E__INCLUDED_)
#define AFX_MAPEDITORDETAILDLG_H__A3AD5B5F_756F_41CE_AAFF_1F862D8C801E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapEditorDetailDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDetailDlg dialog
class CSpaceCowboyDBEditorToolDlg;
class CMapEditorDlg;

class CMapEditorDetailDlg : public CDialog
{
// Construction
public:
	void ReflectionToGrid(int type);//부모 다이얼로그의 Update GridCtrl에 값을 추가한다.
	BOOL ValidateCheck();//제약사항 체크
	CMapEditorDetailDlg(CWnd* pParent = NULL);   // standard constructor
	~CMapEditorDetailDlg();

// Dialog Data
	//{{AFX_DATA(CMapEditorDetailDlg)
	enum { IDD = IDD_DIALOG_MAPDETAIL };
	CEdit	m_ctrlEditMapIndex;
	CButton	m_ctrlButtonSave;
	CButton	m_ctrlButtonMake;
	CString	m_valueEditMapIndex;
	CString	m_valueEditRenderMapIndex;
	CString	m_valueEditMapName;
	CString	m_valueEditMapInfluenceType;
	CString	m_valueEditCityMapIndex;
	CString	m_valueEditCityWarQuestIndex;
	CString	m_valueEditCityWarQuestNPCIndex;
	CString	m_valueEditCityWarQuestMonsterUID;
	CString	m_valueEditMaxUserCount;
	CString	m_valueEditMaxGroundHeight;
	CString	m_valueEditMinGroundHeight;
	CString	m_valueEditWaterHeight;
	CString	m_valueEditUserVisibleDistance;
	CString	m_valueEditMonsterVisibleDistance;
	CString	m_valueEditQuestIndexForWarp;
	CString	m_valueEditDefaultMapIndex;
	CString	m_valueEditFrontPositionDistance;
	CString	m_valueEditAutoRecoveryFlag;
	CString	m_valueEditCityWarpTargetPositionX;
	CString	m_valueEditCityWarpTargetPositionY;
	CString	m_valueEditCityWarpTargetPositionZ;
	CString	m_valueEditClientMaxAltitudeHeight;
	CString	m_valueEditDayFogColor;
	CString	m_valueEditNightFogColor;
	CString	m_valueEditDayFogStartDistance;
	CString	m_valueEditDayFogEndDistance;
	CString	m_valueEditNightFogStartDistance;
	CString	m_valueEditNightFogEndDistance;
	CString	m_valueEditDaySunDirectionX;
	CString	m_valueEditDaySunDirectionY;
	CString	m_valueEditDaySunDirectionZ;
	CString	m_valueEditNightSunDirectionX;
	CString	m_valueEditNightSunDirectionY;
	CString	m_valueEditNightSunDirectionZ;
	CString	m_valueEditWaterType;
	CString	m_valueEditPKZoneFlag;
	CString	m_valueEditTileRenderingFlag;
	CString	m_valueEditSkyRenderingFlag;
	CString	m_valueEditSunRenderingFlag;
	CString	m_valueEditFogRenderingFlag;
	//}}AFX_DATA
	int m_nSelectRow;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg; //AtumAdminToll 다이얼로그 클래스
	CMapEditorDlg					*m_pMapEditorDlg; //부모 다이얼로그 포인터

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditorDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapEditorDetailDlg)
	afx_msg void OnButtonEnd();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMapdMake();
	afx_msg void OnButtonMapdSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITORDETAILDLG_H__A3AD5B5F_756F_41CE_AAFF_1F862D8C801E__INCLUDED_)
