#if !defined(AFX_MONSTERATTACKDLG_H__33D82391_DB14_4D77_8E72_220C2580D45F__INCLUDED_)
#define AFX_MONSTERATTACKDLG_H__33D82391_DB14_4D77_8E72_220C2580D45F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterAttackDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttackDlg dialog

class CMonsterAttackDlg : public CDialog
{
// Construction
public:
	void ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row); //양 GridCtrl간의 1row의 데이터를 옮긴다
	void SetDetailDialog(int row); //선택된 셀을 바탕으로 자식 다이얼로그에 값을 설정한다.
	void InsertItemToGridCurrent(_TI_ITEM *i_pTi_Item, int i_nRow); //DB에서 가저온 1row의 데이터를 Current GridCtrl에 반영한다.
	void GetCurrentList(void); //DB에서 데이터를 모두 불러온다.
	void InitGridUpdate(); //Update GridCtrl 초기 설정
	void InitGridCurrent(); //Current GridCtrl 초기 설정
	CMonsterAttackDlg(MONSTER_ITEMLIST_TYPE i_enItemListTy, int i_arrItemNumList[ARRAY_SIZE_MONSTER_ITEM]=NULL, CWnd* pParent = NULL);   // standard constructor
	CMonsterAttackDlg::~CMonsterAttackDlg();


// Dialog Data
	//{{AFX_DATA(CMonsterAttackDlg)
	enum { IDD = IDD_DIALOG_MONSTERATTACK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; //메인 다이얼로그 클래스
	CMonsterAttackDetailDlg				*m_pMonsterAttackDetailDlg; //자식 다이얼로그 포인터
	CGridCtrl		m_GridCurrent; //Current Grid Ctrl 변수
	CGridCtrl		m_GridUpdate; //Update Grid Ctrl 변수

	MONSTER_ITEMLIST_TYPE			m_enMonsterItemListType;								// 2005-12-23 by cmkwon, 
	int								m_arrMonsterItemNumList[ARRAY_SIZE_MONSTER_ITEM];		// 2005-12-23 by cmkwon, 하나의 몬스터 아이템 리스트

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterAttackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterAttackDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEnd();
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

#endif // !defined(AFX_MONSTERATTACKDLG_H__33D82391_DB14_4D77_8E72_220C2580D45F__INCLUDED_)
