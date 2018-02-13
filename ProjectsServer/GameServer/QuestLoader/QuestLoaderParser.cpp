#include "QuestLoaderParser.h"
#include <stdio.h>
extern FILE *QuestLoaderin;

#include "Quest.h"
#include "QuestLoader.cpp.h"

#pragma warning(disable:4786)

extern QUEST_NPC_INFO g_quest_npc_info_for_loading;
extern ez_map<INT, CQuest*> *g_pQuestInfoMap;
extern vectCQuestPtr		*g_pvectQuestInfoList;		// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해

// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 인자추가(, vectCQuestPtr *o_pvectCQuestInfoList=NULL)
BOOL QuestLoaderParser::ParseFile(const char *i_szFileName, ez_map<INT, CQuest*> *i_pQuestInfoMap, vectCQuestPtr *o_pvectCQuestInfoList/*=NULL*/)
{
	QuestLoaderin = fopen(i_szFileName, "r");

	if (QuestLoaderin == NULL)
	{
		printf("Cannot open file");
		return FALSE;
	}

	g_pQuestInfoMap = i_pQuestInfoMap;
	g_pvectQuestInfoList	= o_pvectCQuestInfoList;	// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 

	g_quest_npc_info_for_loading.Reset();
	return (QuestLoaderparse() == 0);
}

// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 인자추가(, vectCQuestPtr *o_pvectCQuestInfoList=NULL)
BOOL QuestLoaderParser::ParseFiles(const char *i_szUserDefFileName, vector<string> *i_szFileNames, ez_map<INT, CQuest*> *i_pQuestInfoMap, vectCQuestPtr *o_pvectCQuestInfoList/*=NULL*/)
{
	// create temporary file
	FILE *pFile = tmpfile();
	if(NULL == pFile)
	{// 2008-01-29 by cmkwon, S_F, C_Exe3: OS 관리자 권한 계정으로 실행하지 않으면 생성 실패 - 에러 체크 추가
		return FALSE;
	}

	FILE *pDefFile = fopen(i_szUserDefFileName, "r");
	if (pDefFile == NULL)
	{
		return FALSE;
	}
	ConcatenateFile(pFile, pDefFile);
	fclose(pDefFile);

	int nTotalWritten = 0;
	int nTotalRead = 0;
	for (int j = 0; j < i_szFileNames->size(); j++)
	{
		FILE *pInputFile = fopen((*i_szFileNames)[j].c_str(), "r");
		if (pInputFile == NULL)
		{
			// delete temporary file
			_rmtmp();
			return FALSE;
		}

		ConcatenateFile(pFile, pInputFile);
		fclose(pInputFile);
	}

	rewind(pFile);

	QuestLoaderin = pFile;

	if (QuestLoaderin == NULL)
	{
		// delete temporary file
		_rmtmp();
		return FALSE;
	}

	g_pQuestInfoMap = i_pQuestInfoMap;
	g_pvectQuestInfoList	= o_pvectCQuestInfoList;	// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 


	g_quest_npc_info_for_loading.Reset();
	BOOL bRet = (QuestLoaderparse() == 0);

	// delete temporary file
	_rmtmp();

	return bRet;
}

// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 인자추가(, vectCQuestPtr *o_pvectCQuestInfoList=NULL)
BOOL QuestLoaderParser::ParseString(const char *i_szUserDefinedVarString, int i_nVarLen, const char *i_szQuestString, int i_nQuestLen, ez_map<INT, CQuest*> *i_pQuestInfoMap, vectCQuestPtr *o_pvectCQuestInfoList/*=NULL*/)
{
	// write file
	FILE *pFile = tmpfile();
	if(NULL == pFile)
	{// 2008-01-29 by cmkwon, S_F, C_Exe3: OS 관리자 권한 계정으로 실행하지 않으면 생성 실패 - 에러 체크 추가
		return FALSE;
	}

	int nTotalWritten = 0;
	int i = 0;

	// user defined variables
	for (i = 0; i < i_nVarLen; i++)
	{
		if (*(i_szUserDefinedVarString+i) != 0x0D)
		{
			int nWritten = fwrite((i_szUserDefinedVarString+i), 1, 1, pFile);
			if (nWritten != 1)
			{
				return FALSE;
			}
			nTotalWritten += nWritten;
		}
		else
		{
			continue;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-04-09 by cmkwon, 마지막줄에 LF(Line feed)가 추가되어 있는지 체크
	if(0x0A != *(i_szUserDefinedVarString+i_nVarLen-1))
	{
		return FALSE;
	}

	// quests
	for (i = 0; i < i_nQuestLen; i++)
	{
		if (*(i_szQuestString+i) != 0x0D)
		{
			int nWritten = fwrite((i_szQuestString+i), 1, 1, pFile);
			if (nWritten != 1)
			{
				return FALSE;
			}
			nTotalWritten += nWritten;
		}
		else
		{
			continue;
		}
	}

	char eofChar = EOF;
	fwrite(&eofChar, 1, 1, pFile);

	fseek(pFile, 0, SEEK_END);
	int lSize = ftell(pFile);
	rewind(pFile);

	QuestLoaderin = pFile;

	if (QuestLoaderin == NULL)
	{
		return FALSE;
	}

	g_pQuestInfoMap = i_pQuestInfoMap;
	g_pvectQuestInfoList	= o_pvectCQuestInfoList;	// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 

	g_quest_npc_info_for_loading.Reset();
	BOOL bRet = (QuestLoaderparse() == 0);

	_rmtmp();

	return bRet;
}

BOOL QuestLoaderParser::ConcatenateFile(FILE *o_pOutFile, FILE *i_pInputFile)
{
	char charBuf;
	int nTotalRead = 0;
	int nTotalWritten = 0;

	fseek(o_pOutFile, 0, SEEK_END);
	rewind(i_pInputFile);

	while (!feof(i_pInputFile))
	{
		nTotalRead += fread(&charBuf, 1, 1, i_pInputFile);

		int nWritten = fwrite(&charBuf, 1, 1, o_pOutFile);
		if (nWritten != 1)
		{
			assert(0);
			return FALSE;
		}

		nTotalWritten += nWritten;
	}

	rewind(o_pOutFile);
	rewind(i_pInputFile);

	return TRUE;
}

char* QuestLoaderParser::CreateNewBufferAndCopyFile(const char *i_szFileName, int &o_nBufferSize)
{
	long lSize;
	char *buffer;
	FILE *pTmpFile = fopen(i_szFileName, "r");
	if (pTmpFile == NULL)
	{
		return NULL;
	}

	// obtain file size.
	fseek(pTmpFile, 0, SEEK_END);
	lSize = ftell(pTmpFile);
	rewind(pTmpFile);

	// allocate memory to contain the whole file.
	buffer = new char[lSize+1];
	if (buffer == NULL)
	{
		return NULL;
	}

	// copy the file into the buffer.
	int nTotalRead = 0;
	while (!feof(pTmpFile))
	{
		nTotalRead += fread(buffer+nTotalRead, 1, 1, pTmpFile);
	}
	buffer[nTotalRead] = '\0';

	// close file
	fclose(pTmpFile);

	o_nBufferSize = lSize;

	return buffer;
}