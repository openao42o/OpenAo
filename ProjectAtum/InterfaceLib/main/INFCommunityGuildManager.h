// INFCommunityGuildManager.h: interface for the CINFCommunityGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYGUILDMANAGER_H__F8C97E4D_889B_4218_B582_47455597F63E__INCLUDED_)
#define AFX_INFCOMMUNITYGUILDMANAGER_H__F8C97E4D_889B_4218_B582_47455597F63E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
struct structGuildApplicantInfo
{
	UID32_t	CharacterUID;
	char chUser[SIZE_MAX_ARENA_FULL_NAME];					// 보낸 유저
	char chUnitKind[SIZE_MAX_CHARACTER_NAME];				// 유닛의 종류
	USHORT UnitKind;
	char chLevel[SIZE_MAX_CHARACTER_NAME];					// 레벨 정보 
};

class CINFEditBox;
class CINFCurselEditBox;	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
class CINFImageEx;

class CINFCommunityGuildManager  : public CINFBase  
{
public:	
	CINFCommunityGuildManager(CAtumNode* pParent);
	virtual ~CINFCommunityGuildManager();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	void AddMemberList(UID32_t	CharacterUID, char *pUser, USHORT UnitKind, int nLevel);	// 지원자 정보
	void RqGuildGetIntodution();		// 여단 소개를 얻어온다.
	void UpdateGuildIntoduce(char* pNotice);// 여단 소개를 얻어온다.
	void IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg);

private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
	BOOL OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, 
					int nMaxLine, CINFEditBox* pEdit, CINFArenaScrollBar* pScroll);

	void OnClickEditCtl(CINFEditBox*	pSelEdit);
	BOOL OnClickMemberList(POINT pt);
	void RenderMemberList();

	void RqAPPLICANTList();			// 지원자 관리 리스트 요청
	void OnClickGuildIntoduce();			// 여단 소개 등록
	void OnClickGuildIntoduceCancel();		// 여단 소개 취소

	void RqMemberInfo(UID32_t	CharacterUID);	// 유저 정보를 요청한다.

	void DelMemberList(UID32_t CharacterUID);

	// 허락
	void OnClickAccept();
	void RqAccept();

	// 거절
	void OnClickRef();
	void RqRef();

	void TestDB();
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage* m_pFormat;
	CINFGroupImage* m_pFormatControl;															  
#else					
    CINFImageEx*	m_pFormat;	
#endif


	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	

	CINFImageBtn*			m_pRegesterBtn;			// 등록 
	CINFImageBtn*			m_pCancelBtn;			// 해제
	CINFImageBtn*			m_pAcceptBtn;			// 수락
	CINFImageBtn*			m_pRefuBtn;				// 거절

	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	//CINFEditBox*			m_pEditGuildIntro;		// 여단 소개
	CINFCurselEditBox*			m_pEditGuildIntro;		// 여단 소개
	CINFArenaScrollBar*		m_pScrollGuildIntro;

	CINFEditBox*			m_pEditMemberIntro;		// 자기 소개서
	CINFArenaScrollBar*		m_pScrollMemberIntro;

	CINFArenaScrollBar*		m_pScrollMemberList;	// 멤버 리스트

	CINFImageEx*			m_pImgSelect;			// 선택이미지
	int						m_nSelMemberList;
	CD3DHanFont*			m_pFontSel;

	vector<structGuildApplicantInfo>	m_vecGuildApplicantInfo;	// 지원자 정보

	CINFImageBtn*				m_pCloseBtn ;	// 닫기 버튼

};

#endif // !defined(AFX_INFCOMMUNITYGUILDMANAGER_H__F8C97E4D_889B_4218_B582_47455597F63E__INCLUDED_)
