#include "GlobalClass.h"

CGlobalClass::CGlobalClass(void)
{
	m_pGlobalLogManager = new CLogManager;
}

CGlobalClass::~CGlobalClass(void)
{
	util::del(m_pGlobalLogManager);
}


void CGlobalClass::WndRegisterClass(HINSTANCE hInstance, LPCWSTR szClass)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szClass;
	wcex.hIconSm		= NULL;

	::RegisterClassEx(&wcex);
}

BOOL CGlobalClass::InitInstance(HINSTANCE hInstance, int nCmdShow, LPCWSTR szClass, LPCWSTR szWindowText)
{

	m_pGlobalLogManager->InitLogManger(TRUE, szClass);

	m_hWnd = CreateWindow(szClass, szWindowText, WS_OVERLAPPEDWINDOW,
		350, 150, 300, 120, NULL, NULL, hInstance, NULL);

	m_pGlobalLogManager->WriteSystemLog(L"============== Server API Create !!! ==============\r\n",TRUE);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}
