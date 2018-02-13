// SkillInfo.h: interface for the CSkillInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLINFO_H__7B90C53A_7DEA_4545_A5DC_CB8A64A154AD__INCLUDED_)
#define AFX_SKILLINFO_H__7B90C53A_7DEA_4545_A5DC_CB8A64A154AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumParam.h"


///////////////////////////////////////////////////////////////////////////////
/// \class		class CSkillInfo : public ITEM_SKILL 
///
/// \brief		CSkill 이 스킬 전체를 관리하는 클래스라면, CSkillInfo는 스킬 하나에 대한 관리를 담당하는 클래스다.
///				즉, CSkillInfo 는 스킬 1개를 말한다.
/// \author		jschoi
/// \version	
/// \date		2004-10-19 ~ 2004-10-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CSkillInfo : public ITEM_SKILL 
{
public:
	CSkillInfo(ITEM_SKILL* pItemSkill);
	virtual ~CSkillInfo();
	int		GetSkillState() { return m_dwState; }
	void	SetSkillState(int dwState) { m_dwState = dwState; }
	float	GetCheckEnableTime() { return m_fCheckEnableTime; }
	float	GetCheckPrepareTime() { return m_fCheckPrepareTime; }
	float	GetCheckReattackTime() {return m_fCheckReattackTime; }
	void	SetCheckEnableTime() { m_fCheckEnableTime = ItemInfo->Time; }
	void	SetCheckReattackTime() { m_fCheckReattackTime = ItemInfo->ReAttacktime; }
	void	SetReattackTime(float fReattackTime) { m_fCheckReattackTime = fReattackTime; }
	void	SetInitReattackTime() { m_fCheckReattackTime = 0.0f; }	// 2005-11-26 by ispark, ReattackTime 초기화
	void	ChangeSkillState(int dwState, int nTempSkillItemNum = 0); 
	// 2010. 06. 18 by jskim 몬스터 사용 스킬 스트링 변경
	//void	UseSkillFromServer();					// server에서 use skill 허락
	void	UseSkillFromServer(char* Name = NULL, BOOL isUser = TRUE);
	//end 2010. 06. 18 by jskim 몬스터 사용 스킬 스트링 변경
	void	PrepareSkillFromServer();														// 2006-12-20 by ispark, server에서 prepare skill 허락
	void	Tick(float fElapsedTime);
	void	Init();
	BOOL	IsEnableStateForReUsing();
	void	ReUsingSkill();
	void	SendUseSkill();
	void	SendCancelSkill(int nAttackSkillItemNum = 0);									// 2006-12-12 by ispark, 수정
	void	SetParamFactor();
	void	ReleaseParamFactor();
	void	SetCheckAckWaitingTime(float fWaitTime) { m_fCheckAckWaitingTime = fWaitTime; } // 2005-12-05 by ispark
	void	SetbApprovalEnd(BOOL bApproval) { m_bApprovalEnd = bApproval; }					// 2006-07-30 by ispark
	BOOL	GetbApprovalEnd() { return m_bApprovalEnd; }									// 2006-07-30 by ispark
	float	GetCheckAckWaitingTime() { return m_fCheckAckWaitingTime; }						// 2006-10-26 by ispark

	void	SetClientIndex(ClientIndex_t AttackIndex, ClientIndex_t TargetIndex);			// 2006-09-14 by ispark
	ClientIndex_t GetAttackIndex() { return m_AttackIndex; }
	ClientIndex_t GetTargetIndex() { return m_TargetIndex; }
	BOOL	IsMustOneTargetSkill(BYTE i_bySkillTargetType);

	// 2008-12-26 by bhsohn 엠기어 편대 버그 수정
	BOOL	IsSkillEnable();
	void	SetSkillEnable(BOOL i_bSkillEnable);
	// end 2008-12-26 by bhsohn 엠기어 편대 버그 수정

	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	void DisableTimelimitSkill();
	// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

	// 2010. 10. 11. 딜레이 타겟 시전형 스킬 버그 수정.
public :

	inline void ResetClickTargetInfo ( void ) { m_ClickTargetIndex = 0; m_nClickTargetUID = 0; }

	inline void SetClickTagetIndex ( const ClientIndex_t a_UpdateClickTargetIndex ) { m_ClickTargetIndex = a_UpdateClickTargetIndex; }
	inline ClientIndex_t GetClickTargetIndex ( void ) { return m_ClickTargetIndex; }

	inline void SetClickTargetUID ( const UID32_t a_nUpdateClickTragetUID ) { m_nClickTargetUID = a_nUpdateClickTragetUID; }
	inline UID32_t GetClickTargetUID ( void ) { return m_nClickTargetUID; }

	// End 2010. 10. 11. 딜레이 타겟 시전형 스킬 버그 수정.

protected:
	void	Clone(ITEM_SKILL *pItemSkill);
	void	TickPermanent(float fElapsedTime);
	void	TickClick(float fElapsedTime);
	void	TickTimelimit(float fElapsedTime);
	void	TickToggle(float fElapsedTime);
	void	TickCharging(float fElapsedTime);
	BOOL	CheckUsingSkillToMeAtPrepare();

protected:
	float	m_fCheckWaitingTime;		// 서버로 보내고 기다리는 시간
	float	m_fCheckWaitingPrepareTime;	// 서버로 보내고 기다리는 시간
	float	m_fCheckReattackTime;		// 재 가동 예비 시간
	float	m_fCheckEnableTime;			// 시간 후 스킬이 해제된다.
	float	m_fCheckPrepareTime;		// 발동 시간
	float	m_fCheckAckWaitingTime;		// 2005-12-05 by ispark, 제한된 시간 체크(시간이 오버를 하면 자동 스킬 취소)
	int		m_dwState;					// 스킬 상태
	BOOL	m_bSetParamFactor;			// ParamFactor가 적용되었는지.
	BOOL	m_bApprovalEnd;				// 2006-07-30 by ispark, 종료 승인

	ClientIndex_t	m_AttackIndex;
	ClientIndex_t	m_TargetIndex;		// target이 없는 skill인 경우 0

	// 2008-12-26 by bhsohn 엠기어 편대 버그 수정
	BOOL	m_bSkillEnable;

	// 2010. 10. 11. 딜레이 타겟 시전형 스킬 버그 수정.
	ClientIndex_t	m_ClickTargetIndex;
	UID32_t			m_nClickTargetUID;

};

#endif // !defined(AFX_SKILLINFO_H__7B90C53A_7DEA_4545_A5DC_CB8A64A154AD__INCLUDED_)
