// NPCIOCPSocket.h: interface for the CNPCIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
#define AFX_NPCIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "NPCMapWorkspace.h"
#include <thread>
#include <mutex>
#include <condition_variable>

class CNPCMapProject;
class CNPCIOCPSocket : public CIOCPSocket
{
	friend class CNPCIOCP;
public:
	CNPCIOCPSocket();
	virtual ~CNPCIOCPSocket();

#ifdef _DEBUG
	DWORD		m_dwDebugWorkerTime;
#endif

protected:
	CNPCMapProject			*m_pNPCMapProject;						// Map Project
	//HANDLE				m_hThreadMapWorker;						// Map Thread 핸들

	thread*					m_threadMapWorker;
	condition_variable		m_terminateCondition;
	mutex					m_mutexMapWorker;

	BOOL					m_bThreadEndFlagMapWorker;				// Map Thread 종료 플래그

	static CNPCIOCP			*ms_pNPCIOCP;


public:
	// 가상 함수
	BOOL OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP="", int nPeerPort=0, SThreadInfo *i_pThreadInfo=NULL) override;
	void OnConnect(void) override;
	void OnClose(int reason = 0) override;
	BOOL OnError(int errCode) override;

	// 멤버 함수
	BOOL InitializeNPCMap(CMapProject *pmapProject, ez_map<int, MONSTER_INFO> *mapMonsterParam);	// NPCIOCPSocket과 MapProject를 연결한다
	BOOL CreateThreadMapWorker(void);

	DWORD MapWorker();
	void MapWorkerEnd();

private:
	ProcessResult Process_FN_MAPPROJECT_START_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CLIENT_GAMESTART_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CLIENT_GAMEEND_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MOVE_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_GET_CHARACTER_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_SET_ATTACK_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_DROP_FIXER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CHARACTER_CHANGE_MONSTER_HP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CHARACTER_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CHARACTER_CHANGE_STEALTHSTATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CHARACTER_CHANGE_CHARACTER_MODE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CHARACTER_CHANGE_INFLUENCE_TYPE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CHARACTER_CHANGE_INVISIBLE(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FN_MONSTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_ADMIN_SUMMON_MONSTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_SKILL_USE_SKILL_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_SKILL_END_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_STRATEGYPOINT_INIT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_STRATEGYPOINT_SUMMON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_OUTPOST_INIT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_OUTPOST_SUMMON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_TELEPORT_SUMMON(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FN_CONNECT_SET_CHANNEL_STATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CONNECT_INCREASE_CHANNEL(const char* pPacket, int nLength, int &nBytesUsed);
	
	ProcessResult Process_FN_CITYWAR_START_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CITYWAR_END_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_CITYWAR_CHANGE_OCCUPY_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_KEYMONSTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Key몬스터 생성
	ProcessResult Process_FN_EVENT_NEW_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 

	// Monitor
	ProcessResult Process_NM_PRINTMAP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_NM_SAVE_MAPBLOCK_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_NM_GET_MAP_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_NM_SET_LIMIT_MONSTER_COUNT(const char* pPacket, int nLength, int &nBytesUsed);
};

#endif // !defined(AFX_NPCIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
