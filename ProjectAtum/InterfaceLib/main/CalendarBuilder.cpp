#include "CalendarBuilder.h"

const BYTE CCalendarBuilder::DAYTABLE[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

CCalendarBuilder::CCalendarBuilder()
{
}

CCalendarBuilder::~CCalendarBuilder()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::CalMonthDay
/// \brief	: 그 달의 일수를 계산한다(2월일 경우 윤년 적용)
/// \author	: ckPark
/// \date	: 5:1:2009   18:43
/// \warning: 
/// \param	: const YEAR i_nYear
/// \param	: const MONTH i_nMonth
/// \return	: int
///////////////////////////////////////////////////////////////////////////////
int		CCalendarBuilder::CalMonthDay( const YEAR i_nYear, const MONTH i_nMonth )
{
	int nDay = DAYTABLE[i_nMonth - 1];

	if(IsLeapYear(i_nYear) && i_nMonth == 2)
		++nDay;

	return nDay;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::CalTotalDay
/// \brief	: 현재까지의 총 일수를 구한다
/// \author	: ckPark
/// \date	: 5:1:2009   18:43
/// \warning: 0년부터 현재년도까지 총 일수
/// \param	: const YEAR i_nYear
/// \param	: const MONTH i_nMonth
/// \param	: const DAY i_nDay
/// \return	: int
///////////////////////////////////////////////////////////////////////////////
int		CCalendarBuilder::CalTotalDay( const YEAR i_nYear, const MONTH i_nMonth, const DAY i_nDay )
{
	int y1		= i_nYear - 1;
	int total	= 365 * y1;

	total += ( y1 / 400 - y1 / 100 + y1 / 4 );

	for(int i=1; i<i_nMonth; ++i)
		total += CalMonthDay( i_nYear, i );

	total += i_nDay;

	return total;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::CalWeekDay
/// \brief	: 요일 인덱스를 구한다(0일, 1월, 2화, 3수, 4목, 5금, 6토)
/// \author	: 
/// \date	: 5:1:2009   18:44
/// \warning: 
/// \param	: const YEAR i_nYear
/// \param	: const MONTH i_nMonth
/// \param	: const DAY i_nDay
/// \return	: int
///////////////////////////////////////////////////////////////////////////////
int		CCalendarBuilder::CalWeekDay( const YEAR i_nYear, const MONTH i_nMonth, const DAY i_nDay )
{
	return CalTotalDay(i_nYear, i_nMonth, i_nDay) % 7;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::IsLeapYear
/// \brief	: 해당 년도가 윤년인지 판단 (윤년 TRUE)
/// \author	: ckPark
/// \date	: 5:1:2009   18:45
/// \warning: 
/// \param	: const YEAR i_nYear
/// \return	: BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL	CCalendarBuilder::IsLeapYear( const YEAR i_nYear )
{
	return !(i_nYear % 400) || ((i_nYear % 100) && !(i_nYear % 4));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::BuildCalendar
/// \brief	: 달력을 만든다. 총 6줄짜리 달력이므로 전월, 익월부분도 만든다
/// \author	: 
/// \date	: 5:1:2009   18:46
/// \warning: 
/// \param	: const YEAR i_nYear
/// \param	: const MONTH i_nMonth
/// \param	: Calendar * o_Calendar		// 결과물 달력타입
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CCalendarBuilder::BuildCalendar( const YEAR i_nYear, const MONTH i_nMonth, Calendar* o_Calendar )
{
	// 달력 초기화
	memset(o_Calendar, 0, sizeof(Calendar));

	// 이번달 일수
	int		nCurMonthDays	= CalMonthDay( i_nYear, i_nMonth );
	
	// 요일 인덱스
	int		nDay			= 1 - CalWeekDay( i_nYear, i_nMonth, 1 );
	
	// 달력 라인인덱스, 
	int		nLineIndex, nDayIndex;
	nLineIndex = nDayIndex = 0;




	// 익년, 월
	int		nNextYear, nNextMonth;
	
	if(i_nMonth != 12)
	{// 이번달이 12월이아니면
		nNextYear	= i_nYear;			// 현재년
		nNextMonth	= i_nMonth + 1;		// 다음달
	}
	else
	{// 이번달이 12월이면
		nNextYear	= i_nYear + 1;		// 다음년
		nNextMonth	= 1;				// 1월
	}
	
	// 익월 일수
	int		nNextDays	= 0;


	// 전년, 월
	int		nPrevYear, nPrevMonth;
	
	if(i_nMonth != 1)
	{// 이번달이 1월이 아니라면
		nPrevYear	= i_nYear;			// 현재년
		nPrevMonth	= i_nMonth - 1;		// 전달
	}
	else
	{// 이번달이 1월이라면
		nPrevYear	= i_nYear - 1;		// 전년
		nPrevMonth	= 12;				// 12월
	}

	// 전월 일수
	int		nPrevDays;
	// 1월일 경우 전년도 12월 일수를 얻어온다
	if(i_nMonth == 1)
		nPrevDays = CalMonthDay( i_nYear - 1, 12 );
	else	// 아닐경우 전월 일수를 얻어온다
		nPrevDays = CalMonthDay( i_nYear, i_nMonth - 1 );

	// 일수 거꾸로 맞추기
	nPrevDays = nPrevDays + nDay - 1;



	for(int i = 0; i < MAX_CALENDAR_LINE * MAX_DAY; ++i, ++nDay)
	{
		if(nDay < 1)					// 전월일 경우
		{
			(*o_Calendar)[nLineIndex][nDayIndex].nYear	= nPrevYear;
			(*o_Calendar)[nLineIndex][nDayIndex].nMonth	= nPrevMonth;
			(*o_Calendar)[nLineIndex][nDayIndex].nDay	= ++nPrevDays;
		}
		else if(nDay > nCurMonthDays)	// 다음월일 경우
		{
			(*o_Calendar)[nLineIndex][nDayIndex].nYear	= nNextYear;
			(*o_Calendar)[nLineIndex][nDayIndex].nMonth	= nNextMonth;
			(*o_Calendar)[nLineIndex][nDayIndex].nDay	= ++nNextDays;
		}
		else							// 이번월일 경우
		{
			(*o_Calendar)[nLineIndex][nDayIndex].nYear	= i_nYear;
			(*o_Calendar)[nLineIndex][nDayIndex].nMonth	= i_nMonth;
			(*o_Calendar)[nLineIndex][nDayIndex].nDay	= nDay;
		}

		++nDayIndex;					// 요일 인덱스 증가

		if( (i % 7) == 6)				// 요일이 넘어갔을 경우
		{
			++nLineIndex;				// 라인인덱스 증가
			nDayIndex = 0;				// 요일 초기화
		}
	}
}