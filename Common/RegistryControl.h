// RegistryControl.h: interface for the CRegistryControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRYCONTROL_H__765E43C0_BDA7_4C90_B4CB_F9F47B93ABD5__INCLUDED_)
#define AFX_REGISTRYCONTROL_H__765E43C0_BDA7_4C90_B4CB_F9F47B93ABD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int nMaxStringSize = 4096;

class CRegistryControl  
{
public:
	CRegistryControl();
	~CRegistryControl();
	
	BOOL RegistryControlInit(HKEY i_hMainKey, char *i_szKeyPath, BOOL i_bMustCreate=FALSE);
	BOOL RegistryControlClean(void);

	BOOL ReadInt(char *i_szValueName, DWORD *o_pdwData);
	BOOL ReadString(char *i_szValueName, char *o_szData, int i_nDataBufferLen);
	BOOL WriteInt(char *i_szValueName, DWORD i_dwData);
	BOOL WriteString(char *i_szValueName, char *i_szData);


protected:
	HKEY		m_hMainRegsKey;		// 2007-09-01 by cmkwon, MainKey(HKEY_CLASSES_ROOT, HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_USERS, HKEY_CURRENT_CONFIG, HKEY_DYN_DATA)
	HKEY		m_hSubRegsKey;
	char		m_szKeyPath[nMaxStringSize];
};

#endif // !defined(AFX_REGISTRYCONTROL_H__765E43C0_BDA7_4C90_B4CB_F9F47B93ABD5__INCLUDED_)
