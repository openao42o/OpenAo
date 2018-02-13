#include "QuestLoaderParser.h"
#include "Quest.h"
#include <assert.h>
#include <fstream>					// 2007-04-09 by cmkwon

char g_szQuestConfigRootPath[1024];		// 2007-04-09 by cmkwon

BOOL SetQuestConfigRootPath(void);			// 2007-04-09 by cmkwon

int main(int argc, char** argv)
{
	ez_map<INT, CQuest*> QuestInfoMap;

//	BOOL bRet = QuestLoaderParser::ParseFile("../../config/quest/quest.cfg", &QuestInfoMap);
//	FILE *tmpFile = fopen("tmp", "w+");
//	FILE *pDefFile = fopen("../../config/quest/q_def.cfg", "r");
//	FILE *pQFile = fopen("../../config/quest/q_040624.cfg", "r");
//	QuestLoaderParser::ConcatenateFile(tmpFile, pDefFile);
//	QuestLoaderParser::ConcatenateFile(tmpFile, pQFile);
//	fclose(tmpFile);
//	vector<string> vecFiles;
//	vecFiles.push_back("../../config/quest/q_040624.cfg");
//	BOOL bRet = QuestLoaderParser::ParseFiles("../../config/quest/quest_def.cfg", &vecFiles, &QuestInfoMap);

//*

	if(FALSE == SetQuestConfigRootPath())
	{
		printf("[ERROR]: Set ConfigRootPath error !!\r\n");
		system("pause");
		return 0;
	}


	char szFileFullPath[MAX_PATH];
	sprintf(szFileFullPath, "%sq_000000.cfg", g_szQuestConfigRootPath);

	int nDefBufSize;
	char *DefBuf = QuestLoaderParser::CreateNewBufferAndCopyFile(szFileFullPath, nDefBufSize);
	if(NULL == DefBuf)
	{
		printf("[ERROR]: QuestFile(%s) not found !!\r\n", szFileFullPath);
		system("pause");
		return 0;
	}
	nDefBufSize = strlen(DefBuf);

	char *pScriptbuf = NULL;
	for(int i=1; i<10; i++)
	{
		int nBufferSize;
		sprintf(szFileFullPath, "%sq_%06d.cfg", g_szQuestConfigRootPath, i);
		char *buffer = QuestLoaderParser::CreateNewBufferAndCopyFile(szFileFullPath, nBufferSize);
		if(buffer)
		{
			if(NULL == pScriptbuf)
			{
				pScriptbuf = new char[strlen(buffer) +1];
				strcpy(pScriptbuf, buffer);
			}
			else
			{
				char *pTemp = new char[strlen(pScriptbuf) + strlen(buffer) +1];
				strcpy(pTemp, pScriptbuf);
				strcat(pTemp, buffer);
				util::del(pScriptbuf);
				pScriptbuf	= pTemp;
			}
		}
		util::del(buffer);
	}

	BOOL bRet = QuestLoaderParser::ParseString(DefBuf, strlen(DefBuf), pScriptbuf, strlen(pScriptbuf), &QuestInfoMap);
	util::del(DefBuf);
	util::del(pScriptbuf);
//	util::del(qBuf);
//*/
	printf("---------------------------------------------------\r\n");
	if (!bRet)
	{
		printf(" 퀘스트 파싱에 실패하였습니다.\r\n");
		system("pause");
		return 0;
	}

	ez_map<INT, CQuest*>::iterator itr = QuestInfoMap.begin();

	while (QuestInfoMap.end() != itr)
	{
		CQuest *pQuest = itr->second;

		printf("퀘스트 \'%20s\' 번호 %d -> OK\r\n", pQuest->QuestName, pQuest->QuestIndex);
		int i;
		for (i = 0; i < pQuest->ReqRecvQuestIndexVector.size(); i++)
		{
			int a = pQuest->ReqRecvQuestIndexVector[i];
		}

		for (i = 0; i < pQuest->StartPayItemVector.size(); i++)
		{
			QUEST_PAY_ITEM *pItm = &pQuest->StartPayItemVector[i];

			int a = 0;
		}

		for (i = 0; i < pQuest->EndPayItemVector.size(); i++)
		{
			QUEST_PAY_ITEM *pItm = &pQuest->EndPayItemVector[i];

			int a = 0;
		}
		
		//////////////////////////////////////////////////////////////////////////
		// 2008-12-02 by dhjin, 미션마스터
		for (i = 0; i < pQuest->EndMissionMasterPayItemVector.size(); i++)
		{
			QUEST_PAY_ITEM *pItm = &pQuest->EndMissionMasterPayItemVector[i];

		}

		for (i = 0; i < pQuest->vecQuestDropItem.size(); i++)
		{
			QUEST_DROP_ITEM *pDropItem = &pQuest->vecQuestDropItem[i];

			int a = 0;
		}

		for (i = 0; i < pQuest->EndRandomPayItemVector.size(); i++)
		{
			QUEST_RANDOM_PAY_ITEM *pRandomPayItem = &pQuest->EndRandomPayItemVector[i];

			int a = 0;
		}

//		for ();

		itr++;
	}

	itr = QuestInfoMap.begin();
	while (itr != QuestInfoMap.end())
	{
		delete itr->second;
		itr++;
	}

	printf("\r\n퀘스트 파싱에 성공하였습니다.\r\n");
	printf("---------------------------------------------------\r\n");

	printf("엔터 키를 눌러주세요.\r\n");
	system("pause");

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void SetConfigRootPath(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-04-09 ~ 2007-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL SetQuestConfigRootPath(void)
{
		// Default Path를 미리 설정해둠
	util::strncpy(g_szQuestConfigRootPath, "./", 128);

	const int SIZE_BUFF = 1024;
	char		buff[SIZE_BUFF];
	const char	*token;
	char		seps[] = "\r\n";
	ifstream	fin;

	fin.open("./QuestConfig_root.cfg");
	if (!fin.is_open())
	{	// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		char	szDir[1024];
		GetCurrentDirectory(1024, szDir);
		sprintf(szError, "\r\n[WARNING] Cannot find 'QuestConfig_root.cfg': set default path '%s', Current Directory'%s'\r\n\r\n", g_szQuestConfigRootPath, szDir);
		printf("%s", szError);
		return FALSE;
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

		token = strtok(buff, seps);

		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}

		util::strncpy(g_szQuestConfigRootPath, token, 1024);
	}

	if('\\' != g_szQuestConfigRootPath[strlen(g_szQuestConfigRootPath)-1]
		&& '/' != g_szQuestConfigRootPath[strlen(g_szQuestConfigRootPath)-1])
	{
		strcat(g_szQuestConfigRootPath, "/");
	}

	printf("\r\nSET CONFIG ROOT PATH '%s'\r\n\r\n", g_szQuestConfigRootPath);
	return TRUE;
}
