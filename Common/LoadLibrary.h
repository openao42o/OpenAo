#pragma once

class CLoadLibrary  
{
public:
	CLoadLibrary() : m_hLibrary { nullptr } { }
	~CLoadLibrary() { if (m_hLibrary) FreeLibraryw(); }

	static constexpr bool LoadProcList() { return false; }

	bool LoadLibraryw(const char *i_szLibFileName);
	void FreeLibraryw();
	bool GetProcAddressw(FARPROC *o_ppProc, const char *i_szFunctionName);
	mt_lock *GetmtlockPtr() { return &m_mtlock; }

protected:
	HANDLE			m_hLibrary;
	mt_lock			m_mtlock;
};