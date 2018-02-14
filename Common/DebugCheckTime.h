#ifndef COMMON_DEBUGCHECKTIME_H_
#define COMMON_DEBUGCHECKTIME_H_


#define CHECK_TIME_INIT(size)								\
	__int64 freq, tmpFre[size];								\
	int		tmpFreGap[size];								\
	float	ftmpTime[size];									\
	memset(tmpFre, 0x00, sizeof(__int64)*size);				\
	memset(tmpFreGap, 0x00, sizeof(int)*size);			\
	memset(ftmpTime, 0x00, sizeof(float)*size);			\
	QueryPerformanceFrequency((_LARGE_INTEGER*)&freq)


#define CHECK_TIME_SET(index)		QueryPerformanceCounter((_LARGE_INTEGER*)&tmpFre[index]);

#define CHECK_TIME_CALC(index)								\
	tmpFreGap[index]	= tmpFre[index+1] - tmpFre[index];	\
	ftmpTime[index]		= (float)((double)tmpFreGap[index]/freq*1000)

#define CHECK_TIME_TOTAL_CALC(index)						\
	tmpFreGap[index]	= tmpFre[index] - tmpFre[0];		\
	ftmpTime[index]		= (float)((double)tmpFreGap[index]/freq*1000)



#endif // COMMON_DEBUGCHECKTIME_H_
