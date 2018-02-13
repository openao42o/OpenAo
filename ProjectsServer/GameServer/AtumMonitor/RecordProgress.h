#if !defined(AFX_RECORDPROGRESS_H__44AB8DB4_9939_45F5_AA17_6206791B13C6__INCLUDED_)
#define AFX_RECORDPROGRESS_H__44AB8DB4_9939_45F5_AA17_6206791B13C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecordProgress dialog

class CRecordProgress : public CDialog
{
// Construction
public:
	CRecordProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordProgress)
	enum { IDD = IDD_IMPORT_EXCELFILE };
	CProgressCtrl	m_TotalProgress;
	CProgressCtrl	m_RecordProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDPROGRESS_H__44AB8DB4_9939_45F5_AA17_6206791B13C6__INCLUDED_)
