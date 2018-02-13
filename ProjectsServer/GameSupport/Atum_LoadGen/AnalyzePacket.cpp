// AnalyzePacket.cpp: implementation of the CAnalyzePacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Atum_LoadGen.h"
#include "AnalyzePacket.h"
#include "FieldWinSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnalyzePacket::CAnalyzePacket()
{

	m_dwTotalProtocolCnt	= 0;
	m_dwTotalProtocolSize	= 0;
}

CAnalyzePacket::~CAnalyzePacket()
{

}

BOOL CAnalyzePacket::LoadWriteBufDataFile(char *i_szFilePath)
{
	CFile file;
	
	if(FALSE == file.Open(i_szFilePath, CFile::modeRead))
	{
		CString csMsg;
		csMsg.Format("Invalid file path !!, path(%s)", i_szFilePath);
		AfxMessageBox(csMsg);
		return FALSE;
	}
	
	DWORD dwfileSize		= file.GetLength();
	DWORD dwTotalReadSize	= 0;
	
	if(0 == dwfileSize)
	{
		return TRUE;
	}
	
	while(dwTotalReadSize < dwfileSize)
	{
		SAL_PACKET_HEADER	pkHeader;
		SAL_PACKET			packet;
		util::zero(&pkHeader, sizeof(pkHeader));
		packet.InitSAL_PACKET();
		
		UINT uiReadBytes	= 0;
		UINT uiReadedBytes	= 0;
		
		uiReadBytes		= SIZE_PACKET_HEADER;
		uiReadedBytes	= file.Read(&pkHeader, uiReadBytes);
		if(uiReadedBytes != uiReadBytes
			|| pkHeader.usPacketSize > SIZE_MAX_PACKET)
		{
			return FALSE;
		}
		dwTotalReadSize		+= uiReadedBytes;
		packet.PacketSize	= pkHeader.usPacketSize;
		
		uiReadBytes			= pkHeader.usPacketSize;
		uiReadedBytes		= file.Read(packet.PacketBuf, uiReadBytes);
		if(uiReadedBytes != uiReadBytes)
		{
			return FALSE;
		}
		dwTotalReadSize		+= uiReadedBytes;
		
		this->AddPacket(&packet);
	}

	return TRUE;
}

void CAnalyzePacket::AddPacket(SAL_PACKET *i_pPacket)
{
	m_PacketList.push_back(*i_pPacket);
}

struct sort_SAL_PACKET_CLASS_By_MsgTyByte
{
	bool operator()(SAL_PACKET_CLASS op1, SAL_PACKET_CLASS op2)
	{
		return op1.MsgTyByte > op2.MsgTyByte;				// 내림차순 정렬
	}
};

BOOL CAnalyzePacket::Analyze(void)
{
	vectSAL_PACKET::iterator itr(m_PacketList.begin());
	for(; itr != m_PacketList.end(); itr++)
	{
		SAL_PACKET *pPacket = itr;
		if(FALSE == this->_AnalyzePacket(pPacket))
		{
			return FALSE;
		}

		sort(pPacket->PacketClassList.begin(), pPacket->PacketClassList.end(), sort_SAL_PACKET_CLASS_By_MsgTyByte());

		vectSAL_PACKET_CLASS::iterator itrPK(pPacket->PacketClassList.begin());
		for(; itrPK != pPacket->PacketClassList.end(); itrPK++)
		{
			CAnalyzePacket::AddProtocol(&m_TotalPacketClassList, itrPK);
		}
		m_dwTotalProtocolCnt	+= pPacket->ProtocolCnt;
		m_dwTotalProtocolSize	+= pPacket->TotalProtocolSize;
	}

	sort(m_TotalPacketClassList.begin(), m_TotalPacketClassList.end(), sort_SAL_PACKET_CLASS_By_MsgTyByte());
	return TRUE;
}

BOOL CAnalyzePacket::_AnalyzePacket(SAL_PACKET *io_pPacket)
{
	CFieldWinSocket tmFWSoc(NULL, WM_ASYNC_EVENT, WM_PACKET_NOTIFY, TRUE);
	tmFWSoc.SetSAL_PACKET(io_pPacket);
	
	if(FALSE == tmFWSoc.OnRecvdPacket((char*)io_pPacket->PacketBuf, io_pPacket->PacketSize, 0))
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CAnalyzePacket::SaveResult(char *i_szDirectory, char *i_szFileName)
{
	if(m_PacketList.empty())
	{
		return TRUE;
	}

	CSystemLogManager	logFile;
	if(FALSE == logFile.InitLogManger(TRUE, i_szFileName, i_szDirectory))
	{
		return FALSE;
	}

	CAnalyzePacket::WriteSystemLogEX(&logFile, "===============================================================================\r\n");
	CAnalyzePacket::WriteSystemLogEX(&logFile, "Total Packet Count(%6d)    Total Protocol Count(%6d)    Total Protocol Size(%6d) \r\n", m_PacketList.size(), m_dwTotalProtocolCnt, m_dwTotalProtocolSize);

	CAnalyzePacket::SavePacketClass(&logFile, &m_TotalPacketClassList, m_dwTotalProtocolCnt, m_dwTotalProtocolSize);

	CAnalyzePacket::WriteSystemLogEX(&logFile, "===============================================================================\r\n");

// 2009-03-16 by cmkwon, 필요 없을 것으로 보임
// 	vectSAL_PACKET::iterator itr(m_PacketList.begin());
// 	int i = 0;
// 	for(; itr != m_PacketList.end(); itr++, i++)
// 	{
// 		CAnalyzePacket::WriteSystemLogEX(&logFile, "    ===============================================================================\r\n");
// 		CAnalyzePacket::WriteSystemLogEX(&logFile, "    Total Protocol Count: %4d,  PacketIndex: %4d \r\n", itr->PacketClassList.size(), i);
// 
// 		CAnalyzePacket::SavePacketClass(&logFile, &(itr->PacketClassList));
// 
// 		CAnalyzePacket::WriteSystemLogEX(&logFile, "    ===============================================================================\r\n");
// 	}

	return TRUE;
}

void CAnalyzePacket::AddProtocol(vectSAL_PACKET_CLASS *io_pPKClassList, MessageType_t i_msgTy, int i_nMsgSize)
{
	if(NULL == io_pPKClassList)
	{
		return;
	}

	vectSAL_PACKET_CLASS::iterator itr(io_pPKClassList->begin());
	for(; itr != io_pPKClassList->end(); itr++)
	{
		if(itr->MsgTy == i_msgTy)
		{
			itr->MsgTyCount++;
			itr->MsgTyByte	+= i_nMsgSize;
			return;
		}
	}
	
	SAL_PACKET_CLASS tmPKCl;
	
	tmPKCl.MsgTy		= i_msgTy;
	tmPKCl.MsgTyByte	= i_nMsgSize;
	tmPKCl.MsgTyCount	= 1;
	io_pPKClassList->push_back(tmPKCl);
}

void CAnalyzePacket::AddProtocol(vectSAL_PACKET_CLASS *io_pPKClassList, SAL_PACKET_CLASS *i_pPKClass)
{
	if(NULL == io_pPKClassList)
	{
		return;
	}

	vectSAL_PACKET_CLASS::iterator itr(io_pPKClassList->begin());
	for(; itr != io_pPKClassList->end(); itr++)
	{
		if(itr->MsgTy == i_pPKClass->MsgTy)
		{
			itr->MsgTyCount	+= i_pPKClass->MsgTyCount;
			itr->MsgTyByte	+= i_pPKClass->MsgTyByte;
			return;
		}
	}	
	
	io_pPKClassList->push_back(*i_pPKClass);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
/// \author		cmkwon
/// \date		2009-03-13 ~ 2009-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAnalyzePacket::WriteSystemLogEX(CSystemLogManager *i_pLogMan, const char* pFormat, ...)
{
	char szLogMsg[1024];
	va_list args;
	va_start(args, pFormat);
	vsprintf(szLogMsg, pFormat, args);
	
	return i_pLogMan->WriteSystemLog(szLogMsg, FALSE);
}


BOOL CAnalyzePacket::SavePacketClass(CSystemLogManager *i_pLogMan, vectSAL_PACKET_CLASS *i_pPKClassList, DWORD i_dwTotalProtoCnt, DWORD i_dwTotalProtoSize)
{
	vectSAL_PACKET_CLASS::iterator itr(i_pPKClassList->begin());
	for(; itr != i_pPKClassList->end(); itr++)
	{
		CAnalyzePacket::WriteSystemLogEX(i_pLogMan, "    0x%04X  %-50s    Count %6d %2d%%        Size %6d %2d%% \r\n"
			, itr->MsgTy, GetProtocolTypeString(itr->MsgTy), itr->MsgTyCount, (itr->MsgTyCount * 100 / i_dwTotalProtoCnt), itr->MsgTyByte, (itr->MsgTyByte * 100 / i_dwTotalProtoSize));
	}

	return TRUE;
}
