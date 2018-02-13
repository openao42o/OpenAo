/******************************************************************************

	For MT-SAFE and/or Easy Use of STL Classes

******************************************************************************/

#ifndef __MT_STL_H__
#define __MT_STL_H__

#pragma warning(disable:4786)
#include <windows.h>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <functional>
#include "DebugAssert.h"

using namespace std;

class mt_lock
{
public:
	mt_lock()
	{
		InitializeCriticalSection(&m_lock);
	}

	~mt_lock()
	{
		EnterCriticalSection(&m_lock);	// 다른 곳에서 이미 lock을 잡았다면 처리 완료를 기다리기 위해
		LeaveCriticalSection(&m_lock);
		DeleteCriticalSection(&m_lock);
	}

	inline void lock()
	{
#ifdef _DEBUG
// 2005-08-01 by cmkwon
//		if (m_lock.LockCount < -1 || m_lock.LockCount > 100)
//		{
//			ASSERT_NEVER_GET_HERE();
//			return;
//		}
#endif
		EnterCriticalSection(&m_lock);
	}

#if (_WIN32_WINNT >= 0x0400)
	inline BOOL tryLock()
	{
		/*
		BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

		Return Values
			- If the critical section is successfully entered or the current
			thread already owns the critical section, the return value is nonzero.
			- If another thread already owns the critical section,
			the return value is zero.
		 */
		return TryEnterCriticalSection(&m_lock);
	}
#endif	// (_WIN32_WINNT >= 0x0400)_endif

	inline void unlock()
	{
#ifdef _DEBUG
// 2005-08-01 by cmkwon
//		if (m_lock.LockCount < -1 || m_lock.LockCount > 100)
//		{
//			ASSERT_NEVER_GET_HERE();
//			return;
//		}
#endif
		LeaveCriticalSection(&m_lock);
	}

public:
	CRITICAL_SECTION		m_lock;
};

// mt_auto_lock 변수 생성 후, scope를 빠져나오기 전에
// mt_lock이 invalidate되는 경우 cancel_unlock_on_exit()을 반드시 호출해야함.
class mt_auto_lock
{
public:
	mt_auto_lock(mt_lock *i_pMTLock)
	{
		m_pMTLock = i_pMTLock;
		m_pMTLock->lock();
	}

	~mt_auto_lock()
	{
		if (m_pMTLock != NULL)
		{
			m_pMTLock->unlock();
		}
	}

	void auto_unlock_cancel(void)
	{
		if(m_pMTLock)
		{
			m_pMTLock->unlock();
		}
		m_pMTLock = NULL;
	}

public:
	mt_lock	*m_pMTLock;
};

template<class _K, class _Ty, class _Pr = less<_K>, class _A = allocator<_Ty> >
class mt_map : public map< _K, _Ty, _Pr, _A >, public mt_lock
{
public:
	// Functions with Lock
	inline void clearLock()
	{
		lock();
		clear();
		unlock();
	}

	inline BOOL insertLock(const _K& key, const _Ty& value)
	{
		pair<map< _K, _Ty, _Pr, _A >::iterator, bool> res;

		lock();
		res = insert(pair<_K, _Ty>(key, value));
		unlock();

		return res.second;
	}

	inline BOOL deleteLock(const _K& key)
	{
		int nDeleted;

		lock();
		nDeleted = erase(key);
		unlock();

		return (nDeleted!=0)?TRUE:FALSE;
	}

	inline BOOL deleteLock(const _K& key, _Ty& value)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;

		lock();
		itr = find(key);
		if (itr == end())
		{
			unlock();
			return FALSE;
		}

		value = itr->second;
		erase(itr);
		unlock();

		return TRUE;
	}

	inline BOOL deleteLock(iterator it)
	{
		iterator itRet;
		BOOL bRet = FALSE;

		lock();
		itRet = erase(it);
		if(itRet != end())
		{
			bRet = TRUE;
		}
		unlock();

		return bRet;
	}

	inline _Ty findLock(const _K& key)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;
		_Ty retTy = NULL;

		lock();
		itr = find(key);
		if (itr != end())
		{
			retTy = itr->second;
		}
		unlock();
		
		return retTy;
	}


	///////////////////////////////////////////////////////////////////////////////
	/// \fn			inline _Ty popLock(const _K& key)
	/// \brief		
	/// \author		cmkwon
	/// \date		2004-10-21 ~ 2004-10-21
	/// \warning	객체가 value로 사용되면 안된다
	///				value는 Pointer 사용만 가능함
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	inline _Ty popLock(const _K& key)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;
		_Ty	ty = NULL;

		lock();
		itr = find(key);
		if (itr != end())
		{
			ty = itr->second;
			erase(itr);
		}
		unlock();
		return ty;
	}

	inline _Ty* findLock_Ptr(const _K& key)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;
		_Ty *pRetTy = NULL;

		lock();
		itr = find(key);
		if (itr != end())
		{
			pRetTy = &itr->second;
		}
		unlock();
		
		return pRetTy;
	}

	inline _Ty* findNoLock_Ptr(const _K& key)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;
		_Ty *pRetTy = NULL;

		itr = find(key);
		if (itr != end())
		{
			pRetTy = &itr->second;
		}
		
		return pRetTy;
	}

	// Functions without Lock
	inline BOOL insertNoLock(const _K& key, const _Ty& value)
	{
		pair<map< _K, _Ty, _Pr, _A >::iterator, bool> res;

		res = insert(pair<_K, _Ty>(key, value));

		return res.second;
	}

	inline BOOL deleteNoLock(const _K& key)
	{
		int nDeleted;

		nDeleted = erase(key);

		return (nDeleted!=0)?TRUE:FALSE;
	}

	inline BOOL deleteNoLock(const _K& key, _Ty& value)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;

		itr = find(key);
		if (itr == end())
		{
			return FALSE;
		}

		value = itr->second;
		erase(itr);

		return TRUE;
	}

	inline _Ty findNoLock(const _K& key)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;

		itr = find(key);

		if (itr == end())
		{
			return NULL;
		}
		return itr->second;
	}
};

template<class _K, class _Ty, class _Pr = less<_K>,	class _A = allocator<_Ty> >
class mt_multimap : public multimap< _K, _Ty, _Pr, _A >, public mt_lock
{
public:
	// Functions with Lock
	inline void clearLock()
	{
		lock();
		clear();
		unlock();
	}

	inline void insertLock(const _K& key, const _Ty& value)
	{
		lock();
		insert(value_type(key, value));
		unlock();
	}

	inline BOOL deleteLock(const _K& key)
	{
		int nDeleted;

		lock();
		nDeleted = erase(key);
		unlock();

		return (nDeleted!=0)?TRUE:FALSE;
	}

	// Functions without Lock
	inline void insertNoLock(const _K& key, const _Ty& value)
	{
		insert(value_type(key, value));
	}

	inline BOOL deleteNoLock(const _K& key)
	{
		int nDeleted;

		nDeleted = erase(key);

		return (nDeleted!=0)?TRUE:FALSE;
	}
};

template<class _K, class _Pr = less<_K>, class _A = allocator<_K> >
class mt_set : public set<_K, _Pr, _A>, public mt_lock
{
public:
	// Functions with Lock
	inline void clearLock()
	{
		lock();
		clear();
		unlock();
	}

	inline BOOL insertLock(const _K& key)
	{
		pair<set< _K, _Pr, _A >::iterator, bool> res;

		lock();
		res = insert(key);
		unlock();

		return res.second;
	}

	inline BOOL deleteLock(const _K& key)
	{
		int nDeleted;

		lock();
		nDeleted = erase(key);
		unlock();

		return (nDeleted!=0)?TRUE:FALSE;
	}

	inline BOOL deleteLock(iterator it)
	{
		iterator itRet;
		BOOL bRet = FALSE;
		lock();
		itRet = erase(it);
		if(itRet != end())
		{
			bRet = TRUE;
		}
		unlock();

		return bRet;
	}

	inline _K findLock(const _K& key)
	{
		set< _K, _Pr, _A >::iterator itr;
		_K retTy = NULL;

		lock();
		itr = find(key);
		if (itr != end())
		{
			retTy = *itr;
		}		
		unlock();
		
		return retTy;
	}

	inline _K* findLock_Ptr(const _K& key)
	{
		set< _K, _Pr, _A >::iterator itr;
		_K *pRetTy = NULL;

		lock();
		itr = find(key);
		if (itr != end())
		{
			pRetTy = &(*itr);
		}
		unlock();
		
		return pRetTy;
	}

	// Functions without Lock
	inline BOOL insertNoLock(const _K& key)
	{
		pair<set< _K, _Pr, _A >::iterator, bool> res;

		res = insert(key);

		return res.second;
	}

	inline BOOL deleteNoLock(const _K& key)
	{
		int nDeleted;

		nDeleted = erase(key);

		return (nDeleted!=0)?TRUE:FALSE;
	}

	inline _K findNoLock(const _K& key)
	{
		set< _K, _Pr, _A >::iterator itr;

		itr = find(key);

		if (itr == end())
		{
			return NULL;
		}
		return *itr;
	}
};

typedef set<string>				setString;			// 2007-05-04 by cmkwon, 추가함
typedef mt_set<string>			mtsetString;		// 2007-05-04 by cmkwon, 추가함



template<class _Ty, class _A = allocator<_Ty> >
class mt_list: public list<_Ty, _A>, public mt_lock
{
public:
	inline _Ty findLock(const _Ty& key)
	{
		lock();
		iterator itr = begin();
		while(itr != end())
		{
			if (key == *itr)
			{
				unlock();
				return *itr;
			}
			itr++;
		}
		unlock();

		return NULL;
	}

	inline int deleteLock(const _Ty& key)
	{
		int nDeleted = 0;
		lock();
		iterator itr = begin();
		while(itr != end())
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

	inline void pushBackLock(const _Ty& x)
	{
		lock();
		push_back(x);
		unlock();
	}

	inline void clearLock()
	{
		lock();
		clear();
		unlock();
	}

	inline _Ty getByIndexLock(int idx)
	{
		int tmp = 0;
		lock();
		iterator itr = begin();
		while(itr != end())
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

	inline _Ty getByIndexNoLock(int idx)
	{
		int tmp = 0;
		iterator itr = begin();
		while(itr != end())
		{
			if (tmp == idx)
			{
				return *itr;
			}
			itr++;
			tmp++;
		}
		return NULL;
	}
};

template<class _Ty, class _A = allocator<_Ty> >
class mt_vector: public vector<_Ty, _A>, public mt_lock
{
public:
	inline void pushBackLock(const _Ty& x)
	{
		lock();
		push_back(x);
		unlock();
	}

	inline void clearLock()
	{
		lock();
		clear();
		unlock();
	}
};

template<class _Ty, class _C = deque<_Ty> >
class mt_queue: public queue<_Ty, _C>, public mt_lock
{
public:
	inline void pushLock(const _Ty& x)
	{
		lock();
		push(x);
		unlock();
	}

	inline void popLock(const _Ty& x)
	{
		lock();
		pop(x);
		unlock();
	}

	inline void clearLock()
	{
		lock();
		clear();
		unlock();
	}
};

template<class _K, class _Ty, class _Pr = less<_K>, class _A = allocator<_Ty> >
class ez_map : public map< _K, _Ty, _Pr, _A >
{
public:
	inline BOOL insertEZ(const _K& key, const _Ty& value)
	{
		pair<map< _K, _Ty, _Pr, _A >::iterator, bool> res;

		res = insert(pair<_K, _Ty>(key, value));

		return res.second;
	}

	inline BOOL deleteEZ(const _K& key)
	{
		int nDeleted;

		nDeleted = erase(key);

		return (nDeleted!=0)?TRUE:FALSE;
	}

	inline BOOL deleteEZ(const _K& key, _Ty& value)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;

		itr = find(key);
		if (itr == end())
		{
			return FALSE;
		}

		value = itr->second;
		erase(itr);

		return TRUE;
	}

	inline _Ty findEZ(const _K& key)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;

		itr = find(key);

		if (itr == end())
		{
			_Ty		Temp;
			memset(&Temp, 0x00, sizeof(_Ty));
			return Temp;
		}
		return itr->second;
	}

	inline _Ty* findEZ_ptr(const _K& key)
	{
		map< _K, _Ty, _Pr, _A >::iterator itr;

		itr = find(key);

		if (itr == end())
		{
			return NULL;
		}
		return &(itr->second);
	}
};

template<class _K, class _Pr = less<_K>, class _A = allocator<_K> >
class ez_set : public set<_K, _Pr, _A>
{
public:
	inline BOOL insertEZ(const _K& key)
	{
		pair<set< _K, _Pr, _A >::iterator, bool> res;

		res = insert(key);

		return res.second;
	}

	inline BOOL deleteEZ(const _K& key)
	{
		int nDeleted;

		nDeleted = erase(key);

		return (nDeleted!=0)?TRUE:FALSE;
	}

	inline _K findEZ(const _K& key)
	{
		set< _K, _Pr, _A >::iterator itr;

		itr = find(key);

		if (itr == end())
		{
			return NULL;
		}
		return *itr;
	}
};

template<class _Ty, class _A = allocator<_Ty> >
class ez_list: public list<_Ty, _A>
{
public:
	inline _Ty findEZ(const _Ty& key)
	{
		iterator itr = begin();
		while(itr != end())
		{
			if (key == *itr)
			{
				return *itr;
			}
			itr++;
		}
		return NULL;
	}

	inline int deleteEZ(const _Ty& key)
	{
		int nDeleted = 0;
		iterator itr = begin();
		while(itr != end())
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

	inline _Ty getByIndexEZ(int idx)
	{
		int tmp = 0;
		iterator itr = begin();
		while(itr != end())
		{
			if (tmp == idx)
			{
				return *itr;
			}
			itr++;
			tmp++;
		}
		return NULL;
	}
};

typedef vector<string>		vectstring;	// 2007-05-28 by cmkwon
#endif
