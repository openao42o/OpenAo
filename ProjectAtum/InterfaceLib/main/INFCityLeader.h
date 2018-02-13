// INFCityLeader.h: interface for the CINFCityLeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYLEADER_H__46F2F410_1D1C_46BF_B880_E31E4A6458E0__INCLUDED_)
#define AFX_INFCITYLEADER_H__46F2F410_1D1C_46BF_B880_E31E4A6458E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "INFImageBtn.h"
#include "INFEditBox.h"
#include "INFArenaScrollBar.h"

#include "INFBase.h"

#define	LEADER_STATE_NOTICE					0			// 공지사항.
#define	LEADER_STATE_EXPENCE				1			// 판공비 
#define	LEADER_STATE_POLL					2			// 지도자 선출.
#define LEADER_STATE_POLLDATE				3			// 선거 일자.
#define LEADER_STATE_WARINFO				4			// 전장 정보.

//#define	LEADER_STATE_POLL			세부 사항.
#define LEADER_POLL_APP						1			// 등록 상태.
#define	LEADER_POLL_LIST					2			// 리스트 상태.
#define	LEADER_POLL_INFO					3			// 후보 정보.



// 2009. 01. 12 by ckPark 선전 포고 시스템


//// #define LEADER_STATE_WARINFO			4			// 전장 정보.
// #define LEADER_WARINFO_INFLUENCE			0			// 세력정보.
// #define LEADER_WARINFO_MOTHERSHIP		1			// 모선전 정보.
// #define LEADER_WARINFO_OUTPOST			2			// 전진기지전 정보.
// #define LEADER_WARINFO_POINT				3			// 거점 정보.
// #define LEADER_WARINFO_END				4			// end.

#define LEADER_WARINFO_INFLUENCE			0			// 세력정보.
#define LEADER_WARINFO_DECLAREWAR			1			// 선전포고
#define LEADER_WARINFO_MOTHERSHIP			2			// 모선전 결과
#define LEADER_WARINFO_OUTPOST				3			// 전진기지전 정보.
#define LEADER_WARINFO_POINT				4			// 거점 정보.
#define LEADER_WARINFO_END					5			// end.


#define ANI_MOTHERSHIP_NUMBER				2027300		// 모선 몬스터 넘버이름
#define BCU_MOTHERSHIP_NUMBER				2027200

// end 2009. 01. 12 by ckPark 선전 포고 시스템



// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
// 2008-03-19 by bhsohn 모선전, 거점전 정보창
//struct structMotherShipInfo
//{
//	ATUM_DATE_TIME SummonTime;					// 2008-08-19 by bhsohn 세력전, 모선전 정보 소환 시간으로 정렬
//	char chAttackInfluence[64];							// 공격 세력
//	char chWinInfluence[64];							// 승리 세력
//	char chMotherShipName[SIZE_MAX_MONSTER_NAME];		// 공격 세력 모선
//	char chWarPoint[64];								// 세력포인트
//	char chTimeCap[512];								// 세력전 진행 시간	
//};
//
//struct structWarPointInfo
//{
//	ATUM_DATE_TIME SummonTime;					// 2008-08-19 by bhsohn 세력전, 모선전 정보 소환 시간으로 정렬
//	char chAttackInfluence[64];							// 공격 세력
//	char chWinInfluence[64];							// 승리 세력
//	char MapName[SIZE_MAX_MAP_NAME];					// 거점전 맵이름	
//	char chTimeCap[512];								// 세력전 진행 시간	
//};
// end 2008-03-19 by bhsohn 모선전, 거점전 정보창

class CINFCurselEditBox;	// 2008-10-29 by bhsohn 에디트 박스 형태 변경

class CINFImage;
class CD3DHanFont;
class CINFPilotFace;
class CINFImageEx;												// 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupImage;											// 2011. 10. 10 by jskim UI시스템 변경
class CINFCityLeader  : public CINFBase  
{
public:
	CINFCityLeader();
	virtual ~CINFCityLeader();

	CINFCityLeader(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);		

	int WndProcPollInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcPollList(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcPollApp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void SetNotice(char *i_szNotice);
	void ChangeLeaderState(DWORD i_dState);
	DWORD GetLeaderState()   {return m_nRWindowState;}
	void ChangePollState(BYTE i_BPollState);
	void ChangeWarInfoState(BYTE i_BWarInfoState);
	void RenderPoll();
	void RenderWarInfo();
	

	int WndProcNotice(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcExpence(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	int WndProcPoll(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcWarInfo(UINT uMsg,WPARAM wParam,LPARAM lParam);

	BUILDINGNPC*		m_pBuildingInfo;

	// 판공비 관련 정보 업데이트
	void				SetExpenceInfo(float fExplate,DWORD nCumulativeExp, DWORD nExp);
	// 전쟁 시간 설정 관련 처리
	void				SetNextWarInfo(ATUM_DATE_TIME OutPostNextWarTime, INT nOutPostNextWarSelectTimeChoice, vector<ATUM_DATE_TIME> vectmpTimeList);

	VOID				ReFresh();
	// 닫기 버튼 클릭
	void				OnCloseInfWnd();

	void AddCandidateList(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK * pMsg);
	void AddCandidateListDone();
	void AddOutPostInfo(SCITY_WARINFO_OUTPOST * i_pOutPostInfo);
	void SetWarInfoInfluence(MSG_FC_CITY_WARINFO_INFLUENCE_OK *pMsg);
	void ClearOutPostInfo();
	void ClearCandidateList();
	void SetCandidate(BOOL i_bCandidate);
	void SetCandidateInfo(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK * pMsg);
	void SetCandidateInfoGuildMark(UID32_t i_nGuildUID);
	void SetPollDate(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK * pMsg);
	void PollRegLeaderCandidateOk(INT i_nCandidateNum);
	void DeleteCandidateOk();
	INT  GetCandidateNum()   {return m_nCandidateNum;}
	void SendLeaderCandidateList();
	BOOL SetGuildMark(UID32_t i_nGuildUID);
	const MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK * GetCandidateInfo() {return &m_sCandidateInfo;}

	// 2008-03-19 by bhsohn 모선전, 거점전 정보창
	// 모선전 
	void InitWarInfoMothership();	// 모선전 정보 초기화
	void AddWarInfoMothership(BYTE byAttackBelligerence,	// 공격세력
											BYTE byWinBelligerence,		// 승리세력
											char* pMonName,				// 모선이름
											int nWarpoint,				// 세력포인트
											ATUM_DATE_TIME	timeStartWarTime,	// 시작시간
											ATUM_DATE_TIME	timeEndWarTime);		//종료시간	

	// 세력번
	void InitWarInfoPointWar();
	void AddWarInfoPointWar(BYTE byAttackBelligerence,	// 공격세력
											BYTE byWinBelligerence,		// 승리세력
											char* pMapName,				// 맵이름											
											ATUM_DATE_TIME	timeStartWarTime,	// 시작시간
											ATUM_DATE_TIME	timeEndWarTime);		//종료시간	

	// 정보 요청
	void RqMotherShipDB();		//모선전
	void RqPointWarDB();		//거점전
	// end 2008-03-19 by bhsohn 모선전, 거점전 정보창



private:
	void ButtonClickWrite();
	void ButtonClickApp();
	void ButtonClickVote();
	void ButtonClickPollAppSuc();
	void ButtonClickPollApp();
	void ButtonClickDeleteCandidate();
	void ButtonClickPollInfo();
	INT  SetChangeRadioButton(INT i_nSel);
	void SendCandidateInfo();

	void OnClickExpenceOk();

	void RqNextWarTime();
	void WriteAppPLedge();

	// 2008-03-19 by bhsohn 모선전, 거점전 정보창	
	int WndProcWarInfoSub(UINT uMsg, WPARAM wParam, LPARAM lParam);



	// 2009. 01. 12 by ckPark 선전 포고 시스템
	// 탭에 대한 메세지 프로시져를 다 나눔
	int		WndProcInfluenceInfoTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcMotherShipResultTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcWarDeclareTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcOutPostTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcPointWarTab(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// end 2009. 01. 12 by ckPark 선전 포고 시스템



	void TestMotherDB();
	void TestPointWarDB();

	// 모선전
	void RenderWarInfoMothership();



	// 2009. 01. 12 by ckPark 선전 포고 시스템

	// 이번주 다음주 모선 출항 선전포고 정보 렌더링
	void RenderWarInfoDeclareWar( void );

	// end 2009. 01. 12 by ckPark 선전 포고 시스템




	// 거점전
	void RenderWarInfoPointWar();	
	
	// end 2008-03-19 by bhsohn 모선전, 거점전 정보창
	
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	void UpdateScrollPos(float fPosX, float fPosY, float fWidth, float fHeight, int nWhellHeight);
	

private:

	CINFImageEx*			m_pImgBrigadeNoticeB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgDelB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgExpenceB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgWriteB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgRevB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgTitle;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*			m_pImgOutPostLBG;
	CINFGroupImage*			m_pImgRightBG[2];
	CINFGroupImage*			CityLeaderControl;
#else
	CINFImageEx*			m_pImgOutPostLBG;	
#endif
	CINFImageEx*			m_pImgBriNoticeBG;

	//CINFImage*			m_pImgOutPostTimeSetBG;
	CINFImageEx*			m_pImgExpenceBG;
	// 2007-10-10 by dgwoo 폴시스템 
	//-------------------------------------------------------------------------//
	CINFImageBtn*		m_pImgLPollAB;										// 지도자 선출 버튼(알링턴).
	CINFImageBtn*		m_pImgLPollVB;										// 지도자 선출 버튼(바이제니유).
	CINFImageBtn*		m_pImgLDateB;										// 선거 일정.
	CINFImageBtn*		m_pImgLWarInfoB;									// 전장 정보 버튼.
	CINFImageEx*			m_pImgAniBG;										// 알링턴 배경.
	CINFImageEx*			m_pImgVniBG;										// 바이제니유 배경.
	CINFImageEx*			m_pImgAppBG;										// 공약 작성 배경.
	CINFImageBtn*		m_pImgAppSucB;										// 신청 완료.
	CINFImageBtn*		m_pImgPAppB;										// 후보 등록 버튼.
	CINFImageBtn*		m_pImgPUnAppB;										// 후보 탈퇴 버튼.
	CINFImageBtn*		m_pImgPInfoB;										// 후보 정보 버튼.
	CINFImageEx*			m_pImgPInfoBG;										// 후보 정보 배경.
	CINFImageEx*			m_pImgListBG;										// 리스트 배경.
	CINFImageEx*			m_pImgRSelect;										// 선택한 리스트.
	CINFImageEx*			m_pImgPollDateABG;									// 알링턴 선거 일자 배경.
	CINFImageEx*			m_pImgPollDateVBG;									// 바이제니유 선거 일자 배경.
	CINFImageEx*			m_pImgRSelectB[2][BUTTON_BOTH_STATE_NUMBER];		// 선택(목록) 버튼(앞뒤)
	CINFImageBtn*		m_pImgRVoteB;										// 투표 버튼.

	CINFImage*			m_pImgGuildMark;									// 정보 후보 길드 마크.
	CINFImage*			m_pImgFace;											// 정보 후보 얼굴.
	CINFPilotFace*		m_pPilotFace;										// 얼굴정보를 로드해서 가지고있는다.

	CINFArenaScrollBar*	m_pScrollPollList;									// 폴 리스트 스크롤.
	CINFArenaScrollBar* m_pScrollPollPledge;								// 공약 스크롤.
	CINFArenaScrollBar* m_pScrollPollPledgeView;							// 공약 스크롤.

	// 2008-03-19 by bhsohn 모선전, 거점전 정보창
	CINFArenaScrollBar*		m_pScrollMotherShipInfo;							// 모선 정보창 스크롤

	// 전장정보창.
	CINFImageEx*			m_pImgWarInfoBG[LEADER_WARINFO_END];				// 탭 배경들.
	CINFImageEx*			m_pImgWarInfoInflAni;								// 알링턴 배경.
	CINFImageEx*			m_pImgWarInfoInflBcu;								// 바이제니유 배경.


	DWORD				m_nLPollB;											// 지도자 선출 버튼 상태.
	DWORD				m_nLDateB;											// 선거 일정 버튼 상태.
	DWORD				m_nPAppB;											// 등록
	DWORD				m_nPUnAppB;											// 탈퇴.
	DWORD				m_nPInfoB;											// 후보 정보 버튼.
	DWORD				m_nRSelectB_1;										// 0 선택. 1 목록 
	DWORD				m_nRSelectB_2;										// 언더, 노말 - 선택 목록 
	DWORD				m_nRVoteB;											// 투표 버튼.

	BYTE				m_BInfluence;										// 자신의 세력.
	BYTE				m_BPollState;
	BOOL				m_bCandidate;										// 후보자인가?

	vector<MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK> m_vecCandidateList;
	MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK m_sCandidateInfo;
	INT					m_nCandidateNum;						// 후보 넘버.
	INT					m_nSelectNum;							// 목록에 선택한 목록의 번호 상대값
	ATUM_DATE_TIME		m_AppStartDate;							// 후보 신청 시작 날
	ATUM_DATE_TIME		m_AppEndDate;							// 후보 시청 끝나는 날
	ATUM_DATE_TIME		m_VoteStartDate;						// 투표 시작 날
	ATUM_DATE_TIME		m_VoteEndDate;							// 투표 마지막 날
	ATUM_DATE_TIME		m_Election;								// 선출일
	//--------------------------------------------------------------------------//
	CD3DHanFont *		m_pFontExp;
	DWORD				m_nRWindowState;

	DWORD				m_nSelWarTimeRadioB;
	DWORD				m_nWarTimeMaxCount;

	DWORD				m_nBrigadeNoticeB;
	DWORD				m_nDelB;
	DWORD				m_nExpenceB;
	DWORD				m_nExpenceOkB;
	DWORD				m_nOkB;
	DWORD				m_nAppB;
	DWORD				m_nRevB;

	float				m_fExplate;
	DWORD				m_nCumulativeExp;
	DWORD				m_nExp;

	// 2007-09-05 by bhsohn 전진 기지전
	///////////////////여단 공지///////////////////
	// 에디트 컨트롤
	//CINFEditBox*			m_pNoticeEditBox;
	CINFCurselEditBox*			m_pNoticeEditBox;

	//CINFEditBox*			m_pEditPledge;							// 공약.
	CINFCurselEditBox*			m_pEditPledge;							// 공약.

	CINFEditBox*			m_pEditPledgeView;						// 공약 뷰.
	// 쓰기
	CINFImageBtn*			m_pNoticeWrite;
	// 등록
	CINFImageBtn*			m_pRegist;
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	// 스크롤 바 
	CINFArenaScrollBar*		m_pINFScrollBar;


	///////////////////판공비 수령///////////////////
	// 수령
	CINFImageBtn*			m_pExpenceOkBtn;

	///////////////////전쟁 시간 설정 ///////////////////
	vector<ATUM_DATE_TIME>	m_vecNextWarTimeList;
	ATUM_DATE_TIME			m_OutPostNextWarTime;
	// 확인 
	CINFImageBtn*			m_pWarNextOkBtn;

	/////////////////전장 정보 //////////////////////////
	BYTE					m_bStateWarInfo;					// 활성화 전장 정보창의 탭번호.

	MSG_FC_CITY_WARINFO_INFLUENCE_OK m_sInfluenceInfo;
	vector<ST_WARINFO_OUTPOST> m_vecOutPostInfo;				// 전진기지 정보.

	// 2008-03-19 by bhsohn 모선전, 거점전 정보창
	CD3DHanFont *		m_pFontWarInfo;
	// 모선전 로그
	vector<structMotherShipInfo>	m_vecMotherShipInfo;
	// 거점전 로그
	vector<structWarPointInfo>	m_vecWarPointInfo;	
	CINFImageBtn*				m_pBtnMotherShipInfo;

	
	
	// 2009. 01. 12 by ckPark 선전 포고 시스템

	CD3DHanFont*				m_pFontWarDeclare;
	CINFImageBtn*				m_pBtnWarDeclare;					// 선전포고 버튼
	char						m_szOurThisTakeOff[MAX_PATH];		// 이번주 우리 모선 시간
	char						m_szOurNextTakeOff[MAX_PATH];		// 다음주 우리 모선 시간
	char						m_szEnemyThisTakeOff[MAX_PATH];		// 이번주 적 모선 시간
	char						m_szEnemyNextTakeOff[MAX_PATH];		// 다음주 적 모선 시간
	char						m_szOurThisTakeOffTime[MAX_PATH];	// 이번주 우리 실제 모선 시간
	char						m_szOurNextTakeOffTime[MAX_PATH];	// 다음주 우리 실제 모선 시간
	char						m_szEnemyThisTakeOffTime[MAX_PATH];	// 이번주 적 실제 모선 시간
	char						m_szEnemyNextTakeOffTime[MAX_PATH];	// 다음주 적 실제 모선 시간

public:
	// 출항 시간 설정
	void SetWarDeclareTime( const ATUM_DATE_TIME thisWarDeclare, char* pStr );
	
	
	// Getter
	inline char*				GetOurThisTakeOffTime( void ){ return m_szOurThisTakeOffTime; }
	inline char*				GetOurNextTakeOffTime( void ){ return m_szOurNextTakeOffTime; }
	inline char*				GetEnemyThisTakeOffTime( void ){ return m_szEnemyThisTakeOffTime; }
	inline char*				GetEnemyNextTakeOffTime( void ){ return m_szEnemyNextTakeOffTime; }

	// end 2009. 01. 12 by ckPark 선전 포고 시스템
};

#endif // !defined(AFX_INFCITYLEADER_H__46F2F410_1D1C_46BF_B880_E31E4A6458E0__INCLUDED_)
