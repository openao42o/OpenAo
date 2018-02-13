#if !defined(AFX_MAPEDITORDLG_H__AD46AB92_791F_4273_B91F_2FF8CD690764__INCLUDED_)
#define AFX_MAPEDITORDLG_H__AD46AB92_791F_4273_B91F_2FF8CD690764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapEditorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDlg dialog
class CSpaceCowboyDBEditorToolDlg;

class CMapEditorDlg : public CDialog
{
// Construction
public:
	void SetDetailDialog(int row); // 선택된 Row를 자식 다이얼로그의 컨트롤에 셋팅
	void ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row); //Grid의 선택된 row 데이터를 다른 Grid로 이전
	void InsertItemToGridCurrent(_TI_MAPINFO *i_pTi_MapInfo, int i_nRow); //DB에서 select한 1줄의 데이터를 Current Grid Ctrl에 넣는 함수
	void GetCurrentList();// DB에서 값을 가져와서 Current Grid Ctrl에 셋팅
	void InitGridUpdate();// Update Grid Ctrl의 초기화
	void InitGridCurrent();// Current Grid Ctrl의 초기화
	CMapEditorDlg(CWnd* pParent = NULL);   // standard constructor
	~CMapEditorDlg();

	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg; //메인 다이얼로그 클래
	CMapEditorDetailDlg				*m_pMapDetailDlg;
	CGridCtrl						m_GridCurrent; // Current Grid Ctrl
	CGridCtrl						m_GridUpdate; // Update Grid Ctrl
// Dialog Data
	//{{AFX_DATA(CMapEditorDlg)
	enum { IDD = IDD_DIALOG_MAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapEditorDlg)
	afx_msg void OnButtonEnd();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonView();
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonServerupdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPEDITORDLG_H__AD46AB92_791F_4273_B91F_2FF8CD690764__INCLUDED_)
