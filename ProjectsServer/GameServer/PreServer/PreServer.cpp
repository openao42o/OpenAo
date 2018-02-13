#include "stdafx.h"
#include "resource.h"
#include "PreGlobal.h"

#ifdef S_MINI_DUMP_HSKIM
LONG WINAPI ExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo);
#endif

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef S_MINI_DUMP_HSKIM
	SetUnhandledExceptionFilter(&ExceptionHandler);
#endif

	if (!PreGlobal::Initialize(hInstance, IDI_PRESERVER, nCmdShow)) return FALSE;

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	PreGlobal::Destroy();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
		if (!PreGlobal::Create(hWnd)) exit(0);
		break;

	case WM_COMMAND:
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_TIMER:
		switch (wParam)
		{
		case TIMERID_TRAFFIC:
			PreGlobal::OnTimerTraffic();
			break;

		case TIMERID_ALIVE_CHECK:
			PreGlobal::OnTimerPingAlive();
			break;

		default: break;
		}
		break;

	case WM_DESTROY:
		PreGlobal::DestroySocket();
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}

   return 0;
}

#ifdef S_MINI_DUMP_HSKIM

#include "dbgHelp.h"
#pragma comment(lib, "dbghelp.lib")

LONG WINAPI ExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{
	char fileName[MAX_PATH] { };

	strcpy(fileName, "dumps\\"); // cannot contain '.'
	
	{
		auto filelen = strlen(fileName);

		GetModuleFileName(NULL, fileName + filelen, sizeof(fileName) - filelen);
	}

	auto ext = strrchr(fileName, '.');

#ifdef S_SERVER_CRASH_FILENAME_ADD_INFO
	ATUM_DATE_TIME CurrentTime { true };
	sprintf(ext ? ext : fileName + strlen(fileName), "_%s.dmp", CurrentTime.GetFileDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
#else
	strcpy(ext ? ext : fileName + strlen(fileName), ".dmp");
#endif

	HANDLE hProcess = GetCurrentProcess();
	DWORD dwProcessID = GetCurrentProcessId();
	HANDLE hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId = GetCurrentThreadId();
	eInfo.ExceptionPointers = pExceptionInfo;
	eInfo.ClientPointers = FALSE;

	MiniDumpWriteDump(hProcess, dwProcessID, hFile, MiniDumpWithFullMemory, pExceptionInfo ? &eInfo : NULL, NULL, NULL);

	return EXCEPTION_EXECUTE_HANDLER;
}

#endif