#pragma once

#include "LogManager.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class CGlobalClass
{
public:
	CGlobalClass(void);
	~CGlobalClass(void);

	void WndRegisterClass(HINSTANCE hInstance, LPCWSTR szClass);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, LPCWSTR szClass, LPCWSTR szWindowText);

	CLogManager *	m_pGlobalLogManager;
	HWND			m_hWnd;
};
