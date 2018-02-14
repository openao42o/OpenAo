// Debuff.cpp: implementation of the CDebuff class.
////Copyright[2002] MasangSoft
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Debuff.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDebuff::CDebuff()
{
    this->InitDebuff();
}

CDebuff::~CDebuff()
{

}

void CDebuff::InitDebuff() {
    mt_auto_lock mta(&m_mtvectDebuffInfo);
    m_mtvectDebuffInfo.clear();
}


/**************************************************************************************
**
**    디버프 정보 등록.
**
**    Create Info : ??. ??. ??
**
**    Update Info : 2010. 06. 08 by hsLee. - 밸런스 인자 추가. ( fIncreasePowerRatio )
**
***************************************************************************************/
void CDebuff::SetDebuff(ITEM * i_pSkill, INT i_nDesParamIdx , float fIncreasePowerRatio /*= 1.0f*/ ) 
{
    if(NULL == i_pSkill) 
    {
        // 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
        return;
    }

    DEBUFFINFO    DebuffInfo;
        util::zero(&DebuffInfo, sizeof(DEBUFFINFO));

    DebuffInfo.ItemNum            = i_pSkill->ItemNum;
    DebuffInfo.Kind                = i_pSkill->Kind;
    DebuffInfo.DesParam            = i_pSkill->ArrDestParameter[i_nDesParamIdx];

    // 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.)
    //DebuffInfo.DesParamValue    = i_pSkill->ArrParameterValue[i_nDesParamIdx];
    DebuffInfo.DesParamValue    = i_pSkill->ArrParameterValue[i_nDesParamIdx] * fIncreasePowerRatio;

    DebuffInfo.Time                = i_pSkill->Time;
    DebuffInfo.SkillLevel        = i_pSkill->SkillLevel;
    DebuffInfo.SkillType        = i_pSkill->SkillType;
    DebuffInfo.dwUseTime        = timeGetTime();

    mt_auto_lock mta(&m_mtvectDebuffInfo);

    mtvectorDebuffInfo::iterator    itr = m_mtvectDebuffInfo.begin();

    while(itr != m_mtvectDebuffInfo.end()) 
    {
        if ( itr->ItemNum == i_pSkill->ItemNum            // 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 가끔 몬스터 스킬 사용 안되는 버그 수정
            && itr->DesParam == i_pSkill->ArrDestParameter[i_nDesParamIdx]
            && itr->SkillLevel <= i_pSkill->SkillLevel) 
        {
            // 같은 디버프이지만 더 높거나 같은 효과에 디버프이면 갱신한다.
            itr = m_mtvectDebuffInfo.erase(itr);
            m_mtvectDebuffInfo.push_back(DebuffInfo);
            return;
        }
        itr++;
    }
    m_mtvectDebuffInfo.push_back(DebuffInfo);
}

BOOL CDebuff::CheckApplyingDebuff(DestParam_t i_byDestParam) {        // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
    mt_auto_lock mta(&m_mtvectDebuffInfo);
    mtvectorDebuffInfo::iterator    itr = m_mtvectDebuffInfo.begin();
    while(itr != m_mtvectDebuffInfo.end()) {
        if(i_byDestParam == itr->DesParam) {
            return TRUE;
        }
        itr++;
    }

    return FALSE;
}

BOOL CDebuff::ReleaseDebuff(ItemIdx_t i_nSkillNum, DestParam_t i_byDestParam, BOOL i_bTimeCheck/* = TRUE*/) {        // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
    mt_auto_lock mta(&m_mtvectDebuffInfo);
    
    if(i_bTimeCheck) {
        DWORD dwCurTick = timeGetTime();    
        mtvectorDebuffInfo::iterator    itr = m_mtvectDebuffInfo.begin();
        while(itr != m_mtvectDebuffInfo.end()) {
            DWORD dwElapseTick = dwCurTick - itr->dwUseTime;
            if(i_nSkillNum == itr->ItemNum && itr->DesParam == i_byDestParam 
//                && dwElapseTick >= itr->Time
                ) {
                itr = m_mtvectDebuffInfo.erase(itr);
                return TRUE;
            }
            itr++;
        }
    }
    else {    
        mtvectorDebuffInfo::iterator    itr = m_mtvectDebuffInfo.begin();
        while(itr != m_mtvectDebuffInfo.end()) {
            if(i_nSkillNum == itr->ItemNum && itr->DesParam == i_byDestParam) {
                itr = m_mtvectDebuffInfo.erase(itr);
                return TRUE;
            }
            itr++;
        }
    }

    return FALSE;
}

INT CDebuff::MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK(MSG_DEBUFF_INFO *o_pDebuffInfo) {
    if(NULL == o_pDebuffInfo) {
        // 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
        return FALSE;
    }

    int    DebuffInfoCount = 0;
    
    mt_auto_lock mta(&m_mtvectDebuffInfo);
    mtvectorDebuffInfo::iterator itr = m_mtvectDebuffInfo.begin();
    for(; itr != m_mtvectDebuffInfo.end(); itr++) {
        o_pDebuffInfo[DebuffInfoCount].ItemNum        = itr->ItemNum;
        DebuffInfoCount++;
    }
    
    return DebuffInfoCount;
}
