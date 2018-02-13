// WinPacket.cpp: implementation of the Packet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinPacket.h"

const char* CSendPacket::m_pXOR_ENCODE_BYTES	= XOR_ENCODE_BYTES;
const char* CRecvPacket::m_pXOR_ENCODE_BYTES	= XOR_ENCODE_BYTES;
/////////////////////////////////////////////////////////////////////////////
// CRecvPacket class member function
CRecvPacket::CRecvPacket()
{
	memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);
	memset(m_pPacketHeader, 0x00, SIZE_PACKET_HEADER);
	m_nPacketLength			= 0;
	m_nCurrentLength		= 0;
	m_nRemainedHeaderSize	= 0;
	memset(&m_DecodingInfo, 0x00, sizeof(DECODING_INFO));
}

CRecvPacket::~CRecvPacket()
{	
}



#ifdef _ATUM_SERVER
#include "VMemPool.h"
void * CSendPacket::operator new(size_t size)
{
	return CVMemPool::vmObjectNew(size);
}

void CSendPacket::operator delete(void* p)
{
	CVMemPool::vmObjectDelete(p, sizeof(CSendPacket));
}

void * CRecvPacket::operator new(size_t size)
{
	return CVMemPool::vmObjectNew(size);
}

void CRecvPacket::operator delete(void* p)
{
	CVMemPool::vmObjectDelete(p, sizeof(CRecvPacket));
}
#endif