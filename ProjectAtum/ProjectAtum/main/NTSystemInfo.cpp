// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

//######################################################################################
// File    : NTSystemInfo.cpp
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : System 정보 얻기
// Bug     : 
//######################################################################################
#include "NTSystemInfo.h"
#include "NTGetWinVer.h"
#include <tchar.h>

#pragma comment(lib, "dxguid.lib")


#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=NULL; }		}
#define util::del_array(p)	{ if(p) { delete[] (p);		(p)=NULL; }		}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 생성자
//######################################################################################
CNTSystemInfo::CNTSystemInfo() :
m_bCleanupCOM( FALSE ),
m_pDxDiagProvider( NULL ),
m_pDxDiagRoot( NULL ),
m_fSystemPhysicMemory( 0.0f ),
m_dwDXVersionMajor( 0 ),
m_dwDXVersionMinor( 0 ),
m_cDXVersionLetter( NULL )
{
//	m_wszCpuString.clear();
//	m_wsVideoCardName.clear();
//	m_wsDXVersion.clear();
//	m_strWinVersion.clear();
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 소멸자
//######################################################################################
CNTSystemInfo::~CNTSystemInfo()
{
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 초기화
//######################################################################################
bool CNTSystemInfo::Initialize()
{

    HRESULT hr;
	if( m_pDxDiagRoot != NULL )
	{
		return TRUE;
	}
    hr = CoInitialize(NULL);
    m_bCleanupCOM = SUCCEEDED(hr);

//     hr = CoCreateInstance( CLSID_DxDiagProvider,
//                            NULL,
//                            CLSCTX_INPROC_SERVER,
//                            IID_IDxDiagProvider,
//                            (LPVOID*) &m_pDxDiagProvider );
//     if( SUCCEEDED(hr) )
//     {
//         DXDIAG_INIT_PARAMS dxDiagInitParam;
//         ZeroMemory( &dxDiagInitParam, sizeof(DXDIAG_INIT_PARAMS) );
//         dxDiagInitParam.dwSize                  = sizeof(DXDIAG_INIT_PARAMS);
//         dxDiagInitParam.dwDxDiagHeaderVersion   = DXDIAG_DX9_SDK_VERSION;
//         dxDiagInitParam.bAllowWHQLChecks        = false;
//         dxDiagInitParam.pReserved               = NULL;
// 
//         hr = m_pDxDiagProvider->Initialize( &dxDiagInitParam ); 
//         if( SUCCEEDED(hr) )
//         {
//             hr = m_pDxDiagProvider->GetRootContainer( & m_pDxDiagRoot );
// 			if( FAILED(hr) )
// 			{
// 				FreeIDxDiagContainer();
// 				return FALSE;
// 			}
// 		}
// 		else
// 		{
// 			FreeIDxDiagContainer();
// 			return FALSE;
// 		}
// 	}
// 	else
// 	{
// 		FreeIDxDiagContainer();
// 		return FALSE;
// 	}

	TCHAR szWinVer[50], szMajorMinorBuild[50];
	int nWinVer;
	GetWinVer(szWinVer, &nWinVer, szMajorMinorBuild);
	m_strWinVersion = szWinVer;
	m_strWinVersion += szMajorMinorBuild;


	CPUInfo();
	GetDirectXVersion( &m_dwDXVersionMajor, &m_dwDXVersionMinor, &m_cDXVersionLetter );
	GetPhysicalMemoryInMB();

	DWORD dev = 0;
	GetDisplayDeviceDescription( dev, &m_wsVideoCardName );
	GetDisplayDeviceMemoryInMB( dev, &m_nVideoPhysicMemory );


	return TRUE;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : CPU정보
//######################################################################################
void CNTSystemInfo::CPUInfo()
{
	HKEY hKey;
	DWORD dataSize=0;
	TCHAR szCPUNAME [64];
	TCHAR szTempEx [64];
	ZeroMemory(szCPUNAME,64);
	ZeroMemory(szTempEx,64);
	LONG regResult;
	regResult = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,
		_T("Hardware\\Description\\System\\CentralProcessor\\0"), 0, KEY_QUERY_VALUE, &hKey);

	if(regResult == ERROR_SUCCESS){
		dataSize = sizeof (szCPUNAME);
		regResult = ::RegQueryValueEx (hKey, _T("ProcessorNameString"), NULL, NULL,
			(LPBYTE)szCPUNAME, &dataSize);
		int num=0;
		bool bspace=true;
		for(int i=0;i<64;i++)
		{
			if(szCPUNAME[i] != ' ' || bspace==false)
			{
				bspace = false;
				szTempEx[num++] = szCPUNAME[i];
			}
		}
		m_wszCpuString = szTempEx;
		//wcscpy(m_wszCpuString,szTempEx);
	}
	RegCloseKey (hKey);
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 비디오 메모리
//######################################################################################
bool CNTSystemInfo::GetDisplayDeviceMemoryInMB( DWORD dwDevice, int* pDisplayMemory )
{
// 	if( NULL == pDisplayMemory )
// 		return FALSE;
// 	*pDisplayMemory = 0;
// 	TString str;
// 	if( !GetDisplayDeviceProp( dwDevice, _T("szDisplayMemoryEnglish"), &str ) )
// 		return FALSE;
// 
// 	int nMem;
// 	int num_fields;
// 	num_fields = _stscanf_s( str.c_str(), _T("%d"), &nMem );
// 	if( num_fields != 1 )
// 	{
// 		return FALSE;
// 	}
// 
// 	*pDisplayMemory = nMem;
	return TRUE;
}



//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
bool CNTSystemInfo::GetDisplayDeviceProp( DWORD dwDevice, const TCHAR* prop_name, TString* pwstrProp )
{
	if( NULL == prop_name || NULL == pwstrProp )
		return FALSE;

	*pwstrProp = _T("");

	IDxDiagContainer * pDisplayDevice;
	if( !GetDisplayDeviceNode( dwDevice, &pDisplayDevice ) )
		return FALSE;

	if( NULL == pDisplayDevice )
		return FALSE;

	if( !GetProperty( pDisplayDevice, prop_name, pwstrProp ) )
		return FALSE;

	return true;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
bool CNTSystemInfo::GetDisplayDeviceNode( DWORD dwDeviceIndex, IDxDiagContainer ** ppNode )
{
	if( NULL == ppNode )
		return FALSE;

	IDxDiagContainer * pDevicesNode;
	if( !GetChildContainer( _T("DxDiag_DisplayDevices"), &pDevicesNode ) )
		return FALSE;

	if( !GetChildByIndex( pDevicesNode, dwDeviceIndex, ppNode ) )
		return FALSE;

	SAFE_RELEASE( pDevicesNode );
	return TRUE;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
bool CNTSystemInfo::GetChildByIndex(  IDxDiagContainer * pParent,
										DWORD dwIndex,
										IDxDiagContainer ** ppChild )
{
	HRESULT hr = S_OK;
	if( NULL == pParent || NULL == ppChild )
		return FALSE;

	WCHAR wstr[256];
	if( FAILED( pParent->EnumChildContainerNames( dwIndex, wstr, 256 ) ) )
		return FALSE;

	if( FAILED( pParent->GetChildContainer( wstr, ppChild ) ) )
		return FALSE;

	return TRUE;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
bool CNTSystemInfo::GetDisplayDeviceDescription( DWORD dwDevice, TString* pwstrName )
{
	HRESULT hr = S_OK;
	if( NULL == m_pDxDiagRoot || NULL == pwstrName )
		return FALSE;

	IDxDiagContainer * pDisplayDevice;
	if( !GetDisplayDeviceNode( dwDevice, &pDisplayDevice ) )
		return FALSE;

	if( NULL == pDisplayDevice ) 
		return FALSE;

	if( !GetProperty( pDisplayDevice, _T("szDescription"), pwstrName ) )
		return FALSE;

	SAFE_RELEASE( pDisplayDevice );
	return TRUE;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : DX Version
//######################################################################################
bool CNTSystemInfo::GetDirectXVersion( DWORD * pdwDirectXVersionMajor, 
										 DWORD * pdwDirectXVersionMinor,
										 TCHAR * pcDirectXVersionLetter )
{
	if( NULL == m_pDxDiagRoot || NULL == pdwDirectXVersionMajor || 
		NULL == pdwDirectXVersionMinor || NULL == pcDirectXVersionLetter )
		return FALSE;

	TString propval;
	GetProperty( _T("DxDiag_SystemInfo"), _T("dwDirectXVersionMajor"), &propval );
	*pdwDirectXVersionMajor = atoi( propval.c_str() );

	GetProperty( _T("DxDiag_SystemInfo"), _T("dwDirectXVersionMinor"), &propval );
	*pdwDirectXVersionMinor = atoi( propval.c_str() );

	GetProperty( _T("DxDiag_SystemInfo"), _T("szDirectXVersionLetter"), &propval );

	TString str;
	str = propval;

	if( str.length() > 0 )
		*pcDirectXVersionLetter = str.at(0);
	else
		*pcDirectXVersionLetter = ' ';

	return TRUE;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
TString CNTSystemInfo::WStringToString( const TString* in_pwstring )
{
	if( in_pwstring == NULL )
		return( _T("") );
	return( in_pwstring->c_str() );
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
/*TString CNTSystemInfo::lpcwstrToString( const LPCWSTR in_lpcwstr )
{
	TCHAR * mbBuf;
	size_t sz;
	sz = 2 * wcslen( in_lpcwstr );
	mbBuf = new TCHAR[sz];
	wcstombs( mbBuf, in_lpcwstr, sz );
	TString outstr;
	outstr = mbBuf;
	util::del_array( mbBuf );
	return( outstr );
}*/


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
void CNTSystemInfo::GetPhysicalMemoryInMB()
{
// 	HRESULT hr = S_OK;
// 	TString property;
// 	if( !GetProperty( _T("DxDiag_SystemInfo"), _T("ullPhysicalMemory"), &property ) )
// 		return;
// 
// 	float mem = (float) _tstof( property.c_str() );
// 	m_fSystemPhysicMemory = mem / (1024 * 1024);
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
bool CNTSystemInfo::GetProperty( IDxDiagContainer * pContainer, const TCHAR* property_name, TString* out_value )
{
	if( NULL == pContainer || NULL == property_name || NULL == out_value )
		return FALSE;

    TCHAR wszPropValue[256];
	VARIANT var;
	VariantInit( &var );
#ifdef  UNICODE
	if( SUCCEEDED(pContainer->GetProp( property_name, &var )) )
#else
	WCHAR wszName[256];
	ConverteCHAR2WCHAR( wszName, property_name, 256 );
	wszName[255] = NULL;
	if( SUCCEEDED(pContainer->GetProp( wszName, &var )) )
#endif//(UNICODE)
	{
		switch( var.vt )
		{
			case VT_UI4:
				sprintf( wszPropValue, _T("%d"), var.ulVal );
				break;
			case VT_I4:
				sprintf( wszPropValue, _T("%d"), var.lVal );
				break;
			case VT_BOOL:
				sprintf( wszPropValue, _T("%s"), (var.boolVal) ? _T("true") : _T("false") );
				break;
			case VT_BSTR:
#ifdef  UNICODE
				StrNCpy( wszPropValue, var.bstrVal , 255 );
#else
				ConverWCHAR2CHAR( wszPropValue, var.bstrVal, 255 );
#endif//(UNICODE)
				wszPropValue[255] = 0;
				break;
		}
		(*out_value) = wszPropValue;
	}
	else
	{
		return FALSE;
	}

	VariantClear( &var );
	return TRUE;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
bool CNTSystemInfo::GetProperty( const TCHAR* container_name0, const TCHAR* property_name, TString* out_value )
{
	if( NULL == container_name0 || NULL == property_name || NULL == out_value )
		return FALSE;

	IDxDiagContainer * pContainer;
	if( !GetChildContainer( container_name0, &pContainer ) )
		return FALSE;

	if( NULL == pContainer )
		return FALSE;

	if( !GetProperty( pContainer, property_name, out_value ) )
		return FALSE;

	SAFE_RELEASE( pContainer );

	return TRUE;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
bool CNTSystemInfo::GetChildContainer( const TCHAR* name0, IDxDiagContainer ** ppChild )
{
	if( NULL == m_pDxDiagRoot || NULL == ppChild )
		return FALSE;

#ifdef  UNICODE
	if( FAILED( m_pDxDiagRoot->GetChildContainer( name0, ppChild ) ) )
#else
	WCHAR wszName[256];
	ConverteCHAR2WCHAR( wszName, name0, 256 );
	wszName[255] = NULL;
	if( FAILED( m_pDxDiagRoot->GetChildContainer( wszName, ppChild ) ) )
#endif
		return FALSE;

	if( NULL == *ppChild )
		return FALSE;

	return TRUE;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
void CNTSystemInfo::FreeIDxDiagContainer()
{
	SAFE_RELEASE( m_pDxDiagProvider );
	SAFE_RELEASE( m_pDxDiagRoot );
    if( m_bCleanupCOM )
	{
        CoUninitialize();
		m_bCleanupCOM = false;
	}
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 
//######################################################################################
const TCHAR* CNTSystemInfo::GetDxVersionString(void)
{
// 	TCHAR szTemp[256];
// 	_stprintf_s( szTemp, _T("%d.%d%c"), m_dwDXVersionMajor, m_dwDXVersionMinor, m_cDXVersionLetter );
// 	m_wsDXVersion = szTemp;
// 	return m_wsDXVersion.c_str();

	return NULL;
}


//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : CHAR형을 WCHAR로 변형
//######################################################################################
bool CNTSystemInfo::ConverteCHAR2WCHAR( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar )
{
	if( wstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
		return false;

	int nResult = MultiByteToWideChar( CP_ACP, 0, strSource, -1, wstrDestination, cchDestChar );
	wstrDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return false;

	return true;
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : WCHAR형을 CHAR로 변형
//######################################################################################
bool CNTSystemInfo::ConverWCHAR2CHAR( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar )
{
	if( strDestination==NULL || wstrSource==NULL || cchDestChar < 1 )
		return false;

	int nResult = WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, cchDestChar*sizeof(CHAR), NULL, NULL );
	strDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return false;

	return true;
}

// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송