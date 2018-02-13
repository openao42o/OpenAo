// AtumMonitorView.cpp : implementation of the CAtumMonitorView class
//

#include "stdafx.h"
#include "AtumMonitor.h"

#include "AtumMonitorDoc.h"
#include "AtumMonitorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorView

IMPLEMENT_DYNCREATE(CAtumMonitorView, CListView)

BEGIN_MESSAGE_MAP(CAtumMonitorView, CListView)
	//{{AFX_MSG_MAP(CAtumMonitorView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){   return 0;}
/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorView construction/destruction

CAtumMonitorView::CAtumMonitorView()
{
	// TODO: add construction code here

}

CAtumMonitorView::~CAtumMonitorView()
{
}

BOOL CAtumMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT | LVS_SHAREIMAGELISTS;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorView drawing

void CAtumMonitorView::OnDraw(CDC* pDC)
{
	CAtumMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
}

void CAtumMonitorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorView printing

BOOL CAtumMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAtumMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAtumMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorView diagnostics

#ifdef _DEBUG
void CAtumMonitorView::AssertValid() const
{
	CListView::AssertValid();
}

void CAtumMonitorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CAtumMonitorDoc* CAtumMonitorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAtumMonitorDoc)));
	return (CAtumMonitorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorView message handlers
void CAtumMonitorView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

int CAtumMonitorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	////////////////////////////////////////
	// Setting ImageLlist
	m_ImgSmallList.Create(IDB_TREE_ICON, 16,1, RGB(0,128,128));
	GetListCtrl().SetImageList(&m_ImgSmallList, TVSIL_NORMAL);

	GetListCtrl().InsertColumn(1, "Parameter", LVCFMT_LEFT, 100);
	GetListCtrl().InsertColumn(2, "Value", LVCFMT_LEFT, 100);
	GetListCtrl().InsertColumn(3, "Description", LVCFMT_LEFT, 300);

/*	LV_ITEM	lvItem;
	lvItem.iItem = GetListCtrl().GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iSubItem = 0;
	lvItem.iImage = 1;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.state = INDEXTOSTATEIMAGEMASK(1);

	lvItem.pszText = "FieldServer";
	GetListCtrl().InsertItem(&lvItem);
	lvItem.pszText = "NPCServer";
	GetListCtrl().InsertItem(&lvItem);
	lvItem.pszText = "LogServer";
	GetListCtrl().InsertItem(&lvItem);
*/
	return 0;
}
