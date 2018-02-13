#include "LastHitManager.h"

LastHitManager::LastHitManager()
	: _mutex{}, _cv{}, _thread(nullptr), _stop(false)
{

}
#include <fstream>
int LastHitManager::run()
{
	unique_lock<mutex> lock{ _mutex };
	while (!mustStop())
	{
		map<UID32_t, AttackersMap>::iterator it;
		AttackersMap::iterator attackerIt;
		ATUM_DATE_TIME currentDate{ true };
		currentDate.AddDateTime(0, 0, 0, 0, 0, -PVP_AWARD_ASSIST_TIME);
		for (it = m_PlayerHitMap.begin(); it != m_PlayerHitMap.end(); it++) {
			for (attackerIt = it->second.begin(); attackerIt != it->second.end(); attackerIt++) {
				if (attackerIt->second <= currentDate) {
					if (it->second.erase(attackerIt) == it->second.end())
						m_PlayerHitMap.erase(it);
				}
			}
		}
		_cv.wait_for(lock, chrono::seconds(10));
	}
	lock.unlock();
	return 0;
}

bool LastHitManager::mustStop() const
{
	return _stop;
}

AttackersMap LastHitManager::getPlayerHitTime(UID32_t playerId)
{
	if (!mustStop()) {
		lock_guard<mutex> { _mutex };
		map<UID32_t, AttackersMap>::iterator it;
		if ((it = m_PlayerHitMap.find(playerId)) != m_PlayerHitMap.end()) {
			return it->second;
		}
		else {
			AttackersMap emptyMap;
			return emptyMap;
		}
	}
}

void LastHitManager::setPlayerHitTime(UID32_t playerId, UID32_t attackerId)
{
	if (!mustStop()) {
		lock_guard<mutex> { _mutex };
		map<UID32_t, AttackersMap>::iterator it;
		AttackersMap::iterator attackerIt;
		ATUM_DATE_TIME hitDate{ true };

		if ((it = m_PlayerHitMap.find(playerId)) != m_PlayerHitMap.end()) {
			if ((attackerIt = it->second.find(attackerId)) != it->second.end()) {
				attackerIt->second = hitDate;
			}
			else {
				it->second.insert(pair<UID32_t, ATUM_DATE_TIME>(attackerId, hitDate));
			}
		}
		else {
			AttackersMap newAttacker;
			newAttacker.insert(pair<UID32_t, ATUM_DATE_TIME>(attackerId, hitDate));
			m_PlayerHitMap.insert(pair<UID32_t, AttackersMap>(playerId, newAttacker));
		}
	}
}

void LastHitManager::onPlayerDeath(UID32_t playerId)
{
	if (!mustStop()) {
		lock_guard<mutex> { _mutex };
		map<UID32_t, AttackersMap>::iterator it;
		if ((it = m_PlayerHitMap.find(playerId)) != m_PlayerHitMap.end()) {
			m_PlayerHitMap.erase(it);
		}
	}
}

LastHitManager::~LastHitManager()
{
	_stop = true;
	if (_thread && _thread->joinable()) _thread->join();
	delete _thread;
	_stop = false;
	_thread = nullptr;
}
