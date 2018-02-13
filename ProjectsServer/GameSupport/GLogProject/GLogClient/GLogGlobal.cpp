#include "StdAfx.h"
#include "GLogGlobal.h"

GLogGlobal * g_pLogGlobal = NULL;

GLogGlobal::GLogGlobal(void)
{
	if(g_pLogGlobal)
	{
		return;
	}

	g_pLogGlobal = this;

	SetConfigRootPath();		// 2011-12-12 by hskim, GLog 2차

	// 2010-05-17 by dhjin - 로그파일 작성을 위해
	m_pGlobalLogManager = new CLogManager;
	this->InitLogManager();
}

GLogGlobal::~GLogGlobal(void)
{
	util::del(m_pGlobalLogManager);
}

void GLogGlobal::InitLogManager()
{
	CString szDir;		// 2011-12-12 by hskim, GLog 2차	

	szDir.Format(L"%s%s", g_pLogGlobal->m_strConfigRoot, L"../Log/SystemLog/");			// 2011-12-12 by hskim, GLog 2차	
	m_pGlobalLogManager->InitLogManger(TRUE, L"GLog", szDir);								// 2011-12-12 by hskim, GLog 2차
}

// start 2011-12-12 by hskim, GLog 2차
void GLogGlobal::SetConfigRootPath()
{
	// Default Path를 미리 설정해둠
	
	const int SIZE_BUFF = 1024;
	char		CONFIG_ROOT[SIZE_BUFF] = { 0, };
	char		buff[SIZE_BUFF] = { 0, };
	const char	*token;
	char		seps[] = "\r\n";
	ifstream	fin;
	char *next_token1 = NULL;

	fin.open(FILE_PATH_CONFIG_ROOT_UNI);
	if (!fin.is_open())
	{
		return ;		// cannot open file...
	}

	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok_s(buff, seps, &next_token1);

		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}

		strncpy_s(CONFIG_ROOT, token, SIZE_BUFF);
	}

	if('\\' != CONFIG_ROOT[strlen(CONFIG_ROOT)-1]
		&& '/' != CONFIG_ROOT[strlen(CONFIG_ROOT)-1])
	{
		strcat_s(CONFIG_ROOT, "/");
	}

	m_strConfigRoot = CA2W(CONFIG_ROOT);
}
// end 2011-12-12 by hskim, GLog 2차