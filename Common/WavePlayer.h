// WavePlayer.h: interface for the CWavePlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEPLAYER_H__D999EFFB_953C_477F_9660_EDE304DF7A73__INCLUDED_)
#define AFX_WAVEPLAYER_H__D999EFFB_953C_477F_9660_EDE304DF7A73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MMSYSTEM.H>

class CWavePlayer  
{
public:
	CWavePlayer();
	virtual ~CWavePlayer();

	static void PlaySound(LPCTSTR i_szSound);
	inline static void PlaySound(UINT i_nIDS)
	{
		PlaySound(MAKEINTRESOURCE(i_nIDS));
	}
	inline static void StopSound()
	{
		sndPlaySound(NULL, SND_ASYNC|SND_LOOP);
	}
};

#endif // !defined(AFX_WAVEPLAYER_H__D999EFFB_953C_477F_9660_EDE304DF7A73__INCLUDED_)
