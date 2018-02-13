// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

//######################################################################################
// File    : NTDbgHelperWindows.h
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Create Window
// Bug     : 
//######################################################################################
#ifndef __NEXT_DBGHELPER_WINDOW__
#define __NEXT_DBGHELPER_WINDOW__

#include <Windows.h>
#include <tchar.h>
#include <string>

#ifdef  UNICODE                     // r_winnt
typedef std::wstring TString;
#else
typedef std::string TString;
#endif//(UNICODE)


#define IDC_DUMPSEND		101
#define IDC_DUMPCANCEL		102
#define IDC_DUMPEDITBOX		103
#define IDC_PROGRESS		104
#define IDC_ERROREDITBOX	105
#define IDC_MOREINFO		106
#define IDC_MAININFO		107

class CNTSystemInfo;
struct _EXCEPTION_POINTERS;

class CNTDbgHelperWindow
{
public:
	CNTDbgHelperWindow();
	~CNTDbgHelperWindow();

	void OnCreateWindow(void);
	void SetMoreInfo(_EXCEPTION_POINTERS *pException, CNTSystemInfo* pSystemInfo);
	void GetUserString(TCHAR *pszString, int nLen);
	void GetSystemString(TCHAR *pszString, int nLen);
	UINT GetEvent(void);
	HWND GetProgressHWND(void)		{ return m_hProgresshWnd; }
	HWND GetUserEditBoxHWND(void)	{ return m_hUserEdithWnd; }
	HWND GetSendEditBoxHWND(void)	{ return m_hSendEdithWnd; }
	void CleanUp(void);

	void OnCreateControl(void);
	void OnBaseWindows(void);
	void OnMoreInfoWindows(void);


private:
	static	LRESULT CALLBACK CNTDbgHelperWindow::WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	HBITMAP m_Image;
	HWND m_Mainhwnd;
	HWND m_Morehwnd;
	HWND m_hUserEdithWnd;
	HWND m_hSendEdithWnd;
	HWND m_hProgresshWnd;
	HWND m_hMainProgresshWnd;
	HWND m_hMoreProgresshWnd;
	UINT m_nResult;
	bool m_bMainWindow;
};

#endif //(__NEXT_DBGHELPER_WINDOW__)

// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송