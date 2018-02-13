// Chat.h: interface for the CChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAT_H__2D2AD2D9_AEEA_44D6_A4A9_BBFF6FA2234B__INCLUDED_)
#define AFX_CHAT_H__2D2AD2D9_AEEA_44D6_A4A9_BBFF6FA2234B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

// 2007-11-05 by bhsohn 몬스터 오브젝트 체크섬 처리
struct structErrorChatInfo
{		
	char	szErrString[512];
	int		nType;
};

class CChat : public CAtumNode
{
public:
	CChat();
	virtual ~CChat();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	virtual void Render();

	void CreateChatChild(
		const char* str,
		int type,
		int chatType = CHAT_TAB_SYSTEM,
		const char* szWisperToCharacterName = nullptr,
		const char *szWisperFromCharacterName = nullptr,
		BYTE NameColor = 0);
	

	// 2007-11-05 by bhsohn 몬스터 오브젝트 체크섬 처리
	void ErrorInfoAllAddChat();

protected:
	INT m_nDataNum;
	
	// 2007-11-05 by bhsohn 몬스터 오브젝트 체크섬 처리
	vector<structErrorChatInfo>		m_vecErrInfo;

//	CAtumNode	*	m_pRenderer;
};

#endif // !defined(AFX_CHAT_H__2D2AD2D9_AEEA_44D6_A4A9_BBFF6FA2234B__INCLUDED_)
