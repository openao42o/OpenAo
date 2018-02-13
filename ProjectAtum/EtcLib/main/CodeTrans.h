// CodeTrans.h: interface for the CCodeTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODETRANS_H__8569D754_CCE6_48C6_806F_2E5B60BD0093__INCLUDED_)
#define AFX_CODETRANS_H__8569D754_CCE6_48C6_806F_2E5B60BD0093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>

class CCodeTrans  
{
public:
	CCodeTrans();
	virtual ~CCodeTrans();
	void				NtsToLong(unsigned char* szValue, unsigned long& nValue);
	void				LongToNts(unsigned long nValue, unsigned char *szValue);
	unsigned long		shifter(unsigned long i,bool bForward);
	unsigned char*		decrypt(unsigned char* original,int seed);
	void				Encrypt(TCHAR* pszOriginal, int nSeed, TCHAR* pszEncrypt);

};

#endif // !defined(AFX_CODETRANS_H__8569D754_CCE6_48C6_806F_2E5B60BD0093__INCLUDED_)
