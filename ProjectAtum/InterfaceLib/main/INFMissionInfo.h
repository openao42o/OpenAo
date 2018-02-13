// CINFMissionInfo.h: interface for the CINFMissionInfo class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------

  * 미션 인터페이스 변경 2005-07-07 by ydkim
  * 기존 미션샵 변경 없음

  MissionProcFlag(BOOL bFlag) : 전체 실행 미션 플레그로 미션창 실행, 조건검색, 미션창 닫기 검사실행
  - 성공시 0
  - 실패시 0이아닌 다른값을 리턴한다  

 
*///------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_INFMISSIONINFO_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFMISSIONINFO_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

typedef enum 
{
	MISS_BACK				=				0,		// 기본 배경
	MISS_NOM				=				1,		// 일반 미션 배경
	MISS_SCEN				=				2,		// 시나리오 미션 배경
	MISS_SCEN_D				=				3,		// 시나리오 일지 배경
	MISS_CANCEL				=				4,		// 미션 취소
	MISS_SEL_BACK			=				5,		// 선택 배경
	MISS_SEL_RADIO			=				6,		// 선택 라디오 버튼
	MISS_RADIO				=				7,		// 일반 라디오 버튼
	MISS_RADIO_S			=				8,		// 라디오 선택버튼
	MISS_START_N			=				9,		// 일반 미션 버튼 
	MISS_START_O			=				10,		
	MISS_START_S			=				11,
	MISS_START_NOT			=				12,
	MISS_COPLETE_N			=				13,		// 일반 미션 완료 버튼
	MISS_COPLETE_O			=				14,
	MISS_COPLETE_S			=				15,
	MISS_COPLETE_NOT		=				16,
	MISS_LAUNCH_SHUTTLE_N	=				17,
	MISS_LAUNCH_SHUTTLE_O	=				18,
	MISS_LAUNCH_SHUTTLE_S	=				19,
	MISS_LAUNCH_SHUTTLE_NOT	=				20,
	MISS_MAP_MOVE_N			=				21,
	MISS_MAP_MOVE_O			=				22,
	MISS_MAP_MOVE_S			=				23,
	MISS_MAP_MOVE_NOT		=				24,
	MISS_MAP_GO_N			=				25,
	MISS_MAP_GO_O			=				26,
	MISS_MAP_GO_S			=				27,
	MISS_MAP_GO_NOT			=				28,
	MISS_INFLUENCE_BK		=				29,		// 세력 부분 추가
	MISS_INFLUENCE_NR		=				30,
	MISS_INFLUENCE_OV		=				31,
	MISS_INFLUENCE_PS		=				32,

	IMAGE_MAX_COUNT			=				33		// 최대 이미지 수
} MISSION_IMG_LIST;

typedef enum 
{
	COMPENS_EXP				=				0,		// 경험치
	COMPENS_LEV				=				1,		// 레벨
	COMPENS_PRO				=				2,		// 명성
	COMPENS_BUN				=				3,		// 보너스 스탯

	COMPENS_MAX				=				5//4
} MISSION_COMP;

#define LEFTFONT_MAX_LINE_COUNT				15		// 최대 폰트 라인수
#define RIGHTFONT_MAX_LINE_COUNT			10		// 최대 폰트 라인수
#define MISSION_TARGET_LIST					10		// 미션 목표
#define	MISSION_SCROLL_FLUENCE_MAX_LINE		20		// 세력선택 최대 라인수

class CINFImage;
class DataHeader;
class CGameData;
class CD3DHanFont;
class CINFScrollBar;
class CINFImageEx;

class CINFMissionInfo: public CINFBase
{

public:
	CINFMissionInfo();
	virtual ~CINFMissionInfo();
	virtual HRESULT			InitDeviceObjects();
	virtual HRESULT			RestoreDeviceObjects();
	virtual HRESULT			DeleteDeviceObjects();
	virtual HRESULT			InvalidateDeviceObjects();
	virtual void			Render();
	virtual void			Tick();
	virtual int				WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	DWORD					MissionProcFlag(BOOL bFlag);
	BOOL					GetMissionWindowState(){return m_bMissionWindowStart;}
	void					SetMissionGiveUp(int nflag){ m_nMissionGiveUp = nflag;}
	void					RefreshMission();
	int						GetMissionMapIndex(){return m_nMissionMapIndex;}
	void					SetEnterShop(BOOL bFlag){m_bFlagEnterShop = bFlag;}
	CQuest*					GetMissionIndex() {return m_pQuestInfoProgOutDoor;}
	char*					GetMissionList(int nListIndex) {return (char*)m_vecMissionList[nListIndex].c_str();}
	void					SetInfluenceMission(BOOL nFlag, float fANIPercent, float fVANPercent);	// 세력미션 선택, // 2006-02-08 by ispark, 퍼센트 추가
	BOOL					GetInfluenceMission() {return m_bInfluenceFlag;}	// 세력미션 선택, // 2006-02-08 by ispark, 퍼센트 추가

	void					SetPosMissionSearchStart(BOOL nstate){	m_bSearchPosQuest = nstate; }
	void					SetPosMissionInfo(CQuest* pQuest){m_pHidePosQuest = pQuest;}
	void					MissionStart(DWORD dMissionIndex);				// 미션 시작하기
	void					SetPosMissionComplete(BOOL bFlag){m_bPosMissionComplete = bFlag; }
	void					SetFirstMissionSelect(BOOL bFlag){m_bViewMissionSelectFirst = bFlag;}
	int						GetSelectIndexCurrent(){return m_nSelectMissionIndex;}
	void					SetSelectIndexCurrent(int nIndex){m_nSelectMissionIndex = nIndex;}

	void					RenderMissionTarget();		// 2006-08-11 by ispark, 다른 곳에서 렌더링
protected:
	void					InitMission();				// 미션 초기화
	void					InitMissionTitle();			// 미션 리스트 저장
	void					InitInfluenceMission();		// 세력 미션 초기화
	BOOL					MissionWindowRunState();	// 미션창 실행 상태 검사
	void					StartPossibleMission();		// 시작 가능 미션 데이타 설정	
	void					MissionEnd(DWORD dMissionIndex);				// 미션 완료하기
	
	void					RenderCompensationItem();	// 보상아이템 렌더링
	void					RenderPossibleMission();	// 내가 할 수 있는 미션
	void					RenderMissionList();		// 미션 리스트 렌더링
	void					RenderInfluenceMission();	// 세력 미션 렌더링
	int						CheckProgressMission(CQuest *pQuestInfo);
	int						WndProcInfluence(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL					CheckMissionStart();		// 미션 수행을 위한 조건 검색

	// 미션 설정을 위한 부분 (조건 설정)
	void					SetCompleteMissionInput();	// 전체 미션, 세력 미션 분류
	void					SetMissionClassiFy();		// 탭별, 해결, 미해결, 전체 분류
	CQuest*					SetMissionSorting();		// 미션 시나리오별 분류 및 다음 미션 찾기
	void					SetMissionInfo();			// 진행 할수있는 미션과 다음 미션 설정

	void					OverlapSortPayItem(CQuest *pQuest);// 중복 아이템 삭제	

protected:
	CINFImageEx	*			m_pMissionImg[IMAGE_MAX_COUNT];
	CINFImageEx*			m_pImgOkButton[4];					// 버튼 이미지
	CINFImageEx*			m_pImgCancelButton[4];				// 버튼 이미지
	CINFImageEx*			m_pImgWarning;						// 버튼 이미지
	CD3DHanFont	*			m_pMissionFontLeft[LEFTFONT_MAX_LINE_COUNT];
	CD3DHanFont	*			m_pMissionFontRight[RIGHTFONT_MAX_LINE_COUNT];
	CD3DHanFont	*			m_pCompensation;
	CD3DHanFont	*			m_pMissionTarget[MISSION_TARGET_LIST];
	CD3DHanFont	*			m_pInfluenceWarning[3];
	CINFScrollBar *			m_pScrollLeftWindow;
	CINFScrollBar *			m_pScrollRightWindow;

	POINT					m_PointMissionPos;			// 미션 윈도우 좌표
	POINT					m_PointMissionPosOld;		// 미션 윈도우 좌표(창이동)
	POINT					m_PointMissionMouse;		// 미션 마우스 좌표(창이동)
	BOOL					m_bMissionWindowStart;		// 미션 윈도우 실행
	int						m_nMissionGiveUp;			// 미션 포기
	int						m_nSelectMissionTab;		// 미션 탭선택
	int						m_nMissionViewCase;			// 미션 뷰 방식
	BOOL					m_bProgressMission;			// 진행중 미션
	BOOL					m_bMoveMissionWindows;		// 미션창 이동
	
	// 미션리스트 정보
	vector<string>			m_vecMissionList;			// 미션이름 리스트.
	vector<int>				m_vecTabMissionNum;			// 미션넘버 리스트.
	vector<int>				m_vecQuestIndex;			// 모든 미션 인덱스 넘버.
	int						m_nSelectMissionIndex;
	int						m_nMaxNum;

	// 할 수 있는 미션 정보
//	char					m_strMissionName[SIZE_MAX_QUEST_NAME];
	vector<string>			m_vecMissionName;
	vector<QUEST_PAY_ITEM>	m_vecCompensationItemName;	// 미션 보상아이템 명칭
	vector<string>			m_vecPossibleDesc;			// 미션 설명

	// 미션 버튼
	int						m_nMissionButtonStateStart;
	int						m_nMissionButtonStateEnd;
	int						m_nMissionButtonMapMove;
	BOOL					m_bButtonStateStart;
	BOOL					m_bButtonStateEnd;
	BOOL					m_bButtonStateMapMove;
	BOOL					m_bMissionComButtonFlag;	// 미션 완료 버튼 사용
	BOOL					m_bMissionStartButtonUse;	// 미션 시작 버튼 사용 (미사용시 Disable이미지)

	// 출격하기 미션맵이동
	int						m_nLaunchShuttleState;
	BOOL					m_bLaunchShuttleState;
	int						m_nMissionMapMove;
	BOOL					m_bMissionMapMove;
	CQuest					*m_pQuestInfoProg;			// 진행중인 미션검색(미션창)
	CQuest					*m_pQuestInfoProgOutDoor;			// 진행중인 미션검색
	int						m_nMissionMapIndex;
	BOOL					m_bFlagEnterShop;

	// 보상 아이템
	int						m_nCompensation[COMPENS_MAX];
	BOOL					m_bCompenToolTib[COMPENS_MAX];

	// 세력 선택 미션	
	int						m_nInfluencePosX;
	int						m_nInfluencePosY;
	int						m_nVCUButton;
	int						m_nANIButton;
	BOOL					m_bVCUButton;	
	BOOL					m_bANIButton;
	BOOL					m_bInfluenceFlag;			// 세력미션 렌더링
	int						m_nInflChoiceOKButton;
	int						m_nInflChoiceCANButton;

	CINFScrollBar *			m_pScrollLeftfluence;
	CINFScrollBar *			m_pScrollRightfluence;

	vector<string>			m_vecVCUfluenceDesc;			// 미션 설명
	vector<string>			m_vecANIfluenceDesc;			// 미션 설명
	CD3DHanFont	*			m_pMissionFontfluenceL[MISSION_SCROLL_FLUENCE_MAX_LINE];
	CD3DHanFont	*			m_pMissionFontfluenceR[MISSION_SCROLL_FLUENCE_MAX_LINE];

	int						m_nOldMissionIndex;
	vector<string>			m_vecOldString;
	BOOL					m_bSearchPosQuest;
	CQuest*					m_pHidePosQuest;
	// err Vector
	vector<int>				m_vecErrPart;
	vector<string>			m_vecErrQuestName;
	vector<string>			m_vecErritem;
	// 특정지역 완료 미션
	BOOL					m_bPosMissionComplete;
	BOOL					m_bViewMissionSelectFirst;

	
	

public:
	// 2006-02-08 by ispark, 세력 퍼센트
	int						m_nInflChoiceOver;
	float					m_fVCNInflDistributionPercent;	// 2006-02-08 by cmkwon, 바이제니유 정규군 세력분포 - 세력선택 미션시만 사용됨
	float					m_fANIInflDistributionPercent;	// 2006-02-08 by cmkwon, 알링턴 정규군 - 세력선택 미션시만 사용됨

};

#endif
