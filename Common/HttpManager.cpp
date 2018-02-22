//Copyright[2002] MasangSoft
// HttpManager.cpp: implementation of the CHttpManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumError.h"
#include "HttpManager.h"
#include <wininet.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DWORD WINAPI DownloadThreadByHttp(LPVOID lpParam);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpManager::CHttpManager()
{
    m_threadDownload                = nullptr;
    m_bDownloadThreadCancelFlag        = false;
    util::zero(m_szServerName, HM_SIZE_MAX_FILE_NAME);
    m_nServerPort                    = 0;
    util::zero(m_szRemoteFileName, HM_SIZE_MAX_FILE_NAME);
    util::zero(m_szLocalFileName, HM_SIZE_MAX_FILE_NAME);
    m_hUpdateWindow                    = 0;
}

CHttpManager::~CHttpManager()
{
    if (m_threadDownload)
    {
        if (m_threadDownload->joinable())
            
            m_threadDownload->join();

        delete m_threadDownload;
    }
}


bool CHttpManager::ThreadEnd(unsigned reltime/*=1000*/)
{
    auto bRet = true;

    auto t = GetDownloadThreadHandle();

    if (t)
    {
        auto status = m_futureDownload.wait_for(chrono::milliseconds(reltime));

        if (status != future_status::ready) bRet = false;

        this_thread::sleep_for(100ms);
        
        if (t->joinable()) t->detach();

        delete t;

        m_threadDownload = nullptr;
    }

    return bRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            Err_t CHttpManager::DownloadFileByHttp(const char *i_pServerName, int i_nServerPort, const char *i_pRemoteFileName, const char *i_pLocalFileName, BOOL i_bUseThread/*=FALSE*/, HWND i_hUpdateWindow/*=0*/)
/// \brief        
/// \author        cmkwon
/// \date        2007-01-03 ~ 2007-01-03
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
Err_t CHttpManager::DownloadFileByHttp(const char *i_pServerName, int i_nServerPort, const char *i_pRemoteFileName, const char *i_pLocalFileName, BOOL i_bUseThread/*=FALSE*/, HWND i_hUpdateWindow/*=0*/)
{
    util::strncpy(m_szServerName, i_pServerName, HM_SIZE_MAX_FILE_NAME);
    m_nServerPort            = i_nServerPort;
    util::strncpy(m_szRemoteFileName, i_pRemoteFileName, HM_SIZE_MAX_FILE_NAME);
    util::strncpy(m_szLocalFileName, i_pLocalFileName, HM_SIZE_MAX_FILE_NAME);
    if (i_bUseThread)
    {
        m_hUpdateWindow        = i_hUpdateWindow;
        
        //unsigned thrdaddr;
        //m_hDownloadThread = ch BEGINTHREADEX (NULL, 0, DownloadThreadByHttp, (LPVOID)this, 0, &thrdaddr);
        //if (NULL == m_hDownloadThread)
        //{
        //    return FALSE;
        //}

        try
        {
            packaged_task<DWORD(LPVOID)> task { DownloadThreadByHttp };

            m_futureDownload = task.get_future();
            
            m_threadDownload = new thread { move(task), LPVOID(this) };
        }
        catch (const system_error& error)
        {
            return ERR_COMMON_UNKNOWN_ERROR;
        }

        return ERR_NO_ERROR;
    }

    return _DownLoadFileByHttp();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            Err_t CHttpManager::_DownLoadFileByHttp(void)
/// \brief        
/// \author        cmkwon
/// \date        2007-01-04 ~ 2007-01-04
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
Err_t CHttpManager::_DownLoadFileByHttp(void)
{
    HINTERNET hInternetSession = ::InternetOpen(STRMSG_WINDOW_TEXT, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (NULL == hInternetSession)
    {// 2007-01-05 by cmkwon, 인터넷이 연결되어 있지 않습니다

        return ERR_CANNOT_CONNECT_INTERNET;
    }

    HINTERNET hHttpConnection = ::InternetConnect(hInternetSession, m_szServerName, m_nServerPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, (DWORD)this );
    if (NULL == hHttpConnection)
    {// 2007-01-05 by cmkwon, 서버 오류로 연결할 수 없습니다
        ::InternetCloseHandle(hInternetSession);
        hInternetSession    = NULL;

        return ERR_CANNOT_CONNECT_AUTO_UPDATE_SERVER;
    }

    LPCTSTR        ppszAcceptTypes[2];
    ppszAcceptTypes[0]    = _T("*/*");
    ppszAcceptTypes[1]    = NULL;
    DWORD        dwFlag    = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION;
    HINTERNET hHttpFile    = ::HttpOpenRequest(hHttpConnection, NULL, m_szRemoteFileName, NULL, NULL, ppszAcceptTypes, dwFlag, (DWORD)this );
    if (NULL == hHttpFile)
    {
        ::InternetCloseHandle(hHttpConnection);
        hHttpConnection        = NULL;
        ::InternetCloseHandle(hInternetSession);
        hInternetSession    = NULL;

        return ERR_FUNC_HttpOpenRequest;
    }

    if (FALSE == ::HttpSendRequest(hHttpFile, NULL, 0, NULL, 0))
    {
        ::InternetCloseHandle(hHttpFile);
        hHttpFile            = NULL;
        ::InternetCloseHandle(hHttpConnection);
        hHttpConnection        = NULL;
        ::InternetCloseHandle(hInternetSession);
        hInternetSession    = NULL;

        return ERR_FUNC_HttpSendRequest;
    }

    TCHAR    szStatusCode[32];
    DWORD    dwInfoSize = 32;
    int        nStatusCode;
    if (FALSE == ::HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
    {
        ::InternetCloseHandle(hHttpFile);
        hHttpFile            = NULL;
        ::InternetCloseHandle(hHttpConnection);
        hHttpConnection        = NULL;
        ::InternetCloseHandle(hInternetSession);
        hInternetSession    = NULL;

        return ERR_FUNC_HttpQueryInfo;
    }
    
    nStatusCode = atoi( szStatusCode );
    switch (nStatusCode)
    {
    case HTTP_STATUS_OK:
        break;
    default:
        {
            ::InternetCloseHandle(hHttpFile);
            hHttpFile            = NULL;
            ::InternetCloseHandle(hHttpConnection);
            hHttpConnection        = NULL;
            ::InternetCloseHandle(hInternetSession);
            hInternetSession    = NULL;
            
            if (HTTP_STATUS_PROXY_AUTH_REQ == nStatusCode
                || HTTP_STATUS_DENIED == nStatusCode)
            {
                return ERR_AUTO_UPDATE_SERVER_PERMISSION_DENIED;
            }
            else
            {
                return ERR_FUNC_HttpQueryInfo;
            }
        }
    }

    DWORD dwContentLen = 0;
    dwInfoSize            = sizeof(dwContentLen);
    if (FALSE == ::HttpQueryInfo(hHttpFile,  HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&dwContentLen, &dwInfoSize, NULL))
    {// 2007-01-05 by cmkwon, 다운 로드 파일 크기 가져오기

        dwContentLen    = 104857600;    // 2007-01-05 by cmkwon, 100MB = 100 * 1024 * 1024
    }

    ///////////////////////////////////////////////////////////////////////////////
    // 다운로드 파일 크기를 Post 한다
    if (m_hUpdateWindow)
    {
        ::PostMessage(m_hUpdateWindow, WM_UPDATEFILE_DOWNLOAD_INIT, dwContentLen, 0);
    }

    CFile fileObject;
    CFileException ex;
    if (FALSE == fileObject.Open(m_szLocalFileName, CFile::modeCreate | CFile::modeWrite, &ex))
    {
        TCHAR szError[1024];
        ex.GetErrorMessage(szError, 1024);
        DbgOut("Unable to create file(%s): %s\r\n", m_szLocalFileName, szError);

        ::InternetCloseHandle(hHttpFile);
        hHttpFile            = NULL;
        ::InternetCloseHandle(hHttpConnection);
        hHttpConnection        = NULL;
        ::InternetCloseHandle(hInternetSession);
        hInternetSession    = NULL;
        return ERR_LOCAL_FILE_CREATE_FAIL;
    }

    #define _DOWN_BUFFER_SIZE            4096
    char buffer[_DOWN_BUFFER_SIZE];
    DWORD dwRead = 0;
    DWORD dwTotalRead    = 0;
    do
    {
        if ( FALSE == ::InternetReadFile(hHttpFile, buffer, _DOWN_BUFFER_SIZE, &dwRead) )
        {
            DbgOut("InternetReadFile ERROR: %d, TotalRead(%d)\n", GetLastError(), dwTotalRead);

            fileObject.Close();
            ::DeleteFile(m_szLocalFileName);

            ::InternetCloseHandle(hHttpFile);
            hHttpFile            = NULL;
            ::InternetCloseHandle(hHttpConnection);
            hHttpConnection        = NULL;
            ::InternetCloseHandle(hInternetSession);
            hInternetSession    = NULL;
            
            return ERR_UPDATE_FILE_DOWNLOADING_FAIL;
        }

        if (m_bDownloadThreadCancelFlag)
        {// 2007-01-05 by cmkwon, 다운 로드 취소
            
            fileObject.Close();
            ::DeleteFile(m_szLocalFileName);

            ::InternetCloseHandle(hHttpFile);
            hHttpFile            = NULL;
            ::InternetCloseHandle(hHttpConnection);
            hHttpConnection        = NULL;
            ::InternetCloseHandle(hInternetSession);
            hInternetSession    = NULL;

            return ERR_UPDATE_FILE_DOWNLOADING_CANCEL;
        }
        
        if (0 < dwRead)
        {
            fileObject.Write(buffer, dwRead);
            dwTotalRead += dwRead;

            if (m_hUpdateWindow)
            {
                ::PostMessage(m_hUpdateWindow, WM_UPDATEFILE_DOWNLOAD_PROGRESS, dwTotalRead, 0);
            }
        }

        // 2009-01-21 by cmkwon, 자동 업데이트 다운로드 속도 개선 - HTTP, Sleep(10)을 주석 처리함.
        //Sleep(10);
    } while (0 != dwRead);


    fileObject.Close();
    ::InternetCloseHandle(hHttpFile);
    hHttpFile            = NULL;
    ::InternetCloseHandle(hHttpConnection);
    hHttpConnection        = NULL;
    ::InternetCloseHandle(hInternetSession);
    hInternetSession    = NULL;

    return ERR_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            DWORD WINAPI DownloadThreadByHttp(LPVOID lpParam)
/// \brief        
/// \author        cmkwon
/// \date        2007-01-05 ~ 2007-01-05
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
DWORD WINAPI DownloadThreadByHttp(LPVOID lpParam)
{
    CHttpManager *phttpMan = (CHttpManager*)lpParam;

    Err_t err = phttpMan->_DownLoadFileByHttp();

    if (ERR_NO_ERROR == err)
    {
        phttpMan->PostMessage2UpdateWindow(WM_UPDATEFILE_DOWNLOAD_OK, 0, 0);
    }
    else
    {
        phttpMan->PostMessage2UpdateWindow(WM_UPDATEFILE_DOWNLOAD_ERROR, ERR_UPDATE_FILE_NOT_FOUND, 0);
    }
    return 0x20;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CHttpManager::PostMessage2UpdateWindow(UINT i_uiMsgTy, WPARAM i_wParam, LPARAM i_lParam)
/// \brief        
/// \author        cmkwon
/// \date        2007-01-05 ~ 2007-01-05
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CHttpManager::PostMessage2UpdateWindow(UINT i_uiMsgTy, WPARAM i_wParam, LPARAM i_lParam)
{
    if (m_hUpdateWindow)
    {
        ::PostMessage(m_hUpdateWindow, i_uiMsgTy, i_wParam, i_lParam);
    }
}
