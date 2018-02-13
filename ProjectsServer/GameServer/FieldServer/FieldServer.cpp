// FieldServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "FieldGlobal.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"
#include "LogWinsocket.h"
#include "PreWinsocket.h"
#include "config.h"
#include "NPCScripts.h"
#include "VMemPool.h"
#include "AtumError.h"

// 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef S_MINI_DUMP_HSKIM
#include "dbgHelp.h"
#pragma comment(lib, "dbghelp.lib")


LONG __stdcall ExceptionHandler(_EXCEPTION_POINTERS* pExceptionInfo)
{
	char fileName[MAX_PATH];

	strcpy(fileName, "dumps\\"); // cannot contain '.'
	{
		auto filelen = strlen(fileName);

		GetModuleFileName(NULL, fileName + filelen, sizeof(fileName) - filelen);
	}

    auto ext = strrchr(fileName, '.');

#ifdef S_SERVER_CRASH_FILENAME_ADD_INFO
	// 2013-05-28 by hskim, 미니 덤프 파일 이름에 시간 추가
	ATUM_DATE_TIME	CurrentTime(TRUE);
	sprintf(ext ? ext : fileName+strlen(fileName), "_%s.dmp", CurrentTime.GetFileDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
	// end 2013-05-28 by hskim, 미니 덤프 파일 이름에 시간 추가
#else
    strcpy(ext ? ext : fileName + strlen(fileName), ".dmp");
#endif

    char szTemp[256];
    wsprintf(szTemp, "FieldServer Crash !! : Create dump file (Exception 0x%08x arised)", pExceptionInfo->ExceptionRecord->ExceptionCode);

    HANDLE hProcess = GetCurrentProcess();
    DWORD dwProcessID = GetCurrentProcessId();
    HANDLE hFile = CreateFile(fileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId = GetCurrentThreadId();
    eInfo.ExceptionPointers = pExceptionInfo;
    eInfo.ClientPointers = FALSE;

    MiniDumpWriteDump(hProcess, dwProcessID, hFile, MiniDumpWithFullMemory, pExceptionInfo ? &eInfo : NULL, NULL, NULL);

	return EXCEPTION_EXECUTE_HANDLER;
}
// end 2012-04-20 by hskim 미니덤프 기능 추가
#endif
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_pFieldGlobal = new CFieldGlobal { };

// 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef S_MINI_DUMP_HSKIM
	SetUnhandledExceptionFilter(&ExceptionHandler);
#endif
// end 2012-04-20 by hskim 미니덤프 기능 추가

#ifdef ARENA
	if (!g_pFieldGlobal->InitGlobal("Arena Field Server", "ArenaFieldSystem", hInstance, IDI_FIELDSERVER, nCmdShow))
#else
	if (!g_pFieldGlobal->InitGlobal("Field Server", "FieldSystem", hInstance, IDI_FIELDSERVER, nCmdShow))
#endif //ARENA	
	
		return FALSE;

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	VMemPool::vmPoolClean();
	CIOCP::SocketClean();

	server::log("Field Server End\r\n\r\n\r\n");

	util::del(g_pFieldGlobal);
	return 1;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	g_pFieldGlobal->m_dwLastWndMsg			= message;
	g_pFieldGlobal->m_dwLastWndMsgTick		= timeGetTime();
	g_pFieldGlobal->m_dwLastWndMsgWParam	= wParam;
	g_pFieldGlobal->m_dwLastWndMsgLParam	= lParam;
	
	switch (message)
	{
	case WM_CREATE:
		{
			SetLastError(0);
			BOOL bRet = g_pFieldGlobal->LoadConfiguration();

			if(FALSE == bRet)
			{
				MessageBox(hWnd, "LoadConfiguration Error", "ERROR", MB_OK);
			}
			else
			{
/*				// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
				char szServerName[SIZE_MAX_STRING_128] = { 0, };
				sprintf(szServerName, "FieldServer_%s", g_pFieldGlobal->GetServerGroupName());
				SetWindowText(hWnd, szServerName);
				// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
				// Config 읽기 성공 후 설정된 IP정보를 로그로 기록.
				g_pFieldGlobal->WriteSystemLogEX( TRUE , "  [Notify] LoadConfiguration Success !, GetIPLocal(%s), IPLogServer(%s), IPPreServer(%s), IPIMServer(%s)\r\n" , 
															g_pFieldGlobal->GetIPLocal(),
															g_pFieldGlobal->GetIPLogServer(),
															g_pFieldGlobal->GetIPPreServer(),
															g_pFieldGlobal->GetIPIMServer() );

				g_pFieldGlobal->CreateAllF2WSocket(hWnd);
				if (g_pFieldGlobal->m_bCheckLogServer)
				{
					g_pFieldGlobal->ConnectAllF2LWSocket(g_pFieldGlobal->GetIPLogServer(), g_pFieldGlobal->GetPortLogServer());
				}
				g_pFieldGlobal->CreateField2PreWinSocket(hWnd);
				g_pFieldGlobal->GetField2PreWinSocket()->Connect(g_pFieldGlobal->GetIPPreServer(), g_pFieldGlobal->GetPortPreServer());
				g_pFieldGlobal->CreateField2IMWinSocket(hWnd);
				g_pFieldGlobal->GetField2IMWinSocket()->Connect(g_pFieldGlobal->GetIPIMServer(), g_pFieldGlobal->GetPortIMServer());
				// 2007-12-26 by dhjin, 아레나 통합 - 아레나 서버 정보
				if(FALSE == g_pFieldGlobal->IsArenaServer())
				{
					g_pFieldGlobal->CreateField2ArenaFieldWinSocket(hWnd);
					g_pFieldGlobal->GetField2ArenaFieldWinSocket()->Connect(g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerIP, g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerPort);
				}
			}
		}
		break;
	case WM_LOG_ASYNC_EVENT:
		{
			g_pFieldGlobal->OnF2LAsyncEvent(wParam, lParam);
		}
		break;
	case WM_LOG_PACKET_NOTIFY:
		{
			switch(LOWORD(wParam))
			{
			case CWinSocket::WS_ERROR:
				{
				}
				break;
			case CWinSocket::WS_CONNECTED:
				{
					if(HIWORD(wParam) == FALSE)
					{
						char	szSystemLog[256];
						sprintf(szSystemLog, STRMSG_S_F2LOGCONNECT_0000,
							g_pFieldGlobal->GetIPLogServer(), g_pFieldGlobal->GetPortLogServer());
						g_pFieldGlobal->WriteSystemLog(szSystemLog);
						if (g_pFieldGlobal->m_bCheckLogServer)
						{
							DBGOUT(szSystemLog);
						}

						g_pFieldGlobal->StartTimerReconnect();
					}
					else
					{
						// 2007-12-18 by cmkwon, 추가함
						// 2009-04-20 by cmkwon, F2L 관련 시스템 수정 - 
						//g_pFieldGlobal->WriteSystemLogEX(TRUE, STRMSG_S_F2LOGCONNECT_0001);
						CLogWinSocket *pLogWinSoc = (CLogWinSocket*)lParam;
						g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] connected to LogServer !, CLogWinSocket(0x%X) SockH(%ld) \r\n", pLogWinSoc, pLogWinSoc->GetSocketHandle());
						if(g_pFieldGlobal->InitServerSocket())
						{
							if(g_pFieldGlobal->GetTimerIDReconnect()
								&& g_pFieldGlobal->GetField2PreWinSocket()->IsConnected()
								&& g_pFieldGlobal->GetField2IMWinSocket()->IsConnected())
							{
								g_pFieldGlobal->EndTimerReconnect();
							}
							g_pFieldGlobal->StartTimerTraffic();
							g_pFieldGlobal->StartTimerAliveCheck();
						}
					}
				}
				break;
			case CWinSocket::WS_CLOSED:
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, STRMSG_S_F2LOGCONNECT_0002,
						g_pFieldGlobal->GetIPLogServer(), g_pFieldGlobal->GetPortLogServer());
					g_pFieldGlobal->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);

					g_pFieldGlobal->OnF2LClosed((CLogWinSocket*)lParam);	// 2009-04-20 by cmkwon, F2L 관련 시스템 수정 - 

					g_pFieldGlobal->EndTimerReconnect();
					g_pFieldGlobal->StartTimerReconnect();
				}
				break;
			case CWinSocket::WS_RECEIVED:
				{
				}
				break;
			}
		}
		break;
	case WM_PRE_ASYNC_EVENT:
		{
			if(g_pFieldGlobal->GetField2PreWinSocket())
			{
				g_pFieldGlobal->GetField2PreWinSocket()->OnAsyncEvent(lParam);
			}
		}
		break;
	case WM_PRE_PACKET_NOTIFY:
		{
			switch(LOWORD(wParam))
			{
			case CWinSocket::WS_ERROR:
				{
				}
				break;
			case CWinSocket::WS_CONNECTED:
				{
					if(HIWORD(wParam) == FALSE)
					{
						char	szSystemLog[256];
						sprintf(szSystemLog, STRMSG_S_F2PRECONNECT_0000,
							g_pFieldGlobal->GetIPPreServer(), g_pFieldGlobal->GetPortPreServer());
						g_pFieldGlobal->WriteSystemLog(szSystemLog);
						DBGOUT(szSystemLog);

						g_pFieldGlobal->StartTimerReconnect();
					}
					else
					{
						// 2009-04-17 by cmkwon, 시스템 로그 수정 - 
						//DBGOUT(STRMSG_S_F2PRECONNECT_0001);
						g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] connected to PreServer ! \r\n");		// 2009-04-17 by cmkwon, 시스템 로그 수정 - 
						if(g_pFieldGlobal->InitServerSocket())
						{
							if(g_pFieldGlobal->IsConnectedAllF2LWSocket()
								&& g_pFieldGlobal->GetTimerIDReconnect())
							{
								g_pFieldGlobal->EndTimerReconnect();
							}
							g_pFieldGlobal->StartTimerTraffic();
							g_pFieldGlobal->StartTimerAliveCheck();
						}
					}
				}
				break;
			case CWinSocket::WS_RECEIVED:
				{
					char * pPacket = NULL;
					int len,nType;
					g_pFieldGlobal->GetField2PreWinSocket()->Read(&pPacket, len);

					if(pPacket)
					{
						nType = 0;
						memcpy(&nType, pPacket, SIZE_FIELD_TYPE_HEADER);

						switch(nType)
						{
						case T_ERROR:
							{
								MSG_ERROR *pRecvMsg;
								pRecvMsg = (MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER);

								char buf[128];
								Err_t error = pRecvMsg->ErrorCode;

								g_pFieldGlobal->WriteSystemLogEX(TRUE, STRMSG_S_F2PRECONNECT_0002,
									GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode, "ST_PRE_SERVER",
									g_pFieldGlobal->GetField2PreWinSocket()->m_szPeerIP);

								switch (error)
								{
								case ERR_PROTOCOL_NO_SUCH_SERVER_GROUP:
								case ERR_PROTOCOL_NO_SUCH_FIELD_SERVER:
									break;
								default:
									{
										sprintf(buf, STRMSG_S_F2PRECONNECT_0003, GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode);
										//MessageBox(NULL, buf, "Error", MB_OK);
										DBGOUT(buf);
									}
									break;
								}
							}
						}
					}
				}
				break;
			case CWinSocket::WS_CLOSED:
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, STRMSG_S_F2PRECONNECT_0004,
						g_pFieldGlobal->GetIPPreServer(), g_pFieldGlobal->GetPortPreServer());
					g_pFieldGlobal->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);

					if(g_pFieldGlobal->GetField2IMWinSocket()
						&& g_pFieldGlobal->GetField2IMWinSocket()->IsConnected())
					{
						g_pFieldGlobal->GetField2IMWinSocket()->CloseSocket();
					}

					g_pFieldGlobal->EndServerSocket();
					g_pFieldGlobal->StartTimerReconnect();
				}
				break;
			}
		}
		break;
	case WM_IM_ASYNC_EVENT:
		{
			if(g_pFieldGlobal->GetField2IMWinSocket())
			{
				g_pFieldGlobal->GetField2IMWinSocket()->OnAsyncEvent(lParam);
			}
		}
		break;
	case WM_IM_PACKET_NOTIFY:
		{
			switch(LOWORD(wParam))
			{
			case CWinSocket::WS_ERROR:
				{
				}
				break;
			case CWinSocket::WS_CONNECTED:
				{
					if(HIWORD(wParam) == FALSE)
					{
						char	szSystemLog[256];
						sprintf(szSystemLog, STRMSG_S_F2IMCONNECT_0000,
							g_pFieldGlobal->GetIPIMServer(), g_pFieldGlobal->GetPortIMServer());
						g_pFieldGlobal->WriteSystemLog(szSystemLog);
						DBGOUT(szSystemLog);

						g_pFieldGlobal->StartTimerReconnect();
					}
					else
					{
						// 2009-04-17 by cmkwon, 시스템 로그 수정 - 
						//DBGOUT(STRMSG_S_F2IMCONNECT_0001);
						g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] connected to IMServer ! \r\n");		// 2009-04-17 by cmkwon, 시스템 로그 수정 - 
						if(g_pFieldGlobal->InitServerSocket())
						{
							if(g_pFieldGlobal->IsConnectedAllF2LWSocket()
								&& g_pFieldGlobal->GetTimerIDReconnect())
							{
								g_pFieldGlobal->EndTimerReconnect();
							}
							g_pFieldGlobal->StartTimerTraffic();
							g_pFieldGlobal->StartTimerAliveCheck();
						}
					}
				}
				break;
			case CWinSocket::WS_CLOSED:
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, STRMSG_S_F2IMCONNECT_0002,
						g_pFieldGlobal->GetIPIMServer(), g_pFieldGlobal->GetPortIMServer());
					g_pFieldGlobal->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);

					if(g_pFieldGlobal->GetField2PreWinSocket()
						&& g_pFieldGlobal->GetField2PreWinSocket()->IsConnected())
					{
						g_pFieldGlobal->GetField2PreWinSocket()->CloseSocket();
					}

					g_pFieldGlobal->EndServerSocket();
					g_pFieldGlobal->StartTimerReconnect();
				}
				break;
			case CWinSocket::WS_RECEIVED:
				{
					char * pPacket = NULL;
					int len,nType;
					g_pFieldGlobal->GetField2IMWinSocket()->Read(&pPacket, len);

					if(pPacket)
					{
						nType = 0;
						memcpy(&nType, pPacket, SIZE_FIELD_TYPE_HEADER);

						switch(nType)
						{
						case T_ERROR:
							{
								MSG_ERROR *pRecvMsg;
								pRecvMsg = (MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER);

								char buf[128];
								Err_t error = pRecvMsg->ErrorCode;

								g_pFieldGlobal->WriteSystemLogEX(TRUE, STRMSG_S_F2IMCONNECT_0003,
									GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode,
									"ST_IM_SERVER", g_pFieldGlobal->GetField2IMWinSocket()->m_szPeerIP);

								switch (error)
								{
									case ERR_PROTOCOL_NO_SUCH_SERVER_GROUP:
									case ERR_PROTOCOL_NO_SUCH_FIELD_SERVER:
									default:
										{
											sprintf(buf, STRMSG_S_F2IMCONNECT_0004, GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode);
											//MessageBox(NULL, buf, "Error", MB_OK);
											DBGOUT(buf);
										}
										break;
								}
							}
						}

						util::del(pPacket);
					}
				}
				break;
			}
		}
		break;
	// 2007-12-27 by dhjin, 아레나 통합 - 
	case WM_FIELD_ASYNC_EVENT:
		{
			if(g_pFieldGlobal->GetField2ArenaFieldWinSocket())
			{
				g_pFieldGlobal->GetField2ArenaFieldWinSocket()->OnAsyncEvent(lParam);
			}
		}
		break;
	case WM_FIELD_PACKET_NOTIFY:
		{
			switch(LOWORD(wParam))
			{
			case CWinSocket::WS_ERROR:
				{
				}
				break;
			case CWinSocket::WS_CONNECTED:
				{
					if(HIWORD(wParam) == FALSE)
					{
						char	szSystemLog[256];
						sprintf(szSystemLog, STRMSG_S_MF2AFCONNECT_0000,
							g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerIP, g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerPort);
// 2008-03-17 by cmkwon, 주석처리함						g_pFieldGlobal->WriteSystemLog(szSystemLog);
//						DBGOUT(szSystemLog);

						g_pFieldGlobal->StartTimerReconnect();
					}
					else
					{
// 2009-04-17 by cmkwon, 시스템 로그 수정 - 아래와 같이 수정 함.
// 						char	szSystemLog[256];
// 						sprintf(szSystemLog, STRMSG_S_MF2AFCONNECT_0001);
// 						DBGOUT(szSystemLog);
// 						// 2008-05-22 by dhjin, 아레나 서버 접속 실패 관련 로그 찾기
// //						DBGOUT(STRMSG_S_MF2AFCONNECT_0001);
						// 2010-03-02 by cmkwon, 로그 정보에 ArenaServer IP,Port 추가 - 
						g_pFieldGlobal->WriteSystemLogEX(FALSE, "  [Notify] connected to ArenaFieldServer(%s:%d) !, pIOCP(0x%X) \r\n", g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerIP, g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerPort, g_pFieldGlobal->GetGIOCP());	// 2009-04-17 by cmkwon, 시스템 로그 수정 - 
						g_pFieldGlobal->SendArenaServerMFSInfo();
						if(g_pFieldGlobal->IsConnectedAllF2LWSocket()
							&& g_pFieldGlobal->GetTimerIDReconnect())
						{
							g_pFieldGlobal->EndTimerReconnect();
						}
						g_pFieldGlobal->StartTimerTraffic();
						g_pFieldGlobal->StartTimerAliveCheck();
					}
				}
				break;
			case CWinSocket::WS_CLOSED:
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, STRMSG_S_MF2AFCONNECT_0002,
							g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerIP, g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerPort);
						g_pFieldGlobal->WriteSystemLog(szSystemLog);
					//DBGOUT(szSystemLog);

					g_pFieldGlobal->StartTimerReconnect();
				}
				break;
			case CWinSocket::WS_RECEIVED:
				{
					char * pPacket = NULL;
					int len,nType;
					g_pFieldGlobal->GetField2ArenaFieldWinSocket()->Read(&pPacket, len);

					if(pPacket)
					{
						nType = 0;
						memcpy(&nType, pPacket, SIZE_FIELD_TYPE_HEADER);

						switch(nType)
						{
						case T_ERROR:
							{
								MSG_ERROR *pRecvMsg;
								pRecvMsg = (MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER);

								char buf[128];
								Err_t error = pRecvMsg->ErrorCode;

								g_pFieldGlobal->WriteSystemLogEX(TRUE, STRMSG_S_MF2AFCONNECT_0003,
									GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode,
									"ST_ARENA_SERVER", g_pFieldGlobal->GetField2ArenaFieldWinSocket()->m_szPeerIP);
								sprintf(buf, STRMSG_S_MF2AFCONNECT_0004, GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode);
								DBGOUT(buf);
							}
						}

						util::del(pPacket);
					}
				}
				break;
			}
		}
		break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case TIMERID_TRAFFIC:
				{
					g_pFieldGlobal->CheckServerThread();
					g_pFieldGlobal->CalculateIOCPTraffic();

					if(GetTickCount() - g_pFieldGlobal->GetLastTickLogSystem() > TIMERGAP_LOGSYSTEM)
					{
						g_pFieldGlobal->SetLastTickLogSystem(GetTickCount());
						// 2008-04-08 by cmkwon, 아래와 같이 수정함
						CFieldIOCP *pFIOCP = (CFieldIOCP*)g_pFieldGlobal->GetGIOCP();
						if(pFIOCP && pFIOCP->GetListeningFlag())
						{// 2008-04-08 by cmkwon, NULL 체크 추가함
							pFIOCP->SendLogMessageServerInfo();
						}
					}
				}
				break;
			case TIMERID_ALIVE_CHECK:
				{
					//////////////////////////////////////////////////////
					// FieldServer, IMServer, LogServer로 Alive를 전송한다.
					if(g_pFieldGlobal->GetField2PreWinSocket()
						&& g_pFieldGlobal->GetField2PreWinSocket()->IsConnected())
					{
						g_pFieldGlobal->GetField2PreWinSocket()->WriteMessageType(T_FP_CONNECT_ALIVE);
					}
					if(g_pFieldGlobal->GetField2IMWinSocket()
						&& g_pFieldGlobal->GetField2IMWinSocket()->IsConnected())
					{
						g_pFieldGlobal->GetField2IMWinSocket()->WriteMessageType(T_FI_CONNECT_ALIVE);
					}
					// 2007-12-27 by dhjin, 아레나 통합 - 					
					if(g_pFieldGlobal->GetField2ArenaFieldWinSocket()
						&& g_pFieldGlobal->GetField2ArenaFieldWinSocket()->IsConnected())
					{
						g_pFieldGlobal->GetField2ArenaFieldWinSocket()->WriteMessageType(T_FtoA_ALIVE);
					}
					//////////////////////////////////////////////////////
					//
					g_pFieldGlobal->CheckClientAlive();
				}
				break;
			case TIMERID_RECONNECT:
				{
					g_pFieldGlobal->EndTimerReconnect();
					if(g_pFieldGlobal->GetField2PreWinSocket()
						&& g_pFieldGlobal->GetField2PreWinSocket()->IsConnected() == FALSE
						&& g_pFieldGlobal->GetField2PreWinSocket()->GetSocketHandle() == INVALID_SOCKET)
					{
						g_pFieldGlobal->GetField2PreWinSocket()->Connect(g_pFieldGlobal->GetIPPreServer(), g_pFieldGlobal->GetPortPreServer());
					}

					if(g_pFieldGlobal->GetField2IMWinSocket()
						&& g_pFieldGlobal->GetField2IMWinSocket()->IsConnected() == FALSE
						&& g_pFieldGlobal->GetField2IMWinSocket()->GetSocketHandle() == INVALID_SOCKET)
					{
						g_pFieldGlobal->GetField2IMWinSocket()->Connect(g_pFieldGlobal->GetIPIMServer(), g_pFieldGlobal->GetPortIMServer());
					}

					if(g_pFieldGlobal->GetField2ArenaFieldWinSocket()
						&& g_pFieldGlobal->GetField2ArenaFieldWinSocket()->IsConnected() == FALSE
						&& g_pFieldGlobal->GetField2ArenaFieldWinSocket()->GetSocketHandle() == INVALID_SOCKET)
					{
						g_pFieldGlobal->GetField2ArenaFieldWinSocket()->Connect(g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerIP, g_pFieldGlobal->m_sArenaServerInfo.ArenaFieldServerPort);
					}

					if(FALSE == g_pFieldGlobal->IsConnectedAllF2LWSocket())
					{
						g_pFieldGlobal->ReConnectAllF2LWSocket(g_pFieldGlobal->GetIPLogServer(), g_pFieldGlobal->GetPortLogServer());
					}
				}
				break;
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		{
			g_pFieldGlobal->EndServerSocket();
			g_pFieldGlobal->DestroyField2PreWinSocket();
			g_pFieldGlobal->DestroyField2IMWinSocket();
			g_pFieldGlobal->DestroyAllF2LWSocket();
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	g_pFieldGlobal->m_dwLastWndMsg			= 0;
	return 0;
}
