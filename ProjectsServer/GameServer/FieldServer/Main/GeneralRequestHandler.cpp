#include "stdafx.h"
#include "AtumProtocol.h"
#include "GeneralRequestHandler.h"
#include "FieldIOCPSocket.h"

CGeneralRequestHandler::CGeneralRequestHandler()
{
	m_RequestType = REQUEST_TYPE_NULL;
	m_PeerRequesterClientIndex = 0;
	m_RequestTimeStamp = 0;

	Reset();
}

void CGeneralRequestHandler::InitGeneralRequestHandler(CFieldIOCPSocket *i_pFieldIOCPSocket)
{
	m_pFieldIOCPSocket = i_pFieldIOCPSocket;
	Reset();
}

void CGeneralRequestHandler::Reset()
{
	m_RequestType = REQUEST_TYPE_NULL;
	m_PeerRequesterClientIndex = 0;
	m_RequestTimeStamp = 0;
}

void CGeneralRequestHandler::SendCancelRequsetAndReset(BOOL i_bSendToPeer /* = TRUE */)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_REQUEST_CANCEL_REQUEST, T_FC_REQUEST_CANCEL_REQUEST, pMsgCancel, pMsgCancelBuf);

	pMsgCancel->PeerClientIndex = m_PeerRequesterClientIndex;
	pMsgCancel->RequestType = m_RequestType;
	m_pFieldIOCPSocket->SendAddData(pMsgCancelBuf, MSG_SIZE(MSG_FC_REQUEST_CANCEL_REQUEST));

	if (i_bSendToPeer)
	{
		pMsgCancel->PeerClientIndex = m_pFieldIOCPSocket->m_character.ClientIndex;
		pMsgCancel->RequestType = m_RequestType;
		CFieldIOCPSocket *pPeerSocket
			= m_pFieldIOCPSocket->ms_pFieldIOCP->GetFieldIOCPSocket(m_PeerRequesterClientIndex);
		if (pPeerSocket != NULL && pPeerSocket->IsUsing())
		{
			pPeerSocket->SendAddData(pMsgCancelBuf, MSG_SIZE(MSG_FC_REQUEST_CANCEL_REQUEST));
		}
		pPeerSocket->m_RequestHandler.Reset();
	}

	Reset();
}

BOOL CGeneralRequestHandler::IsRequestableAndResetIfTimeOut(TimeUnit_t i_TimeOut /* = FC_REQUEST_TIMEOUT_INTERVAL */)
{
	if (m_RequestTimeStamp != 0)
	{
		TimeUnit_t CurrentTimeStamp = timeGetTime();

		if (CurrentTimeStamp - m_RequestTimeStamp > i_TimeOut)
		{
			SendCancelRequsetAndReset();
			return FALSE;
		}
	}

	return (m_RequestType == REQUEST_TYPE_NULL);
}

BYTE CGeneralRequestHandler::GetWaitingRequestType()
{
	return m_RequestType;
}

void CGeneralRequestHandler::SetPeerRequest(BYTE i_RequestType, ClientIndex_t i_PeerRequesterClientIndex)
{
	m_RequestType = i_RequestType;
	m_PeerRequesterClientIndex = i_PeerRequesterClientIndex;

	m_RequestTimeStamp = timeGetTime();
}

ClientIndex_t CGeneralRequestHandler::GetPeerClientIndex()
{
	return m_PeerRequesterClientIndex;
}