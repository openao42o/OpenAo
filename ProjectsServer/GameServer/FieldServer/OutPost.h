// OutPost.h: interface for the COutPost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPOST_H__EB5C6A5B_8BA0_46C4_9826_119C8413C7A4__INCLUDED_)
#define AFX_OUTPOST_H__EB5C6A5B_8BA0_46C4_9826_119C8413C7A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2007-10-16 by cmkwon, 로그 추가 - AtumParam.h 파일로 위치 이동함
//struct SOutPostProtectorDamage
//{
//	BYTE	Influence;
//	UID32_t	GuildUID;
//	CHAR 	GuildName[SIZE_MAX_GUILD_NAME];
//	float	Damage;
//};
//typedef	mt_vector<SOutPostProtectorDamage>		mtvectSOutPostProtectorDamage;

class CFieldIOCP;

class COutPost  
{
public:
	COutPost(SOUTPOST_INFO * i_pSOutPostInfo, CFieldIOCP * i_pFIOCP);
	virtual ~COutPost();

	void	InitResetPossibleValues();		// 각인에 필요한 변수들 초기화 하기
	void    InitProtectorDamage();			// 보호막에 데미지를 준 정보 초기화
	BYTE	GetOutPostState();		// 전진기지 상태 값 가져오기
	void	SetOutPostState(BYTE i_byOutPostState);	// 전진기지 상태 값 변경
	INT		GetOutPostMapIndex();	// 전진기지 맵 번호 가져오기
	INT		GetOutPostCityMapIndex();	// 전진기지 도시 맵 번호 가져오기
	ATUM_DATE_TIME * GetOutPostNextWarTime();	// 전진기지 전쟁 시간 가져오기
	BOOL	GetOutPostNextWarTimeSet();		// 전진기지 다음 전쟁 시간 설정 가능한지 가져오기
	void	SetOutPostNextWarTimeSet(BOOL i_bNextWarTimeSet);		// 전진기지 다음 전쟁 시간 설정
	BYTE	GetOutPostInfluence();		// 전진기지 소유 세력 가져오기
	UID32_t	GetOutPostGuildUID();		// 전진기지 소유 여단UID 가져오기
	CHAR *	GetOutPostGuildName();		// 전진기지 쇼유 여단 명 가져오기
	ATUM_DATE_TIME * GetOutPostResetStartTime();	// 전진기지 각인 시작 시간 가져오기
	ATUM_DATE_TIME * GetOutPostResetEndTime();		// 전진기지 각인 종료 시간 가져오기

	UID32_t GetOutPostResetPossibleGuildUID();		// 전진기지 각인 가능한 여단UID 반환하기
	void	SetOutPostResetPossibleGuildUID(UID32_t i_nGuildUID);	// 전진기지 각인 가능한 여단UID 설정
	BYTE	GetOutPostResetPossibleInfluence();		// 전진기지 각인 가능한 세력 반환하기
	void	SetOutPostResetPossibleInfluence(BYTE i_byInfluence);	// 전진기지 각인 가능한 세력 설정
	CHAR *	GetOutPostResetPossibleGuildName();		// 전진기지 각인 가능한 여단명
	void	SetOutPostResetPossibleGuildName(CHAR * i_szGuildName);	// 전진기지 각인 가능한 여단명
	ATUM_DATE_TIME * GetPotPostGetTime();			// 전진기지 소유 시간 가져오기
	void	SetOutPostGetTime(ATUM_DATE_TIME * i_pCurrentTime);			// 전진기지 소유 시간 설정
	
	INT		CheckOutPostWarTime(ATUM_DATE_TIME * i_pCurrentTime);	// 현재 시간과 전진기지전 시작 시간을 체크하여 남은 시간을 반환한다.
	BOOL	CheckSetOutPostWarDay(ATUM_DATE_TIME * i_pCurrentTime);	// 현재 시간과 전진기지전 설정 날자를 체크하여 설정 날자가 지났으면 전진기지 전쟁 디폴트 설정 날자로 설정한다.

	void	AddDamageOfProtector(BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage);		// 전진기지 보호막에 데미지를 준 여단과 데미지 값을 저장한다.
	UID32_t GetTopGuildOfProtectorDamage(mtvectSOutPostProtectorDamage *o_pvectDamageList);		// 2007-10-16 by cmkwon, 로그 추가 - 인자추가(mtvectSOutPostProtectorDamage *o_pvectDamageList), // 전진기지 보호막에 데미지를 가장 많이 준 여단을 반환한다.
	
	void	SetOutPostResetStartTime(ATUM_DATE_TIME * i_pCurrentTime);				// 전진기지 각인 시간 시작 시간 설정	

	INT		CheckOutPostResetTime(ATUM_DATE_TIME * i_pCurrenTime);		// 현재 시간과 전진기지 각인 시작 시간을 체크하여 차이 시간을 반환한다.
	
	void	SetOutPostPossessGuildAndInfluenceByResetSuccess();		// 전진기지 소유 여단, 세력 설정
	void	SetOutPostPossessNPCByReset();				// 전진기지 NPC소유
	void	SetOutPostPossessInfluenceByReset();				// 전진기지 세력소유

	void	SetOutPostPossessByWarEnd();		// 전진기지 전쟁이 끝나고 소유 및 설정
	void	SetOutPostNextWarTime(ATUM_DATE_TIME *i_pNextWarTime);	// 전진기지 전 다음 시간 설정 (시,분만)
	void	SetOutPostNextWarTimeByChoiceUser(ATUM_DATE_TIME *i_pNextWarTime);	// 전진기지 전 다음 시간 설정 (유저가 시간 설정 시, 주의~!: 전진기지 전쟁이 끝나 직후가 아니므로 OUTPOST_NEXTWARGAP을 더하지 않는다)

	void	OutPostProtectorDestroy();		// 전진기지 보호막 파괴
	void	OutPostResetDestroy();		// 전진기지 각인 파괴

//////////////////////////////////////////////////////////////////////////

	void	SendRemainTimeWarStart();		// 전진기지 시작 전 남은 시간 전송
	void	InitOutPostResetStartTime();		// 전진기지 각인 시간 초기화
	void	SetOutPostPossessInfluence(BYTE i_byInfluence);			// 2008-03-26 by dhjin, 전진기지 소유 세력 저장

	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	void	SetOutPostWarStartTime();		// 전쟁 시작시간을 현재 시간으로 셋팅
	void	InitOutPostInfluence();			// 소유세력 초기화
	void	InitOutPostGuildUID();			// 소유길드 초기화
	void	InitOutPostGuildName();			// 소유길드 초기화
	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템

protected:
	CFieldIOCP		*m_pFieldIOCP19;

	BYTE			m_OutPostState;											// 전진기지 상태 값
	INT				m_OutPostMapIndex;										// 전진기지 맵 번호
	INT				m_OutPostCityMapIndex;									// 전진기지 도시 맵 번호
	ATUM_DATE_TIME	m_OutPostGetTime;										// 전진기지 소유 시간
	ATUM_DATE_TIME	m_OutPostNextWarTime;									// 전진기지 다음 전쟁 시간
	BOOL			m_OutPostNextWarTimeSet;								// 전진기지 다음 전쟁 시간 설정 가능 => 0 : 가능, 1 : 불가
	BYTE			m_OutPostInfluence;										// 전진기지 소유 세력
	UID32_t			m_OutPostGuildUID;										// 전진기지 소유 여단UID
	CHAR			m_OutPostGuildName[SIZE_MAX_GUILD_NAME];				// 전진기지 소유 여단명
	ATUM_DATE_TIME	m_OutPostResetStartTime;								// 전진기지 각인 시작 시간
	ATUM_DATE_TIME	m_OutPostResetEndTime;									// 전진기지 각인 종료 시간
	UID32_t			m_OutPostResetPossibleGuildUID;							// 전진기지 각인 할 수 있는 여단 UID
	BYTE			m_OutPostResetPossibleInfluence;						// 전진기지 각인 할 수 있는 세력
	CHAR			m_OutPostResetPossibleGuildName[SIZE_MAX_GUILD_NAME];	// 전진기지 각인 할 수 있는 여단명

	mtvectSOutPostProtectorDamage	m_mtvectProtectorDamageInfo;		// 전진기지 보호막이 입은 데미지 정보
};

#endif // !defined(AFX_OUTPOST_H__EB5C6A5B_8BA0_46C4_9826_119C8413C7A4__INCLUDED_)
