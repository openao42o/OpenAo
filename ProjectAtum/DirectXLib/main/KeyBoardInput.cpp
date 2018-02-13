// KeyBoardInput.cpp: implementation of the CKeyBoardInput class.
//
//////////////////////////////////////////////////////////////////////

#include "KeyBoardInput.h"
#include "DXUtil.h"
#include "DefineGlobal.h"


HRESULT CKeyBoardInput::OnCreateDevice( HWND hDlg )
{
    HRESULT hr;

	FreeDirectInput();

	DWORD dwCoopFlags = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;// | DISCL_NOWINKEY;

    // Create a DInput object
    if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&m_pDI, NULL)))
        
		return hr;
    
    // Obtain an interface to the system keyboard device.
    if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL)))
        return hr;

	if (FAILED(hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard)))
        return hr;

    hr = m_pKeyboard->SetCooperativeLevel(hDlg, dwCoopFlags);

    m_pKeyboard->Acquire();

    return S_OK;
}

HRESULT CKeyBoardInput::ReadImmediateData(HWND hDlg)
{
	if (m_pKeyboard)
	{
		util::zero(m_cDiks);

		// Get the input's device state, and put the state in dims
		auto hr = m_pKeyboard->GetDeviceState(sizeof(m_cDiks), &m_cDiks);

		if (FAILED(hr))
		{
			do hr = m_pKeyboard->Acquire();

			while (hr == DIERR_INPUTLOST);
		}
	}

    return S_OK;
}

void CKeyBoardInput::FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if (m_pKeyboard) m_pKeyboard->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE(m_pKeyboard);
    SAFE_RELEASE(m_pDI);
}