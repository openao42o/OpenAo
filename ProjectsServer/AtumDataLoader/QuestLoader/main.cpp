#include "QuestLoaderParser.h"
#include "Quest.h"

int
main(int argc, char** argv)
{
	ez_map<INT, CQuest*> QuestInfoMap;

	QuestLoaderParser parser("../../config/quest.cfg", &QuestInfoMap);
	parser.Parse();

	ez_map<INT, CQuest*>::iterator itr = QuestInfoMap.begin();

	while (QuestInfoMap.end() != itr)
	{
		CQuest *pQuest = itr->second;

		for (int i = 0; i < pQuest->StartPayItemVector.size(); i++)
		{
			ITEM_W_COUNT *pItm = &pQuest->StartPayItemVector[i];

			int a = 0;
		}

		itr++;
	}

	return 0;
}


