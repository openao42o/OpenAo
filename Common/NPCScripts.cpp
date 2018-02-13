#ifdef _ATUM_SERVER
#include "stdafx.h"
#else // _ATUM_SERVER
#include "DefineGlobal.h"
#endif // _ATUM_SERVER

#include "NPCScripts.h"
#include "AtumParam.h"
#include <iostream>

#ifdef _ATUM_SERVER
#include "dbgout_field.h"
#else // _ATUM_SERVER
#undef DBGOUT
#define DBGOUT (void(0))
#endif // _ATUM_SERVER

using namespace std;

NPCTalk::~NPCTalk()
{
}

void NPCTalk::reset()
{
	TalkID = 0;
	TalkString = NULL;
	MenuVector.clear();
}

NPCScript::~NPCScript()
{
}

void NPCScript::reset()
{
	NpcID = 0;
	NpcTalkVector.clear();
}

void NpcScriptParseError(const char* errString)
{
	DBGOUT("NpcScript Parse ERROR: %s\n", errString);
}

void CleanNpcScripts(ez_map<NPCID_t, NPCScript> &mapNpcScripts)
{
	for (ez_map<NPCID_t, NPCScript>::iterator itr = mapNpcScripts.begin(); itr != mapNpcScripts.end(); itr++)
	{
		for (int j = 0; j < itr->second.NpcTalkVector.size(); j++)
		{
			util::del_array(itr->second.NpcTalkVector[j]->TalkString);
			for (int k = 0; k < itr->second.NpcTalkVector[j]->MenuVector.size(); k++)
			{
				util::del_array(itr->second.NpcTalkVector[j]->MenuVector[k]);
			}
			util::del(itr->second.NpcTalkVector[j]);
		}
	}
}
