// AtumLauncher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AtumLauncher.h"
#include "AtumLauncherDlg.h"
#include "MGameDecryption.h"
#include "AtumError.h"				// 2006-10-02 by cmkwon
#include "RegistryControl.h"	// 2008-10-16 by cmkwon, Gameforge4D(Eng,Deu) Launcher Registry event ±¸Çö - 

// 2012-10-21 by mspark, °ÔÀÓ Á¾·á ÈÄ ÇÁ·Î¼¼½º¿¡ ³²¾ÆÀÖ´Â ACEonline.atm Á¦°Å
#include <windows.h>
#include <stdlib.h>
#include <Tlhelp32.h>
// end 2012-10-21 by mspark, °ÔÀÓ Á¾·á ÈÄ ÇÁ·Î¼¼½º¿¡ ³²¾ÆÀÖ´Â ACEonline.atm Á¦°Å

#include "Xor_Lib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void WarriorChecks();

// check: MSG¸¦ printÇÏ´Â levelÀ» Á¶Á¤ÇÏ±â À§ÇØ(AtumMonitor·Î Á¶Àý °¡´É), by kelovon
//BYTE		g_exchangeMsgPrintLevel = PRINTLEVEL_NO_MSG;
//-- CMKWON 2003-07-12 Global.h¿Í Global.cpp¿¡¼­ Á¤ÀÇÇÔ

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherApp

BEGIN_MESSAGE_MAP(CAtumLauncherApp, CWinApp)
	//{{AFX_MSG_MAP(CAtumLauncherApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherApp construction

CAtumLauncherApp::CAtumLauncherApp()
:CWinApp(REGISTRY_BASE_PATH)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_hMutexMonoInstance	= NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAtumLauncherApp object

CAtumLauncherApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherApp initialization

BOOL CAtumLauncherApp::InitInstance()
{
#if	defined(_WARRIOR_ENABLE_VMVARE_CHECK)
	WarriorChecks();
#endif
	
#ifndef S_REMANING_PROCESS_REMOVE_MSPARK
	RemainingProcessRemove();
#endif
	

	m_hMutexMonoInstance = CreateMutex(NULL, TRUE, "Launcher");

	if (NULL == m_hMutexMonoInstance)
	{
		MessageBox(NULL, "CreateMutex Error", "Launcher", MB_OK);
		return FALSE;
	}

	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		MessageBox(NULL, "ERROR : \nNemerian Client already running.", "Nemerian", MB_OK);
		return FALSE;
	}

	// todo : deprecate srand calls and rand in general
	srand(timeGetTime());
	random::init();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	

	if (GSetExcuteParameterList(__argc, __argv)  != ERR_NO_ERROR)
	{
		return FALSE;
	}

	SEXCUTE_PARAMETER exeParam;
	util::zero(&exeParam, sizeof(exeParam));
	exeParam.i_nExcuteFileType = EXCUTE_FILE_TYPE_SC_LAUNCHER_ATM;

	if (GCheckExcuteParameterList(&exeParam) != ERR_NO_ERROR) return FALSE;
	
	
	CAtumLauncherDlg dlg;
	m_pMainWnd = &dlg;

	ReadProfile(dlg);

	if (exeParam.o_szPreServerIP0[0] != '\0') dlg.m_szPreServerIPReg = exeParam.o_szPreServerIP0;
	

	dlg.m_nBirthYear			= exeParam.o_nBirthYear;	
	dlg.m_ctlbWindowMode		= (dlg.m_nWindowModeReg == GAME_MODE_WINDOW) ? TRUE : FALSE;	// 2008-01-08 by cmkwon, Ã¢ ¸ðµå ¹ö±× ¼öÁ¤



	int nResponse = dlg.DoModal();
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

void CAtumLauncherApp::WriteProfile(CAtumLauncherDlg& instance)
{
		instance.m_szAccountNameReg.MakeLower();


		SREG_DATA_EXE_2 regDataExe2;
		regDataExe2.resetREG_DATA_EXE_2();
		regDataExe2.ClientVersion		= instance.m_CurrentVersion;
		regDataExe2.NVersion			= instance.m_CurrentNoticeVersion;
		regDataExe2.DVersion			= instance.m_CurrentDelFileListVersion;

		util::strncpy(regDataExe2.WindowDegree, LPCSTR(instance.m_csWindowsResolutionReg));	// 2007-12-27 by cmkwon, À©µµ¿ìÁî ¸ðµå ±â´É Ãß°¡ -
	
		if (instance.m_ctlBtnRememberID.GetCheck())
			
			util::strncpy(regDataExe2.BeforeAccountName, instance.m_szAccountNameReg);

		util::strncpy(regDataExe2.SelectedServerGroupName, LPCSTR(instance.m_strServerGroupName));


		regDataExe2.IsWindowMode		= instance.m_ctlbWindowMode;


		// 2015-12-16 Future, Vsync setting
		regDataExe2.VSync = instance.m_bVSync;

		GSaveExe2VersionInfo(&regDataExe2, STRMSG_VERSION_INFO_FILE_NAME);
}

void CAtumLauncherApp::ReadProfile(CAtumLauncherDlg& instance)
{
	SREG_DATA_EXE_2 regDataExe2;

	regDataExe2.resetREG_DATA_EXE_2();

	GLoadExe2VersionInfo(&regDataExe2, STRMSG_VERSION_INFO_FILE_NAME);
		

	instance.m_CurrentVersion				= regDataExe2.ClientVersion;
	instance.m_csWindowsResolutionReg		= regDataExe2.WindowDegree;		// 2007-12-27 by cmkwon, À©µµ¿ìÁî ¸ðµå ±â´É Ãß°¡ -
	instance.m_szAccountNameReg			= regDataExe2.BeforeAccountName;
	instance.m_strServerGroupName			= regDataExe2.SelectedServerGroupName;
	instance.m_CurrentDelFileListVersion	= regDataExe2.DVersion;			// 2007-05-09 by cmkwon, DeleteFileList VersionÀº Ç×»ó 0.0.0.0
	instance.m_CurrentNoticeVersion		= regDataExe2.NVersion;			// 2007-05-09 by cmkwon, NoticeVersionÀº »ç¿ëÇÏÁö ¾ÊÀ½


	if (regDataExe2.IsWindowMode)
			
		instance.m_nWindowModeReg = GAME_MODE_WINDOW;
		
	else instance.m_nWindowModeReg = GAME_MODE_FULLSCREEN;
		

	// 2015-12-16 Future, Vsync setting
	instance.m_bVSync = regDataExe2.VSync;

#ifdef ARESBETA
	char szPreServer[1024];
	util::zero(szPreServer, 1024);
	XOR::XORDecrypt(szPreServer, CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR, STR_XOR_KEY_STRING_PRE_SERVER_ADDRESS);
	instance.m_szPreServerIPReg	= szPreServer;
#else
	instance.m_szPreServerIPReg = "warrnet.net";
#endif
}

int CAtumLauncherApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_hMutexMonoInstance)
	{
		::CloseHandle(m_hMutexMonoInstance);
		m_hMutexMonoInstance = NULL;
	}
	
	return CWinApp::ExitInstance();
}

// 2012-10-21 by mspark, °ÔÀÓ Á¾·á ÈÄ ÇÁ·Î¼¼½º¿¡ ³²¾ÆÀÖ´Â ACEonline.atm Á¦°Å
void CAtumLauncherApp::RemainingProcessRemove()
{
	DWORD dwSize = 250;
	HANDLE hSnapShot;
	PROCESSENTRY32 pEntry;
	BOOL bCrrent = FALSE;
	
	
	hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);
	
	pEntry.dwSize =sizeof(PROCESSENTRY32);
	// ½ÇÇàÁßÀÎ ÇÁ·Î¼¼½ºµéÀÇ Ã¹¹øÀç Á¤º¸¸¦ °¡Á®¿Â´Ù.
	Process32First (hSnapShot,&pEntry);
	
	// ToolÀÌ ½ÇÇàÁßÀÎÁö È®ÀÎ
	int nCnt = 0;
	BOOL bRes = TRUE;

	for(nCnt = 0; nCnt < 1000; nCnt++)
	{		
		if(bRes==FALSE)
			break;        
		if (IsWindowsVersionOrGreater(6, 0, 0))
		{
			if (!strncmp(pEntry.szExeFile, CLIENT_EXEUTE_FILE_NAME, 15))
			{
				bCrrent = TRUE;
			}
		}
		else
		{
			if (!strncmp(pEntry.szExeFile, CLIENT_EXEUTE_FILE_NAME_XP, 17))
			{
				bCrrent = TRUE;
			}
		}
		
		if(bCrrent)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pEntry.th32ProcessID);
			if(hProcess)
			{
				if(TerminateProcess(hProcess, 0))
				{
					unsigned long nCode; //ÇÁ·Î¼¼½º Á¾·á »óÅÂ
					GetExitCodeProcess(hProcess, &nCode);
				}
				CloseHandle(hProcess);
			}
			break;
		} 
		// ´ÙÀ½¹ø ÇÁ·Î¼¼½ºÀÇ Á¤º¸¸¦ °¡Á®¿Â´Ù.
		bRes=Process32Next (hSnapShot,&pEntry);
	}
}
// end 2012-10-21 by mspark, °ÔÀÓ Á¾·á ÈÄ ÇÁ·Î¼¼½º¿¡ ³²¾ÆÀÖ´Â ACEonline.atm Á¦°Å

#if	defined(_WARRIOR_ENABLE_VMVARE_CHECK)
char* sModule[] = { "SbieDll.dll"/*, "dbghelp.dll"*/ };   // todo : review, this was giving errors to strah

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

	__asm {MemBpBeingDebugged:}
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

void WarriorChecks()
{
	//2015-03-03 for anti cheat
	if (IsInSandbox())
	{
		MessageBox(NULL, "Using Sandboxie is not allowed! \nPlease run Nemerian outside of Sandboxie.", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
	}
	if (IsInsideVPC())
	{
		MessageBox(NULL, "Using VirtualPC is not allowed! \n Please run Nemerian outside of VirtualPC.", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
	}

	if (IsInsideVMWare())
	{
		MessageBox(NULL, "Using VMWare is not allowed! \nPlease run Nemerian outside of VMWare", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
	}
	if (MemoryBreakpointDebuggerCheck())
	{
		MessageBox(NULL, "Cheat Engine not allowed! \n Remove Cheat Engine, Restart Computer and Login again.", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
	}
	if (CheckHardwareBreakpoints() != 0)
	{
		MessageBox(NULL, "Debug software detected! \n Close your debug software and Login Again.", "Nemerian Anti-Cheat", MB_OK);
		exit(1);
	}
	/*HKEY hSubKey = NULL;
	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Cheat Engine", 0L, KEY_ALL_ACCESS, &hSubKey))
	{// warriorsw checking cheat engine
	MessageBox(NULL, "Cheat Engine not allowed! \n Remove Cheat Engine, Restart Computer and Login again.", "Ares Anti-Cheat", MB_OK);
	exit(1);
	return E_FAIL;
	}
	//endof 2015-03-03*/
	TCHAR buffer[MAX_PATH];
	TCHAR * out;
	DWORD bufSize = sizeof(buffer) / sizeof(*buffer);

	// Get the fully-qualified path of the executable
	if (GetModuleFileName(NULL, buffer, bufSize) == bufSize)
	{
	}

	out = PathFindFileName(buffer);

	string filenameS = string(out);
	if (strcmp(out, "Launcher.atm") != 0) exit(0);

	*(PathFindExtension(out)) = 0;
}

#endif