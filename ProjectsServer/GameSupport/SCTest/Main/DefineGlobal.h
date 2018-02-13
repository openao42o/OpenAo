///////////////////////////////////////////////////////////////////////////////
//  DefineGlobal.h : 프로젝트와 상관없이 기본적으로 적용되는 #define
//
//  Date	: 2004-03-17 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#ifndef _DEFINE_GLOBAL_H_
#define _DEFINE_GLOBAL_H_

#include "DebugAssert.h"
#include <math.h>

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

#define RANDF1(min, max)	((float)((min)*10 + (rand32() % (int)(((max)-(min)+0.1)*10)))/10)		// float, 소수점 이하 1자리
#define RANDF2(min, max)	((float)((min)*100 + (rand32() % (int)(((max)-(min)+0.01)*100)))/100)	// float, 소수점 이하 2자리
#define RANDI(min, max)		((min) + (rand32() % ((max)-(min)+1)))									// int
#define RAND100()			(rand32() % 100)														// int, 0 ~ 99
#define RAND1000()			(rand32() % 1000)														// int, 0 ~ 999
#define RAND10K()			(rand32() % 10000)														// int, 0 ~ 9999
#define RAND100K()			(rand32() % 100000)														// int, 0 ~ 99999
#define RAND256()			(rand32() % 256)														// int, 0 ~ 255

#define util::in_range(_MIN, _VALUE, _MAX)		((_MIN) <= (_VALUE) && (_MAX) >= (_VALUE))

#define util::length(_X, _Y)						sqrt((double)((_X)*(_X)+(_Y)*(_Y)))

#define MAX_INT64_VALUE		((INT64)0x7FFFFFFFFFFFFFFF)
#define MAX_UINT64_VALUE	((UINT64)0xFFFFFFFFFFFFFFFF)
#define MAX_INT_VALUE		((INT)0x7FFFFFFF)
#define MAX_UINT_VALUE		((UINT)0x7FFFFFFF)
#define MAX_SHORT_VALUE		((SHORT)0x7FFF)
#define MAX_USHORT_VALUE	((USHORT)0xFFFF)
#define MAX_BYTE_VALUE		((BYTE)0x7F)

#define util::del(p) { if(NULL != p){ delete (p); (p) = NULL;}}
#define util::del_array(p) { if(NULL != p){ delete[] (p); (p) = NULL;}}

#define IS_VALID_ARRAY_INDEX(idx, ArraySize) (((idx) >= 0) && ((idx) < (ArraySize)))
#define util::zero(p, Size) (memset((p), 0x00, (Size)))


#endif	// _DEFINE_GLOBAL_H_endif