// INFCityOutPost.h: interface for the CINFCityOutPost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYOUTPOST_H__F4225218_78B0_44F7_A83C_87F73C35B52B__INCLUDED_)
#define AFX_INFCITYOUTPOST_H__F4225218_78B0_44F7_A83C_87F73C35B52B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2007-09-05 by bhsohn 전진 기지전
#include "INFImageBtn.h"
#include "INFEditBox.h"
#include "INFArenaScrollBar.h"

#include "INFBase.h"

#define	OUTPOST_STATE_NOTICE				0
#define	OUTPOST_STATE_EXPENCE				1
#define	OUTPOST_STATE_WARTIME				2
#define OUTPOST_STATE_WARINFO				3


// 2009. 01. 12 by ckPark 선전 포고 시스템


//// #define LEADER_STATE_WARINFO			4			// 전장 정보.
// #define LEADER_WARINFO_INFLUENCE			0			// 세력정보.
// #define LEADER_WARINFO_MOTHERSHIP		1			// 모선전 정보.
// #define LEADER_WARINFO_OUTPOST			2			// 전진기지전 정보.
// #define LEADER_WARINFO_POINT				3			// 거점 정보.
// #define LEADER_WARINFO_END				4			// end.

#define LEADER_WARINFO_INFLUENCE			0			// 세력정보.
#define LEADER_WARINFO_DECLAREWAR			1			// 모선전 정보.
#define LEADER_WARINFO_MOTHERSHIP			2			// 모선전 정보.
#define LEADER_WARINFO_OUTPOST				3			// 전진기지전 정보.
#define LEADER_WARINFO_POINT				4			// 거점 정보.
#define LEADER_WARINFO_END					5			// end.

#define ANI_MOTHERSHIP_NUMBER				2027300
#define BCU_MOTHERSHIP_NUMBER				2027200


// end 2009. 01. 12 by ckPark 선전 포고 시스템


class CINFCurselEditBox;	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
class CINFImage;
class CD3DHanFont;
class CINFGroupImage;
class CINFImageEx;
class CINFCityOutPost : public CINFBase  
{
public:
	CINFCityOutPost();
	virtual ~CINFCityOutPost();


	CINFCityOutPost(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);		
	
	void SetNotice(char *i_szNotice);
	void ChangeOutPostState(DWORD i_dState);

	int WndProcNotice(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcExpence(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcWarTime(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BUILDINGNPC*		m_pBuildingInfo;

	// 판공비 관련 정보 업데이트
	void				SetExpenceInfo(float fExplate,DWORD nCumulativeExp, DWORD nExp);
	// 전쟁 시간 설정 관련 처리
	void				SetNextWarInfo(ATUM_DATE_TIME OutPostNextWarTime, INT nOutPostNextWarSelectTimeChoice, vector<ATUM_DATE_TIME> vectmpTimeList);

	VOID				ReFresh();

	void				OnCloseInfWnd();

	void				ChangeWarInfoState(BYTE i_BWarInfoState);
	void AddOutPostInfo(SCITY_WARINFO_OUTPOST * i_pOutPostInfo);
	void ClearOutPostInfo();
	void SetWarInfoInfluence(MSG_FC_CITY_WARINFO_INFLUENCE_OK *pMsg);
	DWORD GetOutPostState() {return m_nRWindowState;}
	BOOL SetGuildMark(UID32_t i_nGuildUID);

	// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
	// 모선전
	void				AddWarInfoMothership(BYTE byAttackBelligerence,	// 공격세력
											BYTE byWinBelligerence,		// 승리세력
											char* pMonName,				// 모선이름
											int nWarpoint,				// 세력포인트
											ATUM_DATE_TIME	timeStartWarTime,	// 시작시간
											ATUM_DATE_TIME	timeEndWarTime);		//종료시간	
	void				InitWarInfoMothership();

	// 세력전
	void				InitWarInfoPointWar();
	void				AddWarInfoPointWar(BYTE byAttackBelligerence,	// 공격세력
											BYTE byWinBelligerence,		// 승리세력
											char* pMapName,				// 맵이름											
											ATUM_DATE_TIME	timeStartWarTime,	// 시작시간
											ATUM_DATE_TIME	timeEndWarTime);		//종료시간	

	
private:
	void ButtonClickWrite();
	void ButtonClickApp();
	INT SetChangeRadioButton(INT i_nSel);
	// 전쟁시간 설정 버튼 클릭
	void OnClickNextWarTime();
	void RenderRadioBtn();
	void RenderWarInfo();
	int	 WndProcWarInfo(UINT uMsg,WPARAM wParam,LPARAM lParam);


	void				OnClickExpenceOk();

	void				RqNextWarTime();
	void				UpdateScrollPos(float fPosX, float fPosY, float fWidth, float fHeight, int nWhellHeight);

	// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
	int					WndProcWarInfoSub(UINT uMsg, WPARAM wParam, LPARAM lParam);


	// 2009. 01. 12 by ckPark 선전 포고 시스템
	
	int		WndProcInfluenceInfoTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcMotherShipResultTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcWarDeclareTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcOutPostTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcPointWarTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	// end 2009. 01. 12 by ckPark 선전 포고 시스템


	// 모선전 	
	void				RqMotherShipDB();	
	void				RenderWarInfoMothership();
	

	
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	
	void RenderWarInfoDeclareWar( void );
	
	// end 2009. 01. 12 by ckPark 선전 포고 시스템





	// 전진기지전
	void				RqPointWarDB();
	void				RenderWarInfoPointWar();

	

private:

	CINFImageEx*			m_pImgBrigadeNoticeB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgDelB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgExpenceB[BUTTON_STATE_NUMBER];
	//CINFImage*			m_pImgExpenceOkB[BUTTON_STATE_NUMBER];
	//CINFImage*			m_pImgAppB[BUTTON_STATE_NUMBER];
	// CINFImage*			m_pImgOkB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgOutPostWarTimeB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgWriteB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgRevB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgRadioB[RADIOBUTTON_STATE_NUMBER];
	CINFImageBtn*		m_pImgLWarInfoB;									// 전장 정보 버튼.

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*			m_pImgOutPostLBG;
	CINFGroupImage*			m_pImgRightBG[2];
	CINFGroupImage*			CityOutPostControl;
#else
	CINFImageEx*			m_pImgOutPostLBG;
#endif
	CINFImageEx*			m_pImgBriNoticeBG;
	CINFImageEx*			m_pImgOutPostTimeSetBG;
	CINFImageEx*			m_pImgExpenceBG;
	CINFImageEx*			m_pImgTitle;

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
	DWORD				m_nOutPostWarTimeB;
	//DWORD				m_nWriteB;
	DWORD				m_nRevB;
	//DWORD				m_bWriteMode;

	float				m_fExplate;
	DWORD				m_nCumulativeExp;
	DWORD				m_nExp;

	// 2007-09-05 by bhsohn 전진 기지전
	///////////////////여단 공지///////////////////
	// 에디트 컨트롤
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	//CINFEditBox*			m_pNoticeEditBox;
	CINFCurselEditBox*			m_pNoticeEditBox;

	// 쓰기
	CINFImageBtn*			m_pNoticeWrite;
	// 등록
	CINFImageBtn*			m_pRegist;
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


	// 전장정보창.
	CINFImageEx*			m_pImgWarInfoBG[LEADER_WARINFO_END];				// 탭 배경들.
	CINFImageEx*			m_pImgWarInfoInflAni;								// 알링턴 배경.
	CINFImageEx*			m_pImgWarInfoInflBcu;								// 바이제니유 배경.

	/////////////////전장 정보 //////////////////////////
	BYTE					m_bStateWarInfo;					// 활성화 전장 정보창의 탭번호.

	MSG_FC_CITY_WARINFO_INFLUENCE_OK m_sInfluenceInfo;
	vector<ST_WARINFO_OUTPOST> m_vecOutPostInfo;				// 전진기지 정보.

	// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
	CINFArenaScrollBar*		m_pScrollMotherShipInfo;							// 모선 정보창 스크롤
	CD3DHanFont *			m_pFontWarInfo;
	CINFImageBtn*			m_pBtnMotherShipInfo;
	vector<structMotherShipInfo>	m_vecMotherShipInfo;
	// 거점전 로그
	vector<structWarPointInfo>	m_vecWarPointInfo;




	// 2009. 01. 12 by ckPark 선전 포고 시스템
	
	CD3DHanFont*				m_pFontWarDeclare;
	CINFImageBtn*				m_pBtnWarDeclare;
	char						m_szOurThisTakeOff[MAX_PATH];
	char						m_szOurNextTakeOff[MAX_PATH];
	char						m_szEnemyThisTakeOff[MAX_PATH];
	char						m_szEnemyNextTakeOff[MAX_PATH];
	char						m_szOurThisTakeOffTime[MAX_PATH];
	char						m_szOurNextTakeOffTime[MAX_PATH];
	char						m_szEnemyThisTakeOffTime[MAX_PATH];
	char						m_szEnemyNextTakeOffTime[MAX_PATH];

public:
	void SetWarDeclareTime( const ATUM_DATE_TIME thisWarDeclare, char* pStr );

	inline char*				GetOurThisTakeOffTime( void ){ return m_szOurThisTakeOffTime; }
	inline char*				GetOurNextTakeOffTime( void ){ return m_szOurNextTakeOffTime; }
	inline char*				GetEnemyThisTakeOffTime( void ){ return m_szEnemyThisTakeOffTime; }
	inline char*				GetEnemyNextTakeOffTime( void ){ return m_szEnemyNextTakeOffTime; }
	
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
};



#endif // !defined(AFX_INFCITYOUTPOST_H__F4225218_78B0_44F7_A83C_87F73C35B52B__INCLUDED_)
