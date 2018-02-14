#ifndef __QuestLoader_PARSER__
#define __QuestLoader_PARSER__

#include "mt_stl.h"

int QuestLoaderparse();

class CQuest;
typedef vector<CQuest*>		vectCQuestPtr;		// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 

class QuestLoaderParser
{
public:
	// load from file(s)
	// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 인자추가(, vectCQuestPtr *o_pvectCQuestInfoList=NULL)
	static BOOL ParseFile(const char *i_szFileName, ez_map<INT, CQuest*> *i_pQuestInfoMap, vectCQuestPtr *o_pvectCQuestInfoList=NULL);
	// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 인자추가(, vectCQuestPtr *o_pvectCQuestInfoList=NULL)
	static BOOL ParseFiles(const char *i_szUserDefFileName, vector<string> *i_szFileNames, ez_map<INT, CQuest*> *i_pQuestInfoMap, vectCQuestPtr *o_pvectCQuestInfoList=NULL);
	
	// load from memory
	// 2007-07-09 by cmkwon, 퀘스트 스크립트를 정보를 vector로 받기 위해 - 인자추가(, vectCQuestPtr *o_pvectCQuestInfoList=NULL)
	static BOOL ParseString(const char *i_szUserDefinedVarString, int i_nVarLen, const char *i_szQuestString, int i_nQuestLen, ez_map<INT, CQuest*> *i_pQuestInfoMap, vectCQuestPtr *o_pvectCQuestInfoList=NULL);

	// helper functions
	static BOOL ConcatenateFile(FILE *o_pOutFile, FILE *i_pInputFile);
	static char* CreateNewBufferAndCopyFile(const char *i_szFileName, int &o_nBufferSize);
};

#endif
