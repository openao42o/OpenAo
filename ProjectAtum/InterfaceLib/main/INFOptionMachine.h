// INFOptionMachine.h: interface for the INFOptionMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPTIONMACHINE_H__CDAC2CA9_6BE1_4B0A_B4BD_FDA9ED7DADEB__INCLUDED_)
#define AFX_INFOPTIONMACHINE_H__CDAC2CA9_6BE1_4B0A_B4BD_FDA9ED7DADEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"

class CINFImage;
class CINFImageEx;			// 2011. 10. 10 by jskim UI시스템 변경
class CItemInfo;

#define OPTION_ITEM_LIST_NUM							5			// 화면에 보여주는 옵션 수
#define OPTION_ITEM_LIST_CENTER							2			// 옵션 머신의 중간

#define OPTION_SELECT_ITEM								3			// 접미, 접두, 아머

#define OPTION_ITEM_LIST_FIX							2			// 접두, 접미 방식

#define OPTION_ITEM_PRE_FIX								0			// 접두
#define OPTION_ITEM_SUF_FIX								1			// 접미
#define OPTION_TARGET_ITEM								2			// 아머, 무기


#define SUPER_OPTION_ITEM								0			// 슈옵 옵션머신 
#define NORMAL_OPTION_ITEM								1			// 일반 옵션머신

enum			
{
	OPTION_STATE_READY,												// 대기
	OPTION_STATE_RULLET,											// 진행
	OPTION_STATE_PRIZE												// 결과 
};

#define	MAX_RULLET_PATTERN								2			// 슬롯이 돌아가는 패턴

enum
{
	PATTERN1,
	PATTERN2,
	PATTERN3,
};

#define	MAX_SLOT_SPEED_STEP								5			// 슬롯의 스피드 단계

#define	OPTION_SPEED_NORMAL								3			// 기본
#define OPTION_SPEED_SLOW								1			// 느림
#define	OPTION_SPEED_FAST								5			// 빠름 

#define	STEP_SPEED										0.02f		
#define TRICK_SPEED										1.0f
		
struct ST_ITEM_INFO
{
	int		nItemCode;					// 코드 번호 
	char*	nItemName;					// 아이템 이름
	int		nItemPosition;				// 아이템 위치 
};

struct ST_RENDER_ITEM
{
	char*					nItemName;
	int						nItemPosition;				// 아이템 위치 
	int						nImageType;
};

struct ST_SLOT_INFO
{
	ST_SLOT_INFO()
	{
		fSlotTime		= NULL;
		fAllSlotTime	= NULL;
		bCompleteStep	= FALSE;
		nSlotNum		= NULL;
		nSpeed			= OPTION_SPEED_NORMAL;
		nSpeedCourse	= NULL;
		nTrick			= NULL;
		nItemPosition	= NULL;
		nUseSlot		= FALSE;
	}
	vector<ST_ITEM_INFO*> nItemInfo;

	FLOAT		fSlotTime;				// 한칸 진행한 시간
	FLOAT		fAllSlotTime;			// 슬롯이 총 진행한 시간 

	int			nSpeed;					// 현재 스피드 단계
	int			nSpeedCourse;			// 움직이는 방향
	int			nItemPosition;			// 현재 보여지는 위치
	BOOL		bCompleteStep;			// 완료 체크
	BOOL		bChosePattern;			// 선택 체크 
	int			nSlotNum;
	int	        nTrick;
	BOOL		nUseSlot;			// 슬롯을 사용하는지
};

class CINFOptionMachine : public CINFBase
{
public:
	virtual ~CINFOptionMachine();
	CINFOptionMachine(CAtumNode* pParent, BUILDINGNPC* pBuilding);

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Render();
	virtual void Tick();
	void		TickSlot(float i_fElapsedTime);
	BOOL		IsAllRuletDone();
	void		RenderSlot();
	void		RenderCard(CItemInfo* nSelectItem, int nPosition);
	
	float		GetStepSpeed(int Step);
	void		OptionSlotDraw(int x, int y, ST_RENDER_ITEM* RenderItem, int nPosition, BOOL slotDark = FALSE);
	void		ChangeState(int i_nState);
	void		ChangePattern(ST_SLOT_INFO* i_nInfo);
	void		ChangeSpeed(ST_SLOT_INFO* i_nInfo);
	void		SlotTranslate(ST_SLOT_INFO* i_nInfo);
	BOOL		TrickPattern(ST_SLOT_INFO* i_nInfo);
	int			GetItemPosition(ST_SLOT_INFO* i_nInfo, ITEM_GENERAL* i_Item);
	int			RestPosition(ST_SLOT_INFO* i_nInfo, int Position);

	void		GambleButtonOK();

	void		SetSlotinfo(vectRARE_ITEM_INFOPtrList mRareitem, int position);
	void		ResetInfo();
	void		ResetInfo(vector<ST_RENDER_ITEM*> mRenderItem);

	void		UpDataInfo();
	void		PrizeItemGiveMe();
	
	int			PossibleUpLoadItemNum(ITEM* pItem);
	BOOL		IsPossibleUpLoadItem(ITEM* pItem, int nPosition);
	void		UpLoadItem(CItemInfo* i_pItem, int nPosition);
	void		ClearItemInfo(int nPosition);
	BOOL		PutRareInfo(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg);
	void		DeleteTargetItem(int nItemUniqueNumber);
	
	// 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 아이템 올려 놓는 구조 변경
	//void		UsingItemLoad(int nPosition);
	void		UsingItemLoad();
	//end 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 아이템 올려 놓는 구조 변경
	BOOL		IsCloseOptionMachine();
	void		EnterMachineShop();
	void		OnCloseInfWnd();
	
private:
	CINFImageEx*				m_pImgOptionBG;				// 옵션 정보의 배경 이미지

	CINFImageEx*				m_pImgOptionEffBG[7];		// 옵션 이펙트 배경 이미지

// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	CINFImageEx*				m_pImgSlotDark;				// 성공시 안보이는 슬롯을 가리는 이미지

	CINFImageEx*				m_pImgPreArrow[3];			// 접두 화살표 이팩트 이미지
	CINFImageEx*				m_pImgSufArrow[3];			// 접두 화살표 이팩트 이미지
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	CINFImageEx*				m_pImgSlotPreQuestion;		// 접두 물음표
	CINFImageEx*				m_pImgSlotPreBG[2];			// 슬롯 돌아갈때 배경
	
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	CINFImageEx*				m_pAME_slot[2];				// 받을 아이템 이팩트 강조
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	
	CINFImageEx*				m_pImgSlotSufQuestion;		// 접미 물음표
	CINFImageEx*				m_pImgSlotSufBG[2];			// 접미 슬롯 돌아갈때 배경
	
	CINFImageEx*				m_pImgPrizePrePositionCur;	// 접두 받을 위치 표시
	CINFImageEx*				m_pImgPrizeSufPositionCur;	// 접두 받을 위치 표시

	CINFImageBtn*			m_pBtnStart;				// 머신 시작 
	CINFImageBtn*			m_pBtnTake;					// 아이템 받기

// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	BOOL					m_IsArrow[2];				// 화살표
	int						m_isArrowCnt[2];			// 화살카운터
	BOOL					m_isEffect;					// 받을 아이템 이팩트
	int						m_bisInit;					//						
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조	
	int						m_nEffectIdx;				// 메인 위 이미지 인덱스
	float					m_fTimeEffectBG;			// 배경 프레임 유지 시간

	BOOL					m_IsShopMode;				// 럭키머신 사용하고 있는지
	int						m_nState;					// 슬롯의 상태	
	
	CD3DHanFont*			m_pOptionFont;				// 옵션에 보여줄 글씨체	

	ST_SLOT_INFO*			m_SlotInfo[OPTION_ITEM_LIST_FIX];	// 슬롯의 정보
	
	float					m_fTrickTime;				// 트릭 시간
	vector<ST_RENDER_ITEM*>	m_RenderPreOption;			// 접미
	vector<ST_RENDER_ITEM*>	m_RenderSufOption;			// 접투
	
	vectRARE_ITEM_INFOPtrList m_RareItemList[OPTION_ITEM_LIST_FIX];			// 전체 아이템 리스트
	vector<ST_ITEM_INFO*>	m_vecItemInfo[OPTION_ITEM_LIST_FIX];			// 선택 아이템에 들어갈 옵션 정보
	CItemInfo*				m_pPrizeItem;				// 받을 아이템 정보
	CItemInfo*				m_pSelectItemInfo[OPTION_SELECT_ITEM];		// 아머 아이템, 접미, 접두
};

#endif // !defined(AFX_INFOPTIONMACHINE_H__CDAC2CA9_6BE1_4B0A_B4BD_FDA9ED7DADEB__INCLUDED_)
