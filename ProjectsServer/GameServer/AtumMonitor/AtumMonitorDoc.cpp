// AtumMonitorDoc.cpp : implementation of the CAtumMonitorDoc class
//

#include "stdafx.h"
#include "AtumMonitor.h"

#include "AtumMonitorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorDoc

IMPLEMENT_DYNCREATE(CAtumMonitorDoc, CDocument)

BEGIN_MESSAGE_MAP(CAtumMonitorDoc, CDocument)
	//{{AFX_MSG_MAP(CAtumMonitorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorDoc construction/destruction

CAtumMonitorDoc::CAtumMonitorDoc()
{
	// TODO: add one-time construction code here

}

CAtumMonitorDoc::~CAtumMonitorDoc()
{
}

BOOL CAtumMonitorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorDoc serialization

void CAtumMonitorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorDoc diagnostics

#ifdef _DEBUG
void CAtumMonitorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAtumMonitorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorDoc commands
