//Copyright[2002] MasangSoft
// FunctionLog.h: interface for the CFunctionLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTIONLOG_H__28BDAB15_95CE_45C6_9F6B_CA1B61F87375__INCLUDED_)
#define AFX_FUNCTIONLOG_H__28BDAB15_95CE_45C6_9F6B_CA1B61F87375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _FUNCTION_LOG

#define FLOG(name)				CFunctionLog fLog(name)
#define SIZE_OF_LOG				256
#define MAX_FUNCTION_LOG_LINE	200

///////////////////////////////////////////////////////////////////////////////
/// \class		CFunctionLog
///
/// \brief		함수의 진입과 이탈을 로그로 남겨, 디버깅에 이용한다.
/// \author		dhkwon
/// \version	
/// \date		2004-03-12 ~ 2004-03-12
/// \warning	함수 진입시에만 표시를 하면 함수 이탈시에는 자동으로 이탈코드를 남긴다.
///////////////////////////////////////////////////////////////////////////////
class CFunctionLog
{
public:
	CFunctionLog(char* szName);
	virtual ~CFunctionLog();
protected:
	char m_szFunctionLog[SIZE_OF_LOG];
	char *m_szFunctionName;
};

#else
#define FLOG(name)			__noop
#endif // _FUNCTION_LOG_endif

#endif // !defined(AFX_FUNCTIONLOG_H__28BDAB15_95CE_45C5_9F6B_CA1B61F87375__INCLUDED_)
