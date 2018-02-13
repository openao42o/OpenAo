#include "stdafx.h"						// 2009-07-10 by cmkwon, 러시아 Frost 신규 Lib로 수정 - 
#include "shieldSecurity.h"
#include "shieldSecurityDll.h"

void frostInitialize(const char* dllPath)
{
    shieldSecurityDll::init(dllPath);
}

void frostFinalize()
{
    shieldSecurityDll::finish();
}

void frostSetUserName(const char* uin)
{
    shieldSecurityDll::setUserName(uin);
}

int frostConnect(SOCKET s,
                 const struct sockaddr* name,
                 int namelen)
{
    return shieldSecurityDll::connect(s, name, namelen);
}

int frostSend(SOCKET s,
              const char* buf,
              int len,
              int flags)
{
    return shieldSecurityDll::send(s, buf, len, flags);
}

int frostRecv(SOCKET s,
              char* buf,
              int len,
              int flags)
{
    return shieldSecurityDll::recv(s, buf, len, flags);
}

int frostWSASend(SOCKET s,
                 LPWSABUF lpBuffers,
                 DWORD dwBufferCount,
                 LPDWORD lpNumberOfBytesSent,
                 DWORD dwFlags,
                 LPWSAOVERLAPPED lpOverlapped,
                 LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    return shieldSecurityDll::WSASend(s, lpBuffers, dwBufferCount,
                                      lpNumberOfBytesSent, dwFlags,
                                      lpOverlapped, lpCompletionRoutine);
}

int frostWSARecv(SOCKET s,
                 LPWSABUF lpBuffers,
                 DWORD dwBufferCount,
                 LPDWORD lpNumberOfBytesRecvd,
                 LPDWORD lpFlags,
                 LPWSAOVERLAPPED lpOverlapped,
                 LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
    return shieldSecurityDll::WSARecv(s, lpBuffers, dwBufferCount,
                                      lpNumberOfBytesRecvd, lpFlags,
                                      lpOverlapped, lpCompletionRoutine);
}

int frostSendto(SOCKET s,
                const char* buf,
                int len,
                int flags,
                const struct sockaddr* to,
                int tolen)
{
    return shieldSecurityDll::sendto(s, buf, len, flags, to, tolen);
}

int frostRecvfrom(SOCKET s,
                  char* buf,
                  int len,
                  int flags,
                  struct sockaddr* from,
                  int* fromlen)
{
    return shieldSecurityDll::recvfrom(s, buf, len, flags, from, fromlen);
}

