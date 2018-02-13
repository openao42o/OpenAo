// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

//######################################################################################
// File    : NTDbgHelperWindows.cpp
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Create Window
// Bug     : 
//######################################################################################
#include "NTDbgHelperWindow.h"
#include "NTSystemInfo.h"
#include "NTExceptionCause.h"
#include <commctrl.h>
#include <dbghelp.h>
#include <stdio.h>

#define WINCLASSNAME	_T("DbgHelperWindow")
#define WINMAINTITLE	_T("DbgHelperApplication")
#define WINMORETITLE	_T("DbgHelperMoreInfo")
#define WIN_WIDTH		450+6
#define WIN_HEIGHT		300+25
#define WIN_MOREHEIGHT	600+25
#define WIN_BGIMAGE		_T("Window.bmp")

CNTDbgHelperWindow *m_pDbgHelperWindow = NULL;

#define MAIN_MAINSTRING _T("문제가 발생하여 프로그램을 종료해야 합니다. \r\n \
불편을 끼쳐드려 죄송합니다.")

#define MAIN_SUBSTRING _T("이 문제에 대해서 오류 보고를 작성했습니다. \r\n \
오류를 전송해 주시면 이 문제를 해결하는데 많은  \r\n 도움이 될 것입니다.  \r\n   \r\n \
이 내용은 기밀로 간주되며 익명으로관리 합니다.")

#define MAIN_MORESUBSTRING1 _T("이 문제에 대해서 의견을 적어주시면 문제를 해결하는데 \r\n \
많은 도움이 될 것입니다.")

#define MAIN_MORESUBSTRING2 _T("사용자 작업에 관한 다음의 내용을 보고합니다.")


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 생성자
//######################################################################################
CNTDbgHelperWindow::CNTDbgHelperWindow() :
m_Mainhwnd( NULL ),
m_Morehwnd( NULL ),
m_Image( NULL ),
m_hProgresshWnd( NULL ),
m_nResult( 0 ),
m_bMainWindow( true ),
m_hUserEdithWnd( NULL ),
m_hSendEdithWnd( NULL )
{

	m_pDbgHelperWindow = this;
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 소멸자
//######################################################################################
CNTDbgHelperWindow::~CNTDbgHelperWindow()
{
	CleanUp();
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 메모리 해제
//######################################################################################
void CNTDbgHelperWindow::CleanUp()
{
	if( NULL != m_Image )
	{
		DeleteObject(m_Image);
	}
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : Window 생성
//######################################################################################
void CNTDbgHelperWindow::OnCreateWindow()
{
	OnCreateControl();
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : SystemInfo Editbox에 설정
//######################################################################################
void CNTDbgHelperWindow::SetMoreInfo(_EXCEPTION_POINTERS *pException, CNTSystemInfo* pSystemInfo)
{
	//if( m_hUserEdithWnd )
	//{
	//	std::string strUserEdit;
	//	strUserEdit = "메롱";
	//	SetWindowText( m_hUserEdithWnd ,strUserEdit.c_str() );
	//	//SendMessage( hWndlistbox, LB_ADDSTRING, 0, (LPARAM)szTempString );
	//}

	if( m_hSendEdithWnd )
	{
		TCHAR szTempString[256];
		TCHAR szExceptionCause[256];
		TString strSnedEdit;

		SYSTEMTIME time;
		::GetLocalTime(&time);
		_snprintf( szTempString, 256, _T(" 발생시간 : %d년%d월%d일 %d:%d:%d \r\n "), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		strSnedEdit += szTempString;

		OnExceptionCause(  pException->ExceptionRecord->ExceptionCode, szExceptionCause, sizeof(szExceptionCause) );
		_snprintf( szTempString, 256, _T("발생원인 : %s \r\n "), szExceptionCause );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("오류주소 : 0x%08x \r\n "), pException->ContextRecord->Eip );
		strSnedEdit += szTempString;


		strSnedEdit += _T("==================컴퓨터 정보================== \r\n ");
		_snprintf( szTempString, 256, _T("운영체제	: %s \r\n "), pSystemInfo->GetWindowVersionString() );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("그래픽 카드	: %s \r\n "), pSystemInfo->GetVideoCardString() );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("CPU		: %s \r\n "), pSystemInfo->GetCpuString() );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("DirectX	: %s \r\n "), pSystemInfo->GetDxVersionString() );
		strSnedEdit += szTempString;


		strSnedEdit += _T("==================메모리 정보================== \r\n ");
		MEMORYSTATUS MemStatus;
		MemStatus.dwLength=sizeof(MemStatus);
		GlobalMemoryStatus(&MemStatus);

		_snprintf( szTempString, 256, _T("전체 메모리		: %dMB \r\n "), MemStatus.dwTotalPhys/(1024*1024) );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("사용한 메모리		: %dMB \r\n "), MemStatus.dwAvailPhys/(1024*1024) );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("가상 메모리		: %dMB \r\n "), MemStatus.dwTotalVirtual/(1024*1024) );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("사용한 가상 메모리	: %dMB \r\n "), MemStatus.dwAvailVirtual/(1024*1024) );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("비디오 메모리		: %dMB \r\n "), pSystemInfo->GetVideoPhysicMemory() );
		strSnedEdit += szTempString;


		strSnedEdit += _T("=================레지스터 정보================= \r\n ");
		_snprintf( szTempString, 256, _T("EDI	: 0x%08x \r\n ESI	: 0x%08x \r\n EAX	: 0x%08x \r\n "), 
			pException->ContextRecord->Edi, pException->ContextRecord->Esi, pException->ContextRecord->Eax );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("EBX	: 0x%08x \r\n ECX	: 0x%08x \r\n EDX	: 0x%08x \r\n "), 
			pException->ContextRecord->Ebx, pException->ContextRecord->Ecx, pException->ContextRecord->Edx );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("EIP	: 0x%08x \r\n EBP	: 0x%08x \r\n SegCs	: 0x%08x \r\n "), 
			pException->ContextRecord->Eip, pException->ContextRecord->Ebp, pException->ContextRecord->SegCs );
		strSnedEdit += szTempString;

		_snprintf( szTempString, 256, _T("EFlags	: 0x%08x \r\n ESP	: 0x%08x \r\n SegSs	: 0x%08x \r\n "), 
			pException->ContextRecord->EFlags, pException->ContextRecord->Esp, pException->ContextRecord->SegSs );
		strSnedEdit += szTempString;

		SetWindowText( m_hSendEdithWnd ,strSnedEdit.c_str() );
	}
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : User가 입력한 정보
//######################################################################################
void CNTDbgHelperWindow::GetUserString(TCHAR *pszString, int nLen)
{
	GetWindowText( m_hUserEdithWnd , pszString, nLen);
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : System정보
//######################################################################################
void CNTDbgHelperWindow::GetSystemString(TCHAR *pszString, int nLen)
{
	GetWindowText( m_hSendEdithWnd , pszString, nLen);
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : Event 발생기다림
//######################################################################################
UINT CNTDbgHelperWindow::GetEvent()
{
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnregisterClass( WINCLASSNAME, GetModuleHandle( NULL ) );

	return m_nResult;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : Event Proc
//######################################################################################
LRESULT CALLBACK 
CNTDbgHelperWindow::WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			HDC MemDC;
			HBITMAP OldBitmap;
			int bx,by;
			BITMAP bit;
			hdc=BeginPaint( hwnd , &ps);
			MemDC=CreateCompatibleDC(hdc);

			if( m_pDbgHelperWindow->m_Image )
			{
				OldBitmap=(HBITMAP)SelectObject(MemDC, m_pDbgHelperWindow->m_Image);

				GetObject(m_pDbgHelperWindow->m_Image,sizeof(BITMAP),&bit);
				bx=bit.bmWidth;
				by=bit.bmHeight;

				BitBlt(hdc,0,0,bx,by,MemDC,0,0,SRCCOPY);
				SelectObject(MemDC,OldBitmap);
			}

			const TCHAR szMainString[] = MAIN_MAINSTRING;
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor( hdc, CLR_NONE );
			RECT rcMainScreen = { 90,20,WIN_WIDTH-15,60 };
			DrawText( hdc, szMainString, (int)_tcslen(szMainString), &rcMainScreen, DT_WORDBREAK ); 

			if( m_pDbgHelperWindow->m_bMainWindow )
			{
				const TCHAR szSubString[] = MAIN_SUBSTRING;
				RECT rcSubScreen = { 10,80,WIN_WIDTH-15,180 };
				DrawText( hdc, szSubString, (int)_tcslen(szSubString), &rcSubScreen, DT_WORDBREAK ); 
			}
			else
			{
				const TCHAR szSubString1[] = MAIN_MORESUBSTRING1;
				RECT rcSubScreen1 = { 10,80,WIN_WIDTH-15,180 };
				DrawText( hdc, szSubString1, (int)_tcslen(szSubString1), &rcSubScreen1, DT_WORDBREAK ); 

				const TCHAR szSubString2[] = MAIN_MORESUBSTRING2;
				RECT rcSubScreen2 = { 10,265,WIN_WIDTH-15,380 };
				DrawText( hdc, szSubString2, (int)_tcslen(szSubString2), &rcSubScreen2, DT_WORDBREAK ); 
			}

			DeleteDC(MemDC);
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_DESTROY :
		{
			PostQuitMessage( 0 );
		}
		break;
	case WM_COMMAND :
		{
			switch( LOWORD(wParam) )
			{
			case IDC_DUMPSEND :
			case IDC_DUMPCANCEL :
				{
					switch( HIWORD(wParam) )
					{
					case BN_CLICKED :
						{
							m_pDbgHelperWindow->m_nResult = (unsigned int)LOWORD(wParam);
							PostQuitMessage( 0 );
						}
						break;
					}
				}
				break;
			case IDC_MOREINFO :
				{
					switch( HIWORD(wParam) )
					{
					case BN_CLICKED :
						{
							m_pDbgHelperWindow->OnMoreInfoWindows();
						}
						break;
					}
				}
				break;
			case IDC_MAININFO:
				{
					switch( HIWORD(wParam) )
					{
					case BN_CLICKED :
						{
							m_pDbgHelperWindow->OnBaseWindows();
						}
						break;
					}
				}
				break;
			}
		}
		break;
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}



//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 컨트롤 생성
//######################################################################################
void CNTDbgHelperWindow::OnCreateControl()
{

	int			x, y, cx, cy;
	WNDCLASS	wc;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)CNTDbgHelperWindow::WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= GetModuleHandle( NULL );
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject( WHITE_BRUSH ) ;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= WINCLASSNAME;

	RegisterClass(&wc);

	cx = WIN_WIDTH;
	cy = WIN_HEIGHT;
	x = GetSystemMetrics( SM_CXSCREEN )/2 - cx/2;
	y = GetSystemMetrics( SM_CYSCREEN )/2 - cy/2;

	m_Image = (HBITMAP)LoadImage( NULL, WIN_BGIMAGE, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );



	// Main 윈도우 생성
	m_Mainhwnd = CreateWindow( WINCLASSNAME, WINMAINTITLE, WS_OVERLAPPED | WS_CAPTION  , x, y, cx, cy, NULL, NULL, GetModuleHandle( NULL ), NULL );

	CreateWindow(_T("button"), _T("Report"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , 10 , WIN_HEIGHT-80, 70, 30, m_Mainhwnd, (HMENU)IDC_DUMPSEND, GetModuleHandle( NULL ), NULL);
	CreateWindow(_T("button"), _T("Detail"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , 90 , WIN_HEIGHT-80, 70, 30, m_Mainhwnd, (HMENU)IDC_MOREINFO, GetModuleHandle( NULL ), NULL);
	CreateWindow(_T("button"), _T("Cancel"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , 170, WIN_HEIGHT-80, 70, 30, m_Mainhwnd, (HMENU)IDC_DUMPCANCEL, GetModuleHandle( NULL ), NULL);

	m_hMainProgresshWnd = CreateWindow(_T("msctls_progress32"), _T("Press Send to Send") , WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 10, WIN_HEIGHT-45, WIN_WIDTH-25, 10, m_Mainhwnd,(HMENU)IDC_PROGRESS, GetModuleHandle( NULL ),NULL);
	SendMessage( m_hProgresshWnd, PBM_SETRANGE, 0, MAKELPARAM( 0, 100) );
	SendMessage( m_hProgresshWnd, PBM_SETPOS, 0, 0 );




	// MoreInfo 윈도우 생성
	cy = WIN_MOREHEIGHT;
	y = GetSystemMetrics( SM_CYSCREEN )/2 - cy/2;
	m_Morehwnd = CreateWindow( WINCLASSNAME, WINMORETITLE, WS_OVERLAPPED | WS_CAPTION , x, y, cx, cy, NULL, NULL, GetModuleHandle( NULL ), NULL );
	CreateWindow(_T("button"), _T("Report"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , 10 , WIN_MOREHEIGHT-80, 70, 30, m_Morehwnd, (HMENU)IDC_DUMPSEND, GetModuleHandle( NULL ), NULL);
	CreateWindow(_T("button"), _T("Simply"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , 90 , WIN_MOREHEIGHT-80, 70, 30, m_Morehwnd, (HMENU)IDC_MAININFO, GetModuleHandle( NULL ), NULL);
	CreateWindow(_T("button"), _T("Cancel"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , 170, WIN_MOREHEIGHT-80, 70, 30, m_Morehwnd, (HMENU)IDC_DUMPCANCEL, GetModuleHandle( NULL ), NULL);

	m_hMoreProgresshWnd = CreateWindow(_T("msctls_progress32"), _T("Press Send to Send") , WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 10, WIN_MOREHEIGHT-45, WIN_WIDTH-25, 10, m_Morehwnd,(HMENU)IDC_PROGRESS, GetModuleHandle( NULL ),NULL);
	SendMessage( m_hProgresshWnd, PBM_SETRANGE, 0, MAKELPARAM( 0, 100) );
	SendMessage( m_hProgresshWnd, PBM_SETPOS, 0, 0 );
	m_hUserEdithWnd = CreateWindow(_T("edit"),_T(" "), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL,
		10, 120, WIN_WIDTH-25, 120,
		m_Morehwnd, (HMENU)IDC_DUMPEDITBOX, GetModuleHandle( NULL ), NULL);

	m_hSendEdithWnd = CreateWindow(_T("edit"),NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,// | WS_DISABLED,
		10, 290, WIN_WIDTH-25, 200,
		m_Morehwnd, (HMENU)IDC_DUMPEDITBOX, GetModuleHandle( NULL ), NULL);


	OnBaseWindows();
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : Main Window Show
//######################################################################################
void CNTDbgHelperWindow::OnBaseWindows()
{
	ShowWindow( m_Mainhwnd, SW_SHOW );
	ShowWindow( m_Morehwnd, SW_HIDE );
	m_hProgresshWnd = m_hMainProgresshWnd;
	m_bMainWindow = true;
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : More Window Show
//######################################################################################
void CNTDbgHelperWindow::OnMoreInfoWindows()
{
	ShowWindow( m_Mainhwnd, SW_HIDE );
	ShowWindow( m_Morehwnd, SW_SHOW );
	m_hProgresshWnd = m_hMoreProgresshWnd;
	m_bMainWindow = false;
}







// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송