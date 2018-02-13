#if !defined(AFX_SCREENKEYBOARDDLG_H__5C515141_87E7_4574_A328_00DA90BF298E__INCLUDED_)
#define AFX_SCREENKEYBOARDDLG_H__5C515141_87E7_4574_A328_00DA90BF298E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// screenkeyboarddlg.h : header file
//

#define EXE2_KEY_TYPE_1_ALL_COUNT		36			
#define EXE2_KEY_TYPE_1_R1_COUNT		10			
#define EXE2_KEY_TYPE_1_R2_COUNT		20			
#define EXE2_KEY_TYPE_1_R3_COUNT		29			
#define EXE2_KEY_TYPE_1_R4_COUNT		36			


/////////////////////////////////////////////////////////////////////////////
// CScreenKeyboardDlg dialog

class CScreenKeyboardDlg : public CDialog
{
// Construction
public:
	CScreenKeyboardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScreenKeyboardDlg)
	enum { IDD = IDD_DLG_SCREEN_KEYBOARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenKeyboardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL				m_bOnShift;
	BOOL				m_bOnCapsLock;
	CBitmap				m_bmpKeyboard;
	CBitmap				m_bmpKeyboardMask;

	CButton *			m_pKeyButton1;
	CBitmapButton		m_arrBtn[EXE2_KEY_TYPE_1_ALL_COUNT];
	CBitmapButton		m_bmpBtnBackSpace;
	CBitmapButton		m_bmpBtnCapsLock;
	CBitmapButton		m_bmpBtnEnter;
	CBitmapButton		m_bmpBtnShift;
	CBitmapButton		m_bmpBtnSpace;

	

	BOOL BitmapRgn( UINT resource, COLORREF TansColor );
	BOOL BitmapRgn( LPCTSTR resource, COLORREF TansColor );
	HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance);	

	void RandomMoveWindow(void);
	void MoveKeyButtons(void);
	void LoadImageOfKeyButtons(void);
	BOOL IsCapticalLetter(void);
	char GetKeyChar(int i_nKeyIndex);
	void SendKeyChar(char i_cKeyChar);
	BOOL IsSpecialLetter(void);			// 2007-09-18 by cmkwon, 화상키보드 수정 - 특수문자 관련

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScreenKeyboardDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg void OnKey0();
	afx_msg void OnKey1();
	afx_msg void OnKey2();
	afx_msg void OnKey3();
	afx_msg void OnKey4();
	afx_msg void OnKey5();
	afx_msg void OnKey6();
	afx_msg void OnKey7();
	afx_msg void OnKey8();
	afx_msg void OnKey9();
	afx_msg void OnKey10();
	afx_msg void OnKey11();
	afx_msg void OnKey12();
	afx_msg void OnKey13();
	afx_msg void OnKey14();
	afx_msg void OnKey15();
	afx_msg void OnKey16();
	afx_msg void OnKey17();
	afx_msg void OnKey18();
	afx_msg void OnKey19();
	afx_msg void OnKey20();
	afx_msg void OnKey21();
	afx_msg void OnKey22();
	afx_msg void OnKey23();
	afx_msg void OnKey24();
	afx_msg void OnKey25();
	afx_msg void OnKey26();
	afx_msg void OnKey27();
	afx_msg void OnKey28();
	afx_msg void OnKey29();
	afx_msg void OnKey30();
	afx_msg void OnKey31();
	afx_msg void OnKey32();
	afx_msg void OnKey33();
	afx_msg void OnKey34();
	afx_msg void OnKey35();
	
	afx_msg void OnKeyBackspace();
	afx_msg void OnKeyCapsLock();
	afx_msg void OnKeyEnter();
	afx_msg void OnKeyShift();
	afx_msg void OnKeySpace();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENKEYBOARDDLG_H__5C515141_87E7_4574_A328_00DA90BF298E__INCLUDED_)
