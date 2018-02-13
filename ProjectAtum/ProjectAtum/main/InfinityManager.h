// InfinityManager.h: interface for the CInfinityManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include "AtumProtocol.h"
#include <vector>

struct tINFINITY_DIFFICULTINFO
{
	tINFINITY_DIFFICULTINFO()
	{
		memset(this, 0x00, sizeof(tINFINITY_DIFFICULTINFO));
	}

	void SetLevel(const INT a_iLevel)
	{
		const INT a_iDefPer = 100;
		const INT a_iBonusPer = 10;

		this->iLevel = a_iLevel;

		this->iLevel = min(this->iLevel, 21);
		this->iLevel = max(this->iLevel, 0);

		this->iBonusExpPer = a_iDefPer + (this->iLevel - 5) * a_iBonusPer;
		this->iBonusDropChancePer = a_iDefPer + (this->iLevel - 5) * a_iBonusPer;
		this->iBonusDropCountPer = a_iDefPer + (this->iLevel - 5) * a_iBonusPer;
	}

	void SetDefault(void)
	{
		SetLevel(5);
	}

	void Reset(void)
	{
		memset(this, 0x00, sizeof(tINFINITY_DIFFICULTINFO));
	}

	INT iLevel;
	INT iBonusExpPer;
	INT iBonusDropChancePer;
	INT iBonusDropCountPer;

};

struct MyInfinityRoomInfo
{
	ClientIndex_t			MasterIndex;
	InfinityCreateUID_t		InfinityCreateUID;
	INFINITY_MODE_INFO		ModeInfo;

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	MONSTER_BALANCE_DATA	sDifficultyInfo;
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	MyInfinityRoomInfo()
	{
		MasterIndex = 0;
		InfinityCreateUID = 0;
		memset(&ModeInfo, 0, sizeof(ModeInfo));
		// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
		sDifficultyInfo.Init(0);
		// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	}
};

class CItemInfo;

class CInfinityManager
{
private:
	// 인피 모드 정보
	std::vector<INFINITY_MODE_INFO*>		m_vecModeInfo;

	// 인피 대기방 리스트리스트
	std::vector<INFINITY_READY_LIST*>		m_vecReadyInfo;

	// 인피 대기 멤버 리스트
	std::vector<INFINITY_MEMBER_INFO_LIST*>	m_vecMemberInfo;

	// 인피 난이도 설정 - 보너스 정보 리스트.
	vectorInfinity_DifficultyInfo_Bonus		m_vecInfinityDifficultyBonusInfo_ListData;

	// 내가 선택한 모드
	int									m_nZoneIdx;

	// 내가 선택한 방
	int									m_nRoomIdx;

	// 내가 선택한 멤버
	int									m_nMemberIdx;

	// 내 인피니티 방 정보
	MyInfinityRoomInfo					m_MyRoomInfo;

	// 가입 요청자 리스트
	std::deque<MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER*> m_deqRequest;

	// 강퇴하려는 클라 인덱스
	ClientIndex_t						m_RejectClientIdx;

	// 내가 생성하려는 방 정보
	MSG_FC_INFINITY_CREATE				m_CreateRoomInfo;

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

//  // 입찰 아이템 목록
//  MSG_FC_INFINITY_TENDER_DROPITEM_INFO m_CurTenderItem;
//  
//  // 현재 입찰하는 아이템 정보
//  CItemInfo*							 m_pCurTenderItemInfo;
// 
// 	// 입찰 아이템 목록
// 	std::vector<MSG_FC_INFINITY_TENDER_DROPITEM_INFO*>	m_vecTenderItemList;

	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	// 인피니티 난이도 레벨 조절 가능 범위.
	INT									m_iDifficultyLevelMin;
	INT									m_iDifficultyLevelMax;
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

public:
	CInfinityManager();
	virtual ~CInfinityManager();

public:
	// 인피니티 모드 정보
	void	ClearModeInfo();

	void	AddModeInfo(INFINITY_MODE_INFO* pModeInfo);

	size_t	GetInfinityModeCount()
	{
		return m_vecModeInfo.size();
	}

	INFINITY_MODE_INFO*	GetInfinityModeByIndex(size_t idx)
	{
		return m_vecModeInfo[idx];
	}

	int		GetInfinityModeIdx()
	{
		return m_nZoneIdx;
	}

	inline	void	SetInfinityModeIdx(int nZoneIdx)
	{
		m_nZoneIdx = nZoneIdx;
	}



	// 인피니티 멤버 정보
	void	ClearMemberInfo();

	void	AddMemberInfo(INFINITY_MEMBER_INFO_LIST* pMemberInfo);

	void	RemoveMemberInfo(ClientIndex_t ClientIdx);

	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	BOOL	IsAllReady();
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

	INFINITY_MEMBER_INFO_LIST*	GetMemberInfoByClientIdx(ClientIndex_t ClientIdx);

	INFINITY_MEMBER_INFO_LIST*	GetMemberInfoByCName(const char *a_pszCName);			// 캐릭터명으로 맴버 정보 찾기.

	inline	size_t	GetMemberCount(void)
	{
		return m_vecMemberInfo.size();
	}

	inline	INFINITY_MEMBER_INFO_LIST*	GetMemberInfoByIndex(size_t idx)
	{
		return m_vecMemberInfo[idx];
	}

	inline	int		GetMemberIdx(void)
	{
		return m_nMemberIdx;
	}

	inline	void	SetMemberIdx(int nMemberIdx)
	{
		m_nMemberIdx = nMemberIdx;
	}



	// 인피니티 대기방 정보
	void ClearReadyInfo();

	void AddReadyInfo(INFINITY_READY_LIST* pReadyInfo);

	size_t	GetInfinityReadyInfoCount()
	{
		return m_vecReadyInfo.size();
	}

	INFINITY_READY_LIST* GetInfinityReadyByIndex(size_t idx)
	{
		return m_vecReadyInfo[idx];
	}

	int GetInfinityReadyIdx() const
	{
		return m_nRoomIdx;
	}

	inline	void	SetInfinityReadyIdx(int nRoomIdx)
	{
		m_nRoomIdx = nRoomIdx;
	}



	// 내가 속한 방의 정보
	void SetMyRoomInfo(ClientIndex_t MasterIndex,
		InfinityCreateUID_t CreateUID,
		INFINITY_MODE_INFO* pModeInfo);

	MyInfinityRoomInfo* GetMyRoomInfo()
	{
		return &m_MyRoomInfo;
	}

	void ClearMyRoomInfo()
	{
		memset(&m_MyRoomInfo, 0, sizeof(MyInfinityRoomInfo));
	}


	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	// 내가 속한 방의 난이도 정보 대입.
	BOOL	SetMyRoomInfo_Difficult(const INT a_InfinityDifficultyLevel);
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	// 인피니티 난이도 - 보너스 정보 등록.
	void	Register_InfinityDifficultyBonusInfo_ListData(INFINITY_DIFFICULTY_BONUS_INFO *pInfiBonusInfo);

	// 인티니티 난이도 - 보너스 정보값 리턴.
	const	INFINITY_DIFFICULTY_BONUS_INFO *Get_InfinityDifficultyBonusInfo(const INT a_DifficultyLevel);

	// 인피니티 난이도 - 보너스 정보 목록 삭제.
	void ClearInfinityDifficultyBonusInfo_ListData()
	{
		m_vecInfinityDifficultyBonusInfo_ListData.clear();

		m_iDifficultyLevelMin = 1;
		m_iDifficultyLevelMax = DEFAULT_LEVEL_INFINITY_DIFFICULTY;
	}

	INT Get_DifficultyLevelMin() const { return m_iDifficultyLevelMin; }
	INT Get_DifficultyLevelMax() const { return m_iDifficultyLevelMax; }
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	// 방 참여 요청자
	void	AddJoinRequest(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pJoinRequest);

	MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER*	GetFirstJoinRequest(void);

	void	RemoveFirstJointRequest(void);

	void	RemoveJoinRequest(ClientIndex_t nRequesterIdx);

	void	ClearJoinRequest(void);



	// 강퇴 요청자
	void SetRejectClientIdx(ClientIndex_t RejectClientIdx)
	{
		m_RejectClientIdx = RejectClientIdx;
	}

	ClientIndex_t GetRejectClientIdx() const
	{
		return m_RejectClientIdx;
	}

	void ClearRejectClientIdx()
	{
		m_RejectClientIdx = 0;
	}



	// 내가 생성하는 방 정보
	void SetCreateRoomInfo(MSG_FC_INFINITY_CREATE* pCreateRoomInfo)
	{
		memcpy(&m_CreateRoomInfo, pCreateRoomInfo, sizeof(m_CreateRoomInfo));
	}

	MSG_FC_INFINITY_CREATE* GetCreateRoomInfo() { return &m_CreateRoomInfo; }

	void ClearCreateRoomInfo() { memset(&m_CreateRoomInfo, 0, sizeof(m_CreateRoomInfo)); }

	void ClearManager()
	{
		ClearModeInfo();

		ClearReadyInfo();

		ClearMemberInfo();

		ClearMyRoomInfo();

		ClearJoinRequest();

		ClearRejectClientIdx();

		ClearCreateRoomInfo();

		ClearInfinityDifficultyBonusInfo_ListData();
	}
};
