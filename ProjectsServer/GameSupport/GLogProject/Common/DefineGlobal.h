#pragma once

#include <math.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
/// 프로토콜 기술을 위해 사용, c++ code로서의 의미는 없다
#define ARRAY_(x)			/// x가 n개 있는 것
#define STRUCT_(x)			/// x라는 struct 하나

#define LODWORD(ll)			((DWORD)(ll))
#define HIDWORD(ll)			((DWORD)(((UINT64)(ll) >> 32) & 0xFFFFFFFF))

// RAND_MAX는 0x7FFF이므로 그 이상이 필요할 때는 이놈을 사용하기, 20040708, kelovon
// inline int rand32()
// {
	// int i1, i2, i3;
	// i1 = rand()&0x0FFF;
	// i2 = (rand()&0x0FFF) << 12;
	// i3 = (rand()&0x007F) << 24;
	// return (i1|i2|i3);
// }

#define RANDF1(min, max)	((float)((min)*10 + (rand32() % (int)(((max)-(min)+0.1)*10)))/10)		// float, 소수점 이하 1자리, (min) ~ (max), [min, max], 주의: max값 포함됨
#define RANDF2(min, max)	((float)((min)*100 + (rand32() % (int)(((max)-(min)+0.01)*100)))/100)	// float, 소수점 이하 2자리, (min) ~ (max), [min, max], 주의: max값 포함됨
#define RANDI(min, max)		((min) + (rand32() % ((max)-(min)+1)))			// int, (min) ~ (max), [min, max], 주의: max값 포함됨
#define RAND100()			(rand32() % 100)								// int, 0 ~ 99
#define RAND1000()			(rand32() % 1000)								// int, 0 ~ 999
#define RAND10K()			(rand32() % 10000)								// int, 0 ~ 9999
#define RAND100K()			(rand32() % 100000)								// int, 0 ~ 99999
#define RAND1000K()			(rand32() % 1000000)							// int, 0 ~ 999999
#define RAND256()			(rand32() % 256)								// int, 0 ~ 255

#define util::in_range(_MIN, _VALUE, _MAX)		((_MIN) <= (_VALUE) && (_MAX) >= (_VALUE))

#define util::length(_X, _Y)						sqrt((double)((_X)*(_X)+(_Y)*(_Y)))

#define MAX_INT64_VALUE			((INT64)0x7FFFFFFFFFFFFFFF)
#define MAX_UINT64_VALUE		((UINT64)0xFFFFFFFFFFFFFFFF)
#define MAX_INT_VALUE			((INT)0x7FFFFFFF)
#define MAX_UINT_VALUE			((UINT)0x7FFFFFFF)
#define MAX_SHORT_VALUE			((SHORT)0x7FFF)
#define MAX_USHORT_VALUE		((USHORT)0xFFFF)
#define MAX_BYTE_VALUE			((BYTE)0x7F)
#define MAX_RAND100_VALUE		99				// int, 0 ~ 99
#define MAX_RAND1000_VALUE		999				// int, 0 ~ 999
#define MAX_RAND10K_VALUE		9999			// int, 0 ~ 9999
#define MAX_RAND100K_VALUE		99999			// int, 0 ~ 99999
#define MAX_RAND1000K_VALUE		999999			// int, 0 ~ 999999
#define MAX_RAND256_VALUE		255				// int, 0 ~ 255

#define util::del(p) { if(NULL != p){ delete (p); (p) = NULL;}}
#define util::del_array(p) { if(NULL != p){ delete[] (p); (p) = NULL;}}

#define IS_VALID_ARRAY_INDEX(idx, ArraySize) (((idx) >= 0) && ((idx) < (ArraySize)))
#define util::zero(p, Size) (memset((p), 0x00, (Size)))

#define LSTRNCPY(pDest, pSource) { if((NULL != pSource)&&(NULL != pDest)) { lstrcpy(pDest,pSource);}}
#define LSTRNCPY_OLEDB(pDest, pSource) { if(VT_NULL != pSource.vt) { lstrcpy(pDest,(TCHAR*)((_bstr_t)pSource));}}

///////////////////////////////////////////////////////////////////////////////
// 2006-05-24 by cmkwon, szDest를 초기화 하고 복사한다.
#define util::strncpy(szDest, szSource, MaxSize)		{ util::zero((szDest), (MaxSize));	\
														  strncpy( (szDest), (szSource), (MaxSize)-1 );}
#define STRNCPY_S_MEMSET(szDest, szSource, MaxSize)		{ util::zero((szDest), (MaxSize));	\
															strncpy_s( (szDest), _countof(szDest), (szSource), (MaxSize)-1 );}

#define GET_CASERETURN_STRING_BY_TYPE(str)	case str:		return #str ""


struct Delete_Object
{
	template<typename T>
		void operator()(T *ptr)
	{
		delete ptr;
	}
};

struct Delete_Object_MAP
{
	template<typename T>
		void operator()(T ptr)
	{
		delete ptr.second;
	}
};


#ifndef chBEGINTHREADEX
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
   pvParam, fdwCreate, pdwThreadID)                 \
      ((HANDLE) _beginthreadex(                     \
         (void *) (psa),                            \
         (unsigned) (cbStack),                      \
         (PTHREAD_START) (pfnStartAddr),            \
         (void *) (pvParam),                        \
         (unsigned) (fdwCreate),                    \
         (unsigned *) (pdwThreadID)))
#endif


///////////////////////////////////////////////////////////////////////////////
// 2009-04-22 by dhjin - Null값 체크, Null 존재시 True리턴
template <typename T_1>
bool IsNullCheck(T_1 * ptr_1)
{
	if(NULL == ptr_1)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2)
{
	if(NULL == ptr_1 || NULL == ptr_2)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2, typename T_3>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2, T_3 * ptr_3)
{
	if(NULL == ptr_1 || NULL == ptr_2 || NULL == ptr_3)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2, typename T_3, typename T_4>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2, T_3 * ptr_3, T_4 * ptr_4)
{
	if(NULL == ptr_1 || NULL == ptr_2 || NULL == ptr_3 || NULL == ptr_4)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2, typename T_3, typename T_4, typename T_5>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2, T_3 * ptr_3, T_4 * ptr_4, T_5 * ptr_5)
{
	if(NULL == ptr_1 || NULL == ptr_2 || NULL == ptr_3 || NULL == ptr_4 || NULL == ptr_5)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2, T_3 * ptr_3, T_4 * ptr_4, T_5 * ptr_5, T_6 * ptr_6)
{
	if(NULL == ptr_1 || NULL == ptr_2 || NULL == ptr_3 || NULL == ptr_4 || NULL == ptr_5 || NULL == ptr_6)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6, typename T_7>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2, T_3 * ptr_3, T_4 * ptr_4, T_5 * ptr_5, T_6 * ptr_6, T_7 * ptr_7)
{
	if(NULL == ptr_1 || NULL == ptr_2 || NULL == ptr_3 || NULL == ptr_4 || NULL == ptr_5 || NULL == ptr_6 || NULL == ptr_7)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6, typename T_7, typename T_8>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2, T_3 * ptr_3, T_4 * ptr_4, T_5 * ptr_5, T_6 * ptr_6, T_7 * ptr_7, T_8 * ptr_8)
{
	if(NULL == ptr_1 || NULL == ptr_2 || NULL == ptr_3 || NULL == ptr_4 || NULL == ptr_5 || NULL == ptr_6 || NULL == ptr_7 || NULL == ptr_8)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6, typename T_7, typename T_8, typename T_9>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2, T_3 * ptr_3, T_4 * ptr_4, T_5 * ptr_5, T_6 * ptr_6, T_7 * ptr_7, T_8 * ptr_8, T_9 * ptr_9)
{
	if(NULL == ptr_1 || NULL == ptr_2 || NULL == ptr_3 || NULL == ptr_4 || NULL == ptr_5 || NULL == ptr_6 || NULL == ptr_7 || NULL == ptr_8 || NULL == ptr_9)
	{
		return TRUE;
	}
	return FALSE;
}
template <typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6, typename T_7, typename T_8, typename T_9, typename T_10>
bool IsNullCheck(T_1 * ptr_1, T_2 * ptr_2, T_3 * ptr_3, T_4 * ptr_4, T_5 * ptr_5, T_6 * ptr_6, T_7 * ptr_7, T_8 * ptr_8, T_9 * ptr_9, T_10 * ptr_10)
{
	if(NULL == ptr_1 || NULL == ptr_2 || NULL == ptr_3 || NULL == ptr_4 || NULL == ptr_5 || NULL == ptr_6 || NULL == ptr_7 || NULL == ptr_8 || NULL == ptr_9 || NULL == ptr_10)
	{
		return TRUE;
	}
	return FALSE;
}


