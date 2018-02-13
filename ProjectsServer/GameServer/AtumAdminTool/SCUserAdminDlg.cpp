// SCUserAdminDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCUserAdminDlg.h"
#include "AtumParam.h"
#include "SelectItemDlg.h"
#include "ModifyItemDlg.h"
#include "atumprotocol.h"
#include "atumsj.h"
#include "characterinfodlg.h"
#include "resource.h"
#include "AtumAdminToolDlg.h"
#include "AtumAdminTool.h"
#include "AtumDBLogWriter.h"
#include "AccountInfoDlg.h"
#include "BlockAccountDlg.h"
#include "PremiumInfoDlg.h"
#include "BlockAccountListDlg.h"
#include "InflWarListDlg.h"
#include "DeletedCharacter.h"
#include "dlgsetinfluencerate.h"		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 

#include "PartnerDlg.h" // 2012-03-15 by jhseol, 파트너 - 해더 포함

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char *GetCPState(int i_nCPState)
{
	switch(i_nCPState) {
	case CP_NOTCONNECTED:				return STRERR_S_SCADMINTOOL_0014;
	case CP_CONNECTED:					return STRERR_S_SCADMINTOOL_0015;
	case CP_SINGLE_FILE_VERSIONCHECKED:	return STRERR_S_SCADMINTOOL_0016;
	case CP_VERSIONCHECKED:				return STRERR_S_SCADMINTOOL_0016;
	case CP_LOGINED:					return STRERR_S_SCADMINTOOL_0017;
	case CP_FIELD_LOGINED:				return STRERR_S_SCADMINTOOL_0018;
	case CP_GAMESTARTED:				return STRERR_S_SCADMINTOOL_0019;
	}

	return STRERR_S_SCADMINTOOL_0020;
}
/////////////////////////////////////////////////////////////////////////////
// CSCUserAdminDlg dialog

CSCUserAdminDlg::CSCUserAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCUserAdminDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCUserAdminDlg)
	m_szAccountNameInput = _T("");
	m_szAccountName = _T("");
	m_CharacterName1 = _T("");
	m_CharacterName2 = _T("");
	m_CharacterName3 = _T("");
	m_szUsetStatus = _T("");
	m_nAccCharac = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pUserAdminPreSocket = NULL;
	m_pODBCStmt = new CODBCStatement;

	memset(&m_CurrentCharacter, 0, sizeof(CHARACTER));
	memset(&m_InflLeaderCharacter, 0, sizeof(CHARACTER));
	memset(&m_InflSub1LeaderCharacter, 0, sizeof(CHARACTER));
	memset(&m_InflSub2LeaderCharacter, 0, sizeof(CHARACTER));

	m_bIsValidAccountName	= FALSE;
	m_uidAccountUID			= 0;

	memset(&m_blockedAccountInfo, 0x00, sizeof(m_blockedAccountInfo));

	m_vectCurrentCharacterGetItemList.clear();		// 2012-03-15 by jhseol, 파트너 - 선택된 캐릭터가 보유한 아이템 리스트 초기화
}

CSCUserAdminDlg::~CSCUserAdminDlg()
{
	util::del(m_pODBCStmt);
}

void CSCUserAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCUserAdminDlg)
	DDX_Text(pDX, IDC_EDIT_USER_INPUT, m_szAccountNameInput);
	DDX_Text(pDX, IDC_EDIT_ACCNAME, m_szAccountName);
	DDX_Text(pDX, IDC_EDIT_CHARAC_NAME_1, m_CharacterName1);
	DDX_Text(pDX, IDC_EDIT_CHARAC_NAME_2, m_CharacterName2);
	DDX_Text(pDX, IDC_EDIT_CHARAC_NAME_3, m_CharacterName3);
	DDX_Text(pDX, IDC_EDIT_POSITION, m_szUsetStatus);
	DDX_Radio(pDX, IDC_RADIO_ACCOUNT, m_nAccCharac);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_ITEM, m_GridItem);
	DDX_GridControl(pDX, IDC_GRID_CHARACTER,m_GridCharacter);
}

BEGIN_MESSAGE_MAP(CSCUserAdminDlg, CDialog)
	//{{AFX_MSG_MAP(CSCUserAdminDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_COMMAND(ID_CMG_INSERT_ITEM, OnCmgInsertItem)
	ON_COMMAND(ID_CMG_DELETE_ITEM, OnCmgDeleteItem)
	ON_COMMAND(ID_CMG_MODIFY_ITEM, OnCmgModifyItem)
	ON_BN_CLICKED(IDC_BUTTON_GET_USER_INFO, OnButtonGetUserInfo)
	ON_BN_CLICKED(IDC_BUTTON_CHARAC_1_OK, OnButtonCharac1Ok)
	ON_BN_CLICKED(IDC_BUTTON_CHARAC_2_OK, OnButtonCharac2Ok)
	ON_BN_CLICKED(IDC_BUTTON_CHARAC_3_OK, OnButtonCharac3Ok)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, OnButtonReload)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_CHARACTER, OnButtonEditCharacter)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BLOCK_ACCOUNT, OnButtonBlockAccount)
	ON_BN_CLICKED(IDC_BUTTON_UNBLOCK_ACCOUNT, OnButtonUnblockAccount)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_ACCOUNT, OnButtonEditAccount)
	ON_BN_CLICKED(IDC_BUTTON_BLOCK_EDIT, OnButtonBlockEdit)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PREMIUM, OnButtonEditPremium)
	ON_COMMAND(ID_CMG_ITEM_INFORMATION, OnCmgItemInformation)
	ON_BN_CLICKED(IDC_BTN_BLOCKED_LIST, OnBtnBlockedList)
	ON_BN_CLICKED(IDC_BTN_INFLWAR_LIST, OnBtnInflwarList)
	ON_BN_CLICKED(IDC_BUTTON_DELETED_CHARACTER, OnButtonDeletedCharacter)
	ON_BN_CLICKED(IDC_BTN_INIT_INFLUENCE, OnBtnInitInfluence)
	ON_BN_CLICKED(IDC_BTN_SET_InfluenceRate, OnBTNSETInfluenceRate)
	ON_BN_CLICKED(IDC_BTN_INFINITY_RESET_IMPUTE, OnBtnInfinityResetImpute)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, IDC_GRID_ITEM, OnGridRClick)
	ON_NOTIFY(NM_CLICK, IDC_GRID_ITEM, OnGridClick)
	ON_MESSAGE(WM_PRE_PACKET_NOTIFY, OnSocketNotifyPre)
//	ON_MESSAGE(WM_IM_PACKET_NOTIFY, OnSocketNotifyIM)
//	ON_MESSAGE(WM_FIELD_PACKET_NOTIFY, OnSocketNotifyField)
	ON_MESSAGE(WM_PRE_ASYNC_EVENT, OnAsyncSocketMessage)
	ON_MESSAGE(WM_IM_ASYNC_EVENT, OnAsyncSocketMessage)
	ON_MESSAGE(WM_FIELD_ASYNC_EVENT, OnAsyncSocketMessage)
	ON_NOTIFY(NM_DBLCLK, IDC_GRID_ITEM, OnGridDClick)		// 2012-03-15 by jhseol, 파트너 - 더블클릭 이벤트 추가
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCUserAdminDlg message handlers

BOOL CSCUserAdminDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

//	// Set the icon for this dialog.  The framework does this automatically
//	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	InitGridItem();
	InitGridCharacter();

	EnableAllOuputControls(FALSE);
	ResetControls();
	//GetDlgItem(IDC_EDIT_USER_INPUT)->SetFocus();

	// 2013-01-07 by khkim, 베트남 GM 권한수정
	#ifdef S_MANAGER_ADMIN_HSSON
	UserManagementToolControls();		
	#endif
	// end 2012-01-07 by khkim, 베트남 GM 권한수정

	// Make socket instance & connect
	m_pUserAdminPreSocket = new CSCAdminPreWinSocket("CSCUserAdminDlg's PreServer Socket", this, GetSafeHwnd());
	if (!m_pUserAdminPreSocket->Connect(m_pMainDlg->m_pServerInfo4Admin->ServerIP, PRE_SERVER_PORT))
	{
		int err = GetLastError();
		MessageBox(STRERR_S_SCADMINTOOL_0005);
		util::del(m_pUserAdminPreSocket);
		EndDialog(-1);
		return FALSE;
	}

	// Connect DB
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
	)
	{
		char szTemp[1024];
		sprintf(szTemp, STRERR_S_SCADMINTOOL_0021
			, m_pMainDlg->m_pServerInfo4Admin->DBName, m_pMainDlg->m_pServerInfo4Admin->DBIP
			, m_pMainDlg->m_pServerInfo4Admin->DBPort);
		MessageBox(szTemp);
		EndDialog(-1);
		return FALSE;
	}
	
	// 데이타 로딩
	CAtumDBHelper::LoadItemInfo(m_pODBCStmt, &m_mapItemInfo, &m_mapItemInfoName);
	CAtumDBHelper::LoadRareItemInfo(m_pODBCStmt, &m_mapRareItemInfo, &m_mapRareItemInfoName);
	CAtumDBHelper::LoadEnchantInfo(m_pODBCStmt,&m_mapEnchantInfo);		// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완

	ReadProfile();
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSCUserAdminDlg::OnPaint() 
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

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSCUserAdminDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSCUserAdminDlg::ReadProfile()
{
	((CAtumAdminToolApp*)AfxGetApp())->ReadProfile();

	m_szAccountNameInput	= ((CAtumAdminToolApp*)AfxGetApp())->m_szAccountNameInputReg;
}

void CSCUserAdminDlg::WriteProfile()
{
	((CAtumAdminToolApp*)AfxGetApp())->m_szAccountNameInputReg	= m_szAccountNameInput;	

	((CAtumAdminToolApp*)AfxGetApp())->WriteProfile();
}

LONG CSCUserAdminDlg::OnSocketNotifyPre(WPARAM wParam, LPARAM lParam)
{
	CSCAdminWinSocket *pSCAdminWinSocket = (CSCAdminWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				// 연결 성공
//				MessageBox("Pre Server에 연결하였습니다.");
				INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_CONNECT, T_PA_ADMIN_CONNECT, msgConnect, msgConnectBuf);
				util::strncpy(msgConnect->UID, m_pMainDlg->m_UID, SIZE_MAX_ACCOUNT_NAME);
				util::strncpy(msgConnect->PWD, m_pMainDlg->m_PWD, SIZE_MAX_PASSWORD);
				m_pUserAdminPreSocket->Write(msgConnectBuf, MSG_SIZE(MSG_PA_ADMIN_CONNECT));
			}
			else
			{
				// 연결 실패
				MessageBox(STRERR_S_SCADMINTOOL_0006);
				OnClose();
			}
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			MessageType_t	msgType;

			char			*pPacket = NULL;
			int				len;
			pSCAdminWinSocket->Read(&pPacket, len);

			if (pPacket)
			{
				msgType = *(MessageType_t*)(pPacket);

				switch(msgType)
				{
				case T_PA_ADMIN_CONNECT_OK:
					{
						MSG_PA_ADMIN_CONNECT_OK *msgConnectOK
							= (MSG_PA_ADMIN_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

// 2006-04-15 by cmkwon
//						if (!msgConnectOK->AuthOK)
						if (0 == msgConnectOK->AccountType0)
						{
							MessageBox("Fail PreServer2 certification!!");
							OnCancel();
						}
					}
					break;
				case T_PA_ADMIN_GET_ACCOUNT_INFO_OK:
					{
						MSG_PA_ADMIN_GET_ACCOUNT_INFO_OK *msgGetOK
							= (MSG_PA_ADMIN_GET_ACCOUNT_INFO_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CString szUserState;

						m_bCurrentCharacterOnline = msgGetOK->IsOnline;
						if (msgGetOK->IsOnline)
						{
							if (msgGetOK->AccountInfo.ClientState == CP_FIELD_LOGINED)
							{
								szUserState.Format(STRMSG_S_SCADMINTOOL_0034,
												GetCPState(msgGetOK->AccountInfo.ClientState),
												msgGetOK->AccountInfo.CurrentServerGroup);
							}
							else if (msgGetOK->AccountInfo.ClientState == CP_GAMESTARTED)
							{
								szUserState.Format(STRMSG_S_SCADMINTOOL_0035,
												msgGetOK->AccountInfo.CurrentCharacterName,
												msgGetOK->AccountInfo.CurrentServerGroup,
												msgGetOK->AccountInfo.CurrentMapChannel.MapIndex,
												msgGetOK->AccountInfo.CurrentMapChannel.ChannelIndex);
							}
						}
						else
						{
							szUserState = "OFFLINE";
						}

						m_bCurrentAccountBlocked	= msgGetOK->IsBlocked;
						m_blockedAccountInfo		= msgGetOK->BlockedAccountInfo;
						if (msgGetOK->IsBlocked)
						{
							szUserState += STRMSG_S_SCADMINTOOL_0036;
						}


						EnableControls_GetAccountInfo( msgGetOK );
						/*
						GetDlgItem(IDC_BUTTON_BLOCK_ACCOUNT)->EnableWindow(!msgGetOK->IsBlocked);
						GetDlgItem(IDC_BUTTON_DELETED_CHARACTER)->EnableWindow(msgGetOK->IsBlocked);
						GetDlgItem(IDC_BUTTON_BLOCK_EDIT)->EnableWindow(msgGetOK->IsBlocked);
						GetDlgItem(IDC_BUTTON_UNBLOCK_ACCOUNT)->EnableWindow(msgGetOK->IsBlocked);
						GetDlgItem(IDC_BUTTON_EDIT_ACCOUNT)->EnableWindow(msgGetOK->IsBlocked && m_bIsValidAccountName);
						

						if (strnicmp(m_CurrentCharacter.AccountName, msgGetOK->AccountInfo.AccountName, SIZE_MAX_ACCOUNT_NAME) == 0)
						{
							GetDlgItem(IDC_BUTTON_EDIT_CHARACTER)->EnableWindow(msgGetOK->IsBlocked);
// 2006-04-15 by cmkwon
//							GetDlgItem(IDC_BUTTON_EDIT_PREMIUM)->EnableWindow(msgGetOK->IsBlocked);
						}
						*/

						GetDlgItem(IDC_EDIT_POSITION)->SetWindowText(szUserState);
					}
					break;
				case T_PA_ADMIN_BLOCK_ACCOUNT_OK:
					{
						// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
						MSG_PA_ADMIN_BLOCK_ACCOUNT_OK *pRMsg = (MSG_PA_ADMIN_BLOCK_ACCOUNT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						if(0 == m_szAccountName.CompareNoCase(pRMsg->szBlockedAccountName))
						{// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 두 계정이 같을 때에만 아래 프로토콜을 요청한다.

							// pre server에 상태 질의
							INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_ACCOUNT_INFO, T_PA_ADMIN_GET_ACCOUNT_INFO, msgGetCharacter, msgGetCharacterBuf);
							util::strncpy(msgGetCharacter->AccountName, m_szAccountName, SIZE_MAX_ACCOUNT_NAME);
							util::strncpy(msgGetCharacter->CharacterName, "", SIZE_MAX_CHARACTER_NAME);
							m_pUserAdminPreSocket->Write(msgGetCharacterBuf, MSG_SIZE(MSG_PA_ADMIN_GET_ACCOUNT_INFO));
						}
					}
					break;
				case T_PA_ADMIN_UNBLOCK_ACCOUNT_OK:
					{
						// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
						MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK *pRMsg = (MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						if(0 == m_szAccountName.CompareNoCase(pRMsg->AccountName))
						{// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 두 계정이 같을 때에만 아래 프로토콜을 요청한다.
							// pre server에 상태 질의
							INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_ACCOUNT_INFO, T_PA_ADMIN_GET_ACCOUNT_INFO, msgGetCharacter, msgGetCharacterBuf);
							util::strncpy(msgGetCharacter->AccountName, m_szAccountName, SIZE_MAX_ACCOUNT_NAME);
							util::strncpy(msgGetCharacter->CharacterName, "", SIZE_MAX_CHARACTER_NAME);
							m_pUserAdminPreSocket->Write(msgGetCharacterBuf, MSG_SIZE(MSG_PA_ADMIN_GET_ACCOUNT_INFO));
						}
					}
					break;
				default:
					{
					}
					break;
				}
			}

			util::del(pPacket);
		}
		break;
	}	// end of switch

	return 0;
}

/*
//LONG CSCUserAdminDlg::OnSocketNotifyIM(WPARAM wParam, LPARAM lParam)
//{
//	CSCAdminWinSocket *pSCUserAdminWinSocket = (CSCUserAdminWinSocket*)lParam;
//
//	switch(LOWORD(wParam))
//	{
//	case CWinSocket::WS_ERROR:
//		{
//		}
//		break;
//	case CWinSocket::WS_CONNECTED:
//		{
//			if (HIWORD(wParam) == TRUE)
//			{
//				// 연결 성공
//				//MessageBox("서버에 연결하였습니다.");
//				INIT_MSG_WITH_BUFFER(MSG_IA_ADMIN_CONNECT, T_IA_ADMIN_CONNECT, msgConnect, msgConnectBuf);
//				strncpy(msgConnect->UID, "admin", SIZE_MAX_ACCOUNT_NAME);
//				strncpy(msgConnect->PWD, "passwd", SIZE_MAX_PASSWORD);
//				m_pUserAdminPreSocket->Write(msgConnectBuf, MSG_SIZE(MSG_IA_ADMIN_CONNECT));
//			}
//			else
//			{
//				// 연결 실패
//				//MessageBox("서버에 연결할 수 없습니다.");
//			}
//		}
//		break;
//	case CWinSocket::WS_RECEIVED:
//		{
//			MessageType_t	msgType;
//
//			char			*pPacket = NULL;
//			int				len;
//			pSCUserAdminWinSocket->Read(&pPacket, len);
//
//			if (pPacket)
//			{
//				msgType = *(MessageType_t*)(pPacket);
//
//				switch(msgType)
//				{
//				case T_IA_ADMIN_CONNECT_OK:
//					{
//						MSG_IA_ADMIN_CONNECT_OK *msgConnectOK
//							= (MSG_IA_ADMIN_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);
//
//						if (!msgConnectOK->AuthOK)
//						{
//							MessageBox("인증 실패하였습니다");
//							OnCancel();
//						}
//					}
//					break;
//				case T_IA_ADMIN_GET_CHARACTER_OK:
//					{
//						MSG_IA_ADMIN_GET_CHARACTER_OK *msgGetOK
//							= (MSG_IA_ADMIN_GET_CHARACTER_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);
//
//						CString szUserState;
//						if (msgGetOK->IsOnline)
//						{
//							szUserState = m_szCurrentCharacterName + " ONLINE";
//						}
//						else
//						{
//							szUserState = m_szCurrentCharacterName + " OFFLINE";
//						}
//
//						GetDlgItem(IDC_EDIT_POSITION)->SetWindowText(szUserState);
//					}
//					break;
//				default:
//					{
//					}
//					break;
//				}
//			}
//
//			util::del(pPacket);
//		}
//		break;
//	}	// end of switch
//
//	return 0;
//}
//*/


/*
LONG CSCUserAdminDlg::OnSocketNotifyField(WPARAM wParam, LPARAM lParam)
{
	CSCAdminWinSocket *pSCUserAdminWinSocket = (CSCAdminWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				// 연결 성공
				//MessageBox("서버에 연결하였습니다.");
				INIT_MSG_WITH_BUFFER(MSG_IA_ADMIN_CONNECT, T_IA_ADMIN_CONNECT, msgConnect, msgConnectBuf);
				strncpy(msgConnect->UID, "admin", SIZE_MAX_ACCOUNT_NAME);
				strncpy(msgConnect->PWD, "passwd", SIZE_MAX_PASSWORD);
				m_pUserAdminPreSocket->Write(msgConnectBuf, MSG_SIZE(MSG_IA_ADMIN_CONNECT));
			}
			else
			{
				// 연결 실패
				//MessageBox("서버에 연결할 수 없습니다.");
			}
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			MessageType_t	msgType;

			char			*pPacket = NULL;
			int				len;
			m_pUserAdminFieldSocket->Read(&pPacket, len);

			if (pPacket)
			{
				msgType = *(MessageType_t*)(pPacket);

				switch(msgType)
				{
				case T_IA_ADMIN_CONNECT_OK:
					{
						MSG_IA_ADMIN_CONNECT_OK *msgConnectOK
							= (MSG_IA_ADMIN_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						if (msgConnectOK->AccountType0 != 128)
						{
							MessageBox("관리자 권한이 아닙니다.");
							OnCancel();
						}
					}
					break;
				case T_IA_ADMIN_GET_CHARACTER_OK:
					{
						MSG_IA_ADMIN_GET_CHARACTER_OK *msgGetOK
							= (MSG_IA_ADMIN_GET_CHARACTER_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CString szUserState;
						if (msgGetOK->IsOnline)
						{
							szUserState = m_szCurrentCharacterName + " ONLINE";
						}
						else
						{
							szUserState = m_szCurrentCharacterName + " OFFLINE";
						}

						GetDlgItem(IDC_EDIT_POSITION)->SetWindowText(szUserState);
					}
					break;
				default:
					{
					}
					break;
				}
			}

			util::del(pPacket);
		}
		break;
	}	// end of switch

	return 0;
}
*/


LONG CSCUserAdminDlg::OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam)
{
	m_pUserAdminPreSocket->OnAsyncEvent(lParam);

	return 0;
}

void CSCUserAdminDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}

void CSCUserAdminDlg::InitGridCharacter()
{
	m_GridCharacter.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 2;

	m_GridCharacter.SetEditable(FALSE);
	m_GridCharacter.SetListMode(TRUE);
// 2007-12-06 by cmkwon, SCAdminTool 수정(그리드 멀티라인 선택가능하게 수정) -
//	m_GridCharacter.SetSingleRowSelection(TRUE);
	m_GridCharacter.EnableSelection(TRUE);
	m_GridCharacter.SetFrameFocusCell(FALSE);
	m_GridCharacter.SetTrackFocusCell(FALSE);

	m_GridCharacter.SetRowCount(m_nRows);
	m_GridCharacter.SetColumnCount(m_nCols);
	m_GridCharacter.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = m_nCols++;
	Item.strText.Format("Classification");
	m_GridCharacter.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Value");
	m_GridCharacter.SetItem(&Item);

	// arrange grid
	m_GridCharacter.AutoSize();
	m_GridCharacter.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridCharacter.GetRowCount()-1, m_GridCharacter.GetColumnCount()-1);
	m_GridCharacter.ClearCells(tmpCellRange);
}

void CSCUserAdminDlg::InitGridItem()
{
	m_GridItem.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 14;		// 2009-11-18 by cmkwon, AdminTool에서 시간제한 아이템의 경과된 정보 보기 추가 - , // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - // 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	int m_nFixRows = 1;
//	int m_nFixCols = 1;
	
	m_GridItem.SetEditable(FALSE);
	m_GridItem.SetListMode(TRUE);
// 2007-12-06 by cmkwon, SCAdminTool 수정(그리드 멀티라인 선택가능하게 수정) -
//	m_GridItem.SetSingleRowSelection(TRUE);
	m_GridItem.EnableSelection(TRUE);
	m_GridItem.SetHeaderSort(FALSE);
	m_GridItem.SetFrameFocusCell(FALSE);
	m_GridItem.SetTrackFocusCell(FALSE);
//	m_GridItem.EnableDragAndDrop(TRUE);
//	m_GridItem.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

	m_GridItem.SetRowCount(m_nRows);
	m_GridItem.SetColumnCount(m_nCols);
	m_GridItem.SetFixedRowCount(m_nFixRows);
	//m_GridItem.SetFixedColumnCount(m_nFixCols);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	Item.col = m_nCols++;
	Item.strText.Format(_T("Count"));
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Item UID"));
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Item Name"));
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Item Num"));
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Rare Prefix"));
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Rare Suffix"));
	m_GridItem.SetItem(&Item);

	// 2005-11-26 by cmkwon, 추가함
	Item.col = m_nCols++;
	Item.strText.Format(_T("Enchant Count"));
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Attach Info"));
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Item Position");
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Counts"));
	m_GridItem.SetItem(&Item);
// 2005-12-08 by cmkwon, 사용하지 않는 필드임
//	Item.col = m_nCols++;
//	Item.strText.Format(_T("Endurance"));
//	m_GridItem.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format(_T("EffectItemNum"));	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ColorItemNum 을 수정
	m_GridItem.SetItem(&Item);

	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	Item.col = m_nCols++;
	Item.strText.Format(_T("ShapeItemNum"));
	m_GridItem.SetItem(&Item);

	// 2009-11-18 by cmkwon, AdminTool에서 시간제한 아이템의 경과된 정보 보기 추가 - 
	Item.col = m_nCols++;
	Item.strText.Format(_T("RemainTime(s)"));
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Created Time"));
	m_GridItem.SetItem(&Item);

	// fill rows/cols with text
/*
	for (int row = 1; row < m_GridItem.GetRowCount(); row++) {
		for (int col = 0; col < m_GridItem.GetColumnCount(); col++) { 
			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			if (row < m_nFixRows) {
//				Item.nFormat = DT_LEFT|DT_WORDBREAK;
//				Item.strText.Format(_T("Col %d"),col);
//			} else if (col < m_nFixCols) {
//				Item.nFormat = dwTextStyle;
//				Item.strText.Format(_T("Row %d"),row);
			} else {
				Item.nFormat = dwTextStyle;
				Item.strText.Format(_T(" "));
			}
			m_GridItem.SetItem(&Item);  


		}
	}
*/

	// Make cell 1,1 read-only
	//m_GridItem.SetItemState(1,1, m_GridItem.GetItemState(1,1) | GVIS_READONLY);

	m_GridItem.AutoSize();
	//m_GridItem.SetRowHeight(0, 3*m_GridItem.GetRowHeight(0)/2);
	m_GridItem.ExpandColumnsToFit();
	//m_GridItem.ExpandToFit(FALSE);

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridItem.GetRowCount()-1, m_GridItem.GetColumnCount()-1);
	m_GridItem.ClearCells(tmpCellRange);
}

// NM_RCLICK
void CSCUserAdminDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	// 2013-01-07 by khkim, 베트남 GM 권한수정
#ifdef S_MANAGER_ADMIN_HSSON
	if(RACE_GAMEMASTER == m_pMainDlg->m_usManagerAccountType)
	{
		return ;
	}
#endif
	// end 2012-01-07 by khkim, 베트남 GM 권한수정
	UpdateData();

    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (m_szCurrentCharacterName == "")
	{
		return;
	}

	if (pItem->iRow == 0)
	{
		return;
	}

	m_GridItem.SetFocusCell(-1,-1);
	m_GridItem.SetSelectedRange(pItem->iRow, 0, pItem->iRow, m_GridItem.GetColumnCount()-1, TRUE, TRUE);
	m_GridItem.SetFocusCell(pItem->iRow, pItem->iColumn);

// 2007-12-06 by cmkwon, 필요 없는 코드임
//	if (m_bCurrentCharacterOnline)
//	{
//		return;
//	}

	// show context menu
	CMenu muTemp, *pContextMenu;
	muTemp.LoadMenu(IDR_CONTEXT_MENU_GRID_ITEM);
	CPoint point;
	GetCursorPos(&point);

	if(FALSE == m_bCurrentAccountBlocked
		|| FALSE == m_pMainDlg->IsManagerAdministrator())	// 2006-04-15 by cmkwon, 관리자 권한이 아니면 아이템 수정,추가,삭제 불가
	{
		pContextMenu = muTemp.GetSubMenu(1);
	}
	else
	{
		pContextMenu = muTemp.GetSubMenu(0);
	}
	pContextMenu->EnableMenuItem(ID_CMG_INSERT_ITEM, FALSE);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

// NM_CLICK
void CSCUserAdminDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (pItem->iRow != 0)
	{
		return;
	}

	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 숫자 정렬 컬럼 수정
	switch(pItem->iColumn) {
	case 0:
	case 1:		
	case 3:
	case 6:
	case 8:
	case 9:
		m_GridItem.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);
		break;
	default:
		m_GridItem.SetCompareFunction(CGridCtrl::pfnCellTextCompare);
	}

    if (pItem->iColumn == m_GridItem.GetSortColumn())
        m_GridItem.SortItems(pItem->iColumn, !m_GridItem.GetSortAscending());
    else
        m_GridItem.SortItems(pItem->iColumn, TRUE);

	m_GridItem.AutoSize();
	m_GridItem.ExpandColumnsToFit();
}

// start 2012-03-15 by jhseol, 파트너 - 파트너 더블클릭
void CSCUserAdminDlg::OnGridDClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	OutputDebugString("[SEOL Debuging Check] : Mouse D Click. \r\n");
	CCellID cellID = m_GridItem.GetFocusCell();
	if(0 > cellID.row)
	{// 2005-12-12 by cmkwon, 선택 오류
		return;
	}
	
	ITEM_GENERAL_READABLE itemToMod;
	GetItemGeneralInRow(cellID.row, &itemToMod);

	CString tmpItemNum, tmpPartnerNum;
	tmpItemNum.Format(_T("%d"), itemToMod.ItemNum);
	tmpPartnerNum.Format(_T("%d"), ITEM_NUM_PARTNER);
	if(strstr(tmpItemNum, tmpPartnerNum))						// 2012-03-15 by jhseol, 파트너 - 더블클릭한 아이템이 파트너 류에만 반응
	{

		if(FALSE == m_bCurrentAccountBlocked
			|| FALSE == m_pMainDlg->IsManagerAdministrator())	// 2012-03-15 by jhseol, 파트너 - 관리자 권한과 계정 블럭상태 확인
		{
			MessageBox(STRERR_S_SCADMINTOOL_0024);
//			MessageBox("You don't have access privileges!");
		}

		else
		{
			CPartnerDlg PartnerDlg((CAtumAdminToolDlg*)AfxGetMainWnd(), this, itemToMod.UniqueNumber);
			if (IDOK != PartnerDlg.DoModal())
			{
				return;
			}
		}
	}
	GetItemInfo(m_szCurrentCharacterName);
}
// end 2012-03-15 by jhseol, 파트너 - 파트너 더블클릭

BOOL CSCUserAdminDlg::InsertItemToDB(const char *i_szCharacterName, int i_nItemNum, int i_nItemCount)
{
	/*[Stored Query Definition]************************************************
	-- !!!!
	-- Name: dbo.atum_admin_InsertStoreItemCountable_DB
	-- Desc:
	--			-- // 2009-08-25 by cmkwon, 사용하지 않는 필드 제거(td_Store.ScarcityNumber) - ScarcityNumber 필드 제거
	--			-- // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - td_Store.ShapeItemNum 필드 추가, 필드리스트 명시
	-- ====
	**************************************************************************/
	BOOL bRet=FALSE;
	CString tmpItemNum, tmpPartnerNum;
	tmpItemNum.Format(_T("%d"), i_nItemNum);
	tmpPartnerNum.Format(_T("%d"), ITEM_NUM_PARTNER);
	if(strstr(tmpItemNum, tmpPartnerNum))						// 2012-03-15 by jhseol, 파트너 - 더블클릭한 아이템이 파트너 류에만 반응
	{
		int iCBCount = 0;
		int iDefault = 0;
		int iItemWindowIndex = 99;
		int iPetIndex = 1000+((i_nItemNum/10)-723000);	// 2012-03-15 by jhseol 임시, 파트너 - 파트너 인덱스를 임시로 계산하는 부분 추후에 ItemNum와 PetIndex설정 패턴이 바뀌면 수정해야함.
		
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_INTEGER,	0, 0, &m_uidAccountUID/*AccountUniqueNumber*/,	0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_INTEGER,	0, 0, &m_CurrentCharacter.CharacterUniqueNumber/*Possess*/,	0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_TINYINT,	0, 0, &iDefault/*ItemStorage*/,					0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_TINYINT,	0, 0, &iDefault/*Wear*/,						0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &i_nItemCount/*CurrentCount*/,			0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &iItemWindowIndex/*ItemWindowIndex*/,		0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &i_nItemNum/*ItemNum*/,					0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_SMALLINT,	0, 0, &iDefault/*NumOfEnchants*/,				0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &iDefault/*PrefixCodeNum*/,				0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &iDefault/*SuffixCodeNum*/,				0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_FLOAT,		0, 0, &iDefault/*CurrentEndurance*/,			0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &iDefault/*ColorCode*/,					0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &iDefault/*UsingTimeStamp*/,				0, NULL); iCBCount++;
		SQLBindParameter(m_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	0, 0, &iPetIndex/*PetIndex*/,					0, NULL); iCBCount++;
		// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		///////////////////////////////////////////////////////////////////////////////////////
		bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_100615_0571));
	}
	else
	{

	SQLINTEGER cb1 = SQL_NTS;
// 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 아래와 같이 수정
// 	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];	
// 	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER, 0, GetSqlPattern((LPCSTR)m_szCurrentCharacterName, spbuf), 0, &cb1);
	char szTmCharName[SIZE_MAX_CHARACTER_NAME];
	util::strncpy(szTmCharName, (LPCSTR)m_szCurrentCharacterName, SIZE_MAX_CHARACTER_NAME);
 	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, szTmCharName, 0, &cb1);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nItemNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nItemCount, 0, NULL);

		bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080822_0025));
	}
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox(STRERR_S_SCADMINTOOL_0022);
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 아래오 같이 추가
	SQLHSTMT	hstmt	= m_pODBCStmt->GetSTMTHandle();
	UID64_t		ItemUID	= 0;
	SQLRETURN	ret		= 0;
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(ItemUID), 0, NULL);	
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	m_pODBCStmt->FreeStatement();	// clean up

	if(FALSE == IS_VALID_UID64(ItemUID))
	{// 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 체크
		MessageBox(STRERR_S_SCADMINTOOL_0022);
		return FALSE;
	}

	// write log
	FL_ITEM_LOG_BASE logBase;
	logBase.CharacterUniqueNumber = m_CurrentCharacter.CharacterUniqueNumber;
	logBase.CurrentMapIndex = 0;
	ITEM_FOR_LOG logItem;
	logItem.ItemNum = i_nItemNum;
	logItem.ItemUniqueNumber = ItemUID;		// 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 추가된 ItemUID 할당.
	logItem.CurrentCount = 0;

	CAtumDBLogWriter::InsertLog_Item_Trade(m_pODBCStmt, T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN, &logBase, &logItem, i_nItemCount, (char*)(LPCSTR)m_pMainDlg->m_UID, 0);

	return TRUE;
}

BOOL CSCUserAdminDlg::GetAccountBlockState(const char *i_szAccountName)
{
	CString szQuery;
	// 2008-01-29 by cmkwon, T_A: 계정 블럭 관련 쿼리 수정
	//szQuery.Format("select * from td_BlockedAccounts WITH (NOLOCK) where accountname like \'%s\'", i_szAccountName);
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("select * from td_BlockedAccounts WITH (NOLOCK) where accountname = \'%s\'", i_szAccountName);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 	
//	szQuery.Format(QUERY_080702_0031, i_szAccountName);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, (char*)i_szAccountName, 0,		&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0031));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();
		MessageBox(STRERR_S_SCADMINTOOL_0023);
		return FALSE;
	}

	int nCount = 0;
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		nCount++;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	if (nCount <= 0)
	{
		return FALSE;
	}
	else if (nCount >= 1)
	{
		return TRUE;
	}

	ASSERT_NEVER_GET_HERE();

	return FALSE;
}

void CSCUserAdminDlg::SendBlockAccount(const char *i_szAccName, int i_nBlockTy
									   , ATUM_DATE_TIME i_atimeS, ATUM_DATE_TIME i_atimeE
									   , const char *i_szAdminName, const char *i_szReasonForUser, const char *i_szReasonForOnlyAdmin)
{
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_BLOCK_ACCOUNT, T_PA_ADMIN_BLOCK_ACCOUNT, pBlock, SendBuf);
	util::strncpy(pBlock->szBlockedAccountName, i_szAccName, SIZE_MAX_ACCOUNT_NAME);
	pBlock->enBlockedType			= (EN_BLOCKED_TYPE)i_nBlockTy;
	pBlock->atimeStartTime			= i_atimeS;
	pBlock->atimeEndTime			= i_atimeE;
	util::strncpy(pBlock->szBlockAdminAccountName, i_szAdminName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(pBlock->szBlockedReasonForUser, i_szReasonForUser, SIZE_MAX_BLOCKED_ACCOUNT_REASON);
	util::strncpy(pBlock->szBlockedReasonForOnlyAdmin, i_szReasonForOnlyAdmin, SIZE_MAX_BLOCKED_ACCOUNT_REASON);		// 2007-01-10 by cmkwon
	m_pUserAdminPreSocket->Write(SendBuf, MSG_SIZE(MSG_PA_ADMIN_BLOCK_ACCOUNT));
}

//void CSCUserAdminDlg::SendUnblockAccount(const char *i_szAccName)
void CSCUserAdminDlg::SendUnblockAccount(const char *i_szAccName, const char *i_szAdminName /* = NULL */ )	// 2012-06-05 by mspark, 계정 블럭 ID와 계정 언블럭 ID 구분 작업
{
// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 아래와 같이 수정 함
// 	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_UNBLOCK_ACCOUNT, T_PA_ADMIN_UNBLOCK_ACCOUNT, msgUnblock, msgUnblockBuf);
// 	util::strncpy(msgUnblock->AccountName, i_szAccName, SIZE_MAX_ACCOUNT_NAME);
// 	m_pUserAdminPreSocket->Write(msgUnblockBuf, MSG_SIZE(MSG_PA_ADMIN_UNBLOCK_ACCOUNT));

	if(FALSE == m_bCurrentAccountBlocked
		|| 0 != strnicmp(m_blockedAccountInfo.szBlockedAccountName, m_szAccountName, SIZE_MAX_ACCOUNT_NAME))
	{
		return;
	}
 	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_UNBLOCK_ACCOUNT, T_PA_ADMIN_UNBLOCK_ACCOUNT, msgUnblock, msgUnblockBuf); 	
	util::strncpy(msgUnblock->szBlockedAccountName, i_szAccName, SIZE_MAX_ACCOUNT_NAME);
	msgUnblock->enBlockedType			= m_blockedAccountInfo.enBlockedType;
	msgUnblock->atimeStartTime			= m_blockedAccountInfo.atimeStartTime;
	msgUnblock->atimeEndTime			= m_blockedAccountInfo.atimeEndTime;
	//util::strncpy(msgUnblock->szBlockAdminAccountName, m_blockedAccountInfo.szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME);
	// 2012-06-05 by mspark, 계정 블럭 ID와 계정 언블럭 ID 구분 작업
	if(i_szAdminName)
	{
		util::strncpy(msgUnblock->szBlockAdminAccountName, i_szAdminName, SIZE_MAX_ACCOUNT_NAME);
	}
	// end 2012-06-05 by mspark, 계정 블럭 ID와 계정 언블럭 ID 구분 작업
	util::strncpy(msgUnblock->szBlockedReasonForUser, m_blockedAccountInfo.szBlockedReasonForUser, SIZE_MAX_BLOCKED_ACCOUNT_REASON);
	util::strncpy(msgUnblock->szBlockedReasonForOnlyAdmin, m_blockedAccountInfo.szBlockedReasonForOnlyAdmin, SIZE_MAX_BLOCKED_ACCOUNT_REASON);		// 2007-01-10 by cmkwon
 	m_pUserAdminPreSocket->Write(msgUnblockBuf, MSG_SIZE(MSG_PA_ADMIN_UNBLOCK_ACCOUNT));
}

void CSCUserAdminDlg::OnCmgInsertItem() 
{
	// TODO: Add your command handler code here
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{// 2006-04-15 by cmkwon, 관리자 권한 체크
		MessageBox("Permission denied !!");
		return;
	}
	if (m_bCurrentCharacterOnline && !m_bCurrentAccountBlocked)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0024);
		return;
	}

	CSelectItemDlg dlg(&m_mapItemInfo, &m_mapItemInfoName, TRUE);
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 다시 체크
	if(FALSE == this->IsEnabledEdit())
	{
		AfxMessageBox("Now, permission denied !!");
		return;
	}

	INT nItemNum = dlg.m_nSelectedItemNum;
	INT nItemCount = dlg.m_nItemCount;

	ITEM *pItemInfo = m_mapItemInfo.findEZ_ptr(nItemNum);

	if (nItemNum == MONEY_ITEM_NUMBER)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0025);
		return;
	}

	if (IS_COUNTABLE_ITEM(pItemInfo->Kind)
		&& FindItemByItemNum(nItemNum))
	{
		MessageBox(STRERR_S_SCADMINTOOL_0026);
		return;
	}

	if (!IS_COUNTABLE_ITEM(pItemInfo->Kind))
	{
		///////////////////////////////////////////
		// Countable Item이 아닌 경우
		///////////////////////////////////////////
		for (int i = 0; i < nItemCount; i++)
		{
			if (IS_WEAPON(pItemInfo->Kind))
			{
				InsertItemToDB((LPCSTR)m_szCurrentCharacterName, nItemNum, pItemInfo->Charging);
			}
			else
			{
				InsertItemToDB((LPCSTR)m_szCurrentCharacterName, nItemNum, 1);
			}
		}
	}
	else
	{
		///////////////////////////////////////////
		// Countable Item인 경우
		///////////////////////////////////////////
		InsertItemToDB((LPCSTR)m_szCurrentCharacterName, nItemNum, nItemCount);
	}

	// 다시 아이템을 업데이트함
	GetItemInfo(m_szCurrentCharacterName);
}

BOOL CSCUserAdminDlg::FindItemByItemNum(int i_nItemNum)
{
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	CString szQuery;
// // 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// //	szQuery.Format("select * from td_store s, td_character c WITH (NOLOCK) where itemnum = %d and s.possess = c.uniquenumber and c.charactername like \'%s\'",
// //					i_nItemNum, (LPCSTR)m_szCurrentCharacterName);
// 	szQuery.Format(QUERY_080702_0032, i_nItemNum, (LPCSTR)m_szCurrentCharacterName);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
// 	if (!bRet)
// 	{
// 		MessageBox(STRERR_S_SCADMINTOOL_0027);
// 		m_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	int nCount = 0;
// 	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
// 	{
// 		nCount++;
// 	}
// 
// 	// clean up
// 	m_pODBCStmt->FreeStatement();
// 
// 	if (nCount <= 0)
// 	{
// 		return FALSE;
// 	}
// 	else if (nCount >= 1)
// 	{
// 		return TRUE;
// 	}
// 
// 	ASSERT_NEVER_GET_HERE();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, (LPSTR)(LPCSTR)m_szCurrentCharacterName, 0,	&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_nItemNum), 0,													&arrCB2[2]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0032));
	if(FALSE == bRet)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0027);
		m_pODBCStmt->FreeStatement();
		return FALSE;
	}
	int nItemCount = 0;
	arrCB2[1] = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_LONG, &(nItemCount), 0, &arrCB2[1]);
	while(1)
	{
		SQLRETURN ret = SQLFetch(hstmt);
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
	}
	m_pODBCStmt->FreeStatement();

	if(0 >= nItemCount)
	{
		return FALSE;
	}
	return TRUE;
}

void CSCUserAdminDlg::OnCmgDeleteItem() 
{
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{// 2006-04-15 by cmkwon, 관리자 권한 체크
		MessageBox("Permission denied !!");
		return;
	}
	if (m_bCurrentCharacterOnline && !m_bCurrentAccountBlocked)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0024);
		return;
	}

	CCellID cellID = m_GridItem.GetFocusCell();
	if(0 > cellID.row)
	{// 2005-12-12 by cmkwon, 선택 오류
		return;
	}

	ITEM_GENERAL_READABLE itemToMod;
	GetItemGeneralInRow(cellID.row, &itemToMod);

	if (itemToMod.ItemNum == MONEY_ITEM_NUMBER
		&& ITEM_IN_CHARACTER == itemToMod.ItemStorage)		// 2005-12-08 by cmkwon
	{
		MessageBox(STRERR_S_SCADMINTOOL_0028);
		return;
	}

	if (IDYES != AfxMessageBox(STRERR_S_SCADMINTOOL_0029, MB_YESNO))
	{
		return;
	}

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 한번더 체크
	if(FALSE == this->IsEnabledEdit())
	{
		AfxMessageBox("Now, permission denied !!");
		return;
	}

	//	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("delete from td_store where uniquenumber = %I64d", itemToMod.UniqueNumber);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	szQuery.Format(QUERY_080702_0033, itemToMod.UniqueNumber);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	BOOL bRet;

	CString tmpItemNum, tmpPartnerNum;					
	// start - 2012-03-15 by jhseol, 파트너 - 삭제 아이템이 소켓아이템이고 파트너에 장착중일때 삭제불가능
	int ttttttt = itemToMod.ItemWindowIndex;
	tmpItemNum.Format(_T("%d"), itemToMod.ItemNum);
	tmpPartnerNum.Format(_T("%d"), 70325);
	if(itemToMod.ItemWindowIndex == 98 && strstr(tmpItemNum, tmpPartnerNum))
	{
		MessageBox(STRERR_S_SCADMINTOOL_0048);
		return;
	}
	// end - 2012-03-15 by jhseol, 파트너 - 삭제 아이템이 소켓아이템이고 파트너에 장착중일때 삭제불가능
	
	// start - 2012-03-15 by jhseol, 파트너 - 삭제 아이탬이 파트너일때 박혀있는 소켓도 같이 삭제
	tmpItemNum.Format(_T("%d"), itemToMod.ItemNum);
	tmpPartnerNum.Format(_T("%d"), ITEM_NUM_PARTNER);
	if(strstr(tmpItemNum, tmpPartnerNum))						// 2012-03-15 by jhseol, 파트너 - 아이템이 파트너 류에만 반응
	{
		SQLINTEGER arrCB2 = SQL_NTS;
		SQLBindParameter ( m_pODBCStmt->m_hstmt , 1 , SQL_PARAM_INPUT , SQL_C_UBIGINT , SQL_BIGINT , 0 , 0 , &itemToMod.UniqueNumber , 0 , NULL );
		/************************************************************
		-------------------------------------------------------------
		-----관련 프로시저
		-- // 2010-06-15 by shcho&hslee 펫시스템 - 개인 펫 정보 로딩 
		-- // 2011-08-05 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		-------------------------------------------------------------
		CREATE PROC [dbo].[atum_Load_td_petInfo]
		@i_ItemUID BIGINT
		AS
		BEGIN
		SELECT ItemUID, PetIndex, Name, [Level], Experience, ExpRatio, EnableSocketCount,
		SocketItem1, SocketItem2, SocketItem3, SocketItem4, SocketItem5, SocketItem6,
		HPKitItemNum, HPKitTriggerValue, ShieldKitItemNum, ShieldKitTriggerValue, SPKitItemNum, SPKitTriggerValue,
		AutoSkilItemNum1, AutoSkilItemNum2, AutoSkilItemNum3, AutoSkilItemNum4, AutoSkilItemNum5, AutoSkilItemNum6 
		FROM dbo.td_pet  WHERE ItemUID = @i_ItemUID;
		END
		GO
		*************************************************************/
		RETCODE bRet = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_100615_0573, SQL_NTS);
		if ( bRet != SQL_SUCCESS && bRet != SQL_SUCCESS_WITH_INFO )
		{
			MessageBox("ERROR : PROCEDURE_100615_0573");
			return ;
		}
		PARTNER_INFO tTempPartnerInfo;
		util::zero(&tTempPartnerInfo, sizeof(PARTNER_INFO));
		SQLINTEGER cb[25];
		int iIndex = 0, iArrIndex = 0;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_SBIGINT,	&tTempPartnerInfo.ItemUID,				0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&tTempPartnerInfo.PartnerIndex,			0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_CHAR,		 tTempPartnerInfo.name,					SIZE_MAX_PET_NAME, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&tTempPartnerInfo.level,				0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&tTempPartnerInfo.Experience,			0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&tTempPartnerInfo.ExpRatio,				0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&tTempPartnerInfo.EnableSocketCount,	0, &cb[iIndex]);	iIndex++;
		for(iArrIndex = 0; iArrIndex < SIZE_MAX_PETSOCKET; iArrIndex++)
		{
			SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,	&tTempPartnerInfo.SocketItem[iArrIndex],0, &cb[iIndex]);	iIndex++;
		}
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&tTempPartnerInfo.HPKitItemNum,			0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_FLOAT,		&tTempPartnerInfo.HPKitTriggerValue,	0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&tTempPartnerInfo.ShieldKitItemNum,		0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_FLOAT,		&tTempPartnerInfo.ShieldKitTriggerValue,0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&tTempPartnerInfo.SPKitItemNum,			0, &cb[iIndex]);	iIndex++;
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_FLOAT,		&tTempPartnerInfo.SPKitTriggerValue,	0, &cb[iIndex]);	iIndex++;
		for(iArrIndex = 0; iArrIndex < SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; iArrIndex++)
		{
			SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,	&tTempPartnerInfo.AutoSkilItemNum[iArrIndex],0, &cb[iIndex]);	iIndex++;
		}
		bRet = SQLFetch(m_pODBCStmt->m_hstmt);
		if ( ( bRet != SQL_SUCCESS && bRet != SQL_SUCCESS_WITH_INFO ) || bRet == SQL_NO_DATA )
		{
			SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
			MessageBox("ERROR : SQLFetch");
			return ;
		}
		SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
		
		for(iArrIndex = 0; iArrIndex < SIZE_MAX_PETSOCKET; iArrIndex++)
		{
			if(PARTNER_SOCKET_NON_USE != tTempPartnerInfo.SocketItem[iArrIndex] ||
				PARTNER_SOCKET_EMPTY != tTempPartnerInfo.SocketItem[iArrIndex])
			{
				SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_BIGINT, 0, 0, &tTempPartnerInfo.SocketItem[iArrIndex], 0,		NULL);
				/*[Query Definition]************************************************
				--------------------------------------------------------------------------------
				-- PROCEDURE NAME	: dbo.atum_PROCEDURE_080827_0033
				-- DESC				: -- // 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
				--------------------------------------------------------------------------------
				ALTER PROCEDURE [dbo].[atum_PROCEDURE_080827_0033]
				@i_ItemUID					BIGINT
				-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
				AS
				DELETE
				FROM td_Store
				WHERE uniquenumber = @i_ItemUID;
				Go
				*******************************************************************/	
				RETCODE ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080827_0033, SQL_NTS);
				if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
				{
					MessageBox(STRERR_S_SCADMINTOOL_0030);
					SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
					return;
				}
				SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
				
				for (int i = 0; i < m_vectCurrentCharacterGetItemList.size(); i++)
				{
					if (tTempPartnerInfo.SocketItem[iArrIndex] == m_vectCurrentCharacterGetItemList[i].UniqueNumber)
					{
						FL_ITEM_LOG_BASE logBase;
						logBase.CharacterUniqueNumber = m_CurrentCharacter.CharacterUniqueNumber;
						logBase.CurrentMapIndex = 0;
						ITEM_FOR_LOG logItem;
						logItem.ItemNum = m_vectCurrentCharacterGetItemList[i].ItemNum;
						logItem.ItemUniqueNumber = m_vectCurrentCharacterGetItemList[i].UniqueNumber;	
						logItem.CurrentCount = m_vectCurrentCharacterGetItemList[i].CurrentCount;
						CAtumDBLogWriter::InsertLog_Item_Trade(m_pODBCStmt, T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN, &logBase, &logItem, 0, (char*)(LPCSTR)m_pMainDlg->m_UID, 0);
					}
				}
				// write log  :  아이템 삭제 로그
			}
		}
	}
	// end - 2012-03-15 by jhseol, 파트너 - 삭제 아이탬이 파트너일때 박혀있는 소켓도 같이 삭제
	
/*
	// start -2012-03-15 by jhseol, 파트너 - 파트너 테이블의 정보도 같이 삭제 하기 위해 기존 소스 블럭 처리
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &(itemToMod.UniqueNumber), 0,		&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0033));
	if (!bRet)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0030);
		m_pODBCStmt->FreeStatement();	// cleanup
		return;
	}
	m_pODBCStmt->FreeStatement();		// cleanup
	// end -2012-03-15 by jhseol, 파트너 - 파트너 테이블의 정보도 같이 삭제 하기 위해 기존 소스 블럭 처리
	*/
	
	// start -2012-03-15 by jhseol, 파트너 - 파트너 테이블의 정보도 같이 삭제 하기 위해 추가된 루틴
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,
		&m_uidAccountUID, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,
		&itemToMod.UniqueNumber, 0, NULL);
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- Name:	atum_DeleteStoreItem
	-- Desc:	아템 삭제
	--	// 2012-08-09 by jhseol, 파트너 기능강화 : 아템스왑 - 펫아이템 삭제시 관련정보도 같이 삭제
	-- ====
	ALTER PROCEDURE [dbo].[atum_DeleteStoreItem]
	@accNum	INT,
	@uniqueNum BIGINT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
	DELETE FROM td_Enchant
	WHERE TargetItemUniqueNumber = @uniqueNum
	
	  DELETE FROM td_Store
	  WHERE AccountUniqueNumber = @accNum AND UniqueNumber = @uniqueNum;
	  
		DELETE FROM td_pet
		WHERE ItemUID = @uniqueNum; -- // 2010-06-15 by shcho&hslee 펫시스템 - 펫 정보 삭제 추가
		
		  DECLARE @PetSocketItem1 BIGINT
		  DECLARE @PetSocketItem2 BIGINT
		  DECLARE @PetSocketItem3 BIGINT
		  DECLARE @PetSocketItem4 BIGINT
		  DECLARE @PetSocketItem5 BIGINT
		  DECLARE @PetSocketItem6 BIGINT
		  SELECT @PetSocketItem1=SocketItem1, @PetSocketItem2=SocketItem2, @PetSocketItem3=SocketItem3, @PetSocketItem4=SocketItem4, @PetSocketItem5=SocketItem5, @PetSocketItem6=SocketItem6	FROM td_pet WHERE ItemUID = @uniqueNum
		  DELETE FROM td_PetSwapItemInfo	-- // 2012-08-09 by jhseol, 파트너 기능강화 : 아템스왑 - 펫아이템 삭제시 관련정보도 같이 삭제
		  WHERE SocketItemUID IN (@PetSocketItem1,@PetSocketItem2,@PetSocketItem3,@PetSocketItem4,@PetSocketItem5,@PetSocketItem6, @uniqueNum)
		  GO
	**************************************************************************/
	RETCODE ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0098, SQL_NTS);
	
	if (ret != SQL_SUCCESS 
		&& ret != SQL_SUCCESS_WITH_INFO
		&& ret != SQL_NO_DATA)
	{
		SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
	// end -2012-03-15 by jhseol, 파트너 - 파트너 테이블의 정보도 같이 삭제 하기 위해 추가된 루틴


	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-26 by cmkwon, 인챈트 정보 삭제하기
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("delete from td_Enchant where TargetItemUniqueNumber = %I64d", itemToMod.UniqueNumber);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 	
//	szQuery.Format(QUERY_080702_0034, itemToMod.UniqueNumber);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &(itemToMod.UniqueNumber), 0,		&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0034));
	if (!bRet)
	{
		MessageBox("Error : Delete Enchant");		
		m_pODBCStmt->FreeStatement();	// cleanup
		return;
	}
	m_pODBCStmt->FreeStatement();		// cleanup





	GetItemInfo(m_szCurrentCharacterName);	// 다시 아이템을 업데이트함

	// write log
	FL_ITEM_LOG_BASE logBase;
	logBase.CharacterUniqueNumber = m_CurrentCharacter.CharacterUniqueNumber;
	logBase.CurrentMapIndex = 0;
	ITEM_FOR_LOG logItem;
	logItem.ItemNum = itemToMod.ItemNum;
	logItem.ItemUniqueNumber = itemToMod.UniqueNumber;
	logItem.CurrentCount = itemToMod.CurrentCount;

	CAtumDBLogWriter::InsertLog_Item_Trade(m_pODBCStmt, T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN, &logBase, &logItem, 0, (char*)(LPCSTR)m_pMainDlg->m_UID, 0);
	return;
}

void CSCUserAdminDlg::OnCmgModifyItem() 
{
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{// 2006-04-15 by cmkwon, 관리자 권한 체크
		MessageBox("Permission denied !!");
		return;
	}
	if (m_bCurrentCharacterOnline && !m_bCurrentAccountBlocked)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0024);
		return;
	}

	CCellID cellID = m_GridItem.GetFocusCell();
	if(0 > cellID.row)
	{// 2005-12-12 by cmkwon, 선택 오류
		return;
	}

	ITEM_GENERAL_READABLE itemToMod;
	GetItemGeneralInRow(cellID.row, &itemToMod);

	// 2008-06-19 by cmkwon, AdminTool 수정(기어별 인챈트 추가 할수 있게) - 
	itemToMod.ItemInfo = m_mapItemInfo.findEZ_ptr(itemToMod.ItemNum);
	if(NULL == itemToMod.ItemInfo)
	{
		MessageBox("Invalid ItemNum !!");
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-24 by cmkwon, AdminTool수정:해당 아이템의 인챈트 리스트 보여주기 - CSCUserAdminDlg::OnCmgModifyItem#
	vectINT enchantItemNumList;
	vectENCHANT		enchantInfo;	// 2013-02-27 by bckim, 인챈트 리스트순서 재정렬(아이템넘버순서->인챈트된순서)
	if(FALSE == this->DBQ_LoadEnchantList(&enchantItemNumList, itemToMod.UniqueNumber, itemToMod.Kind,&enchantInfo))
	{
		return;
	}

	CModifyItemDlg	dlg(this);
	dlg.m_nItemNum				= itemToMod.ItemNum;
	dlg.m_szItemName			= itemToMod.ItemName;
	dlg.m_nItemCount			= itemToMod.CurrentCount;
	dlg.m_nItemPrefixCodeNum	= itemToMod.PrefixCodeNum;
	dlg.m_nItemSuffixCodeNum	= itemToMod.SuffixCodeNum;
	dlg.m_ctl_nEnchantCount		= itemToMod.EnchantCount;
	dlg.m_nArmorColorItemNum	= itemToMod.ColorCode;			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - EffectItemNum으로 사용	// 2005-12-08 by cmkwon
	dlg.m_nShapeItemNum			= itemToMod.ShapeItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가
	dlg.m_nItemKind				= itemToMod.Kind;
	dlg.m_usReqUnitKind			= itemToMod.ItemInfo->ReqUnitKind;	// 2008-06-19 by cmkwon, AdminTool 수정(기어별 인챈트 추가 할수 있게) - 
	dlg.m_vectEnchantItemNumList	= enchantItemNumList;			// 2010-02-24 by cmkwon, AdminTool수정:해당 아이템의 인챈트 리스트 보여주기 - 
	dlg.m_vectEnchantOrder			= enchantInfo;					// 2013-02-27 by bckim, 인챈트 리스트순서 재정렬(아이템넘버순서->인챈트된순서)
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 다시 체크
	if(FALSE == this->IsEnabledEdit())
	{
		AfxMessageBox("Now, permission denied !!");
		return;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_PROCEDURE_080827_0035
	-- DESC				: -- // 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	--						-- // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가, 인자 추가
	--------------------------------------------------------------------------------
	************************************************************************/
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("update td_store set CurrentCount = %d, ColorCode = %d,\
// 						PrefixCodeNum = %d, SuffixCodeNum = %d where UniqueNumber = %I64d",
// 						dlg.m_nItemCount, dlg.m_nArmorColorItemNum, dlg.m_nItemPrefixCodeNum,
// 						dlg.m_nItemSuffixCodeNum, itemToMod.UniqueNumber);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0035,		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//		dlg.m_nItemCount, dlg.m_nArmorColorItemNum, dlg.m_nItemPrefixCodeNum,
//		dlg.m_nItemSuffixCodeNum, itemToMod.UniqueNumber);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &(itemToMod.UniqueNumber), 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_nItemCount), 0,				&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_nArmorColorItemNum), 0,		&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_nItemPrefixCodeNum), 0,		&arrCB2[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_nItemSuffixCodeNum), 0,		&arrCB2[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_nShapeItemNum), 0,			&arrCB2[6]);	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가, 프로시저 인자 추가

	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0035));
	if (!bRet)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0031);
		m_pODBCStmt->FreeStatement();		// cleanup
	}
	m_pODBCStmt->FreeStatement();	// cleanup
	if(dlg.m_nAddEnchantItemNum)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery.Format("insert td_Enchant(TargetItemUniqueNumber, TargetItemNum, EnchantItemNum) Values(%I64d, %d, %d)"
		//	, itemToMod.UniqueNumber, itemToMod.ItemNum, dlg.m_nAddEnchantItemNum);

		// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
		//szQuery.Format(QUERY_080702_0036		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//	, itemToMod.UniqueNumber, itemToMod.ItemNum, dlg.m_nAddEnchantItemNum);
		//BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
		hstmt = m_pODBCStmt->GetSTMTHandle();

		///////////////////////////////////////////////////////////////////////////////
		// 2010-02-24 by cmkwon, AdminTool수정:인챈트 추가 개수 지정 할 수 있게 - 다음과 같이 추가 개수 만큼 호출
		for(int i=0; i < dlg.m_ctl_nInsertEnchatCount; i++)
		{
			arrCB2[1] = arrCB2[2] = arrCB2[3] = SQL_NTS;
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &(itemToMod.UniqueNumber), 0,		&arrCB2[1]);
			SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(itemToMod.ItemNum), 0,				&arrCB2[2]);
			SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(dlg.m_nAddEnchantItemNum), 0,		&arrCB2[3]);
			bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0036));
			if (!bRet)
			{
				MessageBox("Error : Add Enchant Error");
			}
			m_pODBCStmt->FreeStatement();	// cleanup
		}
#ifdef S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM		// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능
		// 아이템 인챈트시 로그 남기기 
		if (bRet)
		{				
			FL_ITEM_LOG_BASE logBase;
			logBase.CharacterUniqueNumber = m_CurrentCharacter.CharacterUniqueNumber;
			logBase.CurrentMapIndex = 0;
			ITEM_FOR_LOG logItem;
			logItem.ItemNum = itemToMod.ItemNum;
			logItem.ItemUniqueNumber = itemToMod.UniqueNumber;
			logItem.CurrentCount = dlg.m_ctl_nEnchantCount + dlg.m_ctl_nInsertEnchatCount;	
			
			CAtumDBLogWriter::InsertLog_Item_Enchant_Change_By_Admin(m_pODBCStmt, T1_FL_LOG_ITEM_ENCHANT_BY_ADMIN, &logBase, &logItem,
											dlg.m_nAddEnchantItemNum, dlg.m_ctl_nInsertEnchatCount, logItem.CurrentCount, (char*)(LPCSTR)m_pMainDlg->m_UID);
		}
#endif

	}

#ifdef S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM		// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능
	if(dlg.m_bDisEnchantAll || dlg.m_nSubEnchantItemNum )
	{
		INT nDisEnchantItemNum	= 0;
		INT nDisEnchantCount	= 0;
		vectSUB_ENCHANT_INFO::iterator	itr_SubEnchant = dlg.m_vectSubEnchantItemInfo.begin();
		while( itr_SubEnchant != dlg.m_vectSubEnchantItemInfo.end() )
		{
			if( dlg.m_bDisEnchantAll )
			{
				 nDisEnchantItemNum	= itr_SubEnchant->nSubEnchantItemNum;
				 nDisEnchantCount	= itr_SubEnchant->nEnchantCount;
			}
			else
			{
				nDisEnchantItemNum	= dlg.m_nSubEnchantItemNum;
				nDisEnchantCount	= dlg.m_ctl_nRemoveEnchantCount;
			}
			
			hstmt = m_pODBCStmt->GetSTMTHandle();
			arrCB2[1] = arrCB2[2] = arrCB2[3] = SQL_NTS;
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &(itemToMod.UniqueNumber), 0,&arrCB2[1]);
			SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(nDisEnchantCount), 0,	&arrCB2[2]);
			SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(nDisEnchantItemNum), 0,&arrCB2[3]);
			bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_121121_0001));	
			if (!bRet)
			{
				MessageBox("Error : Sub Enchant Error");
			}
			else
			{				
				FL_ITEM_LOG_BASE logBase;
				logBase.CharacterUniqueNumber = m_CurrentCharacter.CharacterUniqueNumber;
				logBase.CurrentMapIndex = 0;
				ITEM_FOR_LOG logItem;
				logItem.ItemNum = itemToMod.ItemNum;
				logItem.ItemUniqueNumber = itemToMod.UniqueNumber;
				logItem.CurrentCount = 	dlg.m_nOldItemDisEnchantCount - nDisEnchantCount;
				// 아이템 디스인챈트시 로그 남기기 
				CAtumDBLogWriter::InsertLog_Item_Enchant_Change_By_Admin(m_pODBCStmt, T1_FL_LOG_ITEM_DISENCHANT_BY_ADMIN, &logBase, &logItem,
					nDisEnchantItemNum, nDisEnchantCount, logItem.CurrentCount, (char*)(LPCSTR)m_pMainDlg->m_UID);
			}
			m_pODBCStmt->FreeStatement();	// cleanup

			if( FALSE == dlg.m_bDisEnchantAll ) { break; }		// 전체삭제 
			else
			{
				dlg.m_nOldItemDisEnchantCount -= nDisEnchantCount;		// 전체 삭제일때
				itr_SubEnchant++;
			}
		}
	}
#endif

	// 다시 아이템을 업데이트함
	GetItemInfo(m_szCurrentCharacterName);

	return;
}

BYTE GetWear(CString i_szWear)
{
	if (i_szWear == STRMSG_S_SCAT_COLNAME_0021)
		return WEAR_ATTACHED;
	else if (i_szWear == STRMSG_S_SCAT_COLNAME_0025)
		return WEAR_NOT_ATTACHED;

	return WEAR_NOT_ATTACHED;
}

BOOL CSCUserAdminDlg::GetItemGeneralInRow(int i_nRow, ITEM_GENERAL_READABLE *o_pItem)
{
	memset(o_pItem, 0, sizeof(ITEM_GENERAL_READABLE));
	
	ITEM *pItem = m_mapItemInfo.findEZ_ptr(atoi(m_GridItem.GetItemText(i_nRow, 3)));	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	o_pItem->Kind = pItem->Kind;
	
	o_pItem->UniqueNumber = atoi(m_GridItem.GetItemText(i_nRow, 1));			// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	util::strncpy(o_pItem->ItemName, m_GridItem.GetItemText(i_nRow, 2), SIZE_MAX_ITEM_NAME);	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	o_pItem->ItemNum = atoi(m_GridItem.GetItemText(i_nRow, 3));					// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
// 2005-11-26 by cmkwon
//	RARE_ITEM_INFO *pPrefix = m_mapRareItemInfoName.findEZ_ptr((LPCSTR)m_GridItem.GetItemText(i_nRow, 3));
//	if (pPrefix != NULL)
//		o_pItem->PrefixCodeNum = pPrefix->CodeNum;
//	RARE_ITEM_INFO *pSuffix = m_mapRareItemInfoName.findEZ_ptr((LPCSTR)m_GridItem.GetItemText(i_nRow, 4));
//	if (pSuffix != NULL)
//		o_pItem->SuffixCodeNum = pSuffix->CodeNum;
	o_pItem->PrefixCodeNum		= GetRareCodeNum((LPCSTR)m_GridItem.GetItemText(i_nRow, 4));	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	o_pItem->SuffixCodeNum		= GetRareCodeNum((LPCSTR)m_GridItem.GetItemText(i_nRow, 5));	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	o_pItem->EnchantCount		= atoi(m_GridItem.GetItemText(i_nRow, 6));						// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	o_pItem->Wear				= GetWear(m_GridItem.GetItemText(i_nRow, 7));		// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	// start - 2012-03-15 by jhseol, 파트너 - hidden아이탬의 ItemWindowIndex 번호 설정
	if("Hidden" == m_GridItem.GetItemText(i_nRow, 8))
	{
		o_pItem->ItemWindowIndex	= 98;	
	}
	else
	{
	o_pItem->ItemWindowIndex	= atoi(m_GridItem.GetItemText(i_nRow, 8));			// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	}
	// end - 2012-03-15 by jhseol, 파트너 - hidden아이탬의 ItemWindowIndex 번호 설정
	o_pItem->CurrentCount		= atoi(m_GridItem.GetItemText(i_nRow, 9));			// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	o_pItem->ColorCode			= atoi(m_GridItem.GetItemText(i_nRow, 10));			// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - , // 2005-12-08 by cmkwon, 수정됨
	o_pItem->ShapeItemNum		= atoi(m_GridItem.GetItemText(i_nRow, 11));			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가

	return TRUE;
}

void CSCUserAdminDlg::OnButtonGetUserInfo() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_bIsValidAccountName			= FALSE;
	
	if (m_bCurrentAccountBlocked 
		&& FALSE == m_szAccountName.IsEmpty())
	{
		int ret = AfxMessageBox(STRMSG_S_SCADMINTOOL_0039, MB_YESNO);
		if (ret == IDYES)
		{
			SendUnblockAccount(m_szAccountName, m_pMainDlg->m_UID);		// 2013-07-01 by jhseol, 어드민 툴 계정 unblock시 어드민 계정 남지 않는 버그 수정
		}
	}

	EnableAllOuputControls(FALSE);
	ResetControls(TRUE);

	m_szAccountNameInput.TrimLeft();
	m_szAccountNameInput.TrimRight();
	if (m_szAccountNameInput.IsEmpty())
	{
		MessageBox(STRERR_S_SCADMINTOOL_0032);
		GetDlgItem(IDC_EDIT_USER_INPUT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_USER_INPUT))->SetSel(0, -1);
		return;
	}

	GetAccountInfo((LPCSTR)m_szAccountNameInput, (enumGetAccountQueryType)m_nAccCharac);
	WriteProfile();
}

void CSCUserAdminDlg::GetAccountInfo(const char *i_szName, enumGetAccountQueryType i_nGAQT)
{

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	CString szSQLQuery;
// 	switch(i_nGAQT) {
// 	case GAQT_ACCOUNT_NAME:
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szSQLQuery.Format("select Accountname, AccountUniqueNumber, CharacterName from td_character WITH (NOLOCK) where accountname = \'%s\' AND 0 = Race&%d", i_szName, RACE_DELETED_CHARACTER);		// 2007-02-22 by dhjin, AND Race < 16384 추가 
// 		szSQLQuery.Format(QUERY_080702_0037, i_szName, RACE_DELETED_CHARACTER);		// 2007-02-22 by dhjin, AND Race < 16384 추가 
// 		break;
// 	case GAQT_ACCOUNT_UID:
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szSQLQuery.Format("select Accountname, AccountUniqueNumber, CharacterName from td_character WITH (NOLOCK) where accountuniquenumber = %s AND 0 = Race&%d", i_szName, RACE_DELETED_CHARACTER);	// 2007-02-22 by dhjin, AND Race < 16384 추가 
// 		szSQLQuery.Format(QUERY_080702_0038, i_szName, RACE_DELETED_CHARACTER);	// 2007-02-22 by dhjin, AND Race < 16384 추가 
// 		break;
// 	case GAQT_CHARACTER_NAME:
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szSQLQuery.Format("select Accountname, AccountUniqueNumber, CharacterName from td_character WITH (NOLOCK) where accountname in (select accountname from td_character where charactername = \'%s\') AND 0 = Race&%d", i_szName, RACE_DELETED_CHARACTER);	// 2007-02-22 by dhjin, AND Race < 16384 추가 
// 		szSQLQuery.Format(QUERY_080702_0039, i_szName, RACE_DELETED_CHARACTER);	// 2007-02-22 by dhjin, AND Race < 16384 추가 
// 		break;
// 	case GAQT_CHARACTER_UID:
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szSQLQuery.Format("select Accountname, AccountUniqueNumber, CharacterName from td_character WITH (NOLOCK) where accountname in (select accountname from td_character where uniquenumber = %s) AND 0 = Race&%d", i_szName, RACE_DELETED_CHARACTER);			// 2007-02-22 by dhjin, AND Race < 16384 추가 
// 		szSQLQuery.Format(QUERY_080702_0040, i_szName, RACE_DELETED_CHARACTER);			// 2007-02-22 by dhjin, AND Race < 16384 추가 
// 		break;
// 	default:
// 		return;
// 	}
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	char tmAccName[SIZE_MAX_ACCOUNT_NAME];		util::zero(tmAccName, SIZE_MAX_ACCOUNT_NAME);
	char tmCharacName[SIZE_MAX_CHARACTER_NAME];	util::zero(tmCharacName, SIZE_MAX_CHARACTER_NAME);
	UID32_t tmAccUID		= 0;
	UID32_t tmCharacUID		= 0;
	switch(i_nGAQT)
	{
	case GAQT_ACCOUNT_NAME:		util::strncpy(tmAccName, i_szName, SIZE_MAX_ACCOUNT_NAME);			break;
	case GAQT_ACCOUNT_UID:		tmAccUID	= atoi(i_szName);										break;
	case GAQT_CHARACTER_NAME:	util::strncpy(tmCharacName, i_szName, SIZE_MAX_CHARACTER_NAME);	break;
	case GAQT_CHARACTER_UID:	tmCharacUID	= atoi(i_szName);										break;
	default:
		return;
	}
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, tmAccName, 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(tmAccUID), 0,							&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, tmCharacName, 0,	&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(tmCharacUID), 0,						&arrCB2[4]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0037));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	char	szAccountName[SIZE_MAX_SQL_PATTERN_BUFFER];
	UID32_t	uidAccountUID	= 0;
	char	szCharacterName[SIZE_MAX_CHARACTER_NAME];
	SQLINTEGER cb1, cb2, cb3;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccountName, SIZE_MAX_ACCOUNT_NAME, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &uidAccountUID, 0, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szCharacterName, SIZE_MAX_CHARACTER_NAME, &cb3);
	int nCount = 0;
	memset(szCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		if (nCount == 0)
		{
			GetDlgItem(IDC_EDIT_CHARAC_NAME_1)->EnableWindow();
			GetDlgItem(IDC_STATIC_CHARAC_1)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CHARAC_1_OK)->EnableWindow();
			GetDlgItem(IDC_EDIT_CHARAC_NAME_1)->SetWindowText(szCharacterName);
		}
		else if (nCount == 1)
		{
			GetDlgItem(IDC_EDIT_CHARAC_NAME_2)->EnableWindow();
			GetDlgItem(IDC_STATIC_CHARAC_2)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CHARAC_2_OK)->EnableWindow();
			GetDlgItem(IDC_EDIT_CHARAC_NAME_2)->SetWindowText(szCharacterName);
		}
		else if (nCount == 2)
		{
			GetDlgItem(IDC_EDIT_CHARAC_NAME_3)->EnableWindow();
			GetDlgItem(IDC_STATIC_CHARAC_3)->EnableWindow();
			GetDlgItem(IDC_BUTTON_CHARAC_3_OK)->EnableWindow();
			GetDlgItem(IDC_EDIT_CHARAC_NAME_3)->SetWindowText(szCharacterName);
		}
		else
		{
			ASSERT_NEVER_GET_HERE();
		}

		nCount++;
		memset(szCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	}
	// clean up
	m_pODBCStmt->FreeStatement();

	switch(i_nGAQT)
	{
	case GAQT_ACCOUNT_NAME:
		{
			m_bIsValidAccountName		= TRUE;
			if(0 >= nCount)
			{
				m_bIsValidAccountName	= FALSE;
				strcpy(szAccountName, i_szName);
				MessageBox(STRERR_S_SCADMINTOOL_0033);
			}
		}
		break;
	default:
		{
			if(0 >= nCount)
			{
				MessageBox(STRERR_S_SCADMINTOOL_0034);
				return;
			}
			m_bIsValidAccountName		= TRUE;
		}
	}
	m_uidAccountUID		= uidAccountUID;

	// set account type
	USHORT nAccountType = 0;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szSQLQuery.Format("select accounttype from td_Account WITH (NOLOCK) WHERE AccountName like \'%s\'", szAccountName);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0041, szAccountName);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_SHORT, &nAccountType, 0, NULL);
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, szAccountName, 0,		&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0041));
	if(FALSE == bRet)
	{
		MessageBox("Get AccountType error !!");
		return;
	}
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_SHORT, &nAccountType, 0, NULL);
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	m_pODBCStmt->FreeStatement();
	GetDlgItem(IDC_EDIT_ACC_TYPE)->SetWindowText(CAtumSJ::GetRaceString(nAccountType));

	// set account name
	GetDlgItem(IDC_EDIT_ACCNAME)->EnableWindow();
	GetDlgItem(IDC_EDIT_ACCNAME)->SetWindowText(szAccountName);
	m_szAccountName	= szAccountName;

	// pre server에 상태 질의
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_ACCOUNT_INFO, T_PA_ADMIN_GET_ACCOUNT_INFO, msgGetCharacter, msgGetCharacterBuf);
	util::strncpy(msgGetCharacter->AccountName, szAccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(msgGetCharacter->CharacterName, "", SIZE_MAX_CHARACTER_NAME);
	m_pUserAdminPreSocket->Write(msgGetCharacterBuf, MSG_SIZE(MSG_PA_ADMIN_GET_ACCOUNT_INFO));	
}

void CSCUserAdminDlg::GetCharacterInfo(const char *i_szCharacterName)
{
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 캐릭터 검색시 여단명 재설정함
	CHARACTER_DB_EX retCharacterDBEX;
	memset(&m_CurrentCharacter, 0x00, sizeof(CHARACTER));
	SQLINTEGER pcb1;

	/**********************************************************************
	--!!!!
	-- Name:
	-- Desc:// 2007-07-31 by cmkwon, 추가함
	--====
	CREATE PROCEDURE dbo.atum_admin_GetCharacterByCharacterName
		@i_CharName	VARCHAR(20)
	AS
		-- // 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 캐릭터 정보를 가져올때 td_Guild 테이블을 참조하여 업데이트한 이후에 가져온다
		IF NOT EXISTS(SELECT c.UniqueNumber FROM td_character c, td_guild g, td_guildMember gm WITH(NOLOCK) WHERE c.charactername = @i_CharName AND c.GuildUniqueNumber = g.GuildUniqueNumber AND gm.GuildUniqueNumber = c.GuildUniqueNumber AND gm.CharacterUniqueNumber = c.UniqueNumber)
		BEGIN
			UPDATE td_character
			SET GuildName = NULL, GuildUniqueNumber = 0
			FROM td_character c	WITH (NOLOCK)
			WHERE c.charactername = @i_CharName
		END
		ELSE
		BEGIN
			UPDATE td_character
			SET GuildName = g.GuildName
			FROM td_character c, td_Guild g	WITH (NOLOCK)
			WHERE c.charactername = @i_CharName AND c.GuildUniqueNumber = g.GuildUniqueNumber
		END

		SELECT *
		FROM td_Character
		WITH (NOLOCK)
		WHERE charactername = @i_CharName
	GO
	**********************************************************************/
	pcb1 = SQL_NTS;
	char tmCharName[SIZE_MAX_CHARACTER_NAME]	= {NULL,};
	util::strncpy(tmCharName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME);

	SQLBindParameter(m_pODBCStmt->GetSTMTHandle(), 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, tmCharName, 0,		&pcb1);
	SQLRETURN ret = SQLExecDirect(m_pODBCStmt->GetSTMTHandle(), PROCEDURE_080822_0026, SQL_NTS);

	// 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 비교 추가( && ret!=SQL_NO_DATA)
	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		m_pODBCStmt->FreeStatement();
		MessageBox(STRERR_S_SCADMINTOOL_0035);
		return;
	}

	SQLINTEGER	cb[CB_COUNT_CHARACTER] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - , // 2008-06-23 by dhjin, EP3 유저정보옵션 -		// 2007-06-07 by dhjin
	// bind columns
	CAtumDBHelper::BindColCharacter(m_pODBCStmt->m_hstmt, m_CurrentCharacter, retCharacterDBEX, cb);

	// initialize to 0x00
	memset(&m_CurrentCharacter, 0x00, sizeof(CHARACTER));
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(m_pODBCStmt->GetSTMTHandle(), &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(m_pODBCStmt->GetSTMTHandle());
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(m_pODBCStmt->GetSTMTHandle());
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(m_pODBCStmt->GetSTMTHandle());                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		m_CurrentCharacter				= retCharacterDBEX;
		_strlwr(m_CurrentCharacter.AccountName);
		m_CurrentCharacter.TotalGearStat	= m_CurrentCharacter.GearStat;

		// 2012-10-17 by jhseol, 아템 미리보기 무조건 ON 시키기
#ifdef S_ITEM_VIEW_UNCONDITIONAL_ON
		m_CurrentCharacter.SecretInfoOption = m_CurrentCharacter.SecretInfoOption & USER_INFO_OPTION_ITEMINFO_DELETE_ALL_MASK;
#endif
		// end 2012-10-17 by jhseol, 아템 미리보기 무조건 ON 시키기
	}
	m_pODBCStmt->FreeStatement();		// clean up

	if(FALSE == IS_VALID_UNIQUE_NUMBER(m_CurrentCharacter.CharacterUniqueNumber))
	{
		///////////////////////////////////////////////////////////////////////////
		// 캐릭터가 없음, Error 처리
		MessageBox(STRERR_S_SCADMINTOOL_0036);
		return;
	}

// 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 위와 같이 프로시저 호출로 변경함
//	CString szSQLQuery;
//	szSQLQuery.Format("select * from td_character WITH (NOLOCK) where CharacterName = \'%s\'", i_szCharacterName);
//	SQLINTEGER	cb[CB_COUNT_CHARACTER] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
//											,SQL_NTS};// 2007-06-07 by dhjin
//	CHARACTER_DB_EX characterDBEX;
//	CAtumDBHelper::BindColCharacter(m_pODBCStmt->m_hstmt, m_CurrentCharacter, characterDBEX, cb);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
//	if (!bRet)
//	{
//		// clean up
//		m_pODBCStmt->FreeStatement();
//		MessageBox(STRERR_S_SCADMINTOOL_0035);
//		return;
//	}
//
//	int nCount = 0;
//	memset(&m_CurrentCharacter, 0x00, sizeof(CHARACTER));
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		m_CurrentCharacter = characterDBEX;
//
//		nCount++;
//	}
//	m_pODBCStmt->FreeStatement();		// clean up
//
//	if (nCount == 0)
//	{
//		MessageBox(STRERR_S_SCADMINTOOL_0036);
//		return;
//	}

	DisplayCharacterInfo(&m_CurrentCharacter);

	// 캐릭터 상태 요청
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_ACCOUNT_INFO, T_PA_ADMIN_GET_ACCOUNT_INFO, msgGetCharacter, msgGetCharacterBuf);
	util::strncpy(msgGetCharacter->AccountName, m_szAccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(msgGetCharacter->CharacterName, m_CurrentCharacter.CharacterName, SIZE_MAX_CHARACTER_NAME);
	m_pUserAdminPreSocket->Write(msgGetCharacterBuf, MSG_SIZE(MSG_PA_ADMIN_GET_ACCOUNT_INFO));
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCUserAdminDlg::GetInfluenceLeaderCharacterInfo(BYTE i_byInflType)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCUserAdminDlg::GetInfluenceLeaderCharacterInfo(BYTE i_byInflType)
{
	// 2006-12-08 by dhjin, 지도자 관련 변수 초기화
	util::zero(&m_InflLeaderCharacter, sizeof(CHARACTER));
	util::zero(&m_InflSub1LeaderCharacter, sizeof(CHARACTER));
	util::zero(&m_InflSub2LeaderCharacter, sizeof(CHARACTER));

	CString szSQLQuery;
	SQLINTEGER	cb1 = SQL_NTS, cb2 = SQL_NTS;
	UID32_t		leaderCharUID		= 0;
	UID32_t		Sub1LeaderCharUID	= 0;
	UID32_t		Sub2LeaderCharUID	= 0;
	char		sztmCharName[SIZE_MAX_CHARACTER_NAME];

	// 2006-12-08 by dhjin, 지도자
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szSQLQuery.Format("SELECT TOP 1 c.CharacterName, c.UniqueNumber FROM td_character c, td_InfluenceWarData iw WITH (NOLOCK) \
//		WHERE iw.InfluenceType = %d and iw.InflLeaderCharacterUID = c.UniqueNumber", i_byInflType);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0042, i_byInflType);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_byInflType), 0,		&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0042));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("DB ExecuteQuery Error !!(CSCUserAdminDlg::GetInfluenceLeaderCharacterInfo_)");
		return FALSE;
	}
	
	SQLBindCol(m_pODBCStmt->GetSTMTHandle(), 1, SQL_C_CHAR, sztmCharName, SIZE_MAX_CHARACTER_NAME,	&cb1);
	SQLBindCol(m_pODBCStmt->GetSTMTHandle(), 2, SQL_C_ULONG, &leaderCharUID, 0,	&cb2);

	util::zero(sztmCharName, SIZE_MAX_CHARACTER_NAME);
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	m_pODBCStmt->FreeStatement();		// clean up

//	if(0 >= leaderCharUID)
//	{
//		return FALSE;
//	}

	m_InflLeaderCharacter.CharacterUniqueNumber	= leaderCharUID;
	util::strncpy(m_InflLeaderCharacter.CharacterName, sztmCharName, SIZE_MAX_CHARACTER_NAME);

	// 2006-12-08 by dhjin, 부지도자1
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szSQLQuery.Format("SELECT TOP 1 c.CharacterName, c.UniqueNumber FROM td_character c, td_InfluenceWarData iw WITH (NOLOCK) \
//		WHERE iw.InfluenceType = %d and iw.InflSub1LeaderCharacterUID = c.UniqueNumber", i_byInflType);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0043, i_byInflType);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_byInflType), 0,		&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0043));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("DB ExecuteQuery Error !!(CSCUserAdminDlg::GetInfluenceLeaderCharacterInfo_)");
		return FALSE;
	}
	
	SQLBindCol(m_pODBCStmt->GetSTMTHandle(), 1, SQL_C_CHAR, sztmCharName, SIZE_MAX_CHARACTER_NAME,	&cb1);
	SQLBindCol(m_pODBCStmt->GetSTMTHandle(), 2, SQL_C_ULONG, &Sub1LeaderCharUID, 0,	&cb2);
	util::zero(sztmCharName, SIZE_MAX_CHARACTER_NAME);
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	m_pODBCStmt->FreeStatement();		// clean up

	m_InflSub1LeaderCharacter.CharacterUniqueNumber	= Sub1LeaderCharUID;
	util::strncpy(m_InflSub1LeaderCharacter.CharacterName, sztmCharName, SIZE_MAX_CHARACTER_NAME);

	// 2006-12-08 by dhjin, 부지도자2
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szSQLQuery.Format("SELECT TOP 1 c.CharacterName, c.UniqueNumber FROM td_character c, td_InfluenceWarData iw WITH (NOLOCK) \
//		WHERE iw.InfluenceType = %d and iw.InflSub2LeaderCharacterUID = c.UniqueNumber", i_byInflType);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0044, i_byInflType);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
	arrCB2[1] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_byInflType), 0,		&arrCB2[1]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0044));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("DB ExecuteQuery Error !!(CSCUserAdminDlg::GetInfluenceLeaderCharacterInfo_)");
		return FALSE;
	}
	
	SQLBindCol(m_pODBCStmt->GetSTMTHandle(), 1, SQL_C_CHAR, sztmCharName, SIZE_MAX_CHARACTER_NAME,	&cb1);
	SQLBindCol(m_pODBCStmt->GetSTMTHandle(), 2, SQL_C_ULONG, &Sub2LeaderCharUID, 0,	&cb2);

	util::zero(sztmCharName, SIZE_MAX_CHARACTER_NAME);
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	m_pODBCStmt->FreeStatement();		// clean up

	m_InflSub2LeaderCharacter.CharacterUniqueNumber	= Sub2LeaderCharUID;
	util::strncpy(m_InflSub2LeaderCharacter.CharacterName, sztmCharName, SIZE_MAX_CHARACTER_NAME);

	return TRUE;
}

void CSCUserAdminDlg::GetItemInfo(const char *i_szCharacterName)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_PROCEDURE_080827_0045
	-- DESC				: -- // 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	--						-- // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가, 결과 레코드에 필드 1개 추가
	--						-- // 2009-11-18 by cmkwon, AdminTool에서 시간제한 아이템의 경과된 정보 보기 추가 - UsingTimeStamp 필드 가져오기 추가
	--------------------------------------------------------------------------------
	************************************************************************/
	CString szSQLQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("select s.UniqueNumber, i.ItemNum, s.PrefixCodeNum,\
// 							s.SuffixCodeNum, s.ItemStorage, s.Wear, s.ItemWindowIndex, s.CurrentCount, s.ColorCode, s.possess, s.CreatedTime \
// 						from td_store s, td_character c, ti_item i WITH (NOLOCK) \
// 						where c.charactername = '%s'and i.itemnum = s.itemnum and c.accountuniquenumber = s.accountuniquenumber \
// 						and (c.uniquenumber = s.possess OR s.possess = 0 OR s.possess = -1) and (s.ItemStorage = %d OR s.ItemStorage= %d)"
// 						, i_szCharacterName, ITEM_IN_CHARACTER, ITEM_IN_STORE);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0045, i_szCharacterName, ITEM_IN_CHARACTER, ITEM_IN_STORE);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BYTE byItemStorage1 = ITEM_IN_CHARACTER;
	BYTE byItemStorage2 = ITEM_IN_STORE;
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, (LPSTR)i_szCharacterName, 0,	&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(byItemStorage1), 0,							&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(byItemStorage2), 0,							&arrCB2[3]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0045));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	ITEM_GENERAL_READABLE StoreItemR;
	// 2009-08-26 by cmkwon, SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8, cb9, cb10, cb11;
	// 2009-11-18 by cmkwon, AdminTool에서 시간제한 아이템의 경과된 정보 보기 추가 - 
	SQLINTEGER arrCB[14] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_SBIGINT, &StoreItemR.UniqueNumber, 0,				&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &StoreItemR.ItemNum, 0,					&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &StoreItemR.PrefixCodeNum, 0,				&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &StoreItemR.SuffixCodeNum, 0,				&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &StoreItemR.ItemStorage, 0,				&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_UTINYINT, &StoreItemR.Wear, 0,					&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_SLONG, &StoreItemR.ItemWindowIndex, 0,			&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_SLONG, &StoreItemR.CurrentCount, 0,				&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &StoreItemR.ColorCode, 0,					&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &StoreItemR.Possess, 0,					&arrCB[10]);
	SQL_TIMESTAMP_STRUCT tmpCreatedTime;	util::zero(&tmpCreatedTime, sizeof(tmpCreatedTime));
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_TIMESTAMP, &tmpCreatedTime, 0,					&arrCB[11]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_LONG, &StoreItemR.ShapeItemNum, 0,				&arrCB[12]);		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_LONG, &StoreItemR.UsingTimeStamp, 0,				&arrCB[13]);		// 2009-11-18 by cmkwon, AdminTool에서 시간제한 아이템의 경과된 정보 보기 추가 - UsingTimeStamp 필드 가져오기 추가

// 2005-11-26 by cmkwon
//	int nCount = 0;
//	memset(&StoreItemR, 0x00, sizeof(ITEM_GENERAL_READABLE));
//	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		StoreItemR.CreatedTime = tmpCreatedTime;
//		m_GridItem.SetRowCount(nCount+2);
//		InsertItemToGrid(&StoreItemR, nCount+1);
//
//		nCount++;
//		memset(&StoreItemR, 0x00, sizeof(ITEM_GENERAL_READABLE));
//	}
//	m_pODBCStmt->FreeStatement();	// clean up

	m_vectCurrentCharacterGetItemList.clear();		// 2012-03-15 by jhseol, 파트너 - 선택된 캐릭터가 보유한 아이템 리스트 초기화
	vectITEM_GENERAL_READABLE vectItemRList;
	memset(&StoreItemR, 0x00, sizeof(ITEM_GENERAL_READABLE));
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		StoreItemR.CreatedTime = tmpCreatedTime;
		ITEM *pItemInfo = m_mapItemInfo.findEZ_ptr(StoreItemR.ItemNum);
		if(pItemInfo)
		{
			util::strncpy(StoreItemR.ItemName, pItemInfo->ItemName, SIZE_MAX_ITEM_NAME);
			StoreItemR.ItemInfo		= pItemInfo;		// 2009-11-18 by cmkwon, AdminTool에서 시간제한 아이템의 경과된 정보 보기 추가 - 
		}
		vectItemRList.push_back(StoreItemR);
		m_vectCurrentCharacterGetItemList.push_back(StoreItemR);		// 2012-03-15 by jhseol, 파트너 - 선택된 캐릭터가 보유한 아이템 리스트 등록
		memset(&StoreItemR, 0x00, sizeof(ITEM_GENERAL_READABLE));
	}
	m_pODBCStmt->FreeStatement();	// clean up
	for(int i= 0; i < vectItemRList.size(); i++)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2005-11-26 by cmkwon, Enchant Count를 가져온다.
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//		szSQLQuery.Format("select COUNT(*) from td_Enchant WITH (NOLOCK) where TargetItemUniqueNumber = %I64d", vectItemRList[i].UniqueNumber);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 		szSQLQuery.Format(QUERY_080702_0046, vectItemRList[i].UniqueNumber);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &vectItemRList[i].EnchantCount, 0, &cb1);
// 		BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 		if (!bRet)
// 		{
// 			MessageBox("Error : Get Enchant Count");
// 		}
// 		else
// 		{
// 			SQLFetch(m_pODBCStmt->m_hstmt);
// 		}
		SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
		SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &(vectItemRList[i].UniqueNumber), 0,		&arrCB2[1]);			
		BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0046));
		if(FALSE == bRet)
		{
			MessageBox("Error : Get Enchant Count");
		}
		else
		{
			arrCB2[1] = SQL_NTS;
			SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &vectItemRList[i].EnchantCount, 0, &arrCB2[1]);
			SQLFetch(m_pODBCStmt->m_hstmt);
		}
		m_pODBCStmt->FreeStatement();			// clean up
		
		///////////////////////////////////////////////////////////////////////////////
		// 그리드에 추가한다
		m_GridItem.SetRowCount(i+2);
		InsertItemToGrid(&vectItemRList[i], i+1);
	}

	m_GridItem.AutoSize();
	m_GridItem.ExpandColumnsToFit();
}

void CSCUserAdminDlg::OnButtonCharac1Ok() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_szCurrentCharacterName = m_CharacterName1;

	GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);
	GetItemInfo((LPCSTR)m_szCurrentCharacterName);

	GetDlgItem(IDC_BUTTON_RELOAD)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_CHARAC_NAME_1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_CHARAC_NAME_2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CHARAC_NAME_3)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CHARAC_1)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_CHARAC_2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CHARAC_3)->EnableWindow(FALSE);
}

void CSCUserAdminDlg::OnButtonCharac2Ok() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_szCurrentCharacterName = m_CharacterName2;

	GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);
	GetItemInfo((LPCSTR)m_szCurrentCharacterName);

	GetDlgItem(IDC_BUTTON_RELOAD)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_CHARAC_NAME_1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CHARAC_NAME_2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_CHARAC_NAME_3)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CHARAC_1)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CHARAC_2)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_CHARAC_3)->EnableWindow(FALSE);
}

void CSCUserAdminDlg::OnButtonCharac3Ok() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_szCurrentCharacterName = m_CharacterName3;

	GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);
	GetItemInfo((LPCSTR)m_szCurrentCharacterName);

	GetDlgItem(IDC_BUTTON_RELOAD)->EnableWindow(TRUE);

	GetDlgItem(IDC_EDIT_CHARAC_NAME_1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CHARAC_NAME_2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CHARAC_NAME_3)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_CHARAC_1)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CHARAC_2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_CHARAC_3)->EnableWindow(TRUE);
}

void CSCUserAdminDlg::InsertItemToGrid(ITEM_GENERAL_READABLE *i_pItemGeneral, int i_nRow)
{
	// 칼럼 만들기
	int m_nCols = 0;

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_nRow;
	Item.nFormat = GRID_CELL_FORMAT;

	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	Item.col = m_nCols++;
	Item.strText.Format("%d", m_GridItem.GetRowCount() - 1);
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%I64d", i_pItemGeneral->UniqueNumber);
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pItemGeneral->ItemName);
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->ItemNum);
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	RARE_ITEM_INFO *pRareItemInfo = m_mapRareItemInfo.findEZ_ptr(i_pItemGeneral->PrefixCodeNum);
	if (pRareItemInfo != NULL)
	{
		char szTmp[1024];
		util::zero(szTmp, 1024);
		Item.strText = GetRareString(szTmp, pRareItemInfo->Name, pRareItemInfo->CodeNum);
	}
	else
	{
		Item.strText = "";
	}
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	pRareItemInfo = m_mapRareItemInfo.findEZ_ptr(i_pItemGeneral->SuffixCodeNum);
	if (pRareItemInfo != NULL)
	{
		char szTmp[1024];
		util::zero(szTmp, 1024);
		Item.strText = GetRareString(szTmp, pRareItemInfo->Name, pRareItemInfo->CodeNum);
	}
	else
	{
		Item.strText = "";
	}
	m_GridItem.SetItem(&Item);

	// 2005-11-26 by cmkwon, 추가됨
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->EnchantCount);
	m_GridItem.SetItem(&Item);


	Item.col = m_nCols++;
	if (i_pItemGeneral->Wear == WEAR_NOT_ATTACHED)
		Item.strText = STRMSG_S_SCAT_COLNAME_0025;
	if (i_pItemGeneral->Wear == WEAR_ATTACHED)
		Item.strText = STRMSG_S_SCAT_COLNAME_0021;
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
// 2005-12-08 by cmkwon
//	if (i_pItemGeneral->Possess == POSSESS_STORE_NUMBER)
//	{
//		Item.strText.Format("Storage");
//	}
//	else if (i_pItemGeneral->Possess == POSSESS_AUCTION_NUMBER)
//	{
//		Item.strText.Format("Auction");
//	}
//	else
//	{
//		Item.strText.Format("%d", i_pItemGeneral->ItemWindowIndex);
//	}
	if(ITEM_IN_STORE == i_pItemGeneral->ItemStorage)		// 2005-12-08 by cmkwon
	{
		Item.strText.Format("Storage");
	}
	else if(ITEM_IN_GUILD_STORE == i_pItemGeneral->ItemStorage)
	{
		Item.strText.Format("Guild Storage");
	}
	else
	{
		// start 2011-09-30 by hskim, 파트너 시스템 2차
		/////////
		// 기존
		// Item.strText.Format("%d", i_pItemGeneral->ItemWindowIndex);

		/////////
		// 수정
		if( POS_HIDDEN_ITEM == i_pItemGeneral->ItemWindowIndex )
		{
			Item.strText.Format("Hidden");
		}
		else
		{
		Item.strText.Format("%d", i_pItemGeneral->ItemWindowIndex);
	}
		// end 2011-09-30 by hskim, 파트너 시스템 2차		
	}
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->CurrentCount);
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->ColorCode);
	m_GridItem.SetItem(&Item);

	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가, 12열에 추가
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->ShapeItemNum);
	m_GridItem.SetItem(&Item);
	
	// 2009-11-18 by cmkwon, AdminTool에서 시간제한 아이템의 경과된 정보 보기 추가 - 
	ITEM *pItemInfo	= i_pItemGeneral->ItemInfo;
	if(pItemInfo)
	{
		BOOL bLimitedItem	= FALSE;
		BOOL bUsedItem		= FALSE;
		int nRemainSec		= INT_MAX;
		if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
		{
			bLimitedItem	= TRUE;
			nRemainSec		= CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(pItemInfo) - i_pItemGeneral->UsingTimeStamp;
			bUsedItem		= i_pItemGeneral->UsingTimeStamp == 0 ? FALSE : TRUE;
		}

		if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		{
			bLimitedItem	= TRUE;

			ATUM_DATE_TIME atExpire = i_pItemGeneral->CreatedTime;
			atExpire.AddDateTime(0, 0, 0, 0, 0, CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(pItemInfo, TIME_TERM_DELETE_ITEM));

			ATUM_DATE_TIME atCur { true };
			nRemainSec = min(nRemainSec, atExpire.GetTimeDiffTimeInSeconds(atCur));
		}

		if(FALSE == bLimitedItem)
		{
			Item.strText.Format("");
		}
		else
		{
			if(FALSE == bUsedItem)
			{
				Item.strText.Format("%d(Not use)", nRemainSec);
			}
			else
			{
				Item.strText.Format("%d(Using)", nRemainSec);
			}
		}
	}
	else
	{
		Item.strText.Format("Unknown");
	}
	// 2009-11-18 by cmkwon, AdminTool에서 시간제한 아이템의 경과된 정보 보기 추가 - 
	Item.col = m_nCols++;
	m_GridItem.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pItemGeneral->CreatedTime.GetDateTimeString().GetBuffer());
	m_GridItem.SetItem(&Item);
}

BOOL CSCUserAdminDlg::AddLineToGridCharacter(const char *i_szKey, const char *i_szValue)
{
	int nCurrentRow = m_GridCharacter.GetRowCount();
	m_GridCharacter.SetRowCount(nCurrentRow+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = nCurrentRow;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = 0;
	Item.strText = i_szKey;
	m_GridCharacter.SetItem(&Item);

	Item.col = 1;
	Item.strText = i_szValue;
	m_GridCharacter.SetItem(&Item);

	// set read-only
//	m_GridCharacter.SetItemState(Item.row, 0, m_GridCharacter.GetItemState(Item.row, 0) | GVIS_READONLY);

	return TRUE;
}

BOOL CSCUserAdminDlg::AddLineToGridCharacter(const char *i_szKey, int i_nValue)
{
	CString tmp;
	tmp.Format("%d", i_nValue);

	return AddLineToGridCharacter(i_szKey, (LPCSTR)tmp);
}

BOOL CSCUserAdminDlg::AddLineToGridCharacter(const char *i_szKey, float i_fValue)
{
	CString tmp;
	tmp.Format("%5.2f", i_fValue);

	return AddLineToGridCharacter(i_szKey, (LPCSTR)tmp);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CSCUserAdminDlg::GetRareString(char *o_szRareString, char *i_szRareName, INT i_nRareCodeNum)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-26 ~ 2005-11-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CSCUserAdminDlg::GetRareString(char *o_szRareString, char *i_szRareName, INT i_nRareCodeNum)
{
	sprintf(o_szRareString, "%s(%d)", i_szRareName, i_nRareCodeNum);
	return o_szRareString;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CSCUserAdminDlg::GetRareCodeNum(const char *i_szRareString)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-26 ~ 2005-11-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CSCUserAdminDlg::GetRareCodeNum(const char *i_szRareString)
{
	if(0 > strlen(i_szRareString))
	{
		return 0;
	}

	CString strTmp(i_szRareString);

	int nRareNumIndex = strTmp.ReverseFind('(');
	if(0 > nRareNumIndex)
	{
		return 0;
	}

	return atoi(&i_szRareString[nRareNumIndex+1]);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCUserAdminDlg::IsEnabledEdit(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-15 ~ 2006-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCUserAdminDlg::IsEnabledEdit(void)
{
	if(FALSE == m_bCurrentAccountBlocked
		|| FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		return FALSE;
	}
	return TRUE;
}

void CSCUserAdminDlg::EnableAllOuputControls(BOOL i_bEnable /* = TRUE */)
{
//	GetDlgItem(IDC_EDIT_ACCNAME)->EnableWindow(i_bEnable);
//
	GetDlgItem(IDC_EDIT_CHARAC_NAME_1)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_EDIT_CHARAC_NAME_2)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_EDIT_CHARAC_NAME_3)->EnableWindow(i_bEnable);

	GetDlgItem(IDC_STATIC_CHARAC_1)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_STATIC_CHARAC_2)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_STATIC_CHARAC_3)->EnableWindow(i_bEnable);

	GetDlgItem(IDC_BUTTON_CHARAC_1_OK)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_CHARAC_2_OK)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_CHARAC_3_OK)->EnableWindow(i_bEnable);

//	GetDlgItem(IDC_EDIT_POSITION)->EnableWindow(i_bEnable);

	m_szCurrentCharacterName = "";
}

void CSCUserAdminDlg::ResetControls(BOOL i_bOnlyOutPut /* = FALSE */)
{
	if (!i_bOnlyOutPut)
	{
		GetDlgItem(IDC_EDIT_USER_INPUT)->SetWindowText("");
	}

	GetDlgItem(IDC_EDIT_ACCNAME)->SetWindowText("");
	GetDlgItem(IDC_EDIT_ACC_TYPE)->SetWindowText("");

	GetDlgItem(IDC_EDIT_CHARAC_NAME_1)->SetWindowText("");
	GetDlgItem(IDC_EDIT_CHARAC_NAME_2)->SetWindowText("");
	GetDlgItem(IDC_EDIT_CHARAC_NAME_3)->SetWindowText("");

	GetDlgItem(IDC_EDIT_POSITION)->SetWindowText("");
	UpdateData();

	if (!i_bOnlyOutPut)
	{
		m_nAccCharac = 0;
		UpdateData(FALSE);
	}

	GetDlgItem(IDC_BUTTON_BLOCK_ACCOUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BLOCK_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNBLOCK_ACCOUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETED_CHARACTER)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_RELOAD)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_EDIT_ACCOUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EDIT_CHARACTER)->EnableWindow(FALSE);
// 2006-04-15 by cmkwon
//	GetDlgItem(IDC_BUTTON_EDIT_PREMIUM)->EnableWindow(FALSE);
// 2010.09.07. by hsLee.
	GetDlgItem( IDC_BUTTON_EDIT_PREMIUM )->EnableWindow( FALSE );

	InitGridCharacter();

	InitGridItem();

	// 캐릭터 구조체 초기화
	memset(&m_CurrentCharacter, 0, sizeof(CHARACTER));
}

// 
/****************************************************************************
**
**	유저 정보 받았을 때의 기본 메뉴 컨트롤 설정.
**
**	Create Info : 2010. 09. 06. by hsLee.
**
**
*****************************************************************************/
void CSCUserAdminDlg :: EnableControls_GetAccountInfo ( const MSG_PA_ADMIN_GET_ACCOUNT_INFO_OK *a_pPacket_AccountInfo )
{

	if ( a_pPacket_AccountInfo == NULL )
		return;

	int iIDC_ButtonList[] = 
	{
		IDC_BUTTON_BLOCK_ACCOUNT	, IDC_BUTTON_DELETED_CHARACTER	, IDC_BUTTON_BLOCK_EDIT			,
		IDC_BUTTON_UNBLOCK_ACCOUNT	, IDC_BUTTON_EDIT_ACCOUNT		, IDC_BUTTON_EDIT_CHARACTER		,
		IDC_BUTTON_EDIT_PREMIUM		, IDC_BTN_BLOCKED_LIST			, IDC_BTN_INFLWAR_LIST			,
		IDC_BTN_INIT_INFLUENCE		, IDC_BTN_SET_InfluenceRate		, IDC_BTN_INFINITY_RESET_IMPUTE , 
	};

	int iLoop = sizeof( iIDC_ButtonList ) / sizeof( int );

	for ( int i = 0; i < iLoop; ++i )
	{
		GetDlgItem( iIDC_ButtonList[i] )->EnableWindow ( FALSE );
	}


	switch ( m_pMainDlg->GetManagerAccountType() )
	{
		case RACE_MONITOR :

			break;

		// 2012-01-07 by khkim, 베트남 GM 권한수정
#ifdef S_MANAGER_ADMIN_HSSON
		case RACE_GAMEMASTER :
			GetDlgItem( IDC_BUTTON_BLOCK_ACCOUNT )->EnableWindow( !a_pPacket_AccountInfo->IsBlocked );
			GetDlgItem( IDC_BUTTON_DELETED_CHARACTER )->EnableWindow( a_pPacket_AccountInfo->IsBlocked );
			GetDlgItem( IDC_BUTTON_BLOCK_EDIT )->EnableWindow( a_pPacket_AccountInfo->IsBlocked );
			GetDlgItem( IDC_BUTTON_UNBLOCK_ACCOUNT )->EnableWindow( a_pPacket_AccountInfo->IsBlocked );

			GetDlgItem(IDC_BUTTON_EDIT_ACCOUNT)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_EDIT_CHARACTER)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_EDIT_PREMIUM)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_INFLWAR_LIST)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_INIT_INFLUENCE)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_SET_InfluenceRate)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_INFINITY_RESET_IMPUTE)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_BLOCKED_LIST)->EnableWindow( TRUE );

			break;
#endif
		// end 2012-01-07 by khkim, 베트남 GM 권한수정

		default :

			if ( m_pMainDlg->IsManagerAdministrator() )
			{
				GetDlgItem( IDC_BUTTON_BLOCK_ACCOUNT )->EnableWindow( !a_pPacket_AccountInfo->IsBlocked );
				GetDlgItem( IDC_BUTTON_DELETED_CHARACTER )->EnableWindow( a_pPacket_AccountInfo->IsBlocked );
				GetDlgItem( IDC_BUTTON_BLOCK_EDIT )->EnableWindow( a_pPacket_AccountInfo->IsBlocked );
				GetDlgItem( IDC_BUTTON_UNBLOCK_ACCOUNT )->EnableWindow( a_pPacket_AccountInfo->IsBlocked );
				GetDlgItem( IDC_BUTTON_EDIT_ACCOUNT )->EnableWindow( a_pPacket_AccountInfo->IsBlocked && m_bIsValidAccountName );
				
				if ( strnicmp ( m_CurrentCharacter.AccountName , a_pPacket_AccountInfo->AccountInfo.AccountName , SIZE_MAX_ACCOUNT_NAME ) == 0 )
				{
					GetDlgItem(IDC_BUTTON_EDIT_CHARACTER)->EnableWindow( a_pPacket_AccountInfo->IsBlocked );
				}

				GetDlgItem( IDC_BUTTON_EDIT_PREMIUM )->EnableWindow( TRUE );
			}

			GetDlgItem ( IDC_BTN_BLOCKED_LIST )->EnableWindow ( TRUE );
			GetDlgItem ( IDC_BTN_INFLWAR_LIST )->EnableWindow ( TRUE );
			GetDlgItem ( IDC_BTN_INIT_INFLUENCE )->EnableWindow ( TRUE );
			GetDlgItem ( IDC_BTN_SET_InfluenceRate )->EnableWindow ( TRUE );
			GetDlgItem ( IDC_BTN_INFINITY_RESET_IMPUTE )->EnableWindow ( TRUE );
			break;
		
	}

}


BOOL CSCUserAdminDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
//	{
//		return TRUE;
//	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CSCUserAdminDlg::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	if (m_bCurrentAccountBlocked && m_szAccountName != "")
	{
		int ret = AfxMessageBox(STRMSG_S_SCADMINTOOL_0039, MB_YESNO);
		if (ret == IDYES)
		{
			SendUnblockAccount(m_szAccountName, m_pMainDlg->m_UID);		// 2013-07-01 by jhseol, 어드민 툴 계정 unblock시 어드민 계정 남지 않는 버그 수정
		}
	}

	EnableAllOuputControls(FALSE);
	ResetControls();
	GetDlgItem(IDC_EDIT_USER_INPUT)->SetFocus();
}

void CSCUserAdminDlg::OnButtonReload() 
{
	// TODO: Add your control notification handler code here
	GetCharacterInfo(m_szCurrentCharacterName);
	GetItemInfo(m_szCurrentCharacterName);
}

BOOL CSCUserAdminDlg::DisplayCharacterInfo(CHARACTER *i_pCharacter)
{	
	InitGridCharacter();

	this->GetInfluenceLeaderCharacterInfo(i_pCharacter->InfluenceType);		// 2006-04-25 by cmkwon, 세력지도자 검색

	CString str;

	str.Format("%s(%d)", i_pCharacter->AccountName, i_pCharacter->AccountUniqueNumber);
	AddLineToGridCharacter("AccountName", str);

	str.Format("%s(%d)", i_pCharacter->CharacterName, i_pCharacter->CharacterUniqueNumber);
	AddLineToGridCharacter("CharacterName", str);

	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
	AddLineToGridCharacter("NickName", i_pCharacter->NickName);

	str.Format("%d(%d)", i_pCharacter->MapChannelIndex.MapIndex, i_pCharacter->MapChannelIndex.ChannelIndex);
	AddLineToGridCharacter("MapIndex", str);

	str.Format("(%d, %d) H:%d", (INT)i_pCharacter->PositionVector.x, (INT)i_pCharacter->PositionVector.z, (INT)i_pCharacter->PositionVector.y);
	AddLineToGridCharacter("Position", str);

	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
	AddLineToGridCharacter("StartCityMapIndex", i_pCharacter->Material);

	AddLineToGridCharacter("ColoredName", (i_pCharacter->Color==0?"NO":"YES"));

	AddLineToGridCharacter("Race", CAtumSJ::GetRaceString(i_pCharacter->Race&~RACE_ACCOUNT_TYPE_MASK));

	AddLineToGridCharacter("Authority", CAtumSJ::GetRaceString(i_pCharacter->Race&RACE_ACCOUNT_TYPE_MASK));

	AddLineToGridCharacter("Kind of Unit", CAtumSJ::GetUnitKindString(i_pCharacter->UnitKind));

	str.Format("%s(%d)", CAtumSJ::GetInfluenceTypeString(i_pCharacter->InfluenceType), i_pCharacter->InfluenceType);
	AddLineToGridCharacter("Influence", str);

//	///////////////////////////////////////////////////////////////////////////////
//	// 2006-04-25 by cmkwon, 세력지도자 보여줌
//	if(0 == m_InflLeaderCharacter.CharacterUniqueNumber)
//	{
//		AddLineToGridCharacter("Influence Leader CharacterName", "");		// 2006-04-25 by cmkwon
//	}
//	else
//	{
//		str.Format("%s(%d)", m_InflLeaderCharacter.CharacterName, m_InflLeaderCharacter.CharacterUniqueNumber);
//		AddLineToGridCharacter("Influence Leader CharacterName", str);
//	}

	// 2006-12-08 by dhjin, 세력지도자 관련 화면 보여주기
	if (i_pCharacter->CharacterUniqueNumber == m_InflLeaderCharacter.CharacterUniqueNumber)
	{
		AddLineToGridCharacter("Influence Leader CharacterName", "Leader");
	}
	else if(i_pCharacter->CharacterUniqueNumber == m_InflSub1LeaderCharacter.CharacterUniqueNumber)
	{
		AddLineToGridCharacter("Influence Sub1Leader CharacterName", "Sub1Leader");
	}
	else if(i_pCharacter->CharacterUniqueNumber == m_InflSub2LeaderCharacter.CharacterUniqueNumber)
	{
		AddLineToGridCharacter("Influence Sub2Leader CharacterName", "Sub2Leader");
	}
	else 
	{
		AddLineToGridCharacter("Influence Leader CharacterName", "");
	}
	
	// 2008-12-26 by cmkwon, 예당 세력이전 이벤트 관련 수정 - 버그 수정, 아래와 같이 수정
	//AddLineToGridCharacter("Selectable Influence", i_pCharacter->InfluenceType);
	AddLineToGridCharacter("Selectable Influence", i_pCharacter->SelectableInfluenceMask);

	AddLineToGridCharacter("Level", (INT)i_pCharacter->Level);

	AddLineToGridCharacter("Max Level", (INT)i_pCharacter->MaxLevel);

	str.Format("%5.2f %%, %10.0I64f", CAtumSJ::GetPercentageOfExperience(i_pCharacter->Experience, i_pCharacter->Level), i_pCharacter->Experience);
	AddLineToGridCharacter("Experience", str);

	str.Format("%5.2f", i_pCharacter->DownExperience);
	AddLineToGridCharacter("Down Experience", str);

	str.Format("%d", i_pCharacter->DownSPIOnDeath);
	AddLineToGridCharacter("Dwon SPI on death", str);

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-26 by dhjin, WarPoint관련 추가
	str.Format("W%d/L%d/D%d", i_pCharacter->ArenaWin, i_pCharacter->ArenaLose, i_pCharacter->ArenaDisConnect);
	AddLineToGridCharacter("War Record", str);

	AddLineToGridCharacter("WP", i_pCharacter->WarPoint);

	AddLineToGridCharacter("CWP", i_pCharacter->WarPoint);

	str.Format("%5.2f/%d", i_pCharacter->CurrentHP, i_pCharacter->HP);
	AddLineToGridCharacter("HP", str);

	str.Format("%5.2f/%d", i_pCharacter->CurrentDP, i_pCharacter->DP);
	AddLineToGridCharacter("DP", str);

	str.Format("%d/%d", i_pCharacter->CurrentSP, i_pCharacter->SP);
	AddLineToGridCharacter("SP", str);

	str.Format("%5.2f/%d", i_pCharacter->CurrentEP, i_pCharacter->EP);
	AddLineToGridCharacter("EP", str);

	str.Format("%s", CAtumSJ::GetAutoStatTypeString(i_pCharacter->AutoStatType));
	AddLineToGridCharacter("Auto Stat Type", str);

	AddLineToGridCharacter("Attack Part", i_pCharacter->GearStat.AttackPart);

	AddLineToGridCharacter("Defense Part", i_pCharacter->GearStat.DefensePart);

	AddLineToGridCharacter("Fuel Part", i_pCharacter->GearStat.FuelPart);

	AddLineToGridCharacter("Soul Part", i_pCharacter->GearStat.SoulPart);

	AddLineToGridCharacter("Shield Part", i_pCharacter->GearStat.ShieldPart);

	AddLineToGridCharacter("Dodge Part", i_pCharacter->GearStat.DodgePart);

	// 2008-06-25 by cmkwon, GuildUID 보여주기 추가 - 아래와 같이 수정
	//AddLineToGridCharacter("Guild Name", i_pCharacter->GuildName);
	str.Format("%s(%u)", i_pCharacter->GuildName, i_pCharacter->GuildUniqueNumber);
	AddLineToGridCharacter("Guild Name", str);		// 2008-06-25 by cmkwon, GuildUID 보여주기 추가 - 

	AddLineToGridCharacter("Propensity", i_pCharacter->Propensity);

	str.Format("%s(%d)", CAtumSJ::GetStatusNameKO(i_pCharacter->Status), i_pCharacter->Status);
	AddLineToGridCharacter("Status", str);

	str.Format(STRMSG_S_SCADMINTOOL_0041, i_pCharacter->PKWinPoint, i_pCharacter->PKLossPoint);
	AddLineToGridCharacter("PVP", str);

	AddLineToGridCharacter("RacingPoint", i_pCharacter->RacingPoint);

	AddLineToGridCharacter("BonusStat", i_pCharacter->BonusStat);

// 2005-11-15 by cmkwon, 삭제함
//	AddLineToGridCharacter("Bonus Skill Point", i_pCharacter->BonusSkillPoint);
	AddLineToGridCharacter("BonusStatPoint", i_pCharacter->BonusStatPoint);		// 2005-11-15 by cmkwon

//	str.Format("%d시간%d분%d초(%d초)",
//				(int)i_pCharacter->TotalPlayTime/3600,
//				((int)i_pCharacter->TotalPlayTime%3600)/60,
//				((int)i_pCharacter->TotalPlayTime%60),
//				(int)i_pCharacter->TotalPlayTime);
	str.Format(STRMSG_S_SCADMINTOOL_0040, ATUM_DATE_TIME::GetDateTimeStringFromSeconds(i_pCharacter->TotalPlayTime, string()), (int)i_pCharacter->TotalPlayTime);
	AddLineToGridCharacter("Total Connecting Time", str);

	AddLineToGridCharacter("Created Time", i_pCharacter->CreatedTime.GetDateTimeString().GetBuffer());

	AddLineToGridCharacter("Last Connected Time", i_pCharacter->LastStartedTime.GetDateTimeString().GetBuffer());

	// arrange grid
	m_GridCharacter.AutoSize();
	m_GridCharacter.ExpandColumnsToFit();

	return TRUE;
}

void CSCUserAdminDlg::OnButtonEditCharacter() 
{
	// TODO: Add your control notification handler code here
	if (m_CurrentCharacter.CharacterUniqueNumber == 0)
	{
		return;
	}

	CCharacterInfoDlg dlg(this->IsEnabledEdit(), m_pODBCStmt);
	dlg.Character2Info(&m_CurrentCharacter, m_CurrentCharacter.CharacterUniqueNumber==m_InflLeaderCharacter.CharacterUniqueNumber,
		m_CurrentCharacter.CharacterUniqueNumber==m_InflSub1LeaderCharacter.CharacterUniqueNumber, m_CurrentCharacter.CharacterUniqueNumber==m_InflSub2LeaderCharacter.CharacterUniqueNumber);
	if (IDOK != dlg.DoModal()
		|| FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 캐릭터 정보를 다시 가져온다.
		if(dlg.m_bReloadCharacterInfo)
		{
			GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);
		}
		return;
	}

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 -
	if(FALSE == this->IsEnabledEdit())
	{
		AfxMessageBox("Now, you can't update Character !! Retry");

		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 캐릭터 정보를 다시 가져온다.
		if(dlg.m_bReloadCharacterInfo)
		{
			GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);
		}
		return;
	}

	CString strQuery;

	// 2009-01-23 by cmkwon, 터키 i,I 문자 문제 처리 - AdminTool에서 캐릭터명 변경시 i문자만 소문자로 변경
	char szTmCharName[SIZE_MAX_CHARACTER_NAME];
	util::strncpy(szTmCharName, dlg.m_CharacterName, SIZE_MAX_CHARACTER_NAME);
	dlg.m_CharacterName		= szTmCharName;

	if(0 != strnicmp(m_CurrentCharacter.CharacterName, dlg.m_CharacterName, SIZE_MAX_CHARACTER_NAME))
	{// 2006-03-07 by cmkwon, 캐릭터명이 변경 되었다, 동일한 캐릭터명이 있는지 체크, 로그 남기는 쿼리

		UID32_t tmCharUID32 = 0;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//		strQuery.Format("SELECT uniqueNumber FROM td_character WITH (NOLOCK) WHERE CharacterName = \'%s\'", dlg.m_CharacterName);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 		strQuery.Format(QUERY_080702_0047, dlg.m_CharacterName);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		SQLINTEGER cb1 = SQL_NTS;
// 		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmCharUID32, 0, &cb1);
// 		BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)strQuery);
// 		if (!bRet)
// 		{
// 			MessageBox(STRERR_S_SCADMINTOOL_0037);
// 			m_pODBCStmt->FreeStatement();
// 			return;
// 		}
		SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
		SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, (LPSTR)(LPCSTR)dlg.m_CharacterName, 0,			&arrCB2[1]);
		BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0047));
		if(FALSE == bRet)
		{
 			MessageBox(STRERR_S_SCADMINTOOL_0037);
 			m_pODBCStmt->FreeStatement();

			// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 캐릭터 정보를 다시 가져온다.
			if(dlg.m_bReloadCharacterInfo)
			{
				GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);
			}

 			return;			
		}
		arrCB2[1] = SQL_NTS;
 		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmCharUID32, 0, &arrCB2[1]);
		if((bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			MessageBox("Error !!, Already exist CharacterName");
			m_pODBCStmt->FreeStatement();
			
			// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 캐릭터 정보를 다시 가져온다.
			if(dlg.m_bReloadCharacterInfo)
			{
				GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);
			}

			return;
		}
		m_pODBCStmt->FreeStatement();		// clean up

		// 2006-03-07 by cmkwon, 캐릭터명 변경 로그 추가
		MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN	tmChangeCharacterName;
		tmChangeCharacterName.CharacterUniqueNumber			= m_CurrentCharacter.CharacterUniqueNumber;
		tmChangeCharacterName.CurrentMapIndex				= 0;
		tmChangeCharacterName.Item4Log.ItemUniqueNumber		= 0;
		tmChangeCharacterName.Item4Log.ItemNum				= 0;	// 2006-03-07 by cmkwon, 현재는 유료 아이템이 확정되지 않아 0으로 설정한다. 아이템 로그에서 검색 안됨. 향후 수정해야함
		tmChangeCharacterName.Item4Log.CurrentCount			= 0;
		util::strncpy(tmChangeCharacterName.szOriginCharName, m_CurrentCharacter.CharacterName, SIZE_MAX_CHARACTER_NAME);
		util::strncpy(tmChangeCharacterName.szNewCharName, dlg.m_CharacterName, SIZE_MAX_CHARACTER_NAME);

		CAtumDBLogWriter::InsertLog_Item_Trade(m_pODBCStmt, T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN,
			&tmChangeCharacterName, &tmChangeCharacterName.Item4Log, 0, tmChangeCharacterName.szOriginCharName, tmChangeCharacterName.CharacterUniqueNumber);
		CAtumDBLogWriter::InsertLog_Item_Trade(m_pODBCStmt, T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_NEW,
			&tmChangeCharacterName, &tmChangeCharacterName.Item4Log, 0, tmChangeCharacterName.szNewCharName, tmChangeCharacterName.CharacterUniqueNumber);

		// 2006-03-07 by cmkwon, 변경된 캐릭터명을 적용한다.
		if(0 == m_CharacterName1.Compare(m_szCurrentCharacterName))
		{
			m_CharacterName1		= dlg.m_CharacterName;
		}
		else if(0 == m_CharacterName2.Compare(m_szCurrentCharacterName))
		{
			m_CharacterName2		= dlg.m_CharacterName;
		}
		else if(0 == m_CharacterName3.Compare(m_szCurrentCharacterName))
		{
			m_CharacterName3		= dlg.m_CharacterName;
		}
		m_szCurrentCharacterName	= dlg.m_CharacterName;
		UpdateData(FALSE);
	}

	USHORT nOldRaceAccountType = m_CurrentCharacter.Race & RACE_ACCOUNT_TYPE_MASK;		// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

	dlg.Info2Character(&m_CurrentCharacter);

	DisplayCharacterInfo(&m_CurrentCharacter);

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	strQuery.Format("update td_character\
// 					set CharacterName = \'%s\', Mapindex = %d, ChannelIndex = %d, Position_X = %f, Position_Z = %f,\
// 					Race = %d, InfluenceType = %d, Level = %d, Experience = %I64f, CurrentHP = %f,\
// 					HP = %d, CurrentDP = %f, DP = %d, CurrentSP = %d,\
// 					SP = %d, CurrentEP = %f, EP = %d, AttackPart = %d,\
// 					DefensePart = %d, FuelPart = %d, SoulPart = %d, ShieldPart = %d,\
// 					DodgePart = %d, BonusStat = %d, BonusStatPoint = %d, Propensity = %d, RacingPoint = %d,\
// 					WarPoint = %d, CumulativeWarPoint = %d, ArenaWin = %d, ArenaLose = %d, ArenaDisConnect = %d, PCBangTotalPlayTime = %I64d \
// 					where uniqueNumber = %d",
// 					m_CurrentCharacter.CharacterName,
// 					m_CurrentCharacter.MapChannelIndex.MapIndex, m_CurrentCharacter.MapChannelIndex.ChannelIndex, m_CurrentCharacter.PositionVector.x, m_CurrentCharacter.PositionVector.z,
// 					m_CurrentCharacter.Race, m_CurrentCharacter.InfluenceType, m_CurrentCharacter.Level, m_CurrentCharacter.Experience, m_CurrentCharacter.CurrentHP,
// 					m_CurrentCharacter.HP, m_CurrentCharacter.CurrentDP, m_CurrentCharacter.DP, m_CurrentCharacter.CurrentSP,
// 					m_CurrentCharacter.SP, m_CurrentCharacter.CurrentEP, m_CurrentCharacter.EP, m_CurrentCharacter.GearStat.AttackPart,
// 					m_CurrentCharacter.GearStat.DefensePart, m_CurrentCharacter.GearStat.FuelPart, m_CurrentCharacter.GearStat.SoulPart, m_CurrentCharacter.GearStat.ShieldPart,
// 					m_CurrentCharacter.GearStat.DodgePart, m_CurrentCharacter.BonusStat, m_CurrentCharacter.BonusStatPoint,
// 					m_CurrentCharacter.Propensity, m_CurrentCharacter.RacingPoint, m_CurrentCharacter.WarPoint, 
// 					m_CurrentCharacter.CumulativeWarPoint, m_CurrentCharacter.ArenaWin, m_CurrentCharacter.ArenaLose,
// 					m_CurrentCharacter.ArenaDisConnect, m_CurrentCharacter.PCBangTotalPlayTime,
// 					m_CurrentCharacter.CharacterUniqueNumber);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	strQuery.Format(QUERY_080702_0048,		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//					m_CurrentCharacter.CharacterName,
//					m_CurrentCharacter.MapChannelIndex.MapIndex, m_CurrentCharacter.MapChannelIndex.ChannelIndex, m_CurrentCharacter.PositionVector.x, m_CurrentCharacter.PositionVector.z,
//					m_CurrentCharacter.Race, m_CurrentCharacter.InfluenceType, m_CurrentCharacter.Level, m_CurrentCharacter.Experience, m_CurrentCharacter.CurrentHP,
//					m_CurrentCharacter.HP, m_CurrentCharacter.CurrentDP, m_CurrentCharacter.DP, m_CurrentCharacter.CurrentSP,
//					m_CurrentCharacter.SP, m_CurrentCharacter.CurrentEP, m_CurrentCharacter.EP, m_CurrentCharacter.GearStat.AttackPart,
//					m_CurrentCharacter.GearStat.DefensePart, m_CurrentCharacter.GearStat.FuelPart, m_CurrentCharacter.GearStat.SoulPart, m_CurrentCharacter.GearStat.ShieldPart,
//					m_CurrentCharacter.GearStat.DodgePart, m_CurrentCharacter.BonusStat, m_CurrentCharacter.BonusStatPoint,
//					m_CurrentCharacter.Propensity, m_CurrentCharacter.RacingPoint, m_CurrentCharacter.WarPoint, 
//					m_CurrentCharacter.CumulativeWarPoint, m_CurrentCharacter.ArenaWin, m_CurrentCharacter.ArenaLose,
//					m_CurrentCharacter.ArenaDisConnect, m_CurrentCharacter.PCBangTotalPlayTime,
//					m_CurrentCharacter.CharacterUniqueNumber);
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)strQuery);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[35] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, m_CurrentCharacter.CharacterName, 0,	&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.MapChannelIndex.MapIndex), 0,		&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.MapChannelIndex.ChannelIndex), 0,	&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(m_CurrentCharacter.PositionVector.x), 0,					&arrCB2[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(m_CurrentCharacter.PositionVector.z), 0,					&arrCB2[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.Race), 0,							&arrCB2[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(m_CurrentCharacter.InfluenceType), 0,					&arrCB2[7]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(m_CurrentCharacter.Level), 0,							&arrCB2[8]);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &(m_CurrentCharacter.Experience), 0,							&arrCB2[9]);
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.HP), 0, &arrCB2[10]);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.DP), 0, &arrCB2[11]);
#else
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.HP), 0, &arrCB2[10]);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.DP), 0, &arrCB2[11]);
#endif
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.SP), 0,								&arrCB2[12]);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.EP), 0,								&arrCB2[13]);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(m_CurrentCharacter.CurrentHP), 0,							&arrCB2[14]);	// 2008-10-15 by cmkwon, AdminTool 버그 수정 - 
	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(m_CurrentCharacter.CurrentDP), 0,							&arrCB2[15]);	// 2008-10-15 by cmkwon, AdminTool 버그 수정 - 
	SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.CurrentSP), 0,						&arrCB2[16]);	// 2008-10-15 by cmkwon, AdminTool 버그 수정 - 
	SQLBindParameter(hstmt, 17, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &(m_CurrentCharacter.CurrentEP), 0,							&arrCB2[17]);	// 2008-10-15 by cmkwon, AdminTool 버그 수정 - 
	SQLBindParameter(hstmt, 18, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.GearStat.AttackPart), 0,				&arrCB2[18]);
	SQLBindParameter(hstmt, 19, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.GearStat.DefensePart), 0,			&arrCB2[19]);
	SQLBindParameter(hstmt, 20, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.GearStat.FuelPart), 0,				&arrCB2[20]);

	SQLBindParameter(hstmt, 21, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.GearStat.SoulPart), 0,				&arrCB2[21]);
	SQLBindParameter(hstmt, 22, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.GearStat.ShieldPart), 0,				&arrCB2[22]);
	SQLBindParameter(hstmt, 23, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(m_CurrentCharacter.GearStat.DodgePart), 0,				&arrCB2[23]);
	SQLBindParameter(hstmt, 24, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(m_CurrentCharacter.BonusStat), 0,						&arrCB2[24]);
	SQLBindParameter(hstmt, 25, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(m_CurrentCharacter.BonusStatPoint), 0,				&arrCB2[25]);
	SQLBindParameter(hstmt, 26, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.Propensity), 0,						&arrCB2[26]);
	SQLBindParameter(hstmt, 27, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.RacingPoint), 0,						&arrCB2[27]);
	SQLBindParameter(hstmt, 28, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.WarPoint), 0,							&arrCB2[28]);
	SQLBindParameter(hstmt, 29, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.CumulativeWarPoint), 0,				&arrCB2[29]);
	SQLBindParameter(hstmt, 30, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.ArenaWin), 0,							&arrCB2[30]);

	SQLBindParameter(hstmt, 31, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.ArenaLose), 0,							&arrCB2[31]);
	SQLBindParameter(hstmt, 32, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.ArenaDisConnect), 0,					&arrCB2[32]);
	SQLBindParameter(hstmt, 33, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &(m_CurrentCharacter.PCBangTotalPlayTime), 0,				&arrCB2[33]);
	SQLBindParameter(hstmt, 34, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.CharacterUniqueNumber), 0,			&arrCB2[34]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0048));
	if (!bRet)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0037);
		m_pODBCStmt->FreeStatement();

		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 캐릭터 정보를 다시 가져온다.
		if(dlg.m_bReloadCharacterInfo)
		{
			GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);
		}

		return;
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
	if( nOldRaceAccountType != (m_CurrentCharacter.Race & RACE_ACCOUNT_TYPE_MASK) )
	{
		// 여기서 로그 전송
		SendLogByRaceAccountTypeChange(nOldRaceAccountType, m_CurrentCharacter.Race & RACE_ACCOUNT_TYPE_MASK);
	}
	// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

	MessageBox(STRERR_S_SCADMINTOOL_0038);

	GetCharacterInfo((LPCSTR)m_szCurrentCharacterName);

	return;
}

// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
void CSCUserAdminDlg::SendLogByAccountTypeChange(USHORT i_nOldAccountType, USHORT i_nNewAccountType)
{
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE, T_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE, msgAccountType, msgAccountTypeBuf); 	
	util::strncpy(msgAccountType->AccountName, m_CurrentCharacter.AccountName, SIZE_MAX_ACCOUNT_NAME);
	msgAccountType->AccountUniqueNumber = m_CurrentCharacter.AccountUniqueNumber;
	msgAccountType->OldAccountType	= i_nOldAccountType;
	msgAccountType->NewAccountType	= i_nNewAccountType;
 	m_pUserAdminPreSocket->Write(msgAccountTypeBuf, MSG_SIZE(MSG_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE));
}

void CSCUserAdminDlg::SendLogByRaceAccountTypeChange(USHORT i_nOldAccountType, USHORT i_nNewAccountType)
{
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE, T_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE, msgAccountType, msgAccountTypeBuf); 	
	util::strncpy(msgAccountType->AccountName, m_CurrentCharacter.AccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(msgAccountType->CharacterName, m_CurrentCharacter.CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgAccountType->AccountUniqueNumber = m_CurrentCharacter.AccountUniqueNumber;
	msgAccountType->CharacterUniqueNumber = m_CurrentCharacter.CharacterUniqueNumber;
	msgAccountType->OldAccountType	= i_nOldAccountType;
	msgAccountType->NewAccountType	= i_nNewAccountType;
 	m_pUserAdminPreSocket->Write(msgAccountTypeBuf, MSG_SIZE(MSG_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE));
}
// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

void CSCUserAdminDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pUserAdminPreSocket != NULL && m_pUserAdminPreSocket->IsConnected())
	{
		m_pUserAdminPreSocket->CloseSocket();
	}
	util::del(m_pUserAdminPreSocket);

	CWinSocket::SocketClean();
}

void CSCUserAdminDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	UpdateData();

	if (m_bCurrentAccountBlocked && m_szAccountName != "")
	{
		int ret = AfxMessageBox(STRMSG_S_SCADMINTOOL_0039, MB_YESNO);
		if (ret == IDYES)
		{
			SendUnblockAccount(m_szAccountName, m_pMainDlg->m_UID);		// 2013-07-01 by jhseol, 어드민 툴 계정 unblock시 어드민 계정 남지 않는 버그 수정
		}
	}

	CDialog::OnCancel();
}

void CSCUserAdminDlg::OnButtonBlockAccount() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CBlockAccountDlg dlg(m_bCurrentAccountBlocked, m_szAccountName, &m_blockedAccountInfo);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 
// 	SendBlockAccount(m_szAccountName, dlg.m_nBlockType
// 		, dlg.m_atimeStart, dlg.m_atimeEnd, dlg.m_ctl_strAdminName, dlg.m_szBlockReason, dlg.m_szBlockReasonForOnlyAdmin);	// 2007-01-10 by cmkwon, 수정
	SendBlockAccount(m_szAccountName, dlg.m_nBlockType
		, dlg.m_atimeStart, dlg.m_atimeEnd, m_pMainDlg->m_UID, dlg.m_szBlockReason, dlg.m_szBlockReasonForOnlyAdmin);	// 2007-01-10 by cmkwon, 수정
}

void CSCUserAdminDlg::OnButtonBlockEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	CBlockAccountDlg dlg(m_bCurrentAccountBlocked, m_szAccountName, &m_blockedAccountInfo);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 
// 	SendBlockAccount(m_szAccountName, dlg.m_nBlockType
// 		, dlg.m_atimeStart, dlg.m_atimeEnd, dlg.m_ctl_strAdminName, dlg.m_szBlockReason, dlg.m_szBlockReasonForOnlyAdmin);	// 2007-01-10 by cmkwon, 수정
 	SendBlockAccount(m_szAccountName, dlg.m_nBlockType
 		, dlg.m_atimeStart, dlg.m_atimeEnd, m_pMainDlg->m_UID, dlg.m_szBlockReason, dlg.m_szBlockReasonForOnlyAdmin);	// 2007-01-10 by cmkwon, 수정
}


void CSCUserAdminDlg::OnButtonUnblockAccount() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (IDYES != AfxMessageBox(STRMSG_S_SCADMINTOOL_0039, MB_YESNO))
	{
		return;
	}

	//SendUnblockAccount(m_szAccountName);
	SendUnblockAccount(m_szAccountName, m_pMainDlg->m_UID);		// 2012-06-05 by mspark, 계정 블럭 ID와 계정 언블럭 ID 구분 작업
}

void CSCUserAdminDlg::OnButtonEditAccount() 
{
	// TODO: Add your control notification handler code here
	CString tmpAccountName;
	GetDlgItem(IDC_EDIT_ACCNAME)->GetWindowText(tmpAccountName);
	if (tmpAccountName == "")
	{
		return;
	}

// 2009-02-23 by cmkwon, 사용하지 않음
//	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];
	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
	CAccountInfoDlg dlg(m_pMainDlg->IsManagerAdministrator(), this);
// 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 아래와 같이 GetSqlPattern()를 사용하지 않음
//	dlg.m_szAccountName = GetSqlPattern(tmpAccountName, spbuf);
	dlg.m_szAccountName = tmpAccountName;
	dlg.m_pODBCStmt = m_pODBCStmt;
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
	if( TRUE == dlg.m_bChangeAccountType )
	{
		SendLogByAccountTypeChange(dlg.m_nOldAcountType , dlg.m_nAcountType & RACE_ACCOUNT_TYPE_MASK);
	}
	// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

	GetDlgItem(IDC_EDIT_ACC_TYPE)->SetWindowText(CAtumSJ::GetRaceString(dlg.m_nAcountType));
}


void CSCUserAdminDlg::OnTimerForSendAlivePacket(void)
{
	if(m_pUserAdminPreSocket
		&& m_pUserAdminPreSocket->IsConnected())
	{
		m_pUserAdminPreSocket->WriteMessageType(T_PM_CONNECT_ALIVE);
	}
}

void CSCUserAdminDlg::OnButtonEditPremium() 
{
	// TODO: Add your control notification handler code here
	CString tmpAccountName;
	GetDlgItem(IDC_EDIT_ACCNAME)->GetWindowText(tmpAccountName);
	if (tmpAccountName.IsEmpty()
		|| FALSE == m_bIsValidAccountName
		|| 0 == m_uidAccountUID)
	{
		return;
	}

	if(this->IsEnabledEdit())
	{// 2006-04-15 by cmkwon, 수정 권한이 있을때만 체크
		ATUM_DATE_TIME atimeCur(true);
		INT nSecondGap = m_blockedAccountInfo.atimeEndTime.GetTimeDiffTimeInSeconds(atimeCur);	
		if(nSecondGap < 12*60*60)
		{
			MessageBox("More than 12hour must remain to account block cancellation.");	// 계정블럭 해제까지 1일 이상이 남아 있어야 한다.
			return;
		}
	}

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 -
	CPremiumInfoDlg dlg(this->IsEnabledEdit(), tmpAccountName, m_uidAccountUID, m_pODBCStmt, this);
	if (IDOK != dlg.DoModal())
	{
		return;
	}
}

void CSCUserAdminDlg::OnCmgItemInformation() 
{
	// TODO: Add your command handler code here
// 2007-12-06 by cmkwon, 필요 없는 코드임
//	if (m_bCurrentCharacterOnline)
//	{
//		return;
//	}

	CCellID cellID = m_GridItem.GetFocusCell();
	if(0 > cellID.row)
	{// 2005-12-12 by cmkwon, 선택 오류
		return;
	}

	ITEM_GENERAL_READABLE itemToMod;
	GetItemGeneralInRow(cellID.row, &itemToMod);

	// 2008-06-19 by cmkwon, AdminTool 수정(기어별 인챈트 추가 할수 있게) - 
	itemToMod.ItemInfo = m_mapItemInfo.findEZ_ptr(itemToMod.ItemNum);
	if(NULL == itemToMod.ItemInfo)
	{
		MessageBox("Invalid ItemNum !!");
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-24 by cmkwon, AdminTool수정:해당 아이템의 인챈트 리스트 보여주기 - CSCUserAdminDlg::OnCmgItemInformation#
	vectINT enchantItemNumList;
	vectENCHANT  enchantInfo;				// 2013-02-27 by bckim, 인챈트 리스트순서 재정렬(아이템넘버순서->인챈트된순서)
	if(FALSE == this->DBQ_LoadEnchantList(&enchantItemNumList, itemToMod.UniqueNumber, itemToMod.Kind, &enchantInfo))
	{
		return;
	}

	CModifyItemDlg	dlg(this, TRUE);
	dlg.m_nItemNum				= itemToMod.ItemNum;
	dlg.m_szItemName			= itemToMod.ItemName;
	dlg.m_nItemCount			= itemToMod.CurrentCount;
	dlg.m_nItemPrefixCodeNum	= itemToMod.PrefixCodeNum;
	dlg.m_nItemSuffixCodeNum	= itemToMod.SuffixCodeNum;
	dlg.m_ctl_nEnchantCount		= itemToMod.EnchantCount;
	dlg.m_nArmorColorItemNum	= itemToMod.ColorCode;			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - EffectItemNum으로 사용	// 2005-12-08 by cmkwon
	dlg.m_nShapeItemNum			= itemToMod.ShapeItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가
	dlg.m_nItemKind				= itemToMod.Kind;
	dlg.m_usReqUnitKind			= itemToMod.ItemInfo->ReqUnitKind;	// 2008-06-19 by cmkwon, AdminTool 수정(기어별 인챈트 추가 할수 있게) - 
	dlg.m_vectEnchantItemNumList	= enchantItemNumList;			// 2010-02-24 by cmkwon, AdminTool수정:해당 아이템의 인챈트 리스트 보여주기 - 
	dlg.m_vectEnchantOrder			= enchantInfo;					// 2013-02-27 by bckim, 인챈트 리스트순서 재정렬(아이템넘버순서->인챈트된순서)
	if (IDOK != dlg.DoModal())
	{
		return;
	}
}

void CSCUserAdminDlg::OnBtnBlockedList() 
{
	// TODO: Add your control notification handler code here
	CBlockAccountListDlg	dlg(m_pODBCStmt);
	dlg.DoModal();
}

void CSCUserAdminDlg::OnBtnInflwarList() 
{
	// TODO: Add your control notification handler code here
	CInflWarListDlg	dlg(m_pMainDlg->IsManagerAdministrator(), m_pODBCStmt);
	dlg.DoModal();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCUserAdminDlg::OnButtonDeletedCharacter() 
/// \brief		
/// \author		dhjin
/// \date		2007-02-21 ~ 2007-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCUserAdminDlg::OnButtonDeletedCharacter() 
{
	// TODO: Add your control notification handler code here

	if(FALSE == this->IsEnabledEdit())
	{
		MessageBox("Permission denied !!");
		return;
	}

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - CSCUserAdminDlg 를 인자로 넣어 준다.
	CDeletedCharacter dlg(m_blockedAccountInfo.szBlockedAccountName, m_pODBCStmt, this);
	dlg.DoModal();

	this->OnButtonGetUserInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCUserAdminDlg::OnBtnInitInfluence() 
{
	// TODO: Add your control notification handler code here
	CString csMsg;

	csMsg.Format("Do you want to initialize influence of all character ?");
	if(IDYES != AfxMessageBox(csMsg, MB_YESNO))
	{
		return;
	}

	csMsg.Format("Did you close group game servers(FieldServer, IMServer,...)?");
	if(IDYES != AfxMessageBox(csMsg, MB_YESNO))
	{
		return;
	}

	csMsg.Format("Are you sure initialize influence of all character?");
	if(IDYES != AfxMessageBox(csMsg, MB_YESNO))
	{
		return;
	}

	if(FALSE == this-DBQ_InitializeInfluenceAllCharacter())
	{
		AfxMessageBox("Fail to initialize influence of all character !!");
		return;
	}

	AfxMessageBox("Success !!");
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCUserAdminDlg::DBQ_InitializeInfluenceAllCharacter(void)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: atum_Init_InfluenceType_AllCharacter
	-- DESC				: // 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	--						// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
	--						
	--						#define QUEST_STATE_NONE			(BYTE)0		// 시작하지 않은 상태
	--						#define QUEST_STATE_IN_PROGRESS		(BYTE)1		// 진행중
	--						#define QUEST_STATE_COMPLETED		(BYTE)2		// 완료
	--
	--						#define QUEST_INDEX_OF_SELECT_INFLUENCE				112
	--------------------------------------------------------------------------------
	**************************************************************************/
	BOOL bRet = m_pODBCStmt->ExecuteQuery(PROCEDURE_090401_0344);
	m_pODBCStmt->FreeStatement();
	if (!bRet)
	{		
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-09-02 ~ 2009-09-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCUserAdminDlg::GetItemInfoListByDesParam(vectItemPtr *o_pVectItemList, DestParam_t i_byDesParam)		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
{
	ez_map<INT, ITEM>::iterator itr = m_mapItemInfo.begin();
	for(; itr != m_mapItemInfo.end(); itr++)
	{
		ITEM *pItemInfo = &itr->second;
		if(pItemInfo->IsExistDesParam(i_byDesParam))
		{
			o_pVectItemList->push_back(pItemInfo);
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-02-24 by cmkwon, AdminTool수정:해당 아이템의 인챈트 리스트 보여주기 - 
/// \author		cmkwon
/// \date		2010-02-24 ~ 2010-02-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCUserAdminDlg::DBQ_LoadEnchantList(vectINT *o_pVecEnchantItemNumList, UID64_t i_ItemUID, BYTE i_byItemKind, vectENCHANT* o_pEnchantInfo /*= NULL*/ )
{
	o_pVecEnchantItemNumList->clear();
	o_pEnchantInfo->clear();			// 2013-02-27 by bckim, 인챈트 리스트순서 재정렬(아이템넘버순서->인챈트된순서)
	if(FALSE == IS_ENCHANT_TARGET_ITEMKIND(i_byItemKind))
	{
		return TRUE;
	}

	/*[Stored Query Definition]************************************************
	-- !!!!
	-- Name: atum_GetEnchantBYItemUID
	-- Desc: 2007-01-30 by cmkwon, 추가함
	-- ====
	CALL atum_GetEnchantBYItemUID
	**************************************************************************/
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &i_ItemUID, 0, &arrCB[1]);
	SQLRETURN ret = SQLExecDirect(hstmt, PROCEDURE_080822_0075, SQL_NTS);
	if (ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		AfxMessageBox("db query fail(PROCEDURE_080822_0075)!!");
		return FALSE;
	}
	if(SQL_NO_DATA == ret)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return TRUE;
	}
		
	ENCHANT tmpEnchant;
	arrCB[1] = arrCB[2] = arrCB[3] = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmpEnchant.TargetItemNum, 0,			&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_LONG, &tmpEnchant.EnchantItemNum, 0,			&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_SBIGINT, &tmpEnchant.SequenceNumber_DB, 0,	&arrCB[3]);		// 2013-01-15 by bckim, 인챈트아이템 시퀀스넘버 추가

	util::zero(&tmpEnchant, sizeof(ENCHANT));
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		o_pVecEnchantItemNumList->push_back(tmpEnchant.EnchantItemNum);
		o_pEnchantInfo->push_back(tmpEnchant);			// 2013-02-27 by bckim, 인챈트 리스트순서 재정렬(아이템넘버순서->인챈트된순서)
		util::zero(&tmpEnchant, sizeof(ENCHANT));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-21 ~ 2009-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCUserAdminDlg::OnBTNSETInfluenceRate() 
{
	// TODO: Add your control notification handler code here
	
	// 2009-11-11 by cmkwon, AdminTool 세력 초기화시 비율 설정 관련 버그 수정 - 
	//CDlgSetInfluenceRate dlg(this->IsEnabledEdit(), m_pODBCStmt);
	CDlgSetInfluenceRate dlg(m_pMainDlg->IsManagerAdministrator(), m_pODBCStmt);

	dlg.DoModal();
}

void CSCUserAdminDlg::OnBtnInfinityResetImpute() 
{
	// TODO: Add your control notification handler code here
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 귀속 정보 리셋
	if(FALSE == m_bCurrentAccountBlocked) {
		AfxMessageBox("If you want to update, you must User Block !!");
		return;		
	}

	if(0 == m_CurrentCharacter.CharacterUniqueNumber) {
		AfxMessageBox("Invalid CharacterUniqueNumber !!");
		return;		
	}

	if(IDCANCEL == AfxMessageBox("Do you want to initialize all Infinity belonging information of the character?", MB_OKCANCEL)) {
		return;
	}

	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(m_CurrentCharacter.CharacterUniqueNumber), 0,NULL);

	BOOL bRet = m_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0532);
	m_pODBCStmt->FreeStatement();
	if (!bRet) {		
		AfxMessageBox("Fail to Infinity belonging information initialization !!");
		return;
	}
	
	AfxMessageBox("Success !!");

	return;
}

// 2013-01-07 by khkim, 베트남 GM 권한수정
 void CSCUserAdminDlg::UserManagementToolControls()
 {
#ifdef S_MANAGER_ADMIN_HSSON
	switch(m_pMainDlg->m_usManagerAccountType)
	{
	case RACE_GAMEMASTER:
		{
			GetDlgItem(IDC_BUTTON_EDIT_ACCOUNT)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_EDIT_CHARACTER)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_EDIT_PREMIUM)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_INFLWAR_LIST)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_INIT_INFLUENCE)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_SET_InfluenceRate)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_INFINITY_RESET_IMPUTE)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_BLOCKED_LIST)->EnableWindow( TRUE );
			break;
		}
	default:
		break;
	}
#endif
 }
// end 2012-01-07 by khkim, 베트남 GM 권한수정
