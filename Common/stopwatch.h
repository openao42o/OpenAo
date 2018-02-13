//////////////////////////////////////////////////////////////////////
// This class implements generic StopWatch.
// Warning: This is thread unsafe. 
//////////////////////////////////////////////////////////////////////

#ifndef __swatch_h_
#define __swatch_h_

#ifndef __cplusplus
#error "This code cannot be complied in C"
#endif

//////////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
//////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sys/timeb.h>

class StopWatch {
public:
	StopWatch();
	void reset();
	void start();
	void stop();
	double time() const { return realtime_; }

private:
	enum { RUNNING, STOPPED };
	int status;
	_timeb	start_;
	_timeb	stop_;
	_timeb	interval_;
	int splitCount;
	double realtime_;
};

// don't consider timezone and daylight saving
inline _timeb operator-(const _timeb& a, const _timeb& b) {
	_timeb c;
	if (a.millitm >= b.millitm) {
		c.millitm = a.millitm - b.millitm;
		c.time = a.time - b.time;
	}
	else {
		c.millitm = 1000 + a.millitm - b.millitm;
		c.time = a.time - b.time - 1;
	}
	return c;
}

inline _timeb operator+ (const _timeb& a, const _timeb& b) {
	_timeb c;
	c.time = a.time + b.time;
	c.millitm = a.millitm + b.millitm;
	if (c.millitm >= 1000) {
		c.millitm -= 1000;
		c.time++;
	}
	return c;
}

inline _timeb& operator+= (_timeb& a, const _timeb& b) {
	a = a + b;
	return a;
}

inline _timeb& operator-= (_timeb& a, const _timeb& b) {
	a = a - b;
	return a;
}

inline StopWatch::StopWatch() {
	reset();
}

inline void StopWatch::reset() {
	interval_.time = 0;
	interval_.millitm = 0;
	realtime_ = 0;
	splitCount = 0;
	status = STOPPED;
}

inline void StopWatch::start() {
	if (status == STOPPED) {
		status = RUNNING;
		splitCount++;
		_ftime(&start_);
	}
}

inline void StopWatch::stop() {
	if (status == RUNNING) {
		_ftime(&stop_);
		interval_ += stop_ - start_;
		realtime_ = ((double)interval_.millitm) / 1e3 + interval_.time;
		status = STOPPED;
	}
}

#endif // end of _WIN32


#endif
