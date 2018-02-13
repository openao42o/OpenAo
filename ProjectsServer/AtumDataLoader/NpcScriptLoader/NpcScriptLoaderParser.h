#ifndef __NpcScriptLoader_PARSER__
#define __NpcScriptLoader_PARSER__

#include "NPCScripts.h"
#include "NpcScriptLoader.cpp.h"
int NpcScriptLoaderparse();

class NpcScriptLoaderParser
{
public:
	NpcScriptLoaderParser();
	NpcScriptLoaderParser(char* fname, ez_map<NPCID_t, NPCScript>* i_pNpcScriptsMap);
	int Parse();


};

#endif
