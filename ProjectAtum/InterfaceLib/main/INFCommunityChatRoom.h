// INFCommunityChatRoom.h: interface for the CINFCommunityChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYCHATROOM_H__4064B992_8044_4F20_A7EE_A944D683A1BE__INCLUDED_)
#define AFX_INFCOMMUNITYCHATROOM_H__4064B992_8044_4F20_A7EE_A944D683A1BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

//#define	CHATROOM_ADMISSION_NUMBER_COUNT				6
#define	CHATROOM_ADMISSION_NUMBER_COUNT				7
#define CHATROOM_SUBJECT_MAX_SIZE						34

enum {
	CHATROOM_TAB_LIST = 1,
	CHATROOM_TAB_ROOM = 2
};

class CINFImage;
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFEditBox;
class CINFListBox;
class CINFImageRadioBtn;
class CINFSecuEditBox;
class CINFGroupImage;
struct CHATROOM_INFO
{//EP3 - 채팅방  정보
	INT			ChatRoomNum;
	INT			ChatRoomInfoListCount;
	UID32_t		MasterCharacterUID;
	char		ChatRoomName[SIZE_MAX_CHATROOM_NAME];
	BOOL		ChatRoomLock;
	char		ChatRoomPW[SIZE_MAX_TEAM_PW];
	INT			ChatRoomMaxCount;
};
class CINFCommunityChatRoom : public CINFBase  
{
public:
	CINFCommunityChatRoom(CAtumNode* pParent);
	virtual ~CINFCommunityChatRoom();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	virtual void Tick();

	void InitSetting();

	INT GetListBoxSelect(INT i_nIndex);				// 인덱스값으로 선택된 방의 인원수를 리턴.
	INT GetListBoxSelectItem(INT i_nCount);			//GetListBoxSelect()함수와 반대 용도.
	void ChangeTab(DWORD i_nTab);
	BOOL SendChatRoomInfo();
	BOOL SendEnterRoom();
	void AddRoomList(MSG_IC_CHATROOM_LIST_INFO_OK * pMsg);
	void AddMyRoomInvite(MSG_IC_CHATROOM_ACCEPT_INVITE_OK * pMsg);
	void AddMyRoomInfo(MSG_IC_CHATROOM_MEMBER_INFO_OK * pMsg);
	void AddOtherRoomInfo(MSG_IC_CHATROOM_MEMBER_INFO_OK * pMsg);
	void ChatRoomBan(MSG_IC_CHATROOM_BAN_OK* pMsg);
	void SetButtonActive();
	void InitMyRoomInfo();
	void SendRoomListRequest(INT i_nRoomNum);
	void ModifyChatRoom();
	void ChangeLockPWOK(MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK* pMsg);
	void AllEditBoxEnable();
	void CreateMyRoomOK(MSG_IC_CHATROOM_CREATE_OK* pMsg);
	void InitMyChatRoom();
	void ChatRoomLeaveOK(MSG_IC_CHATROOM_LEAVE_OK* pMsg);

	SCHATROOM_LIST_INFO* FindChatRoomList(INT i_nRoomNum);

	void Render(POINT i_ptParent);
	void RenderList();
	void RenderChatRoom();
	void RenderCreateRoom();
	void RenderRoomInfo();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcList(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcChatRoom(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int	WndProcChatRoomCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int	WndProcChatRoomInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void UpdateBtnPos();

	// 2009-04-14 by bhsohn 채팅 방중 아레나 갔을시, 문제점 수정
	void OutChatRoom();
	// end 2009-04-14 by bhsohn 채팅 방중 아레나 갔을시, 문제점 수정


	POINT					m_ptCommunityPos;				// 윈도우 시작 위치.
	DWORD					m_nChatRoomTab;
	CINFImageEx *			m_pImgVoice;					// 음성 통신 아이콘.	   
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx*			m_pImgBG;						// 이미지 배경.
#endif
	CINFImageEx *			m_pImgSpkVolBar;				// 음성 통신 바.
	INT						m_nPosVolum;					// 스피커 볼륨.
	POINT					m_ptOldVolumPos;
	BOOL					m_bVolumCon;
	CD3DHanFont*			m_pFontRoomNum;
	INT						m_nSelect;
	FLOAT					m_fTimeBtnGap;					// 버튼을 빠르게 누르는것을 방지.
	
	vector<SCHATROOM_MEMBER_INFO>		m_vecOtherMember;	// 다른 방의 멤버 정보.
	CHATROOM_INFO						m_stOtherRoomInfo;	// 다른 방의 정보.

	vector<SCHATROOM_MEMBER_INFO>		m_vecMyMember;		// 내가 들어간 멤버 정보.
	CHATROOM_INFO						m_stMyRoomInfo;		// 내방의 정보.


	//--------------------------------------------------------------------------//
	// 채팅방 목록.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage *		m_pImgRoomListBG;				// 채팅방 목록 배경.
#else
    CINFImageEx *			m_pImgRoomListBG;				// 채팅방 목록 배경.
#endif
	CINFImageEx *			m_pImgLock[2];
	CINFImageEx *			m_pImgSel;						// 선택 이미지.
	CINFArenaScrollBar *	m_pScrRoomList;					// 
	CINFImageBtn *			m_pBtnInfo;						// 정보 버튼.
	CINFImageBtn *			m_pBtnCreate;					// 생성 버튼.
	CINFImageBtn *			m_pBtnAdmission;				// 입장 버튼.
	CINFImageBtn *			m_pBtnRoomListUpdate;			// 새로 고침 버튼.
	DWORD					m_nPersonNum[CHATROOM_ADMISSION_NUMBER_COUNT];	// 입장 인원
	vector<SCHATROOM_LIST_INFO>			m_vecRoomList;
	INT						m_nListTotalRoomCount;
	

	//--------------------------------------------------------------------------//
	
	//--------------------------------------------------------------------------//
	// 채팅방.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage *		m_pImgRoomBg;
#else
	CINFImageEx *		m_pImgRoomBg;
#endif
	CINFArenaScrollBar *	m_pScrRoom;
	CINFImageEx *			m_pImgChatRoomSel;
	CINFImageEx *			m_pImgRoomChief;				// 방장.
	CINFImageBtn *			m_pBtnCharge;					// 위임.
	CINFImageBtn *			m_pBtnInvite;					// 초대.
	CINFImageBtn *			m_pBtnOut;						// 추방.
	CINFImageBtn *			m_pBtnExit;						// 나가기.
	CINFImageBtn *			m_pBtnVoiceOk;					// 음성 채팅 확인버튼.
	CINFImageBtn *			m_pBtnVoiceCan;					// 음성 채팅 취소버튼.
	CINFListBox	*			m_pLBChatRoom;					// 채팅방에 리스트 박스.
	CINFEditBox *			m_pEBChatRoomCS;				// 채팅방 제목.
	CINFImageRadioBtn *		m_pCBVoiceChat;					// 음성채팅 유무.
	CINFImageEx	*			m_pImgRadioBtn[2];				// 래디오 버튼.
	DWORD					m_nVoiceInput;					// 0 : 키입력 방식 1 : 자유방식 
	CINFSecuEditBox*		m_pSEChatRoomSecu;				// 채팅방 비번 창.
	INT						m_nMyInfoTotCnt;
	//--------------------------------------------------------------------------//

	// 채팅방 새로 만들기.
	BOOL					m_bShowCreate;					// 방생성 인터페이스.
	POINT					m_ptCreatePos;					// 방생성 윈도우 위치.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage *		m_pRoomCreateBG;				// 방생성 배경.
#else
	CINFImageEx *	 	    m_pRoomCreateBG;				// 방생성 배경.
#endif
	CINFImageBtn *			m_pBtnCreateOk;					// 확인 버튼.
	CINFImageBtn *			m_pBtnCreateCan;				// 취소 버튼.
	CINFListBox *			m_pLBCreateRoom;				// 방생성창에 리스트 박스.
	CINFEditBox *			m_pEBCreateCS;					// 생성 채팅 제목.
	CINFSecuEditBox*		m_pSECreateSecu;				// 채팅방 새로 만들기 비번 창.

	// 채팅방 정보 윈도우.
	BOOL					m_bShowRoomInfo;
	BOOL					m_bInfoMoving;					// 정보 창 움직이는중인가?
	POINT					m_ptOldPos;
	POINT					m_ptChatRoomInfoPos;
	CINFArenaScrollBar *	m_pScrRoomInfo;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage *		m_pRoomInfoBG;					// 채팅방 정보창 배경.
#else
	CINFImageEx *		    m_pRoomInfoBG;					// 채팅방 정보창 배경.
#endif
	CINFImageBtn *			m_pBtnRoomInfoClose;
	INT						m_nOtherInfoTotCnt;

	int						m_nChatRoomMaxCount;			// 2013-01-24 by mspark, 채팅방 생성 시 방장 스크롤 안되는 문제 해결
};

#endif // !defined(AFX_INFCOMMUNITYCHATROOM_H__4064B992_8044_4F20_A7EE_A944D683A1BE__INCLUDED_)
