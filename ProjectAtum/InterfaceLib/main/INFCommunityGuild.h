// INFCommunityGuild.h: interface for the CINFCommunityGuild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYGUILD_H__182E95F2_E402_480F_86DB_26C11C5016E6__INCLUDED_)
#define AFX_INFCOMMUNITYGUILD_H__182E95F2_E402_480F_86DB_26C11C5016E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define BUTTON_MAKE							0	// 생성
#define BUTTON_LEAVE						1	// 탈퇴
#define BUTTON_INVITE						2	// 초대
#define BUTTON_MARK							3	// 마크
#define BUTTON_BANMEMBER					4	// 추방
#define BUTTON_DISBAND						5	// 해체
#define BUTTON_CANCEL						6	// 해체 취소
#define BUTTON_NUMBER						7	// 위의 총합

#define BUTTON_VOIP							10	// VOIP

//#define MAX_GUILD_NUMBER					9	// 화면상 라인 수
#define MAX_GUILD_NUMBER					13	// 화면상 라인 수


// 2008-06-16 by bhsohn EP3관련 버그수정
#define MAX_COMBO_GUILD_RANK				13	// 길드 랭크 개수
#define BUTTON_STATE_VOIP					4

// 2008-04-04 by bhsohn Ep3 커뮤니티 창
#define GUILD_TAB_ID						0	// 아이디
#define GUILD_TAB_GEAR						1	// 기어
#define GUILD_TAB_LEVEL						2	// 레벨
#define GUILD_TAB_RANK						3	// 계급
#define GUILD_TAB_VOICE						4	// 음성
#define GUILD_TAB_CONNECT					5	// 접속
#define MAX_GUILD_TAB						6
// end 2008-04-04 by bhsohn Ep3 커뮤니티 창


//typedef vector<MEX_GUILD_MEMBER_INFO*>				CVectorGuildMemberInfo;
//typedef vector<MEX_GUILD_MEMBER_INFO*>::iterator	CVectorGuildMemberInfoIterator;

///////////////////////////////////////////////////////////////////////////////
/// \class		CINFCommunityGuild
///
/// \brief		길드 정보, GUI
/// \author		dhkwon
/// \version	
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	SendGetGuildInfo -> resetGuildInfo -> AddGuildMember(n)
///////////////////////////////////////////////////////////////////////////////

class CINFMessageBox;
class CINFImage;
class CD3DHanFont;
class CINFScrollBar;
class CINFComboBox;
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFPopupMenu;	// 메뉴창
class CINFImageComboBox;		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFCommunityGuildManager;
//class CINFEditBox;
class CINFCurselEditBox;	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
class CINFGroupImage;
class CINFImageEx;
class CEnemyData; // 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시

class CINFCommunityGuild : public CINFBase  
{
public:
	CINFCommunityGuild(CAtumNode* pParent);
	virtual ~CINFCommunityGuild();

	BOOL IMSocketSendGuildSetRank( int nGuildRank, UID32_t	MemberUniqueNumber);
	void IMSocketSendGetGuildInfo(UINT nGuildUniqueNumber);
	void IMSocketSendGuildMark(char* szFileName);
	void ResetGuildInfo(MSG_IC_GUILD_GET_GUILD_INFO_OK *pInfo, BOOL bEraseMemberList);
	void AddGuildMember(MEX_GUILD_MEMBER_INFO* pInfo);
	void DeleteGuildMember(UINT nUniqueNumber);
	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//MEX_GUILD_MEMBER_INFO* GetGuildMemberInfo(UINT nUniqueNumber);
	structGuildInfo* GetGuildMemberInfo(UINT nUniqueNumber);

	MSG_IC_GUILD_GET_GUILD_INFO_OK * GetGuildInfo() { return m_pGuildInfo; }
	void RenderGuildMark(int x, int y, int nGuildUniqueNumber, int nCharacterUniqueNumber);
	void RenderGuildRank(int x, int y, int nGuildRank );

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render(POINT ptPos);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);

	BYTE GetMyGuildRank() { return m_nGuildRank; }
	static constexpr bool GetNtoNCallState() { return false; } // { return m_bNtoNCallState; }
	void SetNtoNCallState(BOOL bState);						// 여단통화중!
	void IMSocketSendVoipNtoNInviteUser( UID32_t nMemberUniqueNumber );
	BOOL pGuildQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y);
	void SetMaxGuildMember(int nNum);
	

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//CVectorGuildMemberInfo * GetGuildMemberList(){return &m_vecGuildMember;}
	vector<structGuildInfo*>* GetGuildMemberList(){return &m_stGuildInfo;}

	void RefreshGuildInfo();	// 길드 정보
	void SetGuildOnOffInfo(UINT nUniqueNumber, BYTE IsOnline);	// 길드원 접속 여부	
	void SetLevelInfo(UID32_t CharacterUID, INT	nLevel);	// 레별 변경
	void SetGuildDSetRank(UINT nUniqueNumber, BYTE GuildRank);	// 길드원 랭킹
	void CloseGuildWnd();
	void OnClickAppoint(); // 임명
	void SetNoticeWrite(char* pNotice);

	// 2008-06-18 by bhsohn 여단원증가 카드 관련 처리
	void SetGuildChangeMemberShip(MSG_IC_GUILD_CHANGE_MEMBERSHIP* pMsg);

	BYTE GetGuildOrderIdx(BYTE byRank);

	void UpdateBtnPos();

	BOOL SetVoiceGuild(UID32_t i_CharacterUID);
	BOOL NoneVoiceGuild(UID32_t i_CharacterUID);
	BOOL IsGuildMember()  {		return m_bUIGuildMember;	}			// 길드가 있는가?

	void SetGuildRank(INT nGuildTotalFameRank, INT nGuildMonthlyFameRank);

	// 2008-10-14 by bhsohn VOIP개선
	void SetVOIPSpeaking(char *i_pUserName, BOOL i_bEnableTalking);
	void ReleaseVOIPInfo(char *i_pUserName);

	// 2008-10-20 by bhsohn VOIP 버그 수정
	BOOL IsVoiceONGuildMaster();
	void ReleaseVOIPGuildInfo();	
	

protected:
	void RenderGuildMemberList(BYTE nGuildState);	
	void DeleteAllGuildInfo();	
	void OnClickPopupMenu(int nPopupMenuId);	
	void RenderQuildRanking();
	void RefreshGuildRankCombo();	
	void SetGuildMemberSelect(int nGuildSelect);
	void OnClickTab(int nTab);
	int ConvertUnitKind_TO_Order(USHORT i_nUnitKind);
	structGuildInfo* GetGuildMemeber(int nSelect);	

	void UpdateGuildMemberSelect(POINT pt, BOOL bPopupMenu);
	
	void PopupGuildRankInfo(UID32_t SelMemberUniqueNumber);
	BOOL UpdateGuildRankInfo(POINT pt);
	void AllHideComboItem();	

	void OnClickRegister();		// 여단 공지 등록
	void GetGuidPeopleInfo(char* pGuildMasterName, char* pGuildSecondName);
	void UpdateMenuInfo();

	int GetMaxGuildSize();
	void RqCharacterUserInfo();
	void UpdateComboBoxItem();

	void TestDB();
	BOOL IsPossbleChangeRank(int nGuildRank);


protected:
	MSG_IC_GUILD_GET_GUILD_INFO_OK *m_pGuildInfo;
	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//CVectorGuildMemberInfo	m_vecGuildMember;

	BOOL		m_bGuildMember;	// 가입 여부 
	BYTE		m_nGuildRank;	// 계급 : 길드장,대장,대원,무계급(GUILD_RANK_~)
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	//CINFImage	*m_pBack;
	CINFGroupImage*	m_pBack;
	CINFGroupImage*	m_pBackControl;
#else 
	CINFImageEx	*m_pBack;
#endif
	CINFImageEx	*m_pImgSelect;
	//CINFImage	*m_pButton[BUTTON_NUMBER][BUTTON_STATE_NUMBER];
	CINFImageEx	*m_pImgGuildRank[MAX_COMBO_GUILD_RANK];
	//CINFImage	*m_pGuildVOIPButton[BUTTON_STATE_VOIP];
	int			m_nButtonState[BUTTON_NUMBER];
	int			m_nVOIPButtonState;

	CD3DHanFont	*	m_pFontGuildMaster;
	CD3DHanFont	*	m_pFontGuildMember[MAX_GUILD_NUMBER];
	CD3DHanFont	*	m_pFontGuildOnline[MAX_GUILD_NUMBER];	// color : online:BLUE, offline:RED
	CD3DHanFont	*	m_pFontGuildRank[MAX_GUILD_NUMBER];		// 차후에 문장으로 바꿈
	CD3DHanFont	*	m_pFontGuildAll;

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//CINFScrollBar*	m_pScroll;
	CINFArenaScrollBar*			m_pScrollMember;	
	CINFImageComboBox*	m_pINFImageComboBox[MAX_GUILD_NUMBER];
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	//CINFComboBox*	m_pComboBox[MAX_GUILD_NUMBER];

	//BOOL		m_bNtoNCallState;	// 여단 통화 변수

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	CINFImageBtn*	m_pSpManageBtn;		// 지원자 관리
	CINFImageBtn*	m_pRegisterBtn;		// 등록
	
	CINFImageBtn*	m_pInviteBtn;		// 초대
	CINFImageBtn*	m_pMarkBtn;			// 마크
	CINFImageBtn*	m_pOutBtn;			// 탈퇴
	CINFImageBtn*	m_pCancelBtn;		// 해제
	CINFImageBtn*	m_pOpBtn;			// 옵션

	CINFImageBtn*	m_pSearchBtn;		// 여단검색
	CINFImageBtn*	m_pGuildBattleBtn;	// 여단 전투.
	CINFImageBtn*	m_pOpOpenBtn;		// 공개 설정
	
	CINFImageBtn*	m_pCrateBtn;		// 여단 창설

	CINFImageBtn*	m_pCancelQuildCancelBtn;		// 여단 해체 취소

	CINFImageBtn*	m_pImageTabBtn[MAX_GUILD_TAB];
	BOOL			m_bTabState[MAX_GUILD_TAB];

	vector<structGuildInfo*>	m_stGuildInfo;
	
	// 선택된 길드원
	int				m_nGuildSelect;			
	UID32_t			m_SelMemberUniqueNumber;
	

	// 메뉴창
	CINFPopupMenu*		m_pPopupMenu;

	BOOL			m_bShowRank;

	CINFImageEx*	m_pImgVOIP;
	CINFImageEx*	m_pImgVOIPSpeaking;		// VOIP권한

	// 여단 공지사항
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	//CINFEditBox*	m_pEditOpenArea;
	CINFCurselEditBox*			m_pEditOpenArea;
	CINFArenaScrollBar*			m_pScrollGuildOpen;		

	BOOL		m_bUIGuildMember;	// 가입 여부 
	BYTE		m_nUIGuildRank;	// 계급 : 길드장,대장,대원,무계급(GUILD_RANK_~)

	char		m_chOpenNotice[SIZE_MAX_NOTICE];
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
};

#endif // !defined(AFX_INFCOMMUNITYGUILD_H__182E95F2_E402_480F_86DB_26C11C5016E6__INCLUDED_)
