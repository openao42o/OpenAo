#include "stdafx.h"						// 2009-07-10 by cmkwon, 러시아 Frost 신규 Lib로 수정 - 
#include "shieldSecurityDll.h"
#include "utility.h"
#include "frost.bmp.h"
#include <commctrl.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Advapi32.lib")

shieldSecurityDll* shieldSecurityDll::m_inst = 0;
const char* shieldSecurityDll::m_defaultDll = "gameShieldDll.dll";

shieldSecurityDll::shieldSecurityDll()
{
    memset(this, 0, sizeof(shieldSecurityDll));
}

shieldSecurityDll::~shieldSecurityDll()
{
    if(m_updateLib) {
        typedef void (*freeFrostUpdate_t)();
        freeFrostUpdate_t freeFrostUpdate =
            (freeFrostUpdate_t)GetProcAddress(m_updateLib, "freeFrostUpdate");
        if(freeFrostUpdate){
            freeFrostUpdate();
        }
    }
    if(m_lib) {
        FreeLibrary(m_lib);
    }
}

void shieldSecurityDll::loadLibrary(const char* dllPath)
{
    m_lib = LoadLibraryA(dllPath);
    if(m_lib) {
        m_pSetUserName = (void (*)(const char*))
            GetProcAddress(m_lib, "frostSetUserName");
        m_pConnect = (int (*)(SOCKET, const struct sockaddr*, int))
            GetProcAddress(m_lib, "frostConnect");
        m_pSend = (int (*)(SOCKET, const char*, int, int))
            GetProcAddress(m_lib, "frostSend");
        m_pRecv = (int (*)(SOCKET, char*, int, int))
            GetProcAddress(m_lib, "frostRecv");
        m_pWSASend = (int (*)(SOCKET, LPWSABUF, DWORD, LPDWORD,
                      DWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))
            GetProcAddress(m_lib, "frostWSASend");
        m_pWSARecv = (int (*)(SOCKET, LPWSABUF, DWORD,
            LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE))
            GetProcAddress(m_lib, "frostWSARecv");
        m_pSendto = (int (*)(SOCKET, const char*, int, int,
                             const struct sockaddr*, int))
             GetProcAddress(m_lib, "frostSendto");
        m_pRecvfrom = (int (*)(SOCKET, char*, int, int,
                       struct sockaddr*, int*))
             GetProcAddress(m_lib, "frostRecvfrom");
    }
}

LRESULT CALLBACK shieldSecurityDll::
logoWndProc(HWND hWnd,
            UINT message,
            WPARAM wParam,
            LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_COMMAND:
        if (HIWORD(wParam) == 0xabab && LOWORD(wParam) == 0xefef) {
            DestroyWindow(hWnd);
            PostQuitMessage(0);
            return 0;
        }
        break;
    case WM_LBUTTONDOWN:
        PostMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(lParam, wParam));
        return 0;
    case WM_PAINT:
        if(m_inst){
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);
            HDC hMemDC = CreateCompatibleDC(hDC);
            HGDIOBJ oldBitmap = SelectObject(hMemDC, m_inst->m_hBitmap);
            int oldBkMode = SetBkMode(hMemDC, TRANSPARENT);
            BitBlt(hDC, 0, 0, m_inst->m_logoWidth, m_inst->m_logoHeight, hMemDC, 0, 0, SRCCOPY);
            SelectObject(hMemDC, oldBitmap);
            SetBkMode(hMemDC, oldBkMode);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void shieldSecurityDll::initLogoWnd(void*)
{
    if(!m_inst) return;
    strcpy(m_inst->m_szWindowClass, "frostsecurity_init_window");
    WNDCLASSEXA wcex;
    memset(&wcex, 0, sizeof(wcex));
    wcex.cbSize         = sizeof(wcex);
    wcex.lpfnWndProc    = logoWndProc;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName  = m_inst->m_szWindowClass;
    RegisterClassExA(&wcex);
    m_inst->m_logoWnd =
        CreateWindowExA(WS_EX_PALETTEWINDOW,
                        m_inst->m_szWindowClass,
                        NULL,
                        WS_POPUP | WS_VISIBLE | WS_OVERLAPPED,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        0,
                        0,
                        0,
                        0,
                        0,
                        NULL);
    if(m_inst->m_logoWnd) {
        m_inst->m_logoWidth = bmpWidth;
        m_inst->m_logoHeight = bmpHeight;
        DWORD* bmpBits = GetBmpBits();
        BITMAPINFO bmpInfo;
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo.bmiHeader.biWidth = m_inst->m_logoWidth;
        bmpInfo.bmiHeader.biHeight = m_inst->m_logoHeight;
        bmpInfo.bmiHeader.biPlanes = 1;
        bmpInfo.bmiHeader.biBitCount = 32;
        bmpInfo.bmiHeader.biCompression = BI_RGB;
        bmpInfo.bmiHeader.biSizeImage = 0;
        bmpInfo.bmiHeader.biXPelsPerMeter = 0;
        bmpInfo.bmiHeader.biYPelsPerMeter = 0;
        bmpInfo.bmiHeader.biClrUsed = 0;
        bmpInfo.bmiHeader.biClrImportant = 0;
        HDC hDC = GetDC(m_inst->m_logoWnd);
        m_inst->m_hBitmap = CreateCompatibleBitmap(hDC, m_inst->m_logoWidth, m_inst->m_logoHeight);
        SetDIBits(hDC, m_inst->m_hBitmap, 0, m_inst->m_logoHeight, bmpBits, &bmpInfo, DIB_RGB_COLORS);
        delete[] bmpBits;
        BITMAP mBitmap;
        GetObject(m_inst->m_hBitmap, sizeof(mBitmap), &mBitmap);
        m_inst->m_logoWidth  = mBitmap.bmWidth;
        m_inst->m_logoHeight = mBitmap.bmHeight;
        HDC hMemDC = CreateCompatibleDC(hDC);
        HGDIOBJ pOldBitmap = SelectObject(hMemDC, m_inst->m_hBitmap);
        HRGN crRgnTmp;
        HRGN crRgn = CreateRectRgn(0, 0, 0, 0);
        COLORREF crTransparent = RGB(0, 255, 0);
        int iX = 0;
        for (int iY = 0; iY < m_inst->m_logoHeight; iY++) {
            do {
                while (iX <= m_inst->m_logoWidth && GetPixel(hMemDC, iX, iY) == crTransparent) ++iX;
                int iLeftX = iX;
                while (iX <= m_inst->m_logoWidth && GetPixel(hMemDC, iX, iY) != crTransparent) ++iX;
                crRgnTmp = CreateRectRgn(iLeftX, iY, iX, iY+1);
                CombineRgn(crRgn, crRgn, crRgnTmp, RGN_OR);
                DeleteObject(crRgnTmp);
            } while(iX < m_inst->m_logoWidth);
            iX = 0;
        }
        SetWindowRgn(m_inst->m_logoWnd, crRgn, TRUE);
        int iiX = (GetSystemMetrics(SM_CXSCREEN)) / 2 - (m_inst->m_logoWidth / 2);
        int iiY = (GetSystemMetrics(SM_CYSCREEN)) / 2 - (m_inst->m_logoHeight / 2);
        SetWindowPos(m_inst->m_logoWnd, NULL, iiX, iiY, m_inst->m_logoWidth, m_inst->m_logoHeight, NULL);
        ReleaseDC(m_inst->m_logoWnd, hDC);
        SelectObject(hMemDC, pOldBitmap);
        DeleteDC(hMemDC);
        DeleteObject(crRgn);
    }
    if(m_inst->m_logoEvent) {
        SetEvent(m_inst->m_logoEvent);
    }
    if(m_inst->m_logoWnd) {
        MSG msg;
        while (GetMessage(&msg, 0, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void shieldSecurityDll::loadUpdateDll(const char* dllPathCstr)
{
    std::string dllPath(dllPathCstr);
    std::string pd(utility::get_parent_directory(dllPath));
    bool fnameChanged = std::string::npos == dllPath.find(m_defaultDll);
    if(fnameChanged) {
        CopyFileA(dllPath.c_str(),
                  (pd + "\\" + m_defaultDll).c_str(),
                  false);
    }
    m_updateLib = LoadLibraryA((pd + "\\frostUpdater.dll").c_str());
    if(m_updateLib) {
        typedef void (*frostUpdate_t)(HINSTANCE, const char*);
        frostUpdate_t frostUpdate =
            (frostUpdate_t)GetProcAddress(m_updateLib, "frostUpdate");
        if(frostUpdate){
            frostUpdate(GetModuleHandleA("frostUpdater"), pd.c_str());
        }
    }
    if(fnameChanged) {
        CopyFileA((pd + "\\" + m_defaultDll).c_str(),
                  dllPath.c_str(),
                  false);
        DeleteFileA((pd + "\\" + m_defaultDll).c_str());
    }
}

inline int lowerCase(int c) 
{
    return (int)(((0x40 < c) && (c < 0x5a)) ? (c + 0x20) : c);
}

static bool isTestVersion()
{
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_CURRENT_USER,
                      "software\\frost\\action",
                      0,
                      KEY_QUERY_VALUE,
                      &hKey) != ERROR_SUCCESS) {
        return false;
    }
    DWORD dwType = REG_SZ;
    char ptsPath[MAX_PATH] = {0};
    DWORD ptsPathSize = MAX_PATH;
    if (RegQueryValueExA(hKey,
                         "ignore_updater_update",
                         0,
                         &dwType,
                         (LPBYTE)ptsPath,
                         &ptsPathSize) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }
    RegCloseKey(hKey);
    return true;
}

void shieldSecurityDll::update(const char* dllPathcstr)
{
    static const std::string srvName = "frostsecurity.net";
    static const uint16 srvPort      = 80;
    static const std::string path    = "frost/updater/";
    static bool sockInited           = false;
    if(!sockInited) {
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
            WSACleanup();
            return;
        }
        sockInited = true;
    }
    std::string rootURL  = 
        "http://" + srvName + ":" + utility::int2str(srvPort) + "/" + path;
    std::string dllPath(dllPathcstr);
    if(std::string::npos == dllPath.find_first_of("\\/")) {
        dllPath = std::string(".\\") + dllPath;
    }
    m_logoEvent = CreateEvent(0, false, false, 0);
    CreateThread(0,
                 0,
                 (LPTHREAD_START_ROUTINE)initLogoWnd,
                 0,
                 0,
                 0);
    update_by_crc_param up(rootURL.c_str(),
                           utility::get_parent_directory(dllPath).c_str());
    if((!isTestVersion()) &&
        utility::connection_exists(srvName, srvPort, 5000)) {
       utility::update_by_crc(&up);
    }
    if(m_logoEvent) {
        WaitForSingleObject(m_logoEvent, INFINITE);
        CloseHandle(m_logoEvent);
        m_logoEvent = 0;
    }
    if(m_logoWnd) {
        PostMessage(m_logoWnd,
                    WM_COMMAND,
                    MAKEWPARAM(0xefef, 0xabab),
                    0);
        m_logoWnd = 0;
    }
    loadUpdateDll(dllPath.c_str());
}

void shieldSecurityDll::init(const char* path)
{
    const char* dllPath = path ? path : m_defaultDll;
    if(!m_inst) {
        try {
            m_inst = new shieldSecurityDll();
            if(m_inst) {
                m_inst->update(dllPath);
                m_inst->loadLibrary(dllPath);
            }
        } catch (...) {
            finish();
        }
    }
}

void shieldSecurityDll::finish()
{
    delete m_inst;
    m_inst = 0;
}

void shieldSecurityDll::setUserName(const char* uin)
{
    if(m_inst && m_inst->m_pSetUserName) {
        m_inst->m_pSetUserName(uin);
    }
}

int shieldSecurityDll::connect(SOCKET s,
                               const struct sockaddr* name,
                               int namelen)
{
    if(m_inst && m_inst->m_pConnect) {
        return m_inst->m_pConnect(s, name, namelen);
    }
    return ::connect(s, name, namelen);
}

int shieldSecurityDll::send(SOCKET s,
                            const char* buf,
                            int len,
                            int flags)
{
    if(m_inst && m_inst->m_pSend) {
        return m_inst->m_pSend(s, buf, len, flags);
    }
    return ::send(s, buf, len, flags);
}

int shieldSecurityDll::recv(SOCKET s,
                            char* buf,
                            int len,
                            int flags)
{
    if(m_inst && m_inst->m_pRecv) {
        return m_inst->m_pRecv(s, buf, len, flags);
    }
    return ::recv(s, buf, len, flags);
}

int shieldSecurityDll::WSASend(SOCKET s,
                               LPWSABUF lpBuffers,
                               DWORD dwBufferCount,
                               LPDWORD lpNumberOfBytesSent,
                               DWORD dwFlags,
                               LPWSAOVERLAPPED lpOverlapped,
                               LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    if(m_inst && m_inst->m_pWSASend) {
        return m_inst->m_pWSASend(s,
                                  lpBuffers,
                                  dwBufferCount,
                                  lpNumberOfBytesSent,
                                  dwFlags,
                                  lpOverlapped,
                                  lpCompletionRoutine);
    }
    return ::WSASend(s,
                     lpBuffers,
                     dwBufferCount,
                     lpNumberOfBytesSent,
                     dwFlags,
                     lpOverlapped,
                     lpCompletionRoutine);
}

int shieldSecurityDll::WSARecv(SOCKET s,
                               LPWSABUF lpBuffers,
                               DWORD dwBufferCount,
                               LPDWORD lpNumberOfBytesRecvd,
                               LPDWORD lpFlags,
                               LPWSAOVERLAPPED lpOverlapped,
                               LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    if(m_inst && m_inst->m_pWSARecv) {
        return m_inst->m_pWSARecv(s,
                                  lpBuffers,
                                  dwBufferCount,
                                  lpNumberOfBytesRecvd,
                                  lpFlags,
                                  lpOverlapped,
                                  lpCompletionRoutine);
    }
    return ::WSARecv(s,
                     lpBuffers,
                     dwBufferCount,
                     lpNumberOfBytesRecvd,
                     lpFlags,
                     lpOverlapped,
                     lpCompletionRoutine);
}

int shieldSecurityDll::sendto(SOCKET s,
                              const char* buf,
                              int len,
                              int flags,
                              const struct sockaddr* to,
                              int tolen)
{
    if(m_inst && m_inst->m_pSendto) {
        return m_inst->m_pSendto(s,
                                 buf,
                                 len,
                                 flags,
                                 to,
                                 tolen);
    }
    return ::sendto(s,
                    buf,
                    len,
                    flags,
                    to,
                    tolen);
}

int shieldSecurityDll::recvfrom(SOCKET s,
                                char* buf,
                                int len,
                                int flags,
                                struct sockaddr* from,
                                int* fromlen)
{
    if(m_inst && m_inst->m_pRecvfrom) {
        return m_inst->m_pRecvfrom(s,
                                   buf,
                                   len,
                                   flags,
                                   from,
                                   fromlen);
    }
    return ::recvfrom(s,
                      buf,
                      len,
                      flags,
                      from,
                      fromlen);
}
