// GiveItemManager.cpp: implementation of the CGiveItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldIOCPSocket.h"
#include "GiveItemManager.h"
#include "FieldGlobal.h"			// 2008-01-17 by cmkwon, S_F: 시스템 로그 추가 - 헤더파일 추가

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGiveItemManager::CGiveItemManager()
{
	m_mtEventItemList.reserve(10);
}

CGiveItemManager::~CGiveItemManager()
{
	m_mtEventItemList.clear();
}

// 2007-07-24 by dhjin, INT i_nUnitkind 추가
// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
//void CGiveItemManager::GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bPCBangCheck, BYTE i_byInfluenecType, BOOL i_bMemberShip, ATUM_DATE_TIME *i_Time, INT i_nUnitkind, ATUM_DATE_TIME *i_patLastGameEndDate)
// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -
//void CGiveItemManager::GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, ATUM_DATE_TIME *i_patLastGameEndDate)
// start 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 인자 추가
void CGiveItemManager::GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, ATUM_DATE_TIME *i_patLastGameEndDate, BYTE b_Level, UID32_t i_AccountUniqueNumber, INT i_LoginCheckNum)
//end 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -
{
	mt_auto_lock mtA { GetVectItemEventListPtr() };
	
	ATUM_DATE_TIME CurTime { true };

	for (auto& info : m_mtEventItemList)
	{
		//auto pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 

		if ( info.StartTime < CurTime && info.EndTime > CurTime)
		{

			if (ITEM_EVENT_TYPE_INFLCHANGE == info.ItemEventType || ITEM_EVENT_TYPE_LEVELUP == info.ItemEventType
				|| ITEM_EVENT_TYPE_FIRST_PURCHASE == info.ItemEventType)	// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급

				continue;

			if(info.iLevelMin && info.iLevelMax &&
				(b_Level < info.iLevelMin || b_Level > info.iLevelMax))
			{
				continue;
			}

			if (!IsEnableItemEvent(&info, i_bMemberShip, i_bPCBangUser, i_byInfluenecType, i_nUnitkind, i_patRegisteredDate,i_LoginCheckNum,i_AccountUniqueNumber))
				
				continue;

			// 2008-04-23 by cmkwon, 버그 수정, ITEM_EVENT_TYPE_ONLYONE 타입 일 때에만 atLastGameEndDate 를 체크해야 한다.
			if (ITEM_EVENT_TYPE_ONLYONE == info.ItemEventType && info.UseLastGameEndDate && info.atLastGameEndDate.IsValid())
			{
				auto patCheckDate = *i_patLastGameEndDate;

				// 2008-02-01 by cmkwon, LastGameEndDate가 유효하지 않으므로 RegisteredDate 로 체크한다.
				if (!patCheckDate.IsValid()) patCheckDate = *i_patRegisteredDate;

				// 2008-02-01 by cmkwon, 유효하지 안거나 설정 날짜 이후 시간 이면 가능 이벤트가 아니다.
				if (!patCheckDate.IsValid() || patCheckDate > info.atLastGameEndDate) continue;
			}

// 2013-02-28 by bckim, 복귀유져 버프추가
#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				
			if (ITEM_EVENT_TYPE_ONLYONE == info.ItemEventType && info.UseFixedPeriod && info.FixedPeriod_DAY)
			{
				auto patCheckDate = i_patLastGameEndDate;
				if (!i_patLastGameEndDate->IsValid())
				{				
					patCheckDate = i_patRegisteredDate;
				}
				auto tmpCurrentDate = ATUM_DATE_TIME::GetCurrentDateTime();

				// 마지막 접속시간이 현재 시간보다 이전이면 아이템 지급
				tmpCurrentDate.AddDateTime(0,0, -info.FixedPeriod_DAY , 0, 0, 0);
				if (!patCheckDate->IsValid() || *patCheckDate > tmpCurrentDate) continue;
			}			
#endif

			o_ItemEventInfo->push_back(info);
		}
	}
}

mtvectItemEventInfo * CGiveItemManager::GetVectItemEventListPtr()
{
	return &m_mtEventItemList;
}

BOOL CGiveItemManager::InitItemEventManager(vectItemEventInfo *i_pItemEventList)
{
	// 2008-01-17 by cmkwon, S_F: 시스템 로그 추가
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] ItemEvent reloaded !!, ItemEvent Count(%d)\r\n", i_pItemEventList->size());

	mt_auto_lock mtA(this->GetVectItemEventListPtr());				// 2006-08-24 by cmkwon

	m_mtEventItemList.clear();			// 2006-09-12 by cmkwon, 무조건 초기화 처리해야함

	if(i_pItemEventList->empty())
	{
		return FALSE;
	}
	
	m_mtEventItemList.assign(i_pItemEventList->begin(), i_pItemEventList->end());	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGiveItemManager::GetActEventItemInflChange(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate)
/// \brief		ITEM_EVENT_TYPE_NEWMEMBER_INFLCHANGE 이벤트 지급 체크
///				// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
/// \author		dhjin
/// \date		2007-07-20 ~ 2007-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGiveItemManager::GetActEventItemInflChange(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate)
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());

	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 

		if (ITEM_EVENT_TYPE_INFLCHANGE != m_mtEventItemList[i].ItemEventType)
		{
			continue;
		}

// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 아래와 같이 IsEnableItemEvent() 로 체크
// 		if (FALSE == (i_nUnitkind & m_mtEventItemList[i].UnitKind))
// 		{// 2007-07-24 by dhjin, 지급 대상이 아닌 유닛인지 체크
// 			continue;	
// 		}
// 
// 		if(m_mtEventItemList[i].NewMember
// 			&& (((m_mtEventItemList[i].StartTime) > *i_Time) 
// 				|| ((m_mtEventItemList[i].EndTime) < *i_Time)) )
// 		{// 2007-07-23 by dhjin, 이벤트 기간안에 가입한 유저 대상인지 체크
// 			continue;
// 		}
		if(FALSE == this->IsEnableItemEvent(pItemEvInfo, i_bMemberShip, i_bPCBangUser, i_byInfluenecType, i_nUnitkind, i_patRegisteredDate))
		{// 2008-02-01 by cmkwon, IsEnableItemEvent() 에서 체크
			continue;
		}

		o_ItemEventInfo->push_back(m_mtEventItemList[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGiveItemManager::GetActEventItemLevelUp(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, BYTE i_nLevel)
/// \brief		ITEM_EVENT_TYPE_NEWMEMBER_LEVELUP 이벤트 지급 체크
///				// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
/// \author		dhjin
/// \date		2007-07-20 ~ 2007-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGiveItemManager::GetActEventItemLevelUp(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, BYTE i_nLevel)
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());

	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 

		if (ITEM_EVENT_TYPE_LEVELUP != m_mtEventItemList[i].ItemEventType
			|| i_nLevel != m_mtEventItemList[i].ReqLevel)
		{
			continue;
		}
// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 아래와 같이 IsEnableItemEvent() 로 체크
// 		if (FALSE == (i_nUnitkind & m_mtEventItemList[i].UnitKind))
// 		{// 2007-07-24 by dhjin, 지급 대상이 아닌 유닛인지 체크
// 			continue;	
// 		}
// 
// 		if(m_mtEventItemList[i].NewMember
// 			&& (((m_mtEventItemList[i].StartTime) > *i_Time) 
// 				|| ((m_mtEventItemList[i].EndTime) < *i_Time)) )
// 		{// 2007-07-23 by dhjin, 이벤트 기간안에 가입한 유저 대상인지 체크
// 			continue;
// 		}
		if(FALSE == this->IsEnableItemEvent(pItemEvInfo, i_bMemberShip, i_bPCBangUser, i_byInfluenecType, i_nUnitkind, i_patRegisteredDate))
		{// 2008-02-01 by cmkwon, IsEnableItemEvent() 에서 체크
			continue;
		}

		o_ItemEventInfo->push_back(m_mtEventItemList[i]);
	}
}

// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGiveItemManager::GetActEventItemFirstPurchase(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME i_atAccountLastBuyDate, ATUM_DATE_TIME i_atCharacterLastBuyDate)
/// \brief		매월 첫 결재시 선물지급
/// \author		jhseol
/// \date		2013-03-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGiveItemManager::GetActEventItemFirstPurchase(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME i_atAccountLastBuyDate, ATUM_DATE_TIME i_atCharacterLastBuyDate)
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());
	
	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
		
		if (ITEM_EVENT_TYPE_FIRST_PURCHASE != pItemEvInfo->ItemEventType) continue;

		ATUM_DATE_TIME CurrentTime { true };
		if ( CurrentTime < pItemEvInfo->StartTime || CurrentTime > pItemEvInfo->EndTime )
		{
			continue;
		}
		if ( TRUE == pItemEvInfo->CheckWithCharacterUID )
		{
			if ( i_atCharacterLastBuyDate > pItemEvInfo->StartTime )
			{
				continue;
			}
		}
		else
		{
			if ( i_atAccountLastBuyDate > pItemEvInfo->StartTime )
			{
				continue;
			}
		}
		if(FALSE == this->IsEnableItemEvent(pItemEvInfo, i_bMemberShip, i_bPCBangUser, i_byInfluenecType, i_nUnitkind, &CurrentTime))
		{
			continue;
		}
		
		o_ItemEventInfo->push_back(m_mtEventItemList[i]);
	}
}
// end 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGiveItemManager::IsEnableItemEvent(SITEM_EVENT_INFO *i_pItemEvInfo, BOOL i_bMemberShip, BOOL i_bPCBangCheck, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate)
/// \brief		// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - CGiveItemManager::IsEnableItemEvent() 추가
///				// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 체크넘버 인자추가
/// \author		cmkwon
/// \date		2008-02-01 ~ 2008-02-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGiveItemManager::IsEnableItemEvent(SITEM_EVENT_INFO *i_pItemEvInfo, BOOL i_bMemberShip, BOOL i_bPCBangCheck, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, INT i_LoginEventCheckNumber, UID32_t i_AccountUniqueNumber/*=0*/)
{
	if(i_pItemEvInfo->OnlyPCBang
		&& FALSE == i_bPCBangCheck)
	{
		return FALSE;
	}

	if (i_pItemEvInfo->MemberShip
		&& FALSE == i_bMemberShip)
	{
		return FALSE;
	}

	if (INFLUENCE_TYPE_ALL_MASK != i_pItemEvInfo->byInfluenceType
		&& i_pItemEvInfo->byInfluenceType != i_byInfluenecType)
	{
		return FALSE;
	}

	if (FALSE == (i_nUnitkind & i_pItemEvInfo->UnitKind))
	{
		return FALSE;
	}

	if( i_pItemEvInfo->NewMember
		&& (i_pItemEvInfo->StartTime > *i_patRegisteredDate || i_pItemEvInfo->EndTime < *i_patRegisteredDate) )
	{
		return FALSE;
	}
	// start 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 인자로 받은 체크넘버로 체크

	if( ITEM_EVENT_TYPE_LOGIN == i_pItemEvInfo->ItemEventType
		|| 0 != i_pItemEvInfo->LoginCheckNumber )
	{
		// 한번만 수행되어야 한다.
		if(i_LoginEventCheckNumber == i_pItemEvInfo->LoginCheckNumber) // 같으면 완료된 것으로 처리
		{
			// 이벤트 게임 로그는 남으므로 굳이 안남겨도 된다.
			// g_pFieldGlobal->WriteSystemLogEX(TRUE,"Login Event Success!! CheckEventNumber[%d], AccountUniqueNumber[%d]\r\n",i_LoginEventCheckNumber,i_AccountUniqueNumber);
			return TRUE;
		}
		return FALSE;	// 로그인이벤트이고 위에 두 조건을 충족하지 않으면 무조껀 실패
	}
	// end 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 인자로 받은 체크넘버로 체크
	return TRUE;
}

// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 체크함수 추가
BOOL CGiveItemManager::IsEnableCheckLoginEvent()
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());
	
	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);		// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
		
		if (ITEM_EVENT_TYPE_LEVELUP != m_mtEventItemList[i].ItemEventType) return TRUE;
	}
	return FALSE;
}

// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
SITEM_EVENT_INFO* CGiveItemManager::GetItemEventInfoByEventUID(int i_nItemEventUID)
{
	mt_auto_lock mtA(this->GetVectItemEventListPtr());
	
	int v_size = m_mtEventItemList.size();
	for (int i=0; i < v_size;i++)
	{
		SITEM_EVENT_INFO *pItemEvInfo = &(m_mtEventItemList[i]);
		
		if ( i_nItemEventUID == pItemEvInfo->ItemEventUID )
		{
			return pItemEvInfo;
		}
	}
	return NULL;
}
// end 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
