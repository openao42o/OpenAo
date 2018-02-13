#include "stdafx.h"
#include "config.h"

CConfig::CConfig()
{
}

BOOL CConfig::LoadUserDefinedVariables(const char *configFile)
{
	char		*token;
	char		buff[CONFIG_SIZE_BUFF];
	const char	seps[] = " \t=";
	ifstream	fin;

	fin.open(configFile);

	if (!fin.is_open())
	{
		int nErr = GetLastError();
		SetLastError(0);
		TCHAR	szError[1024];
		TCHAR	szDir[512];
		GetCurrentDirectory(512, szDir);
		wsprintf(szError, L"[ERROR] CConfig::LoadUserDefinedVariables open() error, LastError[%d] ConfigFileName[%s] CurrentDirectory[%s]\r\n"
				, nErr, configFile, szDir);
		m_LogManager.WriteSystemLog(szError);
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, CONFIG_SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, CONFIG_SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}

		// 라인 검사, ';'로 끝나는 라인은 ';'을 NULL로 바꾸고 뒷 부분은 무시한다.
		char *pLineEnd = strstr(buff, ";");
		if (pLineEnd != NULL)
		{
			pLineEnd[0] = NULL;
		}
		
		char *next_token1 = NULL;
		token = ::strtok_s(buff, seps, &next_token1);

		// ignore blank lines
		if (token == NULL)
		{
			continue;
		}

		// ignore comments
		if (strncmp(token, "#", 1) == 0)
		{
			continue;
		}

		// user defined variable
		//if (0 == strncmp(token, "$$", 2))
		//{
		//	char key[CONFIG_SIZE_BUFF];
		//	STRNCPY_S_MEMSET(key, token, CONFIG_SIZE_BUFF);

		//	char value[CONFIG_SIZE_BUFF];
		//	util::zero(value, CONFIG_SIZE_BUFF);
		//	while(TRUE)
		//	{
		//		token = (char*)GetToken((char*)::strtok_s(NULL, seps, &next_token1));
		//		if(NULL == token)
		//		{
		//			break;
		//		}
		//		int nLen = strlen(value);
		//		if(nLen + strlen(token) + 1 >= CONFIG_SIZE_BUFF)
		//		{
		//			break;
		//		}
		//		sprintf_s(value, &(value[nLen]), "%s ", token);				
		//	}
		//	m_mapUserDefinedVariables.insert(pair<string, string>(key, value));
		//}
		//else if (strncmp(token, "$", 1) == 0)
		//{
		//	char key[CONFIG_SIZE_BUFF];
		//	char value[CONFIG_SIZE_BUFF];

		//	STRNCPY_S_MEMSET(key, token, CONFIG_SIZE_BUFF);

		//	token = (char*)GetToken((char*)::strtok_s(NULL, seps, &next_token1));
		//	if (token != NULL)
		//	{
		//		STRNCPY_S_MEMSET(value, token, CONFIG_SIZE_BUFF);
		//	}
		//	else
		//	{
		//		STRNCPY_S_MEMSET(value, "", CONFIG_SIZE_BUFF);
		//	}


		//	m_mapUserDefinedVariables.insert(pair<string, string>(key, value));
		//}
	}

	fin.close();

	return TRUE;
}

const char* CConfig::GetToken(char* token)
{
	if (token == NULL) return NULL;

	map<string, string, greater<string> >::iterator itr = m_mapUserDefinedVariables.find(token);

	if (itr != m_mapUserDefinedVariables.end())
	{
		return itr->second.c_str();
	}

	if(0 == strncmp(token, "$", 1)
		|| 0 == strncmp(token, "#", 1))
	{
		return NULL;
	}
	return token;
}

char* CConfig::getQuotedString(char *strToken, const char* strDelimit)
{
	char *pTmpToken = NULL;
	char *next_token1 = NULL;

	pTmpToken = ::strtok_s(strToken, "\"", &next_token1);
	pTmpToken = ::strtok_s(strToken, "\"", &next_token1);

	return pTmpToken;
}

void CConfig::GetTokenList(vectstring *o_pTokenList, char *strToken, const char *strDelimit)
{
	o_pTokenList->clear();
	char *next_token1 = NULL;

	char *token = ::strtok_s(strToken, strDelimit, &next_token1);
	while(token)
	{
		o_pTokenList->push_back(token);
		
		token = ::strtok_s(NULL, strDelimit, &next_token1);
	}
}
