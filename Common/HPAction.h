// HPAction.h: interface for the CHPAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HPACTION_H__6E1DDDE4_5068_41C9_BD4C_2032B4B4248E__INCLUDED_)
#define AFX_HPACTION_H__6E1DDDE4_5068_41C9_BD4C_2032B4B4248E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHPAction  
{
public:
	CHPAction();
	virtual ~CHPAction();

	void Init();
	void InitHPActionListByDB(vectHPAction * i_pvectHPActionDBValue);			// DB값으로 초기화
	void InitHPActionAttackHPRateList(HPACTION * i_pHPAction);					// HP값에 따른 공격 관련 리스트만 따로 만든다.
	void InitHPActionTalkHPRateList(HPACTION * i_pHPAction);					// HP값 관련 대화 리스트를 따로 만든다.
	void InitHPActionTalkDamagedRandomList(HPACTION * i_pHPAction);				// 피격시 대화 리스트르 따로 만든다.
	void InitHPActionTalkCreate(HPACTION * i_pHPAction);						// 생성시 대화 정보 따로 만든다.
	void InitHPActionTalkDead(HPACTION * i_pHPAction);							// 죽음시 대화 정보 따로 만든다.
	void InitHPActionTalkTargetChange(HPACTION * i_pHPAction);					// 타겟 변경 시 대화 정보 따로 만든다.
	BOOL CheckValidSizeAttackItemIdx();
	BOOL CheckValidSizeTalkCreate();
	BOOL CheckValidSizeTalkDead();
	BOOL CheckValidSizeTalkHPRate();
	BOOL CheckValidSizeTalkDamagedRadom();
	BOOL CheckValidSizeTalkTargetChange();
	BOOL GetAttackItemIdxHPRate(MonHP_t i_CurrentMonHP, ItemIdx_t * o_pAttackItemIdx, INT * o_pSelectVectIdx);	// HP값에 따른 공격 몬스터아이템배열인덱스 가져오기
	BOOL GetNextAttackItem(ItemIdx_t * o_pAttackItemIdx);								// 연속 공격 값이 있는지
// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정, 밑과 같이 수정 HPActionItem 사용시 사용 카운트를 줄인다.
//	void SetSuccessAttackItemIdxHPRate(INT i_SelectVectIdx);							// HP값에 따른 공격 성공 처리
	void SetSuccessAttackItemIdxHPRate();												// HP값에 따른 공격 성공 처리
	BOOL GetTalkCreate(HPACTION_TALK_HPRATE * o_pTalkHPRate);							// 생성시 대화 가져오기
	BOOL GetTalkDead(HPACTION_TALK_HPRATE * o_pTalkHPRate);								// 죽었을 경우 대화 가져오기
	BOOL GetTalkHPRate(MonHP_t i_CurrentMonHP, HPACTION_TALK_HPRATE * o_pTalkHPRate);	// HP값에 따른 대화 가져오기
	BOOL GetTalkDamagedRandom(HPTalk_t * o_pTalk);										// 피격시 대화 가져오기	
	BOOL GetTalkTargetChange(HPTalk_t * o_pTalk);										// 타겟 변경 시 대화 가져오기	
	void EraseHPActionByUseItemArrayIdx(ItemIdx_t i_UseItemArrayIdx);
	void SetHPTalkAttack(ItemIdx_t i_ItemArrayIdx, INT i_ItemNum);							// 공격에 따른 대화 저장
	BOOL GetPreHPTalkAttack(ItemIdx_t i_AttackItemNum, HPTalk_t * o_pPreTalk, MSec_t * o_pPreHPCameraTremble);			// 준비 공격시 대화 및 카메라 떨림 가져오기
	BOOL GetHPTalkAttack(ItemIdx_t i_AttackItemNum , HPTalk_t * o_pTalk, MSec_t * o_pHPCameraTremble);					// 공격시 대화 및 카메라 떨림 가져오기
	
protected:
	vectHPAction					m_vectHPAction;
	vectHPActionAttackHPRate		m_vectHPActionAttackHPRate;
	vectHPActionTalkHPRate			m_vectHPActionTalkHPRate;
	vectHPActionTalkDamagedRandom	m_vectHPActionTalkDamagedRandom;
	vectHPActionTalkAttack			m_vectHPActionTalkAttack;
	
	ItemIdx_t						m_NextAttackItemIdx;
	HPACTION_TALK_HPRATE			m_CreateTalk;
	HPACTION_TALK_HPRATE			m_DeadTalk;
	HPTalk_t						m_TargetChangeTalk[SIZE_MAX_HPTALK_DESCRIPTION];
	HPActionUID_t					m_HPActionAttackUID;			// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
	HPActionUID_t					m_HPActionAttackTalkUID;		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
};

#endif // !defined(AFX_HPACTION_H__6E1DDDE4_5068_41C9_BD4C_2032B4B4248E__INCLUDED_)
