//*****************************************************************************\
//IExplore.h
//Copyright (c) 2002 Matthew Blagden <info@nocturnalnetwork.com>
//You may use this code in your applications at no cost, so long as credit
//is given to Matthew Blagden, nocturnalnetwork.com, or The Nocturnal Network.
//\*****************************************************************************/

// 2009-09-02 by cmkwon, Gameforge4D 게임 가드 동의창 WebPage로 처리 - 
#ifndef COMMON_IEXPLORE_H_
#define COMMON_IEXPLORE_H_


#include <olectl.h>
#include <exdispid.h>
#include <exdisp.h>

class Host;
class ClientSite;
class InPlaceSite;
class WebBrowserEvents2;

class Host : public IUnknown
{
public:
    Host(HWND hwndParent,LPSTR strWebAddress, LPSTR szPostData, LPSTR szPostData2, LPSTR szHeaders, RECT *i_prtHostrWindowPos=NULL);
    ~Host();

    // IUnknown
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    STDMETHODIMP QueryInterface(REFIID iid, void ** ppvObject);

    // Classes Implemented
    ClientSite         *m_pClientSite;
    InPlaceSite        *m_pInPlaceSite;
    WebBrowserEvents2  *m_pWebBrowserEvents2;
    IUnknown                   *m_pUnknown;            
    IConnectionPoint           *m_pConnectionPoint;
    IConnectionPointContainer  *m_pConnectionPointContainer;
    IOleInPlaceObject          *m_pInPlaceObject;
    IOleObject                 *m_pObject;

    // Master reference count
    DWORD               m_dwRefCount;

    // Window to host IE in
    HWND                m_hwndParent;
	
	IWebBrowser2*		m_pWebBrowser;

	void		Refresh();
	RECT		ReSizeWindow();
	void		ReSizeRect(RECT rt);
};

class ClientSite : public IOleClientSite
{
public:
    ClientSite(Host* pFrameSite);
    ~ClientSite();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void ** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    // IOleClientSite
    STDMETHODIMP GetContainer(LPOLECONTAINER FAR* ppContainer);
    STDMETHODIMP SaveObject();
    STDMETHODIMP GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk);
    STDMETHODIMP ShowObject();
    STDMETHODIMP OnShowWindow(BOOL fShow);
    STDMETHODIMP RequestNewObjectLayout();

private:
    Host *m_pHost;
};

class InPlaceSite : public IOleInPlaceSite
{
public:
    InPlaceSite(Host* pFrameSite);
    ~InPlaceSite();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void ** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    // IOleWindow
    HRESULT STDMETHODCALLTYPE GetWindow(HWND * phwnd);
    HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);

    // IOleInPlaceSite
    HRESULT STDMETHODCALLTYPE CanInPlaceActivate(void);
    HRESULT STDMETHODCALLTYPE OnInPlaceActivate(void);
    HRESULT STDMETHODCALLTYPE OnUIActivate(void);
    HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame **ppFrame,
                                               IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect,
                                               LPRECT lprcClipRect,
                                               LPOLEINPLACEFRAMEINFO lpFrameInfo);
    HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant);
    HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable);
    HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate(void);
    HRESULT STDMETHODCALLTYPE DiscardUndoState(void);
    HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void);
    HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect);

private:
    Host *m_pHost;
};

class WebBrowserEvents2 : public IDispatch
{
public:
    WebBrowserEvents2(Host* pFrameSite);
    ~WebBrowserEvents2();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID iid, void ** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    // IDispatch
    HRESULT STDMETHODCALLTYPE GetTypeInfoCount(unsigned int FAR* pctinfo);
    HRESULT STDMETHODCALLTYPE GetTypeInfo(unsigned int iTInfo, LCID  lcid,
                                          ITypeInfo FAR* FAR*  ppTInfo);
    HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, OLECHAR FAR* FAR* rgszNames,
                                            unsigned int cNames, LCID lcid, DISPID FAR* rgDispId);
    HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
                                     DISPPARAMS FAR* pDispParams, VARIANT FAR* parResult,
                                     EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr);

private:
    Host *m_pHost;
};

#endif // COMMON_IEXPLORE_H_
