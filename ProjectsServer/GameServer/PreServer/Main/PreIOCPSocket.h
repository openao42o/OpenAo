#pragma once

#include "PreIOCP.h"
#include "AtumProtocol.h"
#include "PreGlobal.h"

class CPreIOCPSocket : public CIOCPSocket
{
	// private members

	friend CPreIOCP;

	static CPreIOCP		*ms_pPreIOCP;

	CP_State			m_CPClientState;
	char				m_szConnectedServerGroupName[SIZE_MAX_SERVER_NAME];
	char				m_szAdminAccountName[SIZE_MAX_ACCOUNT_NAME];			// 2007-06-20 by cmkwon, 계정 블럭정보 시스템 로그에 추가

public:

	CPreIOCPSocket() : m_CPClientState { CP_NOTCONNECTED }, m_szConnectedServerGroupName { }, m_szAdminAccountName { } { }
	~CPreIOCPSocket() = default;

	BOOL OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo = nullptr) override;
	void OnConnect() override;
	void OnClose(int reason = 0) override;

	//template<MessageType_t> static ProcessResult HandlerT1(CPreIOCPSocket*, const char*, int, int&);
	template<T0> static ProcessResult HandlerT0(CPreIOCPSocket*, MessageType_t, const char*, int, int&);

	template<MessageType_t _msgtype> struct HandlerT1
	{
		using info = protocol::info<_msgtype>;

		constexpr static auto msgtype = _msgtype;

		//constexpr static auto t0 = info::t0;
		//constexpr static auto t1 = info::t1;

		using struct_t = typename info::struct_t;

		static const struct_t* GetMessageData(const char* data, int length, int& offset)
		{
			static_assert(info::msg_size > 0, "This message type does not have any data structure associated with it");

			if (length - offset < sizeof(struct_t)) return nullptr;

			data += offset;

			offset += sizeof(struct_t);

			return reinterpret_cast<const struct_t*>(data);
		}

		static ProcessResult handler(CPreIOCPSocket*, const char*, int, int&);
	};

	// Process DB Query Result
	BOOL ResPreLogin(MSG_PC_CONNECT_LOGIN *pRecvMsgLogin, CAccountInfo *i_pAccInfo, Err_t nErr);

	void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, const char* errMsg = nullptr, BOOL bCloseConnection = FALSE) override;

	bool IsWhitelisted() const { return  PreGlobal::CheckAllowedIP(GetPeerIP()); }
};

// DevX declares what messages are used
namespace proclist
{
	template<> struct ProcessesT0<CPreIOCPSocket> { using list = metalist<T0, T0::PC_DEFAULT_UPDATE, T0::PC_CONNECT, T0::FP_CONNECT, T0::IP_CONNECT, T0::PM_CONNECT, T0::FP_EVENT, T0::FP_CASH, T0::PM_MONITOR, T0::PA_ADMIN>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::PC_DEFAULT_UPDATE> { using T1 = T1<T0::PC_DEFAULT_UPDATE>; using list = metalist<T1, T1::PC_DEFAULT_UPDATE_LAUNCHER_VERSION, T1::PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::PC_CONNECT> { using T1 = T1<T0::PC_CONNECT>; using list = metalist<T1, T1::PC_CONNECT_ALIVE, T1::PC_CONNECT_VERSION, T1::PC_CONNECT_LOGIN, T1::PC_CONNECT_SINGLE_FILE_VERSION_CHECK, T1::PC_CONNECT_GET_SERVER_GROUP_LIST, T1::PC_CONNECT_GET_GAME_SERVER_GROUP_LIST, T1::PC_CONNECT_NETWORK_CHECK, T1::PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::FP_CONNECT> { using T1 = T1<T0::FP_CONNECT>; using list = metalist<T1, T1::FP_CONNECT_ALIVE, T1::FP_CONNECT_AUTH_USER, T1::FP_CONNECT_FIELD_CONNECT, T1::FP_CONNECT_NOTIFY_CLOSE, T1::FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE, T1::FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::IP_CONNECT> { using T1 = T1<T0::IP_CONNECT>; using list = metalist<T1, T1::IP_CONNECT_ALIVE, T1::IP_CONNECT_IM_CONNECT, T1::IP_GET_SERVER_GROUP_INFO_ACK>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::PM_CONNECT> { using T1 = T1<T0::PM_CONNECT>; using list = metalist<T1, T1::PM_CONNECT, T1::PM_CONNECT_ALIVE>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::FP_EVENT> { using T1 = T1<T0::FP_EVENT>; using list = metalist<T1, T1::FP_EVENT_ENTER_SELECT_SCREEN, T1::FP_EVENT_GAME_STARTED, T1::FP_EVENT_MAP_CHANGED>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::FP_CASH> { using T1 = T1<T0::FP_CASH>; using list = metalist<T1, T1::FP_CASH_CHANGE_CHARACTERNAME>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::FP_ADMIN> { using T1 = T1<T0::FP_ADMIN>; using list = metalist<T1, T1::FP_ADMIN_BLOCKACCOUNT, T1::FP_ADMIN_UNBLOCKACCOUNT>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::PM_MONITOR> { using T1 = T1<T0::PM_MONITOR>; using list = metalist<T1, T1::PM_GET_NUM_CLIENTS, T1::PM_SHUTDOWN, T1::PM_PING, T1::PM_RELOAD_VERSION_INFO, T1::PM_CHANGE_BANDWIDTH_FLAG, T1::PM_SET_MSG_PRINT_LEVEL, T1::PM_PAUSE_SERVICE, T1::PM_START_SERVICE, T1::PM_GET_SERVER_GROUP_INFO, T1::PM_SET_LIMIT_GROUP_USER_COUNTS, T1::PM_RELOAD_BLOCKED_ACCOUNTS, T1::PM_CHANGE_ENABLE_SERVER_GROUP, T1::PM_PREPARE_SHUTDOWN, T1::PM_RELOAD_VERSION_INFO_HEADER, T1::PM_RELOAD_VERSION_INFO_LIST, T1::PM_RELOAD_VERSION_INFO_DONE>; };
	template<> struct ProcessesT1<CPreIOCPSocket, T0::PA_ADMIN> { using T1 = T1<T0::PA_ADMIN>; using list = metalist<T1, T1::PA_ADMIN_CONNECT, T1::PA_ADMIN_GET_ACCOUNT_INFO, T1::PA_ADMIN_BLOCK_ACCOUNT, T1::PA_ADMIN_UNBLOCK_ACCOUNT, T1::PA_ADMIN_GET_SERVER_INFO, T1::PA_ADMIN_GET_ACCOUNT_LIST, T1::PA_ADMIN_RELOAD_HAPPYEV, T1::PA_ADMIN_RELOAD_ITEMEV, T1::PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME, T1::PA_ADMIN_PETITION_SET_PERIOD, T1::PA_ADMIN_SET_DBSERVER_GROUP, T1::PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM, T1::PA_ADMIN_RELOAD_WORLDRANKING, T1::PA_ADMIN_RELOAD_INFLUENCERATE, T1::PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE, T1::PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE, T1::PA_ADMIN_STRATRGYPOINT_INFO_CHANGE>; };
}