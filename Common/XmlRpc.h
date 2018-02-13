// XmlRpc.h: interface for the CXmlRpc class.
// 2013-03-13 by hskim, 웹 캐시 상점
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLRPC_H__4E7BD9D8_0358_48D1_B745_BF6E308CCABC__INCLUDED_)
#define AFX_XMLRPC_H__4E7BD9D8_0358_48D1_B745_BF6E308CCABC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XmlRpc/XmlRpc.h"

class CXmlRpc
{
public:
	CXmlRpc();
	virtual ~CXmlRpc();

	void Clear();

	BOOL ParseFormXML(char *pXMLText);

	BOOL IsEmpty()							{ return m_szMethodName.empty(); }
	BOOL IsVaild();
	const char *GetMethodName()				{ return m_szMethodName.c_str(); }

	int	GetParamsCount()					{ return m_ParamsValue.size(); }
	XmlRpc::XmlRpcValue &GetParamsValue()	{ return m_ParamsValue; }

public:
	void GenerateResponse(int iRtnValue);
	std::string GenerateHeader(std::string const& body);
	void GenerateFaultResponse(std::string const& errorMsg, int errorCode);

public:
	std::string						m_szMethodName;
	std::string						m_szResponse;
	XmlRpc::XmlRpcValue				m_ParamsValue;

private:
    static const char METHODNAME_TAG[];
    static const char PARAMS_TAG[];
    static const char PARAMS_ETAG[];
    static const char PARAM_TAG[];
    static const char PARAM_ETAG[];

    static const std::string SYSTEM_MULTICALL;
    static const std::string METHODNAME;
    static const std::string PARAMS;

    static const std::string FAULTCODE;
    static const std::string FAULTSTRING;

};

#endif // !defined(AFX_XMLRPC_H__4E7BD9D8_0358_48D1_B745_BF6E308CCABC__INCLUDED_)
