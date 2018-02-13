// INFSpeakeMsg.h: interface for the CINFSpeakeMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSPEAKEMSG_H__E344490F_433D_43C9_85E3_7F8DEC37BE7C__INCLUDED_)
#define AFX_INFSPEAKEMSG_H__E344490F_433D_43C9_85E3_7F8DEC37BE7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 마이크 아이콘 모양
#define		SP_IMG_STAT_0		0
#define		SP_IMG_STAT_1		1
#define		MAX_SP_IMG_STAT		2

// 페이드 모드
#define		SP_BK_FADE_NONE		-1
#define		SP_BK_FADE_OUT		0
#define		SP_BK_FADE_IN		1
#define		SP_BK_FADE_SHOW		2

#define		MAX_SPEAKER_BUFF		300

typedef struct 
{		
	char	strChatMsg[MAX_SPEAKER_BUFF];			// 2007-08-09 by cmkwon, 모든 세력에 채팅 전송하기 - 
	BOOL	bGm;
	BOOL	bGmColor;
} structSpeakerInfo;

class CINFSpeakeMsg  : public CINFBase
{
public:
	CINFSpeakeMsg();
	virtual ~CINFSpeakeMsg();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 스피커
	void AddSpeakerMsgString(char* pTxt, BOOL bGm, BOOL bGmColor);	
	void DeleteNotGmMsg();

private:
	BOOL IsShowMsgWindow();
	void ShowMsgWindow(BOOL bShow);
	void EndOneString();	

	void TickFadeMode();
	void TickString();
	void TickSpeakerIcon();

	void RenderString();
	void InitStatus();

	void GetNextString(int nStringPosX, CD3DHanFont*	pFont, char* i_pSrc, char* i_pDst);


private:
	// 배경
	CINFImageEx*	m_pMsgBk;									  // 2011. 10. 10 by jskim UI시스템 변경
	POINT		m_ptMsgBk;

	// 2007-11-13 by bhsohn GM스피커 아이템 변경 
	CINFImageEx*	m_pMsgGmBk;

	// 스피커
	CINFImageEx*	m_pSpImg[MAX_SP_IMG_STAT];

	CINFImageEx*	m_pSpGmImg[MAX_SP_IMG_STAT];
	

	int			m_nSelSpImg;	

	char		m_chShowTxt[MAX_SPEAKER_BUFF];

	CD3DHanFont*	m_pFontSp;

	int			m_nStringPosX;	
	float		m_fOneStepTime;
	float		m_fStopTime;

	BOOL		m_bShow;

	deque<structSpeakerInfo>	m_vecSpeakerInfo;	
	deque<structSpeakerInfo>	m_vecGmSpeakerInfo;	

	BOOL		m_bShowGmColor;
	BOOL		m_bShowGmMode;
	float		m_fShowGmTime;

	int			m_nFadeMode;
	float		m_fFadeInTime;
	DWORD		m_dwBkAlpha;
	
};

#endif // !defined(AFX_INFSPEAKEMSG_H__E344490F_433D_43C9_85E3_7F8DEC37BE7C__INCLUDED_)
