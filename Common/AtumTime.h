#ifndef __ATUM_TIME_H__
#define __ATUM_TIME_H__

#include "StdAfx.h"

struct AtumTime {
	int Hour;		// ½Ã
	int Minute;		// ºÐ

	AtumTime() : Hour(0), Minute(0) {}
	AtumTime(int h, int m) : Hour(h), Minute(m) { Rearrange(); }

	inline bool Rearrange() {
		int time = Hour * 60 + Minute;
		if (time < 0) { Hour = 0; Minute = time; return true; }
		Hour = (int)(time / 60); Minute %= 60;
		return false;
	}

	inline void Reset() { Hour = 0; Minute = 0; }
	inline void SetTime(int h, int m) { Hour = h; Minute = m; }
	inline int GetTimeInMinute() { return Hour*60+Minute; }

	inline AtumTime operator+(const AtumTime &time) const;
	inline AtumTime& operator+=(const AtumTime &time);
	inline AtumTime operator+(const int minute) const;
	inline AtumTime& operator+=(const int minute);

	inline AtumTime operator-(const AtumTime &time) const;
	inline AtumTime& operator-=(const AtumTime &time);
	inline AtumTime operator-(const int minute) const;
	inline AtumTime& operator-=(const int minute);

	inline AtumTime operator*(const int n) const;
	inline AtumTime& operator*=(const int n);

	inline AtumTime operator/(const int n) const;
	inline AtumTime& operator/=(const int n);
};

AtumTime AtumTime::operator+(const AtumTime &time) const
{
	AtumTime ret(this->Hour + time.Hour, this->Minute + time.Minute);

	ret.Rearrange();

	return ret;
}

AtumTime& AtumTime::operator+=(const AtumTime &time)
{
	this->Hour += time.Hour;
	this->Minute += time.Minute;

	this->Rearrange();

	return *this;
}

AtumTime AtumTime::operator+(const int minute) const
{
	AtumTime ret(this->Hour, this->Minute + minute);

	ret.Rearrange();

	return ret;
}

AtumTime& AtumTime::operator+=(const int minute)
{
	this->Minute += minute;

	this->Rearrange();

	return *this;
}

AtumTime AtumTime::operator-(const AtumTime &time) const
{
	AtumTime ret(this->Hour - time.Hour, this->Minute - time.Minute);

	ret.Rearrange();

	return ret;
}

AtumTime& AtumTime::operator-=(const AtumTime &time)
{
	this->Hour -= time.Hour;
	this->Minute -= time.Minute;

	this->Rearrange();

	return *this;
}

AtumTime AtumTime::operator-(const int minute) const
{
	AtumTime ret(this->Hour, this->Minute - minute);

	ret.Rearrange();

	return ret;
}

AtumTime& AtumTime::operator-=(const int minute)
{
	this->Minute -= minute;

	this->Rearrange();

	return *this;
}

AtumTime AtumTime::operator*(const int n) const
{
	AtumTime ret(this->Hour*n, this->Minute*n);

	ret.Rearrange();

	return ret;
}

AtumTime& AtumTime::operator*=(const int n)
{
	this->Hour *= n;
	this->Minute *= n;

	this->Rearrange();

	return *this;
}

AtumTime AtumTime::operator/(const int n) const
{
	AtumTime ret(0, (this->Hour*60+this->Minute)/n);

	ret.Rearrange();

	return ret;
}

AtumTime& AtumTime::operator/=(const int n)
{
	this->Minute = (this->Hour*60+this->Minute)/n;
	this->Hour = 0;

	this->Rearrange();

	return *this;
}

#endif