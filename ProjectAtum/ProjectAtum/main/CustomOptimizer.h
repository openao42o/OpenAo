#pragma once

#include "StdAfx.h"

class CAtumApplication;

const int g_nStrSize = 512;

class CCustomOptimizer
{
public:
	CCustomOptimizer();
	~CCustomOptimizer();
	
	TCHAR* ExecuteDirFullName();							// ini파일 경로
	bool IsOn( TCHAR* _lpAppName, TCHAR* _lpKeyName );		// ini에서 ON 인지 OFF 인지
	void OutIniString( TCHAR* _lpAppName, TCHAR* _lpKeyName, TCHAR* _lpReturnedString, int _nSize ); // ini에서 문자열 읽기
	int  OutIniInt( TCHAR* _lpAppName, TCHAR* _lpKeyName );	// ini에서 숫자값 읽기
	void LoadIniValue();										// ini에서 읽어서 정리

	bool CheckView( const int& _val );						// 제외 패킷 확인
	enum E_PACKET_DIRECTION { E_IM_SERVER_TO_CLIENT = 0, E_FL_SERVER_TO_CLIENT, E_CLIENT_TO_SERVER };
	void ViewDebugString( const int& _PacketNum, const E_PACKET_DIRECTION& _eDirection ); // OutputDebug 창에 트레이스문 출력
	void OutputTrace( LPCTSTR pszStr, ... );

	void InitOptionSeting();
	void FastMove( float& _val );

	void Tick();

	HWND MyFindWindow( TCHAR* _strClassName, TCHAR* _strWindowName );	
	HWND MyFindWindowEx( HWND _hParents, HWND _hTemp, TCHAR* _strClassName, TCHAR* _strWindowName );	
	bool MyIsWindow( TCHAR* _strClassName, TCHAR* _strWindowName );	
	void LButtonClick( HWND _hWnd );	
	void AutoLauncher();

	// 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
	void WriteLog(char* strLog, ...);
	// END 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가

	bool m_bOnOff;					// 사용여부 

	bool m_bCoordinates;			// 창고정좌표
	int  m_nX;
	int  m_nY;
	bool m_bInvisible;				// 투명안하기
	bool m_bInvincible;				// 무적안하기
	bool m_bStealth;				// 스텔스안하기
	bool m_bAutoLauncher;			// 자동런쳐
	bool m_bFastMove;				// 마을이속증가
	float m_fSpeed;					// 이속값
	bool m_bSkillCool;				// 쿨타임제로
	bool m_bAddItems;				// 복수아이템생성
	bool m_bStatGoGo;				// 스텟왕창올리기
	bool m_bInfiGoGo;				// 인피창바로켜기
	int  m_nMyInfiShopIndex;		// 인피니티 인덱스

	bool m_bServerToClient;			// 서버->클라 패킷보기
	bool m_bClientToServer;			// 클라->서버 패킷보기
	bool m_bLogFileOut;				// 패킷 파일로 출력

	TCHAR m_szCmdLine[g_nStrSize];	// 실행인자
	vector<int> m_vcExceptPacket;   // 제외패킷

	// 2013-05-23 by bhsohn 클라이언트 파일로그(LOG) 시스템 추가
	BOOL	m_bClientLogInit;		// 파일로그 전용 변수
};

extern CCustomOptimizer g_cCustomOptimizer;