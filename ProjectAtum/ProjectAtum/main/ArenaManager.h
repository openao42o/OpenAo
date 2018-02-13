// ArenaManager.h: interface for the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENAMANAGER_H__79160014_CA51_4F71_9ABC_E7D25DD5EFC7__INCLUDED_)
#define AFX_ARENAMANAGER_H__79160014_CA51_4F71_9ABC_E7D25DD5EFC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArenaManager  
{
public:
	CArenaManager();
	virtual ~CArenaManager();
	
	// 아이디 명 변경
	void ConevertArenaRenderUserName(int nArenaState, char* pSrcDst);

	// 아레나 상태 업데이트
	void SetArenaState(int nState);
	int GetArenaState();	

	// 아레나 서버 정보
	void SetArenaServerInfo(MSG_FC_CONNECT_ARENASERVER_INFO* pMsg);
	MSG_FC_CONNECT_ARENASERVER_INFO* GetArenaServerInfo();

	// 아레나 캐릭터 정보
	void SetArenaCharacterInfo(CHARACTER	 *pAFSCharacter);
	CHARACTER*	GetArenaCharacterInfo();

	// 메인 서버 정보 갱신
	void SetMFSMyShuttleInfo(CHARACTER *pMyShuttleInfo);
	CHARACTER* GetMFSMyShuttleInfo();	

	// GM이 아레나 서버에 접속시도냐?
	void SetGmModeConnect(BOOL bGmModeConnect);
	BOOL IsGmModeConnect();
	
	// 아레나 서버 목록을 갱신한다.
	void FieldSocketArenaServerSServerGroupForClient(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT* pMsg);
	// 아레나 서버 목록을 알려준다.
	BOOL GetArenaId_To_ArenaServerName(char* pArenaId, char* pDstServerName);

	// 아레나 소켓을 강제로 닫는다.
	void SetForceCloseArenaSocket(BOOL bCloseArenaSocket);
	BOOL IsForceCloseArenaSocket();

	// 아레나 세력포인트
	void SetInflPoint(int nInflPoint);
	int GetInflPoint();

private:
	// 아레나 상태
	int					m_nArenaState;	// 아레나 상태
	MSG_FC_CONNECT_ARENASERVER_INFO*	m_pInfoArenaServer;	// 아레나 서버 접속정보
	CHARACTER*			m_pAFSCharacter;	// 아레나 캐릭터 정보	
	CHARACTER*			m_pMFSCharacter;	// 메인서버 캐릭터 정보
	BOOL				m_bGmModeConnect;	// Gm이 아레나 서버에 접속시도
	vector<SSERVER_GROUP_FOR_CLIENT>		m_vecArenaServerInfo;	 // 아레나 서버목록
	BOOL				m_bForceCloseArenaSocket;	// 아레나 소켓을 강제는 닫는다.

	// 2008-04-15 by bhsohn 아레나 워포인트 찍는 시스템 변경
	int					m_nInfPoint;
	
};

#endif // !defined(AFX_ARENAMANAGER_H__79160014_CA51_4F71_9ABC_E7D25DD5EFC7__INCLUDED_)
