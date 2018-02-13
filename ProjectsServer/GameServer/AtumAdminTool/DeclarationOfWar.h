#if !defined(AFX_DECLARATIONOFWAR_H__77C33AB0_E76D_4B36_B812_FB20FB00748A__INCLUDED_)
#define AFX_DECLARATIONOFWAR_H__77C33AB0_E76D_4B36_B812_FB20FB00748A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeclarationOfWar.h : header file
//
#include "GridCtrl/GridCtrl.h"

struct SDECLARATION_OF_WAR_FOR_ADMINTOOL
{
	INT				UID;
	BYTE			Influence;
	BYTE			MSWarStep;
	INT				NCP;
	INT				MSNum;
	MapIndex_t		MSAppearanceMap;
	ATUM_DATE_TIME	MSWarStepStartTime;
	ATUM_DATE_TIME	MSWarStepEndTime;
	ATUM_DATE_TIME	MSWarStartTime;
	ATUM_DATE_TIME	MSWarEndTime;
	BYTE			SelectCount;
	BOOL			GiveUp;
	BYTE			MSWarEndState;		// MSWAR_NOT_START : 전쟁 시작하지 않음, MSWARING : 전쟁중, MSWAR_END_WIN : 승리, MSWAR_END_LOSS : 패배
};
typedef vector<SDECLARATION_OF_WAR_FOR_ADMINTOOL>				vectSDECLARATION_OF_WAR_FOR_ADMINTOOL;				// 2009-01-12 by dhjin, 선전 포고

/////////////////////////////////////////////////////////////////////////////
// CDeclarationOfWar dialog
class CODBCStatement;
class CDeclarationOfWar : public CDialog
{
// Construction
public:
	CDeclarationOfWar(CODBCStatement *i_pODBCStat, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeclarationOfWar)
	enum { IDD = IDD_DLG_DECLARATION_OF_WAR };
	int		m_Step;
	COleDateTime	m_StepDateStart;
	COleDateTime	m_StepTimeStart;
	int		m_Select;
	int		m_Influence;
	CComboBox	m_comboDayOfWeek;
	COleDateTime	m_ForbidTimeEnd;
	COleDateTime	m_ForbidTimeStart;
	COleDateTime	m_MSDateStart;
	COleDateTime	m_MSTimeStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeclarationOfWar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CGridCtrl			m_GridDeclarationOfWarList;
	CODBCStatement		*m_pODBCStmtDeclarationOfWar;
	vectSDECLARATION_OF_WAR_FOR_ADMINTOOL		m_vectDECLARATION_OF_WAR;
	SDECLARATION_OF_WAR_FORBID_TIME m_SDeclarationOfWarForbidTime;

	void				GetDeclarationOfWarByDB();		
	void				InitGridDeclarationOfWarList();	
	void				InsertGridDeclarationOfWarList();
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeclarationOfWar)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStepStartUpdate();
	afx_msg void OnButtonStepSelect();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonForbidUpdate();
	afx_msg void OnButtonMsStartUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECLARATIONOFWAR_H__77C33AB0_E76D_4B36_B812_FB20FB00748A__INCLUDED_)
