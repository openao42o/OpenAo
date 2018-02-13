#include "NpcScriptLoaderParser.h"
#include "mt_stl.h"
#include "AtumParam.h"
#include "NPCScripts.h"

int
main(int argc, char** argv)
{
	ez_map<NPCID_t, NPCScript>	*pNpcScriptsMap = new ez_map<NPCID_t, NPCScript>;
	NpcScriptLoaderParser parser("../../config/npcscripts.cfg", pNpcScriptsMap);
	parser.Parse();

	// 디버그용 메세지
	for (ez_map<NPCID_t, NPCScript>::iterator itr = pNpcScriptsMap->begin(); itr != pNpcScriptsMap->end(); itr++)
	{
		NPCID_t rTmp = itr->first;
		//NPCScript &rTmp = itr->second;
		printf("NPC ID: %d\n", itr->second.NpcID);
		for (int j = 0; j < itr->second.NpcTalkVector.size(); j++)
		{
			printf("  talk %d: %s\n" , j, itr->second.NpcTalkVector[j]->TalkString);
			for (int k = 0; k < itr->second.NpcTalkVector[j]->MenuVector.size(); k++)
			{
				printf("    menu %d: %s\n" , k, itr->second.NpcTalkVector[j]->MenuVector[k]);
			}
		}
		printf("\n");
	}

	delete pNpcScriptsMap;

	return 0;
}


