// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

//######################################################################################
// File    : NTMiniDump.h
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : MiniDump file 생성
// Bug     : 
//######################################################################################
#ifndef __NEXT_MINIDUMP__
#define __NEXT_MINIDUMP__

#include <windows.h>
#include <dbghelp.h>


class CNTMiniDump
{
public:
	CNTMiniDump();
	~CNTMiniDump();

	LONG OnSaveMiniDump(_EXCEPTION_POINTERS *pException, const TCHAR* pszFileName, HMODULE hDllModule,
						const TCHAR* pszSysInfo, UINT nSysInfoLen, const TCHAR* pszUserData, UINT nUserDataLen);

	void SetDumpType(MINIDUMP_TYPE nDumpType)			{ m_eDumpType = nDumpType; }
	MINIDUMP_TYPE GetDumpType(MINIDUMP_TYPE nDumpType)	{ return m_eDumpType; }

private:

	MINIDUMP_TYPE m_eDumpType;
};

#endif //(__NEXT_MINIDUMP__)

// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송