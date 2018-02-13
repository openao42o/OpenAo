// INFLuckyMachine.h: interface for the CINFLuckyMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFLUCKYMACHINE_H__C96FAC40_4C48_40C6_A5C7_E6796220621A__INCLUDED_)
#define AFX_INFLUCKYMACHINE_H__C96FAC40_4C48_40C6_A5C7_E6796220621A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"
// 2009. 05. 07 by ckPark 럭키머신 버튼 수정
#include "INFAUTOSPRITEBTN.h"
// end 2009. 05. 07 by ckPark 럭키머신 버튼 수정

#define	LUCKY_ITEM_LIST_NUM							7
#define	LUCKY_ITEM_LIST_NUM_LINE					3
#define LUCKY_ITEM_POS								3		// 가운데 슬롯 위치

enum {
	LUCKY_STATE_READY,
	LUCKY_STATE_RULLET,
	LUCKY_STATE_PRIZE
};

// 슬롯 돌아 가는 방식 최대 값
#define			MAX_SLOT_RULLET_TYPE				5

// 슬롯 돌아 가는 최대 스텝
#define			MAX_SLOT_RULLET_STEP				10

struct ST_TIME_GAP
{
	FLOAT		fHoldTime;				// 한칸 유지할 시간.
	FLOAT		fSlotTime;				// 슬롯이 돌아가야하는 시간 
	int			nSlotStep;				// 슬롯의 증가 값
	BOOL		bCompleteStep;			// 완료 되는 시점
};

struct struLuckyMachineScenario
{
	ST_TIME_GAP stRulTime[MAX_SLOT_RULLET_STEP];
};

struct ST_ICON_ITEM_NUMBER{
	INT			nIconNum;
	INT			nItemNum;
};

class CINFImage;
class CItemInfo;

// 2009-03-04 by bhsohn 럭키 머신 수정안
// 럭키 머신 스텝
#define		LUCKY_MAINSTEP_SELECT_SHOP			0	// 럭키 머신 상점 선택
#define		LUCKY_MAINSTEP_SELECT_GAMING		1	// 럭키 머신 게임중
#define		MAX_LUCKY_MAINSTEP					2	// 

// 럭키 머신 선택한
#define		LUCKY_SELECT_TYPE_MINI				1	// 미니 타입 럭키 머신
#define		LUCKY_SELECT_TYPE_WEAPON			2	// 웨폰머신	 
// 2010. 05. 31 by jskim 신규 웨폰 머신 구현
#define		LUCKY_SELECT_TYPE_NORMAL			3	// 아머머신	 
#define		LUCKY_SELECT_MAX				    4
//end 2010. 05. 31 by jskim 신규 웨폰 머신 구현	


// 상점 배경 이미지 수
#define		MAX_SHOP_BG							2


struct structNPCList
{	
	// 2009. 05. 07 by ckPark 럭키머신 버튼 수정
	//CINFImageBtn*		pInfImageBtn;
	CINFAUTOSPRITEBTN*	pInfImageBtn;
	// end 2009. 05. 07 by ckPark 럭키머신 버튼 수정
	CINFImageEx*			pInfShopImage[MAX_SHOP_BG];
	BOOL				bReStore;	
	LUCKY_MACHINE_OMI	struLuckyMachineOmi;
};

#define CHG_LUCKY_TITLE_IMAGE			2		// 럭키 머신 변경 인덱스
#define MAX_LUCKY_TITLE_IMAGE			4		// 럭키 머신 최대 배경 이미지

// 럭키 배경 수
#define	MAX_LUCKY_YOULUCKY			2

// end 2009-03-04 by bhsohn 럭키 머신 수정안



// 2009. 05. 07 by ckPark 럭키머신 버튼 수정

#define LUCKY_MACHINE_BTN_FRAME			1000
#define LUCKY_MACHINE_BTN_FRAME_COUNT	4

// end 2009. 05. 07 by ckPark 럭키머신 버튼 수정


class CINFLuckyMachine : public CINFBase  
{
public:	
	virtual ~CINFLuckyMachine();

	CINFLuckyMachine(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	void			TickSlot(int i_nSlot,float i_fElapsedTime);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void		OnCloseInfWnd();
	
	INT			FindSearchItem(INT i_nSlotNum, INT i_nMysteryNum);				// 입력받은 아이템이 얻을수 있는 아이템을 벡터에 넣어줌.
	void		ClearSearchItem();
	void		TakeItem(LUCKY_ITEM_SIMPLEINFO i_sItemInfo);
	void		GambleButtonOK();
	
	void		ChangeState(int i_nState);
	void		ResetData();
	void		Init();
	void		SetPrizeItem(ITEM_GENERAL i_Item);
	void		SetPrizeItem(MSG_FC_TRADE_UPDATE_ITEM_COUNT *pMsg);
	void		SetDeletePrizeItem ( const UID64_t a_ItemUniqueNumber );
	void		PrizeItemGiveMe();
	BOOL		IsCloseLucky();
	BOOL		SlotSpeedStepUp(int i_nSlot);	
	UID64_t		GetSelUID();

	// 2009-03-04 by bhsohn 럭키 머신 수정안
	void		SetLuckyMechineStep(int nLuckyMechineStep);
	void		EnterMachineShop(UINT i_uBuildingIndex);	
	void		ResetLuckyMachineInfo();

	BOOL		IsInfluenceOneShopMode();

	BOOL		IsPossibleUpLoadCoin(ITEM* i_pItem);
	void		UpLoadCoin(ITEM_GENERAL* i_pItem);
	CItemInfo*	GetConinItemInfo();
	
	void		ClearConinItemInfo();
	void		RenderYouLucky();
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
	void		ErrorLuckyMechineInvenFull();
	// end 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리

	// 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 상점에서 사용 가능한 아이템만 보여주기
	LUCKY_MACHINE_OMI GetLuckyMachineInfo() { return m_struSelLuckyMachine; }
	//end 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 상점에서 사용 가능한 아이템만 보여주기



private:
	// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
	BOOL		IsShowLucySuccessEffect(INT	i_MysteryItemDropNum, int i_nMysteryNum);
	void		ShowLuckySuccessEffect();
	
	// 2009-03-04 by bhsohn 럭키 머신 수정안
	// 룰렛 값들 초기화
	void		InitRulType();

	void		CreateNPCListBtn();
	void		InitDeviceObjectsNPCListBtn();
	void		RestoreDeviceObjectsNPCListBtn();
	void		InvalidateDeviceObjectsNPCListBtn();
	void		DeleteDeviceObjectsNPCListBtn();	

	BOOL		IsMyBelignece(LUCKY_MACHINE_OMI* pLuckyMechineOmi);	// 나의 세력 럭키 머신이냐?
	BOOL		IsHasNPCBtn(LUCKY_MACHINE_OMI* pLuckyMechineOmi);	
	void		ShowNPCListBtn(BOOL i_bShow);
	BOOL		RefreshNPCListBtnPos();
	void		RefreshLuckyMachineCtrl();
	void		OnClickSelectNPCShop(LUCKY_MACHINE_OMI	*pSelLuckyMachine);
	
	
	void		RefreshNPCListBtnStat();

	// 선택한 럭키 머신	
	void		TickLuckyStepStepSelectShop();
	void		TickLuckyStepStepSelectGaming();

	// 선택한 럭키 머신		
	void		RenderLuckyStepStepSelectShop();
	void		RenderLuckyStepStepSelectGaming();

	void		RenderLuckyMachineSlot(int nSlotNum);

	int			WndProcCommon(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	int			WndProcLuckyStepStepSelectShop(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcLuckyStepStepSelectGaming(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 럭키 머신 선택
	void		OnClickNPCListPos(int i_nStep);
	void		OnClickPrevStepBtn();
	
	
	// 모든 룰렛 완료
	BOOL		IsAllRuletDone();
	// 올릴수 있는 주화냐?
	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	//BOOL		IsPossibleUpLoadCoin(CItemInfo* i_pItem, int i_nMachineNum);

	// 한개의 슬롯이 완료 됐냐
	BOOL		IsSlotDone(int i_nSlot);

	// 
	void		UpdateSuccessSlot();

	CINFImage*	GetShopImageBk(int nEffectBG);

	void		UpdateCurrentSlotPos(int i_nSlot);

	BOOL		GetInfluenceOneShop(LUCKY_MACHINE_OMI* o_pSelLuckyMachine);

	void		TickYouLucky();
	// end 2009-03-04 by bhsohn 럭키 머신 수정안
protected:

	//INT						m_nTakeItem;
	vector<LUCKY_ITEM_SIMPLEINFO>		m_vecTakeItem;
	int						m_nState;
	vector<ITEM_GENERAL>	m_vecPrizeItem;
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT> 	m_vecPrizeItemCount;
	vector<ST_ICON_ITEM_NUMBER>		m_vecIconNum[LUCKY_ITEM_LIST_NUM_LINE];		// 슬롯의 아이콘 정보.
	
	struLuckyMachineScenario		m_stRulTime[LUCKY_ITEM_LIST_NUM_LINE][MAX_SLOT_RULLET_TYPE];		// 슬롯이 돌아가는 시간을 정해놓은 값.		
	BOOL					m_bSlotDone[LUCKY_ITEM_LIST_NUM_LINE];		// 당첨되어 슬롯이 멈춰있는가?
	int						m_nSlotStep[LUCKY_ITEM_LIST_NUM_LINE];		// 슬롯의 회전 속도.

	// 한개 슬롯마다 유지 시간
	float					m_fTimeHold[LUCKY_ITEM_LIST_NUM_LINE];		
	
	// 한개 스텝 유지 시간 
	float					m_fTimeOneStepSlot[LUCKY_ITEM_LIST_NUM_LINE];		
	// 각 슬롯의 완료 여부
	BOOL					m_bCompleteStep[LUCKY_ITEM_LIST_NUM_LINE];

	// 슬롯에 처음으로 보여야할 카운트.
	int						m_nSlotStartPosIdx[LUCKY_ITEM_LIST_NUM_LINE];			

	// 슬롯 타입
	int						m_nSlotType[LUCKY_ITEM_LIST_NUM_LINE];			

	// 전체 스텝 시간
	float					m_fTimeAllTimeStepSlot[LUCKY_ITEM_LIST_NUM_LINE];		

	// 완료 성공 위치
	int						m_nSuccessSlot[LUCKY_ITEM_LIST_NUM_LINE];		
	
	
	CINFImageEx*				m_pImgQuestionMark;				// 물음표 
//	CINFImage*				m_pImgArrow;					// 화살표.
	
	CINFImageEx*				m_pImgMainBG[LUCKY_SELECT_MAX];						// 메인 배경.

	CINFImageEx*				m_pImgEffectBG[MAX_LUCKY_TITLE_IMAGE];				// 메인 위의 배경.

	CINFImageEx*				m_pImgEffBG[LUCKY_SELECT_MAX][7];

	CINFImageEx*				m_pImgYouLuckyBG[MAX_LUCKY_YOULUCKY];				// 축하 배경

	// 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
	CINFImageEx*				m_pImgDisableItem;				// 사용 아이템 그림자
	//end 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기

	int						m_nEffectBG;					// 배경 프레임.
	int						m_nLuckyEffectBG;				// 배경 프레임.
	float					m_fTimeEffectBG;				// 배경 프레임 유지 시간.
	float					m_fLuckyTimeEffectBG;			// 배경 프레임 유지 시간.
	
	CINFImageBtn*			m_pBtnGamble;					// 뽑기 버튼.
	CINFImageBtn*			m_pBtnTake;						// 받기 버튼.

	// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	CINFImageBtn*			m_pBtnGamble1;					// 뽑기 버튼_해피머신.
	CINFImageBtn*			m_pBtnTake1;					// 받기 버튼_해피머신.
	//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경

	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	CINFImageEx*				m_pAME_slot[2];
	float					m_nAME_EffectNum[LUCKY_ITEM_LIST_NUM_LINE];
	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조

	CItemInfo*				m_pCoinItemInfo;
	ITEM_GENERAL			m_pSelectCoin;

	BOOL					m_bPrizeItem;					// 받을 아이템이 있는가?

	// 2009-03-04 by bhsohn 럭키 머신 수정안
	// 뒤로 가기 버튼
	CINFImageBtn*			m_pPrevStepBtn;			// 뒤로 가기 버튼
	
	// 럭키 머신 상점 선택
	CINFImageEx*				m_pImgSelctShopBK;
	CINFImageBtn*			m_pSelectShopPrevBtn;			// 럭키 머신 상점 전
	CINFImageBtn*			m_pSelectShopNextBtn;			// 럭키 머신 상점 다음
	vector<structNPCList*>	m_vecstruShopList;				// NPC리스트		

	int						m_nLuckyMechineStep;			// 스텝 //LUCKY_MAINSTEP_SELECT_TYPE	

	int						m_nLuckySelectStartIdx;			// 상점 스텝 시작점

	LUCKY_MACHINE_OMI		m_struSelLuckyMachine;			// 선택한 럭키 상점
	int						m_nLoadNpcListInfluenceType;

	// 2009-05-13 by bhsohn 럭키 상점 버그 수정
	UINT					m_uLoadBuildingIndex;
	// end 2009-05-13 by bhsohn 럭키 상점 버그 수정

	BOOL					m_bInfluenceOneShopMode;
	UINT					m_uBuildingIndex;
	
	
	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	BOOL					m_bYouLucky;	
	float					m_fYouLuckyViewTime;
	DWORD					m_dwYouLuckyAlpha;
	
	// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	// end 2009-03-04 by bhsohn 럭키 머신 수정안


	// 2009. 05. 07 by ckPark 럭키머신 버튼 수정

	DWORD					m_dwLastFrameTime;
	int						m_nCurFrame;

	// end 2009. 05. 07 by ckPark 럭키머신 버튼 수정

	// 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
	ITEM_GENERAL			*m_pItemCoinItemTmp;
	// end 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
};

#endif // !defined(AFX_INFLUCKYMACHINE_H__C96FAC40_4C48_40C6_A5C7_E6796220621A__INCLUDED_)
