//Copyright[2002] MasangSoft
// File: DbgOut.h
//

#ifndef COMMON_DBGOUT_C_H_
#define COMMON_DBGOUT_C_H_

static  LPCTSTR    g_dbgOutA                = _T("Client DebugOutput 20020805");
static  LPCTSTR    g_dbgOutwindowClassNameA = _T("Client DebugOut Window");

#include "stdio.h"

inline void DbgOutAA (LPCSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_dbgOutwindowClassNameA, g_dbgOutA); 
    if (hWnd)
    {  
        cd.dwData = 0;
        cd.cbData = (strlen(p)+1)*sizeof(char);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}


// DBGOUTA
#ifdef _DEBUG
#define DBGOUTA(p) DbgOutA(p);
#else
#define DBGOUTA __noop
#endif




inline void DbgOutWA (LPCWSTR p)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_dbgOutwindowClassNameA, g_dbgOutA); 
    if (hWnd)
    {  
        cd.dwData = 0xFEFF;
        cd.cbData = (wcslen(p)+1)*sizeof(wchar_t);
        cd.lpData = (void *)p; 
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);  
    } 
}

// DBGOUTW
#ifdef _DEBUG
#define DBGOUTW(p) DbgOutW(p);
#else
#define DBGOUTW __noop
#endif





inline void DbgOutA (LPCTSTR pFormat, ...)
{
    // 2015-06-30 Future, stopped parsing of Debug messages when no debug target is found
    if (!::FindWindow(g_dbgOutwindowClassNameA, g_dbgOutA))
        return;

    va_list args;
    va_start(args, pFormat);

    _TCHAR buffer[1024*sizeof(_TCHAR)];
    vsprintf_s(buffer, 1024 * sizeof(_TCHAR), pFormat, args);

    #ifdef UNICODE
    DbgOutW (buffer);
    #else
    DbgOutAA (buffer);
    #endif

    va_end(args);
}

// DBGOUT
#ifdef _DEBUG
#define DBGOUT ::DbgOut
#else
#define DBGOUT __noop
#endif

#ifdef _DBGOUT_EFFECT
#define DBGOUT_EFFECT ::DbgOut
#else
#define DBGOUT_EFFECT __noop
#endif


inline DWORD DbgOutLastErrorA (LPCTSTR pFormat, ...)
{
   if (::GetLastError() == 0) 
        return 0;
   
    va_list args;
    va_start(args, pFormat);

    _TCHAR buffer[1024*sizeof(_TCHAR)];
    vsprintf_s(buffer, 1024 * sizeof(_TCHAR), pFormat, args);

    LPVOID pMessage;
    DWORD  result;
    result = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             NULL,
                             GetLastError(),
                             MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                             (LPTSTR)&pMessage,
                             0,
                             NULL);
  
    lstrcat (buffer, _T(" : "));
    lstrcat (buffer, (TCHAR*)pMessage);
    
    DBGOUT (buffer);
    
    if (result)
        ::LocalFree(pMessage);
   
    va_end(args);
    return result;
}

// DBGOUT_LASTERROR
#ifdef _DEBUG
#define DBGOUT_LASTERROR ::DbgOutLastError
#else
#define DBGOUT_LASTERROR __noop
#endif



#endif // COMMON_DBGOUT_C_H_
