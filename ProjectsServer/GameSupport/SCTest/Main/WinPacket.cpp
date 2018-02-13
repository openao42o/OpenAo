// WinPacket.cpp: implementation of the Packet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinPacket.h"

char* CSendPacket::m_pXOR_ENCODE_BYTES	= XOR_ENCODE_BYTES;
char* CRecvPacket::m_pXOR_ENCODE_BYTES	= XOR_ENCODE_BYTES;
/////////////////////////////////////////////////////////////////////////////
// CRecvPacket class member function
CRecvPacket::CRecvPacket()
{
	memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);
	memset(m_pPacketHeader, 0x00, SIZE_PACKET_HEADER);
	m_nPacketLength			= 0;
	m_nCurrentLength		= 0;
	m_nRemainedHeaderSize	= 0;
	RESET_DECOING_INFO(m_DecodingInfo);
}

CRecvPacket::~CRecvPacket()
{	
}

