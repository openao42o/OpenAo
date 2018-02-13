// INFGameMainQSlot.h: interface for the CINFGameMainQSlot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINQSLOT_H__FFB099C3_2ED9_43D2_A454_A5491DE43A39__INCLUDED_)
#define AFX_INFGAMEMAINQSLOT_H__FFB099C3_2ED9_43D2_A454_A5491DE43A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
//#include "GameDataLast.h"
//#include "INFImage.h"
//#include "INFInven.h"

#define QSLOT_NUMBER		10
#define QSLOT_TAB_NUMBER	3//8

#define QSLOT_BUTTON_UP		0
#define QSLOT_BUTTON_DOWN	1

#define QSLOT_BUTTON_STATE_NUMBER	4
#define QSLOT_BUTTON_NUMBER			2

#define QSLOT_ICON_SIZE		 28
#define QSLOT_COUNTERBLE_NUMBER		RGB(255,255,255)

#define QSLOT_ITEMTYPE_NONE		0	// 퀵슬롯 아이템 타입
#define QSLOT_ITEMTYPE_QSLOT	1	// 퀵슬롯 아이템 타입
#define QSLOT_ITEMTYPE_SKILL	2	// 퀵슬롯 아이템 타입
#define QSLOT_ITEMTYPE_ITEM		3	// 퀵슬롯 아이템 타입

#define QSLOT_SIZE_X			318//254
#define QSLOT_SIZE_Y			30
#define QSLOT_ICON_INTERVAL		32
#define QSLOT_TIMER				0.4f

// 2008-11-13 by bhsohn 조이스틱 작업
// 조이스틱에서 사용을 위한 스킬들
typedef struct 
{
	int		nCurrentTab;
	int		nSlotNumber;			// 사용하려는 키 단축
} structJoystikcSkillList;
// end 2008-11-13 by bhsohn 조이스틱 작업


class CAtumNode;
class CINFImage;
class CD3DHanFont;
class CINFImageEx;
class CINFImageBtn;

class CINFGameMainQSlot : public CINFBase  
{
public:
	CINFGameMainQSlot(CAtumNode* pParent);
	virtual ~CINFGameMainQSlot();

	void SetQSlotInfo(int nTab, int nNumber, ITEM_BASE* pItem);
	// 2008-12-15 by bhsohn 기체가 죽었을시, 더블클릭으로 스킬 발동 버그 수정
	//void UseQuickSlot(int nCurrentTab, int nSlotNumber);
	BOOL UseQuickSlot(int nCurrentTab, int nSlotNumber);
	// end 2008-12-15 by bhsohn 기체가 죽었을시, 더블클릭으로 스킬 발동 버그 수정
	BOOL IsValidQSlotInfo(int nTab, int nNumber);
	void UpdateQick(int nItemNum) ;
	void SetAllQSlotInfo();
	BOOL LButtonUpQuickSlot(POINT pt);
	void StartReattackTime(ITEM *pItem);

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 2007-07-09 by bhsohn 출격과 바자상점 동시 사용시 문제점 처리
	BOOL IsBazarOpen();

	// 2007-11-22 by bhsohn 아레나 통합서버
	void RefreshQSlotInfo();

	// 2008-11-13 by bhsohn 조이스틱 작업
	void AddCurrentTab();
	void AddJoystikcQuickSlotList(int nCurrentTab, int nSlotNumber);
	void TickJoysticSlot();
	
	// 2008-12-15 by bhsohn 기체가 죽었을시, 더블클릭으로 스킬 발동 버그 수정
	BOOL IsPossibleJoysticSlot();

	// 2010. 04. 01 by ckPark 리소스 변경 시스템시 팩토리나 연구소에서 아이템 회수한 후 변경이 안되는 문제 해결
	void	SetIconName( CItemInfo* pItemInfo, char* szName );
	void	UpdateIconName( CItemInfo* pItemInfo );
	// end 2010. 04. 01 by ckPark 리소스 변경 시스템시 팩토리나 연구소에서 아이템 회수한 후 변경이 안되는 문제 해결
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	BOOL IsSlotOpen() { return m_pIsSlotOpen; }													  
#endif

	// 2012-12-21 by bhsohn [드레인모듈] 아이템 슬롯 등록 안되게 수정
	BOOL IsPossibleSetQuickSlot(int nItemNum);
	// END 2012-12-21 by bhsohn [드레인모듈] 아이템 슬롯 등록 안되게 수정

	// 2013-07-01 by bhsohn QuickSlot변경시 시간 체크하여 바로 저장하게 변경
	void TickCheckQuiclSlotSave();	
	void SetCheckQuiclSlotSave(float fCheckQuiclSlotSave);
	// END 2013-07-01 by bhsohn QuickSlot변경시 시간 체크하여 바로 저장하게 변경

protected:
	void SetToolTip(int x, int y, ITEM_BASE* pItem);
	void SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo);
	BOOL RenderDisableSkill(int nSkillNum);
	void RenderSkillReAttackTime(int nItemNum, int nRenderIndex, int nLine = 0, float fsizeX = 0.0f, float fsizeY = 0.0f );
	void RenderItemUsableReAttackTime(int nItemNum, int nRenderIndex, int nLine =0, float fsizeX = 0.0f, float fsizeY = 0.0f);
	
	BOOL IsQSlotShowTime(ITEM *ItemInfo);


public:
	INVEN_DISPLAY_INFO m_pQSlotInfo[QSLOT_TAB_NUMBER][QSLOT_NUMBER];//8x8=64개의 단축키 지원
	INVEN_DISPLAY_INFO m_pSelectItem;
//	ITEM_BASE*		   m_pSelectItem;
	int				   m_nSelectSlotNumber;
	int				   m_nCurrentTab;							//현재 슬롯의 탭위치.
	int				   m_nItemType;
	int			m_nX;
	int			m_nY;	

	BOOL				m_bLButtonDown;

protected:
	BOOL		m_bRestored;

	CINFImageEx*	m_pBack;
	CINFImageEx*	m_pNumber;														
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	//CINFImageEx*	m_pImgTabButton[QSLOT_BUTTON_NUMBER][QSLOT_BUTTON_STATE_NUMBER];
	CINFImageBtn*	m_pImgTabButton[ QSLOT_BUTTON_NUMBER ];	
#else  
	CINFImage*	m_pImgTabButton[QSLOT_BUTTON_NUMBER][QSLOT_BUTTON_STATE_NUMBER];
#endif
	CINFImageEx*	m_pImgDisSkill;
	CINFImageEx*	m_pImgBlind;
	int			m_nButtonState[QSLOT_BUTTON_NUMBER];	
	float		m_fQSlotTimer;	
	int			m_nRenderMoveIconIntervalWidth;
	int			m_nRenderMoveIconIntervalHeight;

	CD3DHanFont*		m_vecFontLine[QSLOT_NUMBER];
	// 2007-01-22 by bhsohn 탭키 인터 페이스 수정안
	CD3DHanFont*		m_pFontTabNum;

	int			m_nQSlotSwapTab;
	int			m_nQSlotSwapNum;
	BOOL		m_bQSlotSwapFlag;

	ITEM_BASE*  m_pQSlotMove;

	// 2007-07-09 by bhsohn 출격과 바자상점 동시 사용시 문제점 처리	
	float					m_fClickBazaar;

	// 2008-11-13 by bhsohn 조이스틱 작업
	float					m_fJoystikcSkillList;
	deque<structJoystikcSkillList>	m_vecJoystikcSkillList;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	BOOL					m_pIsSlotOpen;
#endif
	
	float					m_fCheckQuiclSlotSave; // 2013-07-01 by bhsohn QuickSlot변경시 시간 체크하여 바로 저장하게 변경
};

#endif // !defined(AFX_INFGAMEMAINQSLOT_H__FFB099C3_2ED9_43D2_A454_A5491DE43A39__INCLUDED_)
