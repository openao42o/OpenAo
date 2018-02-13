// CodeTrans.cpp: implementation of the CCodeTrans class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "CodeTrans.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCodeTrans::CCodeTrans()
{

}

CCodeTrans::~CCodeTrans()
{

}

void CCodeTrans::NtsToLong(unsigned char* szValue, unsigned long& nValue)
{
	int pos=0;
	int posV;
	int charV;
	nValue = 0;
	while(pos<7) 
	{
		posV=1;
		for (int i=0;i<pos;i++) 
		{
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

void CCodeTrans::LongToNts(unsigned long nValue, unsigned char *szValue)
{
	unsigned char charTable[37]={ "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	int pos=0;
	int pV;
	while (pos < 7)
	{
		  pV = nValue - nValue / 36*36;
		  szValue[pos]=charTable[pV];
		  pos++;
		  nValue = nValue/36;
	}
	szValue[pos] = '\0';
}

unsigned long CCodeTrans::shifter(unsigned long i,bool bForward)
{
	//00,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
	int shiftTable[32]={ 26,31,17,10,30,16,24, 2,29, 8,20,15,28,11,13, 4,19,23, 0,12,14,27, 6,18,21, 3, 9, 7,22, 1,25, 5};
	int reverseTable[32]={ 18,29,07,25,15,31,22,27, 9,26, 3,13,19,14,20,11, 5, 2,23,16,10,24,28,17, 6,30, 0,21,12, 8, 4, 1};

	unsigned long result=0;
	int pos = 0;
	int value;
	while (i>0) 
	{
		value = i - ((i >> 1) << 1);
		i = i >> 1;
		if (value) 
		{
			if (bForward)
				result += value << shiftTable[pos];
			else 
				result += value << reverseTable[pos];
		}
		pos++;
	}
	return result;
}

unsigned char* CCodeTrans::decrypt(unsigned char* original,int seed)
{
	int pos = 0;
	int szpos = 0;
	unsigned long part;
	unsigned long origin;
	static unsigned char nts[1024];		
	ZeroMemory( nts, sizeof(nts) );
	int length;

	length =strlen((char*)original);
	while (szpos < length) 
	{
		NtsToLong(original+szpos,origin);
		part = shifter(origin,false);
		part = part -seed;
		memcpy(nts+pos,(char*)&part,4);
		pos+=4;
		szpos+=7;
	}
	return nts;
}

void CCodeTrans::Encrypt(TCHAR* pszOriginal, int nSeed, TCHAR* pszEncrypt)
{
	int                  nPos       = 0;
	int                  nSzpos     = 0;
	int                  nLength    = 0;
	TCHAR                szID[256]  = {0,};
	unsigned long        lPart      = 0;
	unsigned long        lOrigin    = 0;
	static unsigned char szNts[256] = {0,};
	
	memset(szID, '\0', sizeof(szID));
	lstrcpy(szID, pszOriginal);

	szID[lstrlen(szID)] = '\0';
	nLength = lstrlen(szID);

	while (nPos < nLength) 
	{
		lOrigin = *(unsigned long*)(szID + nPos);
		lOrigin = lOrigin + nSeed;
	 	lPart   = shifter(lOrigin, TRUE);
		
		LongToNts(lPart, szNts+nSzpos);
		
		nPos   += 4;
		nSzpos +=7;
	}

	lstrcpy(pszEncrypt, (TCHAR*)szNts);
}