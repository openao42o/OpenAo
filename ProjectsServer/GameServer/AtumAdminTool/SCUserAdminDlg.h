// SCUserAdminDlg.h : header file
//

#if !defined(AFX_SCUSERADMINDLG_H__F23BF76C_BE5C_4CA6_BB07_CBB949EC9806__INCLUDED_)
#define AFX_SCUSERADMINDLG_H__F23BF76C_BE5C_4CA6_BB07_CBB949EC9806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl/GridCtrl.h"
#include "ODBCStatement.h"
#include "AtumDBHelper.h"
#include "atumparam.h"
#include "./Main/SCAdminWinSocket.h"
#include "resource.h"

class CAtumAdminToolDlg;

struct ITEM_GENERAL_READABLE : public ITEM_GENERAL
{
	char			ItemName[SIZE_MAX_ITEM_NAME];
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	int				EnchantCount;
// 2009-11-18 by cmkwon, 사용하지 않은 변수 주석 처리 - 
//	ATUM_DATE_TIME	CreatedTime;

	ITEM_GENERAL_READABLE() : ITEM_GENERAL(NULL)
	{	
	};
};
typedef vector<ITEM_GENERAL_READABLE>		vectITEM_GENERAL_READABLE;

// PreIOCP.h의 CP_State와 동일해야 함
typedef enum
{
	CP_NOTCONNECTED					= 0,	// Client가 접속하지 않은 상태
	CP_CONNECTED					= 1,	// Client가 Socket 접속한 상태
	CP_SINGLE_FILE_VERSIONCHECKED	= 2,	// Client가 SINGLE FILE VERSION이 update된 상태
	CP_VERSIONCHECKED				= 3,	// Client가 VERSION이 update된 상태
	CP_LOGINED						= 4,	// Client가 AccountName을 가지고 Login 된 상태, Launcher에 의한 로그인
	CP_FIELD_LOGINED				= 5,	// Client가 필드 서버에 의해 로그인 된 상태, 캐릭터 선택 화면에 있는 상황
	CP_GAMESTARTED					= 6		// Client가 게임을 시작하여 맵에 있는 상황
} CP_State;

const char *GetCPState(int i_nCPState);

enum enumGetAccountQueryType
{
	GAQT_ACCOUNT_NAME	= 0,
	GAQT_CHARACTER_NAME	= 1,
	GAQT_ACCOUNT_UID	= 2,
	GAQT_CHARACTER_UID	= 3
};

/////////////////////////////////////////////////////////////////////////////
// CSCUserAdminDlg dialog
class CAtumAdminToolApp;
class CSCUserAdminDlg : public CDialog
{
friend class CSelectItemDlg;
friend class CModifyItemDlg;
friend class CCharacterInfoDlg;
friend class CPartnerDlg;				// 2012-03-15 by jhseol, 파트너 - 파트너 클래스 등록

// Construction
public:
	CSCUserAdminDlg(CWnd* pParent = NULL);	// standard constructor
	~CSCUserAdminDlg();


	void OnTimerForSendAlivePacket(void);

// Dialog Data
	//{{AFX_DATA(CSCUserAdminDlg)
	enum { IDD = IDD_DIALOG_USER_ADMIN_TOOL };
	CString	m_szAccountNameInput;
	CString	m_szAccountName;
	CString	m_CharacterName1;
	CString	m_CharacterName2;
	CString	m_CharacterName3;
	CString	m_szUsetStatus;
	int		m_nAccCharac;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCUserAdminDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// 서버 연결
	BOOL ConnectServer();

	// grid 관련
	void InitGridItem();
	void InsertItemToGrid(ITEM_GENERAL_READABLE *i_pItemGeneral, int i_nRow);
	BOOL GetItemGeneralInRow(int i_nRow, ITEM_GENERAL_READABLE *o_pItem);

	// 유저 정보 관련
	void GetAccountInfo(const char *i_szName, enumGetAccountQueryType i_nGAQT);
	void GetCharacterInfo(const char *i_szCharacterName);
	void GetItemInfo(const char *i_szCharacterName);
	BOOL DisplayCharacterInfo(CHARACTER *i_pCharacter);
	BOOL FindItemByItemNum(int i_nItemNum);
	BOOL GetInfluenceLeaderCharacterInfo(BYTE i_byInflType);
	
	// DB
	BOOL InsertItemToDB(const char *i_szCharacterName, int i_nItemNum, int i_nItemCount);
	BOOL GetAccountBlockState(const char *i_szAccountName);
	void SendBlockAccount(const char *i_szAccName, int i_nBlockTy, ATUM_DATE_TIME i_atimeS, ATUM_DATE_TIME i_atimeE, const char *i_szAdminName, const char *i_szReason, const char *i_szReasonForOnlyAdmin);
	//void SendUnblockAccount(const char *i_szAccName);
	void SendUnblockAccount(const char *i_szAccName, const char *i_szAdminName = NULL);		// 2012-06-05 by mspark, 계정 블럭 ID와 계정 언블럭 ID 구분 작업

	// control 제어
	void EnableAllOuputControls(BOOL i_bEnable = TRUE);
	void ResetControls(BOOL i_bOnlyOutPut = FALSE);
	void EnableControls_GetAccountInfo ( const MSG_PA_ADMIN_GET_ACCOUNT_INFO_OK *a_pPacket_AccountInfo );				// 유저 정보 받았을 때의 기본 메뉴 컨트롤 설정.
	void UserManagementToolControls ();			// 2013-01-07 by khkim, 베트남 GM 권한수정 

	// character 창 관련
	void InitGridCharacter();
	BOOL AddLineToGridCharacter(const char *i_szKey, const char *i_szValue);
	BOOL AddLineToGridCharacter(const char *i_szKey, int i_nValue);
	BOOL AddLineToGridCharacter(const char *i_szKey, float i_fValue);

	// 레어정보 보여주기
	char *GetRareString(char *o_szRareString, char *i_szRareName, INT i_nRareCodeNum);
	int GetRareCodeNum(const char *i_szRareString);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	BOOL DBQ_InitializeInfluenceAllCharacter(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	void GetItemInfoListByDesParam(vectItemPtr *o_pVectItemList, DestParam_t i_byDesParam);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-24 by cmkwon, AdminTool수정:해당 아이템의 인챈트 리스트 보여주기 - 

	// 2013-02-27 by bckim, 인챈트 리스트순서 재정렬(아이템넘버순서->인챈트된순서)
	//BOOL DBQ_LoadEnchantList(vectINT *o_pVecEnchantItemNumList, UID64_t i_ItemUID, BYTE i_byItemKind);
	BOOL DBQ_LoadEnchantList(vectINT *o_pVecEnchantItemNumList, UID64_t i_ItemUID, BYTE i_byItemKind, vectENCHANT* o_pEnchantInfo = NULL);
	// End. 2013-02-27 by bckim, 인챈트 리스트순서 재정렬(아이템넘버순서->인챈트된순서)

	// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
	void SendLogByAccountTypeChange(USHORT i_nOldAccountType, USHORT i_nNewAccountType);
	void SendLogByRaceAccountTypeChange(USHORT i_nOldAccountType, USHORT i_nNewAccountType);
	// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

public:
	BOOL IsEnabledEdit(void);
	
	// registry
	void ReadProfile();
	void WriteProfile();

// Implementation
protected:
	CAtumAdminToolDlg				*m_pMainDlg;

	HICON m_hIcon;

	CSCAdminPreWinSocket			*m_pUserAdminPreSocket;
	CODBCStatement					*m_pODBCStmt;

	CGridCtrl						m_GridItem;
	CGridCtrl						m_GridCharacter;

	ez_map<INT, ITEM>				m_mapItemInfo;
	ez_map<string, ITEM>			m_mapItemInfoName;
	ez_map<INT, RARE_ITEM_INFO>		m_mapRareItemInfo;
	ez_map<string, RARE_ITEM_INFO>	m_mapRareItemInfoName;
	ez_map<INT, ENCHANT_INFO>		m_mapEnchantInfo;		// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완

	CString							m_szCurrentCharacterName;	// 현재 선택된 캐릭터명
	CHARACTER						m_CurrentCharacter;			// 현재 선택된 캐릭터
	CHARACTER						m_InflLeaderCharacter;		// 2006-04-25 by cmkwon, 세력지도자 정보(CharacterUniqueNumber, CharacterName만 유효함)
	CHARACTER						m_InflSub1LeaderCharacter;	// 2006-12-08 by dhjin, 부지도자1
	CHARACTER						m_InflSub2LeaderCharacter;	// 2006-12-08 by dhjin, 부지도자2
	BOOL							m_bCurrentCharacterOnline;	// TRUE이면 NOT-Editable임
	BOOL							m_bCurrentAccountBlocked;	// FALSE이면 NOT-Editable임
	SBLOCKED_ACCOUNT_INFO			m_blockedAccountInfo;
	
	BOOL							m_bIsValidAccountName;
	UID32_t							m_uidAccountUID;

	vectITEM_GENERAL_READABLE		m_vectCurrentCharacterGetItemList;		// 2012-03-15 by jhseol, 파트너 - 선택된 캐릭터가 보유한 아이템 리스트

	// Generated message map functions
	//{{AFX_MSG(CSCUserAdminDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOk();
	afx_msg void OnCmgInsertItem();
	afx_msg void OnCmgDeleteItem();
	afx_msg void OnCmgModifyItem();
	afx_msg void OnButtonGetUserInfo();
	afx_msg void OnButtonCharac1Ok();
	afx_msg void OnButtonCharac2Ok();
	afx_msg void OnButtonCharac3Ok();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonReload();
	afx_msg void OnButtonEditCharacter();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnButtonBlockAccount();
	afx_msg void OnButtonUnblockAccount();
	afx_msg void OnButtonEditAccount();
	afx_msg void OnButtonBlockEdit();
	afx_msg void OnButtonEditPremium();
	afx_msg void OnCmgItemInformation();
	afx_msg void OnBtnBlockedList();
	afx_msg void OnBtnInflwarList();
	afx_msg void OnButtonDeletedCharacter();
	afx_msg void OnBtnInitInfluence();
	afx_msg void OnBTNSETInfluenceRate();
	afx_msg void OnBtnInfinityResetImpute();
	//}}AFX_MSG
    afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg LONG OnSocketNotifyPre(WPARAM wParam, LPARAM lParam);
//	afx_msg LONG OnSocketNotifyIM(WPARAM wParam, LPARAM lParam);
//	afx_msg LONG OnSocketNotifyField(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGridDClick(NMHDR *pNotifyStruct, LRESULT* pResult);		// 2012-03-15 by jhseol, 파트너 - 더블클릭 이벤트 추가
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCUSERADMINDLG_H__F23BF76C_BE5C_4CA6_BB07_CBB949EC9806__INCLUDED_)
