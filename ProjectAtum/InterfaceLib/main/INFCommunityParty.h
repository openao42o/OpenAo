// INFCommunityParty.h: interface for the CINFCommunityParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYPARTY_H__F45C5684_4895_4B19_B2E8_8658E7FAF677__INCLUDED_)
#define AFX_INFCOMMUNITYPARTY_H__F45C5684_4895_4B19_B2E8_8658E7FAF677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
#include "ClientParty.h"
// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

#define SIZE_PARTY_BUTTON_X			36
#define MAX_PARTY_NUMBER			10
#define PARTY_FORMATION_NUMBER		9
#define FLIGHT_FORM_NONE_NAME				STRMSG_C_PARTY_0033
#define FLIGHT_FORM_2_COLUMN_NAME			STRMSG_C_PARTY_0034
#define FLIGHT_FORM_2_LINE_NAME				STRMSG_C_PARTY_0035
#define FLIGHT_FORM_TRIANGLE_NAME			STRMSG_C_PARTY_0036
#define FLIGHT_FORM_INVERTED_TRIANGLE_NAME	STRMSG_C_PARTY_0037
#define FLIGHT_FORM_BELL_NAME				STRMSG_C_PARTY_0038
#define FLIGHT_FORM_INVERTED_BELL_NAME		STRMSG_C_PARTY_0039
#define FLIGHT_FORM_X_NAME					STRMSG_C_PARTY_0040
#define FLIGHT_FORM_STAR_NAME				STRMSG_C_PARTY_0041

#define FORMATION_NAME_LENGTH		20


#define PARTY_LIST_START_X		23
#define PARTY_LIST_HEIGHT		17
#define PARTY_INFO_START_X		146

#define PARTY_COMBO_X			77//95
#define PARTY_COMBO_WIDTH		72//70
#define PARTY_COMBO_HEIGHT		13

#define PARTY_COMBO_START_X		19
#define PARTY_COMBO_START_Y		231
#define PARTY_INFO_BUTTON_X		25
#define PARTY_INFO_START_X2		109
#define PARTY_JOIN_WIDTH		80
#define PARTY_JOIN_HEIGHT		16

#define PARTY_VOIP_X			165
#define PARTY_VOIP_Y			231
#define PARTY_VOIP_WIDTH		35
#define PARTY_VOIP_HEIGHT		16
#define PARTY_VOIP_BUTTON_STATE_NORMAL		0
#define PARTY_VOIP_BUTTON_STATE_UP			1
#define PARTY_VOIP_BUTTON_STATE_DOWN		2
#define PARTY_VOIP_BUTTON_STATE_DISABLE		3


#define PARTY_JOIN_BUTTON_STATE_NORMAL		0
#define PARTY_JOIN_BUTTON_STATE_UP			1
#define PARTY_JOIN_BUTTON_STATE_DOWN		2

// 2008-07-10 by bhsohn 타켓힐이 안되는 문제 해결
//#define PARTY_FRAME_SIZE_X			130
//#define PARTY_FRAME_SIZE_Y			34
#define PARTY_FRAME_ID_START_X		5
#define PARTY_FRAME_ID_START_Y		4
#define PARTY_FRAME_HP_START_X		6
#define PARTY_FRAME_HP_START_Y		21
#define PARTY_FRAME_FUEL_START_X	6
#define PARTY_FRAME_FUEL_START_Y	27

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define PARTY_LIST_START_Y		54//56
#define PARTY_COMBO_Y			231//233
#else
#define PARTY_LIST_START_Y		54//56
#define PARTY_COMBO_Y			231//233
#endif

// 라디오버튼 갯수
#define		RADIO_JOINT_COMMAND				0	// 편대장 초대
#define		RADIO_JOINT_FREE				1	// 자유 참여
#define		MAX_RADIO_JOINT					2

// 라디오버튼 갯수
#define		RADIO_AUDIO_CHAT				0	// 음성 채팅방식
#define		RADIO_AUDIO_KEY					1	// 키 입력방식
#define		RADIO_AUDIO_FREE				2	// 자유방식
#define		MAX_RADIO_AUDIO					3

// 경험치 분배 콤보 박스
#define		COMBO_DIVISION_EXP			0	// 경험치
#define		COMBO_DIVISION_ITEM			1	// 아이템 분배
#define		MAX_COMBO_DIVISION			2

// 편대 비행 갯수
//자유 비행은 이미지 없음 
#define COMBO_FLIGHT_FORM_2_COLUMN			0	// 이렬 종대, 이렬 종대 모양으로 두 줄로 나란히 선 모양이다
#define COMBO_FLIGHT_FORM_2_LINE			1	// 이렬 횡대, 이렬 횡대 모양으로 두 줄로 나란히 선 모양이다
#define COMBO_FLIGHT_FORM_TRIANGLE			2	// 삼각 편대, 삼각형 모양으로 상단부터 1, 2, 3개의 유닛이 위치한다
#define COMBO_FLIGHT_FORM_INVERTED_TRIANGLE	3	// 역삼각 편대, 역 삼각형 모양으로 상단부터 3, 2, 1개의 유닛이 위치한다
#define COMBO_FLIGHT_FORM_BELL				4	// 종 형태, 종 모양으로 상단부터 1, 3, 2개의 유닛이 위치한다
#define COMBO_FLIGHT_FORM_INVERTED_BELL		5	// 역종 형태, 역종 모양으로 상단부터 2, 3, 1개의 유닛이 위치한다
#define COMBO_FLIGHT_FORM_X					6 // X자 형태
#define COMBO_FLIGHT_FORM_STAR				7	// 별 형태
#define MAX_COMBO_FLIGHT_FORM				8

#define	WND_PARTYMODE_OPTION		0		// 파티 옵션
#define	WND_PARTYMODE_SEARCH		1		// 파티 검색
#define	MAX_WND_PARTYMODE			2		

struct structPartyListInfo
{
	PartyID_t	nPartyId;
	int			nIdx;	// 번호
	BOOL		bPartyLock;	// 공개 여부
	char		chPartyName[SIZE_MAX_PARTY_NAME];	// 편대명
	char		chPartyMasterName[SIZE_MAX_ARENA_FULL_NAME];	// 편대장
	int			nMinLV;								// 최소레벨
	int			nMaxLV;								// 최소레벨
	int			nPartyMem;							// 파티원
	int			nMaxPartyMem;						// 최대파티원
	BYTE		ExpDistributeType;
	BYTE		ItemDistributeType;

};


class CINFImage;
class CD3DHanFont;
class CINFMessageBox;
// 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFImageBtn;			
class CINFEditBox;
class CINFSecuEditBox;
class CINFNumEditBox;
class CINFArenaScrollBar;
class CINFImageRadioBtn;
class CINFListBox;
// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
class CINFImageEx;
class CINFGroupImage;
class CINFGroupManager;

// 2009-04-23 by bhsohn 에디트창 커서 이동
class CINFSingleCurselEditBox;
// end 2009-04-23 by bhsohn 에디트창 커서 이동
class CINFCommunityParty : public CINFBase  
{
public:
	CINFCommunityParty(CAtumNode* pParent);
	virtual ~CINFCommunityParty();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	
	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	void	RenderPartyDebuffIcon( PARTYENEMYINFO* pPartyMember, const int nLineNum );
	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

	void Render(POINT ptPos);
	void RenderPartyFrame();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 편대 function
	void OnButtonClicked(int nButton);	// GUI 메뉴 클릭	
	void PartySendCreate();				// 편대 생성 프로토콜을 보낸다.
	BOOL PartyQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y);		// 화면상 x,y에 위치한 유저를 초대여부를 물어본다.
	void PartySendInviteUser();				// 편대 초대 프로토콜을 보낸다.
	void PartyQuestionJoin(char* strMasterName, PartyID_t nPartyID);	// 편대 초대 받은 경우 메시지 박스 띄운다.
	// 2009. 12. 03 by jskim 파티 수락시 파티에 가입되어 있는지 체크
	//void PartyRecvInvite(int i);			// 내가 초대 받은 경우 수락(i=0)/거절(i=1)을 선택한다.
	void PartyRecvInvite(int i, PartyID_t PartyId=0);			// 내가 초대 받은 경우 수락(i=0)/거절(i=1)을 선택한다.
	//end 2009. 12. 03 by jskim 파티 수락시 파티에 가입되어 있는지 체크
	void PartySendSecede();					// 편대 탈퇴
	//void PartyQuestionBanUser();			// 추방의사를 물어본다.
	//void PartySendBanUser();				// 추방 프로토콜을 보낸다.
	//void PartyQuestionTransferMaster();		// 위임 의사를 물어본다.
	void PartySendTransferMaster();			// 위임 프로토콜을 보낸다.
	void ChagePartyFormation(int nFormation);// 편대 대형 변경
	void SetEnablePartyMenu(BOOL bEnable); // 편대전일 경우 편대 메뉴 사용 버튼 조절

	// 2008-02-27 by bhsohn 통합 아레나 수정
	void ISendPartyLeaveArena();

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	void RefreshSearchInfo();	
	void RefreshOptionInfo();
	

	// 2008-06-03 by bhsohn EP3 편대 관련 처리
	void RqPartyList();
	void AddPartyList(SPARTY_LIST_INFO* pInfo);
	void DeletePartyList();
	void SetTotalPartyCnt(int nTotalPartyCnt);
	void CheckPartyListInfo();
	void ChangePartySubMode(int nSubWndMode);
	void ShowPartyFrame();
	
	// 강퇴할 파티원
	UID32_t GetPartyBanCharacterUniqueNumber();
	void SetPartyBanCharacterUniqueNumber(UID32_t nPartyBanCharacterUniqueNumber);
	
	// 위임할 파티원
	UID32_t GetPartyTransferMasterCharacterUniqueNumber();
	void SetPartyTransferMasterCharacterUniqueNumber(UID32_t nPartyTransferMasterCharacterUniqueNumber);

	SPARTY_INFO* GetSPartyInfoSetup();

	void RqClickJoinBtn(char* pPass);	// 방에 들어간다.
	void UpdateSetupInfo(SPARTY_INFO* pInfo);	// 셋업정보 갱신

	void SetRefreshClickState(BOOL bClickRefresh);
	BOOL GetRefreshClickState();

	void UpdateBtnPos();

private:
	// 2008-06-03 by bhsohn EP3 편대 관련 처리
	void RenderParyInfo(POINT ptPos);
	void RenderPartyNumInfo();
	void RenderPartyInfo();
	
	void RenderOption(POINT ptPos);
	void RenderSeach(POINT ptPos);

	int WndProcOption(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcSearch(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void UpdateUIPos();

	DWORD GetParyInfoColor(int nIdx, PARTY_TYPE PartyType, char* pUserName);
	void AllEditBoxDisable();
	void UpdateRadioBtn();

	void OnClickJoint(int nId);
	void OnClickAudioOp(int nId);
	void InitComboInfo();

	void InitControl();
	void LoadControlInfo(SPARTY_INFO* pSPartyInfo);

	void PartyQuestionBanUser();//추방을 물어본다.
	void PartyQuestionTransferMaster();	// 위임체크
	void OnClickOkBtn();	// 적용 클릭 
	void RqChangePartyInfo();	// 파티 정보 변경 요청

	void OnClickRefresh();

	void OnClickJoinBtn();	
	void SetVoiceChatVolum(DWORD i_nVolum);

	// 2009-04-23 by bhsohn 에디트창 커서 이동
	int WndProcEdit(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// end 2009-04-23 by bhsohn 에디트창 커서 이동


	void TestList();
public:
	BOOL			m_bRestored;
	BOOL			m_bInvalidated;

	BOOL			m_bEnablePartyMenu;
	char			m_strInviteUser[20];

	// 스피커 모양 렌더링
	float			m_fSpeakingTimers;
	BOOL			m_bRenderingSpeak;

protected:
	//CINFImage	*	m_pPartyCreateButton[3];
	//CINFImage	*	m_pPartyInviteButton[3];
	//CINFImage	*	m_pPartySecedeButton[3];
	//CINFImage	*	m_pPartyBanButton[3];
	//CINFImage	*	m_pPartyTrustButton[3];
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx	*	m_pFormat;
#endif
	CINFImageEx	*	m_pForcus;
	//CINFImage	*	m_pCombo;
	//CINFImage	*	m_pCaptainCombo;
	//CINFImage	*	m_pJoinFormationButton[3];
	//CINFImage	*	m_pFormationInfo;
	CINFImageEx	*	m_pPartyFrame;
	CINFImageEx	*	m_pPartyFrameFuel;
	CINFImageEx	*	m_pPartyFrameHP;
	//CINFImage	*	m_pPartyVOIPButton[4];
	CINFImageEx	*	m_pPartyTalking;

	int				m_nPartyButtonState[5];
	int				m_nJoinFormationButtonState;
	// 2009. 12. 03 by jskim 파티 수락시 파티에 가입되어 있는지 체크
	//PartyID_t		m_nPartyRequestID;
	//end 2009. 12. 03 by jskim 파티 수락시 파티에 가입되어 있는지 체크
	int				m_nPartyBanCharacterUniqueNumber;
	int				m_nPartyTransferMasterCharacterUniqueNumber;
	int				m_nPartySelectMember;// -1(선택없음), 0 ~ 6:선택
	//CD3DHanFont *	m_pPartyFormation[PARTY_FORMATION_NUMBER];
	CD3DHanFont	*	m_pPartyList[MAX_PARTY_NUMBER];
	CD3DHanFont	*	m_pPartyInfo[MAX_PARTY_NUMBER];
	CD3DHanFont	*	m_pFontPartyFrame[MAX_PARTY_NUMBER];
	char			m_strPartyFormation[PARTY_FORMATION_NUMBER][FORMATION_NAME_LENGTH];
	BOOL			m_bShowFormationList;

	int				m_nVOIPButtonState;	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*				m_pImgBk[MAX_WND_PARTYMODE];	// 배경
	CINFGroupImage*				m_pPartyControlBtn[MAX_WND_PARTYMODE];	// 컨트롤 버튼			  
#else					
	CINFImageEx*				m_pImgBk[MAX_WND_PARTYMODE];	// 배경
#endif

	//////////// 편대 검색 //////////// 	
	CINFImageBtn*				m_pSCrateBtn;			// 생성
	CINFImageBtn*				m_pSJoinBtn;				// 참여
	CINFImageBtn*				m_pSRefreshBtn;			// 새로고침
	PARTY_TYPE					m_bPartyTypeBtn;
	vector<structPartyListInfo*>	m_vecPartyListInfo;
	
	int							m_nTotalPartyCnt;
	
	CINFArenaScrollBar*			m_pScrollPartyList;
	CINFImageEx*				m_pImgSelect;
	
	CINFImageEx*				m_pLockImage;
	CINFImageEx*				m_pUnLockImage;

	int							m_nSelParty;

	int							m_nSubWndMode;			// 파티 옵션
	ATUM_DATE_TIME				m_curRqPartyListTime;	// 파티 정보 요청시간

	//////////// 편대 옵션 //////////// 
	// 2009-04-23 by bhsohn 에디트창 커서 이동
	//CINFEditBox*			m_pEditPartyName;
	CINFSingleCurselEditBox*			m_pEditPartyName;
	// end 2009-04-23 by bhsohn 에디트창 커서 이동
	CINFSecuEditBox*		m_pINFSecuEditBox;
	CINFNumEditBox*			m_pNumMinEditBox;	// 최소 레벨
	CINFNumEditBox*			m_pNumMaxEditBox;	// 최대 레벨	

	CINFImageBtn*			m_pOJoinBtn;			// 초대
	CINFImageBtn*			m_pOLeaveBtn;			// 탈퇴
	CINFImageBtn*			m_pOPurgeBtn;			// 추방
	CINFImageBtn*			m_pOChargeBtn;			// 위임	
	CINFImageBtn*			m_pOOkBtn;				// 확인	
	CD3DHanFont*			m_pOPartyList;			// 파티원 목록
	CINFImageBtn*			m_pOPartyBattle;			// 편대 전투.
	
	CINFImageEx*			m_pSelImage;			// 파티원선택이미지
	int						m_nPartyOPSel;

	SPARTY_INFO				m_pSPartyInfoSetup;

	// 참여방식
	CINFImageRadioBtn*		m_pPartyJoint[MAX_RADIO_JOINT];	
	
	// 음성 옵션
	CINFImageRadioBtn*		m_pAudioOp[MAX_RADIO_AUDIO];
	int						m_nAduioIdx;
	
	// 분배 방식
	CINFListBox*			m_pComboDivision[MAX_COMBO_DIVISION];	

	// 편대 비행
	CINFListBox *			m_pComboForMation;
	CINFImageEx*			m_pFormatImage[MAX_COMBO_FLIGHT_FORM];		

	
	BOOL					m_bClickRefresh;

	POINT						m_ptWindowPos;
	CINFImageEx*			m_pImgSpkVolBar;
	

	DWORD						m_nVolum;
	BOOL						m_bVolumCon;							// 볼륨소리 수정중.
	float						m_fVolumConTimeGap;						// 
	POINT						m_ptOldVolumPos;						// 기존의 볼륨 위치.
	INT							m_nPosVolum;							// 기존위치(0) + m_nPosVolum      : m_nPosVolum = (m_nVolum / VOICECHAT_SPK_W) * 100


};

#endif // !defined(AFX_INFCOMMUNITYPARTY_H__F45C5684_4895_4B19_B2E8_8658E7FAF677__INCLUDED_)
