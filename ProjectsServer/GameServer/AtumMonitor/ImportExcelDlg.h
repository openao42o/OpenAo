#if !defined(AFX_IMPORTEXCELDLG_H__EA4D919F_25DF_4EB5_B7B4_4BC2EB90CC0F__INCLUDED_)
#define AFX_IMPORTEXCELDLG_H__EA4D919F_25DF_4EB5_B7B4_4BC2EB90CC0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportExcelDlg.h : header file
//

//#include "LoadExcelFile.h"
#include "GridCtrl/GridCtrl.h"

#define GRID_CELL_FORMAT	((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE)
#define GRID_COL_INDEX_EXCEL_TABLES			2

/////////////////////////////////////////////////////////////////////////////
// CImportExcelDlg dialog

class CImportExcelDlg : public CDialog
{
// Construction
public:
	CImportExcelDlg(CWnd* pParent = NULL);   // standard constructor

	void InitGridExcelTables(void);
	void ViewGridExcelTables(void);
	void InitAllWindows(void);
	void InitGridSelectedTables(void);		// 2009-11-10 by cmkwon, SCMonitor에 엑셀 파일 밀어넣기 수정 - 
// Dialog Data
	//{{AFX_DATA(CImportExcelDlg)
	enum { IDD = IDD_IMPORT_EXCELFILE };
	CComboBox	m_ComboServerList;
	CEdit	m_DBTables;
	CEdit	m_Select_ExcelFile;
	CGridCtrl	m_ExcelTables;
	CImageList	m_ilExcelTables2;
	// 2009. 10. 27 by jskim 진행률 확인
	CProgressCtrl	m_RecordProgress;
	CProgressCtrl	m_TotalProgress;
	CEdit			m_TableName;
	//end 2009. 10. 27 by jskim 진행률 확인
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportExcelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnGridExcelTablesClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	// 2009. 10. 27 by jskim 진행률 확인
	int			m_nMaxTotalCount;
	int			m_nCurrentTotalCount;
	afx_msg LONG OnUpdateExcelloadProgressRange(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateExcelloadProgressPos(WPARAM wParam, LPARAM lParam);
	//end 2009. 10. 27 by jskim 진행률 확인


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportExcelDlg)
	afx_msg void OnInsertDb();
	afx_msg void OnExcelfileSearch();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// Member variables
//	char m_FileName[MAX_PATH];
//	CLoadExcelFile m_ExcelData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTEXCELDLG_H__EA4D919F_25DF_4EB5_B7B4_4BC2EB90CC0F__INCLUDED_)
