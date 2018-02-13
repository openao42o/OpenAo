// FieldWebIOCPSocket.h: interface for the CFieldWebIOCPSocket class.
// 2013-03-13 by hskim, 웹 캐시 상점
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
#define AFX_WEBIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FieldWebIOCP.h"
#include "AtumProtocol.h"
#include "XmlRpc.h"

#define SIZE_MAX_XMLRPC_METHODNAME							512					// 메소드 이름
#define SIZE_MAX_XMLRPC_PARAMTYPE							100					// 파라미터 타입 수

#define MIN_WEB_GAME_INSERT_COUNTABLEITEM_COUNT				1					// 아이템 지급 : 카운터블 최소
#define MAX_WEB_GAME_INSERT_COUNTABLEITEM_COUNT				20000000			// 아이템 지급 : 카운터블 최대

#define MIN_WEB_GAME_INSERT_NONCOUNTABLEITEM_COUNT			1					// 아이템 지급 : 넌카운터블 최소
#define MAX_WEB_GAME_INSERT_NONCOUNTABLEITEM_COUNT			10					// 아이템 지급 : 넌카운터블 최대

#define MIN_WEB_GAME_DELETE_COUNTABLEITEM_COUNT				1					// 아이템 회수 : 카운터블 최소
#define MAX_WEB_GAME_DELETE_COUNTABLEITEM_COUNT				20000000			// 아이템 회수 : 카운터블 최대

#define MIN_WEB_GAME_DELETE_NONCOUNTABLEITEM_COUNT			1					// 아이템 회수 : 넌카운터블 최소
#define MAX_WEB_GAME_DELETE_NONCOUNTABLEITEM_COUNT			10000				// 아이템 회수 : 넌카운터블 최대

#define MIN_WEB_BILLING_BUY_COUNT							1					// 아이템 구매 : 최소
#define MAX_WEB_BILLING_BUY_COUNT							10					// 아이템 구매 : 최대


typedef struct
{
	char szMethodName[SIZE_MAX_XMLRPC_METHODNAME];
	int (CFieldWebIOCPSocket::*pFunction)(XmlRpc::XmlRpcValue &rParamsValue);
	int ParamCount;
	int ParamType[SIZE_MAX_XMLRPC_PARAMTYPE];

} XMLRPC_METHOD_INFO;

class CFieldIOCP;

class CFieldWebIOCPSocket : public CIOCPSocket
{
	friend class CFieldWebIOCP;
	
public:
	CFieldWebIOCPSocket();
	~CFieldWebIOCPSocket();

public:
	BOOL OnRecvdPacketFieldWebServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	void OnConnect(void);
	void OnClose(int reason = 0);

	// XMLRPC 프로토콜 관련 메소드
	int Process_XMLRPC_METHOD_GET_SERVER_STATUS(XmlRpc::XmlRpcValue &rParamsValue);

	// Game 관련 메소드
	int Process_XMLRPC_METHOD_GAME_ITEM_INSERT(XmlRpc::XmlRpcValue &rParamsValue);
	int Process_XMLRPC_METHOD_GAME_ITEM_DELETE(XmlRpc::XmlRpcValue &rParamsValue);
	int Process_XMLRPC_METHOD_BILLING_BUY_ITEM(XmlRpc::XmlRpcValue &rParamsValue);
	int Process_XMLRPC_METHOD_BILLING_GET_CASH(XmlRpc::XmlRpcValue &rParamsValue);

protected:
	CFieldIOCP *GetFieldIOCP()				{ return ms_pFieldWebIOCP->m_pFieldIOCP; }
	CFieldWebIOCP *GetFieldWebIOCP()		{ return ms_pFieldWebIOCP; }

private:
	static CFieldWebIOCP		*ms_pFieldWebIOCP;

	CXmlRpc		m_XmlRpc;
};

#endif // !defined(AFX_WEBIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
