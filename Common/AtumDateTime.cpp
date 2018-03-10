//Copyright[2002] MasangSoft
#include "StdAfx.h"
#include "AtumDateTime.h"
#include "AtumParam.h"
//Copyright [2002] MasangSoft
// 2008-04-02 by cmkwon, 썸머타임 관련 버그 수정 - 
bool ATUM_DATE_TIME::ms_bSettm_isdst = false;    // 초기화
int ATUM_DATE_TIME::ms_tm_isdst = 0;        // 초기화


ATUM_DATE_TIME ATUM_DATE_TIME::GetCurrentDateTime()
{
    return ATUM_DATE_TIME(true);
}

STRNBUF ATUM_DATE_TIME::GetCurrentDateTimeString()
{
    return ATUM_DATE_TIME(true).GetDateTimeString();
}

const char* ATUM_DATE_TIME::GetDateTimeStringFromSeconds(int i_nSeconds, string &i_szStrBuf)
{
    char buf[512];

    int Day = (int)i_nSeconds / (24 * 3600);
    int Hour = ((int)i_nSeconds % (24 * 3600)) / 3600;
    int Minute = ((int)i_nSeconds % 3600) / 60;
    int Second = ((int)i_nSeconds % 60);
    sprintf(buf, STRCMD_071005_0000);
    i_szStrBuf = buf;
    return i_szStrBuf.c_str();
}

// todo : make these functions really safe

const char* ATUM_DATE_TIME::GetLocalString_YYYYMMDD(int i_Year, int i_Month, int i_Day, string &io_szStrBuf)
{
    if (i_Year == 0 || i_Month == 0 || i_Day == 0) return (io_szStrBuf = "Invalid Date").c_str();

    char buf[512];        util::zero(buf, 512);

    ATUM_DATE_TIME atCur { true };

    atCur.Year = i_Year;
    atCur.Month = i_Month;
    atCur.Day = i_Day;

    auto tmTM = tm(atCur);

    strftime(buf, 512, "%B-%d-%Y", &tmTM);

    io_szStrBuf = buf;
    return io_szStrBuf.c_str();
}


const char* ATUM_DATE_TIME::GetLocalString_YYYYMM(int i_Year, int i_Month, string &io_szStrBuf)
{
    if (i_Year == 0 || i_Month == 0) return (io_szStrBuf = "Invalid Date").c_str();

    char buf[512];        util::zero(buf, 512);

    ATUM_DATE_TIME atCur { true };

    atCur.Year = i_Year;
    atCur.Month = i_Month;

    auto tmTM = tm(atCur);

    strftime(buf, 512, "%B-%Y", &tmTM);    

    io_szStrBuf = buf;

    return io_szStrBuf.c_str();
}


const char* ATUM_DATE_TIME::GetLocalString_MMDD(int i_Month, int i_Day, string &io_szStrBuf)
{
    if (i_Month == 0 || i_Day == 0) return (io_szStrBuf = "Invalid Date").c_str();

    char buf[512];        util::zero(buf, 512);

    ATUM_DATE_TIME atCur { true };
    atCur.Month = i_Month;
    atCur.Day = i_Day;
    auto tmTM = tm(atCur);
    strftime(buf, 512, "%B-%d", &tmTM);        // 2008-03-27 by cmkwon, 날짜 형식 수정(ex> March-01-2008 20:00:00) - March-10

    io_szStrBuf = buf;
    return io_szStrBuf.c_str();
}


const char* ATUM_DATE_TIME::GetLocalDateTimeString(string &o_szStrBuf, BOOL i_bExcludeSecond/*=FALSE*/) const
{
    char buf[512];
    util::zero(buf, 512);

    auto tmTM = tm(*this);
    if (!i_bExcludeSecond) strftime(buf, 512, "%B-%d-%Y %H:%M:%S", &tmTM);        // 2008-03-27 by cmkwon, (ex> March-01-2008 20:00:00) - March-10-2008 15:30:20
    else strftime(buf, 512, "%B-%d-%Y %H:%M", &tmTM);            // 2008-03-27 by cmkwon, (ex> March-01-2008 20:00:00) - March-10-2008 15:30
    
    o_szStrBuf = buf;
    return o_szStrBuf.c_str();
}

ATUM_DATE_TIME::ATUM_DATE_TIME(time_t rvalue)
{
    auto t = localtime(&rvalue);

    Year = t->tm_year + 1900;
    Month = t->tm_mon + 1;
    Day = t->tm_mday;
    Hour = t->tm_hour;
    Minute = t->tm_min;
    Second = t->tm_sec;
    if (!ms_bSettm_isdst) ms_tm_isdst = t->tm_isdst;
}

void ATUM_DATE_TIME::CheckSettm_isdst()
{
    if (ms_bSettm_isdst) return;

    time_t ltime;
    time(&ltime);
    auto today = localtime(&ltime);
    ms_tm_isdst = today->tm_isdst;
    ms_bSettm_isdst = TRUE;
}

void ATUM_DATE_TIME::SetCurrentDateTime(BOOL i_bSetSecondZero/*=FALSE*/)
{
    time_t ltime;
    time(&ltime);
    auto today = localtime(&ltime);

    Year = today->tm_year + 1900;
    Month = today->tm_mon + 1;
    Day = today->tm_mday;
    Hour = today->tm_hour;
    Minute = today->tm_min;
    Second = today->tm_sec;
    if (i_bSetSecondZero)
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
    Year = i_usYear;
    Month = i_byMonth;
    Day = i_byDay;
    Hour = i_byHour;
    Minute = i_byMinute;
    Second = i_bySecond;
}

void ATUM_DATE_TIME::AddDateTime(INT i_nYear, INT i_nMonth, INT i_nDay, INT i_nHour/*=0*/, INT i_nMinute/*=0*/, INT i_nSecond/*=0*/)
{
    auto tmpTM = tm(*this);

    tmpTM.tm_year += i_nYear;
    tmpTM.tm_mon += i_nMonth;
    tmpTM.tm_mday += i_nDay;
    tmpTM.tm_hour += i_nHour;
    tmpTM.tm_min += i_nMinute;
    tmpTM.tm_sec += i_nSecond;

    mktime(&tmpTM);

    Year = tmpTM.tm_year + 1900;
    Month = tmpTM.tm_mon + 1;
    Day = tmpTM.tm_mday;
    Hour = tmpTM.tm_hour;
    Minute = tmpTM.tm_min;
    Second = tmpTM.tm_sec;
}

const char* ATUM_DATE_TIME::GetDateTimeString(char *i_szBuffer, int i_nLen, BOOL i_bExcludeSecond/*=FALSE*/) const
{
    if (FALSE == i_bExcludeSecond)
        
        sprintf(i_szBuffer, ATUM_DATE_TIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);

    else sprintf(i_szBuffer, ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND, Year, Month, Day, Hour, Minute);

    return i_szBuffer;
}

const char* ATUM_DATE_TIME::GetSQLDateTimeString(char* i_szBuffer, int i_nLen) const
{
    sprintf(i_szBuffer, SQL_DATETIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);

    return i_szBuffer;
}

//const char* ATUM_DATE_TIME::GetDateTimeString(const STRNBUF& i_strnbuf, BOOL i_bExcludeSecond/*=FALSE*/) const
//{
//    if (i_bExcludeSecond == FALSE)
//        
//        sprintf(i_strnbuf.GetBuffer(), ATUM_DATE_TIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);
//
//    else sprintf(i_strnbuf.GetBuffer(), ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND, Year, Month, Day, Hour, Minute);
//
//    return i_strnbuf.GetBuffer();
//}

STRNBUF ATUM_DATE_TIME::GetDateTimeString(bool i_bExcludeSecond/*=false*/) const
{
    STRNBUF i_strnbuf { SIZE_MAX_ATUM_DATE_TIME_STRING };

    if (i_bExcludeSecond)
        
        sprintf(i_strnbuf.GetBuffer(), ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND, Year, Month, Day, Hour, Minute);

    else sprintf(i_strnbuf.GetBuffer(), ATUM_DATE_TIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);

    return i_strnbuf;
}

const char* ATUM_DATE_TIME::GetSQLDateTimeString(STRNBUF& i_strnbuf) const
{
    sprintf(i_strnbuf.GetBuffer(), SQL_DATETIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);

    return i_strnbuf.GetBuffer();
}

// 2013-05-28 by hskim, 미니 덤프 파일 이름에 시간 추가
const char* ATUM_DATE_TIME::GetFileDateTimeString(STRNBUF& i_strnbuf) const
{
    sprintf(i_strnbuf.GetBuffer(), FILE_DATETIME_STRING_FORMAT, Year, Month, Day, Hour, Minute, Second);

    return i_strnbuf.GetBuffer();
}
// end 2013-05-28 by hskim, 미니 덤프 파일 이름에 시간 추가

int ATUM_DATE_TIME::GetTimeInSeconds() const
{
    auto tmpTime = tm(*this);
    return mktime(&tmpTime);
}

int ATUM_DATE_TIME::GetTimeDiffToCurrentTimeInSeconds() const
{
    time_t CurrentTime;
    return time(&CurrentTime) - GetTimeInSeconds();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            int ATUM_DATE_TIME::GetTimeDiffTimeInSeconds(ATUM_DATE_TIME i_atOP2)
/// \brief        
/// \author        cmkwon
/// \date        2007-02-07 ~ 2007-02-07
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int ATUM_DATE_TIME::GetTimeDiffTimeInSeconds(ATUM_DATE_TIME i_atOP2) const
{
    return this->GetTimeInSeconds() - i_atOP2.GetTimeInSeconds();
}
int ATUM_DATE_TIME::GetTimeDiffTimeInSeconds(int i_atOP2) const
{
    return this->GetTimeInSeconds() - i_atOP2;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            int ATUM_DATE_TIME::GetTimeDiffTimeInMinutes(ATUM_DATE_TIME i_atOP2)
/// \brief        
/// \author        cmkwon
/// \date        2007-02-07 ~ 2007-02-07
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int ATUM_DATE_TIME::GetTimeDiffTimeInMinutes(ATUM_DATE_TIME i_atOP2) const
{
    return this->GetTimeDiffTimeInSeconds(i_atOP2) / 60;    // 2007-02-07 by cmkwon, 초를 분단위로 변경
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            int ATUM_DATE_TIME::GetTimeDiffTimeInHours(ATUM_DATE_TIME i_atOP2)
/// \brief        
/// \author        cmkwon
/// \date        2007-02-07 ~ 2007-02-07
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int ATUM_DATE_TIME::GetTimeDiffTimeInHours(ATUM_DATE_TIME i_atOP2) const
{
    return this->GetTimeDiffTimeInMinutes(i_atOP2) / 60;    // 2007-02-07 by cmkwon, 분단위를 시간단위로 변경
}

//ATUM_DATE_TIME& ATUM_DATE_TIME::operator=(const tm& rhs)
//{
//    Year = rhs.tm_year + 1900;
//    Month = rhs.tm_mon + 1;
//    Day = rhs.tm_mday;
//    Hour = rhs.tm_hour;
//    Minute = rhs.tm_min;
//    Second = rhs.tm_sec;
//
//    return *this;
//}

ATUM_DATE_TIME::operator tm() const
{
    tm o_tm;

    o_tm.tm_year = Year - 1900;
    o_tm.tm_mon = Month - 1;
    o_tm.tm_mday = Day;
    o_tm.tm_hour = Hour;
    o_tm.tm_min = Minute;
    o_tm.tm_sec = Second;
    o_tm.tm_wday = 0;
    o_tm.tm_yday = 0;
    o_tm.tm_isdst = ms_tm_isdst;    // 2008-04-02 by cmkwon, DST bug fixes

    return o_tm;
}

#ifdef _ATUM_SERVER
ATUM_DATE_TIME& ATUM_DATE_TIME::operator=(const SQL_TIMESTAMP_STRUCT& rhs)
{
    this->Year = rhs.year;
    this->Month = rhs.month;
    this->Day = rhs.day;
    this->Hour = rhs.hour;
    this->Minute = rhs.minute;
    this->Second = rhs.second;

    return *this;
}

void ATUM_DATE_TIME::Convert(SQL_TIMESTAMP_STRUCT &o_SQL_TIMESTAMP_STRUCT) const
{
    o_SQL_TIMESTAMP_STRUCT.year = Year;
    o_SQL_TIMESTAMP_STRUCT.month = Month;
    o_SQL_TIMESTAMP_STRUCT.day = Day;
    o_SQL_TIMESTAMP_STRUCT.hour = Hour;
    o_SQL_TIMESTAMP_STRUCT.minute = Minute;
    o_SQL_TIMESTAMP_STRUCT.second = Second;
    o_SQL_TIMESTAMP_STRUCT.fraction = 0;
}
#endif

