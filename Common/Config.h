#pragma once

#include <map>

constexpr auto CONFIG_SIZE_BUFF = 1024;

class CConfig
{
public:
	BOOL LoadUserDefinedVariables(const char *configFile);
	static char* getQuotedString(char *strToken, const char *strDelimit);
	D3DXVECTOR3 getPosition(char *strToken);
 
	static void GetTokenList(vectString *o_pTokenList, char *strToken, const char *strDelimit);

	const char* strtok(char *strToken, const char *strDelimit) { return GetToken(std::strtok(strToken, strDelimit)); }
private:
	const char* GetToken(const char* token);
	std::map<std::string, std::string> m_mapUserDefinedVariables;
};
