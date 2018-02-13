// NPCIOCP.h: interface for the CNPCIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
#define AFX_NPCIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "AtumDBManager.h"
#include "NPCIOCPSocket.h"
#include "MapWorkspace.h"
#include "NPCMapChannel.h"		// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - 

// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
#include "WayPointManager.h"

class CWayPointManager;
// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

class CNPCTickManager;
class CNPCIOCP : public CIOCP
{
	friend class CNPCIOCPSocket;

public:
	CNPCIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CNPCIOCP();
	ez_map<INT, ITEM*> * GetPtrMapItemInfo(void){		return &m_mapItemInfo;}

	inline CNPCMapWorkspace* GetMapWorkspace();
public:
	inline CNPCIOCPSocket* GetNPCIOCPSocket(int idx);
	BOOL OpenUDPPortForOtherServer(void);

	void LoadItemInfo();
	ITEM* GetItemInfo(int ItemNum);
	MONSTER_INFO *GetMonsterInfo(int i_nMonsterUnitKind);
	void SetSummonJacoMonster(BYTE i_byBelligerence, BOOL i_bIsSummon);
	BOOL CheckSummonJacoMonster(BYTE i_byBelligerence);

	// Monitor 관련 함수
	CNPCMapChannel * GetNPCMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX MapChannIndex);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
	mtvectSEVENT_MONSTER	m_EventMonsterList;				// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
	void SetEventMonsterList(mtvectSEVENT_MONSTER *i_pEvMonList);
	BOOL AddCheckSummonEventMonster(SEVENT_MONSTER *i_pEventMonster, MAP_CHANNEL_INDEX i_mapChannIdx, MONSTER_INFO *i_pMonInfo);
	BOOL GetSummonEventMonsterListAfterDead(mtvectSSUMMON_EVENT_MONSTER *o_pSummonEvMonList, MAP_CHANNEL_INDEX i_mapChannIdx, MONSTER_INFO *i_pMonInfo);

protected:
	CNPCMapWorkspace		*m_pNPCMapWorkspace;

	// 2006-04-18 by cmkwon
	bool					m_bSummonVCNJacoMonster;		// 2006-04-18 by cmkwon
	bool					m_bSummonANIJacoMonster;		// 2006-04-18 by cmkwon


public:
	ez_map<int, MONSTER_INFO>	m_mapMonsterParameter;
	ez_map<INT, ITEM*>		m_mapItemInfo;

	vectorCLIENT_INFO			m_vectorClientInfo;			// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - CNPCMapChannel 에 있던 것을 이곳으로 이동함.
	CLIENT_INFO* GetClientInfoO(int i_Characteridx);		// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - 

	BOOL IsExistClient(MAP_CHANNEL_INDEX i_mapChannIdx);	// 2008-12-03 by cmkwon, NPCServer 클라이언트 없는 맵채널 처리 수정 - 

	ez_map<INT, vectHPAction>	m_mapHPAction;					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - HPAction 로딩
	
	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	CWayPointManager		*m_pWayPointManager;
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	
public:
	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-08 by cmkwon, 서버에서 맵로딩시 AlivePacket 전송 추가 - 
	virtual void SendAlivePacket2OtherServers(void);
};

CNPCIOCPSocket* CNPCIOCP::GetNPCIOCPSocket(int idx)
{
	return (CNPCIOCPSocket*)GetIOCPSocket(idx);
}

CNPCMapWorkspace* CNPCIOCP::GetMapWorkspace()
{
	return m_pNPCMapWorkspace;
}
#endif // !defined(AFX_NPCIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
