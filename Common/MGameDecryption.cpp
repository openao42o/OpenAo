#include "stdafx.h"
#include "MGameDecryption.h"


char g_szMGameExcuteType[MGAME_MAX_PARAM_STRING_SIZE];
char g_szMGameSeed[MGAME_MAX_PARAM_STRING_SIZE];
char g_szMGameID[MGAME_MAX_PARAM_STRING_SIZE];
char g_szMGameEncryptedID[MGAME_MAX_PARAM_STRING_SIZE];
char g_szMGameEncryptedPWD[MGAME_MAX_PARAM_STRING_SIZE];
char g_szMGameSEX[MGAME_MAX_PARAM_STRING_SIZE];
char g_szMGameYear[MGAME_MAX_PARAM_STRING_SIZE];



void NtsToLong(unsigned char* szValue,unsigned long& nValue);
unsigned long shifter(unsigned long i,bool bForward);

///////////////////////////////////////////////////////////////////////////////////
// 
unsigned char* decrypt(unsigned char* original,int seed)
{
	int pos = 0;
	int szpos = 0;
	unsigned long part;
	unsigned long origin;
	static unsigned char nts[1024];  ::ZeroMemory( nts, sizeof(nts) );
	int length;
	
	length =strlen((char*)original);
	while (szpos < length) {
		NtsToLong(original+szpos,origin);
		part = shifter(origin,false);
		part = part -seed;
		memcpy(nts+pos,(char*)&part,4);
		pos+=4;
		szpos+=7;
	}
	return nts;
}


///////////////////////////////////////////////////////////////////////////////////
//
void NtsToLong(unsigned char* szValue,unsigned long& nValue)
{
	int pos=0;
	int posV;
	int charV;
	nValue = 0;
	while(pos<7) {
		posV=1;
		for (int i=0;i<pos;i++) {
			posV=posV*36;
		}
		if (szValue[pos]>='0' && szValue[pos]<='9')
			charV=szValue[pos]-'0';
		else
			charV=szValue[pos]-'A'+10;
		nValue =nValue+charV*posV;
		pos++;
	}
}

///////////////////////////////////////////////////////////////////////////////////
//
unsigned long shifter(unsigned long i,bool bForward)
{
	//00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
	int shiftTable[32]={ 26,31,17,10,30,16,24, 2,29, 8,20,15,28,11,13, 4,19,23, 0,12,14,27, 6,18,21, 3, 9, 7,22, 1,25, 5};
	int reverseTable[32]={ 18,29,07,25,15,31,22,27, 9,26, 3,13,19,14,20,11, 5, 2,23,16,10,24,28,17, 6,30, 0,21,12, 8, 4, 1};
	
	unsigned long result=0;
	int pos = 0;
	int value;
	while (i>0) {
		value = i - ((i >> 1) << 1);
		i = i >> 1;
		if (value) {
			if (bForward)
				result += value << shiftTable[pos];
			else 
				result += value << reverseTable[pos];
		}
		pos++;
	}
	return result;
}
