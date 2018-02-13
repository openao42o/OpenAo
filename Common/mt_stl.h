/******************************************************************************

	For MT-SAFE and/or Easy Use of STL Classes

******************************************************************************/

#pragma once

#pragma warning(push)
#pragma warning(disable:4786)

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <map>
#include <set>
#include <list>
#include <vector>
#include <queue>

namespace mt_stl
{
	using namespace std;

	struct mt_lock
	{
		CRITICAL_SECTION m_lock;

		mt_lock()
		{
			InitializeCriticalSection(&m_lock);
		}

		~mt_lock()
		{
			// If the lock is already held in another place, wait for the process to complete
			EnterCriticalSection(&m_lock);
			LeaveCriticalSection(&m_lock);
			DeleteCriticalSection(&m_lock);
		}

		void lock()
		{
			EnterCriticalSection(&m_lock);
		}

		bool tryLock()
		{
			return TryEnterCriticalSection(&m_lock);
		}

		void unlock()
		{
			LeaveCriticalSection(&m_lock);
		}
	};

	struct mt_auto_lock
	{
		mt_lock* m_pMTLock;

		explicit mt_auto_lock(mt_lock* i_pMTLock)
		{
			m_pMTLock = i_pMTLock;

#ifdef _ATUM_SERVER
			if (!i_pMTLock) return; // 2013-03-13 by hskim, 웹 캐시 상점
#endif

			m_pMTLock->lock();
		}

		explicit mt_auto_lock(mt_lock& i_pMTLock)
		{
			m_pMTLock = &i_pMTLock;
			m_pMTLock->lock();
		}

		~mt_auto_lock()
		{
			if (m_pMTLock != nullptr) m_pMTLock->unlock();
		}

		void auto_unlock_cancel()
		{
			if (m_pMTLock != nullptr) m_pMTLock->unlock();

			m_pMTLock = nullptr;
		}
	};


	template <class _Ty, class _A = allocator<_Ty>>
	class mt_list : public list<_Ty, _A>, public mt_lock
	{
	public:
		_Ty findLock(const _Ty& key)
		{
			lock();

			for (auto itr = begin(); itr != end(); ++itr)

				if (*itr == key)
				{
					unlock();
					return *itr;
				}

			unlock();

			return nullptr;
		}

		int deleteLock(const _Ty& key)
		{
			int nDeleted = 0;
			lock();
			auto itr = begin();
			while (itr != end())
			{
				if (key == *itr)
				{
					itr = erase(itr);
					nDeleted++;
					continue;
				}
				itr++;
			}
			unlock();
			return nDeleted;
		}

		void pushBackLock(const _Ty& x)
		{
			lock();
			push_back(x);
			unlock();
		}

		void clearLock()
		{
			lock();
			clear();
			unlock();
		}

		_Ty getByIndexLock(int idx)
		{
			int tmp = 0;
			lock();
			auto itr = begin();
			while (itr != end())
			{
				if (tmp == idx)
				{
					unlock();
					return *itr;
				}
				itr++;
				tmp++;
			}
			unlock();
			return NULL;
		}

		_Ty getByIndexNoLock(int idx)
		{
			int tmp = 0;
			auto itr = begin();
			while (itr != end())
			{
				if (tmp == idx) return *itr;
				itr++;
				tmp++;
			}
			return NULL;
		}
	};
	
	template <class _Ty, class _A = allocator<_Ty>>
	class ez_list : public list<_Ty, _A>
	{
	public:
		_Ty findEZ(const _Ty& key)
		{
			auto itr = begin();

			while (itr != end())
			{
				if (key == *itr) return *itr;

				itr++;
			}

			return NULL;
		}

		int deleteEZ(const _Ty& key)
		{
			int nDeleted = 0;
			auto itr = begin();
			while (itr != end())
			{
				if (key == *itr)
				{
					itr = erase(itr);
					nDeleted++;
					continue;
				}
				itr++;
			}
			return nDeleted;
		}

		_Ty getByIndexEZ(int idx)
		{
			int tmp = 0;
			auto itr = begin();
			while (itr != end())
			{
				if (tmp == idx) return *itr;
				itr++;
				tmp++;
			}
			return NULL;
		}
	};


	template <class _K, class _Pr = less<_K>, class _A = allocator<_K>>
	class mt_set : public set<_K, _Pr, _A>, public mt_lock
	{
	public:
		// Functions with Lock
		void clearLock()
		{
			lock();
			clear();
			unlock();
		}

		bool insertLock(const _K& key)
		{
			lock();
			auto res = insert(key);
			unlock();

			return res.second;
		}

		bool deleteLock(const _K& key)
		{
			lock();
			auto nDeleted = erase(key);
			unlock();

			return nDeleted != 0;
		}

		bool deleteLock(typename set<_K, _Pr, _A>::iterator it)
		{
			lock();
			auto itRet = erase(it);
			auto bRet = itRet != end();
			unlock();

			return bRet;
		}

		_K findLock(const _K& key)
		{
			_K retTy = NULL;

			lock();
			auto itr = find(key);
			if (itr != end()) retTy = *itr;
			unlock();

			return retTy;
		}

		_K* findLock_Ptr(const _K& key)
		{
			_K* pRetTy = nullptr;

			lock();
			auto itr = find(key);
			if (itr != end()) pRetTy = &(*itr);
			unlock();

			return pRetTy;
		}

		// Functions without Lock
		bool insertNoLock(const _K& key)
		{
			return insert(key).second;
		}

		bool deleteNoLock(const _K& key)
		{
			return erase(key) != 0;
		}

		_K findNoLock(const _K& key)
		{
			auto itr = find(key);

			if (itr == end()) return NULL;

			return *itr;
		}
	};


	template <class _K, class _Ty, class _Pr = less<_K>, class _A = allocator<_Ty>>
	class mt_map : public map<_K, _Ty, _Pr, _A>, public mt_lock
	{
	public:
		// Functions with Lock
		void clearLock()
		{
			lock();
			clear();
			unlock();
		}

		bool insertLock(const _K& key, const _Ty& value)
		{
			lock();
			auto res = insert(pair<_K, _Ty>(key, value));
			unlock();

			return res.second;
		}

		bool deleteLock(const _K& key)
		{
			lock();
			auto nDeleted = erase(key);
			unlock();

			return nDeleted != 0;
		}

		bool deleteLock(const _K& key, _Ty& value)
		{
			lock();
			auto itr = find(key);
			if (itr == end())
			{
				unlock();
				return false;
			}

			value = itr->second;
			erase(itr);
			unlock();

			return true;
		}

		bool deleteLock(typename map<_K, _Ty, _Pr, _A>::iterator it)
		{
			bool bRet = false;

			lock();
			auto itRet = erase(it);
			if (itRet != end()) bRet = true;
			unlock();

			return bRet;
		}

		_Ty findLock(const _K& key)
		{
			_Ty retTy = NULL;

			lock();
			auto itr = find(key);
			if (itr != end()) retTy = itr->second;
			unlock();

			return retTy;
		}

		_Ty popLock(const _K& key)
		{
			_Ty ty = NULL;

			lock();
			auto itr = find(key);
			if (itr != end())
			{
				ty = itr->second;
				erase(itr);
			}
			unlock();
			return ty;
		}

		_Ty* findLock_Ptr(const _K& key)
		{
			_Ty* pRetTy = nullptr;

			lock();
			auto itr = find(key);
			if (itr != end()) pRetTy = &itr->second;
			unlock();

			return pRetTy;
		}

		_Ty* findNoLock_Ptr(const _K& key)
		{
			_Ty* pRetTy = nullptr;

			auto itr = find(key);
			if (itr != end()) pRetTy = &itr->second;

			return pRetTy;
		}

		// Functions without Lock
		bool insertNoLock(const _K& key, const _Ty& value)
		{
			auto res = insert(pair<_K, _Ty>(key, value));

			return res.second;
		}

		bool deleteNoLock(const _K& key)
		{
			return erase(key) != 0;
		}

		bool deleteNoLock(const _K& key, _Ty& value)
		{
			auto itr = find(key);
			if (itr == end()) return false;

			value = itr->second;
			erase(itr);

			return true;
		}

		_Ty findNoLock(const _K& key)
		{
			auto itr = find(key);

			if (itr == end()) return NULL;

			return itr->second;
		}
	};

	template <class _K, class _Ty, class _Pr = less<_K>, class _A = allocator<_Ty>>
	class ez_map : public map<_K, _Ty, _Pr, _A>
	{
	public:
		bool insertEZ(const _K& key, const _Ty& value)
		{
			return insert(pair<_K, _Ty>(key, value)).second;
		}

		bool deleteEZ(const _K& key)
		{
			return erase(key) != 0;
		}

		bool deleteEZ(const _K& key, _Ty& value)
		{
			auto itr = find(key);
			if (itr == end()) false;

			value = itr->second;
			erase(itr);

			return true;
		}

		_Ty findEZ(const _K& key)
		{
			auto itr = find(key);

			if (itr == end()) return { };
			//{
			//	_Ty Temp;
			//	memset(&Temp, 0x00, sizeof(_Ty));
			//	return Temp;
			//}

			return itr->second;
		}

		_Ty* findEZ_ptr(const _K& key)
		{
			auto itr = find(key);

			if (itr == end()) return nullptr;

			return &(itr->second);
		}
	};


	template <class _K, class _Ty, class _Pr = less<_K>, class _A = allocator<_Ty>>
	class mt_multimap : public multimap<_K, _Ty, _Pr, _A>, public mt_lock
	{
	public:
		// Functions with Lock
		void clearLock()
		{
			lock();
			clear();
			unlock();
		}

		void insertLock(const _K& key, const _Ty& value)
		{
			lock();
			insert(value_type(key, value));
			unlock();
		}

		bool deleteLock(const _K& key)
		{
			lock();
			auto nDeleted = erase(key);
			unlock();

			return nDeleted != 0;
		}

		// Functions without Lock
		void insertNoLock(const _K& key, const _Ty& value)
		{
			insert(value_type(key, value));
		}

		bool deleteNoLock(const _K& key)
		{
			return erase(key) != 0;
		}
	};

	template <class _Ty, class _A = allocator<_Ty>>
	class mt_vector : public vector<_Ty, _A>, public mt_lock
	{
	public:
		void pushBackLock(const _Ty& x)
		{
			lock();
			push_back(x);
			unlock();
		}

		void clearLock()
		{
			lock();
			clear();
			unlock();
		}
	};

	template <class _Ty, class _C = deque<_Ty>>
	class mt_queue : public queue<_Ty, _C>, public mt_lock
	{
	public:
		void pushLock(const _Ty& x)
		{
			lock();
			push(x);
			unlock();
		}

		void popLock(const _Ty& x)
		{
			lock();
			pop(x);
			unlock();
		}

		void clearLock()
		{
			lock();
			clear();
			unlock();
		}
	};


	using vectString = vector<string>;
	using setString = set<string>;
	using mtsetString = mt_set<string>;
}

using namespace mt_stl;

#pragma warning(pop)