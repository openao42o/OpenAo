// Atum.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Atum.h"
#include "AtumDlg.h"
#include "MGameDecryption.h"
#include "AtumError.h"
#include "AtumParam.h"			// 2007-09-01 by cmkwon, 웹런처(WebLauncher) 시스템 수정 -
#include "RegistryControl.h"	// 2007-09-01 by cmkwon, 웹런처(WebLauncher) 시스템 수정 -

#include <Shlwapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAtumApp

BEGIN_MESSAGE_MAP(CAtumApp, CWinApp)
	//{{AFX_MSG_MAP(CAtumApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumApp construction

CAtumApp::CAtumApp()
:CWinApp(REGISTRY_BASE_PATH)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAtumApp object

CAtumApp theApp;
#ifdef _WARRIOR_ENABLE_VMVARE_CHECK
char* sModule[] = { "SbieDll.dll"/*, "dbghelp.dll"*/ };		// todo : review, this was giving errors to strah

bool IsInSandbox()
{
	for (auto i = 0; i < (sizeof(sModule) / sizeof(char*)); i++)
	{
		auto handle = GetModuleHandle(sModule[i]);

		if (handle)
		{
			char buffer[128];

			GetModuleFileName(handle, buffer, 128);

			MessageBox(NULL, buffer, "Module location", NULL);

			return true;
		}
	}

	return false;
}

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
		_asm setz[rc]
			_asm pop ebx
	}
	// The except block shouldn't get triggered if VPC is running!!
	__except (IsInsideVPC_exceptionFilter(GetExceptionInformation()))
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
				setz[rc] // set return value

				pop    ebx
				pop    ecx
				pop    edx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
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
	if (GetThreadContext(hThread, &ctx) == 0)
		return -1;

	// Now we can check for hardware breakpoints, its not 
	// necessary to check Dr6 and Dr7, however feel free to
	if (ctx.Dr0 != 0)
		++NumBps;
	if (ctx.Dr1 != 0)
		++NumBps;
	if (ctx.Dr2 != 0)
		++NumBps;
	if (ctx.Dr3 != 0)
		++NumBps;

	return NumBps;
}
bool MemoryBreakpointDebuggerCheck()
{
	unsigned char *pMem = NULL;
	SYSTEM_INFO sysinfo = { 0 };
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
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// The exception occured and no debugger was detected
		VirtualFree(pAllocation, NULL, MEM_RELEASE);
		return false;
	}

	__asm{MemBpBeingDebugged:}
	VirtualFree(pAllocation, NULL, MEM_RELEASE);
	return true;
}

bool CheckForCCBreakpointXor55(void* pMemory, size_t SizeToCheck)
{
	unsigned char *pTmp = (unsigned char*)pMemory;
	unsigned char tmpchar = 0;

	for (size_t i = 0; i < SizeToCheck; i++)
	{
		tmpchar = pTmp[i];
		if (0x99 == (tmpchar ^ 0x55)) // 0xCC xor 0x55 = 0x99
			return true;
	}

	return false;
}

#endif
/////////////////////////////////////////////////////////////////////////////
// CAtumApp initialization
#pragma comment(lib, "Shlwapi.lib")
BOOL CAtumApp::InitInstance()
{
#ifdef _WARRIOR_ENABLE_VMVARE_CHECK
	//2015-03-03 for anti cheat
	if (IsInSandbox())
	{
		MessageBox(NULL, "Using Sandboxie is not allowed! \nPlease run Nemerian outside of Sandboxie.", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
		return E_FAIL;
	}
	if (IsInsideVPC())
	{
		MessageBox(NULL, "Using VirtualPC is not allowed! \n Please run Nemerian outside of VirtualPC.", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
		return E_FAIL;
	}

	if (IsInsideVMWare())
	{
		MessageBox(NULL, "Using VMWare is not allowed! \nPlease run Nemerian outside of VMWare", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
		return E_FAIL;
	}
	if (MemoryBreakpointDebuggerCheck())
	{
		MessageBox(NULL, "Cheat Engine not allowed! \n Remove Cheat Engine, Restart Computer and Login again.", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
		return E_FAIL;
	}
	if (CheckHardwareBreakpoints() != 0)
	{
		MessageBox(NULL, "Debug software detected! \n Close your debug software and Login Again.", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
		return E_FAIL;
	}
#endif

	InitCommonControls();    // 2008-03-03 by cmkwon, manifest 적용 - initialize common control library
	//here
	//w8
	TCHAR buffer[MAX_PATH];
	TCHAR * out;
	DWORD bufSize = sizeof(buffer) / sizeof(*buffer);
	// Get the fully-qualified path of the executable
	if (GetModuleFileName(NULL, buffer, bufSize) == bufSize)
	{
	}
	out = PathFindFileName(buffer);
	string filenameS = string(out);

	if (strcmp(out, "AceOfNemerian.exe") != 0) exit(0);
	
	*(PathFindExtension(out)) = 0;

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-02 by cmkwon
	Err_t errCode = GSetExcuteParameterList(__argc, __argv);
	if(ERR_NO_ERROR != errCode)
	{
		DbgOut("Set Excute Parameter error !! Error = %s(0x%X)\r\n", GetErrorString(errCode), errCode);
		ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
		return FALSE;
	}

	SEXCUTE_PARAMETER exeParam;		util::zero(&exeParam, sizeof(exeParam));
	exeParam.i_nExcuteFileType		= EXCUTE_FILE_TYPE_SC_EXE;
	errCode = GCheckExcuteParameterList(&exeParam);
	if(ERR_NO_ERROR != errCode)
	{
		DbgOut("Check Excute Parameter error !! Error = %s(0x%X)\r\n", GetErrorString(errCode), errCode);
		ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
		return FALSE;
	}
	//
	///////////////////////////////////////////////////////////////////////////////
	

	CAtumDlg dlg;
	m_pMainWnd = &dlg;

	if (exeParam.o_szPreServerIP0[0] != '\0') dlg.m_szMainPreServerIP = exeParam.o_szPreServerIP0;

	ReadProfile(dlg);


	auto nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CAtumApp::WriteProfile(CAtumDlg& instance)
{

	GSaveExe1VersionInfo(&instance.m_CurrentLauncherVersion, STRMSG_VERSION_INFO_FILE_NAME);
}

void CAtumApp::ReadProfile(CAtumDlg& instance)
{

	VersionInfo lVer;
	GLoadExe1VersionInfo(&lVer, STRMSG_VERSION_INFO_FILE_NAME);
	instance.m_CurrentLauncherVersion = lVer;
}