/*
 * $ Generic HTTP Client
 * ----------------------------------------------------------------------------------------------------------------
 *
 * name :          GenericHTTPClient
 *
 * version tag :     0.1.0
 *
 * description :    HTTP Client using WININET
 *
 * author :          Heo Yongsun ( gooshin@opentown.net )
 *
 * This code distributed under BSD LICENSE STYLE.
 */

#pragma once

#ifndef __GENERIC_HTTP_CLIENT
#define __GENERIC_HTTP_CLIENT

#include <tchar.h>
#include <wininet.h>

// use stl
#include <vector>

#pragma comment(lib, "Wininet.lib ")

// PRE-DEFINED CONSTANTS
constexpr auto __DEFAULT_AGENT_NAME = "MERONG(0.9/;p)";

// PRE-DEFINED BUFFER SIZE
constexpr auto	__SIZE_HTTP_ARGUMENT_NAME = 256;
constexpr auto __SIZE_HTTP_ARGUMENT_VALUE = 1024;

constexpr auto __HTTP_VERB_GET = "GET";
constexpr auto __HTTP_VERB_POST = "POST";
constexpr auto __HTTP_ACCEPT_TYPE = "*/*";
constexpr auto __HTTP_ACCEPT = "Accept: */*\r\n";
constexpr auto __SIZE_HTTP_BUFFER = 100000;
constexpr auto __SIZE_HTTP_RESPONSE_BUFFER = 100000;
constexpr auto __SIZE_HTTP_HEAD_LINE = 2048;

constexpr auto __SIZE_BUFFER = 1024;
constexpr auto __SIZE_SMALL_BUFFER = 256;

class GenericHTTPClient {
public:
	typedef struct __GENERIC_HTTP_ARGUMENT {							// ARGUMENTS STRUCTURE
		TCHAR	szName[__SIZE_HTTP_ARGUMENT_NAME];
		TCHAR	szValue[__SIZE_HTTP_ARGUMENT_VALUE];
		DWORD	dwType;
		bool operator==(const __GENERIC_HTTP_ARGUMENT& argV) const {
			return !_tcscmp(szName, argV.szName) && !_tcscmp(szValue, argV.szValue);
		}
	} GenericHTTPArgument;

	enum RequestMethod {															// REQUEST METHOD
		RequestUnknown = 0,
		RequestGetMethod = 1,
		RequestPostMethod = 2,
		RequestPostMethodMultiPartsFormData = 3
	};

	enum TypePostArgument {													// POST TYPE 
		TypeUnknown = 0,
		TypeNormal = 1,
		TypeBinary = 2
	};

	// CONSTRUCTOR & DESTRUCTOR
	GenericHTTPClient();
	virtual ~GenericHTTPClient();

	static RequestMethod GetMethod(int nMethod);
	static TypePostArgument GetPostArgumentType(int nType);

	// Connection handler	
	BOOL Connect(LPCTSTR szAddress, LPCTSTR szAgent = __DEFAULT_AGENT_NAME, unsigned short nPort = INTERNET_DEFAULT_HTTP_PORT, LPCTSTR szUserAccount = NULL, LPCTSTR szPassword = NULL);
	BOOL Close();
	VOID InitilizePostArguments();

	// HTTP Arguments handler	
	VOID AddPostArguments(LPCTSTR szName, DWORD nValue);
	VOID AddPostArguments(LPCTSTR szName, LPCTSTR szValue, BOOL bBinary = FALSE);

	// HTTP Method handler 
	BOOL Request(LPCTSTR szURL, int nMethod = RequestGetMethod, LPCTSTR szAgent = __DEFAULT_AGENT_NAME);
	BOOL RequestOfURI(LPCTSTR szURI, int nMethod = RequestGetMethod);
	BOOL Response(PBYTE pHeaderBuffer, DWORD dwHeaderBufferLength, PBYTE pBuffer, DWORD dwBufferLength, DWORD &dwResultSize);
	LPCTSTR QueryHTTPResponse();
	LPCTSTR QueryHTTPResponseHeader();

	// General Handler
	DWORD GetLastError();
	LPCTSTR GetContentType(LPCTSTR szName);
	VOID ParseURL(LPCTSTR szURL, LPTSTR szProtocol, LPTSTR szAddress, DWORD &dwPort, LPTSTR szURI);

	void InitializeXMLLoader(void);
	int LoadXMLFromQueryHTTPResponse(void);
	void UninitializeXMLLoader(void);
	BOOL GetTextFromXMLLoaderByNodeName(LPSTR o_szNodeText, int i_nBufSize, LPCTSTR i_szNodeName, int i_NodeIndex = 0);
	BOOL GetParameterValue(char *o_szParamValue, int i_nValueBuffSize, char *i_szParameterName, char *i_szSourceStr, char *i_szSepStr);

protected:
	std::vector<GenericHTTPArgument> _vArguments;				// POST ARGUMENTS VECTOR

	TCHAR		_szHTTPResponseHTML[__SIZE_HTTP_BUFFER];		// RECEIVE HTTP BODY
	TCHAR		_szHTTPResponseHeader[__SIZE_HTTP_BUFFER];	// RECEIVE HTTP HEADR

	HINTERNET _hHTTPOpen;				// internet open handle
	HINTERNET _hHTTPConnection;		// internet connection hadle
	HINTERNET _hHTTPRequest;		// internet request hadle

	DWORD		_dwError;					// LAST ERROR CODE
	LPCTSTR		_szHost;					 //	 HOST NAME
	DWORD		_dwPort;					//  PORT

	// HTTP Method handler
	DWORD ResponseOfBytes(PBYTE pBuffer, DWORD dwSize);
	DWORD GetPostArguments(LPTSTR szArguments, DWORD dwLength);
	BOOL RequestPost(LPCTSTR szURI);
	BOOL RequestPostMultiPartsFormData(LPCTSTR szURI);
	BOOL RequestGet(LPCTSTR szURI);
	DWORD AllocMultiPartsFormData(PBYTE &pInBuffer, LPCTSTR szBoundary = "--MULTI-PARTS-FORM-DATA-BOUNDARY-");
	VOID FreeMultiPartsFormData(PBYTE &pBuffer);
	DWORD GetMultiPartsFormDataLength();
};

#endif	// #ifndef __GENERIC_HTTP_CLIENT
