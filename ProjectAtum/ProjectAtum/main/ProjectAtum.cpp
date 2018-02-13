// ProjectAtum.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "AtumApplication.h"
#include "MGameDecryption.h"
#include <ShuttleChild.h>

// 2010-09-29 by jskim, 덤프 남기도록 수정 
#include "dbgHelp.h"
#pragma comment(lib, "dbghelp.lib")

#include "CustomOptimizer.h"

#include <Shlwapi.h>
 #define  SET_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))


static BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	if (hKernel32 == NULL) return FALSE;
	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if (pOrgEntry == NULL) return FALSE;

#ifdef _M_IX86
	// Code for x86:
	// 33 C0                xor         eax,eax  
	// C2 04 00             ret         4 
	unsigned char szExecute[] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };
#elif _M_X64
	// 33 C0                xor         eax,eax 
	// C3                   ret  
	unsigned char szExecute[] = { 0x33, 0xC0, 0xC3 };
#else
#error "The following code only works for x86 and x64!"
#endif

	SIZE_T bytesWritten = 0;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
		pOrgEntry, szExecute, sizeof(szExecute), &bytesWritten);
	return bRet;
}


LONG __stdcall Exception_Minidump(_EXCEPTION_POINTERS* pExceptionInfo)
{
    char fileName[MAX_PATH];
    GetModuleFileName(NULL, fileName, sizeof(fileName));
    char* ext = strrchr(fileName, '.');
    strcpy(ext ? ext : fileName+strlen(fileName), ".dmp");

#ifndef _CRASH_HANDLER
    char temp[256];
    wsprintf(temp, "An unknown error occured! Please report this crash to the Ares Staff.\nOn request upload your: %s, so the developers can fix this issue!\nErrorcode: 0x%08x",fileName, pExceptionInfo->ExceptionRecord->ExceptionCode); //2015-03-04 by St0rmy, changed exception text
    MessageBox(NULL, temp, "[Ares] Unknown Error!", MB_OK);//2015-03-04 by St0rmy, changed msgbox title
#endif

    HANDLE hProcess = GetCurrentProcess();
    DWORD dwProcessID = GetCurrentProcessId();
    HANDLE hFile = CreateFile(fileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId = GetCurrentThreadId();
    eInfo.ExceptionPointers = pExceptionInfo;
    eInfo.ClientPointers = FALSE;

#ifdef MEM_DEBUG
	MiniDumpWriteDump(hProcess, dwProcessID, hFile, MiniDumpWithFullMemory, pExceptionInfo ? &eInfo : NULL, NULL, NULL);
#else
	MiniDumpWriteDump(hProcess, dwProcessID, hFile, MiniDumpNormal, pExceptionInfo ? &eInfo : NULL, NULL, NULL);
#endif // MEM_DEBUG


#ifdef _CRASH_HANDLER
#ifdef _WIN_XP
	char buff[512];
	sprintf(buff, "CrashReporter.exe -code 0x%08x -accountname %s -mapnumber %d -character %s -dmpfile aceonlin3.dmp", pExceptionInfo->ExceptionRecord->ExceptionCode, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.AccountName, g_pD3dApp->GetMyShuttleMapChannelIndex().MapIndex, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName);
#else
	char buff[512];
	sprintf(buff, "CrashReporter.exe -code 0x%08x -accountname %s -mapnumber %d -character %s -dmpfile aceonline.dmp", pExceptionInfo->ExceptionRecord->ExceptionCode, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.AccountName, g_pD3dApp->GetMyShuttleMapChannelIndex().MapIndex, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName);
#endif

	DWORD dwExitCode;
	PROCESS_INFORMATION pi;

	STARTUPINFO si = { sizeof(si) };
	ZeroMemory(&si, sizeof(si));

	CreateProcess(NULL, buff, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
#endif


    return EXCEPTION_EXECUTE_HANDLER;
}
// end 2010-09-29 by jskim, 덤프 남기도록 수정 
HANDLE g_hMutexMonoInstance = NULL;
HANDLE g_hMutexMonoInstanceTemp = NULL;
#if defined(_WARRIOR_ENABLE_OLD_ANTICHEAT)
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	char title[80];
	std::string str(title);
	//Improved by st0rmy (its also now dynamic)
	const string entries[] = { "Cheat Engine", "ArtMoney", "AI Robot", "Macro" };
	GetWindowText(hwnd, title, sizeof(title));
	str = title;
	for (int i = 0; i < sizeof(entries) / sizeof(*entries); i++)
	{
		if(str.find(entries[i]) != std::string::npos)
			return FALSE;
	}
	return TRUE;
}
#endif
#if defined(_WARRIOR_ENABLE_VMVARE_CHECK)
DWORD __forceinline IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)
{
  PCONTEXT ctx = ep->ContextRecord;

  ctx->Ebx = -1; // Not running VPC
  ctx->Eip += 4; // skip past the "call VPC" opcodes
  return EXCEPTION_CONTINUE_EXECUTION;
  // we can safely resume execution since we skipped faulty instruction
}

// High level language friendly version of IsInsideVPC()
bool IsInsideVPC()
{
  bool rc = false;

  __try
  {
    _asm push ebx
    _asm mov  ebx, 0 // It will stay ZERO if VPC is running
    _asm mov  eax, 1 // VPC function number

    // call VPC 
    _asm __emit 0Fh
    _asm __emit 3Fh
    _asm __emit 07h
    _asm __emit 0Bh

    _asm test ebx, ebx
    _asm setz [rc]
    _asm pop ebx
  }
  // The except block shouldn't get triggered if VPC is running!!
  __except(IsInsideVPC_exceptionFilter(GetExceptionInformation()))
  {
  }

  return rc;
}

bool IsInsideVMWare()
{
  bool rc = true;

  __try
  {
    __asm
    {
      push   edx
      push   ecx
      push   ebx

      mov    eax, 'VMXh'
      mov    ebx, 0 // any value but not the MAGIC VALUE
      mov    ecx, 10 // get VMWare version
      mov    edx, 'VX' // port number

      in     eax, dx // read port
                     // on return EAX returns the VERSION
      cmp    ebx, 'VMXh' // is it a reply from VMWare?
      setz   [rc] // set return value

      pop    ebx
      pop    ecx
      pop    edx
    }
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
    rc = false;
  }

  return rc;
}

int CheckHardwareBreakpoints()
{
    unsigned int NumBps = 0;

    // This structure is key to the function and is the 
    // medium for detection and removal
    CONTEXT ctx;
    ZeroMemory(&ctx, sizeof(CONTEXT)); 
    
    // The CONTEXT structure is an in/out parameter therefore we have
    // to set the flags so Get/SetThreadContext knows what to set or get.
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS; 
    
    // Get a handle to our thread
    HANDLE hThread = GetCurrentThread();

    // Get the registers
    if(GetThreadContext(hThread, &ctx) == 0)
        return -1;

    // Now we can check for hardware breakpoints, its not 
    // necessary to check Dr6 and Dr7, however feel free to
    if(ctx.Dr0 != 0)
        ++NumBps; 
    if(ctx.Dr1 != 0)
           ++NumBps; 
    if(ctx.Dr2 != 0)
           ++NumBps; 
    if(ctx.Dr3 != 0)
        ++NumBps;
        
    return NumBps;
}
bool MemoryBreakpointDebuggerCheck()
{
     unsigned char *pMem = NULL;
     SYSTEM_INFO sysinfo = {0}; 
     DWORD OldProtect = 0;
     void *pAllocation = NULL; // Get the page size for the system 
 
    GetSystemInfo(&sysinfo); // Allocate memory 
 
    pAllocation = VirtualAlloc(NULL, sysinfo.dwPageSize, 
                        MEM_COMMIT | MEM_RESERVE, 
                         PAGE_EXECUTE_READWRITE); 
        
    if (pAllocation == NULL)
        return false; 
    
    // Write a ret to the buffer (opcode 0xc3)
    pMem = (unsigned char*)pAllocation;
    *pMem = 0xc3; 
    
    // Make the page a guard page         
    if (VirtualProtect(pAllocation, sysinfo.dwPageSize, 
                    PAGE_EXECUTE_READWRITE | PAGE_GUARD, 
                    &OldProtect) == 0)
    {
        return false;
    } 
    
    __try
    {
        __asm
        {
            mov eax, pAllocation
            // This is the address we'll return to if we're under a debugger
            push MemBpBeingDebugged
            jmp eax // Exception or execution, which shall it be :D?
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        // The exception occured and no debugger was detected
        VirtualFree(pAllocation, NULL, MEM_RELEASE);
        return false;
    }     
    
    __asm{MemBpBeingDebugged:}
    VirtualFree(pAllocation, NULL, MEM_RELEASE);
    return true;
}

bool CheckForCCBreakpointXor55(void* pMemory,  size_t SizeToCheck)
 {
     unsigned char *pTmp = (unsigned char*)pMemory;
    unsigned char tmpchar = 0;
        
    for (size_t i = 0; i < SizeToCheck; i++)
     {
        tmpchar = pTmp[i];
        if( 0x99 == (tmpchar ^ 0x55) ) // 0xCC xor 0x55 = 0x99
            return true;
     } 

    return false;
 }
inline bool Int2DCheck()
{
    __try
    {
        __asm
        {
            int 0x2d
            xor eax, eax
            add eax, 2
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        return false;
    }
    
    return true;
}

inline void ErasePEHeaderFromMemory()
{
    DWORD OldProtect = 0;
    
    // Get base address of module
    char *pBaseAddr = (char*)GetModuleHandle(NULL);

    // Change memory protection
    VirtualProtect(pBaseAddr, 4096, // Assume x86 page size
            PAGE_READWRITE, &OldProtect);

    // Erase the header
    ZeroMemory(pBaseAddr, 4096);
}

#endif
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	//Sleep(15000); // DevX added for easier debugger attachment

	CAtumApplication pD3dApp;
#ifdef _WARRIOR_ENABLE_OLD_ANTICHEAT
	BOOL chk = EnumWindows(EnumWindowsProc, NULL);

	if(chk == FALSE) {
			MessageBox(NULL, "Some functions of Ares has been corrupted. \nPlease close any programs (Macro, Cheat, Bot) which is affecting Ares.\nCheck http://aresonline.in for more detail", "Ares Anti-Cheat", MB_OK);
			exit(1);
	} 
#endif
#ifdef WARRIOR_ENABLE_VMVARE_CHECK
		 if(IsDebuggerPresent())
		{
			MessageBox(NULL, "Debug software detected! \nClose any debug software and Try again.", "Ares Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
		if(IsInsideVPC())
		{
			MessageBox(NULL, "Using VirtualPC is not allowed! \nPlease run Ares outside of VirtualPC.", "Ares Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
		
		if(IsInsideVMWare())
		{
			MessageBox(NULL, "Using VMWare is not allowed! \nPlease run Ares outside of VMWare", "Ares Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}

		if(MemoryBreakpointDebuggerCheck())
		{
			MessageBox(NULL, "Cheat Engine not allowed! \nRemove Cheat Engine, Restart Computer and Login again.", "Ares Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}

		if(CheckHardwareBreakpoints() != 0)
		{
			MessageBox(NULL, "Debug software detected! \nClose any debug software and Try again.", "Ares Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
#endif

#ifdef _WIN_XP
		TCHAR buffer[MAX_PATH];
		TCHAR * out;
		DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
		// Get the fully-qualified path of the executable
		if (GetModuleFileName(NULL, buffer, bufSize) == bufSize)
		{
		}
		out = PathFindFileName(buffer);
		string filenameS = string(out);
		if (strcmp(out, "ACEonlin3.atm") != 0)
		{
			//MessageBox(NULL, "Please restore the original file name!", "Error", MB_OK);
			exit(0);
		}
		*(PathFindExtension(out)) = 0;
#else
		TCHAR buffer[MAX_PATH];
		TCHAR * out;
		DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
		// Get the fully-qualified path of the executable
		if (GetModuleFileName(NULL, buffer, bufSize) == bufSize)
		{
		}
		out = PathFindFileName(buffer);
		string filenameS = string(out);
		if (strcmp(out, "game.dat") != 0)
		{
			//MessageBox(NULL, "Please restore the original file name!", "Error", MB_OK);
			exit(0);
		}
		*(PathFindExtension(out)) = 0;
#endif

	pD3dApp.b_CanadaMutex = true;// 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전
	g_cCustomOptimizer.AutoLauncher();

	// 2013-05-03 by bhsohn 핵쉴드 Dump 안나오는 현상 처리
	// 2010-09-29 by jskim, 덤프 남기도록 수정 
//	SetUnhandledExceptionFilter(Exception_Minidump);
	// end 2010-09-29 by jskim, 덤프 남기도록 수정 
	if(GAMEGUARD_TYPE != USE_GAMEGUARD_AHNLAB_HACKSHIELD)
	{		
		SetUnhandledExceptionFilter(Exception_Minidump);		
	}
	// END 2013-05-03 by bhsohn 핵쉴드 Dump 안나오는 현상 처리


	// 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전
#ifdef C_CANADA_HACKSHEILD_JHAHN
	{
		HANDLE hMutexTemp = NULL;	
			
		hMutexTemp = ::OpenMutex(MUTEX_ALL_ACCESS, TRUE, "CANADA_HACK_SHIELD");
		if(hMutexTemp == NULL)
		{
			pD3dApp.b_CanadaMutex = true;
		}	
		else
		{
			pD3dApp.b_CanadaMutex = false;
		}
		
		
		CloseHandle(hMutexTemp);
		//end 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전
		
	}
#endif
	
	
	if(g_hMutexMonoInstance)
		return 0;

	g_hMutexMonoInstance = CreateMutex(NULL, TRUE, WINDOWTEXT_NAME_CLIENT);

	if(NULL == g_hMutexMonoInstance)
		return 0;

#ifdef MULTI_LOADER_HSSON

#else // MULTI_LOADER_HSSON
	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		MessageBox(NULL, "ERROR : \nApplication is running already...", WINDOWTEXT_NAME_CLIENT, MB_OK);
		return 0;
	}
#endif // MULTI_LOADER_HSSON

	// Validation of command line arguments
	if (__argc != 13)	// 2015-12-16 Future, Vsync setting
	{
		DBGOUT("[Error] Parameter Count Error, Count(%d)(%s)\n", __argc, lpCmdLine);
		MessageBox(NULL, "Some functions of Ares has been corrupted. \nPlease Reinstall the Client.\nCode for support Admin : 0xc6004!", "Ares", MB_OK);
		return FALSE;
	}

	// Reading of comman line arguments
	sscanf(lpCmdLine, "%s %d %s %d %s %s %d %d %d %d %d %d", pD3dApp.m_strFieldIP, &pD3dApp.m_nFieldPort, pD3dApp.m_strChatIP, &pD3dApp.m_nChatPort, pD3dApp.m_strUserID,
		pD3dApp.m_strUserPassword, &pD3dApp.m_IsFullMode, &pD3dApp.m_nWidth, &pD3dApp.m_nHeight, &pD3dApp.m_bDegree, &pD3dApp.m_bMovePlayer, &pD3dApp.m_bVSync);

#ifdef ONLY_FULL_WINDOW_HSSON
		pD3dApp.m_IsFullMode = TRUE;
#endif // ONLY_FULL_WINDOW_HSSON

// 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전

		if (pD3dApp.b_CanadaMutex)
		{
			// 2009-01-22 by bhsohn Xign Code시, Sleep(3000)추가
			if(!pD3dApp.StartGameGuard())
			{
				pD3dApp.CloseGameGuard();
				return FALSE;
			}		
			// end 2009-01-22 by bhsohn Xign Code시, Sleep(3000)추가
		}
		// 2013-05-03 by bhsohn 핵쉴드 Dump 안나오는 현상 처리		
		if(GAMEGUARD_TYPE == USE_GAMEGUARD_AHNLAB_HACKSHIELD)
		{		
			SetUnhandledExceptionFilter(Exception_Minidump);
#ifdef _ENHANCED_EXCEPTIONS
			if(PreventSetUnhandledExceptionFilter())
				DBGOUT("PreventSetUnhandledExceptionFilter() success!");
#endif
		}

	DBGOUT("FullMode = %d\n",pD3dApp.m_IsFullMode);
	if( FAILED( pD3dApp.Create( hInstance ) ) )
		return 0;

	if(!pD3dApp.SetGameGuardHWND())
	{
		pD3dApp.CloseGameGuard();
		return FALSE;
	}	

	g_input.InitInput();

	// 2015-10-16 Future, pass mono instance handle to the main app
	pD3dApp.SetMonoMutexInstance(g_hMutexMonoInstance);

	INT nResult;

	nResult = pD3dApp.Run(); //Run Main App

	pD3dApp.CloseGameGuard();

	// 2015-10-16 Future, Destroy Mutex if not destroyed before
	if (pD3dApp.GetMonoMutexInstance())
		CloseHandle(g_hMutexMonoInstance);

	if(pD3dApp.m_bShutDown && strlen(pD3dApp.m_strMsgLastError))
	{
		MessageBox(NULL, pD3dApp.m_strMsgLastError,STRMSG_WINDOW_TEXT, MB_OK);
	}

	return nResult;
}
