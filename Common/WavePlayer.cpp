// WavePlayer.cpp: implementation of the CWavePlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "atumadmintool.h"
#include "WavePlayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWavePlayer::CWavePlayer()
{

}

CWavePlayer::~CWavePlayer()
{

}

void CWavePlayer::PlaySound(LPCTSTR i_szSound)
{
	HRSRC hRes;
	HGLOBAL hData;
	BOOL bOK = FALSE;

	if ((hRes = ::FindResource(AfxGetResourceHandle(), i_szSound, _T("WAVE"))) != NULL
		&& (hData = ::LoadResource(AfxGetResourceHandle(), hRes)) != NULL)
	{
		bOK = sndPlaySound((LPCTSTR)::LockResource(hData), SND_MEMORY|SND_ASYNC|SND_LOOP);
		FreeResource(hData);
	}
}