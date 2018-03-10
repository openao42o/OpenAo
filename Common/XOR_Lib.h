#ifndef _XOR_LIB_H_
#define	_XOR_LIB_H_
///////////////////////////////////////////////////////////////////////////////
// 2007-10-23 by cmkwon, 추가함

#define SIZE_MAX_XOR_DATA_TO_STRING		500			// 2007-10-23 by cmkwon, XOR로 인코딩 데이터를 스트링으로 변환 할 수 있는 최대 길이
#define SIZE_MAX_XOR_KEY_STRING			1024		// 2007-10-23 by cmkwon, XOR key로 사용 가능한 최대 길이, NULL 문자 포함하지 않은 길이임

class XOR
{
public:
	XOR();
	~XOR();


	static BOOL XOREncode(BYTE *o_pbyEncoded, BYTE *i_pbySource, int i_nSourceSize, char *i_szKeyString);
	static BOOL XOREncode(BYTE *o_pbyEncoded, char *i_szSource, char *i_szKeyString);
	static BOOL XORBinary2String(char *o_szEncodedString, BYTE *i_byBinaryData, int i_nDataSize);
	static BOOL XORString2Binary(BYTE *o_byBinaryData, char *o_szEncodedString);

	// 2008-04-23 by cmkwon, PreServer 주소를 IP와 도메인 둘다 지원 - 
	static BOOL XORDecrypt(char *o_pszOutputString, char *i_pszInputString, char *i_pKeyString);

private:

};



#endif // END - #ifndef _XOR_LIB_H_
