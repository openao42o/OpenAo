//Copyright[2002] MasangSoft
#include "stdafx.h"
#include "config.h"

#ifndef    _ATUM_SERVER
#define DBGOUT(x)
#define g_pGlobal->WriteSystemLog(x)
#endif

using namespace std;


BOOL CConfig::LoadUserDefinedVariables(const char *configFile)
{
    char        buff[CONFIG_SIZE_BUFF];
    const char    seps[] = " \t=";
    ifstream    fin;

    fin.open(configFile);

    if (!fin.is_open())
    {
        int nErr = GetLastError();
        SetLastError(0);
        char    szError[1024];
        char    szDir[512];
        GetCurrentDirectory(512, szDir);
        sprintf(szError, "[Error] CConfig::LoadUserDefinedVariables open() error, LastError[%d] ConfigFileName[%s] CurrentDirectory[%s]\r\n"
            , nErr, configFile, szDir);
        g_pGlobal->WriteSystemLog(szError);
        DBGOUT(szError);
        return FALSE;
    }

    while (true)
    {
        memset(buff, 0x00, CONFIG_SIZE_BUFF);
        
        auto bFlagFileEnd = fin.getline(buff, CONFIG_SIZE_BUFF).eof();
        if (bFlagFileEnd == TRUE && buff[0] == '\0') break;
        
        auto pLineEnd = strstr(buff, ";");
        if (pLineEnd) pLineEnd[0] = '\0';

        const char* token = std::strtok(buff, seps);

        // ignore blank lines
        if (token == nullptr) continue;

        // ignore comments
        if (token[0] == '#') continue;

        // user defined variable
        if (token[0] == '$' && token[1] == '$')
        {
            string key = token;

            char value[CONFIG_SIZE_BUFF] { '\0' };

            while (true)
            {
                token = this->strtok(nullptr, seps);

                if (!token) break;

                int nLen = strlen(value);
                if (nLen + strlen(token) > CONFIG_SIZE_BUFF - 1) break;
                if (nLen > 0) value[nLen++] = ' ';
                strcpy(value + nLen, token);
            }

            m_mapUserDefinedVariables[move(key)] = value;
        }
        else if (token[0] == '$')
        {
            string key = token;

            char value[CONFIG_SIZE_BUFF];

            token = this->strtok(nullptr, seps);

            if (token) util::strncpy(value, token, CONFIG_SIZE_BUFF);
            else value[0] = '\0';

            m_mapUserDefinedVariables[move(key)] = value;
        }
    }

    fin.close();

    return TRUE;
}

const char* CConfig::GetToken(const char* token)
{
    if (token == nullptr) return nullptr;

    auto itr = m_mapUserDefinedVariables.find(token);

    if (itr != m_mapUserDefinedVariables.end()) return itr->second.c_str();

    if (token[0] == '$' || token[0] == '#') return nullptr;

    return token;
}


char* CConfig::getQuotedString(char *strToken, const char* /*strDelimit*/)
{
    std::strtok(strToken, "\""); // why ???

    return std::strtok(strToken, "\"");
}

D3DXVECTOR3 CConfig::getPosition(char *strToken)
{
    D3DXVECTOR3 ret;

    const char *tmpToken;

    tmpToken = this->strtok(strToken, " \t(),");
    if (tmpToken == nullptr) return ret;
    ret.x = float(atoi(tmpToken));

    tmpToken = this->strtok(strToken, " \t(),");
    if (tmpToken == nullptr) return ret;
    ret.y = float(atoi(tmpToken));

    tmpToken = this->strtok(strToken, " \t(),");
    if (tmpToken == nullptr) return ret;
    ret.z = float(atoi(tmpToken));

    return ret;
}

void CConfig::GetTokenList(vectString *o_pTokenList, char *strToken, const char *strDelimit)
{
    o_pTokenList->clear();

    auto token = std::strtok(strToken, strDelimit);
    while (token)
    {
        o_pTokenList->push_back(token);
        
        token = std::strtok(nullptr, strDelimit);
    }
}
