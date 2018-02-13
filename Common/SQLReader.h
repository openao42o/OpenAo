// SQLReader.h: interface for the CSQLReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLREADER_H__C160A48A_73B6_4154_842D_65EF8A4F193B__INCLUDED_)
#define AFX_SQLREADER_H__C160A48A_73B6_4154_842D_65EF8A4F193B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LINE_BUFF			512
#define MAX_TOKEN_PER_LINE	10

#include <assert.h>
#include <string>
#include <fstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// SQL TYPE
///////////////////////////////////////////////////////////////////////////////
#define SQL_TYPE_TINYINT	0
#define SQL_TYPE_SMALLINT	1
#define SQL_TYPE_INT		2
#define SQL_TYPE_BIGINT		3
#define SQL_TYPE_FLOAT		4
#define SQL_TYPE_VARCHAR	5
#define SQL_TYPE_NA			99

struct SQL_TYPE_INFO
{
	int			nType;
	const char	*strTypeSQL;
	const char	*strTypeC;
	const char	*strTargetType;
};

static SQL_TYPE_INFO sqlTypeArr[] = {
	{SQL_TYPE_TINYINT,	"TINYINT",		"BYTE",		"SQL_C_UTINYINT"},
	{SQL_TYPE_SMALLINT,	"SMALLINT",		"SHORT",	"SQL_C_SHORT"},
	{SQL_TYPE_INT,		"INT",			"INT",		"SQL_C_LONG"},
	{SQL_TYPE_BIGINT,	"BIGINT",		"LONGLONG",	"SQL_C_BIGINT"},
	{SQL_TYPE_FLOAT,	"FLOAT",		"float",	"SQL_C_FLOAT"},
	{SQL_TYPE_VARCHAR,	"VARCHAR",		"char",		"SQL_C_CHAR"},
	{SQL_TYPE_NA,		"",				"",			""}
};

class CSQLReader
{
public:
	CSQLReader();
	virtual ~CSQLReader();

public:
	BOOL CreateAccessCode(char *szTableName, BOOL bCBArray);
	BOOL IsComment(const char* token);
	BOOL GetSQLType(string &type);
	void printf2(char* pFormat, ...);

public:
	string m_szOutText;
};

#endif // !defined(AFX_SQLREADER_H__C160A48A_73B6_4154_842D_65EF8A4F193B__INCLUDED_)
