// selectgameserverdlg.cpp : implementation file
//

#include "stdafx.h"
#include "Atum.h"
#include "selectgameserverdlg.h"
#include "IExplore.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define EXE1_IMAGE_BACKGROUND_X_SIZE		650
#define EXE1_IMAGE_BACKGROUND_Y_SIZE		612

#define EXE1_IMAGE_BG_R1_C1_X				EXE1_IMAGE_BACKGROUND_X_SIZE	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_IMAGE_BG_R1_C1_Y				44								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정

#define EXE1_IMAGE_BG_R2_C1_Y				349								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정

#define EXE1_IMAGE_BG_R3_C1_X				326								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_IMAGE_BG_R3_C1_Y				139								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_IMAGE_BG_R3_C2_X				(EXE1_IMAGE_BACKGROUND_X_SIZE-EXE1_IMAGE_BG_R3_C1_X)	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_IMAGE_BG_R3_C2_Y				139								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정

#define EXE1_IMAGE_BG_R4_C1_X				EXE1_IMAGE_BACKGROUND_X_SIZE	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_IMAGE_BG_R4_C1_Y				80								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정

#define EXE1_POS_LB_1_X						24								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_POS_LB_1_Y						435								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_POS_LB_1_WIDTH					140								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_POS_LB_1_HEIGHT				94								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정

#define EXE1_POS_LB_2_X						175								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_POS_LB_2_Y						EXE1_POS_LB_1_Y					// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_POS_LB_2_WIDTH					EXE1_POS_LB_1_WIDTH				// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
#define EXE1_POS_LB_2_HEIGHT				EXE1_POS_LB_1_HEIGHT			// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정

/////////////////////////////////////////////////////////////////////////////
// CSelectGameServerDlg dialog


CSelectGameServerDlg::CSelectGameServerDlg(vectSGAME_SERVER_GROUP *i_pGameServerGroupList, CWnd* pParent /*=NULL*/)
	: CDialog(CSelectGameServerDlg::IDD, pParent), m_ctlLBLoginServer1(0, IDB_BITMAP_LOGINSERVER_LISTITEM_BG), m_ctlLBLoginServer2(0, IDB_BITMAP_LOGINSERVER_LISTITEM_BG)
{
	//{{AFX_DATA_INIT(CSelectGameServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGameServerGroupList			= i_pGameServerGroupList;

	util::zero(&m_selectedGameServerGroup, sizeof(m_selectedGameServerGroup));

	m_listBrush.CreateSolidBrush(RGB(29,29,40));
}


void CSelectGameServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectGameServerDlg)
	DDX_Control(pDX, IDC_LIST_LOGINSERVER_2, m_ctlLBLoginServer2);
	DDX_Control(pDX, IDC_LIST_LOGINSERVER_1, m_ctlLBLoginServer1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectGameServerDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectGameServerDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_LBN_DBLCLK(IDC_LIST_LOGINSERVER_1, OnDblclkListLoginserver1)
	ON_LBN_DBLCLK(IDC_LIST_LOGINSERVER_2, OnDblclkListLoginserver2)
	ON_WM_LBUTTONDOWN()
	ON_LBN_SETFOCUS(IDC_LIST_LOGINSERVER_1, OnSetfocusListLoginserver1)
	ON_LBN_SETFOCUS(IDC_LIST_LOGINSERVER_2, OnSetfocusListLoginserver2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectGameServerDlg message handlers



BOOL CSelectGameServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	this->MoveWindow2Center();

	int nServerCnts = m_pGameServerGroupList->size();
	
	RECT rtBG;
	this->GetClientRect(&rtBG);

	m_ctlLBLoginServer1.MoveWindow(EXE1_POS_LB_1_X, EXE1_POS_LB_1_Y, EXE1_POS_LB_1_WIDTH, EXE1_POS_LB_1_HEIGHT, TRUE);
	m_ctlLBLoginServer2.MoveWindow(EXE1_POS_LB_2_X, EXE1_POS_LB_2_Y, EXE1_POS_LB_2_WIDTH, EXE1_POS_LB_2_HEIGHT, TRUE);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 -
	int nPosX, nPosY;	
	int nTab1OrderIndex = 0;
	int nTab2OrderIndex = 0;
	for(int i=0; i < nServerCnts; i++)
	{
		SGAME_SERVER_GROUP *pLoginServerGroup = &( (*m_pGameServerGroupList)[i] );
		int nTabIndex	= pLoginServerGroup->usPreServerTab8OrderIndex/100;
		switch(nTabIndex)
		{
		case 0:
			{
				m_ctlLBLoginServer1.InsertItem(nTab1OrderIndex++, (*m_pGameServerGroupList)[i].szGameServerGroupName, 0, DEF_COL, RGB(180, 180, 180), 0);
			}
			break;
		default:
			{
				m_ctlLBLoginServer2.InsertItem(nTab2OrderIndex++, (*m_pGameServerGroupList)[i].szGameServerGroupName, 0, DEF_COL, RGB(180, 180, 180), 0);
			}
		}
	}
	
	CDC *pDC = GetDC();
	int nXScreen = EXE1_IMAGE_BACKGROUND_X_SIZE;
	int nYScreen = EXE1_IMAGE_BACKGROUND_Y_SIZE;
	m_BackGround.CreateCompatibleBitmap(pDC, nXScreen, nYScreen);

	CDC		memDCBackGround;	
	memDCBackGround.CreateCompatibleDC(pDC);
	CBitmap	*pOldBitmapBackGround = memDCBackGround.SelectObject(&m_BackGround);

	CBitmap tmBitmap;
	CBitmap *pTmOldBitmap;
	CDC tmMemDC;
// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 중국 Yetime 요청으로 수정함
//	// 2007-07-04 by cmkwon, 1. 배경화면 그리기
//	tmBitmap.LoadBitmap(IDB_BACKGROUND);
//	tmMemDC.CreateCompatibleDC(pDC);
//	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
//	nPosX = 0;
//	nPosY = 0;
//	memDCBackGround.BitBlt(nPosX, nPosY, EXE1_IMAGE_BACKGROUND_X_SIZE, EXE1_IMAGE_BACKGROUND_Y_SIZE, &tmMemDC, 0, 0, SRCCOPY);
//	tmMemDC.SelectObject(pTmOldBitmap);
//	tmBitmap.DeleteObject();

	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정, 제일 상위 이미지 그리기
	tmBitmap.LoadBitmap(IDB_BITMAP_LOGINSERVER_BG_R1_C1);
	tmMemDC.CreateCompatibleDC(pDC);
	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
	nPosX = 0;
	nPosY = 0;
	memDCBackGround.BitBlt(nPosX, nPosY, EXE1_IMAGE_BG_R1_C1_X, EXE1_IMAGE_BG_R1_C1_Y, &tmMemDC, 0, 0, SRCCOPY);
	tmMemDC.SelectObject(pTmOldBitmap);
	tmBitmap.DeleteObject();

	tmBitmap.LoadBitmap(IDB_BITMAP_LOGINSERVER_BG_R3_C1);
	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
	nPosX = 0;
	nPosY = EXE1_IMAGE_BG_R1_C1_Y + EXE1_IMAGE_BG_R2_C1_Y;
	memDCBackGround.BitBlt(nPosX, nPosY, EXE1_IMAGE_BG_R3_C1_X, EXE1_IMAGE_BG_R3_C1_Y, &tmMemDC, 0, 0, SRCCOPY);
	tmMemDC.SelectObject(pTmOldBitmap);
	tmBitmap.DeleteObject();

	tmBitmap.LoadBitmap(IDB_BITMAP_LOGINSERVER_BG_R4_C1);
	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
	nPosX = 0;
	nPosY = EXE1_IMAGE_BG_R1_C1_Y + EXE1_IMAGE_BG_R2_C1_Y + EXE1_IMAGE_BG_R3_C1_Y;
	memDCBackGround.BitBlt(nPosX, nPosY, EXE1_IMAGE_BG_R4_C1_X, EXE1_IMAGE_BG_R4_C1_Y, &tmMemDC, 0, 0, SRCCOPY);
	tmMemDC.SelectObject(pTmOldBitmap);
	tmBitmap.DeleteObject();

	// 2007-07-04 by cmkwon, 초기화
	memDCBackGround.SelectObject(pOldBitmapBackGround);

	///////////////////////////////////////////////////////////////////////////////
	RECT rt;
	rt.left		= -2;
	rt.top		= EXE1_IMAGE_BG_R1_C1_Y;
	rt.right	= EXE1_IMAGE_BACKGROUND_X_SIZE+1;
	rt.bottom	= EXE1_IMAGE_BG_R1_C1_Y + EXE1_IMAGE_BG_R2_C1_Y;
	m_pHost1 = new Host(m_hWnd, EXE1_URL_1, NULL, NULL, NULL, &rt);

	rt.left		= EXE1_IMAGE_BG_R3_C2_X;
	rt.top		= EXE1_IMAGE_BG_R1_C1_Y + EXE1_IMAGE_BG_R2_C1_Y;
	rt.right	= EXE1_IMAGE_BACKGROUND_X_SIZE;
	rt.bottom	= EXE1_IMAGE_BG_R1_C1_Y + EXE1_IMAGE_BG_R2_C1_Y + EXE1_IMAGE_BG_R3_C2_Y;
	m_pHost2 = new Host(m_hWnd, EXE1_URL_2, NULL, NULL, NULL, &rt);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSelectGameServerDlg::MoveWindow2Center(void)
/// \brief		// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정, 백그라운드 크기로 화면 중앙으로 이동 시키기
/// \author		cmkwon
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSelectGameServerDlg::MoveWindow2Center(void)
{
	LONG lSystemMenuSizeY = 0;	
	CMenu *pSysMenu = this->GetSystemMenu(FALSE);
	if(pSysMenu)
	{
		lSystemMenuSizeY = 19;
	}
	LONG x,y; 

	x=LONG((GetSystemMetrics(SM_CXSCREEN)-EXE1_IMAGE_BACKGROUND_X_SIZE)/2); 
	y=LONG((GetSystemMetrics(SM_CYSCREEN)-EXE1_IMAGE_BACKGROUND_Y_SIZE)/2);
	
	MoveWindow(x, y, EXE1_IMAGE_BACKGROUND_X_SIZE+6, lSystemMenuSizeY + EXE1_IMAGE_BACKGROUND_Y_SIZE+6, TRUE); 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SGAME_SERVER_GROUP *CSelectGameServerDlg::FindLoginServerByLoginServerName(char *i_pLoginServerName)
/// \brief		// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정
/// \author		cmkwon
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGAME_SERVER_GROUP *CSelectGameServerDlg::FindLoginServerByLoginServerName(char *i_pLoginServerName)
{
	if(NULL == m_pGameServerGroupList
		|| m_pGameServerGroupList->empty())
	{
		return NULL;
	}

	for(int i = 0; i < m_pGameServerGroupList->size(); i++)
	{
		if(0 == strnicmp(i_pLoginServerName, (*m_pGameServerGroupList)[i].szGameServerGroupName, SIZE_MAX_GAME_SERVER_GROUP_NAME))
		{
			return &(*m_pGameServerGroupList)[i];
		}
	}

	return NULL;
}

void CSelectGameServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

	}
	else
	{
		// TODO: Add your message handler code here

		///////////////////////////////////////////////////////////////////////////////
		// 2007-07-04 by cmkwon, EXE1 인터페이스 수정
		CPaintDC PaintDC (this);
		CDC		dcMem;
		BITMAP	stBitmap;

		dcMem.CreateCompatibleDC(&PaintDC);
		CBitmap *OldBitmap = dcMem.SelectObject(&m_BackGround);

		m_BackGround.GetObject(sizeof(BITMAP), &stBitmap);
		PaintDC.BitBlt(0, 0, stBitmap.bmWidth, stBitmap.bmHeight, &dcMem, 0, 0, SRCCOPY);
	}
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH CSelectGameServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		{
			
			if(m_ctlLBLoginServer1.GetSafeHwnd() == pWnd->m_hWnd
				|| m_ctlLBLoginServer2.GetSafeHwnd() == pWnd->m_hWnd)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetBkColor(RGB(29, 29, 40));
				pDC->SetTextColor(RGB(189,194,198));
				return m_listBrush;
			}
		}
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CSelectGameServerDlg::OnDblclkListLoginserver1() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_ctlLBLoginServer1.GetCurSel();

	CString szSelectedLoginServerName;
	m_ctlLBLoginServer1.GetTextString(nSel, szSelectedLoginServerName);

	SGAME_SERVER_GROUP *pLoginServer = FindLoginServerByLoginServerName((LPSTR)(LPCSTR)szSelectedLoginServerName);
	if(NULL == pLoginServer)
	{
		return;
	}

	m_selectedGameServerGroup = *pLoginServer;
	OnOK();
}

void CSelectGameServerDlg::OnDblclkListLoginserver2() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_ctlLBLoginServer2.GetCurSel();

	CString szSelectedLoginServerName;
	m_ctlLBLoginServer2.GetTextString(nSel, szSelectedLoginServerName);

	SGAME_SERVER_GROUP *pLoginServer = FindLoginServerByLoginServerName((LPSTR)(LPCSTR)szSelectedLoginServerName);
	if(NULL == pLoginServer)
	{
		return;
	}

	m_selectedGameServerGroup = *pLoginServer;
	OnOK();
}

void CSelectGameServerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);

	// 마우스로 끌어서 윈도우 이동
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}

void CSelectGameServerDlg::OnSetfocusListLoginserver1() 
{
	// TODO: Add your control notification handler code here

	// 2007-10-04 by cmkwon, 이전 선택 초기화
	m_ctlLBLoginServer2.SetCurSel(-1);
}

void CSelectGameServerDlg::OnSetfocusListLoginserver2() 
{
	// TODO: Add your control notification handler code here

	// 2007-10-04 by cmkwon, 이전 선택 초기화
	m_ctlLBLoginServer1.SetCurSel(-1);	
}
