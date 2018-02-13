// INFMissionMain.h: interface for the CINFMissionMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSIONMAIN_H__49494CB9_B795_4B0E_A26D_6A026A1D2F42__INCLUDED_)
#define AFX_INFMISSIONMAIN_H__49494CB9_B795_4B0E_A26D_6A026A1D2F42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"
#include "INFMissionTreeInfo.h"
#include "INFMissionPopupInfo.h"
#include "INFMissionSelInfluence.h"
#include "INFMissionMasterReg.h"
// 미션 윈도우 종료
#define		MISSOIN_SHOW_TREE				0		// 리스트 컨트롤
#define		MISSOIN_SHOW_POPUP				1		// 팝업 윈도우 
#define		MISSOIN_SHOW_SEL_INFLUENCE		2		// 세력선택 
#define		MAX_MISSOIN_SHOW				3

class CINFImage;
class CD3DHanFont;


class CINFMissionMain  : public CINFBase
{
public:
	CINFMissionMain();
	virtual ~CINFMissionMain();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void RenderMissionINGInfo();
	void SetMissionEnterShop(BOOL bEnterShopState);

	BOOL IsShowTreeWnd();
	void ShowTreeWnd(BOOL bFlag, BOOL bSoundPlay);

	// 특정 위치 미션
	void SetPosMissionSearchStart(BOOL nstate);
	void SetPosMissionInfo(CQuest* pQuest);
	void SetPosMissionComplete(BOOL bFlag);

	// 세력미션 선택
	BOOL GetInfluenceMission() ;
	void SetInfluenceMission(BOOL bFlag, float fANIPercent, float fVANPercent);

	// 미션 정보 초기화 
	void RefreshMission(BOOL bClick, INT QuestIndex);

	void MissionEnd(DWORD dMissionIndex);

	void SelectNextMission(INT nQuestIndex);	

	void ShowMissionPopWnd(BOOL bShow, int nPossibleQuestIdx);

	BOOL IsLevelSecnQuest(BYTE byUpLevel, int nSuccessQuestIdx, int* o_nPossibleQuestIdx);
	void LoadQuestList(vector<int>* pVecQuestIndex);

	// 퀘스트 이름
	void SetQuestNameString(CQuest* pQuest, vector<string>* o_vecMissionName, 
											CINFImageListBox* pINFImageListBox, int nMaxString);
	// 미션결과
	void SetQuestResult(CQuest* pQuest, vector<string>* o_vecPossibleDesc, 
										 CINFImageListBox* pINFImageListBox, int nMaxString);
	
	// 목표치
	void SetGoalInfo(CQuest* pQuest, vector<string>* o_vecPossibleDesc);

	// 미션이 시작이 가능한지 판단	
	BOOL CheckMissionStart();	
	BOOL CheckMissionIdxStart(DWORD i_dwSelQuestIndex);	

	// 미션 시작
	void MissionStart(DWORD i_dwSelQuestIndex);

	// 세력포인트
	float GetVCNInflDistributionPercent();
	float GetANIInflDistributionPercent();
	
	// 신규 유저 미션창 뜨우기	
	int	GetMissionMapQuestIdx();
	void SetMissionMapQuestIdx(int nQuestIdx);
	void SetFirstPossibleQuestIdx(int nPossibleQuestIdx);
	int  GetFirstPossibleQuestIdx();
	void OnClickFirstUserCloseBtn();

	// 미션맵 관련 UI업데이트
	void RefreshMissionUI();
	
// 2008-12-09 by dgwoo 미션마스터.
	void SetMissionMasterWindowShow(BOOL i_bShow);
	INT	 GetMissionMasterQuestIdx();
	void UpdateMissionMasterButton();
	
private:
	BOOL IsShowWnd(int nShowWnd);
	void TickHideMission();
	void TickBtn();

	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);

	void OnClickGoMap();	// 출격 버튼 
	void OnClickGoMissionMap(); // 미션맵으로 이동

private:
	// 미션 트리
	CINFMissionTreeInfo*	m_pINFMissionTreeInfo;
	// 팝업 트리
	CINFMissionPopupInfo*	m_pINFMissionPopupInfo;
	// 세력 선택 미션
	CINFMissionSelInfluence*	m_pINFMissionSelInfluence;
// 2008-12-09 by dgwoo 미션마스터.
	// 미션 마스터 등록 창.
	CINFMissionMasterReg*	m_pINFMissionMaster;
	
	// 출격 버튼
	CINFImageBtn*			m_pGoMapBtn;
	// 미션맵으로 이동
	CINFImageBtn*			m_pGoMissionMapBtn;


	int						m_nShowMissionWnd;		// 미션 윈도우 실행	

	CD3DHanFont	*			m_pMissionINGFont;


	BOOL					m_bSearchPosQuest;
	CQuest*					m_pHidePosQuest;

	// 세력선택 미션
	BOOL					m_bInfluenceFlag;

	float					m_fVCNInflDistributionPercent;	// 바이제니유 정규군 세력분포 - 세력선택 미션시만 사용됨
	float					m_fANIInflDistributionPercent;	// 알링턴 정규군 - 세력선택 미션시만 사용됨

	// 특정 지역 퀘스트
	BOOL					m_bPosMissionComplete;

	int						m_nMyShuttleCharacter;

	int						m_nMissionMapQuestIndex;
	// 최초의 퀘스트 번호
	int						m_nFirstPossibleQuestIdx;
};

#endif // !defined(AFX_INFMISSIONMAIN_H__49494CB9_B795_4B0E_A26D_6A026A1D2F42__INCLUDED_)
