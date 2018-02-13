#ifndef _ATUM_DATE_TIME_H_
#define _ATUM_DATE_TIME_H_

#ifdef _ATUM_SERVER
#include <SQLTYPES.H>
#endif // _ATUM_SERVER
#include <WINDOWS.H>
#include <TIME.H>
#include <string>


#if defined(_ATUM_SERVER)
#include "StringDefineQuery.h"			// 2008-07-30 by cmkwon
#include "StringDefineProcedure.h"	// 2008-08-22 by cmkwon, MySQL 통합(Procedure 뽑아내기) - 
#endif

using namespace std;

// 2008-01-04 by cmkwon, 언어와 상관없이 하려면 아래와 같은 형식 이어야 함
//#define SQL_DATETIME_STRING_FORMAT					"%04d-%02d-%02d %02d:%02d:%02d.000"

///////////////////////////////////////////////////////////////////////////////
// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
#if defined(DB_SERVER_MYSQL)
#define SQL_DATETIME_STRING_FORMAT					"%04d-%02d-%02d %02d:%02d:%02d.000"
#else
#define SQL_DATETIME_STRING_FORMAT					"%04d%02d%02d %02d:%02d:%02d.000"
#endif
#define ATUM_DATE_TIME_STRING_FORMAT				"%04d-%02d-%02d %02d:%02d:%02d"
#define ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND	"%04d-%02d-%02d %02d:%02d"


#define SIZE_MAX_SQL_DATETIME_STRING	30
#define SIZE_MAX_ATUM_DATE_TIME_STRING	25

#define GET_CURRENT_DATE_TIME_STRING()	ATUM_DATE_TIME::GetCurrentDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING))

struct STRNBUF;

struct ATUM_DATE_TIME
{
    USHORT		Year;
    BYTE		Month;
    BYTE		Day;
    BYTE		Hour;
    BYTE		Minute;
    BYTE		Second;

public:
	ATUM_DATE_TIME () 
	{
		CheckSettm_isdst();		// 2008-04-02 by cmkwon, 썸머타임 관련 버그 수정 - 
		Reset();
	}
	ATUM_DATE_TIME (bool i_bSetCurrentTime)
	{
		CheckSettm_isdst();		// 2008-04-02 by cmkwon, 썸머타임 관련 버그 수정 - 
		if (i_bSetCurrentTime)
			SetCurrentDateTime();
		else
			Reset();
	}
	ATUM_DATE_TIME (const char *i_szDateTimeString)
	{
		CheckSettm_isdst();		// 2008-04-02 by cmkwon, 썸머타임 관련 버그 수정 - 
		SetDateTime(i_szDateTimeString);
	}

	void Reset() { Year = Month = Day = Hour = Minute = Second = 0; }

	void SetCurrentDateTime(BOOL i_bSetSecondZero=FALSE);
	void SetDateTime(const char *i_szDateTimeString);
	void SetDateTime(USHORT i_usYear, BYTE i_byMonth, BYTE i_byDay, BYTE i_byHour, BYTE i_byMinute, BYTE i_bySecond);

	void AddDateTime(INT i_nYear, INT i_nMonth, INT i_nDay, INT i_nHour=0, INT i_nMinute=0, INT i_nSecond=0);

	const char* GetDateTimeString(char *i_szBuffer, int i_nLen, BOOL i_bExcludeSecond=FALSE) const;
	const char* GetSQLDateTimeString(char *i_szBuffer, int i_nLen) const;
	const char* GetDateTimeString(STRNBUF &i_strnbuf, BOOL i_bExcludeSecond=FALSE) const;
	const char* GetSQLDateTimeString(STRNBUF &i_strnbuf) const;

// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - GetLocalDateTimeString() 로 대체함
// 	const char* GetNationalDateTimeString(STRNBUF &i_strnbuf, BOOL i_bExcludeSecond=FALSE) const;
// 	const char* GetNationalDateTimeString(char *i_szBuffer, int i_nLen, BOOL i_bExcludeSecond=FALSE) const;

	const int GetTimeInSeconds() const;
	const int GetTimeDiffToCurrentTimeInSeconds() const;
	int GetTimeDiffTimeInSeconds(ATUM_DATE_TIME i_atOP2);
	int GetTimeDiffTimeInMinutes(ATUM_DATE_TIME i_atOP2);
	int GetTimeDiffTimeInHours(ATUM_DATE_TIME i_atOP2);

	// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
	BOOL IsValidATUM_DATE_TIME(void);

	static ATUM_DATE_TIME GetCurrentDateTime();
	static const char* GetCurrentDateTimeString(STRNBUF &i_strnbuf);

	static const char* GetDateTimeStringFromSeconds(int i_nSeconds, string &i_szStrBuf);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - 인자 추가(int i_nLangTy)
	static const char* GetLocalString_YYYYMMDD(int i_Year, int i_Month, int i_Day, string &io_szStrBuf, int i_nLangTy);	// 2007-10-05 by cmkwon, 연월일을 나라별로 다르게하기 위해
	static const char* GetLocalString_YYYYMM(int i_Year, int i_Month, string &io_szStrBuf, int i_nLangTy);					// 2007-10-05 by cmkwon, 연월일을 나라별로 다르게하기 위해
	static const char* GetLocalString_MMDD(int i_Month, int i_Day, string &io_szStrBuf, int i_nLangTy);					// 2007-10-05 by cmkwon, 연월일을 나라별로 다르게하기 위해

	const char* GetLocalDateTimeString(string &o_szStrBuf, int i_nLangTy, BOOL i_bExcludeSecond=FALSE) const;		// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-02 by cmkwon, 썸머타임 관련 버그 수정 - 
	static BOOL			ms_bSettm_isdst;		// 아래의 썸머타임 관련 변수 설정 했는지 여부 플래그
	static int			ms_tm_isdst;			// 썸머타임 관련 변수 -  0이면 표준시를 사용하는 것이고, 양수라면 시행 중인 것이고, 음수라면 TZ(time zone) 환경 변수에 따라 표준시를 사용하거나 일광 절약 시간을 사용합니다
	static void CheckSettm_isdst(void);

	ATUM_DATE_TIME& operator=(const tm& rhs);
	time_t Convert(tm &o_tm) const;
#ifdef _ATUM_SERVER
	ATUM_DATE_TIME& operator=(const SQL_TIMESTAMP_STRUCT& rhs);
	void Convert(SQL_TIMESTAMP_STRUCT &o_SQL_TIMESTAMP_STRUCT) const;
#endif // _ATUM_SERVER

	inline bool operator==(const ATUM_DATE_TIME & dt)
	{
		if ( Year == dt.Year
			&& Month == dt.Month
			&& Day == dt.Day
			&& Hour == dt.Hour
			&& Minute == dt.Minute
			&& Second == dt.Second
		)
		{
			return TRUE;
		}
		return FALSE;
	}

	inline bool operator!=(const ATUM_DATE_TIME & dt)
	{
		return !(*this == dt);
	}

	inline bool operator<(const ATUM_DATE_TIME & dt)
	{
		if (Year != dt.Year) { return Year < dt.Year; }
		if (Month != dt.Month) { return Month < dt.Month; }
		if (Day != dt.Day) { return Day < dt.Day; }
		if (Hour != dt.Hour) { return Hour < dt.Hour; }
		if (Minute != dt.Minute) { return Minute < dt.Minute; }
		if (Second != dt.Second) { return Second < dt.Second; }

		return FALSE;
	}

	inline bool operator>(const ATUM_DATE_TIME & dt)
	{
		if (Year != dt.Year) { return Year > dt.Year; }
		if (Month != dt.Month) { return Month > dt.Month; }
		if (Day != dt.Day) { return Day > dt.Day; }
		if (Hour != dt.Hour) { return Hour > dt.Hour; }
		if (Minute != dt.Minute) { return Minute > dt.Minute; }
		if (Second != dt.Second) { return Second > dt.Second; }

		return FALSE;
	}

	inline bool operator<=(const ATUM_DATE_TIME &dt) { return !(*this > dt); }
	inline bool operator>=(const ATUM_DATE_TIME &dt) { return !(*this < dt); }
};

bool operator<(const ATUM_DATE_TIME &dt1, const ATUM_DATE_TIME &dt2);

#endif // _ATUM_DATE_TIME_H_
