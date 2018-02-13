// INFGameMain.h: interface for the CInterfaceGameMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAIN_H__0FB2504C_BB3C_4763_9ADD_64687E4275A3__INCLUDED_)
#define AFX_INFGAMEMAIN_H__0FB2504C_BB3C_4763_9ADD_64687E4275A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"
#include "AtumApplication.h"
#include "INFMission.h"
#include "INFStrategicPointManager.h"
// 2007-07-04 by bhsohn 오퍼레이터 추가
#include "INFImageBtn.h"

// 2007-08-07 by bhsohn 스피커 아이템 추가
#include "INFSpeakeMsg.h"

// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵�?
#include "ChatTab_t.h"

// 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
#include "SkillInfo.h"
// end 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연

#include "INFImageEx.h"
#include "INFImageList.h"

//#include "INFQuickSlot.h"
//#include "INFParty.h"
//#include "INFMiniMap.h"
//#include "INFQuest.h"

#define LEFT_WINDOW_WEAPON	0			//평소 왼쪽에 총알 정보가 나올때..
#define LEFT_WINDOW_INVEN	1			//인벤키 (I key)를 눌렀을때 인벤창이 뜨는 상태.
#define LEFT_WINDOW_TRANS	2			//
#define LEFT_WINDOW_PARTY	3			//파티창..
#define LEFT_WINDOW_SHOP	4			//상점.

#define RIGHT_WINDOW_WEAPON 0
#define RIGHT_WINDOW_INFO	1
#define RIGHT_WINDOW_OPTION 2

#define INF_WINDOW_MOVING_VEL	3000

#define UNITKIND_LEVEL_1_MASK		(USHORT)0x1111	// 파일럿
#define UNITKIND_LEVEL_2_MASK		(USHORT)0x2222	// 윙맨
#define UNITKIND_LEVEL_3_MASK		(USHORT)0x4444	// 에이스
#define UNITKIND_LEVEL_4_MASK		(USHORT)0x8888	// 탑건
#define IS_UNITKIND_LEVEL_1(x)		((x&UNITKIND_LEVEL_1_MASK) != 0)
#define IS_UNITKIND_LEVEL_2(x)		((x&UNITKIND_LEVEL_2_MASK) != 0)
#define IS_UNITKIND_LEVEL_3(x)		((x&UNITKIND_LEVEL_3_MASK) != 0)
#define IS_UNITKIND_LEVEL_4(x)		((x&UNITKIND_LEVEL_4_MASK) != 0)


#define GAMEMAIN_BUTTON_INVEN		0
#define GAMEMAIN_BUTTON_INFO		1
#define GAMEMAIN_BUTTON_COMMUNITY	2
#define GAMEMAIN_BUTTON_TRADE		3
#define GAMEMAIN_BUTTON_SYSTEM		4

#define GAMEMAIN_BUTTON_NUMBER		5

#define HELP_NUM					2
#define	GAMEMAIN_TIMESET_ZERO		0
#define	GAMEMAIN_TIME_VIEW_TOP		27


#define HELP_START_X				(g_pD3dApp->GetBackBufferDesc().Width/2)-220
#define HELP_START_Y				130
#define HELP_TEXT_CHECK_TIME		20

// 2007-03-02 by bhsohn 다중 선택 추가 보안
#define MAX_MULTI_SEL				3

// 거점전 생성 시간 
// 2007-09-19 by bhsohn 거점전 시간 1시간으로 변경
//#define	WAR_MONSTER_LIVETIME_HOUR					2
#define	WAR_MONSTER_LIVETIME_HOUR					1
// 모선 살아있는 시간 
#define	MOTHERSHIP_LIVETIME_HOUR					2
#define	MOTHERSHIP_LIVETIME_SECOND					MOTHERSHIP_LIVETIME_HOUR*3600

// 2007-09-12 by bhsohn 2차 암호 시스템 구현
// 보안 시스템 관련 변수
#define		SECURITY_MODE_LOCK					0		// Lock
#define		SECURITY_MODE_SETUP_PASSWORD		1		// SETUP_PASSWORD
#define		SECURITY_MODE_SETUP_CHANGE_PASS		2		// 
#define		SECURITY_MODE_PASS_WND				3		// 
#define		MAX_SECURITY_MODE_WND				4		// SETUP_PASSWORD

// 2007-09-27 by bhsohn 2차 패스워드 추가 기획안
// Lock, UnLock, SecondeModeCanel
#define		SECURITY_MODE_PASS_LOCK					0		// Lock
#define		SECURITY_MODE_PASS_UNLOCK				1		// UnLock
#define		SECURITY_MODE_PASS_SENCOND_CANCEL		2		// Cacncel

// 2006-07-26 by ispark, 선택 아이템
typedef struct 
{
	BYTE					bySelectType;
	INVEN_DISPLAY_INFO	*	pSelectItem;
	CINFImageEx			*	pSelectIcon;
	POINT					ptIcon;
} stSelectItem;

// 2008-06-17 by dgwoo VoIP 음성 채팅 관리 구조체.
//struct stVOIPData
//{
//	BYTE					byVoiceType;					// 현재 진행중인 음성채팅.(VOICE_NONE, VOICE_ONE, VOICE_PARTY, VOICE_GUILD)
//	BYTE					byVoiceInput;					// 음성 입력 방식.
//	DWORD					nVolum;							// 볼륨.
//	stVOIPData()
//	{
//		byVoiceType		= 0;
//		byVoiceInput	= 0;
//		nVolum			= 100;
//	};
//};



// 2008. 12. 16 by ckPark 아이템 사용 지연
///////////////////////////////////////////////////////////////////////////////
// \class 	: ITEM_DELAY
// \brief	: 아이템 사용시 일정 시간뒤, 서버로 보내기 위해 리스트에 담아둘 구조체
//			  현재 시간이 nItemDelayTime + nItemUseTime보다 클 경우 서버로 보내면 된다
// \author	: ckPark
// \version : 
// \date	: 16:12:2008   14:32
// \warning	: nItemUniqueNumber는 ITEM_BASE의 UniqueNumber 사용
//			  nItemUseTime는 사용했을시 시간을 기록
//			  nItemDelayTime는 ITEM의 AttackTime 사용
struct ITEM_DELAY
{
	UINT64	nItemUniqueNumber;		// 아이템 고유 넘버
	DWORD	nItemUseTime;			// 아이템 사용한 시간 (1/1000초 단위)
	USHORT	nItemDelayTime;			// 아이템 딜레이 타임
};
// end 2008. 12. 16 by ckPark 아이템 사용 지연




// 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
///////////////////////////////////////////////////////////////////////////////
// \class 	: SKILL_DELAY
// \brief	: 스킬 사용시 일정 시간뒤, 서버로 보내기 위해 리스트에 담아둘 구조체
//			  현재 시간이 nSkillUseTime + nSkillDelayTime보다 클 경우 서버로 보내면 된다
// \author	: ckPark
// \version : 
// \date	: 26:12:2008   12:58
// \warning	: nTargetUniqueNumber 타겟 유니크 넘버
//			  nSkillUseTime는 스킬 사용시 시간
//			  nSkillDelayTime는 ITEM의 AttackTime 사용
///////////////////////////////////////////////////////////////////////////////
struct SKILL_DELAY
{
	CSkillInfo*	pSkillInfo;
	UID32_t		nTargetUniqueNumber;
	DWORD		nSkillUseTime;
	USHORT		nSkillDelayTime;
};
// end 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연


// 2010. 02. 11 by ckPark 발동류 장착아이템
struct INVOKEITEM_COOLTIME
{
	UID64_t		nItemUID;			// 아이템 UID
	DWORD		nPrevTickTime;		// 맨 마지막 쿨타임 계산 시간
	BOOL		bSendEndCoolTime;	// 서버로 쿨타임 종료를 보냈는가?

	INVOKEITEM_COOLTIME() : nItemUID( 0 ), nPrevTickTime( 0 ), bSendEndCoolTime( FALSE ) 
	{

	}
};
// end 2010. 02. 11 by ckPark 발동류 장착아이템


class CGameData;
class CINFAttackDirection;
class CINFUnitState;
class CINFGameMainQSlot;
class CINFGameMainUnitInfoBar;
class CINFGameMainMiniMap;
class CINFGameMainOutPost;
class CINFCommunity;
class CINFWindow;
class CINFWeaponInfo;
class CINFTrade;

// 2008-08-22 by bhsohn EP3 인벤토리 처리
//class CINFInven;
class CINFInvenExtend;

// 2008-03-19 by bhsohn Ep3옵션창
//class CINFSystem;
class CINFOptionSystem;
// end 2008-03-19 by bhsohn Ep3옵션창

// 2008-09-22 by bhsohn EP3 캐릭터 창
//class CINFCharacterInfo;
class CINFCharacterInfoExtend;

class CINFGameMainChat;
class CINFIcon;
class CINFShop;
class CINFItemInfo;
//class CINFNotice;
class CINFUnitNameInfo;
class CINFImage;
class CD3DHanFont;
class DataHeader;
class CINFGameHelpDesk;
class CINFQuest;
class CINFSkill;
class CINFGameCountDown;
class CINFDamageView;
class CINFOtherCharInfo;
class CINFGameMainFAQ;
class CINFMenuList;
class CINFMp3Player;
class CINFMissionInfo;
// 2007-06-12 by bhsohn 미션 인터페이스 수정안
class CINFMissionMain;

//class CINFCommunityVOIP;
class CINFCityBoard;

// 2007-09-12 by bhsohn 2차 암호 시스템 구현
class CINFSecuMain;
class CINFChangeCharactor;					// 2007-11-21 by dgwoo 캐릭터 변경 카드
class CINFCouponWindow;						// 2008-01-15 by dgwoo 쿠폰 시스템 추가.
class CINFMotherShipManager;				// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
class CINFStrategicPointManager;			// 2015-05-01 by killburne
class CINFGameMainSysMsg;					// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
class CINFOpJoystick;						// 2008-11-13 by bhsohn 조이스틱 작업
class CINFWorldRankWnd;						// 2009-02-13 by bhsohn 월드 랭킹 시스템
class CINFItemMixWnd;						// 2013-02-20 by bhsohn 인게임 조합창 처리
class CINFItemMenuList;						// 2013-02-26 by bhsohn 인게임 조합 검색 처리
class CINFArmorCollectWnd;					// 2013-05-28 by bhsohn 아머 컬렉션 시스템

class CINFImageList;
class CINFGroupManager;
class CINFGroupImage;

class CINFGameMain : public CAtumNode
{
public:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGameMain(CAtumNode* pParent, CGameData* pGameData);
#else
	CINFGameMain(CAtumNode* pParent);
#endif
	virtual ~CINFGameMain();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void InitWarp();
	void Render();
	void RenderLeftRightWindow();
	void RenderHelp();
	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcLeftRightWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int	WndProcKeyDown(WPARAM wParam, LPARAM lParam);

	// 2008-07-10 by bhsohn 시스템 창 우선순위 추가
	int WndProcSystemMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	int WndProcSysKeyDown(WPARAM wParam, LPARAM lParam);
	BOOL CheckSysKeyUseState(int nwParamFlage, int nlParamFlage, BOOL bUse, BOOL bOneUse);
	VOID FieldSecondLockOk(BOOL bIsUnlockFlag, BOOL bSuccess);
	VOID FieldSocketSecondPasswordUpdateOk(BOOL bSuccess, BOOL bFirstUseSecondPassword);

//	void OnButtonClick(int nButton);
	int WndProcButtonMouseMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
	void OnGameMainButtonClicked(int nButton);

	HRESULT	SetResourceFile(char* szFileName) ;
	DataHeader* FindResource(const char* szRcName);

	BOOL ChangeLeftWindowMode();//return : m_nLeftWindowModeChage
	BOOL ChangeRightWindowMode();//return : m_nRightWindowModeChage
	void RightWindowShow(BOOL bHide, int nWindow);
	void LeftWindowShow(BOOL bHide, int nWindow);

	void SetToolTip(int x, int y, char* pszToolTip);

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	void SetToolTipEx ( int x , int y , char* strToolTip , PRECT prcBound = NULL );
	void ResetToolTipEx ( void );
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//	void SetItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y, int nItemCount = 0, BOOL bShowMyEq=TRUE);	
//	void SetItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y, int nItemCount = 0, BOOL bShowMyEq=TRUE, int nLinkItem=0 , BOOL ArmorCollect = FALSE);	// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
	void SetItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y, int nItemCount = 0, BOOL bShowMyEq=TRUE, int nLinkItem=0 , BOOL ArmorCollect = FALSE, BOOL bInven = FALSE);
	// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

	void SetEnemyItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y, int nItemCount = 0, BOOL bShowMyEq=TRUE);// 2012-06-14 by isshin 아템미리보기
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	void SetItemInfo1(CItemInfo* pItemInfo, int nItemNum, int x, int y, int nItemCount = 0, BOOL bShowMyEq=TRUE);	
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
	void ToggleHelpImage();
//	BOOL LoadEnvironment() ;
//	BOOL SaveEnvironment();

	void SetNotice(int index, int x=HELP_START_X, int y=HELP_START_Y);
	void SetNotice(char* szString, int x=HELP_START_X, int y=HELP_START_Y);
	void SetQuestLimitTime(int limitTime, int PlayTime, char* MissionName);

	bool IsShowing() const { return	!(m_nLeftWindowInfo == LEFT_WINDOW_WEAPON && 
								m_nRightWindowInfo == RIGHT_WINDOW_WEAPON &&
								m_bShowHelp == FALSE &&
								!m_pMission->m_bShow); }
	void InitHelpSystem(BOOL bShow);
	void SetMentMessage(int x, int y, char *strMent);
	void SetGuildName(int x, int y, char *strGuild, DWORD dwFontColor);
	// 2013-03-06 by bhsohn 복귀 유저 시스템
	int GetMentMessageSize(char *strMent);
	// END 2013-03-06 by bhsohn 복귀 유저 시스템
	void RenderQSlotIcon();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 기타 추가 함수 
	CEnemyData	*	GetEnemyCharaterID(POINT pt);																// 가장 가까운 캐릭터 정보 가져오기
	void			RenderPopUpWindowImage(int x, int y, int cx, int cy);										// 글자에 틀만들어 주기
	void			RenderToolTipBg ( int ix , int iy , int iWid , int iHgt );
	char		*	GetMapNameByIndex(int nMapIndex);
	void			InvalidWndRect(int x, int y, int nSizeW, int nSizeH);
	void			ChangeItemByShop(int nItemNum, int nUnitColor=-1);											// 아이템 착용해보기
	void			BackUpWearItem();																			// 착용 아이템 백업
	void			ResetWearItem(int nPos=-1);																			// 착용 아이템 리셋
	void			ProcessAltiMeter(float fMinHeight, float fMaxHeight,  float fMyPosHeight);					// 고도계 계산
	void			RenderAltiMeter(int x, int y, int nPos);
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// 2005-03-03 by jschoi
	void RenderRequestEnable(float fAniTime);
	DWORD GetCurrentColor(float fTime, DWORD dwType, float fTotal = 1.0f, float fDelay = 0.0f);

	// 2005-07-20 by ispark
	void RenderToolTip();

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	void RenderToolTipEx( void );
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	// 2005-08-05 by ispark
	void InitShowWindow();
	BOOL CheckKeyUseState(int nwParamFlage, int nlParamFlage, BOOL bUse, BOOL bOneUse);
	void TickDrawWindowInfo();

	void DrawSignBoardString();
	void InitCityBoard(int nMapNumber);
	void InitChangeInfluenceBoard();

	// 2006-07-26 by ispark, 선택 아이템 재정리
	void SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo, POINT pt, BYTE byType);
	void RenderSelectItem();

	// 2006-08-01 by ispark
	BOOL CheckPopUpMenu(D3DXVECTOR2 vPos, CEnemyData* pTp);

	// 2006-08-22 by ispark, 스탯 및 미션 추가 정보 알림 렌더링
	void RenderOnInfo();
	
	void UpdateNoviceUserHelper();				// 2007-01-26 by bhsohn 신규유저 미션 인터페이스
	void UpdateLV1NoviceUserHelper();			// 2007-01-26 by bhsohn 신규유저 미션 인터페이스

	// 2007-02-12 by bhsohn Item 다중 선택 처리
	void SetMultiSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon);
	void SetMultiSelectCityStoreItem(CItemInfo* pItemInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon, INVEN_DISPLAY_INFO  *pDisSelectItem);
	int	 GetCityStoreMultiSelectItem();

	// 2007-03-02 by bhsohn 다중 선택 추가 보안
	void PopupMultiItemSelect();
	void PopupStoreMultiItemSelect();
	void RenderMultiSelectItem();

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//void AddMultiSelectItem(int nSourceIndex, BYTE bySelectType, POINT ptIcon);
	void AddMultiSelectItem( int nSourceIndex, BYTE bySelectType, POINT ptIcon, char* szIconName );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


	void ClearMultiSelectItem();	
	void UpdateIconSelectItem(BYTE byType);

	// 2007-03-22 by bhsohn 아이콘 크기 버그 수정
	BOOL IsDoingSkill(int nSourceIndex);

	// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
	void Render_AltiMeter();

	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	void RenderMission();
	void RenderMissionINGInfo();
	int WndProcMiss(UINT uMsg, WPARAM wParam, LPARAM lParam);
	ITEM * GetServerItemInfo(int nItemNum);
	CINFImageEx* FindIcon(char* strName);
	void SetWindowOrder(int nWndIndex);
	void CreateChatChild(TCHAR *str,int type, int chatType=CHAT_TAB_SYSTEM, char* szWisperToCharacterName=NULL, char *szWisperFromCharacterName=NULL);
	// 몬스터 정보를 얻어옴.
	MEX_MONSTER_INFO * CheckMonsterInfo(int nMonType);
	MAP_INFO * GetMapInfo(MapIndex_t nMapIndex);
	void SetMissionEnterShop(BOOL bEnterShopState);	
	void MissionStart(DWORD dMissionIndex);
	BOOL IsBazaar();
	void TickMissonMain();	
	void ShowMissionTreeWnd(BOOL bFlag, BOOL bSoundPlay);
	void FirstUserShowMissionTreeWnd();
	BOOL IsShowTreeWnd();
	
	void SetMissionPosMissionSearchStart(BOOL nstate);
	void SetMissionPosMissionInfo(CQuest* pQuest);
	void SetMissionPosMissionComplete(BOOL bFlag);

	// 세력선택 미션 
	BOOL GetInfluenceMission();
	void SetInfluenceMission(BOOL nFlag, float fANIPercent, float fVANPercent);

	// 미션 정보 초기화 
	void RefreshMission(BOOL bClick);
	void GoWarpMapChange(int nMapIndex);

	// 캐릭정보
	const CHARACTER& GetMyShuttleInfo() const;

	// 다음 미션 선택
	void SelectNextMission(INT nQuestIndex);
	void AddMsgBox(char* strMsg, int nType, DWORD dwData1=0, DWORD dwData2=0, int nLiveTime=0,UID64_t UniqueNumber=0, char* strName=NULL);
	// 팝업 윈도우 Show
	void UpdateMissionPopWnd(BYTE byUpLevel, int nSuccessQuestIdx);
	// 선택한 미션맵 
	int GetMissionMapQuestIdx();
	void SetImageInfo(int nImageIndex, BOOL bMissionImg);
	
	// 2007-07-04 by bhsohn 오퍼레이터 추가
	DataHeader * FindResource_LoadOp(char* szRcName);

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	void SetFaqBtnBlingBling(BOOL bSet);
	void NextTutirialState();
	void ChangeChapterReady();
	void InitTutorialLesson();
	BOOL IsTutorialMode();
	void ShowOpBossMonsterSummon(BYTE byBelligerence, int nHour, int nMinute);
	// 처음 마을 들어온 유저가 오퍼레이션 닫기 버튼 클릭
	void OnClickFirstUserCloseBtn();
	

	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	void OnClickBriefingRoom();

	// 2007-07-27 by bhsohn 스탯 추가 된 도움말 오퍼레이터에 통합
	VOID	ShowOpStatChange(int nStat);
	VOID	HideOpStatChange();

	// 2007-08-02 by bhsohn 바자맵에서 출격버튼 표현안함.
	VOID	RefreshMissionUI();

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	void AddSpeakerMsgString(char* pTxt, BOOL bGm, BOOL bGmColor);	
	void DeleteNotGmMsg();
	// end 2007-08-07 by bhsohn 스피커 아이템 추가

	// 2007-09-05 by dgwoo 기어(유저) alt + click
	void EnemyAltClick(CEnemyData * pTp,POINT pt);
	void MonsterAltClick(CMonsterData * pMon);

	// 2007-09-05 by bhsohn 전진 기지전
	void StringCulling(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont);

	// 2007-09-27 by bhsohn 2차 패스워드 추가 기획안
	VOID ShowSecondPassword();
	int GetPassChangeMode();

	// 2007-10-05 by bhsohn F키로 숨겨도 스피커 스트링 보이게 변경
	VOID		RenderSpeakerMsg();
	VOID			TickSpeakerMsg();

	// 2007-10-15 by bhsohn 총알 아이템 추가 처리
	int GetMaxBulletItem(int nMaxBullet);

	// 2007-10-16 by bhsohn 거래 상에 죽었을시, 키 안먹는 문제처리
	void SetTradeInfo(BOOL bTrade);
	void ShowCouponWindow(BOOL i_bShow);

	void SetCouponActive(MSG_FC_EVENT_COUPON_EVENT_INFO* pMsg);

	// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵�?
	void GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER]);

	// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
	void CloseCurrentEnterBuilding();
	void SetForceCloseShop(BOOL bForceClose);	

	// 2008-03-19 by bhsohn Ep3옵션창
	void PopupSystemOptinWnd();
  
	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	// 모선전 정보 
	void DestroyMotherShipInfo(BYTE byBelligerence);
	void SummonMotherShipInfo(BYTE byBelligerence, 
										ATUM_DATE_TIME  *pSummonMonsterTime, 
										INT	 nContributionPoint, 
										INT SummonMonsterUnitkind);
	int GetSummonMotherShipCnt();
	BOOL GetMotherShipInfl(UINT i_nMotherShipInf);
	BOOL SetWarPointMapInfo(SMSWARINFO_DISPLAY * pWarInfo);
	void SetStrategicPointInfo(SSTRATEGYPOINT_DISPLAY_INFO* pStrInfo);
	void DeleteStrategicPointInfo(MapIndex_t mapIndex);
	void ResetStrategicPointDisplay();
	void SetMSWarInfoDisplayOption(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK* pMsg);
	CINFMotherShipManager* GetINFMotherShipManager() {return m_pMotherShipC;}
	void SetMotherShipTeleportTime(MSG_FC_EVENT_CLICK_TELEPORT_OK *pMsg);

	// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
	float	GetQuestTimeElapse();
	void	SetQuestTimeElapse(float fGetQuestTimeElapse);	

	// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	void UpdatePlayingMotherShipInfo();
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	void OnClickCommunity();	// 커뮤니티 창 클릭
	void RenderCommunity();
	int WndProcCommunityWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void TextReduce(CD3DHanFont *pFont, int nCutPixelX, char* buf);
	// 2008-06-17 by dgwoo 음성채팅 관련 함수.
	//DWORD GetVoiceVolum()			{	return m_stVOIP.nVolum;				}
	//BYTE GetVoiceType()				{   return m_stVOIP.byVoiceType;		}			//VOICE_NONE, VOICE_ONE, VOICE_PARTY, VOICE_GUILD)
	//BYTE GetVoiceInputType()		{	return m_stVOIP.byVoiceInput;		}		
	//void SetVoiceVolum(DWORD i_nVolum);
	//void SetVoiceType(BYTE i_byType);

	// 2008-10-20 by bhsohn VOIP 버그 수정
	void InitSetVoiceVolum(DWORD i_nVolum);


	// 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함
	BOOL IsUseSecondPassword();

	// 2008-07-14 by bhsohn EP3 관련 처리
	BOOL IsRejectUser(char* pRejectName);

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	void RenderMirror(POINT *pMirrorPos=NULL);
	BOOL IsEquipInvenShow();
	D3DXMATRIX	GetInvenMatInven();
	void RenderInvenWnd();	// 인벤창 랜더링
	int WndProcInvenWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);	// 인벤창 윈도우 메시지
	// 2009-03-04 by bhsohn 럭키 머신 수정안
	//void ShowShopInven(int nBuildingKind);
	POINT ShowShopInven(int nBuildingKind, BOOL bShowInven=FALSE); // 2013-03-18 by bhsohn 팩토리 조합 추가
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2008-09-22 by bhsohn EP3 캐릭터 창
	void RenderCharacterWnd();

	// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
	//글씨를 짜름
	BOOL StringCullingUserData_ToNormal( char* strTalk, int nCullPixel, vector<string> *vecDstString, CD3DHanFont* pFont, BOOL bChRt = FALSE);
	char StringCullingAddVector_ToNormal(char chColorSave, char* pString, vector<string> *vecDstString);
	char DelCharacterRetrn(char* o_pTxt);
	// end 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리

	// 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
	BOOL CreateChatChild_OperationMode(TCHAR *str,int type, int chatType=CHAT_TAB_SYSTEM, char* szWisperToCharacterName=NULL, char *szWisperFromCharacterName=NULL);
	// end 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가

	// 2008-10-14 by bhsohn VOIP개선
	//void ChangeVOIPMode();
	//void ReleaseVOIPMode();
	//void SetVOIPSpeaking(BOOL i_bVOIPSpeaking);
	//BOOL GetVOIPSpeaking();

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	void OnClickServerTime();	

	// 2008-11-13 by bhsohn EP3-2 에서 다중 선택 판매 안되는 버그 수정
	void ClearSetIconInfo();

	// 2008-11-13 by bhsohn 조이스틱 작업
	BOOL IsShowOpJoystick();
	void ShowOpJoystick(BOOL bShow);
	int WndProcOpJoystickWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void RenderOpJoystickWnd();
	void SaveAsJoysticSetupInfo(char* pSaveAsFilename);

	// 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
	int		GetKeyDownToNumber(LPARAM i_lParam);
	// end 2009-01-30 by bhsohn 바자 상점 숫자 입력 방식 변경
	
	// 2012-12-10 by jhjang 아레나 암호 영문도 가능하게 입력 수정 
	char GetKeyDownToNumberAndAlphabet(WPARAM wParam);
	// end 2012-12-10 by jhjang 아레나 암호 영문도 가능하게 입력 수정 

	
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//	void SetItemInfoUser( CItemInfo* pItemInfo, int x, int y, BOOL bShowMyEq=TRUE);
	void SetItemInfoUser( CItemInfo* pItemInfo, int x, int y, BOOL bShowMyEq=TRUE, int nLinkItem=0, BOOL bInven  = FALSE);	   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	void ShowItemInfo(BOOL bShow);
	void SetItemInfoNormal( ITEM* pITEM, int x, int y, BOOL bShop=TRUE, int nItemCount = 0, BOOL bShowMyEq=TRUE, int nLinkItem=0,  BOOL ArmorCollect = FALSE);
	void RenderItemInfo();
	CItemInfo*	GetItem_To_MyEqITEM(ITEM* i_pItem);	// 현재 아이템 정보로 내 장비 알아옴
	void ShowMyEqItemTooltip();	// 내 장비만 보여줌
	void RefreshItemTooltipPos();	
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	void OnClickWorldRankBoard();
	void RenderWorldRank();
	int WndProcWorldRank(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void RqWorldRankInfo();		// 월드 랭킹 정보 요청
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

	// 2013-02-20 by bhsohn 인게임 조합창 처리
	// 아이템 조합창
	void OnClickItemMixWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum);
	int WndProcItemMixWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void RenderItemMixWnd();		
	// END 2013-02-20 by bhsohn 인게임 조합창 처리

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	// 아머 컬렉션 시스템창
	void	OnClickArmorCollectionWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum);
	int		WndProcArmorCollectionWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void	RenderArmorCollectionWnd();		
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	void SetKillCount(INT nCount);
	void SetRestCount(INT nCount);
	void SetBonusExpRate(INT nPartyExpRate,INT nFriendExpRate,INT nGuildExpRate);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	void UpdateServerTimeString();	// 2012-12-10 by jhjang 시간 표시 수정

	BOOL ShowOnScreenDisplay();
	
	// 2015-09-25 Future, inactivity kick
#ifdef C_DISCONNECT_INACTIVE_PLAYERS
	void InitInactivityKick(int nTimeTillKick);
	void CancelInactivityKick();
	bool	m_bShowingInactivityDCWarning;
#endif // C_DISCONNECT_INACTIVE_PLAYERS

public:
	//좌, 우 윈도우 위치 계산한 결과값
	int		m_nLeftWindowY;
	int		m_nRightWindowY;

	//////////////////////////////////////////////////////////////////////////	
	// 아이템을 착용해 보기 위한 정보
	CItemInfo*					m_pItemInfoWear[SIZE_MAX_POS];					// 착용해보기 아이템 정보
	ITEM_GENERAL*				m_pITEMG[SIZE_MAX_POS];							// 착용해보기 아이템 정보
	int							m_nItemSourceNum[SIZE_MAX_POS];					// 그전에 착용하고 있던 아이템 정보
	BOOL						m_bUseShopItem;	
	int							m_nArmorColor;									// 전착용 아머 칼라
	int							m_nLeaveBuildingIndex;							// 착용아이템 상점
	//
	//////////////////////////////////////////////////////////////////////////	

public:
	CGameData				*	m_pGameData ;

	CINFAttackDirection		*	m_pAttackDirection;
	CINFUnitState			*	m_pUnitState;
	CINFGameMainQSlot		*	m_pQuickSlot;
	CINFGameMainUnitInfoBar	*	m_pUnitInfoBar;
	CINFGameMainMiniMap		*	m_pMiniMap;
	CINFCommunity			*	m_pCommunity;
	CINFWindow				*	m_pInfWindow;
	CINFWeaponInfo			*	m_pWeaponInfo ;
	CINFTrade				*	m_pTrade;
	
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	//CINFInven				*	m_pInven ;
	CINFInvenExtend				*	m_pInven ;
	
	// 2008-03-19 by bhsohn Ep3옵션창
	//CINFSystem				*	m_pSystem ;
	CINFOptionSystem		*	m_pSystem ;
	
	// 2008-09-22 by bhsohn EP3 캐릭터 창
	//CINFCharacterInfo		*	m_pCharacterInfo ;
	CINFCharacterInfoExtend	*	m_pCharacterInfo ;

	CINFGameMainChat		*	m_pChat;
	CINFIcon				*	m_pIcon;
	CINFMission				*	m_pMission;	
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	//CINFItemInfo			*	m_pItemInfo;	
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	CINFUnitNameInfo		*	m_pUnitNameInfo;
	CINFSkill				*	m_pInfSkill;
	CINFGameCountDown		*	m_pGameCountDown;
	CINFDamageView			*	m_pDamageView;
	CINFOtherCharInfo		*	m_pOtherCharInfo;
	CINFGameMainFAQ			*	m_pInfGameMainFaq;
	CINFMenuList			*	m_pMenuList;
	CINFCouponWindow		*	m_pCouponWindow;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageBtn			*	m_pImgButton[GAMEMAIN_BUTTON_NUMBER];
	CINFImageEx				*	m_pImgButtonBK;
#else 
	CINFImage				*	m_pImgButton[GAMEMAIN_BUTTON_NUMBER][BUTTON_STATE_NUMBER];
#endif
	CINFImageEx				*	m_pImgMission;
	CINFImageEx				*	m_pHelp[HELP_NUM];	
	CINFImageEx				*	m_pSelectIcon;
	CINFImageEx				*	m_pImgTextPopUp[3];	
	CINFImageEx				*	m_pImgTextBg;
	CINFGameHelpDesk		*	m_pHelpDesk;	
	CINFQuest				*	m_pQuest;
	CINFMp3Player			*	m_pMp3Player;
	// 2007-08-28 by dgwoo 전진기지전 class추가.
	CINFGameMainOutPost		*	m_pOutPost;
	
	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	//CINFMissionInfo			*	m_pMissionInfo;
	CINFMissionMain			*	m_pMissionMain;	
	CINFChangeCharactor		*	m_pChangeCharactor;				// 2007-11-21 by dgwoo 캐릭터 변경 카드

	//CINFCommunityVOIP		*	m_pCommunityVOIP;
	CINFCityBoard			*	m_pCityBoard;			
	CINFImageEx				*	m_pImgStack;

	int		m_nIconPosX;
	int		m_nIconPosY;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int		m_nButtonState[GAMEMAIN_BUTTON_NUMBER];
#endif
	int		m_nLeftWindowState;	
	int		m_nLeftWindowInfo;
	int		m_nLeftWeaponInfoPosX;
	int		m_nLeftWindowPosY;
	int		m_nLeftWindowScaleSize;
	int		m_nRightWindowState;	
	int		m_nRightWindowInfo;
	int		m_nRightWeaponInfoPosX;
	int		m_nRightWindowPosY;
	int		m_nRightWindowScaleSize;
	
	BOOL	m_bHelpDeskFlag;
	BOOL	m_bLeftWindowModeChage;				//왼쪽창에 변화가있는 동안의 상태. 참
	BOOL	m_bRightWindowModeChage;
	BOOL	m_bMp3PlayFlag;

	CD3DHanFont		*	m_pFontToolTip;
	CD3DHanFont		*	m_pFontTimeLimit;
	CD3DHanFont		*	m_pFontDrawMent;

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	char				m_szSave_ToolTip[1024];

	vector < string >	m_vecToolTip;

	INT					m_iToolTipMaxWid;
	BOOL				m_bRenderToolTipEx;
	POINT				m_ptToolTipEx;
	RECT				m_rcToolTipEx;

	DWORD				m_dwFrameToolTip[2];
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	

	char				m_strToolTip[128];
	POINT				m_ptToolTip;
	BOOL				m_bShowHelp;
	BOOL				m_bRestored;
	BOOL				m_bQuestView;	
	

	BOOL				m_bQuestLimitTimeView;
	// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
//	float				m_fTimeElapse;	
//	int					m_nTimeSecond;
//	char				m_strNPCName[32];

	// 랩업시 미션버튼 깜박이기
//	BOOL				m_bMissionON;
//	BOOL				m_bMissionTwinkle;
// 	float				m_fTimeElapseMission;

	// FAQ 관련
	BOOL				m_bFAQProsFlag;
	// 메뉴 리스트
	BOOL				m_bMenuListFlag;
	BOOL				m_bMenuLock;
	BOOL				m_bEnterBuilingCashShop;
	BOOL				m_bSendCashItemBuy;

	BOOL				m_bBuyMessage;

	// 2005-03-03 by jschoi - RequestEnable
	CINFImageEx*		m_pRequestEnableBack;
	CINFImageEx*		m_pRequestEnableTriangle;

	// 고도 게이지 설정
	CINFImageEx*	m_pAltiMeterGage;
	CINFImageEx*	m_pAltiMeterPin;
	POINT			m_ptGageBack;
	POINT			m_ptGagePin;

	// 거래 1:1대결시 마우스 포인트 변경
	BOOL			m_bChangeMousePoint;
	// 단축 아이콘
	BOOL			m_bQSlotIconFlag;
	int				m_nQSlotPosX;
	int				m_nQSlotPosY;

	// 미션 인포
	int				m_nTimeLimitMissionOldTime;
	// VOIP
	BOOL			m_bVOIPFlag;

	// 2006-08-23 by ispark, 버튼 깜박이는 소스 수정
	BOOL			m_bKnowON[HELP_SPECIAL_COUNT];
	BOOL			m_bKnowTwinkle;
	float			m_fTimeElapseButtonCheck;

	stSelectItem	m_stSelectItem;			// 2006-07-26 by ispark, 선택 아이템

	// 2007-03-02 by bhsohn 다중 선택 추가 보안
	stSelectItem	m_stMultiSelectItem[MAX_MULTI_SEL];	
	int				m_nMultiSelCnt;	
	// 2008-01-15 by dgwoo 쿠폰 시스템 추가.
	CINFImageBtn*			m_pCouponBtn;
	// 2009. 10. 14 by jskim 프리스카 제거	
	CINFImageBtn*			m_pTut_SelBtn;
	//end 2009. 10. 14 by jskim 프리스카 제거	
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupManager*		m_GruopImagemanager;
//#endif
private:
	// 2007-07-04 by bhsohn 오퍼레이터 추가
	CINFImageBtn*			m_pFaqBtn;

	// 2007-07-27 by bhsohn 스탯 추가 된 도움말 오퍼레이터에 통합
	BOOL					m_bShowStatOpWnd;

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	CINFSpeakeMsg*			m_pINFSpeakeMsg;
	// end 2007-08-07 by bhsohn 스피커 아이템 추가

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	CINFSecuMain*			m_pINFSecuMain;
	// end 2007-09-12 by bhsohn 2차 암호 시스템 구현

	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	CINFMotherShipManager*		m_pMotherShipC;

	// 2015-05-01 by killburne
	CINFStrategicPointManager*	m_pStrategicPointDisplay;

	// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
	float						m_fQuestTimeElapse;		

	// 2008-06-17 by dgwoo 음성 채팅.
	//stVOIPData					m_stVOIP;

	// 2008-10-14 by bhsohn VOIP개선
	//BOOL						m_bVOIPSpeaking;

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	CINFGameMainSysMsg*			m_pINFGameMainSysMsg;

	// 2008-11-13 by bhsohn 조이스틱 작업
	CINFOpJoystick*				m_pINFOpJoystick;

	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	CINFItemInfo			*	m_pItemInfo;		// 아이템 툴팁
	CINFItemInfo			*	m_pEquipItemInfo;	// 현재 장착한 아이템 툴팁	
	BOOL						m_bShowMyEqTooltip;
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	CINFWorldRankWnd*			m_pINFWorldRankWnd;
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

	// 2013-02-20 by bhsohn 인게임 조합창 처리
	CINFItemMixWnd*				m_pINFItemMixWnd;
	// END 2013-02-20 by bhsohn 인게임 조합창 처리
	
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	CINFArmorCollectWnd*		m_pINFArmorCollectWnd;
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

    // 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	int				m_nAddPosY;
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	ATUM_DATE_TIME m_OldServerTime; 	// 2012-12-10 by jhjang 시간 표시 수정

	

protected:	
	BOOL			IsNoviceUserMode();		// 2007-01-26 by bhsohn 신규유저 미션 인터페이스
	void			StartNoviceUserHelper();		
	void			EndNoviceUserHelper();		// 2007-01-26 by bhsohn 신규유저 미션 인터페이스
	BOOL			IsMyLevelQuestComplete(const CHARACTER* pShuttleInfo);// 2007-01-26 by bhsohn 신규유저 미션 인터페이스


	// 2007-08-07 by bhsohn 스피커 아이템 추가
	//VOID			RenderSpeakerMsg();
	//VOID			TickSpeakerMsg();
	// end 2007-08-07 by bhsohn 스피커 아이템 추가

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	void			InitSecurityPassMode();
	VOID			RenderSecuritySystem();
	VOID			TickSecuritySystem();
	void			OnClickF10Btn();

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가	
	void			ShowSysMsgWnd(BOOL bShowWnd);
	void			RenderSysMsgWnd();
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	void TickArmorTimeCheck();		  // 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현
#endif
	// 2008. 12. 16 by ckPark 아이템 사용 지연
	// 지연 처리할 아이템을 넣어둘 리스트
	std::list<ITEM_DELAY>	m_listItemDelay;

public:
	// 지연처리할 아이템 추가
	BOOL PushDelayItem(CItemInfo* pItemInfo);
	// 지연처리할 아이템리스트 초기화
	void ReleaseDelayItem(void);
	void TickItemDelay(void);
	// end 2008. 12. 16 by ckPark 아이템 사용 지연

	// 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	CINFItemInfo* GetINFItemInfo() { return m_pItemInfo; }
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경

	void			RenderSysTimer();						// 2012-03-16 by jhahn F키로 숨겨도 시계가 보이게 변경
protected:
	std::list<SKILL_DELAY>	m_listSkillDelay;

public:
	BOOL PushDelaySkill(CSkillInfo* pSkillInfo, UID32_t nTargetUniqueNumber);
	void ReleaseDelaySkill(void);
	void TickSkillDealy(void);
	// end 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연


	// 2009. 08. 21 by ckPark 캐쉬아이템 선물하기에 블럭계정 체크
protected:
	char	m_szCashGiftSendCharacterName[256];

public:
	inline	void	SetCashGiftSendCharacterName( const char* szCharacterName ) { strcpy( m_szCashGiftSendCharacterName, szCharacterName ); }
	inline	void	GetCashGiftSendCharacterName( char* szCharacterName ) { strcpy( szCharacterName, m_szCashGiftSendCharacterName ); }
	// end 2009. 08. 21 by ckPark 캐쉬아이템 선물하기에 블럭계정 체크


	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	CINFImageEx*	FindBigIcon( int nItemnum );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


	// 2010. 02. 11 by ckPark 발동류 장착아이템
protected:
	// 쿨타임 리스트
	std::list<INVOKEITEM_COOLTIME>	m_listInvokeItemCoolTime;

public:
	// 쿨타임 아이템 리스트에 추가
	void	PushInvokeWearItem( UID64_t nItemUID );
	// 쿨타임 아이템 리스트에서 제거
	void	PopInvokeWearItem( UID64_t nItemUID );
	// 쿨타임 틱
	void	TickInvokeWearItem( void );
	// 쿨타임 시작
	void	SetItemCoolStart( UID64_t nItemUID );
	// 쿨타임 끝
	void	SetItemCoolEnd( UID64_t nItemUID );
	// 쿨타임 계산시간 설정
	void	RefreshInvokeItemTickTime( UID64_t nItemUID );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	//2016-01-04 by St0rmy, Fix City Fly bug
	bool IsShowRanking();

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	INT GetArmorCollectType();	// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
#endif
};

#define POS_LEFT_WINDOW_Y	(((CINFGameMain*)m_pParent)->m_nLeftWindowPosY - ((CINFGameMain*)m_pParent)->m_nLeftWindowScaleSize/2)
#define POS_RIGHT_WINDOW_Y	(((CINFGameMain*)m_pParent)->m_nRightWindowPosY - ((CINFGameMain*)m_pParent)->m_nRightWindowScaleSize/2)
#define POS_RIGHT_WINDOW_X		(g_pD3dApp->GetBackBufferDesc().Width - SIZE_NORMAL_WINDOW_X)


#endif // !defined(AFX_INFGameMain_H__0FB2504C_BB3C_4763_9ADD_64687E4275A3__INCLUDED_)
