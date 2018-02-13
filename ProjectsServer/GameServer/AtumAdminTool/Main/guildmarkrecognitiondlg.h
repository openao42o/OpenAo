#if !defined(AFX_GUILDMARKRECOGNITIONDLG_H__45EB361B_FDD5_4E18_BB52_8FBD3A8BAFF4__INCLUDED_)
#define AFX_GUILDMARKRECOGNITIONDLG_H__45EB361B_FDD5_4E18_BB52_8FBD3A8BAFF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// guildmarkrecognitiondlg.h : header file
//
#include "GridCtrl/GridCtrl.h"
#include "ODBCStatement.h"
/////////////////////////////////////////////////////////////////////////////
// CGuildMarkRecognitionDlg dialog
struct SGUILD_MARK_M1
{
	UID32_t		GuildUID1;
	char		GuildName1[SIZE_MAX_GUILD_NAME];
	char		GuildMarkImage1[SIZE_MAX_GUILD_MARK_IMAGE];
	BYTE		GuildMarkState1;
};
typedef vector<SGUILD_MARK_M1>			vectSGUILD_MARK_M1;

#define GRID_COL_INDEX_GUILD_UID				1			
#define GRID_COL_INDEX_GUILD_MARK_STATE			4

class CGuildMarkRecognitionDlg : public CDialog
{
// Construction
public:
	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 인자추가(, BOOL i_bEnableEdit)
	CGuildMarkRecognitionDlg(CODBCStatement *i_pODBCStat, BOOL i_bEnableEdit, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGuildMarkRecognitionDlg)
	enum { IDD = IDD_DLG_GUILD_MARK_RECOGNITION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuildMarkRecognitionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnGridGuildMarkListClick(NMHDR *pNotifyStruct, LRESULT* pResult);

	void InitGridGuildMarkList(void);
	void ViewGridGuildMarkList(void);
	BOOL AddLineToGridGuildMarkListMembers(const char *i_szKey, CBitmap *i_pbmGuildMark);
	SGUILD_MARK_M1 *FindGuildMarkByGuildUID(UID32_t i_guildUID);

	BOOL GetGuildMarkList(void);
	int UpdateGuildMarkState(vectSGUILD_MARK_M1 *i_pvectGuildLit);

// Implementation
protected:
	CGridCtrl				m_GridGuildMarkList;
	CImageList				m_ilGuildMarkList2;
	CODBCStatement			*m_pODBCStmt4;
	vectSGUILD_MARK_M1		m_vectGuildMarkList;

	BOOL					m_bEnableEdit2;			// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 					


	// Generated message map functions
	//{{AFX_MSG(CGuildMarkRecognitionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUILDMARKRECOGNITIONDLG_H__45EB361B_FDD5_4E18_BB52_8FBD3A8BAFF4__INCLUDED_)
