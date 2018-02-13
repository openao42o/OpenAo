#pragma once

#include <mutex>
#include <condition_variable>

#include "AtumThread.h"

struct structLoadingGameInfo;

class CMeshInitThread : public CAtumThread 
{
	mutex _mutex;
	condition_variable _cv;

	queue<structLoadingGameInfo*> m_queLoadingGameInfo;

public:
	CMeshInitThread() : CAtumThread { }, _mutex { }, _cv { } { }
	~CMeshInitThread(); // virtual
	DWORD Run() override;

	void QuePushGameData(structLoadingGameInfo* GameInfo);

	static void CreateGameData(structLoadingGameInfo* GameInfo);
};
