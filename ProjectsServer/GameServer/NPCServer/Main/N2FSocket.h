// N2FSocket.h: interface for the CN2FSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N2FSOCK_H__3D8B9313_775E_4A34_98DE_E5373AEC1FFA__INCLUDED_)
#define AFX_N2FSOCK_H__3D8B9313_775E_4A34_98DE_E5373AEC1FFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

#define	WM_N2F_ASYNC_EVENT				(WM_USER + 1)				// 
#define	WM_N2F_PACKET_NOTIFY			(WM_USER + 2)				// HWND에 전달되는 메시지




class CNPCIOCP;
class CN2FSocket : public CWinSocket  
{
public:
	CN2FSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CN2FSocket();

	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);

	void SetNPCIOCP(CNPCIOCP *pNPCIOCP);

	ProcessResult Process_FN_NPCSERVER_SUMMON_JACO_MONSTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-08-22 by cmkwon, 해당 맵채널 몬스터 모두 삭제하기 기능 추가
	ProcessResult Process_FN_NPCSERVER_CINEMA_MONSTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed);	// 2010-03-31 by dhjin, 인피니티(기지방어) -	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Key몬스터 생성
	ProcessResult Process_FN_NPCSERVER_CINEMA_MONSTER_DESTROY(const char* pPacket, int nLength, int &nBytesUsed);	// 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
	ProcessResult Process_FN_NPCSERVER_CINEMA_MONSTER_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);	// 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가
	ProcessResult Process_FN_NPCSERVER_CINEMA_MONSTER_REGEN(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-06-02 인피니티 3차 - 스텝 6 - 주기적 소환 기능 제작
	ProcessResult Process_FN_NPCSERVER_NEW_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 
	ProcessResult Process_FN_NPCSERVER_RESET_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 변경 오브젝트를 위해!!!! 
	
public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CNPCIOCP		*m_pNPCIOCP1;
};


#endif // !defined(AFX_N2FSOCK_H__3D8B9313_775E_4A34_98DE_E5373AEC1FFA__INCLUDED_)
