#ifndef __NPC_SCRIPTS_H__
#define __NPC_SCRIPTS_H__

#include "AtumParam.h"
#include "mt_stl.h"

struct NPCTalk
{
	int					TalkID;
	char*				TalkString;		// allocated by strdup(), freed by free()
	vector<char*>		MenuVector;		// allocated by strdup(), freed by free()

	~NPCTalk();
	void reset();
};

struct NPCScript
{
	NPCID_t				NpcID;
	vector<NPCTalk*>	NpcTalkVector;	// NpcScripts 파일에서 talk, menu 번호는
										// 꼭 순서대로 입력되어야 함!
	~NPCScript();
	void reset();
};

void CleanNpcScripts(ez_map<NPCID_t, NPCScript> &mapNpcScripts);

#endif // __NPC_SCRIPTS_H__
