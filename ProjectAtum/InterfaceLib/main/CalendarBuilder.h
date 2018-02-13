#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include <windows.h>

#define MAX_DAY				7			// 요일(일, 월, 화, 수, 목, 금, 토)
#define MAX_CALENDAR_LINE	6			// 달력 최대 라인
#define MAX_MONTH			12			// 12달 (1~12월)

typedef USHORT		YEAR;				// 년
typedef BYTE		MONTH;				// 월
typedef BYTE		DAY;				// 일

struct sYearMonthDay
{
	YEAR	nYear;
	MONTH	nMonth;
	DAY		nDay;
};


typedef sYearMonthDay	Calendar[MAX_CALENDAR_LINE][MAX_DAY];	// 6줄짜리 7일 달력

///////////////////////////////////////////////////////////////////////////////
// \class 	: CCalendarBuilder
// \brief	: 달력 생성기
// \author	: ckPark
// \version : 
// \date	: 5:1:2009   15:09
// \warning	: 
///////////////////////////////////////////////////////////////////////////////
class CCalendarBuilder
{
private:
	// 각 월별 일수 테이블
	const	static	BYTE	DAYTABLE[MAX_MONTH];
	
public:
	CCalendarBuilder();
	~CCalendarBuilder();
	
public:
	// 그 달의 일수를 계산한다(2월일 경우 윤년 적용)
	int		CalMonthDay( const YEAR i_nYear, const MONTH i_nMonth );
	// 현재까지의 총 일수를 구한다
	int		CalTotalDay( const YEAR i_nYear, const MONTH i_nMonth, const DAY i_nDay );
	// 요일 인덱스를 구한다(0일, 1월, 2화, 3수, 4목, 5금, 6토)
	int		CalWeekDay( const YEAR i_nYear, const MONTH i_nMonth, const DAY i_nDay );
	// 해당 년도가 윤년인지 판단
	BOOL	IsLeapYear( const YEAR i_nYear );
	// 달력을 만든다. 총 6줄짜리 달력이므로 전월, 익월부분도 만든다
	void	BuildCalendar(const YEAR i_nYear, const MONTH i_nMonth, Calendar* o_Calendar );
};

#endif