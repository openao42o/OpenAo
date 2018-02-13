// INFCharacterInfo.h: interface for the CINFCharacterInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCHARACTERINFO_H__F599EA3D_A58A_4070_A9C1_6C692D4FABEC__INCLUDED_)
#define AFX_INFCHARACTERINFO_H__F599EA3D_A58A_4070_A9C1_6C692D4FABEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
//#include "INFInven.h"


#define CHARACTER_INFO		0
#define CHARACTER_AMMO		1
#define CHARACTER_UTC		2
#define CHARACTER_ARENA		3

#define CHARACTER_INTERFACE_FONT_NUM		6

#define STAT_ATTACK			0
#define STAT_DURABILITY		1
#define STAT_FUEL			2
#define STAT_INDUCTION		3
#define STAT_AVOID			4
#define STAT_KALISMA		5
#define STAT_NONE			8

#define STAT_BUTTON_DISABLE		0
#define STAT_BUTTON_UP			1
#define STAT_BUTTON_DOWN		2

#define CHAR_UTC_TYPE_NUMBER	4
#define CHAR_UTC_SLOT_NUMBER	5

#define C_QUEST_LIST_NUM		6
#define C_QUEST_DESC_NUM		5

// 2007-05-15 by bhsohn 기어 스탯 관련 처리
#define STAT_SHUTTLEINFO_ATTACK		0		// 공격
#define STAT_SHUTTLEINFO_DEFENCE	1		// 방어
#define STAT_SHUTTLEINFO_FUEL		2		// 연료
#define STAT_SHUTTLEINFO_SOUL		3		// 정신
#define STAT_SHUTTLEINFO_DODGE		4		// 회피
#define STAT_SHUTTLEINFO_SHIELD		5		// 쉴드
#define	MAX_STAT_SHUTTLEINFO		6

class CD3DHanFont;
class CINFImage;
class CINFImageEx;										   // 2011. 10. 10 by jskim UI시스템 변경
class CAtumNode;
class CINFScrollBar;
class CSkillInfo;
class CINFImageBtn;		// 2008-06-20 by bhsohn EP3 옵션관련 처리
class CINFCharacterInfo : public CINFBase
{
public:
	CINFCharacterInfo(CAtumNode* pParent);
	virtual ~CINFCharacterInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void RenderSkillInShop();
	virtual void Tick();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int InfoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int UTCWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int AmmoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcSkillInShop(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int QuestWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int ArenaWndProc(UINT uMsg,WPARAM wParam,LPARAM lParam);

	void SetAllUTCInfo();
	void SetSelectUTC( INVEN_DISPLAY_INFO* pDisplayInfo );
	void SendUseSkill( ITEM_BASE* pSkill );

	int GetCharacterQuestCount();
	void SetQuestDesc(int nQuestWindowDataIndex);
	int GetUtcMaxScrollNumber();
	void ShowStatInfo() ;
	void SendDeleteSelectItemSkill();
	void RenderNonQuest(int nWindowPosY, int nLine);
	void RenderStatDescToolTip(int nType, int x, int y);

	BOOL CheckUseSkillApprovalEnd(ITEM_BASE* pSkill);
	
public:
	BOOL					m_bRestored;
	BOOL					m_bInvalidated;

	CINFImageEx			*	m_pAmmo;
	CINFImageEx			*	m_pStatImage ;
	CINFImageEx			*	m_pStat[3];
	CINFImageEx			*	m_pInfo;
	CINFImageEx			*	m_pInfoLog;
	CINFImageEx			*	m_pId;
	CINFImageEx			*	m_pUtc;
	CINFImageEx			*	m_pUtcScr;
//	CINFImage			*	m_pQuest;
	CINFImageEx			*	m_pImgArenaTab;
	CINFImageEx			*	m_pQuestSelect;
	CINFImageEx			*	m_pSkillShopBack;
//	CINFImage			*	m_pScrollBar;

	CD3DHanFont			*   m_CharacInfoFont[5];
	CD3DHanFont			*   m_StatInfoFont[7];
	CD3DHanFont			*	m_pFontName;
	CD3DHanFont			*	m_pFontLevel;
	CD3DHanFont			*	m_pFontGuild;
	CD3DHanFont			*	m_pFontGeneral;
	CD3DHanFont			*	m_pFontPropensity;
	CD3DHanFont			*	m_pFontItemNum[CHAR_UTC_SLOT_NUMBER];

	CD3DHanFont			*	m_pFontQuestList[C_QUEST_LIST_NUM];
//	CD3DHanFont			*	m_pFontQuestState[6];
	CD3DHanFont			*	m_pFontQuestDesc[C_QUEST_DESC_NUM];
	CD3DHanFont			*	m_pFontAmmoWeapon[CHARACTER_INTERFACE_FONT_NUM];	// 1형 공격/연사, 2형 공격/멀티샷, 방어, 회피
	CD3DHanFont			*	m_pFontAutoStat;
	CD3DHanFont			*	m_pFontStatDec;

	int						m_nCharacterInfoType;
	int						m_nStatType ;
	int						m_nButtonState ;
	int						m_nMissionProCount;		// 진행중 미션 카운트



	int					m_nCurrentUTCScroll;
	INVEN_DISPLAY_INFO	m_pUTCDisplayInfo[CHAR_UTC_TYPE_NUMBER][CHAR_UTC_SLOT_NUMBER];
	CINFImageEx*		m_pSelectIcon;// 마우스 클릭에 의해 선택된 아이콘 이미지(reference)
	ITEM_SKILL*			m_pSelectSkill;// 스킬 포인터(reference)
	ITEM_SKILL*			m_pDeleteSkill;// 지울 스킬 포인터(reference)
	BOOL				m_bUTCScrollLock;
	POINT				m_ptMouse;

//	int					m_nCurrentQuestListScroll;
//	int					m_nCurrentSelectQuestList;
//	BOOL				m_bQuestListLock;
//	int					m_nCurrentQuestDescScroll;
//	int					m_nCurrentSelectQuestDesc;
//	BOOL				m_bQuestDescLock;
	int					m_nRenderMoveIconIntervalWidth;
	int					m_nRenderMoveIconIntervalHeight;

	CINFScrollBar*		m_pQuestListScrollBar;
	CINFScrollBar*		m_pQuestDescScrollBar;
	vector<string>		m_vecQuestDescription;
	int					m_nOldQuestSelectDataIndex;

	CINFScrollBar*		m_pShopSkillScrollBar;
	BOOL				m_bLbuttonDown;	
//	BOOL				m_bToolTipStat;				// 2005-07-04 by ispark 자동 스탯 분배 삭제

private:
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	void				GetStatShowInfo();

private:
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	CD3DHanFont			*   m_StatInfoBoldFont;		// 두꺼운 글씨체 
	CD3DHanFont			*   m_StatOverInfoFont;
	BOOL					m_bStatShow[MAX_STAT_SHUTTLEINFO];
	BOOL					m_nShowOverStat[MAX_STAT_SHUTTLEINFO];

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	CINFImageBtn*			m_pOpenBtn;			// 닫기	
};

#endif // !defined(AFX_INFCHARACTERINFO_H__F599EA3D_A58A_4070_A9C1_6C692D4FABEC__INCLUDED_)
