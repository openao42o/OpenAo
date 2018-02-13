// ACEonlineLauncherPpg.cpp : Implementation of the CACEonlineLauncherPropPage property page class.

#include "stdafx.h"
#include "ACEonlineLauncher.h"
#include "ACEonlineLauncherPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CACEonlineLauncherPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CACEonlineLauncherPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CACEonlineLauncherPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CACEonlineLauncherPropPage, "ACEONLINELAUNCHER.ACEonlineLauncherPropPage.1",
	0xd8a28a21, 0xae7b, 0x496a, 0xa0, 0xa4, 0x59, 0x7d, 0x46, 0xc9, 0xa1, 0x61)


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherPropPage::CACEonlineLauncherPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CACEonlineLauncherPropPage

BOOL CACEonlineLauncherPropPage::CACEonlineLauncherPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ACEONLINELAUNCHER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherPropPage::CACEonlineLauncherPropPage - Constructor

CACEonlineLauncherPropPage::CACEonlineLauncherPropPage() :
	COlePropertyPage(IDD, IDS_ACEONLINELAUNCHER_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CACEonlineLauncherPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherPropPage::DoDataExchange - Moves data between page and properties

void CACEonlineLauncherPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CACEonlineLauncherPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonlineLauncherPropPage message handlers
