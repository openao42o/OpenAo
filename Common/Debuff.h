// Debuff.h: interface for the CDebuff class.
////Copyright[2002] MasangSoft
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUFF_H__D3E2A7B5_A30F_41FE_9FC3_BD4485E3B78E__INCLUDED_)
#define AFX_DEBUFF_H__D3E2A7B5_A30F_41FE_9FC3_BD4485E3B78E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDebuff  
{
public:
	CDebuff();
	virtual ~CDebuff();

	void InitDebuff();
	void SetDebuff(ITEM * i_pSkill, INT i_nDesParamIdx , float fIncreasePowerRatio = 1.0f );
	BOOL CheckApplyingDebuff(DestParam_t i_byDestParam);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	BOOL ReleaseDebuff(ItemIdx_t i_nSkillNum, DestParam_t i_byDestParam, BOOL i_bTimeCheck = TRUE);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	INT	 MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK(MSG_DEBUFF_INFO *o_pDebuffInfo);

protected:
	mtvectorDebuffInfo						m_mtvectDebuffInfo;
};

#endif // !defined(AFX_DEBUFF_H__D3E2A7B5_A30F_41FE_9FC3_BD4485E3B78E__INCLUDED_)
