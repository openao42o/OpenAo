/*****************************************************************************\
IExplore.cpp
Copyright (c) 2002 Matthew Blagden <info@nocturnalnetwork.com>
You may use this code in your applications at no cost, so long as credit
is given to Matthew Blagden, nocturnalnetwork.com, or The Nocturnal Network.
\*****************************************************************************/
#include <windows.h>
#include <olectl.h>
#include <exdispid.h>

#include "IExplore.h"

#define Alert(x) { MessageBox(NULL, x, x, MB_OK); return E_NOTIMPL; }

//int __cdecl memcmp(const void * buf1, const void * buf2, size_t count)
//{
//   if (!count) return(0);
//   while ( --count && *(char *)buf1 == *(char *)buf2 ) {
//      buf1 = (char *)buf1 + 1;
//      buf2 = (char *)buf2 + 1;
//   }
//   return( *((unsigned char *)buf1) - *((unsigned char *)buf2) );
//}

inline void * __cdecl operator new(size_t size) { return (PBYTE)LocalAlloc(LPTR, size);}
inline void __cdecl operator delete(void *ptr)  { if (ptr) LocalFree(ptr);}


/////////////////////////////////////////////////////////////////////////////////////////
Host::Host(HWND hwndParent,LPSTR strWebAddress, LPSTR szPostData, LPSTR szHeaders)
{
    #define Assert(x) if(!x) MessageBox(NULL, "IE ERROR!", "Error", MB_ICONSTOP)

    HRESULT                     hret;
    IUnknown                   *pUnknown;            
    IWebBrowser2               *pWebBrowser;
    IConnectionPoint           *pConnectionPoint;
    IConnectionPointContainer  *pConnectionPointContainer;
    IOleInPlaceObject          *pInPlaceObject;
    IOleObject                 *pObject;
    DWORD                       dwAdviseCookie;

    // Initialize COM library
    OleInitialize(NULL);

    // Setup member variables
    m_dwRefCount = 1;
    m_hwndParent = hwndParent;

    // Instantiate all needed interfaces
    m_pClientSite = new ClientSite(this);
    m_pInPlaceSite = new InPlaceSite(this);
    m_pWebBrowserEvents2 = new WebBrowserEvents2(this);

    // Instantiate Web Browser control
    hret = CoCreateInstance(CLSID_WebBrowser, NULL, CLSCTX_ALL, IID_IUnknown, (void**)(&pUnknown));
    Assert(SUCCEEDED(hret));

    // Set the client site
    hret = pUnknown->QueryInterface(IID_IOleObject, (void**)(&pObject));
    Assert(SUCCEEDED(hret));
    hret = pObject->SetClientSite(m_pClientSite);
    Assert(SUCCEEDED(hret));

    // Set the site limits
    RECT rWindowPos;
    GetClientRect(hwndParent, &rWindowPos);
    
    rWindowPos.right -= rWindowPos.left;
    rWindowPos.bottom -= rWindowPos.top;
    rWindowPos.left = 0;
    rWindowPos.top = 0;
    
    hret = pUnknown->QueryInterface(IID_IOleInPlaceObject, (void**)(&pInPlaceObject));
    Assert(SUCCEEDED(hret));
    hret = pInPlaceObject->SetObjectRects(&rWindowPos, &rWindowPos);
    Assert(SUCCEEDED(hret));
    pInPlaceObject->Release();

    // Activate the site
    hret = pObject->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, m_pClientSite, 0, hwndParent, &rWindowPos);
    Assert(SUCCEEDED(hret));
    pObject->Release();

    // Set event handler
    hret = pUnknown->QueryInterface(IID_IConnectionPointContainer, (void**)(&pConnectionPointContainer));
    Assert(SUCCEEDED(hret));
    hret = pConnectionPointContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &pConnectionPoint);
    Assert(SUCCEEDED(hret));
    hret = pConnectionPoint->Advise(m_pWebBrowserEvents2, &dwAdviseCookie);
    Assert(SUCCEEDED(hret));
    pConnectionPointContainer->Release();
    pConnectionPoint->Release();

    // Get Web Browser interface
    hret = pUnknown->QueryInterface(IID_IWebBrowser2, (void**)&pWebBrowser);
    Assert(SUCCEEDED(hret));
    if(pUnknown) pUnknown->Release();

	// Transfer BSTR from LPSTR
	VARIANT *pvarURL, *pvarPostData, *pvarHeaders;
	VARIANT varURL, varPostData, varHeaders;

 	pvarURL = NULL;
	pvarPostData = NULL;
	pvarHeaders = NULL;

	if ( !strWebAddress )
		return;
 
	if ( !*strWebAddress )
		return;
 
	int len = lstrlen( strWebAddress ) + 1;
 
	WCHAR *pszW;
 
#ifndef UNICODE
 
	pszW = new WCHAR[len];
 
	MultiByteToWideChar( CP_ACP, 0, strWebAddress, -1, pszW, len );
 
#else
 
	pszW = strWebAddress;
 
#endif
 
	VariantInit( &varURL );
 
	varURL.vt = VT_BSTR;
	varURL.bstrVal = SysAllocString( pszW );
 
#ifndef UNICODE
 
	delete []pszW;
 
#endif
 
	pvarURL = &varURL;
 

	len = lstrlen ( szHeaders );
	if (0<len)
	{
		len++;
 
#ifndef UNICODE
 
		pszW = new WCHAR[len];
 
		MultiByteToWideChar( CP_ACP, 0, szHeaders, -1, pszW, len );
 
#else
 
		pszW = strWebAddress;
 
#endif
 
		VariantInit( &varHeaders );
		varHeaders.vt = VT_BSTR;
		varHeaders.bstrVal = SysAllocString( pszW );
 
#ifndef UNICODE
 
		delete []pszW;
 
#endif
 
		pvarHeaders = &varHeaders;
	}
 
	len = lstrlen ( szPostData );
	if (0<len)
	{
		VariantInit( &varPostData );
 
		SAFEARRAYBOUND rgsabound;
		rgsabound.cElements = len;
		rgsabound.lLbound = 0;
 
		varPostData.parray = ::SafeArrayCreate(VT_UI1, 1, &rgsabound);
		varPostData.vt = unsigned short(VT_UI1 | VT_ARRAY);
 
		void* pvDestData;
		::SafeArrayAccessData(varPostData.parray, &pvDestData);
		memcpy(pvDestData, szPostData, len);
		::SafeArrayUnaccessData (varPostData.parray);
 
		pvarPostData = &varPostData;
	}	

	// Go to Url
//	hret = pWebBrowser->Navigate2( pvarURL, NULL, NULL, pvarPostData, pvarHeaders  );
	hret = pWebBrowser->Navigate2( pvarURL, NULL, NULL, NULL, NULL  );
	
	VariantClear( pvarURL );
 
	if (pvarPostData)
		VariantClear( pvarPostData );    
 
	if (pvarHeaders)
		VariantClear( pvarHeaders );

    Assert(SUCCEEDED(hret));
    if(pWebBrowser) pWebBrowser->Release();

	m_pWebBrowser = pWebBrowser;
//	int nLen = MultiByteToWideChar(CP_ACP, 0, strWebAddress,strlen(strWebAddress), NULL, NULL);
//	BSTR bstr = SysAllocStringLen(NULL, nLen);
//	if (bstr == NULL)
//	{
//		if(pWebBrowser) pWebBrowser->Release();
//		return;
//	}
//	MultiByteToWideChar(CP_ACP, 0, strWebAddress, strlen(strWebAddress),bstr, nLen);
//
//	VARIANT *pvarPostData = NULL;
//	VARIANT varPostData;
//
//	// Make Post Data
//	len = lstrlen ( szPostData );
//	if (0<len)
//	{
//		VariantInit( &varPostData );
// 
//		SAFEARRAYBOUND rgsabound;
//		rgsabound.cElements = len;
//		rgsabound.lLbound = 0;
// 
//		varPostData.parray = ::SafeArrayCreate(VT_UI1, 1, &rgsabound);
//		varPostData.vt = unsigned short(VT_UI1 | VT_ARRAY);
// 
//		void* pvDestData;
//		::SafeArrayAccessData(varPostData.parray, &pvDestData);
//		memcpy(pvDestData, szPostData, len);
//		::SafeArrayUnaccessData (varPostData.parray);
// 
//		pvarPostData = &varPostData;
//	}
//
//
//	// Make Header
//	WCHAR *pszW;
//	VARIANT *pvarHeaders = NULL;
//	VARIANT varHeaders;
//	len = lstrlen ( szHeaders );
//	if (0<len)
//	{
//		len++;
// 
//#ifndef UNICODE
// 
//		pszW = new WCHAR[len];
// 
//		MultiByteToWideChar( CP_ACP, 0, szHeaders, -1, pszW, len );
// 
//#else
// 
//		pszW = szURL;
// 
//#endif
// 
//		VariantInit( &varHeaders );
//		varHeaders.vt = VT_BSTR;
//		varHeaders.bstrVal = SysAllocString( pszW );
// 
//#ifndef UNICODE
// 
//		delete []pszW;
// 
//#endif
// 
//		pvarHeaders = &varHeaders;
//	}
//
//
//
//    // Go to URL
//    hret = pWebBrowser->Navigate(bstr, NULL, NULL, pvarPostData, NULL);

	// Free Sys String Memory
//	SysFreeString( bstr );
//
//	if (pvarPostData)
//	{
//		VariantClear( pvarPostData ); 
//	}
//
//    Assert(SUCCEEDED(hret));
//    if(pWebBrowser) pWebBrowser->Release();
}

Host::~Host()
{
    // Clean up hosted classes
    if(m_pClientSite) delete m_pClientSite;
    if(m_pInPlaceSite) delete m_pInPlaceSite;
    if(m_pWebBrowserEvents2) delete m_pWebBrowserEvents2;

    // Release COM library
    OleUninitialize();
}

ULONG STDMETHODCALLTYPE Host::AddRef()
    { return ++m_dwRefCount; }

ULONG STDMETHODCALLTYPE Host::Release()
{
    if(!--m_dwRefCount)
        delete this;
    return m_dwRefCount;
}

STDMETHODIMP Host::QueryInterface(REFIID riid, void ** ppvObject)
{
    if(ppvObject == NULL) return E_INVALIDARG;
    *ppvObject = NULL;

    if(riid == IID_IUnknown)
        *ppvObject = this;
    else if(riid == IID_IOleClientSite)
        *ppvObject = m_pClientSite;
    else if(riid == IID_IOleInPlaceSite)
        *ppvObject = m_pInPlaceSite;
    else if(riid == DIID_DWebBrowserEvents2)
        *ppvObject = m_pWebBrowserEvents2;

    if(*ppvObject == NULL) return E_NOINTERFACE;
    AddRef();
    return S_OK;
}

void Host::Refresh()
{
	m_pWebBrowser->Refresh();
}

/////////////////////////////////////////////////////////////////////////////////////////
ClientSite::ClientSite(Host* pFrameSite) { m_pHost = pFrameSite; }
ClientSite::~ClientSite() {}

// IUnknown
STDMETHODIMP ClientSite::QueryInterface(REFIID iid, void ** ppvObject)
    { return m_pHost->QueryInterface(iid, ppvObject); }

ULONG STDMETHODCALLTYPE ClientSite::AddRef()
    { return m_pHost->AddRef(); }

ULONG STDMETHODCALLTYPE ClientSite::Release()
    { return m_pHost->Release(); }

// IOleClientSite
STDMETHODIMP ClientSite::SaveObject()
    { Alert("ClientSite::SaveObject") }

STDMETHODIMP ClientSite::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk)
    { Alert("ClientSite::GetMoniker") }

STDMETHODIMP ClientSite::OnShowWindow(BOOL fShow)
    { Alert("ClientSite::OnShowWindow") }

STDMETHODIMP ClientSite::RequestNewObjectLayout()
    { Alert("ClientSite::RequestNewObjectLayout") }

STDMETHODIMP ClientSite::GetContainer(LPOLECONTAINER FAR* ppContainer)
    { return E_NOTIMPL; }

STDMETHODIMP ClientSite::ShowObject()
    { return S_OK; }

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
InPlaceSite::InPlaceSite(Host* pFrameSite) { m_pHost = pFrameSite; }
InPlaceSite::~InPlaceSite() {}

// IUnknown
STDMETHODIMP InPlaceSite::QueryInterface(REFIID iid, void ** ppvObject)
    { return m_pHost->QueryInterface(iid, ppvObject); }

ULONG STDMETHODCALLTYPE InPlaceSite::AddRef()
    { return m_pHost->AddRef(); }

ULONG STDMETHODCALLTYPE InPlaceSite::Release()
    { return m_pHost->Release(); }

// IOleWindow
HRESULT STDMETHODCALLTYPE InPlaceSite::GetWindow(HWND * phwnd)
    { *phwnd = m_pHost->m_hwndParent; return S_OK; }

HRESULT STDMETHODCALLTYPE InPlaceSite::ContextSensitiveHelp(BOOL fEnterMode)
    { Alert("InPlaceSite::ContextSensitiveHelp") }

// IOleInPlaceSite
HRESULT STDMETHODCALLTYPE InPlaceSite::CanInPlaceActivate(void)
    { return S_OK; }

HRESULT STDMETHODCALLTYPE InPlaceSite::OnInPlaceActivate(void)
    { return S_OK; }

HRESULT STDMETHODCALLTYPE InPlaceSite::OnUIActivate(void)
    { return S_OK; }

HRESULT STDMETHODCALLTYPE InPlaceSite::GetWindowContext(IOleInPlaceFrame **ppFrame,
                                                        IOleInPlaceUIWindow **ppDoc,
                                                        LPRECT lprcPosRect, LPRECT lprcClipRect,
                                                        LPOLEINPLACEFRAMEINFO lpFrameInfo)
    { return S_OK; }

HRESULT STDMETHODCALLTYPE InPlaceSite::Scroll(SIZE scrollExtant)
    { Alert("InPlaceSite::Scroll") }

HRESULT STDMETHODCALLTYPE InPlaceSite::OnUIDeactivate(BOOL fUndoable)
    { Alert("InPlaceSite::OnUIDeactivate") }

HRESULT STDMETHODCALLTYPE InPlaceSite::OnInPlaceDeactivate(void)
    { Alert("InPlaceSite::OnInPlaceDeactivate") }

HRESULT STDMETHODCALLTYPE InPlaceSite::DiscardUndoState(void)
    { Alert("InPlaceSite::DiscardUndoState") }

HRESULT STDMETHODCALLTYPE InPlaceSite::DeactivateAndUndo(void)
    { Alert("InPlaceSite::DeactivateAndUndo") }

HRESULT STDMETHODCALLTYPE InPlaceSite::OnPosRectChange(LPCRECT lprcPosRect)
    { Alert("InPlaceSite::OnPosRectChange") }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
WebBrowserEvents2::WebBrowserEvents2(Host* pFrameSite) { m_pHost = pFrameSite; }
WebBrowserEvents2::~WebBrowserEvents2() {}

// IUnknown
STDMETHODIMP WebBrowserEvents2::QueryInterface(REFIID iid, void ** ppvObject)
    { return m_pHost->QueryInterface(iid, ppvObject); }

ULONG STDMETHODCALLTYPE WebBrowserEvents2::AddRef()
    { return m_pHost->AddRef(); }

ULONG STDMETHODCALLTYPE WebBrowserEvents2::Release()
    { return m_pHost->Release(); }

// IDispatch
HRESULT STDMETHODCALLTYPE WebBrowserEvents2::GetTypeInfoCount(unsigned int FAR* pctinfo)
    { Alert("WebBrowserEvents2::GetTypeInfoCount") }

HRESULT STDMETHODCALLTYPE WebBrowserEvents2::GetTypeInfo(unsigned int iTInfo, LCID  lcid,
                                                         ITypeInfo FAR* FAR*  ppTInfo)
    { Alert("WebBrowserEvents2::GetTypeInfo") }

HRESULT STDMETHODCALLTYPE WebBrowserEvents2::GetIDsOfNames(REFIID riid,
                                                           OLECHAR FAR* FAR* rgszNames,
                                                           unsigned int cNames,
                                                           LCID lcid,
                                                           DISPID FAR* rgDispId)
    { Alert("WebBrowserEvents2::GetIDsOfNames") }

HRESULT STDMETHODCALLTYPE WebBrowserEvents2::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
                                                    WORD wFlags, DISPPARAMS FAR* pDispParams,
                                                    VARIANT FAR* parResult,
                                                    EXCEPINFO FAR* pExcepInfo,
                                                    unsigned int FAR* puArgErr) 
{
//    if(dispIdMember == DISPID_DOCUMENTCOMPLETE)
//    {
//        if (pDispParams->rgvarg[0].pvarVal->vt & VT_BYREF)
//            MessageBoxW(NULL, (LPCWSTR)*pDispParams->rgvarg[0].pvarVal->pbstrVal, (LPCWSTR)*pDispParams->rgvarg[0].pvarVal->pbstrVal, MB_OK);
//        else
//            MessageBoxW(NULL, (LPCWSTR)pDispParams->rgvarg[0].pvarVal->bstrVal, (LPCWSTR)pDispParams->rgvarg[0].pvarVal->bstrVal, MB_OK);
//    }

    return E_NOTIMPL;
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//LRESULT CALLBACK WndProc(HWND hWindow, UINT uiMessage, WPARAM wParam, LPARAM lParam)
//{
//    switch(uiMessage)
//    {
//    case WM_CLOSE:
//        DestroyWindow(hWindow);
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWindow, uiMessage, wParam, lParam);
//    }
//    return NULL;
//}
//
//void main()
//{
//    MSG         msg;
//    HWND        hwnd;
//    WNDCLASSEX  wcexInfo;
//
//    wcexInfo.cbClsExtra = NULL;
//    wcexInfo.cbSize = sizeof(wcexInfo);
//    wcexInfo.cbWndExtra = NULL;
//    wcexInfo.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//    wcexInfo.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wcexInfo.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//    wcexInfo.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//    wcexInfo.hInstance = GetModuleHandle(NULL);
//    wcexInfo.lpfnWndProc = WndProc;
//    wcexInfo.lpszClassName = "NNIExplore";
//    wcexInfo.lpszMenuName = NULL;
//    wcexInfo.style = NULL;
//
//    RegisterClassEx(&wcexInfo);
//    hwnd = CreateWindow("NNIExplore", "Nocturnal Network IE", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
//                        10, 10, 600, 600, NULL, NULL, GetModuleHandle(NULL), NULL);
//    Host *pHost = new Host(hwnd);
//    while(GetMessage(&msg, NULL, NULL, NULL))
//    {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//    pHost->Release();
//    ExitProcess(0);
// }
