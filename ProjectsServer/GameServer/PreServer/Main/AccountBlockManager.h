#pragma once

typedef mt_map<string, SBLOCKED_ACCOUNT_INFO>			mtmapBLOCKED_ACCOUNT_INFO;

class CAtumPreDBManager;
class CPreIOCPSocket;
class CAccountBlockManager  
{
public:
	explicit CAccountBlockManager(CAtumPreDBManager *i_pDBManager) : m_pPreDBManager { i_pDBManager } { }

	void ResetAccountBlockManager();

	// 2016-01-09 Future, improved Account block loading
	BOOL LoadBlockedAccountListFromDB();
	BOOL AddBlockedAccountList(const SBLOCKED_ACCOUNT_INFO *i_pBlockedInfo);
	BOOL DeleteBlockedAccountList(const char *i_szAccountName);			// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - DeleteBlockedAccountList() 수정
	
	BOOL Check(SBLOCKED_ACCOUNT_INFO *o_pBlockedInfo, const char *i_szAccName, const char* pMACAddress = nullptr, CPreIOCPSocket *i_pNotifySock = nullptr);	// 2016-01-09 Future, MAC Blocking

	// 2016-01-09 Future, MAC Blocking
	BOOL LoadBlockedMACListFromDB();
	BOOL AddBlockedMACAddress(const char* pMACAddress);
	BOOL RemoveBlockedMACAddress(const char* pMACAddress);

protected:
	mtmapBLOCKED_ACCOUNT_INFO	m_mtmapBlockedAccountList;
	mtsetString					m_strsetBlockedMACs;	// 2016-01-09 Future, MAC Blocking
	CAtumPreDBManager			*m_pPreDBManager;
};