// SetupConfig.cpp: implementation of the CSetupConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SetupConfig.h"


#define	SETUP_STRING_SEPS		"="
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetupConfig::CSetupConfig()
{
	m_setupInfo.clear();
	memset(m_strconfigFile, 0x00, MAX_PATH);

}

CSetupConfig::~CSetupConfig()
{
	ResetSetupInfo();	
}

void CSetupConfig::ResetSetupInfo()
{
	vector<SetupConfig*>::iterator it = m_setupInfo.begin();
	while(it != m_setupInfo.end())
	{
		SetupConfig* pSetup = (*it);
		if(NULL == pSetup)
		{	 
			it++;
			continue;
		}
		util::del_array(pSetup->chTitle);
		util::del_array(pSetup->chString);
		util::del(pSetup);
		it++;
	}
	m_setupInfo.clear();

}

BOOL CSetupConfig::LoadSetupInfo(const char *configFile)
{		
	char		buff[CONFIG_SIZE_BUFF];		

	strncpy(m_strconfigFile, configFile ,strlen(configFile)+1);
	

	FILE* fp = fopen( configFile, "rb" );
	if(NULL == fp)
	{
		//InitSetupInfo();
		return FALSE;
	}	

	while(TRUE)
	{
		memset(buff, 0x00, CONFIG_SIZE_BUFF);
		if(FALSE == GetLineString(fp, buff)) // 한줄 스트링을 얻어온다.
		{
			break;
		}
		if(feof(fp))
		{
			break;
		}
		char* pFind = strstr(buff, SETUP_STRING_SEPS);
		if(NULL == pFind)
		{
			// 못찾았으면 다음 글씨로
			continue;
		}
		int nPos = pFind - buff;
		int nBackPos = nPos +1;
		int nBackLen = strlen(buff) - nBackPos;

		SetupConfig* pSetupConfig = new SetupConfig;		
		pSetupConfig->chTitle = new char[CONFIG_SIZE_BUFF];
		pSetupConfig->chString = new char[CONFIG_SIZE_BUFF];

		memset(pSetupConfig->chTitle, 0x00, CONFIG_SIZE_BUFF);
		strncpy(pSetupConfig->chTitle, buff, nPos);		

		memset(pSetupConfig->chString, 0x00, CONFIG_SIZE_BUFF);
		if(nBackLen > 0)
		{
			strncpy(pSetupConfig->chString, &buff[nBackPos], nBackLen);	
		}
		
		m_setupInfo.push_back(pSetupConfig);
	}
	fclose(fp);

	return TRUE;
}

// 라인의 스트링을 얻어온다. 
BOOL CSetupConfig::GetLineString(FILE* i_fp,char* o_pDst)
{
	if(NULL == i_fp)
	{
		return FALSE;
	}
	char chBuffTmp[32];	
	int nPos = 0;
	BOOL bBreak = FALSE;
	
	while(FALSE == bBreak)
	{	
		if(feof(i_fp))
		{
			bBreak = TRUE;
			break;
		}
		if(nPos >= CONFIG_SIZE_BUFF)
		{
			bBreak = TRUE;
			break;
		}
		memset(chBuffTmp, 0x00, 32);
		if(0 == fread(chBuffTmp, sizeof(char), 1, i_fp))
		{
			bBreak = TRUE;
			break;
		}
		if(chBuffTmp[0] == '\t')
		{
			continue;
		}
		else if(chBuffTmp[0] == '\n')
		{
			o_pDst[nPos] = NULL;
			bBreak = TRUE;			
			return TRUE;			
		}
		else if(chBuffTmp[0] == '\r')
		{
			o_pDst[nPos++] = NULL;			
		}
		else
		{
			o_pDst[nPos++] = chBuffTmp[0];		
		}
		
	}

	return TRUE;
}

BOOL CSetupConfig::GetSetupInfo(char* i_pTitle, char* o_pTxt)
{
	vector<SetupConfig*>::iterator it = m_setupInfo.begin();
	while(it != m_setupInfo.end())
	{
		SetupConfig* pSetup = (*it);
		if(NULL == pSetup)
		{
			continue;
		}
		if(0 == strncmp(pSetup->chTitle, i_pTitle, strlen(i_pTitle)+1))
		{
			strncpy(o_pTxt, pSetup->chString, strlen(pSetup->chString)+1);
			return TRUE;
		}		
		it++;
	}
	return FALSE;
}
void CSetupConfig::SetSetupInfo(char* i_pTitle, char* i_pTxt)
{
	vector<SetupConfig*>::iterator it = m_setupInfo.begin();
	while(it != m_setupInfo.end())
	{
		SetupConfig* pSetup = (*it);
		if(NULL == pSetup)
		{
			continue;
		}
		if(0 == strncmp(pSetup->chTitle, i_pTitle, strlen(i_pTitle)+1))
		{
			strncpy(pSetup->chString, i_pTxt, strlen(i_pTxt)+1);
			return;
		}		
		it++;
	}
	// 없으면 추가한다. 
	AddSetupInfo(i_pTitle, i_pTxt);
}

void CSetupConfig::SaveSetupInfo(char* strconfigFile)
{
	FILE* fp = fopen( strconfigFile, "w+b" );	
	if(NULL == fp)
	{
		return;
	}
	char *pFullSetupInfo = new char[CONFIG_SIZE_BUFF*2];
	vector<SetupConfig*>::iterator it = m_setupInfo.begin();
	while(it != m_setupInfo.end())
	{
		SetupConfig* pSetup = (*it);
		if(NULL == pSetup)
		{
			continue;
		}

		wsprintf(pFullSetupInfo, "%s%s%s\r\n", pSetup->chTitle,SETUP_STRING_SEPS, pSetup->chString);
		//fwrite(pFullSetupInfo,sizeof(char),strlen(pFullSetupInfo)+1,fp);
		fwrite(pFullSetupInfo,sizeof(char),strlen(pFullSetupInfo),fp);
		it++;
	}

	fclose(fp);
	util::del_array(pFullSetupInfo);
}
void CSetupConfig::AddSetupInfo(char* i_pTitle, char* i_pTxt)
{
	SetupConfig* pSetupConfig = new SetupConfig;		
	pSetupConfig->chTitle = new char[CONFIG_SIZE_BUFF];
	pSetupConfig->chString = new char[CONFIG_SIZE_BUFF];

	memset(pSetupConfig->chTitle, 0x00, CONFIG_SIZE_BUFF);
	strncpy(pSetupConfig->chTitle, i_pTitle, strlen(i_pTitle)+1);		

	memset(pSetupConfig->chString, 0x00, CONFIG_SIZE_BUFF);
	strncpy(pSetupConfig->chString, i_pTxt, strlen(i_pTxt)+1);	
		
	m_setupInfo.push_back(pSetupConfig);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		조이스틱 정보 읽어옴
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
vector<SetupConfig*>	*CSetupConfig::GetSetupInfoIterator()
{
	return &m_setupInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		조이스틱 정보 읽어옴
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char* CSetupConfig::GetConfigFile()
{
	return m_strconfigFile;
}