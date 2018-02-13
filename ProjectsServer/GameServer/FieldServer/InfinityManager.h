/// InfinityManager.h: interface for the CInfinityManager class.
/// \brief		인피니티 - 인피니티 관리 클래스
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYMANAGER_H__9E1738B9_2E9B_4A12_8CDE_FF761AAC14A8__INCLUDED_)
#define AFX_INFINITYMANAGER_H__9E1738B9_2E9B_4A12_8CDE_FF761AAC14A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumFieldDBManager.h"

#include "Cinema.h"
#include "Revision.h"
#include "InfinityMapManager.h"
#include "InfinityBase.h"
#include "InfinityBossrush.h"
#include "InfinityDefence.h"
#include "InfinityMShipBattle.h"			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업

typedef mt_vector<CInfinityBossrush*>	mtvectInfiBossRush;
typedef mt_vector<CInfinityDefence*>	mtvectInfiDefence;
typedef mt_vector<CInfinityMShipBattle*>	mtvectInfiMShipBattle;			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
typedef vector<CFieldIOCPSocket*>   vectCFieldIOCPSocket;

class CInfinityTickManager;
class CInfinityManager
{
public:
	CInfinityManager();
	virtual ~CInfinityManager();

	void InitInfinityManager();
	void SetInfinityMapManagerW(CFieldMapWorkspace * i_pFieldMapWorkspace);
	BOOL SetDBManager(CAtumFieldDBManager * i_pAtumDBManager);
	void SetCinemaInfoW(vectorCinemaInfo * i_pVectCinemaInfo);
	void SetRevisionInfoW(vectorRevisionInfo * i_pVectRevisionInfo);
	void SetDBInfinityModeInfo(vectorInfinityModeInfo * i_pVectInfiModeInfo);
	void SetDBInfinityMonsterInfo(vectorInfinityMonsterInfo * i_pVectInfiMonsterInfo);
	void GetInfinityModeInfo(vectorInfinityModeInfo * o_pCopyVectInfiModeInfo);
	INT MakeMsgInfinityPlayingList(INFINITY_READY_LIST * o_pInfinityPlayingList, MapIndex_t i_nInfinityMapIdx, eINFINITY_MODE i_nInfinityMode, BYTE i_byInfluenceType);
	BOOL CheckInfinityModeLevel(InfiModeUID_t i_nInfinityModeUID, Lv_t i_Lv);

	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
	BOOL CheckEntranceCount(InfiModeUID_t i_nInfinityModeUID, EntranceCount_t i_EntranceCount, int i_nAddLimiteCnt=0);

	BOOL CreateInfinity(MSG_FC_INFINITY_CREATE * i_pInfinityCreateInfo, CFieldIOCPSocket * i_pFISoc, InfinityCreateUID_t * o_pCreateUID, INT * o_nInfinityDifficultyLevel );	
	Err_t JoinCheckInfinity(MSG_FC_INFINITY_JOIN * i_pInfinityJoinRequestInfo, CFieldIOCPSocket * i_pFISoc);		// 인피 가입 전 체크 사항 
	Err_t JoinInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc);		// 인피 가입
	void GetPlayerListW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, vectCFieldIOCPSocket * o_pInfinityMemberList, ClientIndex_t * o_pMasterUserClientIdx);
	Err_t ChangeMasterUserW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pChangeMasterUserFISoc);
	Err_t LeaveInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc);		// 인피 탈퇴
	Err_t BanInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pBanFISoc);		// 인피 추방
	Err_t StartInfinity(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, CFieldIOCPSocket * i_pFISoc);		// 인피 시작
	BOOL GetRevisionInfoW(REVISIONINFO * o_pRevisionInfo, InfiModeUID_t i_InfinityModeUID, INT i_nUnitKind);		// 보정치 가져오기
	Err_t UserMapLoadedComplete(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, MapIndex_t i_MapIndex);		// 인피 맵 생성
	Err_t ChoiceTenderItemW(DiceCnt_t *o_pDiceResult, MSG_FC_INFINITY_TENDER_PUT_IN_TENDER * i_pPutInTenderInfo, ClientIndex_t i_PlayerClientIdx);		// 입찰 ㄱㄱ
	void ProcessingInfinityPenalty(char * i_szCharacterName, eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID);	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 죽은 유저 이름 정보 전송 추가, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	void SendInfinityTeamChatW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, BYTE * i_pDATA, int i_nSize);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
	Err_t ImputeInfinityW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID);		// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 방에 난이도 레벨 변경.
	Err_t ChangeInfinityDifficultyLevel ( const INT i_cst_ChangeDifficultyLevel , eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pMasterUserFISoc );

	// 인피니티 엔딩 시네마 연출 스킵 활성화. // 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	Err_t InfinitySkipEndingCinema ( eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pPlayerFISoc , const bool a_bNormalEnding = false );

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	void SetDisConnectUserInfo(INFINITY_DISCONNECTUSER_INFO * i_pDisConnectUserInfo);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 인피 튕긴 유저일 경우 정보 저장.
	Err_t DisConnectUserReStart(char * i_DisConnectUserName, INFINITY_PLAYING_INFO * o_pInfinityPlayingInfo);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 인피 튕긴 유저 시작 처리
	Err_t CheckIsCreateInfinityUID(INFINITY_PLAYING_INFO * o_pInfinityPlayingInfo);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	BOOL DeleteDisConnectUserInfo(char * i_DisConnectUserName);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 인피 튕긴 유저일 경우 정보 삭제.
	void ReStartDisConnectUserW(InfinityCreateUID_t i_nInfinityCreateUID, eINFINITY_MODE i_eInfiMode, CFieldIOCPSocket * i_pUserFISoc);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리 고고씽

	// Tick관련
	void StartTick();
	void DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime);


	void CreateKeyMonster_DeadForNextStepW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_CreateMonsterIdx);

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	void DeleteKeyMonster_DeadForNextStepW(BOOL *o_pbCompensationFlag, MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_DeadMonsterIdx);

	void CreateKeyMonster_AliveForGameClearW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_CreateMonsterIdx);
	void DeleteKeyMonster_AliveForGameClearW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, MonIdx_t i_DeadMonsterIdx);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
	Err_t SendFtoA_INFINITY_START_CHECK_W(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID);
	CFieldIOCPSocket *GetMasterPlayerW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID);
	BOOL CheckInfinityStartCheckAckW(eINFINITY_STATE *o_pInfiRoomState, MSG_FtoA_INFINITY_START_CHECK_ACK *i_pStartCheckAck);
	void SetAllPlayerStateW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, eINFINITY_STATE i_InfiState);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-26 by cmkwon, 인피2차 수정 - CFieldIOCP 관리구조 추가
	CFieldIOCP					*m_pFieldIOCP21;

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	CInfinityBossrush *FindBossrushNoLock(InfinityCreateUID_t i_CreateUID);
	CInfinityDefence *FindDefenceNoLock(InfinityCreateUID_t i_CreateUID);
	CInfinityMShipBattle *FindMShipBattleNoLock(InfinityCreateUID_t i_CreateUID);			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	CInfinityBossrush *FindBossrushNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx);
	CInfinityDefence *FindDefenceNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx);
	CInfinityMShipBattle *FindMShipBattleNoLock(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx);			// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업

	Err_t PushTenderItemW(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIdx, CTenderItemInfo *i_pTenderItemInfo);

	///////////////////////////////////////////////////////////////////////////////	
	// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
	Err_t CheckInfinityAllPlayerStateW(eINFINITY_MODE i_eInfiMode, InfinityCreateUID_t i_CreateUID, eINFINITY_STATE i_infiState);

	///////////////////////////////////////////////////////////////////////////////	
	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	///////////////////////////////////////////////////////////////////////////////
	// 2010-05-06 by shcho, 난이도 데이터 정보 가져오기
	BOOL Get_Difficulty_BonusInfo_ListData(vectorInfinity_DifficultyInfo_Bonus *p_vecGetInfinity_DifficulytList);
	BOOL Set_Difficulty_BonusInfo_ListData(vectorInfinity_DifficultyInfo_Bonus *p_vecListInfo);

	const INFINITY_DIFFICULTY_BONUS_INFO *Get_Difficulty_BonusInfo ( const int a_iStep );
	///////////////////////////////////////////////////////////////////////////////
	// 2010-05-06 by shcho, 난이도 데이터 정보 저장
	const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO *Get_Difficulty_MonsterInfo (int iStep);

	BOOL Set_Difficulty_MonsterInfo_ListData(vectorInfinity_DifficultyInfo_Monster *p_vecListInfo);
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	void UpdateNextSceneProc ( eINFINITY_MODE i_eInfiMode , InfinityCreateUID_t i_CreateUID , CFieldIOCPSocket * i_pMasterFISoc , int nUpdateSceneCount = 1 );	// 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) ) - GM 다음 씨네마 바로 호출.
	
protected:

	CAtumFieldDBManager			*m_pAtumDBManager;
	CInfinityTickManager		*m_pTickManager;
	CCinema						m_Cinema;
	CRevision					m_Revision;
	CInfinityMapManager			m_InfiMapManager;
	vectorInfinityModeInfo		m_vectInfiModeInfo;
	vectorInfinityMonsterInfo	m_vectInfiMonsterInfo; // 인피니티 몬스터 info 정보
	mtvectInfiBossRush			m_mtvectInfiBossRush;
	mtvectInfiDefence			m_mtvectInfiDefence;
	mtvectInfiMShipBattle		m_mtvectInfiMShipBattle;		// 2011-02-18 by hskim, 인피니티 3차 - 방 생성 작업
	
	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	vectorInfinity_DifficultyInfo_Bonus			m_vecInfinityDifficultyBonusInfo;	// 2010-05-13 by shcho, 인피니티 난이도 조절 - 유저에게 보여주는 난이도 정보
	vectorInfinity_DifficultyInfo_Monster		m_vecInfinityDifficultyMonsterInfo;	// 2010-05-13 by shcho, 인피니티 난이도 조절 - 몬스터 적용 정보
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	
	InfinityCreateUID_t				m_nInfinityCreateUID;
	mtvectorInfinityDisConnectUser  m_mtInfinityDisConnectUserList;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	CRITICAL_SECTION				m_criticalSectionCreate;				// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
};

#endif // !defined(AFX_INFINITYMANAGER_H__9E1738B9_2E9B_4A12_8CDE_FF761AAC14A8__INCLUDED_)
