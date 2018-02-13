// CImm.h - part if Input Context management class



#if !defined(_C_IME_HANDLER_H_)
#define _C_IME_HANDLER_H_

typedef /* [public][public] */ struct __MIDL___MIDL_itf_dimm_0000_0012
    {
    HWND hWnd;
    BOOL fOpen;
    POINT ptStatusWndPos;
    POINT ptSoftKbdPos;
    DWORD fdwConversion;
    DWORD fdwSentence;
    union 
        {
        LOGFONTA A;
        LOGFONTW W;
        } 	lfFont;
    COMPOSITIONFORM cfCompForm;
    CANDIDATEFORM cfCandForm[ 4 ];
    HIMCC hCompStr;
    HIMCC hCandInfo;
    HIMCC hGuideLine;
    HIMCC hPrivate;
    DWORD dwNumMsgBuf;
    HIMCC hMsgBuf;
    DWORD fdwInit;
    DWORD dwReserve[ 3 ];
    } 	INPUTCONTEXT;

class CIMM
{
public:
	CIMM();
	virtual ~CIMM();

	void	Init();

	INPUTCONTEXT*	LockIMC(HIMC hImc);
	BOOL			UnlockIMC(HIMC hImc);
	LPVOID			LockIMCC( HIMCC );
	BOOL			UnlockIMCC( HIMCC );

protected:

	HINSTANCE		m_hDllImm32;

	INPUTCONTEXT*	(WINAPI * _ImmLockIMC)( HIMC );
	BOOL			(WINAPI * _ImmUnlockIMC)( HIMC );
	LPVOID			(WINAPI * _ImmLockIMCC)( HIMCC );
	BOOL			(WINAPI * _ImmUnlockIMCC)( HIMCC );
};

#endif //_C_IME_HANDLER_H_