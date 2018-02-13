// WinPacket.h: interface for the Packet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_)
#define AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SocketHeader.h"

class CSendPacket
{
public:
	// Construction
	CSendPacket(char* pBlock = NULL, int nLength = 0, BOOL EncodingFlag = FALSE, BYTE seq = 0)
	{
		m_nPacketLength	= 0;
		m_bEncodingFlag = FALSE;
		memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);

		if(pBlock
			&& SetPacket(pBlock, nLength, EncodingFlag, seq) == FALSE)
		{
			m_nPacketLength	= 0;
			m_bEncodingFlag = FALSE;
			memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);
		}
	}
	~CSendPacket(){};

public:	
	inline void Init();
	inline BOOL SetPacket(char *pBlock, int nLength, BOOL EncodingFlag = FALSE, BYTE seq = 0);
	inline int GetLength(void);
	inline char* GetPacket(void);	
	inline BOOL IsPacketEncoded(void);

#ifdef _ATUM_SERVER
	void * operator new(size_t size);
	void operator delete(void* p);
#endif
private:
	inline void EncodePacket(char *pBlock, int nLength, BYTE seq);

private:
	// Attributes
	char			m_pPacket[SIZE_MAX_SOCKET_BUFFER];
	int				m_nPacketLength;
	BOOL			m_bEncodingFlag;						// is packet encoded
	static const char*	m_pXOR_ENCODE_BYTES;
};

class CRecvPacket
{
public:
	// Construction
	CRecvPacket();
	~CRecvPacket();

public:
	inline void Init(void);	
	inline int AddTail(char* pBlock, int nLength, int* UsedBytes);

	inline int GetLength(void);
	inline char* GetPacket(void);	
	inline BOOL RecvCompleted();
	inline BOOL IsPacketEncoded();
	inline BOOL IsValidPacket();
	inline BYTE GetSequenceNumber();

#ifdef _ATUM_SERVER
	void * operator new(size_t size);
	void operator delete(void* p);
#endif

protected:
	inline void DecodePacket();

protected:
	// Attributes
	char 	 	 	m_pPacket[SIZE_MAX_SOCKET_BUFFER];
	char			m_pPacketHeader[SIZE_PACKET_HEADER];
	int	            m_nPacketLength;
	int				m_nCurrentLength;
	int				m_nRemainedHeaderSize;

	// decoding
	DECODING_INFO	m_DecodingInfo;
	static const char*	m_pXOR_ENCODE_BYTES;
};

///////////////////////////////////////////////////////////////////////////////
// CSendPacket's Inline functions
///////////////////////////////////////////////////////////////////////////////
void CSendPacket::Init()
{
	m_nPacketLength = 0;
	m_bEncodingFlag = FALSE;
	memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);
}

int CSendPacket::SetPacket(char *pBlock, int nLength, BOOL EncodingFlag, BYTE seq)
{
	if(NULL == pBlock 
		|| 0 >= nLength 
		|| SIZE_MAX_PACKET < nLength)
	{
		return FALSE;
	}

	m_bEncodingFlag = EncodingFlag;
	if (EncodingFlag == TRUE)
	{
		// int nDummyLen = seq % 4;
		EncodePacket(pBlock, nLength, seq);
	}
	else
	{
		*(unsigned short*)m_pPacket = nLength;							// packet body length
		*(BYTE*)(m_pPacket+SIZE_BODY_LENGTH) = 0x00;					// encode flag
		*(BYTE*)(m_pPacket+SIZE_BODY_LENGTH+SIZE_ENCODE_FLAG) = 0x00;	// seq. #
		memcpy(m_pPacket + SIZE_PACKET_HEADER, (const void*)pBlock, nLength);

		m_nPacketLength = nLength + SIZE_PACKET_HEADER;					// 전체 길이는 body와 헤더 부분 합한 길이
	}

	return TRUE;
}

int CSendPacket::GetLength(void)
{
	return m_nPacketLength;
}

char* CSendPacket::GetPacket(void)
{
	return m_pPacket;
}

void CSendPacket::EncodePacket(char *pBlock, int nLength, BYTE seq)
{
	// XOR_N 생성, 4, 8, 12, ... , 124
	BYTE XOR_N = (BYTE)((GetTickCount() % 30 + 1) * 4);

	// 변수들 선언, 정의
	BYTE checkSum = 0;
	int nDummyLen = seq%4;
	int woffset = SIZE_BODY_LENGTH;
	int roffset = 0;
	int xoffset = XOR_N;

	// size를 복사
	*(unsigned short*)m_pPacket
		= (unsigned short)(nLength + SIZE_CHECKSUM + nDummyLen);

	// encode flag 1B를 생성하고 복사
	BYTE encodeFlag = ENCODE_MASK | (XOR_N_MASK & XOR_N);
	*(BYTE*)(m_pPacket+woffset) = encodeFlag;
	woffset += SIZE_ENCODE_FLAG;

	// sequence number(1B)를 XOR한후, 복사, checksum 계산 시작
	checkSum ^= seq;
	*(BYTE*)(m_pPacket+woffset) = (seq ^ m_pXOR_ENCODE_BYTES[XOR_N-1]);
	woffset += SIZE_SEQ_NUM;

	// MSGs를 4B씩 XOR, checksum 계산
	int nEndValue = (int)(nLength / 4) * 4;
	DWORD checkSum_4B = 0;
	while (roffset < nEndValue)
	{
		checkSum_4B ^= *(DWORD*)(pBlock+roffset);
		*(DWORD*)(m_pPacket+woffset)
			= *(DWORD*)(pBlock+roffset) ^ *(DWORD*)(m_pXOR_ENCODE_BYTES+xoffset);
		woffset += 4;
		roffset += 4;
		xoffset = (xoffset + 4) % SIZE_XOR_ENCODE_BYTES;
	}

	// 4B의 checkSum_4B를 1B의 checkSum으로 변환
	checkSum ^= ( *((char*)&checkSum_4B)
				^ *((char*)&checkSum_4B+1)
				^ *((char*)&checkSum_4B+2)
				^ *((char*)&checkSum_4B+3) );

	// 남아있는 nLength - nEndValue 만큼의 데이터를 처리 -> XOR + checkSum
	for (int i = 0; i < nLength - nEndValue; i++)
	{
		checkSum ^= pBlock[roffset];
		m_pPacket[woffset]
			= pBlock[roffset] ^ m_pXOR_ENCODE_BYTES[xoffset];
		woffset++;
		roffset++;
		xoffset = (xoffset + 1) % SIZE_XOR_ENCODE_BYTES;
	}

	// checksum을 추가
	m_pPacket[woffset++] =  checkSum ^ m_pXOR_ENCODE_BYTES[xoffset];

	// dummy data는 XOR_ENCODE_BYTES를 사용함
	auto dummyData = m_pXOR_ENCODE_BYTES + XOR_N;

	memcpy(m_pPacket+woffset, dummyData, nDummyLen);
	woffset += nDummyLen;

	m_nPacketLength = woffset;

}

BOOL CSendPacket::IsPacketEncoded(void)
{
	return m_bEncodingFlag;
}


///////////////////////////////////////////////////////////////////////////////
// CRecvPacket's Inline functions
///////////////////////////////////////////////////////////////////////////////
void CRecvPacket::Init(void)
{
	memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);
	memset(m_pPacketHeader, 0x00, SIZE_PACKET_HEADER);
	m_nPacketLength			= 0;
	m_nCurrentLength		= 0;
	m_nRemainedHeaderSize	= 0;
	memset(&m_DecodingInfo, 0x00, sizeof(DECODING_INFO));
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CRecvPacket::AddTail
// 반환되는 형  : int
// 함 수 인 자  : char *pBlock
// 함 수 인 자  : int nLength
// 함 수 인 자  : int* UsedBytes
// 함 수 설 명  : inline 함수
//					Socket으로 받은 데이터를 Packet으로 완성하는 함수
//					packet이 암호화되어 있다면, 필요로 하는 데이터를 다 받은 후에
//					AddTail()에서 자동으로 Decoding을 수행함
//					0 보다 작은 값을 리턴할때가 오류가 발생했을때이다.(리턴값이 0일때 정상적으로 처리됨)
int CRecvPacket::AddTail(char *pBlock, int nLength, int* UsedBytes)
{
	*UsedBytes = 0;
	if(NULL == pBlock 
		|| 0 >= nLength 
		|| RecvCompleted())
	{	// 파라미터가 유효하지 않고 이미 패킷이 완성되어있으면 FALSE를 리턴
		
		return -1;
	}

	if(m_nRemainedHeaderSize > 0)
	{	// 받아야할 Header Size가 있다면 먼저 Header를 완성한다.

		if(m_nRemainedHeaderSize > SIZE_PACKET_HEADER){		return -2;}	// 받아야할 Header Size가 유효하지 않음
		if(m_nRemainedHeaderSize <= nLength)		
		{	// 받은 데이터가 남은 패킷 헤더 사이즈 보다 크면

			memcpy(m_pPacketHeader + SIZE_PACKET_HEADER - m_nRemainedHeaderSize, pBlock, m_nRemainedHeaderSize);
			pBlock += m_nRemainedHeaderSize;
			nLength -= m_nRemainedHeaderSize;
			*UsedBytes += m_nRemainedHeaderSize;
			m_nRemainedHeaderSize = 0;
			
			m_nPacketLength = *(unsigned short*)m_pPacketHeader;		// body length를 저장
			if(SIZE_MAX_PACKET + SIZE_MAX_DUMMY_DATA + SIZE_CHECKSUM < m_nPacketLength)
			{	// Packet length의 유효성을 체크

				return -3;
			}
		}
		else
		{
			memcpy(m_pPacketHeader + SIZE_PACKET_HEADER - m_nRemainedHeaderSize, pBlock, nLength);
			*UsedBytes += nLength;
			m_nRemainedHeaderSize -= nLength;
			return m_nCurrentLength;
		}
	}
	else if(0 == m_nPacketLength)
	{
		if(SIZE_PACKET_HEADER <= nLength)		
		{	// 받은 데이터가 패킷 헤더 사이즈 보다 크면
			
			memcpy(m_pPacketHeader, pBlock, SIZE_PACKET_HEADER);		// header에서 packet size만 빼낸다.
			pBlock += SIZE_PACKET_HEADER;
			nLength -= SIZE_PACKET_HEADER;
			*UsedBytes += SIZE_PACKET_HEADER;
			
			m_nPacketLength = *(unsigned short*)m_pPacketHeader;		// body length를 저장
			if(SIZE_MAX_PACKET + SIZE_MAX_DUMMY_DATA + SIZE_CHECKSUM < m_nPacketLength)
			{	// Packet length의 유효성을 체크

				return -4;
			}
		}
		else
		{
			memcpy(m_pPacketHeader, pBlock, nLength);
			*UsedBytes += nLength;
			m_nRemainedHeaderSize  = SIZE_PACKET_HEADER - nLength;
			return m_nCurrentLength;
		}
	}

	if(m_nPacketLength - m_nCurrentLength < nLength)	
	{	// 입력된 데이타가 완성해야할 패킷보다 클경우, 완성해야할 데이타 크기만큼만 복사한다.
		
		nLength = m_nPacketLength - m_nCurrentLength;
	}

	if(nLength > 0)
	{
		memcpy(m_pPacket+m_nCurrentLength, pBlock, nLength);
		m_nCurrentLength += nLength;
		*UsedBytes += nLength;
	}
	
	if (m_nCurrentLength == m_nPacketLength)
	{	// 완성해야 할 만큼의 데이타가 다 입력됨

		m_DecodingInfo.bIsVaildPacket = TRUE;
		BYTE encode_flag = *((BYTE*)m_pPacketHeader+SIZE_BODY_LENGTH);
		m_DecodingInfo.bIsPacketEncoded = ((encode_flag&ENCODE_MASK)==ENCODE_MASK)?TRUE:FALSE;

		// 암호화 패킷이면 decode 시작
		if (m_DecodingInfo.bIsPacketEncoded)
		{
			m_DecodingInfo.nXOR_N = (encode_flag&XOR_N_MASK);
			DecodePacket();
		}
		else
		{
			m_DecodingInfo.nSeqNumber = *((BYTE*)m_pPacketHeader + SIZE_BODY_LENGTH + SIZE_ENCODE_FLAG);
		}
	}

	return	m_nCurrentLength;
}

BOOL CRecvPacket::RecvCompleted()
{
	return (m_nPacketLength && m_nCurrentLength == m_nPacketLength);
}

void CRecvPacket::DecodePacket()
{
	char checkSum = 0;
	int woffset	= 0;			// for WRITING decoded data
	int roffset	= 0;			// for READING received data
	int xoffset = m_DecodingInfo.nXOR_N;

	// packet에 포함된 seq. number(1B) 꺼내기
	m_DecodingInfo.nSeqNumber
		= m_pPacketHeader[SIZE_BODY_LENGTH+SIZE_ENCODE_FLAG] ^ m_pXOR_ENCODE_BYTES[xoffset-1];
	checkSum ^= m_DecodingInfo.nSeqNumber;

	// dummy data의 length
	int nDummyLen = m_DecodingInfo.nSeqNumber % 4;

	// XOR 연산 처리, locality 위해 checksum 함께 계산
	// 빠른 계산 위해 4B 씩 계산함
	DWORD checkSum_4B = 0;
	int	nEndValue = (int)((m_nPacketLength - nDummyLen - SIZE_CHECKSUM)/4)*4;
	while(roffset < nEndValue)
	{
		// XOR 처리
		*(DWORD*)(m_pPacket+woffset)
			= *(DWORD*)(m_pPacket+roffset) ^ *(DWORD*)(m_pXOR_ENCODE_BYTES+xoffset);
		// checksum 계산
		checkSum_4B ^= *(DWORD*)(m_pPacket+woffset);

		woffset += 4;
		roffset += 4;
		xoffset = (xoffset + 4) % SIZE_XOR_ENCODE_BYTES;
	}
	// checksum 합치기
	checkSum ^= ( *((char*)&checkSum_4B)
				^ *((char*)&checkSum_4B+1)
				^ *((char*)&checkSum_4B+2)
				^ *((char*)&checkSum_4B+3) );

	// 4바이트씩 처리하고 남은 바이트를 처리
	int nRepeat = (m_nPacketLength - nDummyLen - SIZE_CHECKSUM) - nEndValue;
	for ( int i = 0; i < nRepeat; i++)
	{
		m_pPacket[woffset]
			= m_pPacket[roffset] ^ m_pXOR_ENCODE_BYTES[xoffset];
		checkSum ^= m_pPacket[woffset];

		woffset += 1;
		roffset += 1;
		xoffset = (xoffset + 1) % SIZE_XOR_ENCODE_BYTES;
	}

	// packet에 포함된 checksum 꺼내기
	char packetCheckSum
		= m_pPacket[roffset] ^ m_pXOR_ENCODE_BYTES[xoffset];

	// checksum 확인
	if ( checkSum != packetCheckSum )
	{
		// error, invalid checksum
		m_DecodingInfo.bIsVaildPacket = FALSE;
		return;
	}

	m_nPacketLength = m_nCurrentLength = woffset;
}

int CRecvPacket::GetLength(void)
{
	return m_nPacketLength;
}

char* CRecvPacket::GetPacket(void)
{
	return m_pPacket;
}

BOOL CRecvPacket::IsPacketEncoded()
{
	return m_DecodingInfo.bIsPacketEncoded;
}

BOOL CRecvPacket::IsValidPacket()
{
	return m_DecodingInfo.bIsVaildPacket;
}

BYTE CRecvPacket::GetSequenceNumber()
{
	return m_DecodingInfo.nSeqNumber;
}
#endif // !defined(AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_)
