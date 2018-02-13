#include "NpcScriptLoaderParser.h"
#include <stdio.h>
extern FILE *NpcScriptLoaderin;
extern ez_map<NPCID_t, NPCScript> *g_NpcScriptsMap;

NpcScriptLoaderParser::
NpcScriptLoaderParser()
{
}

NpcScriptLoaderParser::
NpcScriptLoaderParser(char* fname, ez_map<NPCID_t, NPCScript>* i_pNpcScriptsMap)
{
	NpcScriptLoaderin = fopen(fname, "r");
	g_NpcScriptsMap = i_pNpcScriptsMap;
}

int
NpcScriptLoaderParser::
Parse()
{
	return NpcScriptLoaderparse();
}
