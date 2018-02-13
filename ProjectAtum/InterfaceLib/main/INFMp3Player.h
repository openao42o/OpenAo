// INFImageAnimation.h: interface for the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINFMP3PLAYER_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_CINFMP3PLAYER_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// mp3 관련 기본
#define MP3PLAYER_BASE_TAB				4
#define MP3PLAYER_BUTTON_STATE			3
#define MP3PLAYER_BUTTON_MAIN_STATE		6
#define MP3PLAYER_MAX_LIST				10

class CINFImage;
class DataHeader;
class CGameData;
class CD3DHanFont;
class CINFScrollBar;
class CMusicMP3;
class CINFImageEx;

typedef struct
{
	char		szFileNameString[MAX_PATH];
	int			nIndex;
} Mp3File_t;

class CINFMp3Player: public CINFBase
{

public:
	CINFMp3Player();
	virtual ~CINFMp3Player();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	void Render(int posX, int posY);
#else
	virtual void Render();
#endif
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetMp3Directory(char *strDir){ memset(m_strDirectory,0x00, MAX_PATH);strncpy(m_strDirectory, strDir, strlen(strDir));}
	char* GetMp3Directory() { return m_strDirectory; }
	void FindMp3File();
	void DeleteSelectPlayList();
	void RandomPlayList();
	void SetStopButton(BOOL flag) { m_bStop_Flag = flag; }
	BOOL GetStopButton() {		return m_bStop_Flag;}
	void SetPlayButton(BOOL flag) { m_bPlay_Mute = flag; }
	BOOL GetPlayButton() { return m_bPlay_Mute;	}
	void Play_Mp3Music(BOOL bMute = FALSE);	
	
protected:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	void RenderButtonState(int posX, int posY);
	void RenderPlayListBox(int posX, int posY);
#else
	void RenderButtonState();
	void RenderPlayListBox();
#endif

	void FileNameCull(char *strName);

	// 기본 재생관련 설정

	
protected:
	CINFImageEx			*	m_pImgBack;
	CINFImageEx			*	m_pImgTab[MP3PLAYER_BASE_TAB];
	CINFImageEx			*	m_pImgHidenButton[MP3PLAYER_BUTTON_STATE];
	CINFImageEx			*	m_pImgMiniButton[MP3PLAYER_BUTTON_STATE];
	CINFImageEx			*	m_pImgNextButton[MP3PLAYER_BUTTON_STATE];
	CINFImageEx			*	m_pImgPlayButton[MP3PLAYER_BUTTON_STATE];
	CINFImageEx			*	m_pImgPrvButton[MP3PLAYER_BUTTON_STATE];
	CINFImageEx			*	m_pImgShowListButton[MP3PLAYER_BUTTON_STATE];
	CINFImageEx			*	m_pImgStopButton[MP3PLAYER_BUTTON_STATE];
	CINFImageEx			*	m_pImgVolum;
	CINFImageEx			*	m_pImgListBox;
	CINFImageEx			*	m_pImgMiniBase;
	CINFImageEx			*	m_pImgSelectMp3;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx			*	m_pImgPlayImage;
	CINFImageEx			*	m_pImgStopImage;
#endif
	CD3DHanFont			*	m_pFontTitle[MP3PLAYER_MAX_LIST];	
	CINFScrollBar		*	m_pScrollVolum;
	//CMusicMP3			*	m_pMp3;
	
	BOOL					m_bButtonStateMain[MP3PLAYER_BUTTON_MAIN_STATE];
	char					m_strDirectory[MAX_PATH];
	LONGLONG				m_LnMutePos;
	int						m_nButtonStateMain[MP3PLAYER_BUTTON_MAIN_STATE];
	int						m_nCurrentRenderIndex;
	int						m_nCurrentRenderSelect;
	int						m_nSelectTab;
	BOOL					m_bPlay_Mute;					// 플레이 버튼 상태.
	BOOL					m_bStop_Flag;					// 스탑 버튼 상태.
	char					m_strMp3Name[MAX_PATH];
	vector<int>				m_vecRandom;

	BOOL					m_bOriBGSound;					// 배경음악 파일 플레이중

public:
	BOOL					m_bMp3PlayListShow;
	BOOL					m_bBaseSizeFull;
	BOOL					m_bMp3Play;						// 음악 갱신 여부.
	vector<Mp3File_t>		m_vecMp3FileNames;
	CINFScrollBar		*	m_pScroll;
	BOOL					m_bSetMp3Dir;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int						m_nBkPosX;
	int						m_nBkPosY;
	float					m_fPlaytime;
	int						m_nPlaytimeGab;
	int		 	 		    m_nMoveFontPos;
#endif
};

#endif

