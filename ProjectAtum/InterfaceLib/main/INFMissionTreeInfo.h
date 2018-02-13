// INFMissionTreeInfo.h: interface for the CINFMissionTreeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSIONTREEINFO_H__AA8A3F7B_5F6B_4110_8D3B_A96B2282984C__INCLUDED_)
#define AFX_INFMISSIONTREEINFO_H__AA8A3F7B_5F6B_4110_8D3B_A96B2282984C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"
#include "INFImageRadioBtn.h"
#include "INFImageListCtrl.h"
#include "INFImageListBox.h"
#include "INFMissionMapViewCtrl.h"
#include "INFMissionAniCtrl.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	#define		TAB_MISSION_MODE_EP2			0		// Ep2미션
	#define		TAB_MISSION_MODE_EP3			1		// Ep3미션
	#define		TAB_MISSION_MODE_EP4			2		// Ep4미션
	#define		MAX_TAB_MISSION_MODE_EP			3		// 
#else
#define		TAB_MISSION_MODE_EP2			0		// Ep2미션
#define		TAB_MISSION_MODE_EP3			1		// Ep3미션
#define		MAX_TAB_MISSION_MODE_EP			2		// 
#endif
// 큰종류



// 미션 종류
#define		TAB_MISSION_MODE_SCEN			0		// 시나리오
#define		TAB_MISSION_MODE_NORMAL			1		// 일반
#define		TAB_MISSION_MODE_OUT			2		// 번외 미션
#define		MAX_TAB_MISSION_MODE			3		// 

#define		TAB_MISSION_ALL_SIZE			30		// 전체보기 

// 미션 모드 종류
#define		TAB_MISSION_ALL				0		// 전체보기 
#define		TAB_MISSION_COMPLETE		1		// 해결 미션 보기
#define		TAB_MISSION_NOTCOMPLETE		2		// 미해결 미션 보기
#define		MAX_TAB_MISSION				3		// 

// 미션 보상
#define		TAB_COMPENS_EXP				0		// 경험치
#define		TAB_COMPENS_LEV				1		// 레벨
#define		TAB_COMPENS_PRO				2		// 명성
#define		TAB_COMPENS_BUN				3		// 보너스 스탯
#define		MAX_TAB_COMPENS				5

typedef struct 
{	
	BOOL			bComplete;
	int				nQuestKind;
	int				nQuestIndex;	
	int				nMainIdx;
	int				nSubIdx;	
	DWORD			dwColor;
	int				nOrderIdx;			// 순서 인덱스
	char			szItemTxt[256];
	// 2007-07-30 by bhsohn 일반, 번외 퀘스트 재정렬
	int				nMinLevel;
	int				nMaxLevel;
	// end 2007-07-30 by bhsohn 일반, 번외 퀘스트 재정렬
	BYTE			QuestEpisodeType;	// 2008-06-18 by bhsohn EP3 퀘스트 추가
} structQuestInfo;


class CINFImage;
class CD3DHanFont;
class CINFMissionMain;
class CINFImageEx;
class CINFGroupImage;

class CINFMissionTreeInfo  
{
public:
	CINFMissionTreeInfo(CINFMissionMain* i_pParent);
	virtual ~CINFMissionTreeInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetMissionEnterShop(BOOL bEnterShopState);

	// 미션맵으로 이동
	void OnClickGoMissionMap(DWORD i_dwSelQuestIndex);
	
	// 숨겨진 퀘스트
	int HideMissionCheckProgressMission(CQuest*	 i_pHidePosQuest);

	// 미션 정보 리프레쉬
	void RefreshMission(BOOL bClick, INT QuestIndex);

	void InitMissionInfo(BOOL bFirstPopup);

	void SelectNextMission(INT nQuestIndex);
// 2008-12-09 by dgwoo 미션마스터.
	INT	 GetMissionMasterIdx(){
		return m_nMissionMasterQuestIdx;
	}
	void UpdateMissionMasterButton();
	
private:	
	void RednerMissionMode();
	void RenderCompensationItem();

	void SelectRadioBtn(int nSelRadioBtn);
	
	int	 OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int	 OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int	 OnMouseMove(WPARAM wParam, LPARAM lParam);
	int	 OnMouseWheel(WPARAM wParam, LPARAM lParam);

	void LoadListItem();
	
	
	void AddScenItem(int nMainId);
	void AddNormalItem(int nMainId);
	void AddOutItem(int nMainId);

	int GetQuestKind_TO_Tab(BYTE QuestKind);

	int CheckProgressMission(CQuest *pQuestInfo, 
								vector<int>* o_vecErrPart, 
								vector<string>*  o_vecErrQuestName,
								vector<string>*  o_vecErritem);
	int	GetListCtrlSel_TO_QuestIndex(int nSelMainIdx, int nSelSubIdx);

	void OnSelectListCtrl(INT QuestIndex);
	void OnSelectMission(int nQuestIndex);
	
	void SetErrorString(CQuest* i_pQuest, 
							vector<string>* o_vecPossibleDesc,
							vector<int>* o_vecErrPart, 
							vector<string>*  o_vecErrQuestName,
							vector<string>* o_vecErritem);	

	// 보상아이템 이름설정
	void SetQuestPay(CQuest* pQuest, vector<QUEST_PAY_ITEM>	*o_vecCompensationItemName, int *o_nCompensation);
	
	// 전체 퀘스트 정보 얻어옴
	void LoadQuest(int nSelMission);
	// 완료된 퀘스트 리스트 얻어오기	
	void LoadQuestInfo(int nSelMission, vector<int>* pVecQuestIndex);

	// 미션 정보 
	void GetMissionShow(int nSelMission, BOOL *o_bAllShow,BOOL *o_bShowComplete);

	void OnRadioButtonDown(int nRadioBtn);

	// 미션 보상 툴팁
	BOOL IsMouseMoveMissoinComp(POINT pt);	

	void OnClickGiveupMission(DWORD dwSelQuestIndex);	// 미션 포기 버튼

	void SortQuestInfo(vector<structQuestInfo>* i_vecQuestInfo);

	int ConvertOrderIdx(char* pItemTxt);

	void InitBtn();

	void SortQuestInfo_SideMision(vector<structQuestInfo>* i_vecQuestInfo);
	BOOL SortSideMision(int nFindQuestIdx, int nOrderIdx, vector<structQuestInfo>* i_vecQuestInfo);

	void SortQuestInfo_Scen(vector<structQuestInfo>* i_vecQuestInfo);		// 2012-11-29 by bhsohn 혼돈의 사막 퀘스트 안나오는 버그수정

	void ResortLoadQuest();
// 2008-12-09 by dgwoo 미션마스터.
	void MissionMasterButton();
	void MissionMasterHelpButton();
	void MissionMasterPartyButton();
	BOOL IsMissionMasterBtnAct();


private:
	// 배경 이미지
	//CINFImage*		m_pMissionBk;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*		m_pMissionBk;
#else
	CINFImageEx*		m_pMissionBk;
#endif

	// 미션 모드 이미지
	CINFImageRadioBtn*		m_pMissionMode[MAX_TAB_MISSION];
	int				m_nSelMission;	// 선택한 미션

	// 퀘스트 정보 벡터
	vector<structQuestInfo>		m_vecQuestInfo[MAX_TAB_MISSION_MODE_EP][MAX_TAB_MISSION_MODE];

	// 배경 이미지 위치및 크기
	float			m_fBackPosX;
	float			m_fBackPosY;

	// 배경 이미지 위치및 크기
	float			m_fBackWidth;
	float			m_fBackHeight;

	// err Vector
	vector<int>				m_vecErrPart;
	vector<string>			m_vecErrQuestName;
	vector<string>			m_vecErritem;

	// 미션 정보
	vector<string>			m_vecMissionName;			// 미션 이름
	vector<string>			m_vecPossibleDesc;			// 미션 설명

	// 보상 아이템
	CD3DHanFont	*			m_pCompensation;
	vector<QUEST_PAY_ITEM>	m_vecCompensationItemName;	// 미션 보상아이템 명칭
	int						m_nCompensation[MAX_TAB_COMPENS];
	BOOL					m_bCompenToolTib[MAX_TAB_COMPENS];

	// 리스트 컨트롤
	CINFImageListCtrl*		m_pMissionListCtrl;
	// 미션 설명 에디트 박스
	CINFImageListBox*		m_pINFImageListBox; // 미션 설명 에디트 박스
	// 미션 시작 버튼
	CINFImageBtn*			m_pStartMissionBtn;
	// 미션맵으로 이동 
	CINFImageBtn*			m_pGoMissionBtn;	
	// 미션포기
	CINFImageBtn*			m_pGiveupMissionBtn;
	// 2008-12-09 by dgwoo 미션 마스터.
	CINFImageBtn*			m_pBtnHelper;
	CINFImageBtn*			m_pBtnPartyHelper;
	CINFImageBtn*			m_pBtnMissionMaster;
	CINFImageBtn*			m_pBtnMissionUnMaster;

	float					m_fMMBtnTime;					// 버튼을 연타로 못누르게
	// end 2008-12-09 by dgwoo 미션 마스터.
	// 닫기 창
	CINFImageBtn*			m_pCloseBtn;
	// 맵 정보 윈도우
	CINFMissionMapViewCtrl*		m_pINFMissionMapViewCtrl;
	// 애니메이션 컨트롤
	#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFMissionAniCtrl*			m_pINFAniCtrl;
	#endif

	// 선택한 퀘스트	
	DWORD					m_dwSelQuestIndex;

	// 부모 윈도우
	CINFMissionMain*		m_pParentMissionMain;
	INT						m_nMissionMasterQuestIdx;	// 2008-12-15 by dgwoo 미션 마스터 - 미션번호 

	
};

#endif // !defined(AFX_INFMISSIONTREEINFO_H__AA8A3F7B_5F6B_4110_8D3B_A96B2282984C__INCLUDED_)
