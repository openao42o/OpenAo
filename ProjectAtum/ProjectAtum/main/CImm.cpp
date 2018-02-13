// CImm.cpp - part if Input Context management class


#include "stdafx.h"
#include "CImm.h"


/*----------------------------------------------------------------------------*/ /* CIMM */ 
CIMM::CIMM() 
:	m_hDllImm32(NULL) 
{
}

/*----------------------------------------------------------------------------*/ 
CIMM::~CIMM()
{ 
	if(m_hDllImm32) 
		FreeLibrary(m_hDllImm32);
}

/*----------------------------------------------------------------------------*/ 
void	CIMM::Init()
{
    CHAR szPath[MAX_PATH+1];
    
	if( GetSystemDirectoryA( szPath, MAX_PATH+1 ) ) 
	{
		strcat( szPath, "\\imm32.dll" );
		m_hDllImm32 = LoadLibraryA( szPath );
		if( m_hDllImm32 )
		{
			_ImmLockIMC		= (INPUTCONTEXT*(WINAPI *)( HIMC ))
				GetProcAddress( m_hDllImm32, "ImmLockIMC" );
			_ImmUnlockIMC	= (BOOL(WINAPI *)( HIMC ))
				GetProcAddress( m_hDllImm32, "ImmUnlockIMC" );
			_ImmLockIMCC	= (LPVOID(WINAPI *)( HIMCC ))
				GetProcAddress( m_hDllImm32, "ImmLockIMCC" );
			_ImmUnlockIMCC	= (BOOL(WINAPI *)( HIMCC ))
				GetProcAddress( m_hDllImm32, "ImmUnlockIMCC" );
		}
	}
}

/*----------------------------------------------------------------------------*/ 
INPUTCONTEXT*	CIMM::LockIMC(HIMC hImc)
{
	if(_ImmLockIMC == NULL) 
	{
		return NULL;
	} else 
	{
		return _ImmLockIMC(hImc);
	}
}

/*----------------------------------------------------------------------------*/ 
BOOL	CIMM::UnlockIMC(HIMC hImc)
{
	if(_ImmUnlockIMC == NULL) 
	{
		return FALSE;
	} else 
	{
		return _ImmUnlockIMC(hImc);
	}
}

/*----------------------------------------------------------------------------*/ 
LPVOID	CIMM::LockIMCC( HIMCC himcc )
{
	if(_ImmLockIMCC == NULL) 
	{
		return NULL;
	} else 
	{
		return _ImmLockIMCC(himcc);
	}
}

/*----------------------------------------------------------------------------*/ 
BOOL	CIMM::UnlockIMCC( HIMCC himcc )
{
	if(_ImmUnlockIMCC == NULL) 
	{
		return NULL;
	} else 
	{
		return _ImmUnlockIMCC(himcc);
	}
}

/*----------------------------------------------------------------------------*/ 

