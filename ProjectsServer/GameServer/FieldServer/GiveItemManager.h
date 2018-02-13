// GiveItemManager.h: interface for the CGiveItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIVEITEMMANAGER_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_)
#define AFX_GIVEITEMMANAGER_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef	mt_vector<SITEM_EVENT_INFO>	mtvectItemEventInfo;

class CFieldIOCPSocket;
class CGiveItemManager  
{
public:
	BOOL InitItemEventManager(vectItemEventInfo *i_pItemEventList);
	mtvectItemEventInfo * GetVectItemEventListPtr(void);
	CGiveItemManager();
	virtual ~CGiveItemManager();

// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
// 	void GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bPCBangCheck, BYTE i_byInfluenecType, BOOL i_bMemberShip, ATUM_DATE_TIME *i_Time, INT i_nUnitkind, ATUM_DATE_TIME *i_patLastGameEndDate);          // 2007-07-23 by dhjin, 유저 접속 시간에 진행중인 이벤트 얻어오는 함수(이벤트 기안안에 가입한 유저 대상 체크 추가)
// 	void GetActEventItemInflChange(vectItemEventInfo *o_ItemEventInfo, ATUM_DATE_TIME *i_Time, INT i_nUnitkind);				// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_INFLCHANGE 이벤트 지급 체크
// 	void GetActEventItemLevelUp(vectItemEventInfo *o_ItemEventInfo, ATUM_DATE_TIME *i_Time, BYTE i_nLevel, INT i_nUnitkind);				// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_LEVELUP 이벤트 지급 체크
	//void GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, ATUM_DATE_TIME *i_patLastGameEndDate);          // 2007-07-23 by dhjin, 유저 접속 시간에 진행중인 이벤트 얻어오는 함수(이벤트 기안안에 가입한 유저 대상 체크 추가)
	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 인자 추가 
	void GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, ATUM_DATE_TIME *i_patLastGameEndDate, BYTE b_Level, UID32_t i_AccountUniqueNumber, INT i_LoginCheckNum);  // 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -
	void GetActEventItemInflChange(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate);				// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_INFLCHANGE 이벤트 지급 체크
	void GetActEventItemLevelUp(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, BYTE i_nLevel);				// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_LEVELUP 이벤트 지급 체크
	void GetActEventItemFirstPurchase(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME i_atAccountLastBuyDate, ATUM_DATE_TIME i_atCharacterLastBuyDate);			// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급

	// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 체크넘버 인자추가 
	BOOL IsEnableItemEvent(SITEM_EVENT_INFO *i_pItemEvInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, INT i_LoginEventCheckNumber=0, UID32_t i_AccountUniqueNumber = 0);
	
	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 로그인이벤트 진행유무 판단 함수
	BOOL IsEnableCheckLoginEvent();

	SITEM_EVENT_INFO* GetItemEventInfoByEventUID(int i_nItemEventUID);	// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급

protected:
	mtvectItemEventInfo	m_mtEventItemList;
};

#endif // !defined(AFX_GIVEITEMMANAGER_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_)
