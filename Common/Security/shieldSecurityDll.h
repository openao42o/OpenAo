#ifndef GAME_SHIELD_LIB_SHIELDSECURITYDLL_H
#define GAME_SHIELD_LIB_SHIELDSECURITYDLL_H

#include <winsock2.h>
#include <windows.h>

class shieldSecurityDll
{
public:
    static void init(const char* dllPath);

    static void finish();

    static void setUserName(const char*);

    static int connect(SOCKET s,
                       const struct sockaddr* name,
                       int namelen);

    static int send(SOCKET s,
                    const char* buf,
                    int len,
                    int flags);

    static int recv(SOCKET s,
                    char* buf,
                    int len,
                    int flags);

    static int WSASend(SOCKET s,
                       LPWSABUF lpBuffers,
                       DWORD dwBufferCount,
                       LPDWORD lpNumberOfBytesSent,
                       DWORD dwFlags,
                       LPWSAOVERLAPPED lpOverlapped,
                       LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

    static int WSARecv(SOCKET s,
                       LPWSABUF lpBuffers,
                       DWORD dwBufferCount,
                       LPDWORD lpNumberOfBytesRecvd,
                       LPDWORD lpFlags,
                       LPWSAOVERLAPPED lpOverlapped,
                       LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

    static int sendto(SOCKET s,
                      const char* buf,
                      int len,
                      int flags,
                      const struct sockaddr* to,
                      int tolen);

    static int recvfrom(SOCKET s,
                        char* buf,
                        int len,
                        int flags,
                        struct sockaddr* from,
                        int* fromlen);

public:
    ~shieldSecurityDll();
private:
    void (*m_pSetUserName)(const char*);

    int (*m_pConnect)(SOCKET,
                      const struct sockaddr*,
                      int);

    int (*m_pSend)(SOCKET,
                   const char*,
                   int,
                   int);

    int (*m_pRecv)(SOCKET,
                   char*,
                   int,
                   int);

    int (*m_pWSASend)(SOCKET,
                      LPWSABUF,
                      DWORD,
                      LPDWORD,
                      DWORD,
                      LPWSAOVERLAPPED,
                      LPWSAOVERLAPPED_COMPLETION_ROUTINE);

    int (*m_pWSARecv)(SOCKET,
                      LPWSABUF,
                      DWORD,
                      LPDWORD,
                      LPDWORD,
                      LPWSAOVERLAPPED,
                      LPWSAOVERLAPPED_COMPLETION_ROUTINE);

    int (*m_pSendto)(SOCKET,
                     const char*,
                     int,
                     int,
                     const struct sockaddr*,
                     int);

    int (*m_pRecvfrom)(SOCKET,
                       char*,
                       int,
                       int,
                       struct sockaddr*,
                       int*);

private:
    shieldSecurityDll();

    void loadLibrary(const char* dllPath);

    void update(const char* dllPath);

    void loadUpdateDll(const char* dllPath);

    static void initLogoWnd(void*);

    static LRESULT __stdcall logoWndProc(HWND,
                                         UINT,
                                         WPARAM,
                                         LPARAM);

    static void loadUpdateDll();

private:
    static shieldSecurityDll* m_inst;
    static const char* m_defaultDll;

    HMODULE m_lib;
private:
    HMODULE m_updateLib;
    HANDLE  m_logoEvent;
    HWND    m_logoWnd;
    char    m_szWindowClass[64];
    HBITMAP m_hBitmap;
    int     m_logoWidth;
    int     m_logoHeight;

private:
    shieldSecurityDll(const shieldSecurityDll&);
    shieldSecurityDll& operator=(const shieldSecurityDll&);
};

#endif //GAME_SHIELD_LIB_SHIELDSECURITYDLL_H

