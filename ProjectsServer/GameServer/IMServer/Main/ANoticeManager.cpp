// ANoticeManager.cpp: implementation of the CANoticeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ANoticeManager.h"
#include "IMGlobal.h"
#include "IMIOCP.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
CANoticeManager::CANoticeManager(CIMIOCP *i_pIOCP)
{
	m_pIMIOCP6		= i_pIOCP;
	this->ResetANoticeManager();
}

// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
CANoticeManager::~CANoticeManager()
{
	this->ResetANoticeManager();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CANoticeManager::ResetANoticeManager(void)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-15 ~ 2009-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CANoticeManager::ResetANoticeManager(void)
{
	mt_auto_lock mtA(&m_mtvectANoticeStringList);

	util::zero(&m_ANoticeInfo, sizeof(m_ANoticeInfo));
	m_mtvectANoticeStringList.clear();

	m_nSizemtvectANoticeStringList	= 0;
	m_nNextNoticeStringVectIndex	= -1;	// 2009-01-15 by cmkwon, 초기값
	m_atLastSentTime.SetCurrentDateTime();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL InitANoticeManager(SANoticeInfo *i_pNoticeInfo, mtvectSANoticeString *i_pmtvectNoticeStringList, BOOL i_bInstantlyStart=FALSE)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-15 ~ 2009-01-15
/// \warning	
///
/// \param		i_pmtvectNoticeStringList : NoticeStringIndex로 정렬된 상태여야 한다.
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CANoticeManager::InitANoticeManager(SANoticeInfo *i_pNoticeInfo, mtvectSANoticeString *i_pmtvectNoticeStringList, BOOL i_bInstantlyStart/*=FALSE*/)
{	
	mt_auto_lock mtA(&m_mtvectANoticeStringList);

	this->ResetANoticeManager();

	m_ANoticeInfo.UsingFlag		= i_pNoticeInfo->UsingFlag;
	m_ANoticeInfo.LoopSec		= max(MIN_LOOP_SECONDS_OF_ADMIN_NOTICE, i_pNoticeInfo->LoopSec);
	m_ANoticeInfo.LoopSec		= min(MAX_LOOP_SECONDS_OF_ADMIN_NOTICE, i_pNoticeInfo->LoopSec);
	m_ANoticeInfo.IntervalSec	= max(MIN_INTERVAL_SECONDS_OF_ADMIN_NOTICE, i_pNoticeInfo->IntervalSec);
	m_ANoticeInfo.IntervalSec	= min(MAX_INTERVAL_SECONDS_OF_ADMIN_NOTICE, i_pNoticeInfo->IntervalSec);
	util::strncpy(m_ANoticeInfo.EditorAccountName, i_pNoticeInfo->EditorAccountName, SIZE_MAX_ACCOUNT_NAME);

	if(i_pNoticeInfo->UsingFlag
		&& false == i_pmtvectNoticeStringList->empty())
	{
		m_mtvectANoticeStringList.insert(m_mtvectANoticeStringList.end(), i_pmtvectNoticeStringList->begin(), i_pmtvectNoticeStringList->end());
		m_nSizemtvectANoticeStringList	= m_mtvectANoticeStringList.size();
	}

	BOOL bValid = this->IsValidAdminAutoNoticeNoLock();

	if(i_bInstantlyStart
		&& bValid)
	{
		// 2009-01-19 by cmkwon, 바로 실행을 위해서 간격 시간 뺀것을 설정한다.
		m_atLastSentTime.AddDateTime(0, 0, 0, 0, 0, -DELAY_SEC_AFTER_SERVER_STARTED);
	}

	return bValid;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CANoticeManager::IsValidAdminAutoNoticeNoLock(void)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-15 ~ 2009-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CANoticeManager::IsValidAdminAutoNoticeNoLock(void)
{
	if(FALSE == m_ANoticeInfo.UsingFlag
		|| 0 >= m_ANoticeInfo.LoopSec
		|| 0 >= m_ANoticeInfo.IntervalSec
		|| 0 >= m_nSizemtvectANoticeStringList)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CANoticeManager::GetNoticeStringforSending(SANoticeString *o_pANoticeStr, ATUM_DATE_TIME *i_pDateTime)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-15 ~ 2009-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CANoticeManager::GetNoticeStringforSending(SANoticeString *o_pANoticeStr, ATUM_DATE_TIME *i_pDateTime)
{
	mt_auto_lock mtA(&m_mtvectANoticeStringList);
	if(FALSE == this->IsValidAdminAutoNoticeNoLock()
		|| 0 >= m_mtvectANoticeStringList.size())
	{
		return FALSE;
	}

	int nNeedSec	= 0;
	if(0 > m_nNextNoticeStringVectIndex)
	{
		nNeedSec	= DELAY_SEC_AFTER_SERVER_STARTED;	// 2009-01-15 by cmkwon, 시스템 시작 됨, 기본 지연 시간 체크		
	}
	else if(0 == m_nNextNoticeStringVectIndex)
	{
		nNeedSec	= m_ANoticeInfo.LoopSec;			// 2009-01-15 by cmkwon, 자동 공지 전송 다시 시작됨, Loop Time 체크
	}
	else
	{
		nNeedSec	= m_ANoticeInfo.IntervalSec;		// 2009-01-15 by cmkwon, 다음 자동 공지 전송, Interval Time 체크
	}

	int nElapseSec	= i_pDateTime->GetTimeDiffTimeInSeconds(m_atLastSentTime);
	if(nElapseSec < nNeedSec)
	{
		return FALSE;
	}

	if(0 > m_nNextNoticeStringVectIndex)
	{// 2009-01-15 by cmkwon, 초기값일 경우 0으로 설정
		m_nNextNoticeStringVectIndex	= 0;
	}

	m_atLastSentTime				= *i_pDateTime;		// 2009-01-15 by cmkwon, 전달된 시간 설정
	*o_pANoticeStr					= m_mtvectANoticeStringList[m_nNextNoticeStringVectIndex];			// 2009-01-15 by cmkwon, 전송 할 공지 사항 설정
	m_nNextNoticeStringVectIndex	= (++m_nNextNoticeStringVectIndex)%m_nSizemtvectANoticeStringList;	// 2009-01-15 by cmkwon, 다음 전송할 공지 사항 인덱스 설정
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CANoticeManager::OnDoSecondlyWorkCANoticeManager(ATUM_DATE_TIME *i_pDateTime)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-15 ~ 2009-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CANoticeManager::OnDoSecondlyWorkCANoticeManager(ATUM_DATE_TIME *i_pDateTime)
{
	if(FALSE == this->IsValidAdminAutoNoticeNoLock())
	{
		return;
	}

	SANoticeString ANoticeStr;
	util::zero(&ANoticeStr, sizeof(ANoticeStr));
	if(FALSE == this->GetNoticeStringforSending(&ANoticeStr, i_pDateTime))
	{
		return;
	}

	if(0 >= strlen(ANoticeStr.NoticeString))
	{
		// 2009-01-19 by cmkwon, 공지 내용이 없으면 전송할 메시지가 없다.
		//g_pIMGlobal->WriteSystemLogEX(TRUE, "[ERROR] CANoticeManager::OnDoSecondlyWorkCANoticeManager# AdminAutoNoticeString is null !! %d \r\n", ANoticeStr.NoticeStringIndex);
		return;
	}

	// 시스템 로그 저장
	g_pIMGlobal->WriteSystemLogEX(TRUE, "[Notify] AdminAutoNotice NoticeStringIndex(%d) NoticeString(%s) \r\n", ANoticeStr.NoticeStringIndex, ANoticeStr.NoticeString);

	// 공지 사항을 모든 클라이언트에 전송.
	INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_ALL, T_IC_CHAT_ALL, pChatAll, SendBuf);
	util::strncpy(pChatAll->FromCharacterName, "GameMaster", SIZE_MAX_CHARACTER_NAME);
	pChatAll->MessageLength = min(SIZE_MAX_CHAT_MESSAGE, strlen(ANoticeStr.NoticeString) + 1);
	util::strncpy((char*)(SendBuf + MSG_SIZE(MSG_IC_CHAT_ALL)), ANoticeStr.NoticeString, SIZE_MAX_CHAT_MESSAGE);
	m_pIMIOCP6->SendChatMsgToAllClients(CHAT_TYPE_UNCONDITIONAL, SendBuf, MSG_SIZE(MSG_IC_CHAT_ALL) + pChatAll->MessageLength);

}