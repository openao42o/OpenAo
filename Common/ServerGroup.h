#pragma once

#include "SocketHeader.h"

class CIOCPSocket;

struct ServerInfo
{
	ServerInfo() { ServerInfo::Reset(); }
	virtual ~ServerInfo() { ServerInfo::Reset(); }

	virtual void Reset()
	{
		ServerType = ST_INVALID_TYPE;
		memset(&serverID, 0x00, sizeof(SERVER_ID));
		IsActive = FALSE;
		pSocket = NULL;
	}

	ENServerType	ServerType;
	SERVER_ID		serverID;
	BOOL			IsActive;
	CIOCPSocket		*pSocket;
};

struct FieldServerInfo : public ServerInfo
{
public:
	FieldServerInfo() 
	{
		MapIndexVector.reserve(50);
		FieldServerInfo::Reset();
	}
	~FieldServerInfo() override { FieldServerInfo::Reset(); }

	void Reset() override
	{
		MapIndexVector.clear();		
		AccumulatedFieldUserCounts = 0;
		CurrentFieldUserCounts = 0;
		MaxFieldUserCounts = 0;
		AccumulatedFieldMonsterCounts = 0;
		CurrentFieldMonsterCounts = 0;

		ServerInfo::Reset();
	}

	BOOL IsExistMapIndex(MapIndex_t i_mapIdx)
	{
		vector<MapIndex_t>::iterator itr(MapIndexVector.begin());
		for(; itr != MapIndexVector.end(); itr++)
		{
			if(*itr == i_mapIdx)
			{
				return TRUE;
			}
		}
		return FALSE;
	}

public:
	vector<MapIndex_t>	MapIndexVector;
	UINT				AccumulatedFieldUserCounts;
	UINT				CurrentFieldUserCounts;
	UINT				MaxFieldUserCounts;
	UINT				AccumulatedFieldMonsterCounts;
	UINT				CurrentFieldMonsterCounts;
};

class CServerGroup
{
public:
	CServerGroup();
	~CServerGroup();

	CServerGroup(const char* GroupName, unsigned int LimitGroupUserCounts, bool EnableServerGroup) :
		m_bEnableServerGroup { EnableServerGroup }, m_bForbidViewServer { 0 },
		m_AccumulatedGroupUserCounts { 0 }, m_MaxGroupUserCounts { 0 },
		m_LimitGroupUserCounts { LimitGroupUserCounts },
		m_bLockCreateCharacterForNewAccount { 0 }
	{
		strncpy(m_ServerGroupName, GroupName, SIZE_MAX_SERVER_NAME);
	}

	void InitServerGroup();
	void ResetServerGroup(BOOL i_bExcludeIMServer = FALSE);

	auto GetUserCount() const { return m_setLoginedAccount.size(); }

	void SendMessageToFieldServer(BYTE *i_pBuff, int i_nSize);
	void SendMsgToIMServer(BYTE *i_pBuff, int i_nSize);

	template<template<MessageType_t> typename data_t, MessageType_t msgtype>
	void SendMessageToFieldServer(data_t<msgtype>& data) { SendMessageToFieldServer(PBYTE(&data), data.size()); }
	template<template<MessageType_t> typename data_t, MessageType_t msgtype>
	void SendMsgToIMServer(data_t<msgtype>& data) { SendMsgToIMServer(PBYTE(&data), data.size()); }

	void SendMessageToFieldServer(MessageType_t msgtype) { SendMessageToFieldServer(PBYTE(&msgtype), sizeof(msgtype)); }
	void SendMsgToIMServer(MessageType_t msgtype) { SendMsgToIMServer(PBYTE(&msgtype), sizeof(msgtype)); }

	char							m_ServerGroupName[SIZE_MAX_SERVER_NAME];
	BOOL							m_bEnableServerGroup;
	BOOL							m_bForbidViewServer;		// 2007-12-22 by dhjin, 아레나 통합 - 런쳐 리스트에서 보이면 안되는 서버 체크, TRUE = 보이지 않는다.

	// statistics
	UINT							m_AccumulatedGroupUserCounts;
	UINT							m_MaxGroupUserCounts;
	UINT							m_LimitGroupUserCounts;
	//int							m_nMGameServerID;
	BOOL							m_bLockCreateCharacterForNewAccount;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - from FieldServer

	// server management
	ServerInfo						m_IMServerInfo;						// IMServer
	FieldServerInfo					m_FieldServerInfo;					// FieldServer
	mt_set<string>					m_setLoginedAccount;				// 접속중인 account 리스트
};
