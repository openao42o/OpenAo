// AtumMonitorView.h : interface of the CAtumMonitorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMMONITORVIEW_H__50868F1F_5382_4737_82C1_407208C10D0B__INCLUDED_)
#define AFX_ATUMMONITORVIEW_H__50868F1F_5382_4737_82C1_407208C10D0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAtumMonitorDoc;

class CAtumMonitorView : public CListView
{
protected: // create from serialization only
	CAtumMonitorView();
	DECLARE_DYNCREATE(CAtumMonitorView)

// Attributes
public:
	CAtumMonitorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumMonitorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CImageList m_ImgSmallList;
	virtual ~CAtumMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAtumMonitorView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AtumMonitorView.cpp
inline CAtumMonitorDoc* CAtumMonitorView::GetDocument()
   { return (CAtumMonitorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMMONITORVIEW_H__50868F1F_5382_4737_82C1_407208C10D0B__INCLUDED_)
