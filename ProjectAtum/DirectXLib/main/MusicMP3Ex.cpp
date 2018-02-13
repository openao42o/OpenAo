// MusicMP3Ex.cpp: implementation of the CMusicMP3Ex class.
//
//////////////////////////////////////////////////////////////////////
/*
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
*/
// #include "mp3uuids.h"
// #include "FunctionLog.h"
// #include "FunctionLogGFunctions.h"

#include "MusicMP3Ex.h"
#include <initguid.h>
//#include "mp3uuids.h"
#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMusicMP3Ex::CMusicMP3Ex()
{
	memset(m_wFileName, 0x00 ,sizeof(WCHAR)*1024);
	m_pBasicAudio = NULL;
	m_pGraph = NULL;
	m_pMediaControl = NULL;
	m_pEvent = NULL;
	m_pSeeking = NULL;
	m_pLPos = 0;
	m_pLStopPos = 0;
	m_bDefaultMusic = TRUE;
	m_bNowPlay = FALSE;

	// Initialize COM
	CoInitialize( NULL );

}

CMusicMP3Ex::~CMusicMP3Ex()
{
	ReleaseSound();
	CoUninitialize();	
}
HRESULT CMusicMP3Ex::Atum_MusicInit(TCHAR * strFileName)
{
	HRESULT	hr;

	MultiByteToWideChar(CP_ACP,0,strFileName,-1,m_wFileName,1024);	

	// 사운드 객체 제거
	ReleaseSound();

	// Create the graph
	if( FAILED(CoCreateInstance(	CLSID_FilterGraph, 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, 
		(void **)&m_pGraph )))
	{
		return E_FAIL;
	}
	
	// Query for interface objects
	m_pGraph->QueryInterface(IID_IBasicAudio, (void **)&m_pBasicAudio);		
	m_pGraph->QueryInterface( IID_IMediaControl, (void **)&m_pMediaControl );
	m_pGraph->QueryInterface( IID_IMediaEvent, (void **)&m_pEvent );
	m_pGraph->QueryInterface( IID_IMediaSeeking, (void **)&m_pSeeking );	
	
// 	WCHAR	wchTmp[MAX_PATH];
// 	for(int nCnt = 0; nCnt < (int)strlen(strFileName);nCnt++)
// 	{
// 		wchTmp[nCnt] = strFileName[nCnt];
// 	}
// 	wchTmp[nCnt] = '\0';
	hr = m_pGraph->RenderFile(m_wFileName, NULL);
	
	if( hr != S_OK ) {
		return( E_FAIL );
	}
	// Set the playback rate
	m_pSeeking->SetRate( 1.0f );
	// Play the song
	m_pMediaControl->Run();	

	return	S_OK;
}
HRESULT	CMusicMP3Ex::Atum_PlayMP3(long Volume)
{	
	//	if(Volumn > 0)
	//		Volumn = 0;
	// 미디어파일의 재생을 위한 IMediaControl포인터	
		
	if(!m_pGraph)
	{
		return E_FAIL;
	}	
	
	if(m_pBasicAudio)	
	{
		m_pBasicAudio->put_Volume(Volume);
	}
	if(m_pMediaControl)
	{
		m_pMediaControl->Run();
	}	
	
	return	S_OK;
}
HRESULT CMusicMP3Ex::Atum_MusicStop()
{
	if(!m_pGraph)
		return S_OK;
	
	if(m_pMediaControl)
	{
		m_pMediaControl->Stop();
	}
	
	
	LONGLONG pos=0L;
	
	// 미디어파일의 재생위치를 찾기 위한 IMediaSeeking 포인터
		
	// 정지시에는 위치를 맨앞으로 돌려놓는다.
	if(m_pSeeking)
	{
		m_pSeeking->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,NULL, AM_SEEKING_NoPositioning);
	}	
	

	return	S_OK;
}
HRESULT	CMusicMP3Ex::Atum_LoopMusic(LONGLONG LMute)
{
	
	if(!m_pGraph) 
	{
		return E_FAIL;
	}
	if(!m_pSeeking)
	{
		return E_FAIL;
	}
	
	
	{
		m_pSeeking->GetPositions(&m_pLPos,&m_pLStopPos);
		if(m_pLPos == m_pLStopPos)
			m_bNowPlay = FALSE;
		else
			m_bNowPlay = TRUE;
	}
	if(LMute != 0)
	{
		m_pLPos = LMute;
		m_pSeeking->SetPositions(&m_pLPos, AM_SEEKING_AbsolutePositioning,NULL, AM_SEEKING_NoPositioning);
	}		

	
	if(TRUE == m_bDefaultMusic)
	{
		if(m_pLPos >= m_pLStopPos)
		{
			m_pLPos = 0;
			if(m_pSeeking)
				m_pSeeking->SetPositions(&m_pLPos, AM_SEEKING_AbsolutePositioning,NULL, AM_SEEKING_NoPositioning);
		}
	}

	return	S_OK;
}

HRESULT	CMusicMP3Ex::SetAtumMusicVolume(long volume)
{		
	if(!m_pGraph)
		return E_FAIL;
	
		
	if(m_pBasicAudio)
	{
		m_pBasicAudio->put_Volume(volume);
	}	
	

	return	S_OK;

}

void CMusicMP3Ex::ReleaseSound() 
{
	// Stop the song if playing
	if(m_pMediaControl)
	{
		m_pMediaControl->Stop();
		// Clean up
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}
	if(m_pSeeking)
	{
		m_pSeeking->Release();
		m_pSeeking = NULL;		
	}

	if(m_pBasicAudio)
	{
		m_pBasicAudio->Release();
		m_pBasicAudio = NULL;
	}
	if(m_pEvent)
	{
		m_pEvent->Release();
		m_pEvent = NULL;
	}
	if(m_pGraph)
	{
		m_pGraph->Release();
		m_pGraph = NULL;
	}	
	
}

BOOL CMusicMP3Ex::IsNowPlay()
{
	return m_bNowPlay;
}