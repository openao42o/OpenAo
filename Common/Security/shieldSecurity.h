#ifndef GAME_SHIELD_LIB_SHIELDSECURITY_H
#define GAME_SHIELD_LIB_SHIELDSECURITY_H

#include <winsock2.h>

// path to innova's dll to load, if not specified or not called
// then by default loads "gameShieldDll.dll" from current directory
void frostInitialize(const char* dllPath);

// call at the end of application, to free frost's resourses
void frostFinalize();

// call to set userName
void frostSetUserName(const char* uin);

// replacemant for ::connect()
int frostConnect(SOCKET s,
                 const struct sockaddr* name,
                 int namelen);

// replacemant for ::send()
int frostSend(SOCKET s,
              const char* buf,
              int len,
              int flags);

// replacemant for ::recv()
int frostRecv(SOCKET s,
              char* buf,
              int len,
              int flags);

// replacemant for ::WSASend()
int frostWSASend(SOCKET s,
                 LPWSABUF lpBuffers,
                 DWORD dwBufferCount,
                 LPDWORD lpNumberOfBytesSent,
                 DWORD dwFlags,
                 LPWSAOVERLAPPED lpOverlapped,
                 LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

// replacemant for ::WSARecv()
int frostWSARecv(SOCKET s,
                 LPWSABUF lpBuffers,
                 DWORD dwBufferCount,
                 LPDWORD lpNumberOfBytesRecvd,
                 LPDWORD lpFlags,
                 LPWSAOVERLAPPED lpOverlapped,
                 LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

// replacemant for ::sendto()
int frostSendto(SOCKET s,
                const char* buf,
                int len,
                int flags,
                const struct sockaddr* to,
                int tolen);

// replacemant for ::recvfrom()
int frostRecvfrom(SOCKET s,
                  char* buf,
                  int len,
                  int flags,
                  struct sockaddr* from,
                  int* fromlen);

#endif //GAME_SHIELD_LIB_SHIELDSECURITY_H

