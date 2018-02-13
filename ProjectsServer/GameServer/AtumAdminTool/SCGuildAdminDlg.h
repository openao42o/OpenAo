#if !defined(AFX_SCGUILDADMINDLG_H__6609BE4A_6DF3_4E16_A2B5_A4DC38C5A64D__INCLUDED_)
#define AFX_SCGUILDADMINDLG_H__6609BE4A_6DF3_4E16_A2B5_A4DC38C5A64D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCGuildAdminDlg.h : header file
//
#include "AtumProtocol.h"
#include "GridCtrl/GridCtrl.h"
#include "SCUserAdminDlg.h"
#include "EditGuildDlg.h"

// 2007-07-31 by cmkwon, SCAdminTool에 여단명 변경 기능 추가 - EditGuildDlg.h 파일로 이동함
//struct SGUILD		// 2006-03-07 by cmkwon
//{
//	UID32_t				GuildUID;
//	char				GuildName[SIZE_MAX_GUILD_NAME];			// 길드명
//	UID32_t				GuildMasterCharacterUID;				// 여단장 CharacterUniqueNumber
//	INT					GuildMemberCapacity;					// 가입 가능한 길드원의 최대 수
//	BYTE				GuildState;
//	ATUM_DATE_TIME		GuildDismemberDate;
//	UINT				GuildMarkVersion;
//	INT					GuildMarkSize;
//	INT					WarWinPoint;							// 여단전 승리 수
//	INT					WarLossPoint;							// 여단전 패배 수
//	INT					TotalFame;								// 여단 총 누적 명성
//	INT					MonthlyFame;							// 여단 한달 누적 명성
//	char				GuildMarkImage[SIZE_MAX_GUILD_MARK_IMAGE];	// 2007-07-26 by cmkwon, SCAdminTool에 GuildManager 수정 - 여단마크도 가져오기
//};
/////////////////////////////////////////////////////////////////////////////
// CSCGuildAdminDlg dialog
class CODBCStatement;
class CAtumAdminToolDlg;
class CSCGuildAdminDlg : public CDialog
{
// Construction
public:
	CSCGuildAdminDlg(CWnd* pParent = NULL);   // standard constructor
	~CSCGuildAdminDlg();

	void InitGridGuild(void);
	void InitGridGuildMembers(void);
	void InitGridGuildItems(void);				// 2006-09-27 by cmkwon

	BOOL AddLineToGridGuildMembers(const char *i_szKey, const char *i_szValue);
	BOOL AddLineToGridGuildMembers(const char *i_szKey, int i_nValue);
	BOOL AddLineToGridGuildMembers(const char *i_szKey, UINT i_uiValue);
	BOOL AddLineToGridGuildMembers(const char *i_szKey, float i_fValue);
	BOOL AddLineToGridGuildMembers(const char *i_szKey, CBitmap *i_pbmGuildMark);	// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - AddLineToGridGuildMembers() 함수 더 추가

	BOOL DisplayGuildInfo(SGUILD *i_pGUILD, UID32_t i_nGuildMemberCardCashPrice);		// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
	BOOL GetGuildInfo(const char *i_szGuildName, UID32_t i_guildUID=INVALID_UID32);		// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - 인자추가(, UID32_t i_guildUID=INVALID_UID32)
	BOOL GetGuildMembersInfo(SGUILD *i_pGUILD);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-27 by cmkwon
	BOOL GetGuildItems(SGUILD *i_pGuild);
	void InsertItemToGridGuildItems(ITEM_GENERAL_READABLE *i_pItemGeneral, int i_nRow);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - LoadBMPFileFromMemory() 함수 추가
	static BOOL LoadBMPFileFromMemory(CBitmap *o_pBitmap, char *i_pData, int i_nDataLen);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-31 by cmkwon, SCAdminTool에 여단명 변경 기능 추가 - DB에서 여단명 검색 함수 추가
	BOOL SearchGuildNameFromDB(char *i_szGuildName);
	BOOL UpdateGuild2DB(SGUILD *i_pGuild);
public:
	CGridCtrl				m_GridGuild;
	CGridCtrl				m_GridGuildMembers;
	CGridCtrl				m_GridGuildItems;				// 2006-09-27 by cmkwon
	
	CAtumAdminToolDlg		*m_pMainDlg;
	CODBCStatement			*m_pODBCStmt;

	SGUILD					m_CurGUILD; 

	ez_map<INT, ITEM>				m_mapItemInfo;
	ez_map<INT, RARE_ITEM_INFO>		m_mapRareItemInfo;

	CImageList				m_ilGuildMarkList;			// 2007-07-26 by cmkwon, SCAdminTool에 GuildManager 수정 - 여단 마크 리스트 멤버 변수 추가

// Dialog Data
	//{{AFX_DATA(CSCGuildAdminDlg)
	enum { IDD = IDD_DLG_GUILD_ADMIN };
	CString	m_ctlstrSearchGuildName;
	UINT	m_ctluiSearchGuildUID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCGuildAdminDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSCGuildAdminDlg)
	afx_msg void OnBtnSearchguild();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSearchguildByguid();
	afx_msg void OnPaint();
	afx_msg void OnBtnEditGuild();
	virtual void OnOK();
	afx_msg void OnBtnGuildMarkRecognition();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCGUILDADMINDLG_H__6609BE4A_6DF3_4E16_A2B5_A4DC38C5A64D__INCLUDED_)
