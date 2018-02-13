#pragma once

// KeyBoardInput.h: interface for the CKeyBoardInput class.

#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <windows.h>
#include <dinput.h>

class CKeyBoardInput final
{
public:
	bool GetAsyncKeyState(int code) const
	{
		// must have not lost device, must have focus and key must be pressed to return true
		return !m_bLostDevice && m_bGetFocus && m_cDiks[code];
	}

	HRESULT OnCreateDevice(HWND);
	HRESULT ReadImmediateData(HWND);
	void    FreeDirectInput();
 


	void SetKeyBoardFocus(bool bGetFocus) { m_bGetFocus = bGetFocus; }
	void SetKeyBoardLostDevice(bool bLostDevice) { m_bLostDevice = bLostDevice; }

	bool GetKeyBoardFocus() const { return m_bGetFocus; }
	bool GetKeyBoardLostDevice() const { return m_bLostDevice; }

  
	CKeyBoardInput() :
		m_cDiks { },
		m_pDI { nullptr },
		m_pKeyboard { nullptr },
		m_bLostDevice { false },
		m_bGetFocus { true }
	{		
	}
	
private:
	BYTE m_cDiks[256];

	LPDIRECTINPUT8			m_pDI;         
	LPDIRECTINPUTDEVICE8	m_pKeyboard;  

	bool	m_bLostDevice;
	bool	m_bGetFocus;
};

#endif
