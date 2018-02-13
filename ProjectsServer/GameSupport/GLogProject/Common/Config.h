#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <fstream>
#include <string>
#include <map>

using namespace std;

#define CONFIG_SIZE_BUFF	512

class CConfig
{
public:
	CConfig();

	BOOL LoadUserDefinedVariables(const char *configFile);
	char* getQuotedString(char *strToken, const char *strDelimit);
	static void GetTokenList(vectstring *o_pTokenList, char *strToken, const char *strDelimit);
	const char* GetToken(char* token);

private:
	map<string, string, greater<string> >	m_mapUserDefinedVariables;
	CLogManager m_LogManager;
};

#endif // __CONFIG_H__
