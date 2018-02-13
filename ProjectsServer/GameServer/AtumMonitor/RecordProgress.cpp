// RecordProgress.cpp : implementation file
//

#include "stdafx.h"
#include "atummonitor.h"
#include "RecordProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordProgress dialog


CRecordProgress::CRecordProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRecordProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordProgress)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecordProgress, CDialog)
	//{{AFX_MSG_MAP(CRecordProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordProgress message handlers
