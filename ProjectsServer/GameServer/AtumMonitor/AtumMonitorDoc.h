// AtumMonitorDoc.h : interface of the CAtumMonitorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMMONITORDOC_H__8477C70F_5FEF_4080_AF57_744DC2B1870A__INCLUDED_)
#define AFX_ATUMMONITORDOC_H__8477C70F_5FEF_4080_AF57_744DC2B1870A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAtumMonitorDoc : public CDocument
{
protected: // create from serialization only
	CAtumMonitorDoc();
	DECLARE_DYNCREATE(CAtumMonitorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumMonitorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAtumMonitorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAtumMonitorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMMONITORDOC_H__8477C70F_5FEF_4080_AF57_744DC2B1870A__INCLUDED_)
