#pragma once

#include <thread>
typedef std::chrono::duration<int> seconds;
class CAtumThread  
{
	bool	_stop;
	thread* _thread;

protected:
	// force the thread to terminate by raising the flag
	void ForceStop() { _stop = true; }

public:
	// whether the thread must terminate
	bool MustStop() const { return _stop; }

	// default constructor
	CAtumThread() : _stop { false }, _thread { nullptr } { }

	// virtual destructor waits for the thread to stop if running
	virtual ~CAtumThread()
	{
		_stop = true;

		if (_thread && _thread->joinable()) _thread->join();

		delete _thread;

		_stop = false;
		_thread = nullptr;
	}

	// must be overriden; defines functionality of running thread
	virtual DWORD Run() = 0;

	// initializes and starts the thread
	void CreateThread()
	{
		if (!_thread) _thread = new thread { [=]() { Run(); } };
	}
};
