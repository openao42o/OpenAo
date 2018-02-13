#include "StdAfx.h"
#include "AtumDateTime.h"
#include "AtumParam.h"

// 2008-04-02 by cmkwon, 썸머타임 관련 버그 수정 - 
BOOL ATUM_DATE_TIME::ms_bSettm_isdst		= FALSE;	// 초기화
int ATUM_DATE_TIME::ms_tm_isdst				= 0;		// 초기화


ATUM_DATE_TIME ATUM_DATE_TIME::GetCurrentDateTime()
{
	ATUM_DATE_TIME retDateTime;
	retDateTime.SetCurrentDateTime();

	return retDateTime;
}

const char* ATUM_DATE_TIME::GetCurrentDateTimeString(STRNBUF &i_strnbuf)
{
	ATUM_DATE_TIME retDateTime;
	retDateTime.SetCurrentDateTime();
	retDateTime.GetDateTimeString(i_strnbuf);

	return i_strnbuf.GetBuffer();
}

const char* ATUM_DATE_TIME::GetDateTimeStringFromSeconds(int i_nSeconds, string &i_szStrBuf)
{
	char buf[512];
// 2007-10-05 by cmkwon, 연월일을 나라별로 다르게하기 위해, 버그 수정한것
//	sprintf(buf, STRCMD_CS_COMMON_DATETIME_0000,
//			(int)i_nSeconds/(24*3600),
//			((int)i_nSeconds%(24*3600))/3600,
//			((int)i_nSeconds%3600)/60,
//			((int)i_nSeconds%60));
	int Day		= (int)i_nSeconds/(24*3600);
	int Hour	= ((int)i_nSeconds%(24*3600))/3600;
	int Minute	= ((int)i_nSeconds%3600)/60;
	int Second	= ((int)i_nSeconds%60);
	sprintf(buf, STRCMD_071005_0000);
	i_szStrBuf = buf;
	return i_szStrBuf.c_str();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			const char* ATUM_DATE_TIME::GetLocalString_YYYYMMDD(int i_Year, int i_Month, int i_Day, string &io_szStrBuf, int i_nLangTy)
/// \brief		// 2007-10-05 by cmkwon, 연월일을 나라별로 다르게하기 위해
///				// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - 인자 추가(int i_nLangTy)
/// \author		cmkwon
/// \date		2007-10-05 ~ 2007-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
const char* ATUM_DATE_TIME::GetLocalString_YYYYMMDD(int i_Year, int i_Month, int i_Day, string &io_szStrBuf, int i_nLangTy)
{
	char buf[512];		util::zero(buf, 512);
	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - LANGUAGE_TYPE_ENGLISH 만 아래와 같은 형식으로 처리
	switch(i_nLangTy)
	{
	case LANGUAGE_TYPE_ENGLISH:
		{
			ATUM_DATE_TIME atCur(TRUE);
			atCur.Year	= i_Year;
			atCur.Month	= i_Month;
			atCur.Day	= i_Day;
			tm tmTM;
			atCur.Convert(tmTM);
			// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - 아래와 같이 수정
			//strftime(buf, 512, "%B %d, %Y", &tmTM);		// March 10, 2008
			strftime(buf, 512, "%B-%d-%Y", &tmTM);		// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - March-10-2008
		}
		break;
	default:
		{
			int Year	= i_Year;
			int Month	= i_Month;
			int Day		= i_Day;
			sprintf(buf, STRCMD_071005_0001);
		}
	}
	io_szStrBuf = buf;
	return io_szStrBuf.c_str();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			const char* ATUM_DATE_TIME::GetLocalString_YYYYMM(int i_Year, int i_Month, string &io_szStrBuf, int i_nLangTy)
/// \brief		// 2007-10-05 by cmkwon, 연월일을 나라별로 다르게하기 위해
///				// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - 인자 추가(int i_nLangTy)
/// \author		cmkwon
/// \date		2007-10-05 ~ 2007-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
const char* ATUM_DATE_TIME::GetLocalString_YYYYMM(int i_Year, int i_Month, string &io_szStrBuf, int i_nLangTy)
{
	char buf[512];		util::zero(buf, 512);
	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - LANGUAGE_TYPE_ENGLISH 만 아래와 같은 형식으로 처리
	switch(i_nLangTy)
	{
	case LANGUAGE_TYPE_ENGLISH:
		{
			ATUM_DATE_TIME atCur(TRUE);
			atCur.Year	= i_Year;
			atCur.Month	= i_Month;
			tm tmTM;
			atCur.Convert(tmTM);
			// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - 아래와 같이 수정
			//strftime(buf, 512, "%B %Y", &tmTM);		// March 2008
			strftime(buf, 512, "%B-%Y", &tmTM);		// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - March-2008
		}
		break;
	default:
		{
			int Year	= i_Year;
			int Month	= i_Month;
			sprintf(buf, STRCMD_071005_0002);
		}
	}
	io_szStrBuf = buf;
	return io_szStrBuf.c_str();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			const char* ATUM_DATE_TIME::GetLocalString_MMDD(int i_Month, int i_Day, string &io_szStrBuf, int i_nLangTy)
/// \brief		// 2007-10-05 by cmkwon, 연월일을 나라별로 다르게하기 위해
///				// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - 인자 추가(int i_nLangTy)
/// \author		cmkwon
/// \date		2007-10-05 ~ 2007-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
const char* ATUM_DATE_TIME::GetLocalString_MMDD(int i_Month, int i_Day, string &io_szStrBuf, int i_nLangTy)
{
	char buf[512];		util::zero(buf, 512);
	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - LANGUAGE_TYPE_ENGLISH 만 아래와 같은 형식으로 처리
	switch(i_nLangTy)
	{
	case LANGUAGE_TYPE_ENGLISH:
		{
			ATUM_DATE_TIME atCur(TRUE);
			atCur.Month	= i_Month;
			atCur.Day	= i_Day;
			tm tmTM;
			atCur.Convert(tmTM);
			// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - 아래와 같이 수정
			//strftime(buf, 512, "%B %d", &tmTM);		// March 10
			strftime(buf, 512, "%B-%d", &tmTM);		// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - March-10
		}
		break;
	default:
		{
			int Month	= i_Month;
			int Day		= i_Day;
			sprintf(buf, STRCMD_071005_0003);
		}
	}
	io_szStrBuf = buf;
	return io_szStrBuf.c_str();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			const char* ATUM_DATE_TIME::GetLocalDateTimeString(string &o_szStrBuf, int i_nLangTy, BOOL i_bExcludeSecond/*=FALSE*/) const
/// \brief		// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - 
/// \author		cmkwon
/// \date		2008-03-18 ~ 2008-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
const char* ATUM_DATE_TIME::GetLocalDateTimeString(string &o_szStrBuf, int i_nLangTy, BOOL i_bExcludeSecond/*=FALSE*/) const
{
	char buf[512];		util::zero(buf, 512);
	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - LANGUAGE_TYPE_ENGLISH 만 아래와 같은 형식으로 처리
	switch(i_nLangTy)
	{
	case LANGUAGE_TYPE_ENGLISH:
		{
			tm tmTM;
			this->Convert(tmTM);
			if(FALSE == i_bExcludeSecond)
			{
				// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - 아래와 같이 수정
				//strftime(buf, 512, "%B %d, %Y %H:%M:%S", &tmTM);		// March 10, 2008 15:30:20
				strftime(buf, 512, "%B-%d-%Y %H:%M:%S", &tmTM);		// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - March-10-2008 15:30:20
			}
			else
			{
				// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - 아래와 같이 수정
				//strftime(buf, 512, "%B %d, %Y %H:%M", &tmTM);			// March 10, 2008 15:30
				strftime(buf, 512, "%B-%d-%Y %H:%M", &tmTM);			// 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - March-10-2008 15:30
			}
			o_szStrBuf = buf;
		}
		break;
	default:
		{
			if(FALSE == i_bExcludeSecond)
			{
				sprintf(buf, NATIONAL_ATUM_DATE_TIME_STRING_FORMAT(Year, Month, Day, Hour, Minute, Second));
			}
			else
			{
				sprintf(buf, NATIONAL_ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND(Year, Month, Day, Hour, Minute));
			}
		}
	}
	
	o_szStrBuf = buf;
	return o_szStrBuf.c_str();
}

void ATUM_DATE_TIME::CheckSettm_isdst(void)
{
	if(ms_bSettm_isdst)
	{
		return;
	}

	time_t ltime;
	struct tm *today = NULL;
	time(&ltime);
	today = localtime(&ltime);
	ms_tm_isdst		= today->tm_isdst;
	ms_bSettm_isdst	= TRUE;
}

void ATUM_DATE_TIME::SetCurrentDateTime(BOOL i_bSetSecondZero/*=FALSE*/)
{
	time_t ltime;
	struct tm *today = NULL;
	time(&ltime);
	today = localtime(&ltime);

	Year	= today->tm_year + 1900;
	Month	= today->tm_mon + 1;
	Day		= today->tm_mday;
	Hour	= today->tm_hour;
	Minute	= today->tm_min;	
	Second	= today->tm_sec;
	if(i_bSetSecondZero)
	{
		Second = 0;
	}
}

void ATUM_DATE_TIME::SetDateTime(const char *i_szDateTimeString)
{
	Reset();

	char *token;
	char seps[] = "-.: \t";
	char buffer[SIZE_MAX_SQL_DATETIME_STRING];
	util::strncpy(buffer, i_szDateTimeString, SIZE_MAX_SQL_DATETIME_STRING);

	token = strtok(buffer, seps); if (token != NULL) Year = atoi(token); else return;
	token = strtok(NULL, seps); if (token != NULL) Month = atoi(token); else return;
	token = strtok(NULL, seps); if (token != NULL) Day = atoi(token); else return;
	token = strtok(NULL, seps); if (token != NULL) Hour = atoi(token); else return;
	token = strtok(NULL, seps); if (token != NULL) Minute = atoi(token); else return;
	token = strtok(NULL, seps); if (token != NULL) Second = atoi(token); else return;
}

void ATUM_DATE_TIME::SetDateTime(USHORT i_usYear, BYTE i_byMonth, BYTE i_byDay, BYTE i_byHour, BYTE i_byMinute, BYTE i_bySecond)
{
	Year	= i_usYear;
	Month	= i_byMonth;
	Day		= i_byDay;
	Hour	= i_byHour;
	Minute	= i_byMinute;
	Second	= i_bySecond;
}

void ATUM_DATE_TIME::AddDateTime(INT i_nYear, INT i_nMonth, INT i_nDay, INT i_nHour/*=0*/, INT i_nMinute/*=0*/, INT i_nSecond/*=0*/)
{
	time_t ltime = time(0);
	tm tmpTM;

	Convert(tmpTM);

	tmpTM.tm_year	+= i_nYear;
	tmpTM.tm_mon 	+= i_nMonth;
	tmpTM.tm_mday	+= i_nDay;
	tmpTM.tm_hour	+= i_nHour;
	tmpTM.tm_min	+= i_nMinute;
	tmpTM.tm_sec	+= i_nSecond;

	ltime = mktime(&tmpTM);

	*this = tmpTM;
}

const char* ATUM_DATE_TIME::GetDateTimeString(char *i_szBuffer, int i_nLen, BOOL i_bExcludeSecond/*=FALSE*/) const
{
	if(FALSE == i_bExcludeSecond)
	{
		sprintf(i_szBuffer, ATUM_DATE_TIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);
	}
	else
	{
		sprintf(i_szBuffer, ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND, Year, Month, Day, Hour, Minute);
	}
	return i_szBuffer;
}

const char* ATUM_DATE_TIME::GetSQLDateTimeString(char *i_szBuffer, int i_nLen) const
{
	sprintf(i_szBuffer, SQL_DATETIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);

	return i_szBuffer;
}

const char* ATUM_DATE_TIME::GetDateTimeString(STRNBUF &i_strnbuf, BOOL i_bExcludeSecond/*=FALSE*/) const
{
	if(FALSE == i_bExcludeSecond)
	{
		sprintf(i_strnbuf.GetBuffer(), ATUM_DATE_TIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);
	}
	else
	{
		sprintf(i_strnbuf.GetBuffer(), ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND, Year, Month, Day, Hour, Minute);
	}
	return i_strnbuf.GetBuffer();
}

const char* ATUM_DATE_TIME::GetSQLDateTimeString(STRNBUF &i_strnbuf) const
{
	sprintf(i_strnbuf.GetBuffer(), SQL_DATETIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);
	
	return i_strnbuf.GetBuffer();
}

// 2008-03-18 by cmkwon, 나라별 날짜 형식 수정 - GetLocalDateTimeString() 로 대체함
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			const char* ATUM_DATE_TIME::GetNationalDateTimeString(char *i_szBuffer, int i_nLen, BOOL i_bExcludeSecond/*=FALSE*/) const
// /// \brief		
// /// \author		cmkwon
// /// \date		2006-08-03 ~ 2006-08-03
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// const char* ATUM_DATE_TIME::GetNationalDateTimeString(char *i_szBuffer, int i_nLen, BOOL i_bExcludeSecond/*=FALSE*/) const
// {
// 	if(FALSE == i_bExcludeSecond)
// 	{
// 		sprintf(i_szBuffer, NATIONAL_ATUM_DATE_TIME_STRING_FORMAT(Year, Month, Day, Hour, Minute, Second));
// 	}
// 	else
// 	{
// 		sprintf(i_szBuffer, NATIONAL_ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND(Year, Month, Day, Hour, Minute));
// 	}
// 	return i_szBuffer;
// }
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			const char* ATUM_DATE_TIME::GetNationalDateTimeString(STRNBUF &i_strnbuf, BOOL i_bExcludeSecond/*=FALSE*/) const
// /// \brief		
// /// \author		cmkwon
// /// \date		2006-08-03 ~ 2006-08-03
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// const char* ATUM_DATE_TIME::GetNationalDateTimeString(STRNBUF &i_strnbuf, BOOL i_bExcludeSecond/*=FALSE*/) const
// {
// 	if(FALSE == i_bExcludeSecond)
// 	{
// 		sprintf(i_strnbuf.GetBuffer(), NATIONAL_ATUM_DATE_TIME_STRING_FORMAT(Year, Month, Day, Hour, Minute, Second));
// 	}
// 	else
// 	{
// 		sprintf(i_strnbuf.GetBuffer(), NATIONAL_ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND(Year, Month, Day, Hour, Minute));
// 	}
// 	return i_strnbuf.GetBuffer();
// }



const int ATUM_DATE_TIME::GetTimeInSeconds() const
{
	tm tmpTime;
	return Convert(tmpTime);
}

const int ATUM_DATE_TIME::GetTimeDiffToCurrentTimeInSeconds() const
{
	time_t CurrentTime;
	return time(&CurrentTime) - GetTimeInSeconds();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int ATUM_DATE_TIME::GetTimeDiffTimeInSeconds(ATUM_DATE_TIME i_atOP2)
/// \brief		
/// \author		cmkwon
/// \date		2007-02-07 ~ 2007-02-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int ATUM_DATE_TIME::GetTimeDiffTimeInSeconds(ATUM_DATE_TIME i_atOP2)
{
	return this->GetTimeInSeconds() - i_atOP2.GetTimeInSeconds();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int ATUM_DATE_TIME::GetTimeDiffTimeInMinutes(ATUM_DATE_TIME i_atOP2)
/// \brief		
/// \author		cmkwon
/// \date		2007-02-07 ~ 2007-02-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int ATUM_DATE_TIME::GetTimeDiffTimeInMinutes(ATUM_DATE_TIME i_atOP2)
{
	return this->GetTimeDiffTimeInSeconds(i_atOP2)/60;	// 2007-02-07 by cmkwon, 초를 분단위로 변경
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int ATUM_DATE_TIME::GetTimeDiffTimeInHours(ATUM_DATE_TIME i_atOP2)
/// \brief		
/// \author		cmkwon
/// \date		2007-02-07 ~ 2007-02-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int ATUM_DATE_TIME::GetTimeDiffTimeInHours(ATUM_DATE_TIME i_atOP2)
{
	return this->GetTimeDiffTimeInMinutes(i_atOP2)/60;	// 2007-02-07 by cmkwon, 분단위를 시간단위로 변경
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL ATUM_DATE_TIME::IsValidATUM_DATE_TIME(void)
/// \brief		// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
/// \author		cmkwon
/// \date		2008-02-01 ~ 2008-02-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL ATUM_DATE_TIME::IsValidATUM_DATE_TIME(void)
{
	if(0 == Year
		|| 0 == Month
		|| 0 == Day)
	{// 2008-02-01 by cmkwon, 년월일이 설정되어 있는지 여부를 리턴한다.
		return FALSE;
	}

	return TRUE;
}

ATUM_DATE_TIME& ATUM_DATE_TIME::operator=(const tm& rhs)
{
	this->Year = rhs.tm_year + 1900;
	this->Month = rhs.tm_mon + 1;
	this->Day = rhs.tm_mday;
	this->Hour = rhs.tm_hour;
	this->Minute = rhs.tm_min;
	this->Second = rhs.tm_sec;

	return *this;
}

time_t ATUM_DATE_TIME::Convert(tm &o_tm) const
{
	util::zero(&o_tm, sizeof(tm));		// 2008-01-09 by cmkwon, Gameforge4D 버그 수정 - 초기화 처리	

	o_tm.tm_year	= min(137, Year - 1900);		// 2008-06-27 by cmkwon, 날짜시간 관련 버그 수정(1970 ~ 2037) - 최대 년도를 임시로 처리, fail after 2038-01-18
	o_tm.tm_mon 	= Month - 1;
	o_tm.tm_mday	= Day;
	o_tm.tm_hour	= Hour;
	o_tm.tm_min		= Minute;
	o_tm.tm_sec		= Second;
	o_tm.tm_isdst	= ATUM_DATE_TIME::ms_tm_isdst;	// 2008-04-02 by cmkwon, 썸머타임 관련 버그 수정 - 

	return mktime(&o_tm);
}

#ifdef _ATUM_SERVER
ATUM_DATE_TIME& ATUM_DATE_TIME::operator=(const SQL_TIMESTAMP_STRUCT& rhs)
{
	this->Year		= rhs.year;
	this->Month		= rhs.month;
	this->Day		= rhs.day;
	this->Hour		= rhs.hour;
	this->Minute	= rhs.minute;
	this->Second	= rhs.second;

	return *this;
}

void ATUM_DATE_TIME::Convert(SQL_TIMESTAMP_STRUCT &o_SQL_TIMESTAMP_STRUCT) const
{
	o_SQL_TIMESTAMP_STRUCT.year		= Year;
	o_SQL_TIMESTAMP_STRUCT.month	= Month;
	o_SQL_TIMESTAMP_STRUCT.day		= Day;
	o_SQL_TIMESTAMP_STRUCT.hour		= Hour;
	o_SQL_TIMESTAMP_STRUCT.minute	= Minute;
	o_SQL_TIMESTAMP_STRUCT.second	= Second;
	o_SQL_TIMESTAMP_STRUCT.fraction	= 0;
}
#endif // _ATUM_SERVER

bool operator<(const ATUM_DATE_TIME &dt1, const ATUM_DATE_TIME &dt2)
{
	if (dt1.Year != dt2.Year) { return dt1.Year < dt2.Year; }
	if (dt1.Month != dt2.Month) { return dt1.Month < dt2.Month; }
	if (dt1.Day != dt2.Day) { return dt1.Day < dt2.Day; }
	if (dt1.Hour != dt2.Hour) { return dt1.Hour < dt2.Hour; }
	if (dt1.Minute != dt2.Minute) { return dt1.Minute < dt2.Minute; }
	if (dt1.Second != dt2.Second) { return dt1.Second < dt2.Second; }

	return FALSE;
}

