
#include "StdAfx.h"
#include "dllMD5.h"
#include "md5_lib_src.h"


BOOL MD5Encode(BYTE *o_pbyEncodedData, char *i_szSourceString)
{
	if((int)strlen(i_szSourceString) < 1
		|| (int)strlen(i_szSourceString) > 16)
	{
		return FALSE;
	}

	MD5		instance;
	instance.MD5Encode(i_szSourceString, o_pbyEncodedData);
	return TRUE;
}