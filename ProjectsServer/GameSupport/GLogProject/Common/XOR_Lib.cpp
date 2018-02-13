
#include "stdafx.h"
#include "XOR_Lib.h"



XOR::XOR()
{
}

XOR::~XOR()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL XOR::XOREncode(BYTE *o_pbyEncoded, BYTE *i_pbySource, int i_nSourceSize, char *i_szKeyString)
/// \brief		// 2007-10-24 by cmkwon, 서버 정보 암호화
/// \author		cmkwon
/// \date		2007-10-24 ~ 2007-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL XOR::XOREncode(BYTE *o_pbyEncoded, BYTE *i_pbySource, int i_nSourceSize, char *i_szKeyString)
{
	if(0 >= i_nSourceSize)
	{
		return FALSE;
	}

	int nKeyLen = strlen(i_szKeyString);
	if(SIZE_MAX_XOR_KEY_STRING < nKeyLen)
	{
		return FALSE;
	}

	int nROffset	= 0;
	int nWOffset	= 0;
	int nKeyOffset	= 0;
	if(4 > nKeyLen)
	{
		for(int i=0; i < i_nSourceSize; i++)
		{
			o_pbyEncoded[nWOffset] = i_pbySource[nROffset] ^ i_szKeyString[nKeyOffset];

			nWOffset++;
			nROffset++;
			nKeyOffset = (nKeyOffset+1)%nKeyLen;
		}
	}
	else
	{
		int nEndOffset;
		nEndOffset	= i_nSourceSize - i_nSourceSize%4;	// 2007-10-24 by cmkwon, 4의 배수 단위까지만 while()문으로 처리
		nKeyLen		= nKeyLen - nKeyLen%4;				// 2007-10-24 by cmkwon, 4의 배수 단위까지만 사용한다.
		while(nROffset < nEndOffset)
		{
			*(DWORD*)(o_pbyEncoded+nWOffset) = *(DWORD*)(i_pbySource+nROffset) ^ *(DWORD*)(i_szKeyString+nKeyOffset);

			nWOffset	+= 4;
			nROffset	+= 4;
			nKeyOffset	= (nKeyOffset + 4) % nKeyLen;
		}

		for(int i=0; i < i_nSourceSize - nEndOffset; i++)
		{
			o_pbyEncoded[nWOffset] = i_pbySource[nROffset] ^ i_szKeyString[nKeyOffset];

			nWOffset++;
			nROffset++;
			nKeyOffset = (nKeyOffset+1) % nKeyLen;
		}		
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL XOR::XOREncode(BYTE *o_pbyEncoded, char *i_szSource, char *i_szKeyString)
/// \brief		// 2007-10-24 by cmkwon, 서버 정보 암호화 - 
/// \author		cmkwon
/// \date		2007-10-24 ~ 2007-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL XOR::XOREncode(BYTE *o_pbyEncoded, char *i_szSource, char *i_szKeyString)
{
	return XOR::XOREncode(o_pbyEncoded, (BYTE*)i_szSource, strlen(i_szSource), i_szKeyString);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL XOR::XORBinary2String(TCHAR *o_szEncodedString, BYTE *i_byBinaryData, int i_nDataSize)
/// \brief		// 2007-10-24 by cmkwon, 서버 정보 암호화
/// \author		cmkwon
/// \date		2007-10-24 ~ 2007-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL XOR::XORBinary2String(TCHAR *o_szEncodedString, BYTE *i_byBinaryData, int i_nDataSize)
{
	if(SIZE_MAX_XOR_DATA_TO_STRING < i_nDataSize)
	{
		return FALSE;
	}
	
	memset(o_szEncodedString, 0x00, 2*i_nDataSize); // 2007-10-24 by cmkwon, Output 버퍼 초기화
	for(int i=0; i < i_nDataSize; i++)
	{
		TCHAR szTemp[512];
		wsprintf(szTemp, L"%02X", i_byBinaryData[i]);
		_tcsncpy_s(o_szEncodedString, 1024, szTemp, 1024);  
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL XOR::XORString2Binary(BYTE *o_byBinaryData, char *o_szEncodedString)
/// \brief		// 2007-10-24 by cmkwon, 서버 정보 암호화
/// \author		cmkwon
/// \date		2007-10-24 ~ 2007-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL XOR::XORString2Binary(BYTE *o_byBinaryData, char *o_szEncodedString)
{
	int nStringLen = strlen(o_szEncodedString);
	if(SIZE_MAX_XOR_DATA_TO_STRING < nStringLen)
	{
		return FALSE;
	}

	memset(o_byBinaryData, 0x00, nStringLen/2);	// 2007-10-24 by cmkwon, Output 버퍼 초기화
	for(int i=0; i < nStringLen/2; i++)
	{
		char szTemp[512];
		memset(szTemp, 0x00, 512);
		memcpy(szTemp, &o_szEncodedString[i*2], 2);
		o_byBinaryData[i] = (BYTE)strtol(szTemp, NULL, 16);
	}	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL XOR::XORDecrypt(char *o_pszOutputString, char *i_pszInputString, char *i_pKeyString)
/// \brief		// 2008-04-23 by cmkwon, PreServer 주소를 IP와 도메인 둘다 지원 - XOR::XORDecrypt() 추가
/// \author		cmkwon
/// \date		2008-04-23 ~ 2008-04-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL XOR::XORDecrypt(char *o_pszOutputString, char *i_pszInputString, char *i_pKeyString)
{
	BYTE byEncodedBinary[1024];	
	memset(byEncodedBinary, 0x00, 1024);
	if(FALSE == XOR::XORString2Binary(byEncodedBinary, i_pszInputString))
	{
		return FALSE;
	}

	return XOR::XOREncode((BYTE*)o_pszOutputString, byEncodedBinary, strlen(i_pszInputString)/2, i_pKeyString);
}