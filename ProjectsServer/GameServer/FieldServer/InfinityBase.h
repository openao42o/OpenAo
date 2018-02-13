/// InfinityBase.h: interface for the CInfinityBase class.
/// \brief		인피니티 - 인피니티 클래스 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYBASE_H__679CAEA9_97E4_46FF_AD8F_2808F8C05E3E__INCLUDED_)
#define AFX_INFINITYBASE_H__679CAEA9_97E4_46FF_AD8F_2808F8C05E3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCPSocket;
class CFieldMapChannel;
typedef vector<CFieldIOCPSocket*>		VectCFISoc;
typedef mt_vector<CFieldIOCPSocket*>	mtVectCFISoc;
struct KEY_MONSTER_INFO {
	MonIdx_t		KeyMonsterIdx;
	BOOL			KeyMonsterAlive;
	SummonCnt_t		KeyMonsterCount;		// 2010-03-31 by dhjin, 인피니티(기지방어) -
	CinemaOrder_t	KeyMonsterOrder;		// 2010-03-31 by dhjin, 인피니티(기지방어) -
};
typedef vector<KEY_MONSTER_INFO>				VectKeyMonsterInfo;

struct TENDER_USER_INFO {
	ClientIndex_t	UserClientIdx;
	UINT			ItemFieldIndex;
	DiceCnt_t		DiceCount;
	BOOL			GiveUp;
};
typedef vector<TENDER_USER_INFO>				VectTenderUserInfo;

///////////////////////////////////////////////////////////////////////////////
// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
struct BIDDER_INFO
{
	ClientIndex_t	UserClientIdx;
	DiceCnt_t		DiceCount;		// 주사위 카운트(0: 굴리지 않은 상태, 1~100: 굴린 상태의 값, 같은 주사위 값을 굴린 유저는 없도록 처리)
	BOOL			GiveUp;			// 입찰 포기 여부
};
typedef vector<BIDDER_INFO>				vectBIDDER_INFO;

class CTenderItemInfo
{
public:
	CTenderItemInfo();
	CTenderItemInfo(const CTenderItemInfo & i_tenderingInfo);
	
	BOOL CheckAllUserChoice(VectCFISoc *i_pUserList);
	BIDDER_INFO *GetBidderInfo(ClientIndex_t i_cliIdx);
	Err_t PlayBidding(DiceCnt_t *o_pDiceResult, ClientIndex_t i_cliIdx, BOOL i_bGiveUp);
	BOOL IsExistDiceValue(DiceCnt_t i_nDiceValue);
	BIDDER_INFO *FindSuccessfulBidder(void);
	BOOL PushBidder(BIDDER_INFO *i_pBidderInfo);
public:
	UINT				m_ItemFieldIndex;
	//DWORD				m_TenderStartTick;
	ATUM_DATE_TIME		m_TenderStartTime;
	D3DXVECTOR3			m_TenderItemPosition;
	ItemNum_t			m_TenderItemNum;
	INT					m_TenderItemCount;
	BOOL				m_IsTimeOver;			// TRUE: 입찰이 시간 경과로 처리됨, FALSE: 모든 유저가 입찰 혹은 포기를 선택함으로 처리됨
	vectBIDDER_INFO		m_BidderList;			// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
};
typedef mt_vector<CTenderItemInfo>						mtvectTenderItemInfo;


class CFieldIOCP;		// 2010-03-26 by cmkwon, 인피2차 수정 - CFieldIOCP 관리구조 추가
class CInfinityBase  
{
	friend class CAtumLogSender;				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
public:
	CInfinityBase();
	virtual ~CInfinityBase();

	void InitInfinityBase();
	void DeleteNullFISocPtrNoLock();		// CFieldIOCPSocket NULL 포인터를 삭제한다. 단 Lock을 잡지 않으므로 함수 호출 상위에서 잡아야한다.!
	void SetInfinityCreateUID(InfinityCreateUID_t i_nInfinityCreateUID);
	void InitMasterPlayer(CFieldIOCPSocket * i_pMasterPlayerSoc);
	void SetModeInfo(INFINITY_MODEINFO * i_pInfinityModeInfo);
	void SetCinemaInfo(vectorCinemaInfo * i_pVectCinemaInfo);
	void SetKeyMonsterList();
	BOOL SetKeyMonster_DeadForNextStep(KEY_MONSTER_INFO * i_pKeyMonsterInfo);	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	void SetKeyMonster_AliveForGameClear(KEY_MONSTER_INFO * i_pKeyMonsterInfo);	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	void CreateKeyMonster_DeadForNextStep(MonIdx_t i_CreateMonsterIdx);
	void CreateKeyMonster_AliveForGameClear(MonIdx_t i_CreateMonsterIdx);		// 2010-03-31 by dhjin, 인피니티(기지방어) -

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	void DeleteKeyMonster_DeadForNextStep(BOOL *o_pbCompensationFlag, MonIdx_t i_DeadMonsterIdx);

	void DeleteKeyMonster_AliveForGameClear(MonIdx_t i_DeadMonsterIdx);			// 2010-03-31 by dhjin, 인피니티(기지방어) -
	BOOL CheckEmptyMonster_DeadForNextStepAlive();
	BOOL CheckEmptyMonster_AliveForGameClearAlive();									// 2010-03-31 by dhjin, 인피니티(기지방어) -
	BOOL CheckKeyMonster_DeadForNextStepAlive();								// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 살아있는 키몬스터가 있는지 체크한다.	
	BOOL CheckKeyMonster_AliveForGameClearAlive();									// 2010-03-31 by dhjin, 인피니티(기지방어) -
	void SetInfinityTeamName(CHAR * i_pTeamName);
	void GetPlayerList(VectCFISoc * o_pVectPlayer, ClientIndex_t * o_pMasterUserClientIdx=NULL);	// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
	INT GetPlayerListSize();
	INT	GetMaxPlayerSize();
	CFieldIOCPSocket * GetMasterPlayer();
	ClientIndex_t GetMasterPlayerClientIdx();
	CHAR * GetMasterPlayerName();
	InfinityCreateUID_t GetInfinityCreateUID();
	eINFINITY_MODE GetInfinityMode();
	CHAR * GetInfinityTeamName();
	eINFINITY_STATE GetInfinityState();
	void SetInfinityState(eINFINITY_STATE i_infiState);		// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	BYTE GetInfluenceType();
	MapIndex_t GetInfinityMapIndex();
	ChannelIndex_t GetInfinityChannelIndex();
	Err_t JoinCheck(CFieldIOCPSocket * i_pJoinRequestPlayerSoc);		// 가입 가능한 유저인지 체크
	Err_t Join(CFieldIOCPSocket * i_pJoinPlayerSoc);					// 유저 가입
	Err_t ChangeMasterUser(CFieldIOCPSocket * i_pChangeMasterUserFISoc);
	Err_t Leave(CFieldIOCPSocket * i_pLeavePlayerSoc);					// 유저 탈퇴
	Err_t Ban(CFieldIOCPSocket * i_pBanPlayerSoc);						// 유저 추방
	Err_t Start(CFieldIOCPSocket * i_pMasterPlayerSoc, CFieldMapChannel *i_pFMChann);					// 시작
	Err_t CreateMap(CFieldMapChannel * i_pInfinityMapChannel);

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	Err_t ChangeDifficultyLevel ( CFieldIOCPSocket * i_pMasterPlayerSoc , const INT a_ChangeDifficultyLevel );			// 방의 난이도 변경. 2010. 05. 24. by hsLee.

	BOOL SetDifficultyLevel ( INT DifficultyLevel );																	// 방의 난이도 값 대입. 2010. 05. 25. by hsLee.

	inline INT GetDifficultyLevel ( void ) { return m_InfinityDifficultyLevel; }										// 방의 난이도 값 리턴.	2010. 05. 25. by hsLee.
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	Err_t SkipEndingCinema ( CFieldIOCPSocket * i_pPlayerSoc , const bool a_bNormalEnding = false );															// 인피니티 엔딩 연출 스킵 처리.
	// End 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

	// 2010. 06. 04 by hsLee 인피니티 필드 2차 난이도 조절. (서버 - 키 몬스터 리스트 등록 정보의 소환 카운트에도 밸런스 적용.)
	void SetUpdate_KeyMonsterSummonCount( void );																		// 방의 등록된 키 몬스터 소환 Count 횟수 난이도 적용. 
	// End 2010. 06. 04 by hsLee 인피니티 필드 2차 난이도 조절. (서버 - 키 몬스터 리스트 등록 정보의 소환 카운트에도 밸런스 적용.)

	void Fin(BOOL i_bSuccess, INFINITY_FIN_t i_finType);			// 종료
	BOOL CheckDestory();				// 파괴 체크
	BOOL CheckAllReady();
	
	BOOL CheckTenderItemSize();			// Tender Item이 있는지.
	Err_t ChoiceTenderItem(DiceCnt_t *o_pDiceResult, ClientIndex_t i_UserClientIdx, UINT i_ItemFieldIndex, BOOL i_bGiveUp);	// 유저 Tender
	void CalcLimitTimeByUserDeath(char * i_szCharacterName);		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 죽은 유저 이름 정보 전송 추가,	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	void CalcAliveForGameClearMonsterHPByUserDeath(char * i_szCharacterName);		// 2010-03-31 by dhjin, 인피니티(기지방어) -

	void LogInfinityFinAliveKeyMonster();										// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 유저 아이템 정보 제외 종료 관련 로그 여러번 남는 버그 한 번만 남게 수정	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
	void LogInfinityFinUser(CFieldIOCPSocket * i_pPlayerSoc, BOOL i_bClear);	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 유저 아이템 정보 제외 종료 관련 로그 여러번 남는 버그 한 번만 남게 수정 // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
	void LogInfinityLeave(CFieldIOCPSocket * i_pPlayerSoc);							// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보 

	void SendInfinityTeamChat(BYTE * i_pDATA, int i_nSize);							// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
	
	void ReStartDisConnectUser(CFieldIOCPSocket * i_pUserFISoc);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리 고고씽
	void SetDeleteCinemaInfo(CINEMAINFO * i_pDeleteCinemaInfo);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	void SendDeleteCinemaInfoList(CFieldIOCPSocket * i_pReStartPlayerSoc);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리

	void AutoPartyCreate();						// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 
	void SendAddDataPlayerList(BYTE* pData, int nSize);		// 2010-03-31 by dhjin, 인피니티(기지방어) -
	void ProcessingCinemaFinSuccess();		// 2010-03-31 by dhjin, 인피니티(기지방어) - 성공 종료 연출 진행
	void ProcessingCinemaFinFail();			// 2010-03-31 by dhjin, 인피니티(기지방어) - 실패 종료 연출 진행
	BOOL DoSecondlyWorkInfinityBaseIsContinue();		// 2010-03-31 by dhjin, 인피니티(기지방어) -

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
	Err_t SendFtoA_INFINITY_START_CHECK(void);
	BOOL CheckInfinityStartCheckAck(eINFINITY_STATE *o_pInfiRoomState, MSG_FtoA_INFINITY_START_CHECK_ACK *i_pStartCheckAck);
	BOOL CheckInfinityAllPlayerState(eINFINITY_STATE i_infiState);
	CFieldIOCPSocket *GetPlayerInVectPlayerByClientIndex(ClientIndex_t i_cliIdx);
	void SetAllPlayerState(eINFINITY_STATE i_infiState);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-26 by cmkwon, 인피2차 수정 - CFieldIOCP 관리구조 추가
	static CFieldIOCP		*ms_pFieldIOCP4;

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	BOOL ImputeInfinity(void);

	///////////////////////////////////////////////////////////////////////////////	
	// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
	int GetPlayerStateCount(eINFINITY_STATE i_infiState);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	KEY_MONSTER_INFO *GetKeyMonsterInfoFromKeyMonsterDeadForNextStep(MonIdx_t i_MonNum);
	BOOL DeleteKeyMonsterInfoFromKeyMonsterDeadForNextStep(MonIdx_t i_MonNum);

	CTenderItemInfo *GetTenderItemInfoNoLock(UINT i_ItemFieldIndex);
	BOOL PickupTenderItem(CTenderItemInfo *i_pTenderItemInfo, VectCFISoc *i_pPlayerList);
	BOOL CancelTenderItemInfo(CTenderItemInfo *i_pTenderItemInfo);
	Err_t PushTenderItem(CTenderItemInfo *i_pTenderItemInfo);
	void TenderProcessing(void);

	// 등록되어 있는 입찰 아이템들의 입찰 시작 시간을 일괄 갱신.
	void UpdateTenderItemInfoAll_TenderStartTick ( ATUM_DATE_TIME sUpdateTime );

	// start 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
	eINFINITY_STATE GetInfinityStatePlayingNext(eINFINITY_STATE Now);		// 다음 Playing Step
	// end 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현

	void MoveAllPlayer(D3DXVECTOR3 vecPostion);		// 2011-06-01 인피니티 3차 스텝 7 - 캐릭터 위치 이동

	BOOL CheckInfinityMinAdmission();		// 2012-07-17 by hskim, 인피니티 인원수 부족할대 START 할경우 예외 처리

protected:
	// start 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 
	void EnablePenaltyApply();			// 패널티 적용
	void DisablePenaltyApply();			// 패널티 적용 금지
	// end 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 
	
	BOOL IsPenaltyApply();
	// end 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 

public :	// Virtual Process.

	// 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )
	virtual void ProcessingCinema ( CFieldIOCPSocket * i_pMasterFISoc , int nUpdateScene = 1 ) {}

	// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	virtual void ResetValue_SkipEndingCinema ( void );						// 시네마 스킵 관련 변수 초기화.
	virtual bool SetValue_SkipEndingCinema ( const CHAR *pszRqUserName , const bool a_bNormalEnding );			// 시네마 스킵 활성화 & 요청 유저정보 저장.
	// End 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

	// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
	BOOL SendInfinityItemTender( CFieldIOCPSocket* i_pBidderFISoc, DiceCnt_t i_DiceCount, INT i_ItemNum );
	// END 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장 

	void CheckInfinityPartyState();		// 2013-05-16 by jhseol, 인피 재접속 시 파티지원

protected:
	mtVectCFISoc			m_mtVectPlayer;
	CFieldIOCPSocket	*	m_MasterPlayerSoc;
	CFieldMapChannel	*	m_pInfinityMapChannel;
	VectKeyMonsterInfo		m_KeyMonsterDeadForNextStepList;	// 2010-03-31 by dhjin, 인피니티(기지방어) - 이름변경 m_KeyMonsterList -> m_KeyMonsterDeadForNextStepList
	KEY_MONSTER_INFO		m_KeyMonsterAliveForGameClear;	// 2010-03-31 by dhjin, 인피니티(기지방어) - 해당 몬스터가 다 죽으면 게임 오버~

	BYTE					m_InfluenceType;
	eINFINITY_STATE			m_eInfinityState;
	INFINITY_MODEINFO		m_InfinityModeInfo;
	vectorCinemaInfo		m_CinemaInfoList;

	// start 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현
	vectorCinemaInfo		m_CinemaStepInfoList[CINEMA_PLAYING_STEP_MAX];

	DWORD					m_InfinityStepStartTick;	// 2011-04-11 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현 - 현재 스텝의 시작 시간
	INT						m_PlayingStepCount;			// 2011-04-11 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현 - 현재 진행중인 Playing Step
	// end 2011-04-08 by hskim, 인피니티 3차 - 시네마 단계별 기능 구현

	InfinityCreateUID_t		m_nInfinityCreateUID;
	CHAR					m_szTeamName[SIZE_MAX_PARTY_NAME];
	DWORD					m_InfinityCreatedTick;
	ATUM_DATE_TIME			m_InfinityCreatedTime;
	CinemaOrder_t			m_CinemaOrder;
	DWORD					m_dwForcedTick;
	mtvectTenderItemInfo	m_mtTenderItemList;
	VectTenderUserInfo		m_TenderUserInfoList;
	CRITICAL_SECTION		m_criticalLimitTime;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	MSec_t					m_InfinityLimitTick;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	vectorCinemaInfo		m_DeleteCinemaInfoList;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	vectorCinemaInfo		m_FinSuccessCinemaInfoList; // 2010-03-31 by dhjin, 인피니티(기지방어) -
	vectorCinemaInfo		m_FinFailCinemaInfoList;	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	DWORD					m_dwTickChangedInfinityState;	// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	INT						m_InfinityDifficultyLevel;		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 방의 난이도 레벨값.
	MONSTER_BALANCE_DATA	m_InfinityDifficultyData;		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 방의 난이도 정보.
	eINFINITY_SKIP_CINEMA	m_eCinemaSkipState;											// 시네마 연출 스킵 활성화값. 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	CHAR					m_szRqSkipEndingCinema_UserName[SIZE_MAX_CHARACTER_NAME];	// 시네마 연출 스킵 요청한 유저 이름. 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	ATUM_DATE_TIME			m_sCinemaSkipTime;											// 시메나 연출 스킵된 시각.

	BOOL					m_bPenaltyApply;		// 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 
};

#endif // !defined(AFX_INFINITYBASE_H__679CAEA9_97E4_46FF_AD8F_2808F8C05E3E__INCLUDED_)
