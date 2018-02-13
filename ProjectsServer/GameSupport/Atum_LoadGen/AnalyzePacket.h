// AnalyzePacket.h: interface for the CAnalyzePacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYZEPACKET_H__75E6FCDC_51B6_4EA0_88F5_7D799788EE11__INCLUDED_)
#define AFX_ANALYZEPACKET_H__75E6FCDC_51B6_4EA0_88F5_7D799788EE11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SystemLogManager.h"

/////////////////////////////////////////////////////////////////////////////
// CAnalyzePacketDlg dialog

struct SAL_PACKET_HEADER
{
	unsigned short int		usPacketSize;
	BYTE					byChunk[2];
};

// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
struct SAL_PACKET_CLASS
{
	MessageType_t	MsgTy;
	int				MsgTyCount;
	int				MsgTyByte;
};
typedef vector<SAL_PACKET_CLASS>		vectSAL_PACKET_CLASS;

struct SAL_PACKET
{
	USHORT	PacketSize;
	BYTE	PacketBuf[SIZE_MAX_PACKET];
	DWORD	ProtocolCnt;
	DWORD	TotalProtocolSize;
	vectSAL_PACKET_CLASS	PacketClassList;

	void InitSAL_PACKET(void)
	{
		PacketSize		= 0;
		util::zero(PacketBuf, SIZE_MAX_PACKET);
		PacketClassList.clear();
		ProtocolCnt			= 0;
		TotalProtocolSize	= 0;
	}
};
typedef vector<SAL_PACKET>		vectSAL_PACKET;

class CAnalyzePacket  
{
public:
	CAnalyzePacket();
	~CAnalyzePacket();

	BOOL LoadWriteBufDataFile(char *i_szFilePath);
	void AddPacket(SAL_PACKET *i_pPacket);
	BOOL Analyze(void);
	BOOL _AnalyzePacket(SAL_PACKET *io_pPacket);
	BOOL SaveResult(char *i_szDirectory, char *i_szFileName);

	static void AddProtocol(vectSAL_PACKET_CLASS *io_pPKClassList, MessageType_t i_msgTy, int i_nMsgSize);
	static void AddProtocol(vectSAL_PACKET_CLASS *io_pPKClassList, SAL_PACKET_CLASS *i_pPKClass);
	static BOOL WriteSystemLogEX(CSystemLogManager *i_pLogMan, const char* pFormat, ...);
	static BOOL SavePacketClass(CSystemLogManager *i_pLogMan, vectSAL_PACKET_CLASS *i_pPKClassList, DWORD i_dwTotalProtoCnt, DWORD i_dwTotalProtoSize);

	vectSAL_PACKET			m_PacketList;

	DWORD					m_dwTotalProtocolCnt;
	DWORD					m_dwTotalProtocolSize;
	vectSAL_PACKET_CLASS	m_TotalPacketClassList;
};

#endif // !defined(AFX_ANALYZEPACKET_H__75E6FCDC_51B6_4EA0_88F5_7D799788EE11__INCLUDED_)
