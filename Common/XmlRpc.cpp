// XmlRpc.cpp: implementation of the CXmlRpc class.
// 2013-03-13 by hskim, 웹 캐시 상점 Copyright [2002] MasangSoft
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XmlRpc.h"
#include "XmlRpc/XmlRpc.h"

using namespace XmlRpc;

#if defined(_DEBUG)
#pragma comment(lib, "xmlrpc_d.lib")
#else
#pragma comment(lib, "xmlrpc.lib")
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define    XMLRPC_METHODNAME_TAG        "<methodName>"
#define    XMLRPC_PARAMS_TAG            "<params>"
#define    XMLRPC_PARAMS_ETAG            "</params>"
#define    XMLRPC_PARAM_TAG            "<param>"
#define    XMLRPC_PARAM_ETAG            "</param>"

const char CXmlRpc::METHODNAME_TAG[]    = "<methodName>";
const char CXmlRpc::PARAMS_TAG[]        = "<params>";
const char CXmlRpc::PARAMS_ETAG[]        = "</params>";
const char CXmlRpc::PARAM_TAG[]            = "<param>";
const char CXmlRpc::PARAM_ETAG[]        = "</param>";

const std::string CXmlRpc::SYSTEM_MULTICALL = "system.multicall";
const std::string CXmlRpc::METHODNAME        = "methodName";
const std::string CXmlRpc::PARAMS            = "params";

const std::string CXmlRpc::FAULTCODE        = "faultCode";
const std::string CXmlRpc::FAULTSTRING        = "faultString";

CXmlRpc::CXmlRpc()
{
    Clear();
}

CXmlRpc::~CXmlRpc()
{
}

void CXmlRpc::Clear()
{
    m_ParamsValue.clear();
    m_szResponse = "";        //m_szResponse.clear();
    m_szMethodName = "";    //m_szMethodName.clear();
}

BOOL CXmlRpc::IsVaild()
{ 
    return ((!m_szMethodName.empty()) && (m_ParamsValue.getType() == XmlRpc::XmlRpcValue::TypeArray)); 
}

BOOL CXmlRpc::ParseFormXML(char *pXMLText)
{
    Clear();
    
    if( NULL == pXMLText )
    {
        return FALSE;
    }

    std::string xml(pXMLText);

    int offset = 0;

    m_szMethodName = XmlRpc::XmlRpcUtil::parseTag(XMLRPC_METHODNAME_TAG, xml, &offset);
    
    if (! m_szMethodName.empty() && XmlRpc::XmlRpcUtil::findTag(XMLRPC_PARAMS_TAG, xml, &offset))
    {
        int nArgs = 0;

        while( XmlRpc::XmlRpcUtil::nextTagIs(XMLRPC_PARAM_TAG, xml, &offset) ) 
        {
            XmlRpc::XmlRpcValue arg(xml, &offset);

            if ( ! arg.valid()) 
            {
                return TRUE;
            }

            m_ParamsValue[nArgs++] = arg;

            XmlRpc::XmlRpcUtil::nextTagIs(XMLRPC_PARAM_ETAG, xml, &offset);
        }

        XmlRpc::XmlRpcUtil::nextTagIs(XMLRPC_PARAMS_ETAG, xml, &offset);
    }

    return TRUE;
}

void CXmlRpc::GenerateResponse(int iRtnValue)
{
    const char RESPONSE_1[] = 
        "<?xml version=\"1.0\"?>\r\n"
        "<methodResponse><params><param>\r\n";
    const char RESPONSE_2[] =
        "\r\n</param></params></methodResponse>\r\n";

    const char RESPONSE_VALUE[] =
        "<value><i4>";
    const char RESPONSE2_VALUE[] =
        "</i4></value>";

    char szRtnValue[SIZE_MAX_STRING_256] = { 0, };
    sprintf(szRtnValue, "%s%d%s", RESPONSE_VALUE, iRtnValue, RESPONSE2_VALUE);

    std::string rtnvalue(szRtnValue);
    std::string body = RESPONSE_1 + rtnvalue + RESPONSE_2;
    std::string header = GenerateHeader(body);
    
    m_szResponse = header + body;
}

std::string CXmlRpc::GenerateHeader(std::string const& body)
{
    std::string header = 
        "HTTP/1.1 200 OK\r\n"
        "Server: ";
    header += "Masangsoft Game Server";
    header += "\r\n"
        "Content-Type: text/xml\r\n"
        "Content-Length: ";
    
    char buffLen[40];
    sprintf(buffLen,"%d\r\n\r\n", body.size());
    
    return header + buffLen;
}


void CXmlRpc::GenerateFaultResponse(std::string const& errorMsg, int errorCode)
{
    const char RESPONSE_1[] = 
        "<?xml version=\"1.0\"?>\r\n"
        "<methodResponse><fault>\r\n";
    const char RESPONSE_2[] =
        "\r\n</fault></methodResponse>\r\n";
    
    XmlRpcValue faultStruct;
    faultStruct[FAULTCODE] = errorCode;
    faultStruct[FAULTSTRING] = errorMsg;
    std::string body = RESPONSE_1 + faultStruct.toXml() + RESPONSE_2;
    std::string header = GenerateHeader(body);
    
    m_szResponse = header + body;
}
