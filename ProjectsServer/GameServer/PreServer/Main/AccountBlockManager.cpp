// AccountBlockManager.cpp: implementation of the CAccountBlockManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccountBlockManager.h"
#include "PreIOCP.h"
#include "AtumPreDBManager.h"

void CAccountBlockManager::ResetAccountBlockManager()
{
	m_mtmapBlockedAccountList.clearLock();
	m_strsetBlockedMACs.clearLock();	// 2016-01-09 Future, MAC Blocking
}

BOOL CAccountBlockManager::LoadBlockedAccountListFromDB()
{
	return m_pPreDBManager->ExecuteQuery(QT_LoadBlockedAccounts, nullptr, this);
}

BOOL CAccountBlockManager::AddBlockedAccountList(const SBLOCKED_ACCOUNT_INFO *i_pBlockedInfo)
{
	if(strncmp(i_pBlockedInfo->szBlockedAccountName, "", SIZE_MAX_ACCOUNT_NAME) == 0) return FALSE;

	mt_auto_lock mtA(&m_mtmapBlockedAccountList);
	auto pBlocked = m_mtmapBlockedAccountList.findLock_Ptr(i_pBlockedInfo->szBlockedAccountName);
	
	if (pBlocked)
	{
		*pBlocked = *i_pBlockedInfo;
		return TRUE;
	}

	return m_mtmapBlockedAccountList.insertNoLock(i_pBlockedInfo->szBlockedAccountName, *i_pBlockedInfo);	
}

// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - CAccountBlockManager::DeleteBlockedAccountList() 수정
BOOL CAccountBlockManager::DeleteBlockedAccountList(const char *i_szAccountName)
{
	return m_mtmapBlockedAccountList.deleteLock(i_szAccountName);
}

BOOL CAccountBlockManager::Check(SBLOCKED_ACCOUNT_INFO *o_pBlockedInfo
																  , const char *i_szAccName
																  , const char* pMACAddress/*= NULL*/ // 2016-01-09 Future, MAC Blocking
																  , CPreIOCPSocket *i_pNotifySock/*=NULL*/)
{
	// Check Account Block
	mt_auto_lock mtaAccount(&m_mtmapBlockedAccountList);	

	auto pBlocked = m_mtmapBlockedAccountList.findLock_Ptr(i_szAccName);

	if (pBlocked)
	{
		// Unblock user if his ban expired
		if (pBlocked->atimeEndTime <= ATUM_DATE_TIME { true })
		{
			// Delete from List
			m_mtmapBlockedAccountList.deleteLock(i_szAccName);
			mtaAccount.auto_unlock_cancel();

			// Delete from DB
			m_pPreDBManager->MakeAndEnqueueQuery(QT_UnblockUser, (CIOCPSocket*)(i_pNotifySock), pBlocked);		// 2008-04-04 by cmkwon, 계정 Block/Unblock 버그 수정

			// Account is not blocked presently
			return FALSE;
		}

		// Give blocked info and return
		*o_pBlockedInfo = *pBlocked;
		return TRUE;
	}

	// Check MAC Block
	mt_auto_lock mtaMac(&m_strsetBlockedMACs);
	if (pMACAddress && m_strsetBlockedMACs.find(pMACAddress) != m_strsetBlockedMACs.end())
	{
		// Is MAC Blocked
		o_pBlockedInfo->IsMACBlocked = TRUE;
		return TRUE;
	}

	// Account is not blocked presently
	return FALSE;
}

BOOL CAccountBlockManager::LoadBlockedMACListFromDB()
{
	return m_pPreDBManager->ExecuteQuery(QT_LoadBlockedMACs, NULL, &m_strsetBlockedMACs);
}

BOOL CAccountBlockManager::AddBlockedMACAddress(const char* pMACAddress)
{
	if (!pMACAddress) return FALSE;

	if (m_strsetBlockedMACs.insertLock(pMACAddress))
	{
		// Insert to DB
		QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS* pQuery = new QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS;
		strcpy_s(pQuery->MACAddress, SIZE_MAX_MAC_ADDRESS, pMACAddress);
		m_pPreDBManager->MakeAndEnqueueQuery(QT_BlockMAC, NULL, pQuery);

		return TRUE;
	}
	
	return FALSE;
}

BOOL CAccountBlockManager::RemoveBlockedMACAddress(const char* pMACAddress)
{
	if (!pMACAddress)
		return FALSE;

	// Delete from DB
	QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS* pQuery = new QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS;
	strcpy_s(pQuery->MACAddress, SIZE_MAX_MAC_ADDRESS, pMACAddress);
	m_pPreDBManager->MakeAndEnqueueQuery(QT_UnblockMAC, NULL, pQuery);

	// Return deletion from List
	return m_strsetBlockedMACs.deleteLock(pMACAddress);
}
