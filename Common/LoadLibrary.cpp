#include "stdafx.h"
#include "LoadLibrary.h"



bool CLoadLibrary::LoadLibraryw(const char *i_szLibFileName)
{
	mt_auto_lock mtA(GetmtlockPtr());

	m_hLibrary	= ::LoadLibrary(i_szLibFileName);
	
	return m_hLibrary != nullptr;
}

void CLoadLibrary::FreeLibraryw()
{
	mt_auto_lock mtA(GetmtlockPtr());
	
	if(!m_hLibrary) return;

	FreeLibrary(HMODULE(m_hLibrary));

	m_hLibrary = NULL;
}

bool CLoadLibrary::GetProcAddressw(FARPROC *o_ppProc, const char *i_szFunctionName)
{
	mt_auto_lock mtA(GetmtlockPtr());

	*o_ppProc = nullptr;

	if (!m_hLibrary)
	{
		g_pGlobal->WriteSystemLogEX(true, "[ERROR] innBill CLoadLibrary::GetProcAddressw# invalid Handle !!, hLibaray(0x%X) FuncName(%s) \r\n", m_hLibrary, i_szFunctionName);
		return false;
	}

	*o_ppProc = GetProcAddress(HMODULE(m_hLibrary), i_szFunctionName);
	
	if(*o_ppProc == nullptr)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] innBill CLoadLibrary::GetProcAddressw# no such functionName!!, hLibaray(0x%X) FuncName(%s) \r\n", m_hLibrary, i_szFunctionName);
		return false;
	}
	
	return true;
}