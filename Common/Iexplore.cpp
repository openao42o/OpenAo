/*****************************************************************************\
IExplore.cpp
Copyright (c) 2002 Matthew Blagden <info@nocturnalnetwork.com>
You may use this code in your applications at no cost, so long as credit
is given to Matthew Blagden, nocturnalnetwork.com, or The Nocturnal Network.
\*****************************************************************************/
#include "stdafx.h"
#include "IExplore.h"
#include <TCHAR.H>
#include <exdisp.h>		// Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above
#include <mshtml.h>		// Defines of stuff like IHTMLDocument2. This is an include file with Visual C 6 and above



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

//inline void * __cdecl operator new(size_t size) { return (PBYTE)LocalAlloc(LPTR, size); }
//inline void __cdecl operator delete(void *ptr)  { if (ptr) LocalFree(ptr); }


/////////////////////////////////////////////////////////////////////////////////////////
// 2007-09-03 by cmkwon, 인자 추가함(, RECT *i_prtHostrWindowPos/*=NULL*/) - 초기 생성 윈도우 설정
Host::Host(HWND hwndParent,LPSTR strWebAddress, LPSTR szPostData, LPSTR szPostData2, LPSTR szHeaders, RECT *i_prtHostrWindowPos/*=NULL*/)
{
    #define Assert(x) if(!x) MessageBox(NULL, "IE ERROR!", "Error", MB_ICONSTOP)

    HRESULT                     hret;
//    IUnknown                   *pUnknown;            
//    IWebBrowser2               *pWebBrowser;
//    IConnectionPoint           *pConnectionPoint;
//    IConnectionPointContainer  *pConnectionPointContainer;
//    IOleInPlaceObject          *pInPlaceObject;
//    IOleObject                 *pObject;
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
	m_pWebBrowser			= NULL;				// 2007-09-06 by cmkwon, 초기화

    // Instantiate Web Browser control
    hret = CoCreateInstance(CLSID_WebBrowser, NULL, CLSCTX_ALL, IID_IUnknown, (void**)(&m_pUnknown));
    Assert(SUCCEEDED(hret));

    // Set the client site
    hret = m_pUnknown->QueryInterface(IID_IOleObject, (void**)(&m_pObject));
    Assert(SUCCEEDED(hret));
    hret = m_pObject->SetClientSite(m_pClientSite);
    Assert(SUCCEEDED(hret));

//    // Set the site limits
//    RECT rWindowPos;
//    GetClientRect(hwndParent, &rWindowPos);
//
//    hret = m_pUnknown->QueryInterface(IID_IOleInPlaceObject, (void**)(&m_pInPlaceObject));
//    Assert(SUCCEEDED(hret));
//    hret = m_pInPlaceObject->SetObjectRects(&rWindowPos, &rWindowPos);
//    Assert(SUCCEEDED(hret));
//    m_pInPlaceObject->Release();

	RECT rWindowPos;
	
	if(NULL == i_prtHostrWindowPos)
	{
		rWindowPos = ReSizeWindow();
	}
	else
	{
		rWindowPos	= *i_prtHostrWindowPos;
		this->ReSizeRect(rWindowPos);
	}
	
    // Activate the site
    hret = m_pObject->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, m_pClientSite, 0, hwndParent, &rWindowPos);
    Assert(SUCCEEDED(hret));
	m_pObject->Release();

    // Set event handler
    hret = m_pUnknown->QueryInterface(IID_IConnectionPointContainer, (void**)(&m_pConnectionPointContainer));
    Assert(SUCCEEDED(hret));
    hret = m_pConnectionPointContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &m_pConnectionPoint);
    Assert(SUCCEEDED(hret));
    hret = m_pConnectionPoint->Advise(m_pWebBrowserEvents2, &dwAdviseCookie);
    Assert(SUCCEEDED(hret));
    m_pConnectionPointContainer->Release();
    m_pConnectionPoint->Release();

    // Get Web Browser interface
    hret = m_pUnknown->QueryInterface(IID_IWebBrowser2, (void**)&m_pWebBrowser);
    Assert(SUCCEEDED(hret));
    if(m_pUnknown) m_pUnknown->Release();

	// Transfer BSTR from LPSTR
	VARIANT *pvarURL;
	VARIANT varURL;		

 	pvarURL = NULL;
	
	if ( !strWebAddress )
		return;
 
	if ( !*strWebAddress )
		return;
 
	int len = lstrlen( strWebAddress ) + 1;
 
	WCHAR *pszW;
 	pszW = new WCHAR[len];
 
	MultiByteToWideChar( CP_ACP, 0, strWebAddress, -1, pszW, len );
	VariantInit( &varURL );
 
	varURL.vt = VT_BSTR;
	varURL.bstrVal = SysAllocString( pszW );

	delete []pszW;

	pvarURL = &varURL;

	// Go to Url
	hret = m_pWebBrowser->Navigate2( pvarURL, NULL, NULL, NULL, NULL  );
//	hret = pWebBrowser->Navigate( strWebAddress, NULL, NULL, pvarPostData, pvarHeaders  );

	VariantClear( pvarURL );
 
    Assert(SUCCEEDED(hret));
	m_pClientSite->Release();
    if(m_pWebBrowser) 
	{
		m_pWebBrowser->Release();
	}
	
// 2007-09-06 by cmkwon, 정상 동작 하지 않음
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2007-09-06 by cmkwon, 스크롤 삭제하기
//	hr = m_pWebBrowser->QueryInterface(IID_IWebBrowser2, reinterpret_cast<void**>(&pWB));
//	IDispatch *pDocDisp		= NULL;
//	hret = m_pWebBrowser->get_Document(&pDocDisp);
//	if(SUCCEEDED(hret) && pDocDisp)
//	{
//		IHTMLDocument2 *pDoc	= NULL;
//		hret = pDocDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pDoc);
//		if(SUCCEEDED(hret) && pDoc)
//		{
//			IHTMLElement *pIElement = NULL;
//			hret = pDoc->get_body(&pIElement);
//
//			IHTMLBodyElement *pIBodyElement = NULL;
//			hret = pIElement->QueryInterface(IID_IHTMLBodyElement, (void**)&pIBodyElement);
//
//			CString strOption = _T("no");   // yes no auto
//			BSTR bstr;
//			bstr = strOption.AllocSysString();
//			pIBodyElement->put_scroll(bstr);
//		}
//		pDoc->Release();
//	}
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

RECT Host::ReSizeWindow()
{
    // Set the site limits
	HRESULT hr;
    RECT rWindowPos;
    GetClientRect(m_hwndParent, &rWindowPos);

    hr = m_pUnknown->QueryInterface(IID_IOleInPlaceObject, (void**)(&m_pInPlaceObject));
    Assert(SUCCEEDED(hr));
    hr = m_pInPlaceObject->SetObjectRects(&rWindowPos, &rWindowPos);
    Assert(SUCCEEDED(hr));
    m_pInPlaceObject->Release();

	return rWindowPos;
}

void Host::ReSizeRect(RECT rt)
{
    // Set the site limits
	HRESULT hr;

    hr = m_pUnknown->QueryInterface(IID_IOleInPlaceObject, (void**)(&m_pInPlaceObject));
    Assert(SUCCEEDED(hr));
    hr = m_pInPlaceObject->SetObjectRects(&rt, &rt);
    Assert(SUCCEEDED(hr));
    m_pInPlaceObject->Release();
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
	if(m_pWebBrowser)
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

