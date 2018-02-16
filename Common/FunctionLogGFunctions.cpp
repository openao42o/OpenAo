//Copyright[2002] MasangSoft
// FunctionLog.cpp: implementation of the CFunctionLog class.
//
//////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
//#include <afxwin.h>
#define DIRECTINPUT_VERSION 0x800
#define _WIN32_WINNT 0x0400
#include <windows.h>

#include <tchar.h>
#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"
#include "DbgOut_c.h"

#ifdef _FUNCTION_LOG

#define FUNCTION_LOG_SHARED_MEMORY_NAME        "AtumFunctionLogMemory"
int        g_nCurrentSharedMemoryLine = 0;
char    *g_szFunctionLogSharedMemory = NULL; //[MAX_FUNCTION_LOG_LINE][SIZE_OF_LOG];
HANDLE    g_hFunctionLogSharedMemory = NULL;
CRITICAL_SECTION        m_csFunctionLog;

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL GInitFunctionLogSharedMemory(void)
/// \brief        펑션로깅을 위한 공유 메모리 초기화
/// \author        dhkwon
/// \date        2004-03-15 ~ 2004-03-15
/// \warning    
///
/// \param        VOID
/// \return        BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL GInitFunctionLogSharedMemory(void)
{
    g_hFunctionLogSharedMemory = CreateFileMapping( (HANDLE)0XFFFFFFFF, 
                                  NULL, 
                                  PAGE_READWRITE, 
                                  0, 
                                  MAX_FUNCTION_LOG_LINE*SIZE_OF_LOG,
                                  FUNCTION_LOG_SHARED_MEMORY_NAME );
    if( g_hFunctionLogSharedMemory != NULL )
    {
#ifdef _DEBUG
        if( GetLastError() == ERROR_ALREADY_EXISTS )
        {
            DBGOUT("GInitFunctionLogSharedMemory(void) : Opend pre_existing shared memory.\n");
        }
#endif // _DEBUG_endif
    }
    else
    {
        DBGOUT( "GInitFunctionLogSharedMemory(void) : Unable to create shared memory.\n");
        return NULL;
    }
    g_szFunctionLogSharedMemory = (LPTSTR) MapViewOfFile( g_hFunctionLogSharedMemory, 
                                             FILE_MAP_ALL_ACCESS,
                                             0,
                                             0,
                                             0 );
    if( g_szFunctionLogSharedMemory == NULL )
    {
        DBGOUT( "GInitFunctionLogSharedMemory(void) : Unable to map into memory.\n");
        return FALSE;
    }
    InitializeCriticalSection(&m_csFunctionLog);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL GCloseFunctionLogSharedMemory(void)
/// \brief        펑션로깅을 위한 공유메모리 해제
/// \author        dhkwon
/// \date        2004-03-15 ~ 2004-03-15
/// \warning    
///
/// \param        VOID
/// \return        BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL GCloseFunctionLogSharedMemory(void)
{
    UnmapViewOfFile(g_szFunctionLogSharedMemory);
    CloseHandle(g_hFunctionLogSharedMemory);
    g_szFunctionLogSharedMemory = NULL;
    DeleteCriticalSection(&m_csFunctionLog);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL GWriteFunctionLogSharedMemory( char *i_pBuffer, int i_nSize )
/// \brief        펑션로깅의 공유메모리로 전송
/// \author        dhkwon
/// \date        2004-03-15 ~ 2004-03-15
/// \warning    
///
/// \param        char *i_pBuffer, int i_nSize
/// \return        BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL GWriteFunctionLogSharedMemory( char *i_pBuffer, int i_nSize )
{
    if( !g_szFunctionLogSharedMemory )
    {
        return FALSE;
    }
    if( g_nCurrentSharedMemoryLine * SIZE_OF_LOG > (MAX_FUNCTION_LOG_LINE-1)*SIZE_OF_LOG )
    {
        DBGOUT("ERROR : GWriteFunctionLogSharedMemory( char *i_pBuffer, int i_nSize ) CurrentLine이 너무 크다.\n");
        return FALSE;
    }

    EnterCriticalSection(&m_csFunctionLog);
    memcpy( g_szFunctionLogSharedMemory + g_nCurrentSharedMemoryLine * SIZE_OF_LOG, 
            i_pBuffer,
            i_nSize);
    if( ++g_nCurrentSharedMemoryLine >= MAX_FUNCTION_LOG_LINE )
    {
        g_nCurrentSharedMemoryLine = 0;
    }
    LeaveCriticalSection(&m_csFunctionLog);
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL GReadFunctionLogSharedMemory( int *io_nStartIndex, char *io_pBuffer, int *io_nSize )
/// \brief        펑션로깅의 공유메모리를 읽음
/// \author        dhkwon
/// \date        2004-03-15 ~ 2004-03-15
/// \warning    전체를 읽어온다.
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL GReadFunctionLogSharedMemory( int *io_nCurrentLine, char *&io_pBuffer, int *io_nSize )
{
    *io_nCurrentLine = g_nCurrentSharedMemoryLine;
    io_pBuffer = g_szFunctionLogSharedMemory;
    *io_nSize = MAX_FUNCTION_LOG_LINE * SIZE_OF_LOG;
    return TRUE;
}

#endif // _FUNCTION_LOG_endif
