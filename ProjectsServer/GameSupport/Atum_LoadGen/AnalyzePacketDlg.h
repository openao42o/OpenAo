#if !defined(AFX_ANALYZEPACKET_H__001B77FE_6B5C_4311_B8F7_39970A28D153__INCLUDED_)
#define AFX_ANALYZEPACKET_H__001B77FE_6B5C_4311_B8F7_39970A28D153__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnalyzePacketDlg.h : header file
//
#include "SystemLogManager.h"
#include "AnalyzePacket.h"

typedef vector<CString>		vectCString;

class CAnalyzePacketDlg : public CDialog
{
// Construction
public:
	CAnalyzePacketDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnalyzePacketDlg)
	enum { IDD = IDD_DLG_ANALYZE_PACKET };
	CString	m_ctlcsFilePath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalyzePacketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	BOOL dividePath(char *i_szPath, char *o_szDirectory, char *o_szFileNameExExtention);
	BOOL GetFilePathList(CString i_csSourDir, vectCString *o_pFilePathList);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnalyzePacketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnFindFile();
	afx_msg void OnBtnAnalyze();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALYZEPACKET_H__001B77FE_6B5C_4311_B8F7_39970A28D153__INCLUDED_)
