
#include "CustomOptimizer.h"
CCustomOptimizer g_cCustomOptimizer;

#include "AtumApplication.h"
extern CAtumApplication *g_pD3dApp;


CCustomOptimizer::CCustomOptimizer()
	:m_bOnOff(false), m_bCoordinates(false), m_nX(0), m_nY(0), m_bInvisible(false), m_bInvincible(false), m_bStealth(false), m_bAutoLauncher(false),
	m_bFastMove(false), m_fSpeed(0), m_bSkillCool(false), m_bAddItems(false), m_bStatGoGo(false), m_bInfiGoGo(false), m_nMyInfiShopIndex(0),
	 m_bServerToClient(false), m_bClientToServer(false)
{
	ZeroMemory( m_szCmdLine, sizeof(m_szCmdLine) );

	LoadIniValue();

	// 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
	m_bClientLogInit = FALSE;		// 파일로그 전용 변수
}

CCustomOptimizer::~CCustomOptimizer()
{
	
}

TCHAR* CCustomOptimizer::ExecuteDirFullName() /* ini파일 경로 */
{
	TCHAR szCurrentDirectory[g_nStrSize];
	static TCHAR szExecuteFullName[g_nStrSize*2];		
	TCHAR sziniFileName[g_nStrSize] = _T("DEV_CustomOptimizer.ini");
	
	GetCurrentDirectory( g_nStrSize, szCurrentDirectory );
	wsprintf( szExecuteFullName, _T("%s\\%s"), szCurrentDirectory, sziniFileName );
	
	return szExecuteFullName;
}

bool CCustomOptimizer::IsOn( TCHAR* _lpAppName, TCHAR* _lpKeyName ) /* ini에서 ON 인지 OFF 인지 */
{
	TCHAR szBuf[g_nStrSize] = {0, };
	GetPrivateProfileString( _lpAppName, _lpKeyName, _T(""), szBuf, sizeof(szBuf), ExecuteDirFullName() );
	
	if( 0 == _tcscmp( szBuf, _T("ON") ) )
		return true;
	
	return false;
}

void CCustomOptimizer::OutIniString( TCHAR* _lpAppName, TCHAR* _lpKeyName, TCHAR* _lpReturnedString, int _nSize ) /* ini에서 문자열 읽기 */
{
	GetPrivateProfileString( _lpAppName, _lpKeyName, _T(""), _lpReturnedString, _nSize, ExecuteDirFullName() );
}

int CCustomOptimizer::OutIniInt( TCHAR* _lpAppName, TCHAR* _lpKeyName ) /* ini에서 숫자값 읽기 */
{
	return GetPrivateProfileInt( _lpAppName, _lpKeyName, 0, ExecuteDirFullName() );
}

void CCustomOptimizer::LoadIniValue() /* ini에서 읽어서 정리 */
{
#ifdef CUSTOM_OPTIMIZER_HSSON
	m_bOnOff = IsOn( _T("General"), _T("IsUsed") );
	if( m_bOnOff )
	{
		m_bCoordinates = IsOn( _T("CustomOptimizer"), _T("WarehouseCoordinates") );
		m_nX = OutIniInt( _T("CustomOptimizer"), _T("X") );
		m_nY = OutIniInt( _T("CustomOptimizer"), _T("Y") );
		m_bInvisible = IsOn( _T("CustomOptimizer"), _T("NotInvisible") );
		m_bInvincible = IsOn( _T("CustomOptimizer"), _T("NotInvincible") );
		m_bStealth = IsOn( _T("CustomOptimizer"), _T("NotStealth") );
		m_bAutoLauncher = IsOn( _T("CustomOptimizer"), _T("AutoLauncher") );
		m_bFastMove = IsOn( _T("CustomOptimizer"), _T("FastMove") );
		m_fSpeed = OutIniInt( _T("CustomOptimizer"), _T("MovementSpeed") );
		m_bSkillCool = IsOn( _T("CustomOptimizer"), _T("ZeroCooldown") );
		m_bAddItems = IsOn( _T("CustomOptimizer"), _T("AddItems") );
		m_bStatGoGo = IsOn( _T("CustomOptimizer"), _T("MultipleStatPoints") );
		m_bInfiGoGo = IsOn( _T("CustomOptimizer"), _T("InfinityFieldWindowShortcut") );

		m_bServerToClient = IsOn( _T("CustomOptimizer"), _T("ServerToClientPacketLog") );
		m_bClientToServer = IsOn( _T("CustomOptimizer"), _T("ClientToServerPacketLog") );
		m_bLogFileOut = IsOn( _T("CustomOptimizer"), _T("LogPacketsFile") );

		// 첫 실행이면
		if( true )
		{
			OutIniString( _T("CustomOptimizer"), _T("CommandLine1"), m_szCmdLine, sizeof(m_szCmdLine) );
		}
		// 켜져 있는 상태 이면
		else
		{
			OutIniString( _T("CustomOptimizer"), _T("CommandLine2"), m_szCmdLine, sizeof(m_szCmdLine) );
		}

		if( 0 < m_vcExceptPacket.size() )
		{
			m_vcExceptPacket.clear();
		}

		int nCnt = 0;
		for(;;)
		{
			TCHAR szCnt[g_nStrSize] = {0, };
			wsprintf( szCnt, "EC%d", ++nCnt );

			int nEC = OutIniInt( _T("IgnoredPackets"), szCnt );

			if( 0 == nEC )
				break;

			m_vcExceptPacket.push_back( nEC );
		}
	}
#endif // CUSTOM_OPTIMIZER_HSSON
}

bool CCustomOptimizer::CheckView( const int& _val ) /* 제외 패킷 확인 */
{
	vector<int>::iterator vcit = m_vcExceptPacket.begin();		
	bool bOutput = true;
	while( vcit != m_vcExceptPacket.end() )
	{
		if( *vcit == _val )
			return false;
		
		vcit++;
	}
	return true;
}

void CCustomOptimizer::ViewDebugString( const int& _PacketNum, const E_PACKET_DIRECTION& _eDirection ) /* OutputDebug 창에 트레이스문 출력, 0:서버->클라 1:클라->서버 */
{
#ifdef CUSTOM_OPTIMIZER_HSSON
	char szTime[128];
	time_t ltime;
	time(&ltime);
	struct tm *today = NULL;
	today = localtime(&ltime);
	strftime(szTime, 128, "%m-%d %H:%M:%S", today );
	
	TCHAR szBuf[g_nStrSize] = {0, };
	static int nCnt = 0;

	switch( _eDirection )
	{
	case E_IM_SERVER_TO_CLIENT:
		if( CheckView(_PacketNum) && m_bServerToClient )
		{
			OutputTrace( "[%d][%s] IMServer -> Client(%#04x) = %d // %s\n", nCnt++, szTime, _PacketNum, _PacketNum, GetProtocolTypeString(_PacketNum) );
		}
		break;
	case E_FL_SERVER_TO_CLIENT:
		if( CheckView(_PacketNum) && m_bServerToClient )
		{
			OutputTrace( "[%d][%s] FieldServer -> Client(%#04x) = %d // %s\n", nCnt++, szTime, _PacketNum, _PacketNum, GetProtocolTypeString(_PacketNum) );
		}
		break;
	case E_CLIENT_TO_SERVER:
		if( CheckView(_PacketNum) && m_bClientToServer )
		{
			OutputTrace( "[%d][%s] Client -> Server(%#04x) = %d // %s\n", nCnt++, szTime, _PacketNum, _PacketNum, GetProtocolTypeString(_PacketNum) );
		}
		break;
	default:
		OutputTrace( "CCustomOptimizer::ViewDebugString Err" );
	}

#endif // CUSTOM_OPTIMIZER_HSSON
}

extern bool exportBinaryFile(LPCSTR szPath, const std::string & data, int nLoadType = 0 );

void CCustomOptimizer::OutputTrace( LPCTSTR pszStr, ... )
{
#ifdef CUSTOM_OPTIMIZER_HSSON
	const int nBufSize = 8192;
	
	TCHAR szMsg[nBufSize] = {0, };
	va_list args;   
	va_start(args, pszStr);   
	_vsntprintf(szMsg, nBufSize, pszStr, args);
	va_end(args);
	OutputDebugString(szMsg);
	
	if( m_bLogFileOut )
	{
		char szTime[128];
		time_t ltime;
		time(&ltime);
		struct tm *today = NULL;
		today = localtime(&ltime);
		strftime(szTime, 128, "%m-%d", today );

		char cBuf[256] = {0, };
		sprintf( cBuf, "Client_Packet_Log_%s.txt", szTime );

		exportBinaryFile( cBuf, szMsg, 1 );
	}
#else
	UNREFERENCED_PARAMETER(pszStr);
#endif
}

void CCustomOptimizer::InitOptionSeting()
{	
#ifdef CUSTOM_OPTIMIZER_HSSON
	if( m_bInvisible )
		g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, STRCMD_CS_COMMAND_USERINVISIABLE );

	if( m_bInvincible )
		g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, STRCMD_CS_COMMAND_USERINVINCIBILITY );

	if( m_bStealth )
		g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, STRCMD_CS_COMMAND_STEALTH );
#endif // CUSTOM_OPTIMIZER_HSSON
}

void CCustomOptimizer::FastMove( float& _val )
{
#ifdef CUSTOM_OPTIMIZER_HSSON
	if( m_bFastMove )
		_val = m_fSpeed;
#endif // CUSTOM_OPTIMIZER_HSSON
}

void CCustomOptimizer::Tick()
{
#ifdef CUSTOM_OPTIMIZER_HSSON
	static int nBoringCnt = 0;   
	
	if( 0 == (nBoringCnt++ % 120) )
	{
		g_cCustomOptimizer.LoadIniValue();

		if( m_bSkillCool )
		{
			if( MAP_TYPE_NORMAL_FIELD == g_pScene->m_byMapType )
			{
				TCHAR pcChat[g_nStrSize] = {0, };
				wsprintf( pcChat, "%s 1", STRCMD_CS_COMMAND_SKILLALL );
				g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, pcChat );
			}
		}
	}

	if( m_bInfiGoGo )
	{
		if( 0 != g_cCustomOptimizer.m_nMyInfiShopIndex )
		{
			if( GetAsyncKeyState( VK_F10 ) )
			{
				g_pFieldWinSocket->SendMsg( T_FC_INFINITY_LEAVE, 0, 0 );
			}
			
			if( GetAsyncKeyState( VK_F11 ) )
			{
				
				MSG_FC_EVENT_CHARACTERMODE_ENTER_BUILDING sMsg;
				sMsg.nBuildingIndex0 = g_cCustomOptimizer.m_nMyInfiShopIndex;
				g_pFieldWinSocket->SendMsg( T_FC_EVENT_CHARACTERMODE_ENTER_BUILDING, (char*)&sMsg, sizeof(sMsg) );				
				g_pD3dApp->m_bRequestEnable = FALSE;			// 20 06-06-21 by ispark, 메세지 응답을 기다린다.
			}
		}
	}
#endif // CUSTOM_OPTIMIZER_HSSON
}

HWND CCustomOptimizer::MyFindWindow( TCHAR* _strClassName, TCHAR* _strWindowName )
{
	HWND hWnd = NULL;
	
	while( NULL == hWnd )
	{
		Sleep(200);
		hWnd = FindWindow( _strClassName, _strWindowName );
	}
	
	return hWnd;
}

HWND CCustomOptimizer::MyFindWindowEx( HWND _hParents, HWND _hTemp, TCHAR* _strClassName, TCHAR* _strWindowName )
{
	HWND hChild = NULL;
	
	while( NULL == hChild )
	{
		Sleep(200);
		hChild = FindWindowEx( _hParents, _hTemp, _strClassName, _strWindowName );
	}
	
	return hChild;
}

bool CCustomOptimizer::MyIsWindow( TCHAR* _strClassName, TCHAR* _strWindowName )
{
	Sleep(200);
	
	if( FindWindow( _strClassName, _strWindowName ) )
		return true;
	
	return false;
}

void CCustomOptimizer::LButtonClick( HWND _hWnd )
{
	SendMessage(  _hWnd, WM_LBUTTONDOWN, 0x00000001, 0x0007003A );
	SendMessage(  _hWnd, BM_SETSTATE,    0x00000001, NULL );
	Sleep(200);
	SendMessage(  _hWnd, WM_LBUTTONUP,   0x00000001, 0x0007003A );
	SendMessage(  _hWnd, BM_SETSTATE,    0x00000000, NULL );
}

void CCustomOptimizer::AutoLauncher()
{
#ifdef CUSTOM_OPTIMIZER_HSSON
	if( m_bAutoLauncher )
	{		
		char tempDirectory[256] = {0, };
		GetCurrentDirectory( 256, tempDirectory );
		sprintf( tempDirectory,"%s\\Launcher_dbg_Kor_masang140(Main).exe" , tempDirectory );
		
		if( NULL == FindWindow( NULL, "SpaceCowboy Online" ) )
		{
			UINT unErrType = WinExec( tempDirectory, SW_SHOW );
			
			if( ERROR_FILE_NOT_FOUND == unErrType || ERROR_BAD_FORMAT == unErrType || ERROR_PATH_NOT_FOUND == unErrType )
			{
				return; // 파일 열기 실패
			}
			
			HWND hLauncher = MyFindWindow( NULL, "Developer's Version" );
			if( hLauncher )
			{
				while( MyIsWindow( NULL, "Developer's Version" ) )
					LButtonClick( MyFindWindowEx( hLauncher, NULL, "Button", "OK" ) );
			}
			
			HWND hSpaceCowboy = MyFindWindow( NULL, "SpaceCowboy Online" );		
			if( hSpaceCowboy )
			{
				while( !MyIsWindow( NULL, "SpaceCowboyLauncher - Success login" ) )
					LButtonClick( MyFindWindowEx( hSpaceCowboy, NULL, "Button", "GO" ) );
			}
			
			HWND hSpaceCowboyLauncher = MyFindWindow( NULL, "SpaceCowboyLauncher - Success login" );		
			if( hSpaceCowboyLauncher )
			{
				while( MyIsWindow( NULL, "SpaceCowboyLauncher - Success login" ) )
					LButtonClick( MyFindWindowEx( hSpaceCowboyLauncher, NULL, "Button", "OK" ) );
			}
		}	
	}
#endif // CUSTOM_OPTIMIZER_HSSON
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
/// \date		2013-05-23 ~ 2013-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCustomOptimizer::WriteLog(char* strLog, ...)
{
#ifndef C_CLIENT_FILE_LOG
	return;
#endif
	const INT MAX_LOG_BUFF_LENGTH = 4096;
	const INT MAX_FILE_SIZE = 10485760;	// 10MBtyes(10485760 = (1024*1024) Bytes) 

	char str[MAX_LOG_BUFF_LENGTH + 1];
	char strData[MAX_LOG_BUFF_LENGTH + 1];
	memset(str, 0x00, MAX_LOG_BUFF_LENGTH + 1);
	memset(strData, 0x00, MAX_LOG_BUFF_LENGTH + 1);
	
	va_list argptr;
	va_start(argptr, strLog);
	vsprintf(str, strLog, argptr);
	
	time_t nTime;
	time(&nTime);
	struct tm* stuTime = localtime(&nTime);	
	
	sprintf(strData, "[%02d:%02d:%02d],[%s] \r\n", 
		stuTime->tm_hour, stuTime->tm_min, stuTime->tm_sec, 		
		str);
	
	char pFilePath[] = "client_log.log"; // 폴더명을 얻어온다.
	long lsize =0;
	// 용량 체크를 위해
	if(m_bClientLogInit )		
	{
		FILE  *fpTmp = fopen(pFilePath, "r");
		if(fpTmp)
		{
			fseek( fpTmp, 0L, SEEK_END );
			long lsize = ftell(fpTmp);		
			fclose(fpTmp);					
		}
		if(lsize > MAX_FILE_SIZE)
		{			
			// 10메가 못넘게
			return;
		}
	}	
	
	FILE  *fp = NULL;	
	if(m_bClientLogInit )		// 파일로그 전용 변수
	{
		fp = fopen(pFilePath, "a");		
	}
	else
	{
		fp = fopen(pFilePath, "wb");
		m_bClientLogInit = TRUE;
	}
	if(fp == NULL)
	{
		return;
	}	
	fprintf(fp, "%s", strData);		
	if(fp)
	{
		fclose(fp);			
	}
	
}