// MusicMP3Ex.h: interface for the CMusicMP3Ex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSICMP3EX_H__EFA209D7_920D_434D_B497_ADE2935AB137__INCLUDED_)
#define AFX_MUSICMP3EX_H__EFA209D7_920D_434D_B497_ADE2935AB137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dshow.h> 
#include <commdlg.h>

class CMusicMP3Ex  
{
public:
	CMusicMP3Ex();
	virtual ~CMusicMP3Ex();

	HRESULT Atum_MusicInit(TCHAR * strFileName);
	HRESULT	Atum_PlayMP3(long Volume);
	HRESULT Atum_MusicStop();
	HRESULT	Atum_LoopMusic(LONGLONG LMute = 0);
	
	HRESULT	SetAtumMusicVolume(long volume);
	
	BOOL IsNowPlay();
private:
	void ReleaseSound() ;

public:
	BOOL				m_bNowPlay;
	LONGLONG			m_pLPos;
	BOOL			m_bDefaultMusic;
	
private:
	WCHAR m_wFileName[1024];
	
	IGraphBuilder	*m_pGraph;
	IMediaControl	*m_pMediaControl;
	IMediaEvent		*m_pEvent;
	IMediaSeeking	*m_pSeeking;
	IBasicAudio		*m_pBasicAudio;
	
	
	LONGLONG			m_pLStopPos;
	
	
	
};

#endif // !defined(AFX_MUSICMP3EX_H__EFA209D7_920D_434D_B497_ADE2935AB137__INCLUDED_)
