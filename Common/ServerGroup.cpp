#include "stdafx.h"
#include "servergroup.h"
#include "IOCPSocket.h"

CServerGroup::CServerGroup()
{
	InitServerGroup();
}

CServerGroup::~CServerGroup()
{
	InitServerGroup();
}

void CServerGroup::InitServerGroup()
{
	memset(m_ServerGroupName, 0x00, SIZE_MAX_SERVER_NAME);
	m_bEnableServerGroup			= TRUE;
	m_LimitGroupUserCounts			= 0;
	m_bForbidViewServer				= FALSE;		// 2007-12-22 by dhjin, 아레나 통합 - 런쳐 리스트에서 보이면 안되는 서버 체크, TRUE = 보이지 않는다.
	
	m_AccumulatedGroupUserCounts	= 0;
	m_MaxGroupUserCounts			= 0;

	m_bLockCreateCharacterForNewAccount	= FALSE;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 

	// reset m_IMServerInfo
	m_IMServerInfo.Reset();

	// init m_setLoginedAccount
	m_setLoginedAccount.clearLock();

	m_FieldServerInfo.Reset();
}

void CServerGroup::ResetServerGroup(BOOL i_bExcludeIMServer/*=FALSE*/)
{
	m_AccumulatedGroupUserCounts	= 0;
	m_MaxGroupUserCounts			= 0;

	// init m_setLoginedAccount
	m_setLoginedAccount.clearLock();

	m_FieldServerInfo.Reset();

	if(FALSE == i_bExcludeIMServer)
	{
		// reset m_IMServerInfo
		m_IMServerInfo.Reset();
	}
}

void CServerGroup::SendMessageToFieldServer(BYTE *i_pBuff, int i_nSize)
{
	if (m_FieldServerInfo.IsActive && m_FieldServerInfo.pSocket && m_FieldServerInfo.pSocket->IsUsing())
		
		m_FieldServerInfo.pSocket->SendAddData(i_pBuff, i_nSize);
}

void CServerGroup::SendMsgToIMServer(BYTE *i_pBuff, int i_nSize)
{
	if (m_IMServerInfo.IsActive && m_IMServerInfo.pSocket && m_IMServerInfo.pSocket->IsUsing())
		
		m_IMServerInfo.pSocket->SendAddData(i_pBuff, i_nSize);
}