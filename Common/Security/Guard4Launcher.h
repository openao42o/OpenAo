//////////////////////////////////////////////////////////////////////////////////
//	VTCGuard API for launcher
//////////////////////////////////////////////////////////////////////////////////
#ifndef __GUARDAPI_LAUNCHER
#define __GUARDAPI_LAUNCHER

/**
	* lpDomain		: server update
	* lpGamePath	: path to game folder. Ex: C:\Program Files\Ran
	* Msg			: return message
*/
VOID vt_Autoupdate(IN LPCSTR lpDomain,IN LPCSTR lpGamePath,OUT LPSTR Msg);

/**
	* lpGamePath	: parameters passed when calling guard.
		Ex: C:\Program Files\Ran\RanOnline.exe#param1#KJSHDF8932N3B2M3NBCPS8DHLO63G9FV 
		"KJSHDF8932N3B2M3NBCPS8DHLO63G9FV" is constant
	* Msg			: return message
*/
VOID vt_run(IN LPCSTR lpArg,OUT LPSTR Msg);


#endif

