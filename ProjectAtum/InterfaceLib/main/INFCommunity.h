// INFCommunity.h: interface for the CINFCommunity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITY_H__518CBD3B_8B48_4BB5_B7F7_62E03BDA55FD__INCLUDED_)
#define AFX_INFCOMMUNITY_H__518CBD3B_8B48_4BB5_B7F7_62E03BDA55FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define COMMUNITY_PARTY			0
#define COMMUNITY_FRIEND		1
#define COMMUNITY_GUILD			2
#define COMMUNITY_CHATROOM		3
#define COMMUNITY_MAIL			4
#define COMMUNITY_REJECT		5
#define COMMUNITY_MAX			6

// 길드 요청 윈도우
#define COMMUNITY_MEMINFO_MANAGER	0
#define COMMUNITY_MEMINFO_SEARCH	1

class CAtumNode;
class CINFCommunityParty;
class CINFCommunityGuild;
class CINFCommunityFriend;
class CINFCommunityReject;
class CINFCommunityChatRoom;
class CINFCommunityLetter;	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFCommuPartyInvite;	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFCommuPartyCre;	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFCommunityGuildManager;// 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFCommunityGuildSearch;// 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFCommunityGuildIntroMe;// 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFCommunityUserOpenSetup;
class CINFCommunityUserInfo;
class CINFEnemyItemInfo;	// 2012-06-14 by isshin 아템미리보기
class CINFImage;
class CINFImageBtn;
class CINFImageEx;											 // 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupImage;
class CINFGroupManager;

class CINFCommunity : public CINFBase  
{
public:
	CINFCommunity(CAtumNode* pParent);
	virtual ~CINFCommunity();

	CINFCommunityParty* GetParty() { return m_pParty; }
	CINFCommunityGuild* GetGuild() { return m_pGuild; }
	CINFCommunityFriend* GetFriend() { return m_pFriend; }
	CINFCommunityReject* GetReject() { return m_pReject; }

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	CINFCommunityLetter* GetLetter() { return m_pLetter; }
	CINFCommuPartyCre*	GetPartyCreate(){return m_pCommuPartyCreate;}	// 편대생성	
	CINFCommunityGuildManager* GetGuildManager() { return m_pCommunityGuildManager; }
	CINFCommunityGuildSearch* GetGuildSearch() {return m_pCommunityGuildSearch;	}
	CINFCommunityGuildIntroMe* GetGuildIntroMe() {return m_pCommunityGuildIntroMe;	}
	CINFCommuPartyInvite*	GetPartyInvite(){ return m_pCommuPartyInvite;}// 편대 초대
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetCommunityType(int nType);

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	void ShowCommunityWindow(BOOL bShow, int nSubTyepe=COMMUNITY_PARTY);
	BOOL IsShowCommunityWindow();
	POINT GetCommunityBkPos();	
	void OnClickCurrentParty();
	
	// 편대 생성
	void OnPopupCreateParty();
	void OnClickCreateParty();
	void SetAutoCreateParty(SPARTY_INFO i_PartyInfo);		/// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 자동으로 파티만들기.

	// 편대 초대
	void OnPopupInviteParty();

	// 편대원 초대
	void PartySendInviteUser(char* pUserName);

	// 편대 탈퇴
	void OnClickSecedeParty();

	// 편대원강퇴
	void PartySendBanUser();

	// 편대장 위임
	void PartySendTransferMaster();

	// 편지 보내기
	void OnClickLetterWrite(char* pUser);

	void OnPopupGuildManager();
	void OnPopupGuildSearch();
	void OnPopupIntroMe();		// 자기소개

	// 유닛이름을 가져온다.
	void GetUnitKindString(USHORT i_nUnitKind,char* pTxt);
	INT GetChatRoomNum();
	CINFCommunityChatRoom* GetChatRoom()				{	return m_pChatRoom;			}

	// 선택한 유저 UID를 얻어온다.
	BOOL GetGuildSearchSelGuidUID(UID32_t *pUID);
	BOOL GetGuildSearchSelGuidName(char* pGuildName);
	char* GetGuildName(UINT nGuildUniqueNumber);

	void GetFormatString(int nSelTxt, char* pDst);
	void RqMemberInfo(int nWndIdx, UID32_t	CharacterUID);
	void IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg);
	MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* GetMySelfIntro();
	
	void PopupRqCancelGuild();
	void RqCancelMyGuildIntro();
	void ShowGuildSearch(BOOL bShow);

	void OnPopupUserOpenSetup(BOOL bCharcterWnd);
	void ShowUserInfo(MSG_FC_CHARACTER_GET_USER_INFO_OK* pMsg);
	void HideUserInfo();	// 2015-06-27 Future, Hides the showing user Info
	
	// 2012-06-14 by isshin 아템미리보기
	void ShowEnemyItemInfo(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE* pMsg);
	void UpdateEnemyItemInfo(BOOL bUpdatEnemyItemInfo, ClientIndex_t ClientIndex);
	void RqCharacterUserItemInfo(UID32_t	TargetCharcterUID, ClientIndex_t TargetCharcterCID);
	// end 2012-06-14 by isshin 아템미리보기

	void RqCharacterUserInfo(UID32_t	TargetCharcterUID);	// 유저 정보 요청
	void RqChangeOpenUserInfo(INT SecretInfoOption);		// 정보 공개 설정 변경
	void FieldSocketCharacterChangeInfoOptionSecetOk();

	BOOL IsShowWnds();

	void UpdateBtnPos();
	BOOL IsMouseCaps(POINT ptPos);
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창	
	void SetMissionMasterOption(BOOL i_bMissionMaster);// 2008-12-09 by dgwoo 미션마스터.

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int SetBtnClick(int nNum);																	  
#endif

protected:
	BOOL					m_bRestored;
	int						m_nCommunityType;	//	COMMUNITY_~

	//CINFImage	*			m_pLogo;

	CINFCommunityParty		*m_pParty;
	CINFCommunityGuild		*m_pGuild;
	CINFCommunityChatRoom	*m_pChatRoom;
	CINFCommunityFriend		*m_pFriend;
	CINFCommunityReject		*m_pReject;
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	CINFCommunityLetter		*m_pLetter;		// 편지 
	

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	POINT					m_ptCommunityBk;	
	BOOL					m_bShowWnd;
	
	CINFCommuPartyCre*			m_pCommuPartyCreate;	// 편대생성	
	CINFCommuPartyInvite*		m_pCommuPartyInvite;	// 편대 초대
	CINFCommunityGuildManager*	m_pCommunityGuildManager;
	CINFCommunityGuildSearch*	m_pCommunityGuildSearch;	// 길드 찾기
	CINFCommunityGuildIntroMe*	m_pCommunityGuildIntroMe;
	CINFCommunityUserOpenSetup*			m_pCommunityUserOpenSetup;	// 공개 설정
	CINFCommunityUserInfo*		m_pCommunityUserInfo;
	CINFEnemyItemInfo*			m_pEnemyItemInfo;		// 2012-06-14 by isshin 아템미리보기	
	

	CINFImageBtn*				m_pCloseBtn ;	// 닫기 버튼
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*				m_pCommunityBKImage;
	CINFGroupImage*				m_pCommunityControl;
#endif
	int							m_nRqMemInfoWnd;

	int							m_nLastRqSecretInfoOption;

	BOOL						m_bMove;
	POINT						m_ptCommOpMouse;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	CINFImageBtn*				m_pCommMenuBtn[COMMUNITY_MAX];
#endif

};

#endif // !defined(AFX_INFCOMMUNITY_H__518CBD3B_8B48_4BB5_B7F7_62E03BDA55FD__INCLUDED_)
