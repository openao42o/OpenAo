// File: DbgOut.h
//

#ifndef DBGOUT_H_D7274660_0C36_11d2_9253_0000C06932AE__INCLUDED_
#define DBGOUT_H_D7274660_0C36_11d2_9253_0000C06932AE__INCLUDED_

#include <TCHAR.h>

static  LPCTSTR    g_dbgOut                = _T("Field Server 20021017");
static  LPCTSTR    g_dbgOutwindowClassName = _T("Field DebugOut Window");


inline void DbgOutA (LPCSTR p)
{
    COPYDATASTRUCT cd;
    HWND hWnd = ::FindWindow (g_dbgOutwindowClassName, g_dbgOut);
    if (hWnd)
    {
        cd.dwData = 0;
        cd.cbData = (strlen(p)+1)*sizeof(char);
        cd.lpData = (void *)p;
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);
    }
	else
	{
		SetLastError(0);
	}
}


// DBGOUTA
#ifdef _DEBUG
#define DBGOUTA(p) DbgOutA(p);
#else
#define DBGOUTA __noop
#endif




inline void DbgOutW (LPCWSTR p)
{
    COPYDATASTRUCT cd;
    HWND hWnd = ::FindWindow (g_dbgOutwindowClassName, g_dbgOut);
    if (hWnd)
    {
        cd.dwData = 0xFEFF;
        cd.cbData = (wcslen(p)+1)*sizeof(wchar_t);
        cd.lpData = (void *)p;
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);
    }
	else
	{
		SetLastError(0);
	}
}

// DBGOUTW
#ifdef _DEBUG
#define DBGOUTW(p) DbgOutW(p);
#else
#define DBGOUTW __noop
#endif





inline void DbgOut (LPCTSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

    _TCHAR buffer [1024*sizeof(_TCHAR)];
	vsprintf(buffer, pFormat, args);

    #ifdef UNICODE
    DbgOutW (buffer);
    #else
    DbgOutA (buffer);
    #endif

    va_end(args);
}

// DBGOUT
#ifdef _DEBUG
#define DBGOUT ::DbgOut
#else
#define DBGOUT __noop
#endif



inline DWORD DbgOutLastError (LPCTSTR pFormat, ...)
{
   if (::GetLastError() == 0)
        return 0;

	va_list args;
	va_start(args, pFormat);

    _TCHAR buffer [1024*sizeof(_TCHAR)];
	vsprintf(buffer, pFormat, args);

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

    if(result)
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



#endif//  DBGOUT_H_D7274660_0C36_11d2_9253_0000C06932AE__INCLUDED_
