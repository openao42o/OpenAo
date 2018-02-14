//Copyright[2002] MasangSoft
// Dot.h: interface for the CDot class.
//
//////////////////////////////////////////////////////////////////////

#ifndef COMMON_FTP_DRAWGDIX_H_
#define COMMON_FTP_DRAWGDIX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDot  
{
public:
	CDot();
	virtual ~CDot();

	void InitDot();
	void SetDot( ITEM * i_pSkill , INT i_nDesParamIdx , ClientIndex_t i_nClientIndex , UID32_t i_nTimerUID , float i_fIncreasePowerRatio = 1.0f );
	BOOL CheckApplyingDot(UID32_t i_nTimerUID, DestParam_t	i_byDesParam);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	BOOL DecreaseApplyingDotTime(UID32_t i_nTimerUID, DestParam_t i_byDesParam);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	BOOL ReleaseDot(UID32_t	i_nTimerUID);
	BOOL ReleaseDotByCancelSkill(ITEM * i_pSkill, DestParam_t i_byDestParam);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	ClientIndex_t GetClientIdx(UID32_t	i_nTimerUID);
	void CopyDotInfo(UID32_t i_nTimerUID, DOTINFO * o_pDotInfo);
	INT	 MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK(MSG_DOT_INFO *o_pDotInfo);

protected:
	mtvectorDotInfo						m_mtvectDotInfo;
};

#endif // COMMON_FTP_DRAWGDIX_H_
