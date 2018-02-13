#pragma once

//-----------------------------------------------------------------------------
// File: DSUtil.h
//
// Desc: 
//
// Copyright (c) 1999-2001 Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef DSUTIL_H
#define DSUTIL_H

#include <windows.h>
#include <mmsystem.h>
//#include <mmreg.h>
#include <dsound.h>
// 2005-01-04 by jschoi
#include <d3dx9math.h>
#include <DefineGlobal.h>

//-----------------------------------------------------------------------------
// Classes used by this header
//-----------------------------------------------------------------------------
class CSoundManager;
class CSound;
class CWaveFile;




//-----------------------------------------------------------------------------
// Name: class CSoundManager
// Desc: 
//-----------------------------------------------------------------------------
class CSoundManager final
{
protected:
    LPDIRECTSOUND8 m_pDS;

public:
    CSoundManager() : m_pDS { nullptr } { }
	~CSoundManager() { util::release(m_pDS); }

	// called inside CAtumSound::InitDeviceObjects() only
    HRESULT Initialize( HWND hWnd, DWORD dwCoopLevel, DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate );
    

    HRESULT SetPrimaryBufferFormat( DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate ) const;
    HRESULT Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener ) const;

	HRESULT CreateFromMemory( CSound** ppSound, BYTE* pbData, ULONG ulDataSize, LPWAVEFORMATEX pwfx, DWORD dwCreationFlags = 0, GUID guid3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 1, BOOL i_b3DSoundFlag=TRUE);
};

class CSoundManagerEx final
{
	// todo : panoskj, implement xaudio replacement here
	// but temporarily use workaround that fixes the sounds anyway
};

struct SCDIRECT_3DSOUND
{
	LPDIRECTSOUNDBUFFER		pDSBuffer;
	LPDIRECTSOUND3DBUFFER	pDS3DBuffer;
	DS3DBUFFER              ds3DBufferParams;	               // 3D buffer properties	
};

//-----------------------------------------------------------------------------
// Name: class CSound
// Desc: Encapsulates functionality of a DirectSound buffer. Used solely by CAtumSound.
//-----------------------------------------------------------------------------
class CSound final
{
protected:
    DWORD					m_dwDSBufferSize;
    CWaveFile*				m_pWaveFile;
    DWORD					m_dwNumBuffers;
	SCDIRECT_3DSOUND*		m_pSCDirect3DSound;
	D3DXVECTOR3				m_Vec3SoundPosition;

	static HRESULT RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB, bool* pbWasRestored);

public:
    CSound(LPDIRECTSOUNDBUFFER* apDSBuffer, DWORD dwDSBufferSize, DWORD dwNumBuffers, CWaveFile* pWaveFile, BOOL i_b3DSoundFlag=TRUE);
    ~CSound();

    HRESULT Get3DBufferInterface( DWORD dwIndex, LPDIRECTSOUND3DBUFFER* ppDS3DBuffer );
    HRESULT FillBufferWithSound( LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger ) const;

	// Finds the next sound buffer that is not playing or the sound buffer that will finish next.
    SCDIRECT_3DSOUND* GetFreeSCDirect3DSound();

	// Finds the sound buffer at given index with bounds checking (nullptr returned on error).
    SCDIRECT_3DSOUND* GetBuffer(DWORD dwIndex) const;

    HRESULT Play(DWORD dwPriority = 0, DWORD dwFlags = 0, LONG lVolume = 0);
    HRESULT Stop();	
    HRESULT Reset();
    BOOL    IsSoundPlaying() const;

	void Set3DSoundPosition(D3DXVECTOR3 *i_pVec3Position);
	
	int						m_nSoundType;
	int						m_nCurrentIndex;
	BOOL					m_b3DSoundFlag;
};

//-----------------------------------------------------------------------------
// Name: class CWaveFile
// Desc: Encapsulates reading or writing sound data to or from a wave file
//-----------------------------------------------------------------------------
class CWaveFile
{
public:
    WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure
    HMMIO         m_hmmio;       // MM I/O handle for the WAVE
    MMCKINFO      m_ck;          // Multimedia RIFF chunk
    MMCKINFO      m_ckRiff;      // Use in opening a WAVE file
    DWORD         m_dwSize;      // The size of the wave file
    MMIOINFO      m_mmioinfoOut;
    DWORD         m_dwFlags;
    BOOL          m_bIsReadingFromMemory;
    BYTE*         m_pbData;
    BYTE*         m_pbDataCur;
    ULONG         m_ulDataSize;
    CHAR*         m_pResourceBuffer;

protected:
    HRESULT ReadMMIO();
    HRESULT WriteMMIO( WAVEFORMATEX *pwfxDest );

public:
    CWaveFile();
    ~CWaveFile();

    HRESULT Open( LPTSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags );
    HRESULT OpenFromMemory( BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags );
    HRESULT Close();

    HRESULT Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead );
    HRESULT Write( UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote );

    DWORD   GetSize();
    HRESULT ResetFile();
    WAVEFORMATEX* GetFormat() const { return m_pwfx; };
};




#endif // DSUTIL_H
