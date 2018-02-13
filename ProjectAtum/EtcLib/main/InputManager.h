// InputManager.h: interface for the CInputManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INPUTMANAGER_H__02DDADDC_C45E_4EC0_B537_2AA9CA463297__INCLUDED_)
#define AFX_INPUTMANAGER_H__02DDADDC_C45E_4EC0_B537_2AA9CA463297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"

class CInputManager  
{
public:
	CInputManager();
	virtual ~CInputManager();
private:
	BOOL		m_bEscKey;			// Esc
	BOOL		m_bTabKey;			// Tab
	BOOL		m_bCapsLockKey;		// CapsLock
	BOOL		m_bLeftShiftKey;	// Left Shift
	BOOL		m_bRightShiftKey;	// Right Shift
	BOOL		m_bLeftCtrlKey;		// Left Ctrl
	BOOL		m_bRightCtrlKey;	// Right Ctrl
	BOOL		m_bLeftAltKey;		// Left Alt
	BOOL		m_bWindowKey;		// Window

	BOOL		m_bF1Key;		// F1
	BOOL		m_bF2Key;		// F2
	BOOL		m_bF3Key;		// F3
	BOOL		m_bF4Key;		// F4
	BOOL		m_bF5Key;		// F5
	BOOL		m_bF6Key;		// F6
	BOOL		m_bF7Key;		// F7
	BOOL		m_bF8Key;		// F8
	BOOL		m_bF9Key;		// F9
	BOOL		m_bF10Key;		// F10
	BOOL		m_bF11Key;		// F11
	BOOL		m_bF12Key;		// F12

	BOOL		m_b1Key;		// 1
	BOOL		m_b2Key;		// 2
	BOOL		m_b3Key;		// 3
	BOOL		m_b4Key;		// 4
	BOOL		m_b5Key;		// 5
	BOOL		m_b6Key;		// 6
	BOOL		m_b7Key;		// 7
	BOOL		m_b8Key;		// 8
	BOOL		m_b9Key;		// 9
	BOOL		m_b0Key;		// 0

	BOOL		m_bAKey;		// A
	BOOL		m_bBKey;		// B
	BOOL		m_bCKey;		// C
	BOOL		m_bDKey;		// D
	BOOL		m_bEKey;		// E
	BOOL		m_bFKey;		// F
	BOOL		m_bGKey;		// G
	BOOL		m_bHKey;		// H
	BOOL		m_bIKey;		// I
	BOOL		m_bJKey;		// J
	BOOL		m_bKKey;		// K
	BOOL		m_bLKey;		// L
	BOOL		m_bMKey;		// M
	BOOL		m_bNKey;		// N
	BOOL		m_bOKey;		// O
	BOOL		m_bPKey;		// P
	BOOL		m_bQKey;		// Q
	BOOL		m_bRKey;		// R
	BOOL		m_bSKey;		// S
	BOOL		m_bTKey;		// T
	BOOL		m_bUKey;		// U
	BOOL		m_bVKey;		// V
	BOOL		m_bWKey;		// W
	BOOL		m_bXKey;		// X
	BOOL		m_bYKey;		// Y
	BOOL		m_bZKey;		// Z


};

#endif // !defined(AFX_INPUTMANAGER_H__02DDADDC_C45E_4EC0_B537_2AA9CA463297__INCLUDED_)
