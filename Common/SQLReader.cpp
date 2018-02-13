// SQLReader.cpp: implementation of the CSQLReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SQLReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLReader::CSQLReader()
{

}

CSQLReader::~CSQLReader()
{

}

void CSQLReader::printf2(char* pFormat, ...)
{
	char buff2[1024];

	va_list args;
	va_start(args, pFormat);
	vsprintf(buff2, pFormat, args);
	va_end(args);

	m_szOutText += buff2;
	DBGOUT(buff2);
}

BOOL CSQLReader::IsComment(const char* token)
{
	// ignore comments
	if (strncmp(token, "-", 1) == 0
		|| strncmp(token, "/", 1) == 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CSQLReader::GetSQLType(string &type)
{
	if (type == sqlTypeArr[SQL_TYPE_TINYINT].strTypeSQL) { return SQL_TYPE_TINYINT; }
	else if (type == sqlTypeArr[SQL_TYPE_SMALLINT].strTypeSQL) { return SQL_TYPE_SMALLINT; }
	else if (type == sqlTypeArr[SQL_TYPE_INT].strTypeSQL) { return SQL_TYPE_INT; }
	else if (type == sqlTypeArr[SQL_TYPE_BIGINT].strTypeSQL) { return SQL_TYPE_BIGINT; }
	else if (type == sqlTypeArr[SQL_TYPE_FLOAT].strTypeSQL) { return SQL_TYPE_FLOAT; }
	else if (type == sqlTypeArr[SQL_TYPE_VARCHAR].strTypeSQL) { return SQL_TYPE_VARCHAR; }

	return SQL_TYPE_NA;
}

BOOL CSQLReader::CreateAccessCode(char *szTableName, BOOL bCBArray)
{
	// output text 초기화
	m_szOutText = "";

	char		*token;
	char		buff[LINE_BUFF];
	const char	seps[] = " \t,[]()";
	ifstream	fin;
	int nColumnCount = 1;

	fin.open(szTableName);

	if (!fin.is_open())
	{
		int nErr = GetLastError();
		SetLastError(0);
		char	szDir[512];
		GetCurrentDirectory(512, szDir);
		printf2("[Error] open() error, LastError[%d] ConfigFileName[%s] CurrentDirectory[%s]\r\n"
			, nErr, szTableName, szDir);
		return FALSE;
	}

	while(TRUE)
	{
		int nTokenOffset = 0;
		string arrToken[MAX_TOKEN_PER_LINE];

		memset(buff, 0x00, LINE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, LINE_BUFF).eof();

		// table 정의의 끝이면 cbXX 출력
		if (strcmp(")", buff) == 0)
		{
			printf2("\r\nSQLINTEGER\t");
			for (int i = 0; i < nColumnCount-1; i++)
			{
				printf2("cb%d", i+1);
				if (i != nColumnCount-2)
				{
					printf2(", ");
				}
				else
				{
					printf2(";\r\n");
				}

				if (i != 1 && i%10 == 9)
				{
					printf2("\r\n\t\t");
				}
			}
		}

		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		token = ::strtok(buff, seps);

		// ignore blank lines
		if (token == NULL)
		{
			continue;
		}

		// ignore comments
		if (IsComment(token))
		{
			continue;
		}

		// 첫 토큰 할당

		arrToken[nTokenOffset] = token;
#ifdef _DEBUG
		assert(nTokenOffset == 0);
#endif
		while (++nTokenOffset < MAX_TOKEN_PER_LINE)
		{
			token = ::strtok(0, seps);
			if (token == NULL || IsComment(token))
			{
				break;
			}

			arrToken[nTokenOffset] = token;
		}

		int sqlType = SQL_TYPE_NA;

		// CREATE TABLE
		if (
			arrToken[0] == "CREATE"
		&&	arrToken[1] == "TABLE"
		)
		{
			printf2("\r\n///////////////////////////////////////////////////////////\r\n");
			printf2("// Table: %s\r\n", arrToken[2].c_str());
			printf2("///////////////////////////////////////////////////////////\r\n");
			nColumnCount = 1;
		}
		// column
		else if ( (sqlType = GetSQLType(arrToken[1])) != SQL_TYPE_NA)
		{
			if (sqlType == SQL_TYPE_VARCHAR)
			{
				if (bCBArray)
				{
					printf2("cb%d = SQL_NTS;\r\nSQLBindCol(hstmt, %2d, %-15s, retVar.%-20s, SIZE_MAX_, &cb[%d]);\r\n"
						, nColumnCount, nColumnCount, sqlTypeArr[sqlType].strTargetType, arrToken[0].c_str(), nColumnCount);
				}
				else
				{
					printf2("cb%d = SQL_NTS;\r\nSQLBindCol(hstmt, %2d, %-15s, retVar.%-20s, SIZE_MAX_, &cb%d);\r\n"
						, nColumnCount, nColumnCount, sqlTypeArr[sqlType].strTargetType, arrToken[0].c_str(), nColumnCount);
				}
			}
			else
			{
				if (bCBArray)
				{
					printf2("SQLBindCol(hstmt, %2d, %-15s, &retVar.%-19s, 0        , &cb[%d]);\r\n"
						, nColumnCount, sqlTypeArr[sqlType].strTargetType, arrToken[0].c_str(), nColumnCount);
				}
				else
				{
					printf2("SQLBindCol(hstmt, %2d, %-15s, &retVar.%-19s, 0        , &cb%d);\r\n"
						, nColumnCount, sqlTypeArr[sqlType].strTargetType, arrToken[0].c_str(), nColumnCount);
				}
			}
			nColumnCount++;
		}
	}

	fin.close();

	return TRUE;
}
