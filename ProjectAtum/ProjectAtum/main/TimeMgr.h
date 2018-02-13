// 2011-08-12 by hsson 편대 대열에 추가/탈퇴 시 딜레이를 삽입 (편대 동기화 버그를 줄임)

#pragma once
//
// 특정한 시간 마다 1번만 들어오도록 설정하고 싶을 때 사용, 최대 eMAX_TIMER개까지 사용 가능합니다.
// 더 사용하고 싶을 때는 eMAX_TIMER 값만 수정하면 됩니다.
// 
// ex code)
// 사용하는 cpp or h 에서 #include "TimeMgr.h" 한 후 다음 코드를 삽입
// if( g_cTimeMgr.CheckTime( 1, 1000 ) )
// {
// 	_asm nop
// }
// 
// 1번 타이머를 사용하고 1초에 1번만 if문을 통과 후 _asm nop에 도달 합니다.
// 단, 타이머 번호의 중복사용은 특수한 용도 이외에는 사용하지 않는것을 권장합니다.
//

#define g_cTimeMgr CTimeMgr::GetInstance()

class CTimeMgr
{
private:
	enum { eMAX_TIMER = 30, };
	DWORD m_bCur[eMAX_TIMER];

public:
	CTimeMgr()
	{
		for( int i = 0; i < eMAX_TIMER; ++i )
			m_bCur[i] = timeGetTime();
	}
	
	static CTimeMgr& GetInstance()
	{
		static CTimeMgr Instance;
		return Instance;
	}

	bool CheckTime( const int& _nIndex, const int& _nDelay )
	{
		if( eMAX_TIMER <= _nIndex || 0 > _nIndex )
			return false;

		if( m_bCur[_nIndex] + _nDelay < timeGetTime() )
		{
			m_bCur[_nIndex] = timeGetTime();
			return true;
		}

		return false;
	}
};


// end 2011-08-12 by hsson 편대 대열에 추가/탈퇴 시 딜레이를 삽입 (편대 동기화 버그를 줄임)