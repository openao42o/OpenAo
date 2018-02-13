// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

//######################################################################################
// File    : NTExceptionCause.h
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Exception Cause
// Bug     : 
//######################################################################################
#ifndef __NEXT_EXCEPTION_CAUSE__
#define __NEXT_EXCEPTION_CAUSE__

#include <Windows.h>
#include <tchar.h>

void OnExceptionCause(DWORD ExceptionCode, TCHAR* pszString, size_t nSize);

#endif //(__NEXT_EXCEPTION_CAUSE__)

// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송