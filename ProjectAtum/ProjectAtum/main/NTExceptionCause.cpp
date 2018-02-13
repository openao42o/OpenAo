// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

//######################################################################################
// File    : NTExceptionCause.cpp
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Exception Cause
// Bug     : 
//######################################################################################
#include "NTExceptionCause.h"

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 충돌 원인 분석
//######################################################################################
void OnExceptionCause(DWORD ExceptionCode, TCHAR* pszString, size_t nSize)
{
	struct ExceptionNames
	{
		DWORD	ExceptionCode;
		TCHAR *	ExceptionName;
	};

	ExceptionNames ExceptionMap[] =
	{
		{0x40010005, _T("Control-C로 인한 오류가 발생하였습니다.")},
		{0x40010008, _T("Control-Break로 인한 오류가 발생하였습니다.")},
		{0x80000002, _T("정렬되지 않은 데이타에 접근하였습니다.")},
		{0x80000003, _T("Breakpoint가 걸렸습니다.")},
		{0xc0000005, _T("잘못된 참조가 발생하였습니다")},
		{0xc0000006, _T("page를 Load할 수 없어 진행 할 수 없습니다.")},
		{0xc0000017, _T("Memory할당에 실패 하였습니다.")},
		{0xc000001d, _T("잘못된 명령어를 실행하였습니다.")},
		{0xc0000025, _T("오류로 인해 더이상 진행 시킬수 없습니다.")},
		{0xc0000026, _T("잘못된 기능에 접근 하였습니다.")},
		{0xc000008c, _T("배열 범위를 초과 하였습니다.")},
		{0xc000008d, _T("Float정규화가 되지않는 작은값 입니다.")},
		{0xc000008e, _T("Float를 0으로 나누었습니다.")},
		{0xc000008f, _T("Float소수를 정확하게 나타낼수 없습니다.")},
		{0xc0000090, _T("Float에서 알수없는 오류가 발생하였습니다.")},
		{0xc0000091, _T("Float Overflow가 발생하였습니다.")},
		{0xc0000092, _T("Float Stack값을 넘었거나 값이 너무 작습니다.")},
		{0xc0000093, _T("Float 값이 너무 작습니다.")},
		{0xc0000094, _T("Integer를 0으로 나누었습니다.")},
		{0xc0000095, _T("Integer Overflow가 발생하였습니다.")},
		{0xc0000096, _T("해당 명령어를 실행 할 수 없습니다.")},
		{0xc00000fD, _T("Stack Overflow가 발생하였습니다.")},
		{0xc0000142, _T("DLL을 초기화 할 수 없습니다.")},
		{0xe06d7363, _T("Microsoft C++에서 오류가 발생하였습니다.")},
	};

	int nStringLen = nSize/sizeof(TCHAR);
	for (int i = 0; i < sizeof(ExceptionMap) / sizeof(ExceptionMap[0]); i++)
	{
		if (ExceptionCode == ExceptionMap[i].ExceptionCode)
		{
			_tcsncpy( pszString, ExceptionMap[i].ExceptionName, nStringLen );
			pszString[nStringLen-1] = NULL;
			return;
		}
	}
	_tcsncpy( pszString, _T("정확한 오류 원인을 찾을 수 없습니다."), nStringLen );
	pszString[nStringLen-1] = NULL;
}

// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송