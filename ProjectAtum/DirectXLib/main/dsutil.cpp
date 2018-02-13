//-----------------------------------------------------------------------------
// File: DSUtil.cpp
//
// Desc: DirectSound framework classes for reading and writing wav files and
//       playing them in DirectSound buffers. Feel free to use this class 
//       as a starting point for adding extra functionality.
//
// Copyright (c) 1999-2001 Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include <windows.h>
#include <mmsystem.h>
//#include <dxerr8.h>
#include <dsound.h>
#include "DSUtil.h"
#include "DXUtil.h"
#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"
#include <tchar.h>
#include "DbgOut_C.h"



#include "DefineGlobal.h"

#ifndef DEBUG
#define DXTRACE_ERR(x, y) y
#define DXTRACE_ERR_NOMSGBOX(x, y) y
#endif


#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2

//-----------------------------------------------------------------------------
// Name: CSoundManager::Initialize()
// Desc: Initializes the IDirectSound object and also sets the primary buffer
//       format.  This function must be called before any others.
//-----------------------------------------------------------------------------
HRESULT CSoundManager::Initialize( HWND  hWnd, 
                                   DWORD dwCoopLevel, 
                                   DWORD dwPrimaryChannels, 
                                   DWORD dwPrimaryFreq, 
                                   DWORD dwPrimaryBitRate )
{
	FLOG( "CSoundManager::Initialize( HWND  hWnd, DWORD dwCoopLevel, DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate )" );
    HRESULT             hr;

    SAFE_RELEASE( m_pDS );

    // Create IDirectSound using the primary sound device
    if( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS, NULL ) ) )
        return DXTRACE_ERR( TEXT("DirectSoundCreate8"), hr );

    // Set DirectSound coop level 
    if( FAILED( hr = m_pDS->SetCooperativeLevel( hWnd, dwCoopLevel ) ) )
        return DXTRACE_ERR( TEXT("SetCooperativeLevel"), hr );
    
    // Set primary buffer format
    if( FAILED( hr = SetPrimaryBufferFormat( dwPrimaryChannels, dwPrimaryFreq, dwPrimaryBitRate ) ) )
        return DXTRACE_ERR( TEXT("SetPrimaryBufferFormat"), hr );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::SetPrimaryBufferFormat()
// Desc: Set primary buffer to a specified format 
//       For example, to set the primary buffer format to 22kHz stereo, 16-bit
//       then:   dwPrimaryChannels = 2
//               dwPrimaryFreq     = 22050, 
//               dwPrimaryBitRate  = 16
//-----------------------------------------------------------------------------
HRESULT CSoundManager::SetPrimaryBufferFormat(DWORD dwPrimaryChannels, 
                                              DWORD dwPrimaryFreq, 
                                              DWORD dwPrimaryBitRate) const
{
	FLOG( "CSoundManager::SetPrimaryBufferFormat( DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate )" );
    HRESULT             hr;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    if( m_pDS == NULL )
        return CO_E_NOTINITIALIZED;

    // Get the primary buffer 
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
    if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbd, &pDSBPrimary, NULL ) ) )
        return DXTRACE_ERR( TEXT("CreateSoundBuffer"), hr );

    WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = (WORD) dwPrimaryChannels; 
    wfx.nSamplesPerSec  = dwPrimaryFreq; 
    wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate; 
    wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hr = pDSBPrimary->SetFormat(&wfx) ) )
        return DXTRACE_ERR( TEXT("SetFormat"), hr );

    SAFE_RELEASE( pDSBPrimary );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::Get3DListenerInterface()
// Desc: Returns the 3D listener interface associated with primary buffer.
//-----------------------------------------------------------------------------
HRESULT CSoundManager::Get3DListenerInterface(LPDIRECTSOUND3DLISTENER* ppDSListener) const
{
	FLOG( "CSoundManager::Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener )" );
    HRESULT             hr;
    DSBUFFERDESC        dsbdesc;
    LPDIRECTSOUNDBUFFER pDSBPrimary = NULL;

    if( ppDSListener == NULL )
        return E_INVALIDARG;
    if( m_pDS == NULL )
        return CO_E_NOTINITIALIZED;

    *ppDSListener = NULL;

    // Obtain primary buffer, asking it for 3D control
    ZeroMemory( &dsbdesc, sizeof(DSBUFFERDESC) );
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
    if( FAILED( hr = m_pDS->CreateSoundBuffer( &dsbdesc, &pDSBPrimary, NULL ) ) )
        return DXTRACE_ERR( TEXT("CreateSoundBuffer"), hr );

    if( FAILED( hr = pDSBPrimary->QueryInterface( IID_IDirectSound3DListener, 
                                                  (VOID**)ppDSListener ) ) )
    {
        SAFE_RELEASE( pDSBPrimary );
        return DXTRACE_ERR( TEXT("QueryInterface"), hr );
    }

    // Release the primary buffer, since it is not need anymore
    SAFE_RELEASE( pDSBPrimary );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CSoundManager::Create()
// Desc: creates a CSound
//-----------------------------------------------------------------------------
//HRESULT CSoundManager::Create( CSound** ppSound, 
//                               LPTSTR strWaveFileName, 
//                               DWORD dwCreationFlags, 
//                               GUID guid3DAlgorithm,
//                               DWORD dwNumBuffers )
//{
//	FLOG( "CSoundManager::Create( CSound** ppSound, LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm, DWORD dwNumBuffers )" );
//    HRESULT hr;
//    HRESULT hrRet = S_OK;
//    DWORD   i;
//    LPDIRECTSOUNDBUFFER* apDSBuffer     = NULL;
//    DWORD                dwDSBufferSize = NULL;
//    CWaveFile*           pWaveFile      = NULL;
//
//    if( m_pDS == NULL )
//        return CO_E_NOTINITIALIZED;
//    if( strWaveFileName == NULL || ppSound == NULL || dwNumBuffers < 1 )
//        return E_INVALIDARG;
//
//    apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
//    if( apDSBuffer == NULL )
//    {
//        hr = E_OUTOFMEMORY;
//        goto LFail;
//    }
//
//    pWaveFile = new CWaveFile();
//    if( pWaveFile == NULL )
//    {
//        hr = E_OUTOFMEMORY;
//        goto LFail;
//    }
//
//    pWaveFile->Open( strWaveFileName, NULL, WAVEFILE_READ );
//
//    if( pWaveFile->GetSize() == 0 )
//    {
//        // Wave is blank, so don't create it.
//        hr = E_FAIL;
//        goto LFail;
//    }
//
//    // Make the DirectSound buffer the same size as the wav file
//    dwDSBufferSize = pWaveFile->GetSize();
//
//    // Create the direct sound buffer, and only request the flags needed
//    // since each requires some overhead and limits if the buffer can 
//    // be hardware accelerated
//    DSBUFFERDESC dsbd;
//    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
//    dsbd.dwSize          = sizeof(DSBUFFERDESC);
//    dsbd.dwFlags         = dwCreationFlags;
//    dsbd.dwBufferBytes   = dwDSBufferSize;
//    dsbd.guid3DAlgorithm = guid3DAlgorithm;
//    dsbd.lpwfxFormat     = pWaveFile->m_pwfx;
//
//    // DirectSound is only guarenteed to play PCM data.  Other
//    // formats may or may not work depending the sound card driver.
//    hr = m_pDS->CreateSoundBuffer( &dsbd, &apDSBuffer[0], NULL );
//
//    // Be sure to return this error code if it occurs so the
//    // callers knows this happened.
//    if( hr == DS_NO_VIRTUALIZATION )
//        hrRet = DS_NO_VIRTUALIZATION;
//            
//    if( FAILED(hr) )
//    {
//        // DSERR_BUFFERTOOSMALL will be returned if the buffer is
//        // less than DSBSIZE_FX_MIN (100ms) and the buffer is created
//        // with DSBCAPS_CTRLFX.
//        if( hr != DSERR_BUFFERTOOSMALL )
//            DXTRACE_ERR( TEXT("CreateSoundBuffer"), hr );
//            
//        goto LFail;
//    }
//
//    for( i=1; i<dwNumBuffers; i++ )
//    {
//        if( FAILED( hr = m_pDS->DuplicateSoundBuffer( apDSBuffer[0], &apDSBuffer[i] ) ) )
//        {
//            DXTRACE_ERR( TEXT("DuplicateSoundBuffer"), hr );
//            goto LFail;
//        }
//    }
//
//    // Create the sound
//    *ppSound = new CSound( apDSBuffer, dwDSBufferSize, dwNumBuffers, pWaveFile );
//    
//    util::del( apDSBuffer );
//    return hrRet;
//
//LFail:
//    // Cleanup
//    util::del( pWaveFile );
//    util::del( apDSBuffer );
//    return hr;
//}
//
//
//
//





//-----------------------------------------------------------------------------
// Name: CSoundManager::CreateFromMemory()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CSoundManager::CreateFromMemory( CSound** ppSound, 
                                        BYTE* pbData,
                                        ULONG  ulDataSize,
                                        LPWAVEFORMATEX pwfx,
                                        DWORD dwCreationFlags, 
                                        GUID guid3DAlgorithm,
                                        DWORD dwNumBuffers,
										BOOL i_b3DSoundFlag/*=TRUE*/)
{
	FLOG( "CSoundManager::CreateFromMemory( CSound** ppSound, BYTE* pbData, ULONG  ulDataSize, LPWAVEFORMATEX pwfx, DWORD dwCreationFlags, GUID guid3DAlgorithm, DWORD dwNumBuffers )" );
    HRESULT hr;
    DWORD                dwDSBufferSize = NULL;
    CWaveFile*           pWaveFile      = NULL;

    if( m_pDS == NULL )
        return CO_E_NOTINITIALIZED;
    if( pbData == NULL || ppSound == NULL || dwNumBuffers < 1 )
        return E_INVALIDARG;

    auto apDSBuffer = new LPDIRECTSOUNDBUFFER[dwNumBuffers];
    
	if (!apDSBuffer)
    {
        hr = E_OUTOFMEMORY;
        goto myexit;
    }

    pWaveFile = new CWaveFile();

    if (!pWaveFile)
    {
        hr = E_OUTOFMEMORY;
        goto myexit;
    }

    pWaveFile->OpenFromMemory( pbData,ulDataSize, pwfx, WAVEFILE_READ );


    // Make the DirectSound buffer the same size as the wav file
    dwDSBufferSize = ulDataSize;

    // Create the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can 
    // be hardware accelerated
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = dwCreationFlags;
    dsbd.dwBufferBytes   = dwDSBufferSize;
    dsbd.guid3DAlgorithm = guid3DAlgorithm;
    dsbd.lpwfxFormat     = pwfx;

    if (FAILED(hr = m_pDS->CreateSoundBuffer(&dsbd, &apDSBuffer[0], NULL)))
    {
        DXTRACE_ERR( TEXT("CreateSoundBuffer"), hr );
        goto myexit;
    }

    for (auto i = 1; i < dwNumBuffers; ++i)
    {
        if (FAILED(hr = m_pDS->DuplicateSoundBuffer(apDSBuffer[0], &apDSBuffer[i])))
        {
            DXTRACE_ERR(TEXT("DuplicateSoundBuffer"), hr);
            goto myexit;
        }
    }

    // Create the sound
    *ppSound = new CSound(apDSBuffer, dwDSBufferSize, dwNumBuffers, pWaveFile, i_b3DSoundFlag);

	hr = S_OK;

myexit:
    // Cleanup
   
    delete apDSBuffer;

    return hr;
}




//-----------------------------------------------------------------------------
// Name: CSound::CSound()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
CSound::CSound(LPDIRECTSOUNDBUFFER* apDSBuffer, 
			   DWORD dwDSBufferSize, 
               DWORD dwNumBuffers, 
			   CWaveFile* pWaveFile,
			   BOOL i_b3DSoundFlag/*=TRUE*/)
{
	FLOG( "CSound( LPDIRECTSOUNDBUFFER* apDSBuffer, DWORD dwDSBufferSize,DWORD dwNumBuffers, CWaveFile* pWaveFile )" );
    
	m_b3DSoundFlag		= i_b3DSoundFlag;
	m_nSoundType		= 0;
	m_nCurrentIndex		= 0;


    m_pSCDirect3DSound = new SCDIRECT_3DSOUND[dwNumBuffers];
	DWORD i;
    for(i=0; i<dwNumBuffers; i++ )
	{
		m_pSCDirect3DSound[i].pDSBuffer		= apDSBuffer[i];
		m_pSCDirect3DSound[i].pDS3DBuffer	= NULL;
		memset(&m_pSCDirect3DSound[i].ds3DBufferParams, 0x00, sizeof(DS3DBUFFER));
	}

    m_dwDSBufferSize = dwDSBufferSize;
    m_dwNumBuffers   = dwNumBuffers;
    m_pWaveFile      = pWaveFile;
    
    for( i=0; i<dwNumBuffers; i++ )
    {		
		FillBufferWithSound(m_pSCDirect3DSound[i].pDSBuffer, FALSE );

		// Make DirectSound do pre-processing on sound effects
		m_pSCDirect3DSound[i].pDSBuffer->SetCurrentPosition(0);
		
		if(m_b3DSoundFlag)
		{
			// Get the 3D buffer from the secondary buffer
			Get3DBufferInterface(i, &m_pSCDirect3DSound[i].pDS3DBuffer);

			// Get the 3D buffer parameters
			m_pSCDirect3DSound[i].ds3DBufferParams.dwSize = sizeof(DS3DBUFFER);
			m_pSCDirect3DSound[i].pDS3DBuffer->GetAllParameters(&m_pSCDirect3DSound[i].ds3DBufferParams);
			
			// Set new 3D buffer parameters
			m_pSCDirect3DSound[i].ds3DBufferParams.dwMode = DS3DMODE_HEADRELATIVE;
			m_pSCDirect3DSound[i].ds3DBufferParams.flMinDistance = 200.0f;
			m_pSCDirect3DSound[i].ds3DBufferParams.flMaxDistance = 1000.0f;
			m_pSCDirect3DSound[i].pDS3DBuffer->SetAllParameters(&m_pSCDirect3DSound[i].ds3DBufferParams, DS3D_IMMEDIATE );
		}
	}
}




//-----------------------------------------------------------------------------
// Name: CSound::~CSound()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
CSound::~CSound()
{
	FLOG("~CSound()");
	
	for (auto i = 0; i < m_dwNumBuffers; ++i)
	{
		SAFE_RELEASE(m_pSCDirect3DSound[i].pDS3DBuffer);
		SAFE_RELEASE(m_pSCDirect3DSound[i].pDSBuffer);
	}

    util::del_array(m_pSCDirect3DSound); 
    util::del(m_pWaveFile);
}




//-----------------------------------------------------------------------------
// Name: CSound::FillBufferWithSound()
// Desc: Fills a DirectSound buffer with a sound file 
//-----------------------------------------------------------------------------
HRESULT CSound::FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger) const
{
	FLOG( "CSound::FillBufferWithSound( LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger )" );
    HRESULT hr; 
    VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

    if( pDSB == NULL )
        return CO_E_NOTINITIALIZED;

    // Make sure we have focus, and we didn't just switch in from
    // an app which had a DirectSound device
    if( FAILED( hr = RestoreBuffer( pDSB, NULL ) ) ) 
        return DXTRACE_ERR( TEXT("RestoreBuffer"), hr );

    // Lock the buffer down
    if( FAILED( hr = pDSB->Lock( 0, m_dwDSBufferSize, 
                                 &pDSLockedBuffer, &dwDSLockedBufferSize, 
                                 NULL, NULL, 0L ) ) )
        return DXTRACE_ERR( TEXT("Lock"), hr );

    // Reset the wave file to the beginning 
    m_pWaveFile->ResetFile();

    if( FAILED( hr = m_pWaveFile->Read( (BYTE*) pDSLockedBuffer,
                                        dwDSLockedBufferSize, 
                                        &dwWavDataRead ) ) )           
        return DXTRACE_ERR( TEXT("Read"), hr );

    if( dwWavDataRead == 0 )
    {
        // Wav is blank, so just fill with silence
        FillMemory( (BYTE*) pDSLockedBuffer, 
                    dwDSLockedBufferSize, 
                    (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }
    else if( dwWavDataRead < dwDSLockedBufferSize )
    {
        // If the wav file was smaller than the DirectSound buffer, 
        // we need to fill the remainder of the buffer with data 
        if( bRepeatWavIfBufferLarger )
        {       
            // Reset the file and fill the buffer with wav data
            DWORD dwReadSoFar = dwWavDataRead;    // From previous call above.
            while( dwReadSoFar < dwDSLockedBufferSize )
            {  
                // This will keep reading in until the buffer is full 
                // for very short files
                if( FAILED( hr = m_pWaveFile->ResetFile() ) )
                    return DXTRACE_ERR( TEXT("ResetFile"), hr );

                hr = m_pWaveFile->Read( (BYTE*)pDSLockedBuffer + dwReadSoFar,
                                        dwDSLockedBufferSize - dwReadSoFar,
                                        &dwWavDataRead );
                if( FAILED(hr) )
                    return DXTRACE_ERR( TEXT("Read"), hr );

                dwReadSoFar += dwWavDataRead;
            } 
        }
        else
        {
            // Don't repeat the wav file, just fill in silence 
            FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
                        dwDSLockedBufferSize - dwWavDataRead, 
                        (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
        }
    }

    // Unlock the buffer, we don't need it anymore.
    pDSB->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

    return S_OK;
}





// Desc: Restores the lost buffer. *pbWasRestored returns TRUE if the buffer was 
//       restored.  It can also be NULL if the information is not needed.
HRESULT CSound::RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB, bool* pbWasRestored)
{
    if (!pDSB) return CO_E_NOTINITIALIZED;

    if (pbWasRestored) *pbWasRestored = false;

    HRESULT hr;
    DWORD dwStatus;

    if (FAILED(hr = pDSB->GetStatus( &dwStatus)))

        return DXTRACE_ERR(TEXT("GetStatus"), hr);

    if (dwStatus & DSBSTATUS_BUFFERLOST)
    {
        // Since the app could have just been activated, then DirectSound 
        // may not be giving us control yet, so restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do 
        {
            hr = pDSB->Restore();

            if (hr == DSERR_BUFFERLOST) Sleep(10);
        }
        while (hr != DS_OK);

        if (pbWasRestored)  *pbWasRestored = true;

        return S_OK;
    }
	
	return S_FALSE;
}

// Finds the next sound buffer that is not playing or the sound buffer that will finish next.
SCDIRECT_3DSOUND * CSound::GetFreeSCDirect3DSound()
{
	FLOG("CSound::GetFreeBuffer()");

	if (!m_pSCDirect3DSound) return nullptr;

    for (auto i = 0; i < m_dwNumBuffers; ++i)
    {
        if(m_pSCDirect3DSound[m_nCurrentIndex].pDSBuffer)
        {
            DWORD dwStatus = 0;
            m_pSCDirect3DSound[m_nCurrentIndex].pDSBuffer->GetStatus( &dwStatus );
            if (( dwStatus & DSBSTATUS_PLAYING ) == 0)
            {				
				break;
			}
        }
		m_nCurrentIndex = ++m_nCurrentIndex % m_dwNumBuffers;
    }

	auto pRet = m_pSCDirect3DSound + m_nCurrentIndex;

	m_nCurrentIndex = ++m_nCurrentIndex % m_dwNumBuffers;

	return pRet;
}




//-----------------------------------------------------------------------------
// Desc: Checks if the internal array is allocated and index is within bounds.
// Ret: m_pSCDirect3DSound + dwIndex or nullptr
//-----------------------------------------------------------------------------
SCDIRECT_3DSOUND* CSound::GetBuffer(DWORD dwIndex) const
{
	FLOG("CSound::GetBuffer(DWORD dwIndex)");

	return m_pSCDirect3DSound != nullptr && dwIndex < m_dwNumBuffers ?
		
		m_pSCDirect3DSound + dwIndex : nullptr;
}




//-----------------------------------------------------------------------------
// Name: CSound::Get3DBufferInterface()
// Desc: 
//-----------------------------------------------------------------------------
HRESULT CSound::Get3DBufferInterface( DWORD dwIndex, LPDIRECTSOUND3DBUFFER* ppDS3DBuffer)
{
	FLOG("CSound::Get3DBufferInterface(DWORD dwIndex, LPDIRECTSOUND3DBUFFER* ppDS3DBuffer)");

    if (!m_pSCDirect3DSound) return CO_E_NOTINITIALIZED;

    if (dwIndex >= m_dwNumBuffers) return E_INVALIDARG;

    *ppDS3DBuffer = nullptr;

    return m_pSCDirect3DSound[dwIndex].pDSBuffer->QueryInterface( IID_IDirectSound3DBuffer, 
                                                  (VOID**)ppDS3DBuffer );
}


//-----------------------------------------------------------------------------
// Name: CSound::Play()
// Desc: Plays the sound using voice management flags.  Pass in DSBPLAY_LOOPING
//       in the dwFlags to loop the sound
//-----------------------------------------------------------------------------
HRESULT CSound::Play(DWORD dwPriority/* = 0*/, DWORD dwFlags/* = 0*/, LONG lVolume/*=0*/)
{
	FLOG( "CSound::Play( DWORD dwPriority, DWORD dwFlags )" );
    HRESULT hr;
    bool    bRestored;

    if (!m_pSCDirect3DSound) return CO_E_NOTINITIALIZED;

	auto pSC3DSound = GetFreeSCDirect3DSound();

    if (!pSC3DSound) return DXTRACE_ERR(TEXT("GetFreeBuffer"), E_FAIL);
	
    // Restore the buffer if it was lost
    if (FAILED(hr = RestoreBuffer(pSC3DSound->pDSBuffer, &bRestored)))

        return DXTRACE_ERR(TEXT("RestoreBuffer"), hr);

    if (bRestored)
    {
        // The buffer was restored, so we need to fill it with new data
        if (FAILED(hr = FillBufferWithSound(pSC3DSound->pDSBuffer, FALSE)))
            
			return DXTRACE_ERR(TEXT("FillBufferWithSound"), hr );

        // Make DirectSound do pre-processing on sound effects
        Reset();
    }

	if (lVolume > 0)
		lVolume = 0;

	if (FAILED(hr = pSC3DSound->pDSBuffer->SetVolume(lVolume)))
		
		return DXTRACE_ERR(TEXT("SetVolume"), hr);

	DWORD dwStatus = 0;

    pSC3DSound->pDSBuffer->GetStatus(&dwStatus);  
	pSC3DSound->pDSBuffer->SetCurrentPosition(0); 

    if ((dwStatus & DSBSTATUS_PLAYING) != 0)
    { 
		// pSC3DSound->pDSBuffer->Stop();
	}

	if (m_b3DSoundFlag)
	{
		pSC3DSound->pDS3DBuffer->SetPosition(m_Vec3SoundPosition.x, m_Vec3SoundPosition.y, m_Vec3SoundPosition.z, DS3D_IMMEDIATE);
	}

	if ((dwStatus & DSBSTATUS_PLAYING) == 0)
	{
		return pSC3DSound->pDSBuffer->Play(0, dwPriority, dwFlags);
	}

	return S_OK;	
}



//-----------------------------------------------------------------------------
// Name: CSound::Stop()
// Desc: Stops the sound from playing
//-----------------------------------------------------------------------------
HRESULT CSound::Stop()
{
	FLOG( "CSound::Stop()" );
    if( m_pSCDirect3DSound == NULL )
        return CO_E_NOTINITIALIZED;

    auto hr = S_OK;

    for( DWORD i=0; i<m_dwNumBuffers; i++ )
	{		
        hr |= m_pSCDirect3DSound[i].pDSBuffer->Stop();
	}

    return hr;
}




//-----------------------------------------------------------------------------
// Name: CSound::Reset()
// Desc: Reset all of the sound buffers
//-----------------------------------------------------------------------------
HRESULT CSound::Reset()
{
	FLOG( "CSound::Reset()" );
    if( m_pSCDirect3DSound == NULL )
        return CO_E_NOTINITIALIZED;

    HRESULT hr = 0;

    for( DWORD i=0; i<m_dwNumBuffers; i++ )
	{
		hr |= m_pSCDirect3DSound[i].pDSBuffer->SetCurrentPosition( 0 );
	}

    return hr;
}




//-----------------------------------------------------------------------------
// Name: CSound::IsSoundPlaying()
// Desc: Checks to see if a buffer is playing and returns TRUE if it is.
//-----------------------------------------------------------------------------
BOOL CSound::IsSoundPlaying() const
{
	if (m_pSCDirect3DSound)
		
		for (auto i = 0; i < m_dwNumBuffers; ++i)
			
			if (m_pSCDirect3DSound[i].pDSBuffer)
			{
				DWORD dwStatus = 0;

				m_pSCDirect3DSound[i].pDSBuffer->GetStatus(&dwStatus);

				if ((dwStatus & DSBSTATUS_PLAYING) != 0)
					
					return true;
			}
	
    return false;
}

void CSound::Set3DSoundPosition(D3DXVECTOR3 *i_pVec3Position)
{
	if(D3DXVec3Length(i_pVec3Position) < 10)
	{
		m_Vec3SoundPosition = D3DXVECTOR3(0,0,0);
	}
	else
	{
		m_Vec3SoundPosition = *i_pVec3Position;	
	}
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::CWaveFile()
// Desc: Constructs the class.  Call Open() to open a wave file for reading.  
//       Then call Read() as needed.  Calling the destructor or Close() 
//       will close the file.  
//-----------------------------------------------------------------------------
CWaveFile::CWaveFile() :
	m_pwfx { nullptr },
	m_hmmio { nullptr },
	m_dwSize { 0 },
	m_dwFlags { 0 },
	m_bIsReadingFromMemory { false },
	m_pbData { nullptr },
	m_pbDataCur { nullptr },
	m_ulDataSize { 0 },
	m_pResourceBuffer { nullptr }
{
}


//-----------------------------------------------------------------------------
// Name: CWaveFile::~CWaveFile()
// Desc: Destructs the class
//-----------------------------------------------------------------------------
CWaveFile::~CWaveFile()
{
    Close();

    if (!m_bIsReadingFromMemory)

        util::del_array(m_pwfx);
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Open()
// Desc: Opens a wave file for reading
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Open(LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags)
{
    HRESULT hr;

    m_dwFlags = dwFlags;
    m_bIsReadingFromMemory = FALSE;

    if (m_dwFlags == WAVEFILE_READ)
    {
        if (!strFileName) return E_INVALIDARG;

        util::del_array(m_pwfx);

        m_hmmio = mmioOpen(strFileName, nullptr, MMIO_ALLOCBUF | MMIO_READ);

        if (!m_hmmio)
        {
            HRSRC   hResInfo;
            HGLOBAL hResData;
            DWORD   dwSize;
            VOID*   pvRes;

            // Loading it as a file failed, so try it as a resource
            if( NULL == ( hResInfo = FindResource( NULL, strFileName, TEXT("WAVE") ) ) )
            {
                if( NULL == ( hResInfo = FindResource( NULL, strFileName, TEXT("WAV") ) ) )
                    return DXTRACE_ERR_NOMSGBOX( TEXT("FindResource"), E_FAIL );
            }

            if( NULL == ( hResData = LoadResource( NULL, hResInfo ) ) )
                return DXTRACE_ERR( TEXT("LoadResource"), E_FAIL );

            if( 0 == ( dwSize = SizeofResource( NULL, hResInfo ) ) ) 
                return DXTRACE_ERR( TEXT("SizeofResource"), E_FAIL );

            if( NULL == ( pvRes = LockResource( hResData ) ) )
                return DXTRACE_ERR( TEXT("LockResource"), E_FAIL );

            m_pResourceBuffer = new CHAR[ dwSize ];
            memcpy( m_pResourceBuffer, pvRes, dwSize );

            MMIOINFO mmioInfo;
            ZeroMemory( &mmioInfo, sizeof(mmioInfo) );
            mmioInfo.fccIOProc = FOURCC_MEM;
            mmioInfo.cchBuffer = dwSize;
            mmioInfo.pchBuffer = (CHAR*) m_pResourceBuffer;

            m_hmmio = mmioOpen( NULL, &mmioInfo, MMIO_ALLOCBUF | MMIO_READ );
        }

        if( FAILED( hr = ReadMMIO() ) )
        {
            // ReadMMIO will fail if its an not a wave file
            mmioClose( m_hmmio, 0 );
            return DXTRACE_ERR_NOMSGBOX( TEXT("ReadMMIO"), hr );
        }

        if( FAILED( hr = ResetFile() ) )
            return DXTRACE_ERR( TEXT("ResetFile"), hr );

        // After the reset, the size of the wav file is m_ck.cksize so store it now
        m_dwSize = m_ck.cksize;
    }
    else
    {
        m_hmmio = mmioOpen( strFileName, NULL, MMIO_ALLOCBUF  | 
                                                  MMIO_READWRITE | 
                                                  MMIO_CREATE );
        if( NULL == m_hmmio )
            return DXTRACE_ERR( TEXT("mmioOpen"), E_FAIL );

        if( FAILED( hr = WriteMMIO( pwfx ) ) )
        {
            mmioClose( m_hmmio, 0 );
            return DXTRACE_ERR( TEXT("WriteMMIO"), hr );
        }
                        
        if( FAILED( hr = ResetFile() ) )
            return DXTRACE_ERR( TEXT("ResetFile"), hr );
    }

    return hr;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::OpenFromMemory()
// Desc: copy data to CWaveFile member variable from memory
//-----------------------------------------------------------------------------
HRESULT CWaveFile::OpenFromMemory( BYTE* pbData, 
								  ULONG ulDataSize, 
                                  WAVEFORMATEX* pwfx, 
								  DWORD dwFlags )
{
	FLOG( "CWaveFile::OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags )" );
    m_pwfx       = pwfx;
    m_ulDataSize = ulDataSize;
    m_pbData     = pbData;
    m_pbDataCur  = m_pbData;
    m_bIsReadingFromMemory = TRUE;
    
    if( dwFlags != WAVEFILE_READ )
        return E_NOTIMPL;       
    
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::ReadMMIO()
// Desc: Support function for reading from a multimedia I/O stream.
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_pwfx. 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::ReadMMIO()
{
	FLOG( "CWaveFile::ReadMMIO()" );
    MMCKINFO        ckIn;           // chunk info. for general use.
    PCMWAVEFORMAT   pcmWaveFormat;  // Temp PCM structure to load in.       

    m_pwfx = NULL;

    if( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) ) )
        return DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );

    // Check to make sure this is a valid wave file
    if( (m_ckRiff.ckid != FOURCC_RIFF) ||
        (m_ckRiff.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
        return DXTRACE_ERR_NOMSGBOX( TEXT("mmioFOURCC"), E_FAIL ); 

    // Search the input file for for the 'fmt ' chunk.
    ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) )
        return DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );

    // Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
    // if there are extra parameters at the end, we'll ignore them
       if( ckIn.cksize < (LONG) sizeof(PCMWAVEFORMAT) )
           return DXTRACE_ERR( TEXT("sizeof(PCMWAVEFORMAT)"), E_FAIL );

    // Read the 'fmt ' chunk into <pcmWaveFormat>.
    if( mmioRead( m_hmmio, (HPSTR) &pcmWaveFormat, 
                  sizeof(pcmWaveFormat)) != sizeof(pcmWaveFormat) )
        return DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );

    // Allocate the waveformatex, but if its not pcm format, read the next
    // word, and thats how many extra bytes to allocate.
    if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
    {
        m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
        if( NULL == m_pwfx )
            return DXTRACE_ERR( TEXT("m_pwfx"), E_FAIL );

        // Copy the bytes from the pcm structure to the waveformatex structure
        memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        m_pwfx->cbSize = 0;
    }
    else
    {
        // Read in length of extra bytes.
        WORD cbExtraBytes = 0L;
        if( mmioRead( m_hmmio, (CHAR*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD) )
            return DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );

        m_pwfx = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) + cbExtraBytes ];
        if( NULL == m_pwfx )
            return DXTRACE_ERR( TEXT("new"), E_FAIL );

        // Copy the bytes from the pcm structure to the waveformatex structure
        memcpy( m_pwfx, &pcmWaveFormat, sizeof(pcmWaveFormat) );
        m_pwfx->cbSize = cbExtraBytes;

        // Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
        if( mmioRead( m_hmmio, (CHAR*)(((BYTE*)&(m_pwfx->cbSize))+sizeof(WORD)),
                      cbExtraBytes ) != cbExtraBytes )
        {
            util::del( m_pwfx );
            return DXTRACE_ERR( TEXT("mmioRead"), E_FAIL );
        }
    }

    // Ascend the input file out of the 'fmt ' chunk.
    if( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) )
    {
        util::del( m_pwfx );
        return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::GetSize()
// Desc: Retuns the size of the read access wave file 
//-----------------------------------------------------------------------------
DWORD CWaveFile::GetSize()
{
	FLOG( "CWaveFile::GetSize()" );
    return m_dwSize;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::ResetFile()
// Desc: Resets the internal m_ck pointer so reading starts from the 
//       beginning of the file again 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::ResetFile()
{
	FLOG( "CWaveFile::ResetFile()" );
    if( m_bIsReadingFromMemory )
    {
        m_pbDataCur = m_pbData;
    }
    else 
    {
        if( m_hmmio == NULL )
            return CO_E_NOTINITIALIZED;

        if( m_dwFlags == WAVEFILE_READ )
        {
            // Seek to the data
            if( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof(FOURCC),
                            SEEK_SET ) )
                return DXTRACE_ERR( TEXT("mmioSeek"), E_FAIL );

            // Search the input file for the 'data' chunk.
            m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
            if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) )
              return DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );
        }
        else
        {
            // Create the 'data' chunk that holds the waveform samples.  
            m_ck.ckid = mmioFOURCC('d', 'a', 't', 'a');
            m_ck.cksize = 0;

            if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) ) 
                return DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );

            if( 0 != mmioGetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
                return DXTRACE_ERR( TEXT("mmioGetInfo"), E_FAIL );
        }
    }
    
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Read()
// Desc: Reads section of data from a wave file into pBuffer and returns 
//       how much read in pdwSizeRead, reading not more than dwSizeToRead.
//       This uses m_ck to determine where to start reading from.  So 
//       subsequent calls will be continue where the last left off unless 
//       Reset() is called.
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
	FLOG( "CWaveFile::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )" );
    if( m_bIsReadingFromMemory )
    {
        if( m_pbDataCur == NULL )
            return CO_E_NOTINITIALIZED;
        if( pdwSizeRead != NULL )
            *pdwSizeRead = 0;

        if( (BYTE*)(m_pbDataCur + dwSizeToRead) > 
            (BYTE*)(m_pbData + m_ulDataSize) )
        {
            dwSizeToRead = m_ulDataSize - (DWORD)(m_pbDataCur - m_pbData);
        }
        
        CopyMemory( pBuffer, m_pbDataCur, dwSizeToRead );
        
        if( pdwSizeRead != NULL )
            *pdwSizeRead = dwSizeToRead;

        return S_OK;
    }
    else 
    {
        MMIOINFO mmioinfoIn; // current status of m_hmmio

        if( m_hmmio == NULL )
            return CO_E_NOTINITIALIZED;
        if( pBuffer == NULL || pdwSizeRead == NULL )
            return E_INVALIDARG;

        if( pdwSizeRead != NULL )
            *pdwSizeRead = 0;

        if( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) )
            return DXTRACE_ERR( TEXT("mmioGetInfo"), E_FAIL );
                
        UINT cbDataIn = dwSizeToRead;
        if( cbDataIn > m_ck.cksize ) 
            cbDataIn = m_ck.cksize;       

        m_ck.cksize -= cbDataIn;
    
        for( DWORD cT = 0; cT < cbDataIn; cT++ )
        {
            // Copy the bytes from the io to the buffer.
            if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
            {
                if( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) )
                    return DXTRACE_ERR( TEXT("mmioAdvance"), E_FAIL );

                if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
                    return DXTRACE_ERR( TEXT("mmioinfoIn.pchNext"), E_FAIL );
            }

            // Actual copy.
            *((BYTE*)pBuffer+cT) = *((BYTE*)mmioinfoIn.pchNext);
            mmioinfoIn.pchNext++;
        }

        if( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) )
            return DXTRACE_ERR( TEXT("mmioSetInfo"), E_FAIL );

        if( pdwSizeRead != NULL )
            *pdwSizeRead = cbDataIn;

        return S_OK;
    }
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Close()
// Desc: Closes the wave file 
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Close()
{
	FLOG( "CWaveFile::Close()" );
    if( m_dwFlags == WAVEFILE_READ )
    {
        mmioClose( m_hmmio, 0 );
        m_hmmio = NULL;
        util::del_array( m_pResourceBuffer );
    }
    else
    {
        m_mmioinfoOut.dwFlags |= MMIO_DIRTY;

        if( m_hmmio == NULL )
            return CO_E_NOTINITIALIZED;

        if( 0 != mmioSetInfo( m_hmmio, &m_mmioinfoOut, 0 ) )
            return DXTRACE_ERR( TEXT("mmioSetInfo"), E_FAIL );
    
        // Ascend the output file out of the 'data' chunk -- this will cause
        // the chunk size of the 'data' chunk to be written.
        if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
            return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
    
        // Do this here instead...
        if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
            return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
        
        mmioSeek( m_hmmio, 0, SEEK_SET );

        if( 0 != (INT)mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) )
            return DXTRACE_ERR( TEXT("mmioDescend"), E_FAIL );
    
        m_ck.ckid = mmioFOURCC('f', 'a', 'c', 't');

        if( 0 == mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) ) 
        {
            DWORD dwSamples = 0;
            mmioWrite( m_hmmio, (HPSTR)&dwSamples, sizeof(DWORD) );
            mmioAscend( m_hmmio, &m_ck, 0 ); 
        }
    
        // Ascend the output file out of the 'RIFF' chunk -- this will cause
        // the chunk size of the 'RIFF' chunk to be written.
        if( 0 != mmioAscend( m_hmmio, &m_ckRiff, 0 ) )
            return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
    
        mmioClose( m_hmmio, 0 );
        m_hmmio = NULL;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::WriteMMIO()
// Desc: Support function for reading from a multimedia I/O stream
//       pwfxDest is the WAVEFORMATEX for this new wave file.  
//       m_hmmio must be valid before calling.  This function uses it to
//       update m_ckRiff, and m_ck.  
//-----------------------------------------------------------------------------
HRESULT CWaveFile::WriteMMIO( WAVEFORMATEX *pwfxDest )
{
	FLOG( "CWaveFile::WriteMMIO( WAVEFORMATEX *pwfxDest )" );
    DWORD    dwFactChunk; // Contains the actual fact chunk. Garbage until WaveCloseWriteFile.
    MMCKINFO ckOut1;
    
    dwFactChunk = (DWORD)-1;

    // Create the output file RIFF chunk of form type 'WAVE'.
    m_ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');       
    m_ckRiff.cksize = 0;

    if( 0 != mmioCreateChunk( m_hmmio, &m_ckRiff, MMIO_CREATERIFF ) )
        return DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );
    
    // We are now descended into the 'RIFF' chunk we just created.
    // Now create the 'fmt ' chunk. Since we know the size of this chunk,
    // specify it in the MMCKINFO structure so MMIO doesn't have to seek
    // back and set the chunk size after ascending from the chunk.
    m_ck.ckid = mmioFOURCC('f', 'm', 't', ' ');
    m_ck.cksize = sizeof(PCMWAVEFORMAT);   

    if( 0 != mmioCreateChunk( m_hmmio, &m_ck, 0 ) )
        return DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );
    
    // Write the PCMWAVEFORMAT structure to the 'fmt ' chunk if its that type. 
    if( pwfxDest->wFormatTag == WAVE_FORMAT_PCM )
    {
        if( mmioWrite( m_hmmio, (HPSTR) pwfxDest, 
                       sizeof(PCMWAVEFORMAT)) != sizeof(PCMWAVEFORMAT))
            return DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );
    }   
    else 
    {
        // Write the variable length size.
        if( (UINT)mmioWrite( m_hmmio, (HPSTR) pwfxDest, 
                             sizeof(*pwfxDest) + pwfxDest->cbSize ) != 
                             ( sizeof(*pwfxDest) + pwfxDest->cbSize ) )
            return DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );
    }  
    
    // Ascend out of the 'fmt ' chunk, back into the 'RIFF' chunk.
    if( 0 != mmioAscend( m_hmmio, &m_ck, 0 ) )
        return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
    
    // Now create the fact chunk, not required for PCM but nice to have.  This is filled
    // in when the close routine is called.
    ckOut1.ckid = mmioFOURCC('f', 'a', 'c', 't');
    ckOut1.cksize = 0;

    if( 0 != mmioCreateChunk( m_hmmio, &ckOut1, 0 ) )
        return DXTRACE_ERR( TEXT("mmioCreateChunk"), E_FAIL );
    
    if( mmioWrite( m_hmmio, (HPSTR)&dwFactChunk, sizeof(dwFactChunk)) != 
                    sizeof(dwFactChunk) )
         return DXTRACE_ERR( TEXT("mmioWrite"), E_FAIL );
    
    // Now ascend out of the fact chunk...
    if( 0 != mmioAscend( m_hmmio, &ckOut1, 0 ) )
        return DXTRACE_ERR( TEXT("mmioAscend"), E_FAIL );
       
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: CWaveFile::Write()
// Desc: Writes data to the open wave file
//-----------------------------------------------------------------------------
HRESULT CWaveFile::Write( UINT nSizeToWrite, 
						 BYTE* pbSrcData, 
						 UINT* pnSizeWrote )
{
	FLOG( "CWaveFile::Write( UINT nSizeToWrite, BYTE* pbSrcData, UINT* pnSizeWrote )" );
    UINT cT;

    if( m_bIsReadingFromMemory )
        return E_NOTIMPL;
    if( m_hmmio == NULL )
        return CO_E_NOTINITIALIZED;
    if( pnSizeWrote == NULL || pbSrcData == NULL )
        return E_INVALIDARG;

    *pnSizeWrote = 0;
    
    for( cT = 0; cT < nSizeToWrite; cT++ )
    {       
        if( m_mmioinfoOut.pchNext == m_mmioinfoOut.pchEndWrite )
        {
            m_mmioinfoOut.dwFlags |= MMIO_DIRTY;
            if( 0 != mmioAdvance( m_hmmio, &m_mmioinfoOut, MMIO_WRITE ) )
                return DXTRACE_ERR( TEXT("mmioAdvance"), E_FAIL );
        }

        *((BYTE*)m_mmioinfoOut.pchNext) = *((BYTE*)pbSrcData+cT);
        (BYTE*)m_mmioinfoOut.pchNext++;

        (*pnSizeWrote)++;
    }

    return S_OK;
}





