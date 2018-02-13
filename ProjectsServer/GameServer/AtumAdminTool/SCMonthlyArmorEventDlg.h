#if !defined(AFX_SCMONTHLYARMOREVENTDLG_H__4767653E_0941_43AB_916A_7FC5D287E132__INCLUDED_)
#define AFX_SCMONTHLYARMOREVENTDLG_H__4767653E_0941_43AB_916A_7FC5D287E132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCMonthlyArmorEventDlg.h : header file
//
#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "ODBCStatement.h"	

#include "AtumAdminToolDlg.h"

#define GRID1_COL_EVENT_INFO_EVENT_NUM			0
#define GRID1_COL_EVENT_INFO_EVENT_START_DATE	1
#define GRID1_COL_EVENT_INFO_EVENT_END_DATE		2
#define GRID1_COL_EVENT_INFO_ARMOR_NAME			3
#define GRID1_COL_EVENT_INFO_OPTION				4

#define GRID2_COL_ARMOR_INFO_SKILLLEVEL		0
#define GRID2_COL_ARMOR_INFO_ITEM_INDEX		1
#define GRID2_COL_ARMOR_INFO_ARMOR_NAME		2

#define COMBO_LIST1			"Name"
#define COMBO_LIST2			"ItemIndex"


// 2013-04-18 by jhseol,bckim 이달의 아머
struct MONTHLY_ARMOR_EVENT
{
	MONTHLY_ARMOR_EVNET_INFO	stMonthlyEventInfo;
	CHAR ArmorName[50];
	CHAR ArmorOptionName[50];
	INT		TempRows;

	MONTHLY_ARMOR_EVENT() 
	{ memset(this,0x00,sizeof(MONTHLY_ARMOR_EVENT)); }
};
typedef vector<MONTHLY_ARMOR_EVENT> vectMONTHLY_ARMOR_EVENT_LIST;
// end 2013-04-18 by jhseol,bckim 이달의 아머



struct MONTHLY_ARMOR_INFO
{
	BYTE	SkillLevel;
	INT		ItemNum;
	INT		LinkItem;
	char	ItemName[SIZE_MAX_ITEM_NAME];
	INT		TempRows;
	INT		ReqUnitKind;

	MONTHLY_ARMOR_INFO()
	{ memset(this,0x00,sizeof(MONTHLY_ARMOR_INFO)); }
};
typedef vector<MONTHLY_ARMOR_INFO>		vectMONTHLY_ARMOR_INFO;

struct MONTHLY_OPTION_INFO
{
	INT		ItemNum;
	char	ItemName[SIZE_MAX_ITEM_NAME];
	INT		TempRows;
	INT		ReqUnitKind;

	MONTHLY_OPTION_INFO()
	{ memset(this,0x00,sizeof(MONTHLY_OPTION_INFO)); }
};
typedef vector<MONTHLY_OPTION_INFO>		vectMONTHLY_OPTION_INFO;


/////////////////////////////////////////////////////////////////////////////
// SCMonthlyArmorEventDlg dialog
class CAtumAdminToolDlg;
class SCMonthlyArmorEventDlg : public CDialog
{
// Construction
public:
	SCMonthlyArmorEventDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SCMonthlyArmorEventDlg();

	CGridCtrl					m_GridList1;
	CGridCtrl					m_GridList2;


	vectMONTHLY_ARMOR_EVENT_LIST	m_vectCurMonthlyEventInfo;
	vectMONTHLY_ARMOR_INFO			m_vectCurMonthlyArmorInfoBase;
	vectMONTHLY_OPTION_INFO			m_vectEditMonthlyOptionList;

	vectMONTHLY_OPTION_INFO			m_vectSortedMonthlyOptionList;

	vectMONTHLY_ARMOR_INFO			m_vectCurMonthlyArmorInfo;		//	 조회된 아머 리스트 

	BOOL DBQueryGetMonthlyEventInfo(vectMONTHLY_ARMOR_EVENT_LIST *o_pVectEventList);
	BOOL DBQueryGetMonthlyArmorInfo(vectMONTHLY_ARMOR_INFO *o_pVectArmorLis);
	BOOL DBQueryGetMonthlyOptionInfo(vectMONTHLY_OPTION_INFO *o_pVectOptionList);

	BOOL IsExsistSameKit(MONTHLY_ARMOR_EVNET_INFO * i_pNewEvent);

	void Refresh();

	BOOL InsertDBMonthlyEvent(MONTHLY_ARMOR_EVNET_INFO* tmpInsertEvent );
	BOOL DeleteDBMonthlyEvent(INT EventUID);

	MONTHLY_OPTION_INFO			m_SelectedOption;
	MONTHLY_ARMOR_INFO			m_SelectedArmor;
	MONTHLY_ARMOR_EVENT			m_SelectedEvent;

	INT	GetOptionRowByItemNum(INT OptionItemNum);
	INT	GetArmorRowByItemNum(INT ArmorItemNum);

// Dialog Data
	//{{AFX_DATA(SCMonthlyArmorEventDlg)
	enum { IDD = IDD_DLG_MONTHLY_ARMOR_EVENT };
	CEdit	m_EditSearchArmor;
	CEdit	m_EditEventIndex;
	CComboBox	m_comboIndexType;
	CComboBox	m_comboOptionList;
	COleDateTime	m_timeEndTime;
	COleDateTime	m_timeEndDate;
	COleDateTime	m_timeStartDate;
	COleDateTime	m_timeStartTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SCMonthlyArmorEventDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// 2013-06-18 by bckim, 이달의 아머 운영툴 보완작업
	//}}AFX_VIRTUAL
	
// Implementation

public:

	void _InitGrid_EventInfo(CGridCtrl *i_pGridCtrl);
	INT _AddEventInfo(CGridCtrl *i_pGridCtrl, MONTHLY_ARMOR_EVENT *i_pEvenrInfo);
	void ViewGridEventInfo(CGridCtrl *i_pGridCtrl);

	void _InitGrid_ArmorInfo(CGridCtrl *i_pGridCtrl);
	INT _AddArmorInfo(CGridCtrl *i_pGridCtrl, MONTHLY_ARMOR_INFO *i_pARmor_Info);
	void ViewGridArmorInfo(CGridCtrl *i_pGridCtrl);
	void Refresh_Option();

protected:


	CODBCStatement			m_odbcStmt2;
	CAtumAdminToolDlg *		m_pMainDlg;

	// Generated message map functions
	//{{AFX_MSG(SCMonthlyArmorEventDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonMonthlySearch();
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnReloadMonthlyEvent();
	//}}AFX_MSG
	afx_msg void OnGrid1Click(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGrid2Click(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCMONTHLYARMOREVENTDLG_H__4767653E_0941_43AB_916A_7FC5D287E132__INCLUDED_)
