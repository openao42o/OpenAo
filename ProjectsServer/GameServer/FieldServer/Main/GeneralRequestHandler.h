#ifndef _GENERAL_REQUEST_HANDLER_H_
#define _GENERAL_REQUEST_HANDLER_H_

#define FC_REQUEST_TIMEOUT_INTERVAL		30000

class CFieldIOCPSocket;

class CGeneralRequestHandler  
{
public:
	CGeneralRequestHandler();

	void InitGeneralRequestHandler(CFieldIOCPSocket *i_pFieldIOCPSocket);

	void Reset();
	void SendCancelRequsetAndReset(BOOL i_bSendToPeer = TRUE);	// Reset() 이전에 호출되어야 함

	BOOL IsRequestableAndResetIfTimeOut(TimeUnit_t i_TimeOut = FC_REQUEST_TIMEOUT_INTERVAL);
	BYTE GetWaitingRequestType();	// returns, REQUEST_TYPE_XXX
	void SetPeerRequest(BYTE i_RequestType, ClientIndex_t i_PeerRequesterClientIndex);
	ClientIndex_t GetPeerClientIndex();

private:
	BYTE				m_RequestType;				// REQUEST_TYPE_XXX
	ClientIndex_t		m_PeerRequesterClientIndex;	// 상대방 ClientIndex
	TimeUnit_t			m_RequestTimeStamp;			// 신청 시간

	CFieldIOCPSocket	*m_pFieldIOCPSocket;
};

#endif // _GENERAL_REQUEST_HANDLER_H_
