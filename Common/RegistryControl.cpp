// RegistryControl.cpp: implementation of the CRegistryControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegistryControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistryControl::CRegistryControl()
{
	m_hMainRegsKey			= HKEY_CURRENT_USER;	// 2007-09-01 by cmkwon, 기본값
	m_hSubRegsKey			= NULL;
	memset(m_szKeyPath, 0x00, nMaxStringSize);
}

CRegistryControl::~CRegistryControl()
{
	RegistryControlClean();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CRegistryControl::RegistryControlInit(HKEY i_hMainKey, char *i_szKeyPath, BOOL i_bMustCreate=FALSE)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-01 ~ 2007-09-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CRegistryControl::RegistryControlInit(HKEY i_hMainKey, char *i_szKeyPath, BOOL i_bMustCreate/*=FALSE*/)
{
	if(NULL != m_hSubRegsKey)
	{// 2007-09-01 by cmkwon, RegistryControlClean()함수를 호출하여 초기화해야지 다시 사용 가능하다
		return FALSE;
	}

	HKEY hKey = NULL;
	if(ERROR_SUCCESS != RegOpenKeyEx(i_hMainKey, i_szKeyPath, 0L, KEY_ALL_ACCESS, &hKey))
	{// 레지스트리 Key가 존재하지는지 체크

		if(FALSE == i_bMustCreate)
		{// 2007-09-01 by cmkwon, 강제 생성 체크
			return FALSE;
		}

		if(ERROR_SUCCESS != RegCreateKey(i_hMainKey, i_szKeyPath, &hKey))
		{// 2007-09-01 by cmkwon, HKEY_LOCAL_MACHINE 레지스트리 생성 권한이 없는것 같음
			return FALSE;
		}
	}

	m_hMainRegsKey	= i_hMainKey;
	m_hSubRegsKey	= hKey;
	strncpy(m_szKeyPath, i_szKeyPath, nMaxStringSize-1);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CRegistryControl::RegistryControlClean(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-01 ~ 2007-09-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CRegistryControl::RegistryControlClean(void)
{
	if(m_hSubRegsKey)
	{
		RegCloseKey(m_hSubRegsKey);
		m_hSubRegsKey		= NULL;
	}	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CRegistryControl::ReadInt(char *i_szValueName, DWORD *o_pdwData)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-01 ~ 2007-09-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CRegistryControl::ReadInt(char *i_szValueName, DWORD *o_pdwData)
{
	if(NULL == m_hSubRegsKey)
	{// 2007-09-01 by cmkwon, RegistryControlInit()함수를 호출해서 초기화를 먼저 해야한다
		return FALSE;
	}

	DWORD dwSize		= sizeof(DWORD);
	DWORD dwValueType	= REG_DWORD; 
	LONG lRet = RegQueryValueEx(m_hSubRegsKey,
								i_szValueName,
								NULL,
								&dwValueType,
								(BYTE*)&o_pdwData,
								&dwSize);
	if(ERROR_SUCCESS != lRet)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CRegistryControl::ReadString(char *i_szValueName, char *o_szData, int i_nDataBufferLen)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-01 ~ 2007-09-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CRegistryControl::ReadString(char *i_szValueName, char *o_szData, int i_nDataBufferLen)
{
	if(NULL == m_hSubRegsKey)
	{// 2007-09-01 by cmkwon, RegistryControlInit()함수를 호출해서 초기화를 먼저 해야한다
		return FALSE;
	}

	char achString[nMaxStringSize];
	DWORD dwSize		= nMaxStringSize;
	DWORD dwValueType	= REG_SZ; 
	LONG lRet = RegQueryValueEx(m_hSubRegsKey,
								i_szValueName,
								NULL,
								&dwValueType,
								(BYTE*)&achString,
								&dwSize);
	if(ERROR_SUCCESS != lRet)
	{
		return FALSE;
	}

	memset(o_szData, 0x00, i_nDataBufferLen);
	strncpy(o_szData, achString, min(nMaxStringSize, i_nDataBufferLen) - 1);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CRegistryControl::WriteInt(char *i_szValueName, DWORD i_dwData)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-01 ~ 2007-09-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CRegistryControl::WriteInt(char *i_szValueName, DWORD i_dwData)
{
	if(NULL == m_hSubRegsKey)
	{// 2007-09-01 by cmkwon, RegistryControlInit()함수를 호출해서 초기화를 먼저 해야한다
		return FALSE;
	}

	LONG lRet = RegSetValueEx(m_hSubRegsKey,		// subkey handle 
								i_szValueName,		// value name 
								NULL,				// must be zero 
								REG_DWORD,			// value type 
								(LPBYTE)&i_dwData,	// pointer to value data 
								sizeof(DWORD));		// length of value data 
	if(ERROR_SUCCESS != lRet)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CRegistryControl::WriteString(char *i_szValueName, char *i_szData)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-01 ~ 2007-09-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CRegistryControl::WriteString(char *i_szValueName, char *i_szData)
{
	if(NULL == m_hSubRegsKey)
	{// 2007-09-01 by cmkwon, RegistryControlInit()함수를 호출해서 초기화를 먼저 해야한다
		return FALSE;
	}

	char sztmData[nMaxStringSize];
	memset(sztmData, 0x00, nMaxStringSize);
	strncpy(sztmData, i_szData, nMaxStringSize-1);

	LONG lRet = RegSetValueEx(m_hSubRegsKey,			// subkey handle 
								i_szValueName,			// value name 
								NULL,					// must be zero 
								REG_SZ,					// value type canbe REG_DWORD, 
								(LPBYTE)sztmData,		// pointer to value data 
								strlen(sztmData)+1);	// length of value data 
	if(ERROR_SUCCESS != lRet)
	{
		return FALSE;
	}

	return TRUE;
}
