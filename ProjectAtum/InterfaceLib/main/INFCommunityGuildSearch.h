// INFCommunityGuildSearch.h: interface for the CINFCommunityGuildSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYGUILDSEARCH_H__8D69F6B5_56DC_4606_962A_8925549605F3__INCLUDED_)
#define AFX_INFCOMMUNITYGUILDSEARCH_H__8D69F6B5_56DC_4606_962A_8925549605F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
struct structGuildSearchInfo
{
	UID32_t	nGuildUniqueNumber;								// 길드 아뒤
	char	GuildName[SIZE_MAX_GUILD_NAME];							// 길드 이름
	char	GuildCommanderCharacterName[SIZE_MAX_ARENA_FULL_NAME];		// 길드장	
	ATUM_DATE_TIME	RegsterDateTime;								// 등록 일정
	char	chNotice[SIZE_MAX_NOTICE];		// 공지사항
};

class CINFEditBox;
class CINFGroupImage;

class CINFCommunityGuildSearch  : public CINFBase  
{
public:	
	CINFCommunityGuildSearch(CAtumNode* pParent);
	virtual ~CINFCommunityGuildSearch();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	// 지원자 정보
	void AddGuildList(UINT nGuildUniqueNumber, char *pGuildName, 
												char *pGuildCommanderCharacterName, 
												ATUM_DATE_TIME	RegsterDateTime,
												char* pNotice);	

	BOOL GetGuildSearchSelGuidUID(UID32_t *pUID);
	BOOL GetGuildSearchSelGuidName(char* pGuildName);

	void IMSocketGuildSearchIntroDone();
	void IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg);

	MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* GetMySelfIntro();
	void OnClickGuildRqCancel();

private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
	BOOL OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, 
					int nMaxLine, CINFEditBox* pEdit, CINFArenaScrollBar* pScroll);
	
	BOOL OnClickMemberList(POINT pt);
	void RenderMemberList();
	void OnClickGuildRq();		//길드 요청
	void RqGuildSearchIntro();	// 여단 리스트 요청
	void DelGuildSearchVector(); // 벡터 초기화

	void TestDB();
	void UpdateBtnState();
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	
	CINFGroupImage*	m_pFormat;																	  
#else
    CINFImageEx*    m_pFormat;
#endif

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	
	
	CINFImageBtn*			m_pRqBtn;			// 수락
	CINFImageBtn*			m_pRqCancelBtn;				// 거절

	CINFImageBtn*				m_pCloseBtn ;	// 닫기 버튼
	
	CINFEditBox*			m_pEditGuildIntro;		// 자기 소개서
	CINFArenaScrollBar*		m_pScrollGuildIntro;

	CINFArenaScrollBar*		m_pScrollGuildList;	// 멤버 리스트

	CINFImageEx*			m_pImgSelect;			// 선택이미지
	int						m_nSelMemberList;
	CD3DHanFont*			m_pFontSel;

	vector<structGuildSearchInfo*>	m_vecGuildSearchInfo;	// 지원자 정보

	MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK	m_myShuttleIntroduceInfo;
};

#endif // !defined(AFX_INFCOMMUNITYGUILDSEARCH_H__8D69F6B5_56DC_4606_962A_8925549605F3__INCLUDED_)
