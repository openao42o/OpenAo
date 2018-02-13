#include "QuestLoaderParser.h"
#include <stdio.h>
extern FILE *QuestLoaderin;

#include "Quest.h"
#include "QuestLoader.cpp.h"

extern ez_map<INT, CQuest*> *g_pQuestInfoMap;

QuestLoaderParser::QuestLoaderParser()
{
}

QuestLoaderParser::QuestLoaderParser(char* fname, ez_map<INT, CQuest*> *i_pQuestInfoMap)
{
	QuestLoaderin = fopen(fname, "r");

	if (QuestLoaderin == NULL)
	{
		printf("Cannot open file");
	}

	g_pQuestInfoMap = i_pQuestInfoMap;
}

int QuestLoaderParser::Parse()
{
	return (QuestLoaderparse() == 0);
}
