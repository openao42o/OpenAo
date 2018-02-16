//Copyright[2002] MasangSoft
#if !defined(AFX_FUNCTIONLOGGFUNCTIONS_H__28BDAB15_95C4_45C6_9F6B_CA1B61F87375__INCLUDED_)
#define AFX_FUNCTIONLOGGFUNCTIONS_H__28BDAB15_95C4_45C6_9F6B_CA1B61F87375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _FUNCTION_LOG

BOOL GInitFunctionLogSharedMemory(void);
BOOL GCloseFunctionLogSharedMemory(void);
BOOL GWriteFunctionLogSharedMemory( char *i_pBuffer, int i_nSize );
BOOL GReadFunctionLogSharedMemory( int *io_nCurrentLine, char *&io_pBuffer, int *io_nSize );

#endif //_FUNCTION_LOG_endif

#endif // !defined(AFX_FUNCTIONLOGGFUNCTIONS_H__28BDAB15_95C4_45C6_9F6B_CA1B61F87375__INCLUDED_)
