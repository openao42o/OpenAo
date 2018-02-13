#pragma once

#include <thread>
#include <map>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include "DefineGlobal.h"
#include "SocketHeader.h"
#include "AtumParam.h"

using namespace std;

typedef map<UID32_t, ATUM_DATE_TIME> AttackersMap;

class LastHitManager
{
	mutex _mutex;
	condition_variable _cv;
	thread *_thread;
	bool		_stop;

	map<UID32_t, AttackersMap> m_PlayerHitMap;
public:
	LastHitManager();
	~LastHitManager();
	int run();
	bool mustStop() const;
	AttackersMap getPlayerHitTime(UID32_t playerId);
	void setPlayerHitTime(UID32_t playerId, UID32_t attackerId);
	void onPlayerDeath(UID32_t playerId);
	void createThread()
	{
		if (!_thread) _thread = new thread{ [=]() { run(); } };
	}
};

