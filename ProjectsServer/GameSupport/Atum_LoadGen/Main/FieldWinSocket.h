// FieldWinSocket.h: interface for the CFieldWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDWINSOCKET_H__CB2738BC_78A1_4334_A35F_5AB344D9BE08__INCLUDED_)
#define AFX_FIELDWINSOCKET_H__CB2738BC_78A1_4334_A35F_5AB344D9BE08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"
#include "AnalyzePacket.h"

class CAtum_LoadGenDlg;
class CFieldWinSocket : public CWinSocket  
{
public:

	// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 인자 추가(, BOOL i_bIsDummySocketToAnalyzePacket=FALSE)
	CFieldWinSocket(HWND hWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY, BOOL i_bIsDummySocketToAnalyzePacket=FALSE);
	virtual ~CFieldWinSocket();

public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);		// 하나의 패킷이 도착했을 때 발생
	BOOL SendMsg( MessageType_t nType, char *pPacket, int nSize );					// 프로토콜 전송
	int		m_nOldType;
	int		m_nArrayIndex;
	

	CHARACTER	m_character;			// 2004-03-09 by cmkwon
	BOOL		m_bSendMoveFlag;		// 2004-03-09 by cmkwon, Move 패킷을 보내는지에 대한 플래그
	DWORD		m_dwMoveCounts;			// 2004-03-12 by cmkwon, Move Count를 저장한다, 일정 시간 방향 벡터를 변경하기 위해 사용
	DWORD		m_dwConnectTick;		// 2004-03-15 by cmkwon, Connect와 GameStart의 시간차를 구하기 위해

	ClientIndex_t	m_idxTarget;
	D3DXVECTOR3		m_vec3TargetPosition;
	

	// 2004-03-15 by cmkwon
	CHARACTER* GetCharacterPtr(void){			return &m_character;}
	void SetConnectTick(DWORD i_dwConnectTick){	m_dwConnectTick = i_dwConnectTick;}
	DWORD GetConnectTick(void){					return m_dwConnectTick;}

	void CharacterMove(int xStart, int xEnd, int y, int zStart, int zEnd, int nSpeed, BOOL bAttack);

	void SendPriAttackMsg(void);
	void SendSecAttackMsg(void);

	// 2007-01-23 by cmkwon
	CAtum_LoadGenDlg	*m_pLoadDlg;		
	void SetLoadDlg(CAtum_LoadGenDlg *i_pLoadDlg);

	ITEM				m_PriItemInfo;
	ITEM_GENERAL		m_PriItemG;
	ITEM				m_SecItemInfo;
	ITEM_GENERAL		m_SecItemG;
	UINT				m_uiSecItemFieldIndex;
	
	///////////////////////////////////////////////////////////////////////////////	
	// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
	BOOL					m_bIsDummySocketToAnalyzePacket;
	SAL_PACKET				*m_pSAL_PACKET;

	void RecvedProtocol(MessageType_t i_msgTy, int i_nMsgSize);
	void SetSAL_PACKET(SAL_PACKET *i_pSAL_PACKET);
	
};

#endif // !defined(AFX_FIELDWINSOCKET_H__CB2738BC_78A1_4334_A35F_5AB344D9BE08__INCLUDED_)
