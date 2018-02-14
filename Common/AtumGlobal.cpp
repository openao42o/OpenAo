// AtumGlobal.cpp
// Atum global function group
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumGlobal.h"
#include "AtumDefine.h"
#include "StoreData.h"
#include "SceneData.h"
#include "AtumApplication.h"
// 2005-01-08 by jschoi - Test
#include "ShuttleChild.h"
#include "CharacterChild.h"                // 2005-07-21 by ispark
#include "Interface.h"
#include "INFTarget.h"
#include "TutorialSystem.h"
#include "AtumDatabase.h"
#include "Camera.h"
#include "D3DHanFont.h"

// 2010. 02. 11 by ckPark 발동류 장착아이템
#include "ItemInfo.h"
// end 2010. 02. 11 by ckPark 발동류 장착아이템

// 2015-11-2, PanKJ FreeWar
#include "FreeWar.h"

extern CStoreData * g_pStoreData;
extern CSceneData * g_pScene;
extern CAtumApplication * g_pD3dApp;

void SetParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue)
{
    switch(nDestParam)
    {
    case DES_NULL:                        // 대상 파라미터가 없는 경우 사용
    case DES_FACTION_01    :                // 속성 01, check: 추가됨
    case DES_FACTION_02    :                // 속성 02, check: 추가됨
    case DES_UNITKIND    :                // 기체 업그레이드시 해당 기체
    case DES_ITEMKIND    :                // 아이템의 종류(ITEMKIND_XXX)
    case DES_SUMMON        :                // 몬스터 소환
    case DES_GRADUAL_HP_UP:                // 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel
    //case DES_GRADUAL_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel, check: 추가됨
    case DES_GRADUAL_SP_UP:                // 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
    case DES_GRADUAL_EP_UP:                // 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
    case DES_IN_TIME_HP_UP:                // 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IN_TIME_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌, check: 추가됨
    case DES_IN_TIME_SP_UP:                // 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IN_TIME_EP_UP:                // 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IMMEDIATE_HP_UP:            // 에너지류, 현재 HP를 즉시 VALUE만큼 올려줌
    case DES_IMMEDIATE_DP_UP:            // 에너지류, 현재 DP를 즉시 VALUE만큼 올려줌, check: 추가됨
    case DES_IMMEDIATE_SP_UP:            // 에너지류, 현재 SP를 즉시 VALUE만큼 올려줌
    case DES_IMMEDIATE_EP_UP:            // 에너지류, 현재 EP를 즉시 VALUE만큼 올려줌
    case DES_GROUNDMODE    :                // 폭격모드
    case DES_SIEGEMODE    :                // 시즈모드
    case DES_BULLET_01    :                // (*) 빔소모총알량 01, check: 추가됨
    case DES_BULLET_02    :                // (*) 빔소모총알량 02, check: 추가됨
    case DES_PRIMARY_WEAPON    :            // 1형 무기, 범용
    case DES_SECONDARY_WEAPON:            // 2형 무기, 범용
    case DES_ALL_WEAPON    :                // 1,2형 무기 모두, 범용
        break;
    case DES_OVERHITTIME_01:            // (*) 오버힛회복시간 01, check: 추가됨
//        paramFactor.pfm_TIME_01 += fDestValue;
        break;
    case DES_OVERHITTIME_02:            // (*) 오버힛회복시간 02, check: 추가됨
//        paramFactor.pfm_TIME_02 += fDestValue;
        break;
    case DES_ATTACK_PART:                    // 공격 디바이스 파트
//        paramFactor.pfp_ATTACK_PART += fDestValue;
        break;
    case DES_DEFENSE_PART:                // 내구 디바이스 파트
//        paramFactor.pfp_DEFENSE_PART += fDestValue;
        break;
    case DES_FUEL_PART:                    // 연료 디바이스 파트
//        paramFactor.pfp_FUEL_PART += fDestValue;
        break;
    case DES_SOUL_PART:                    // 감응 디바이스 파트
//        paramFactor.pfp_SOUL_PART += fDestValue;
        break;
    case DES_SHIELD_PART:                // 방어 디바이스 파트
//        paramFactor.pfp_SHIELD_PART += fDestValue;
        break;
    case DES_DODGE_PART    :                // 회피 디바이스 파트
//        paramFactor.pfp_DODGE_PART += fDestValue;
        break;
    case DES_BODYCONDITION:                // 몸상태
    case DES_ENDURANCE_01:                // 내구도 01
        paramFactor.pfp_ENDURANCE_01 += fDestValue;
        break;
    case DES_ENDURANCE_02:                // 내구도 02
        paramFactor.pfp_ENDURANCE_02 += fDestValue;
        break;
    case DES_CHARGING_01    :            // 장탄수 01
        paramFactor.pfp_CHARGING_01 += fDestValue;
        break;
    case DES_CHARGING_02    :            // 장탄수 02
        paramFactor.pfp_CHARGING_02 += fDestValue;
        break;
    case DES_PROPENSITY    :                // 성향
        paramFactor.pfp_PROPENSITY += (INT)fDestValue;
        break;
    case DES_HP            :                // 히트포인트, MAX HP를 +VALUE만큼 올려줌
        paramFactor.pfp_HP += fDestValue;
        break;
    case DES_DP            :                // 쉴드
        paramFactor.pfp_DP += fDestValue;
        break;
    case DES_MAX_SP_UP  :                // 소울포인트 뻥 아이템
    case DES_SP            :                // 소울포인트
        paramFactor.pfp_SP += fDestValue;
        break;
    case DES_EP            :                // 엔진포인트
        paramFactor.pfp_EP += fDestValue;
        break;
    case DES_SPRECOVERY    :                // 소울포인트회복력
        paramFactor.pfp_SPRECOVERY += fDestValue;
        break;
    case DES_HPRECOVERY    :                // 에너지포인트회복력
        paramFactor.pfp_HPRECOVERY += fDestValue;
        break;
    case DES_MINATTACK_01:                // 최소 공격력 01, check: * 
        paramFactor.pfm_MINATTACK_01 += fDestValue;
        break;
    case DES_MAXATTACK_01:                // 최대 공격력 01, check: * 
        paramFactor.pfm_MAXATTACK_01 += fDestValue;
        break;
    case DES_MINATTACK_02:                // 최소 공격력 02, check: * 
        paramFactor.pfm_MINATTACK_02 += fDestValue;
        break;
    case DES_MAXATTACK_02:                // 최대 공격력 02, check: * 
        paramFactor.pfm_MAXATTACK_02 += fDestValue;
        break;
    case DES_ATTACKPROBABILITY_01:        // 공격확률 01
        // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
        //paramFactor.pfp_ATTACKPROBABILITY_01 += (Prob255_t)fDestValue;
        paramFactor.pfp_ATTACKPROBABILITY_01 += fDestValue;
        break;
    case DES_ATTACKPROBABILITY_02:        // 공격확률 02
        // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
        //paramFactor.pfp_ATTACKPROBABILITY_02 += (Prob255_t)fDestValue;
        paramFactor.pfp_ATTACKPROBABILITY_02 += fDestValue;
        break;
    case DES_DEFENSE_01    :                // 방어력 01
        paramFactor.pfp_DEFENSE_01 += fDestValue;    // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
        break;
    case DES_DEFENSE_02    :                // 방어력 02
        paramFactor.pfp_DEFENSE_02 += fDestValue;   // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
        break;
    case DES_DEFENSEPROBABILITY_01:        // 방어확률 01
        paramFactor.pfp_DEFENSEPROBABILITY_01 += fDestValue; // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
        break;
    case DES_DEFENSEPROBABILITY_02:        // 방어확률 02
        paramFactor.pfp_DEFENSEPROBABILITY_02 += fDestValue; // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
        break;
    // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)
    case DES_SKILLPROBABILITY_01    :    // 스킬공격확률 01
    case DES_SKILLPROBABILITY_02    :    // 스킬공격확률 02
        paramFactor.pfp_SKILLPROBABILITY += (Prob255_t)fDestValue;
        break;
    case DES_FACTIONRESISTANCE_01:        // 속성저항력 01
    case DES_FACTIONRESISTANCE_02:        // 속성저항력 02
        paramFactor.pfp_FACTIONRESISTANCE += (Prob255_t)fDestValue;
        break;
    case DES_SPEED        :                // 이동속도
        paramFactor.pfm_SPEED += fDestValue;
        break;
    case DES_TRANSPORT    :                // 운반력
        paramFactor.pfp_TRANSPORT += fDestValue;
        break;
    case DES_MATERIAL    :                // 재질
        paramFactor.pfp_MATERIAL += fDestValue;
        break;
    case DES_REATTACKTIME_01    :        // (*) 리어택타임 01 (- 증가)
        paramFactor.pfm_REATTACKTIME_01 += fDestValue;
        break;
    case DES_REATTACKTIME_02    :        // (*) 리어택타임 02 (- 증가)
        paramFactor.pfm_REATTACKTIME_02 += fDestValue;
        break;
    case DES_ABRASIONRATE_01    :        // 마모율 01
        paramFactor.pfp_ABRASIONRATE_01 += (Prob255_t)fDestValue;
        break;
    case DES_ABRASIONRATE_02    :        // 마모율 02
        paramFactor.pfp_ABRASIONRATE_02 += (Prob255_t)fDestValue;
        break;
    case DES_RANGE_01:                    // (*) 유효거리 01
        paramFactor.pfm_RANGE_01 += fDestValue;
        break;
    case DES_RANGE_02:                    // (*) 유효거리 02
        paramFactor.pfm_RANGE_02 += fDestValue;
        break;
    case DES_RANGEANGLE_01:                // 유효각도 01
        paramFactor.pfp_RANGEANGLE_01 += fDestValue;
        break;
    case DES_RANGEANGLE_02:                // 유효각도 02
        paramFactor.pfp_RANGEANGLE_02 += fDestValue;
        break;
    case DES_MULTITAGET_01:                // 멀티타겟 01
    case DES_MULTITAGET_02:                // 멀티타겟 02
        paramFactor.pfp_MULTITAGET += fDestValue;
        break;
    case DES_EXPLOSIONRANGE_01:            // 폭발반경 01
        paramFactor.pfp_EXPLOSIONRANGE_01 += fDestValue;
        break;
    case DES_EXPLOSIONRANGE_02:            // 폭발반경 02
        paramFactor.pfp_EXPLOSIONRANGE_02 += fDestValue;
        break;
    case DES_UNIT    :                    // 유닛의 종류 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
        paramFactor.pfp_UNIT += fDestValue;
        break;
    case DES_REVISION:                    // 유닛의 보정값 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
        paramFactor.pfp_REVISION += fDestValue;
        break;
    case DES_FACTIONPROBABILITY_01:        // 속성에 대한 방어확률 01
    case DES_FACTIONPROBABILITY_02:        // 속성에 대한 방어확률 02
        paramFactor.pfp_FACTIONPROBABILITY += (Prob255_t)fDestValue;
        break;
    case DES_SHOTNUM_01    :                // 일점사 당 발사 수 01
        paramFactor.pfp_SHOTNUM_01 += (int)fDestValue;
        break;
    case DES_SHOTNUM_02    :                // 일점사 당 발사 수 02
        paramFactor.pfp_SHOTNUM_02 += (int)fDestValue;
        break;
    case DES_MULTINUM_01    :            // 동시 발사 수 01
        paramFactor.pfp_MULTINUM_01 += (int)fDestValue;
        break;
    case DES_MULTINUM_02    :            // 동시 발사 수 02
        paramFactor.pfp_MULTINUM_02 += (int)fDestValue;
        break;
    case DES_ATTACKTIME_01:                // 처음 공격 시의 타임 01 (- 증가)
        paramFactor.pfp_ATTACKTIME_01 += fDestValue;
        break;
    case DES_ATTACKTIME_02:                // 처음 공격 시의 타임 02 (- 증가)
        paramFactor.pfp_ATTACKTIME_02 += fDestValue;
        break;
    case DES_TIME_01        :            // (*) 지속 시간 01, check: + -> *
        paramFactor.pfm_TIME_01 += fDestValue;
        break;
    case DES_TIME_02        :            // (*) 지속 시간 02, check: + -> *
        paramFactor.pfm_TIME_02 += fDestValue;
        break;
    case DES_WEIGHT_01    :                // (*) 무게 01, check: 추가됨
        paramFactor.pfm_WEIGHT_01 += fDestValue;
        break;
    case DES_WEIGHT_02    :                // (*) 무게 02, check: 추가됨
        paramFactor.pfm_WEIGHT_02 += fDestValue;
        break;
    case DES_REACTION_RANGE:
        paramFactor.pfp_REACTION_RANGE += (USHORT)fDestValue;
        break;
    case DES_REQ_MIN_LEVEL:
        paramFactor.pfp_REQ_MIN_LEVEL += (int)fDestValue;
        break;
    case DES_REQ_MAX_LEVEL:
        paramFactor.pfp_REQ_MAX_LEVEL += (int)fDestValue;
        break;
    case DES_ATTACK_RANGE_01:
        paramFactor.pfm_ATTACK_RANGE_01 += fDestValue;
        break;
    case DES_ATTACK_RANGE_02:
        paramFactor.pfm_ATTACK_RANGE_02 += fDestValue;
        break;
    case DES_SKILL_SMARTSP:
        paramFactor.pfm_SKILL_SMARTSP += fDestValue;
        break;
    case DES_SKILL_REACTIONSPEED:
        paramFactor.pfm_SKILL_REACTIONSPEED += fDestValue;
        break;
    case DES_SKILL_ENGINEANGLE:
        paramFactor.pfm_SKILL_ENGINEANGLE += fDestValue;
        break;
    case DES_SKILL_ENGINEBOOSTERANGLE:
        paramFactor.pfm_SKILL_ENGINEBOOSTERANGLE += fDestValue;
        break;
    case DES_SKILL_SLOWMOVING:
        paramFactor.pfm_SPEED += fDestValue;
        break;
    // 2006-10-11 by ispark, 하이퍼 부스터는 값 셋팅이 아닌 설정 값이다.
    case DES_HYPER_BOOSTER:
        g_pShuttleChild->m_nHyperBooster = HYPER_BOOSTER_MAX;
        break;
    case DES_SKILL_SHIELD_PARALYZE: //쉴드 마비.
        break;
    case DES_GRADUAL_DP_UP:    // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel, check: 추가됨
        paramFactor.pfm_DP_REPAIR_RATE += fDestValue;
        break;
    case DES_SKILL_NO_WARNING:
        g_pShuttleChild->SetSkillMissileWarning(TRUE);
        break;
    // 2006-11-17 by dgwoo 체프.
    case DES_SKILL_HALLUCINATION:
        break;
    case DES_SKILL_CHAFF_HP: //체프의 HP
        break;
    case DES_WARHEAD_SPEED:
        paramFactor.pfm_WARHEAD_SPEED += fDestValue;
        break;
        // 2008-09-26 by bhsohn 신규 인첸트 처리
    case DES_ENGINE_BOOSTER_TIME_UP:    // 부스터 시간 증가
        {
            paramFactor.pfn_ENGINE_BOOSTER_TIME_UP+= fDestValue;            
        }
        break;
    case DES_ENGINE_MAX_SPEED_UP:        // 엔진 일반속도(최대) 증가
        {
            paramFactor.pfn_ENGINE_MAX_SPEED_UP+= fDestValue;
        }
        break;
    case DES_ENGINE_MIN_SPEED_UP:        // 엔진 일반속도(최소) 증가
        {
            paramFactor.pfn_ENGINE_MIN_SPEED_UP+= fDestValue;
        }
        break;
    case DES_ENGINE_BOOSTER_SPEED_UP:        // 엔진 부스터속도 증가
        {
            paramFactor.pfn_ENGINE_BOOSTER_SPEED_UP+= fDestValue;
        }
        break;
    case DES_ENGINE_GROUND_SPEED_UP:        // 엔진 지상속도 증가
        {
            paramFactor.pfn_ENGINE_GROUND_SPEED_UP+= fDestValue;
        }
        break;
    case DES_RADAR_OBJECT_DETECT_RANGE:        // 레이더 물체 감지 반경
        {
            paramFactor.pfn_RADAR_OBJECT_DETECT_RANGE+= fDestValue;
        }
        break;
    case DES_PIERCE_UP_01:        // 기본무기 피어스율 증가 카드
        {
            paramFactor.pfm_PIERCE_UP_01+= fDestValue;
        }
        break;
    case DES_PIERCE_UP_02:        // 고급무기 피어스율 증가 카드
        {
            paramFactor.pfm_PIERCE_UP_02+= fDestValue;
        }
        break;
    case DES_ENGINE_ANGLE_UP:    // 엔진 회전각 증가 카드
        {
            paramFactor.pfm_ENGINE_ANGLE_UP += fDestValue;

        }
        break;
    case DES_ENGINE_BOOSTERANGLE_UP:    // 엔진 부스터 회전각 증가 카드
        {
            paramFactor.pfm_ENGINE_BOOSTERANGLE_UP += fDestValue;
        }
        break;

        // end 2008-09-26 by bhsohn 신규 인첸트 처리
        // 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
    case DES_RARE_ITEM_PARTNER_SPEED:    // 마을 이동 스피드 증가
        {
            paramFactor.pf_ITEMS_MOVE_SPEED_RATE += fDestValue;
        }
        break;
    // end 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
        // 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
    case DES_SKILL_BUFF_MON_ATTACK_POWER:            // 몬스터 공격시 - 공격력 증가 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_ATTACK_POWER += fDestValue;
        }
        break;
    case DES_SKILL_BUFF_MON_ATTACK_PROBABILITY:        // 몬스터 공격시 - 공격력 확율 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_ATTACK_PROBABILITY += fDestValue;
        }
        break;
    case DES_SKILL_BUFF_MON_ATTACK_PIERCE:            // 몬스터 공격시 - 피어스 증가 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_ATTACK_PIERCE += fDestValue;
        }
        break;
    case DES_SKILL_BUFF_MON_DEFENCE:                // 몬스터 방어시 - 방어력 증가 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_DEFENCE += fDestValue;            
        }
        break;
    case DES_SKILL_BUFF_MON_DEFENCE_AVOID:            // 몬스터 방어시 - 회피력 증가 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_DEFENCE_AVOID += fDestValue;                        
        }
        break;
        // END 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
    }
}


float GetParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam)
{
    switch(nDestParam)
    {
    case DES_NULL:                        // 대상 파라미터가 없는 경우 사용
    case DES_FACTION_01    :                // 속성 01, check: 추가됨
    case DES_FACTION_02    :                // 속성 02, check: 추가됨
    case DES_OVERHITTIME_01:            // (*) 오버힛회복시간 01, check: 추가됨
    case DES_OVERHITTIME_02:            // (*) 오버힛회복시간 02, check: 추가됨
    case DES_UNITKIND    :                // 기체 업그레이드시 해당 기체
    case DES_ITEMKIND    :                // 아이템의 종류(ITEMKIND_XXX)
    case DES_SUMMON        :                // 몬스터 소환
    case DES_GRADUAL_HP_UP:                // 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel
    //case DES_GRADUAL_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel, check: 추가됨
    case DES_GRADUAL_SP_UP:                // 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
    case DES_GRADUAL_EP_UP:                // 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
    case DES_IN_TIME_HP_UP:                // 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IN_TIME_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌, check: 추가됨
    case DES_IN_TIME_SP_UP:                // 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IN_TIME_EP_UP:                // 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IMMEDIATE_HP_UP:            // 에너지류, 현재 HP를 즉시 VALUE만큼 올려줌
    case DES_IMMEDIATE_DP_UP:            // 에너지류, 현재 DP를 즉시 VALUE만큼 올려줌, check: 추가됨
    case DES_IMMEDIATE_SP_UP:            // 에너지류, 현재 SP를 즉시 VALUE만큼 올려줌
    case DES_IMMEDIATE_EP_UP:            // 에너지류, 현재 EP를 즉시 VALUE만큼 올려줌
    case DES_GROUNDMODE    :                // 폭격모드
    case DES_SIEGEMODE    :                // 시즈모드
    case DES_BULLET_01    :                // (*) 빔소모총알량 01, check: 추가됨
    case DES_BULLET_02    :                // (*) 빔소모총알량 02, check: 추가됨
    case DES_PRIMARY_WEAPON    :            // 1형 무기, 범용
    case DES_SECONDARY_WEAPON:            // 2형 무기, 범용
    case DES_ALL_WEAPON    :                // 1,2형 무기 모두, 범용
        break;
    case DES_ATTACK_PART:                    // 공격 디바이스 파트
        return paramFactor.pfp_ATTACK_PART;
    case DES_DEFENSE_PART:                // 내구 디바이스 파트
        return paramFactor.pfp_DEFENSE_PART;
    case DES_FUEL_PART:                    // 연료 디바이스 파트
        return paramFactor.pfp_FUEL_PART;
    case DES_SOUL_PART:                    // 감응 디바이스 파트
        return paramFactor.pfp_SOUL_PART;
    case DES_SHIELD_PART:                // 방어 디바이스 파트
        return paramFactor.pfp_SHIELD_PART;
    case DES_DODGE_PART    :                // 회피 디바이스 파트
        return paramFactor.pfp_DODGE_PART;
    case DES_BODYCONDITION:                // 몸상태
        return 0;
    case DES_ENDURANCE_01:                // 내구도 01
        return paramFactor.pfp_ENDURANCE_01;
    case DES_ENDURANCE_02:                // 내구도 02
        return paramFactor.pfp_ENDURANCE_02;
    case DES_CHARGING_01    :            // 장탄수 01
        return paramFactor.pfp_CHARGING_01;
    case DES_CHARGING_02    :            // 장탄수 02
        return paramFactor.pfp_CHARGING_02;
    case DES_PROPENSITY    :                // 성향
        return (float)paramFactor.pfp_PROPENSITY;
    case DES_HP            :                // 히트포인트, MAX HP를 +VALUE만큼 올려줌
        return paramFactor.pfp_HP;
    case DES_DP            :                // 쉴드
        return paramFactor.pfp_DP;
    case DES_SP            :                // 소울포인트
        return paramFactor.pfp_SP;
    case DES_EP            :                // 엔진포인트
        return paramFactor.pfp_EP;
    case DES_SPRECOVERY    :                // 소울포인트회복력
        return paramFactor.pfp_SPRECOVERY;
    case DES_HPRECOVERY    :                // 에너지포인트회복력
        return paramFactor.pfp_HPRECOVERY;
    case DES_MINATTACK_01:                // 최소 공격력 01, check: * 
        return paramFactor.pfm_MINATTACK_01;
    case DES_MAXATTACK_01:                // 최대 공격력 01, check: * 
        return paramFactor.pfm_MAXATTACK_01;
    case DES_MINATTACK_02:                // 최소 공격력 02, check: * 
        return paramFactor.pfm_MINATTACK_02;
    case DES_MAXATTACK_02:                // 최대 공격력 02, check: * 
        return paramFactor.pfm_MAXATTACK_02;
    case DES_ATTACKPROBABILITY_01:        // 공격확률 01
        return paramFactor.pfp_ATTACKPROBABILITY_01; // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
    case DES_ATTACKPROBABILITY_02:        // 공격확률 02
        return paramFactor.pfp_ATTACKPROBABILITY_02; // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
    case DES_DEFENSE_01    :                // 방어력 01
        return paramFactor.pfp_DEFENSE_01;           // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
    case DES_DEFENSE_02    :                // 방어력 02
        return paramFactor.pfp_DEFENSE_02;           // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
    case DES_DEFENSEPROBABILITY_01:        // 방어확률 01
        return paramFactor.pfp_DEFENSEPROBABILITY_01; // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
    case DES_DEFENSEPROBABILITY_02:        // 방어확률 02
        return paramFactor.pfp_DEFENSEPROBABILITY_02; // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
    case DES_SKILLPROBABILITY_01    :    // 스킬공격확률 01
    case DES_SKILLPROBABILITY_02    :    // 스킬공격확률 02
        return paramFactor.pfp_SKILLPROBABILITY;
    case DES_FACTIONRESISTANCE_01:        // 속성저항력 01
    case DES_FACTIONRESISTANCE_02:        // 속성저항력 02
        return (float)paramFactor.pfp_FACTIONRESISTANCE;
    case DES_SPEED        :                // 이동속도
        return paramFactor.pfm_SPEED;
    case DES_TRANSPORT    :                // 운반력
        return paramFactor.pfp_TRANSPORT;
    case DES_MATERIAL    :                // 재질
        paramFactor.pfp_MATERIAL;
    case DES_REATTACKTIME_01    :        // (*) 리어택타임 01 (- 증가)
        return paramFactor.pfm_REATTACKTIME_01;
    case DES_REATTACKTIME_02    :        // (*) 리어택타임 02 (- 증가)
        return paramFactor.pfm_REATTACKTIME_02;
    case DES_ABRASIONRATE_01    :        // 마모율 01
        return (float)paramFactor.pfp_ABRASIONRATE_01;
    case DES_ABRASIONRATE_02    :        // 마모율 02
        return (float)paramFactor.pfp_ABRASIONRATE_02;
    case DES_RANGE_01:                    // (*) 유효거리 01
        return paramFactor.pfm_RANGE_01;
    case DES_RANGE_02:                    // (*) 유효거리 02
        return paramFactor.pfm_RANGE_02;
    case DES_RANGEANGLE_01:                // 유효각도 01
        return paramFactor.pfp_RANGEANGLE_01;
    case DES_RANGEANGLE_02:                // 유효각도 02
        return paramFactor.pfp_RANGEANGLE_02;
    case DES_MULTITAGET_01:                // 멀티타겟 01
    case DES_MULTITAGET_02:                // 멀티타겟 02
        return paramFactor.pfp_MULTITAGET;
    case DES_EXPLOSIONRANGE_01:            // 폭발반경 01
        return paramFactor.pfp_EXPLOSIONRANGE_01;
    case DES_EXPLOSIONRANGE_02:            // 폭발반경 02
        return paramFactor.pfp_EXPLOSIONRANGE_02;
    case DES_UNIT    :                    // 유닛의 종류 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
        return paramFactor.pfp_UNIT;
    case DES_REVISION:                    // 유닛의 보정값 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
        return paramFactor.pfp_REVISION;
    case DES_FACTIONPROBABILITY_01:        // 속성에 대한 방어확률 01
    case DES_FACTIONPROBABILITY_02:        // 속성에 대한 방어확률 02
        return (float)paramFactor.pfp_FACTIONPROBABILITY;
    case DES_SHOTNUM_01    :                // 일점사 당 발사 수 01
        return paramFactor.pfp_SHOTNUM_01;
    case DES_SHOTNUM_02    :                // 일점사 당 발사 수 02
        return paramFactor.pfp_SHOTNUM_02;
    case DES_MULTINUM_01    :            // 동시 발사 수 01
        return paramFactor.pfp_MULTINUM_01;
    case DES_MULTINUM_02    :            // 동시 발사 수 02
        return paramFactor.pfp_MULTINUM_02;
    case DES_ATTACKTIME_01:                // 처음 공격 시의 타임 01 (- 증가)
        return paramFactor.pfp_ATTACKTIME_01;
    case DES_ATTACKTIME_02:                // 처음 공격 시의 타임 02 (- 증가)
        return paramFactor.pfp_ATTACKTIME_02;
    case DES_TIME_01        :            // (*) 지속 시간 01, check: + -> *
        return paramFactor.pfm_TIME_01;
    case DES_TIME_02        :            // (*) 지속 시간 02, check: + -> *
        return paramFactor.pfm_TIME_02;
    case DES_WEIGHT_01    :                // (*) 무게 01, check: 추가됨
        return paramFactor.pfm_WEIGHT_01;
    case DES_WEIGHT_02    :                // (*) 무게 02, check: 추가됨
        return paramFactor.pfm_WEIGHT_02;
    case DES_REACTION_RANGE:
        return (float)paramFactor.pfp_REACTION_RANGE;
    case DES_REQ_MIN_LEVEL:
        return paramFactor.pfp_REQ_MIN_LEVEL;
    case DES_REQ_MAX_LEVEL:
        return paramFactor.pfp_REQ_MAX_LEVEL;
    case DES_ATTACK_RANGE_01:
        return paramFactor.pfm_ATTACK_RANGE_01;
    case DES_ATTACK_RANGE_02:
        return paramFactor.pfm_ATTACK_RANGE_02;
    case DES_SKILL_SMARTSP:
        return paramFactor.pfm_SKILL_SMARTSP;
    case DES_SKILL_REACTIONSPEED:
        return paramFactor.pfm_SKILL_REACTIONSPEED;
    case DES_SKILL_ENGINEANGLE:
        return paramFactor.pfm_SKILL_ENGINEANGLE;
    case DES_SKILL_ENGINEBOOSTERANGLE:
        return paramFactor.pfm_SKILL_ENGINEBOOSTERANGLE;
    case DES_SKILL_SLOWMOVING:
        return paramFactor.pfm_SPEED;
    case DES_WARHEAD_SPEED:
        return paramFactor.pfm_WARHEAD_SPEED;
        // 2008-09-26 by bhsohn 신규 인첸트 처리
    case DES_ENGINE_BOOSTER_TIME_UP:    // 부스터 시간 증가
        return paramFactor.pfn_ENGINE_BOOSTER_TIME_UP;            
    case DES_ENGINE_MAX_SPEED_UP:        // 엔진 일반속도(최대) 증가
        return paramFactor.pfn_ENGINE_MAX_SPEED_UP;
    case DES_ENGINE_MIN_SPEED_UP:        // 엔진 일반속도(최소) 증가
        return paramFactor.pfn_ENGINE_MIN_SPEED_UP;
    case DES_ENGINE_BOOSTER_SPEED_UP:        // 엔진 부스터속도 증가
        return paramFactor.pfn_ENGINE_BOOSTER_SPEED_UP;
    case DES_ENGINE_GROUND_SPEED_UP:        // 엔진 지상속도 증가
        return paramFactor.pfn_ENGINE_GROUND_SPEED_UP;
    case DES_RADAR_OBJECT_DETECT_RANGE:        // 레이더 물체 감지 반경
        return paramFactor.pfn_RADAR_OBJECT_DETECT_RANGE;
    case DES_PIERCE_UP_01:        // 기본무기 피어스율 증가 카드
        return paramFactor.pfm_PIERCE_UP_01;
    case DES_PIERCE_UP_02:        // 고급무기 피어스율 증가 카드
        return paramFactor.pfm_PIERCE_UP_02;
    case DES_ENGINE_ANGLE_UP:    // 엔진 회전각 증가 카드
        return paramFactor.pfm_ENGINE_ANGLE_UP;
    case DES_ENGINE_BOOSTERANGLE_UP:    // 엔진 부스터 회전각 증가 카드
        return paramFactor.pfm_ENGINE_BOOSTERANGLE_UP;
        // end 2008-09-26 by bhsohn 신규 인첸트 처리
    // 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
    case DES_RARE_ITEM_PARTNER_SPEED:    // 마을 이동 스피드 증가
        return paramFactor.pf_ITEMS_MOVE_SPEED_RATE;
    // end 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
        // 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
    case DES_SKILL_BUFF_MON_ATTACK_POWER:            // 몬스터 공격시 - 공격력 증가 : Value 증가 %
        return paramFactor.pfp_SKILL_BUFF_MON_ATTACK_POWER;
    case DES_SKILL_BUFF_MON_ATTACK_PROBABILITY:        // 몬스터 공격시 - 공격력 확율 : Value 증가 %
        return paramFactor.pfp_SKILL_BUFF_MON_ATTACK_PROBABILITY;
    case DES_SKILL_BUFF_MON_ATTACK_PIERCE:            // 몬스터 공격시 - 피어스 증가 : Value 증가 %
        return paramFactor.pfp_SKILL_BUFF_MON_ATTACK_PIERCE;
    case DES_SKILL_BUFF_MON_DEFENCE:                // 몬스터 방어시 - 방어력 증가 : Value 증가 %
        return paramFactor.pfp_SKILL_BUFF_MON_DEFENCE;
    case DES_SKILL_BUFF_MON_DEFENCE_AVOID:            // 몬스터 방어시 - 회피력 증가 : Value 증가 %
        return paramFactor.pfp_SKILL_BUFF_MON_DEFENCE_AVOID;
        // END 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
    }

    return 0;
}

void ReleaseParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue)
{
    switch(nDestParam)
    {
    case DES_NULL:                        // 대상 파라미터가 없는 경우 사용
    case DES_FACTION_01    :                // 속성 01, check: 추가됨
    case DES_FACTION_02    :                // 속성 02, check: 추가됨
    case DES_OVERHITTIME_01:            // (*) 오버힛회복시간 01, check: 추가됨
    case DES_OVERHITTIME_02:            // (*) 오버힛회복시간 02, check: 추가됨
    case DES_UNITKIND    :                // 기체 업그레이드시 해당 기체
    case DES_ITEMKIND    :                // 아이템의 종류(ITEMKIND_XXX)
    case DES_SUMMON        :                // 몬스터 소환
    case DES_GRADUAL_HP_UP:                // 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel
    //case DES_GRADUAL_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel, check: 추가됨
    case DES_GRADUAL_SP_UP:                // 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
    case DES_GRADUAL_EP_UP:                // 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
    case DES_IN_TIME_HP_UP:                // 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IN_TIME_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌, check: 추가됨
    case DES_IN_TIME_SP_UP:                // 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IN_TIME_EP_UP:                // 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
    case DES_IMMEDIATE_HP_UP:            // 에너지류, 현재 HP를 즉시 VALUE만큼 올려줌
    case DES_IMMEDIATE_DP_UP:            // 에너지류, 현재 DP를 즉시 VALUE만큼 올려줌, check: 추가됨
    case DES_IMMEDIATE_SP_UP:            // 에너지류, 현재 SP를 즉시 VALUE만큼 올려줌
    case DES_IMMEDIATE_EP_UP:            // 에너지류, 현재 EP를 즉시 VALUE만큼 올려줌
    case DES_GROUNDMODE    :                // 폭격모드
    case DES_SIEGEMODE    :                // 시즈모드
    case DES_BULLET_01    :                // (*) 빔소모총알량 01, check: 추가됨
    case DES_BULLET_02    :                // (*) 빔소모총알량 02, check: 추가됨
    case DES_PRIMARY_WEAPON    :            // 1형 무기, 범용
    case DES_SECONDARY_WEAPON:            // 2형 무기, 범용
    case DES_ALL_WEAPON    :                // 1,2형 무기 모두, 범용
        break;
    case DES_ATTACK_PART:                    // 공격 디바이스 파트
//        paramFactor.pfp_ATTACK_PART -= fDestValue;
        break;
    case DES_DEFENSE_PART:                // 내구 디바이스 파트
//        paramFactor.pfp_DEFENSE_PART -= fDestValue;
        break;
    case DES_FUEL_PART:                    // 연료 디바이스 파트
//        paramFactor.pfp_FUEL_PART -= fDestValue;
        break;
    case DES_SOUL_PART:                    // 감응 디바이스 파트
//        paramFactor.pfp_SOUL_PART -= fDestValue;
        break;
    case DES_SHIELD_PART:                // 방어 디바이스 파트
//        paramFactor.pfp_SHIELD_PART -= fDestValue;
        break;
    case DES_DODGE_PART    :                // 회피 디바이스 파트
//        paramFactor.pfp_DODGE_PART -= fDestValue;
        break;
    case DES_BODYCONDITION:                // 몸상태
    case DES_ENDURANCE_01:                // 내구도 01
        paramFactor.pfp_ENDURANCE_01 -= fDestValue;
        break;
    case DES_ENDURANCE_02:                // 내구도 02
        paramFactor.pfp_ENDURANCE_02 -= fDestValue;
        break;
    case DES_CHARGING_01    :            // 장탄수 01
        paramFactor.pfp_CHARGING_01 -= fDestValue;
        break;
    case DES_CHARGING_02    :            // 장탄수 02
        paramFactor.pfp_CHARGING_02 -= fDestValue;
        break;
    case DES_PROPENSITY    :                // 성향
        paramFactor.pfp_PROPENSITY -= (INT)fDestValue;
        break;
    case DES_HP            :                // 히트포인트, MAX HP를 +VALUE만큼 올려줌
        paramFactor.pfp_HP -= fDestValue;
        break;
    case DES_DP            :                // 쉴드
        paramFactor.pfp_DP -= fDestValue;
        break;
    case DES_MAX_SP_UP  :                // 소울포인트 뻥 아이템
    case DES_SP            :                // 소울포인트
        paramFactor.pfp_SP -= fDestValue;
        break;
    case DES_EP            :                // 엔진포인트
        paramFactor.pfp_EP -= fDestValue;
        break;
    case DES_SPRECOVERY    :                // 소울포인트회복력
        paramFactor.pfp_SPRECOVERY -= fDestValue;
        break;
    case DES_HPRECOVERY    :                // 에너지포인트회복력
        paramFactor.pfp_HPRECOVERY -= fDestValue;
        break;
    case DES_MINATTACK_01:                // 최소 공격력 01, check: * 
        paramFactor.pfm_MINATTACK_01 -= fDestValue;
        break;
    case DES_MAXATTACK_01:                // 최대 공격력 01, check: * 
        paramFactor.pfm_MAXATTACK_01 -= fDestValue;
        break;
    case DES_MINATTACK_02:                // 최소 공격력 02, check: * 
        paramFactor.pfm_MINATTACK_02 -= fDestValue;
        break;
    case DES_MAXATTACK_02:                // 최대 공격력 02, check: * 
        paramFactor.pfm_MAXATTACK_02 -= fDestValue;
        break;
    case DES_ATTACKPROBABILITY_01:        // 공격확률 01
        paramFactor.pfp_ATTACKPROBABILITY_01 -= fDestValue; // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
        break;
    case DES_ATTACKPROBABILITY_02:        // 공격확률 02
        paramFactor.pfp_ATTACKPROBABILITY_02 -= fDestValue; // 2010-07-28 by dgwoo 확률 수식 변경 (255 => 100%)    
        break;

    // 2010. 03. 15 by ckPark 방어력 소수점 계산 버그 수정
//     case DES_DEFENSE_01    :                // 방어력 01
//         paramFactor.pfp_DEFENSE_01 -= fDestValue;
//         break;
//     case DES_DEFENSE_02    :                // 방어력 02
//         paramFactor.pfp_DEFENSE_02 -= fDestValue;
//         break;
    case DES_DEFENSE_01    :                // 방어력 01
        // 2011-09-26 by hsSon, 회피나 방어 아머류를 장착 했다가 벗으면 내 정보에 회피, 방어가 상승되는 버그 수정
        paramFactor.pfp_DEFENSE_01 -= fDestValue;
        // end 2011-09-26 by hsSon, 회피나 방어 아머류를 장착 했다가 벗으면 내 정보에 회피, 방어가 상승되는 버그 수정
        break;
    case DES_DEFENSE_02    :                // 방어력 02
        // 2011-09-26 by hsSon, 회피나 방어 아머류를 장착 했다가 벗으면 내 정보에 회피, 방어가 상승되는 버그 수정
        paramFactor.pfp_DEFENSE_02 -= fDestValue;
        // end 2011-09-26 by hsSon, 회피나 방어 아머류를 장착 했다가 벗으면 내 정보에 회피, 방어가 상승되는 버그 수정
        break;
    // end 2010. 03. 15 by ckPark 방어력 소수점 계산 버그 수정

    case DES_DEFENSEPROBABILITY_01:        // 방어확률 01
        // 2011-09-26 by hsSon, 회피나 방어 아머류를 장착 했다가 벗으면 내 정보에 회피, 방어가 상승되는 버그 수정
        paramFactor.pfp_DEFENSEPROBABILITY_01 -= fDestValue;
        // end 2011-09-26 by hsSon, 회피나 방어 아머류를 장착 했다가 벗으면 내 정보에 회피, 방어가 상승되는 버그 수정
        break;
    case DES_DEFENSEPROBABILITY_02:        // 방어확률 02
        // 2011-09-26 by hsSon, 회피나 방어 아머류를 장착 했다가 벗으면 내 정보에 회피, 방어가 상승되는 버그 수정
        paramFactor.pfp_DEFENSEPROBABILITY_02 -= fDestValue;
        // end 2011-09-26 by hsSon, 회피나 방어 아머류를 장착 했다가 벗으면 내 정보에 회피, 방어가 상승되는 버그 수정
        break;
    case DES_SKILLPROBABILITY_01    :    // 스킬공격확률 01
    case DES_SKILLPROBABILITY_02    :    // 스킬공격확률 02
        paramFactor.pfp_SKILLPROBABILITY -= (Prob255_t)fDestValue;
        break;
    case DES_FACTIONRESISTANCE_01:        // 속성저항력 01
    case DES_FACTIONRESISTANCE_02:        // 속성저항력 02
        paramFactor.pfp_FACTIONRESISTANCE -= (Prob255_t)fDestValue;
        break;
    case DES_SPEED        :                // 이동속도
        paramFactor.pfm_SPEED -= fDestValue;
        break;
    case DES_TRANSPORT    :                // 운반력
        paramFactor.pfp_TRANSPORT -= fDestValue;
        break;
    case DES_MATERIAL    :                // 재질
        paramFactor.pfp_MATERIAL -= fDestValue;
        break;
    case DES_REATTACKTIME_01    :        // (*) 리어택타임 01 (- 증가)
        paramFactor.pfm_REATTACKTIME_01 -= fDestValue;
        break;
    case DES_REATTACKTIME_02    :        // (*) 리어택타임 02 (- 증가)
        paramFactor.pfm_REATTACKTIME_02 -= fDestValue;
        break;
    case DES_ABRASIONRATE_01    :        // 마모율 01
        paramFactor.pfp_ABRASIONRATE_01 -= (Prob255_t)fDestValue;
        break;
    case DES_ABRASIONRATE_02    :        // 마모율 02
        paramFactor.pfp_ABRASIONRATE_02 -= (Prob255_t)fDestValue;
        break;
    case DES_RANGE_01:                    // (*) 유효거리 01
        paramFactor.pfm_RANGE_01 -= fDestValue;
        break;
    case DES_RANGE_02:                    // (*) 유효거리 02
        paramFactor.pfm_RANGE_02 -= fDestValue;
        break;
    case DES_RANGEANGLE_01:                // 유효각도 01
        paramFactor.pfp_RANGEANGLE_01 -= fDestValue;
        break;
    case DES_RANGEANGLE_02:                // 유효각도 02
        paramFactor.pfp_RANGEANGLE_02 -= fDestValue;
        break;
    case DES_MULTITAGET_01:                // 멀티타겟 01
    case DES_MULTITAGET_02:                // 멀티타겟 02
        paramFactor.pfp_MULTITAGET -= fDestValue;
        break;
    case DES_EXPLOSIONRANGE_01:            // 폭발반경 01
        paramFactor.pfp_EXPLOSIONRANGE_01 -= fDestValue;
        break;
    case DES_EXPLOSIONRANGE_02:            // 폭발반경 02
        paramFactor.pfp_EXPLOSIONRANGE_02 -= fDestValue;
        break;
    case DES_UNIT    :                    // 유닛의 종류 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
        paramFactor.pfp_UNIT -= fDestValue;
        break;
    case DES_REVISION:                    // 유닛의 보정값 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
        paramFactor.pfp_REVISION -= fDestValue;
        break;
    case DES_FACTIONPROBABILITY_01:        // 속성에 대한 방어확률 01
    case DES_FACTIONPROBABILITY_02:        // 속성에 대한 방어확률 02
        paramFactor.pfp_FACTIONPROBABILITY -= (Prob255_t)fDestValue;
        break;
    case DES_SHOTNUM_01    :                // 일점사 당 발사 수 01
        paramFactor.pfp_SHOTNUM_01 -= (int)fDestValue;
        break;
    case DES_SHOTNUM_02    :                // 일점사 당 발사 수 02
        paramFactor.pfp_SHOTNUM_02 -= (int)fDestValue;
        break;
    case DES_MULTINUM_01    :            // 동시 발사 수 01
        paramFactor.pfp_MULTINUM_01 -= (int)fDestValue;
        break;
    case DES_MULTINUM_02    :            // 동시 발사 수 02
        paramFactor.pfp_MULTINUM_02 -= (int)fDestValue;
        break;
    case DES_ATTACKTIME_01:                // 처음 공격 시의 타임 01 (- 증가)
        paramFactor.pfp_ATTACKTIME_01 -= fDestValue;
        break;
    case DES_ATTACKTIME_02:                // 처음 공격 시의 타임 02 (- 증가)
        paramFactor.pfp_ATTACKTIME_02 -= fDestValue;
        break;
    case DES_TIME_01        :            // (*) 지속 시간 01, check: + -> *
        paramFactor.pfm_TIME_01 -= fDestValue;
        break;
    case DES_TIME_02        :            // (*) 지속 시간 02, check: + -> *
        paramFactor.pfm_TIME_02 -= fDestValue;
        break;
    case DES_WEIGHT_01    :                // (*) 무게 01, check: 추가됨
        paramFactor.pfm_WEIGHT_01 -= fDestValue;
        break;
    case DES_WEIGHT_02    :                // (*) 무게 02, check: 추가됨
        paramFactor.pfm_WEIGHT_02 -= fDestValue;
        break;
    case DES_REACTION_RANGE:
        paramFactor.pfp_REACTION_RANGE -= (USHORT)fDestValue;
        break;
    case DES_REQ_MIN_LEVEL:
        paramFactor.pfp_REQ_MIN_LEVEL -= (int)fDestValue;
        break;
    case DES_REQ_MAX_LEVEL:
        paramFactor.pfp_REQ_MAX_LEVEL -= (int)fDestValue;
        break;
    case DES_ATTACK_RANGE_01:
        paramFactor.pfm_ATTACK_RANGE_01 -= fDestValue;
        break;
    case DES_ATTACK_RANGE_02:
        paramFactor.pfm_ATTACK_RANGE_02 -= fDestValue;
        break;
    case DES_SKILL_SMARTSP:
        paramFactor.pfm_SKILL_SMARTSP -= fDestValue;
        break;
    case DES_SKILL_REACTIONSPEED:
        paramFactor.pfm_SKILL_REACTIONSPEED -= fDestValue;
        break;
    case DES_SKILL_ENGINEANGLE:
        paramFactor.pfm_SKILL_ENGINEANGLE -= fDestValue;
        break;
    case DES_SKILL_ENGINEBOOSTERANGLE:
        paramFactor.pfm_SKILL_ENGINEBOOSTERANGLE -= fDestValue;
        break;    
    case DES_SKILL_SLOWMOVING:
        paramFactor.pfm_SPEED -= fDestValue;
        break;    
    // 2006-10-11 by ispark, 하이퍼 부스터는 값 셋팅이 아닌 설정 값이다.
    case DES_HYPER_BOOSTER:
        g_pShuttleChild->m_nHyperBooster = HYPER_BOOSTER_NORMAL;
        break;
    case DES_GRADUAL_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel, check: 추가됨
        paramFactor.pfm_DP_REPAIR_RATE -= fDestValue;
        break;
    case DES_SKILL_NO_WARNING:
        g_pShuttleChild->SetSkillMissileWarning(FALSE);
        break;
    case DES_WARHEAD_SPEED:
        paramFactor.pfm_WARHEAD_SPEED -= fDestValue;
        break;
    // 2008-09-26 by bhsohn 신규 인첸트 처리
    case DES_ENGINE_BOOSTER_TIME_UP:    // 부스터 시간 증가
        {
            paramFactor.pfn_ENGINE_BOOSTER_TIME_UP -= fDestValue;            
        }
        break;
    case DES_ENGINE_MAX_SPEED_UP:        // 엔진 일반속도(최대) 증가
        {
            paramFactor.pfn_ENGINE_MAX_SPEED_UP -= fDestValue;
        }
        break;
    case DES_ENGINE_MIN_SPEED_UP:        // 엔진 일반속도(최소) 증가
        {
            paramFactor.pfn_ENGINE_MIN_SPEED_UP-= fDestValue;
        }
        break;
    case DES_ENGINE_BOOSTER_SPEED_UP:        // 엔진 부스터속도 증가
        {
            paramFactor.pfn_ENGINE_BOOSTER_SPEED_UP-= fDestValue;
        }
        break;
    case DES_ENGINE_GROUND_SPEED_UP:        // 엔진 지상속도 증가
        {
            paramFactor.pfn_ENGINE_GROUND_SPEED_UP-= fDestValue;
        }
        break;
    case DES_RADAR_OBJECT_DETECT_RANGE:        // 레이더 물체 감지 반경
        {
            paramFactor.pfn_RADAR_OBJECT_DETECT_RANGE-= fDestValue;
        }
        break;
    case DES_PIERCE_UP_01:        // 기본무기 피어스율 증가 카드
        {
            paramFactor.pfm_PIERCE_UP_01-= fDestValue;
        }
        break;
    case DES_PIERCE_UP_02:        // 고급무기 피어스율 증가 카드
        {
            paramFactor.pfm_PIERCE_UP_02-= fDestValue;
        }
        break;
    case DES_ENGINE_ANGLE_UP:    // 엔진 회전각 증가 카드
        {
            paramFactor.pfm_ENGINE_ANGLE_UP -= fDestValue;

        }
        break;
    case DES_ENGINE_BOOSTERANGLE_UP:    // 엔진 부스터 회전각 증가 카드
        {
            paramFactor.pfm_ENGINE_BOOSTERANGLE_UP -= fDestValue;
        }
        break;
        // end 2008-09-26 by bhsohn 신규 인첸트 처리
        // 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
    case DES_RARE_ITEM_PARTNER_SPEED:    // 마을 이동 스피드 증가
        {
            paramFactor.pf_ITEMS_MOVE_SPEED_RATE -= fDestValue;
        }
        break;
    // end 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
        // 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
    case DES_SKILL_BUFF_MON_ATTACK_POWER:            // 몬스터 공격시 - 공격력 증가 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_ATTACK_POWER -= fDestValue;
        }
        break;
    case DES_SKILL_BUFF_MON_ATTACK_PROBABILITY:        // 몬스터 공격시 - 공격력 확율 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_ATTACK_PROBABILITY -= fDestValue;
        }
        break;
    case DES_SKILL_BUFF_MON_ATTACK_PIERCE:            // 몬스터 공격시 - 피어스 증가 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_ATTACK_PIERCE -= fDestValue;
        }
        break;
    case DES_SKILL_BUFF_MON_DEFENCE:                // 몬스터 방어시 - 방어력 증가 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_DEFENCE -= fDestValue;            
        }
        break;
    case DES_SKILL_BUFF_MON_DEFENCE_AVOID:            // 몬스터 방어시 - 회피력 증가 : Value 증가 %
        {
            paramFactor.pfp_SKILL_BUFF_MON_DEFENCE_AVOID -= fDestValue;                        
        }
        break;
        // END 2013-05-15 by bhsohn 세력포인트 개선안 버프 시스템
    }
}

void ParamFactor_plus_ParamFactor( CParamFactor &destParamFactor, CParamFactor &srcParamFactor )
{
    destParamFactor.pfp_ATTACK_PART        +=        srcParamFactor.pfp_ATTACK_PART;                
    destParamFactor.pfp_DEFENSE_PART    +=        srcParamFactor.pfp_DEFENSE_PART;            
    destParamFactor.pfp_FUEL_PART        +=        srcParamFactor.pfp_FUEL_PART;                
    destParamFactor.pfp_SOUL_PART        +=        srcParamFactor.pfp_SOUL_PART;                
    destParamFactor.pfp_SHIELD_PART        +=        srcParamFactor.pfp_SHIELD_PART;            
    destParamFactor.pfp_DODGE_PART        +=        srcParamFactor.pfp_DODGE_PART;                
    destParamFactor.pfp_BODYCONDITION    +=        srcParamFactor.pfp_BODYCONDITION;            
    destParamFactor.pfp_ENDURANCE_01     +=        srcParamFactor.pfp_ENDURANCE_01;            
    destParamFactor.pfp_ENDURANCE_02    +=        srcParamFactor.pfp_ENDURANCE_02;            
    destParamFactor.pfp_CHARGING_01        +=        srcParamFactor.pfp_CHARGING_01;            
    destParamFactor.pfp_CHARGING_02        +=        srcParamFactor.pfp_CHARGING_02;            
    destParamFactor.pfp_PROPENSITY        +=        srcParamFactor.pfp_PROPENSITY;                
    destParamFactor.pfp_HP                +=        srcParamFactor.pfp_HP;                        
    destParamFactor.pfp_SP                +=        srcParamFactor.pfp_SP;                        
    destParamFactor.pfp_EP                +=        srcParamFactor.pfp_EP;                        
    destParamFactor.pfp_DP                +=        srcParamFactor.pfp_DP;                        
    destParamFactor.pfp_SPRECOVERY        +=        srcParamFactor.pfp_SPRECOVERY;                
    destParamFactor.pfp_HPRECOVERY        +=        srcParamFactor.pfp_HPRECOVERY;                
    destParamFactor.pfm_MINATTACK_01    +=        srcParamFactor.pfm_MINATTACK_01;            
    destParamFactor.pfm_MAXATTACK_01    +=        srcParamFactor.pfm_MAXATTACK_01;            
    destParamFactor.pfm_MINATTACK_02    +=        srcParamFactor.pfm_MINATTACK_02;            
    destParamFactor.pfm_MAXATTACK_02    +=        srcParamFactor.pfm_MAXATTACK_02;            
    destParamFactor.pfp_ATTACKPROBABILITY_01+=    srcParamFactor.pfp_ATTACKPROBABILITY_01;    
    destParamFactor.pfp_ATTACKPROBABILITY_02+=    srcParamFactor.pfp_ATTACKPROBABILITY_02;    
    destParamFactor.pfp_DEFENSE_01        +=        srcParamFactor.pfp_DEFENSE_01;                
    destParamFactor.pfp_DEFENSE_02        +=        srcParamFactor.pfp_DEFENSE_02;                
    destParamFactor.pfp_DEFENSEPROBABILITY_01+=    srcParamFactor.pfp_DEFENSEPROBABILITY_01;    
    destParamFactor.pfp_DEFENSEPROBABILITY_02+=    srcParamFactor.pfp_DEFENSEPROBABILITY_02;    
    destParamFactor.pfp_SKILLPROBABILITY    +=    srcParamFactor.pfp_SKILLPROBABILITY;        
    destParamFactor.pfp_FACTIONRESISTANCE    +=    srcParamFactor.pfp_FACTIONRESISTANCE;        
    destParamFactor.pfm_SPEED            +=        srcParamFactor.pfm_SPEED;                    
    destParamFactor.pfp_TRANSPORT        +=        srcParamFactor.pfp_TRANSPORT;                
    destParamFactor.pfp_MATERIAL        +=        srcParamFactor.pfp_MATERIAL;                
    destParamFactor.pfm_REATTACKTIME_01    +=        srcParamFactor.pfm_REATTACKTIME_01;        
    destParamFactor.pfm_REATTACKTIME_02    +=        srcParamFactor.pfm_REATTACKTIME_02;        
    destParamFactor.pfp_ABRASIONRATE_01    +=        srcParamFactor.pfp_ABRASIONRATE_01;        
    destParamFactor.pfp_ABRASIONRATE_02    +=        srcParamFactor.pfp_ABRASIONRATE_02;        
    destParamFactor.pfm_RANGE_01        +=        srcParamFactor.pfm_RANGE_01;                
    destParamFactor.pfm_RANGE_02        +=        srcParamFactor.pfm_RANGE_02;                
    destParamFactor.pfp_RANGEANGLE_01    +=        srcParamFactor.pfp_RANGEANGLE_01;            
    destParamFactor.pfp_RANGEANGLE_02    +=        srcParamFactor.pfp_RANGEANGLE_02;            
    destParamFactor.pfp_MULTITAGET        +=        srcParamFactor.pfp_MULTITAGET;                
    destParamFactor.pfp_EXPLOSIONRANGE_01 +=    srcParamFactor.pfp_EXPLOSIONRANGE_01;            
    destParamFactor.pfp_EXPLOSIONRANGE_02 +=    srcParamFactor.pfp_EXPLOSIONRANGE_02;            
    destParamFactor.pfp_UNIT            +=        srcParamFactor.pfp_UNIT;                    
    destParamFactor.pfp_REVISION        +=        srcParamFactor.pfp_REVISION;                
    destParamFactor.pfp_FACTIONPROBABILITY    +=    srcParamFactor.pfp_FACTIONPROBABILITY;        
    destParamFactor.pfp_SHOTNUM_01        +=        srcParamFactor.pfp_SHOTNUM_01;
    destParamFactor.pfp_SHOTNUM_02        +=        srcParamFactor.pfp_SHOTNUM_02;                
    destParamFactor.pfp_MULTINUM_01        +=        srcParamFactor.pfp_MULTINUM_01;                
    destParamFactor.pfp_MULTINUM_02        +=        srcParamFactor.pfp_MULTINUM_02;                
    destParamFactor.pfp_ATTACKTIME_01    +=        srcParamFactor.pfp_ATTACKTIME_01;            
    destParamFactor.pfp_ATTACKTIME_02    +=        srcParamFactor.pfp_ATTACKTIME_02;            
    destParamFactor.pfm_TIME_01            +=        srcParamFactor.pfm_TIME_01;                
    destParamFactor.pfm_TIME_02            +=        srcParamFactor.pfm_TIME_02;                
    destParamFactor.pfm_WEIGHT_01        +=        srcParamFactor.pfm_WEIGHT_01;                    
    destParamFactor.pfm_WEIGHT_02        +=        srcParamFactor.pfm_WEIGHT_02;                    
    destParamFactor.pfp_REACTION_RANGE    +=        srcParamFactor.pfp_REACTION_RANGE;                    
    destParamFactor.pfp_REQ_MIN_LEVEL    +=        srcParamFactor.pfp_REQ_MIN_LEVEL;
    destParamFactor.pfp_REQ_MAX_LEVEL    +=        srcParamFactor.pfp_REQ_MAX_LEVEL;
    destParamFactor.pfm_ATTACK_RANGE_01 +=        srcParamFactor.pfm_ATTACK_RANGE_01;
    destParamFactor.pfm_ATTACK_RANGE_02 +=        srcParamFactor.pfm_ATTACK_RANGE_02;
    destParamFactor.pfm_SKILL_SMARTSP    +=        srcParamFactor.pfm_SKILL_SMARTSP;
    destParamFactor.pfm_SKILL_REACTIONSPEED +=    srcParamFactor.pfm_SKILL_REACTIONSPEED;
    destParamFactor.pfm_SKILL_ENGINEANGLE +=    srcParamFactor.pfm_SKILL_ENGINEANGLE;
    destParamFactor.pfm_SKILL_ENGINEBOOSTERANGLE +=    srcParamFactor.pfm_SKILL_ENGINEBOOSTERANGLE;
    destParamFactor.pfm_DP_REPAIR_RATE += srcParamFactor.pfm_DP_REPAIR_RATE;
    destParamFactor.pfm_WARHEAD_SPEED += srcParamFactor.pfm_WARHEAD_SPEED;

    // 2008-09-26 by bhsohn 신규 인첸트 처리
    destParamFactor.pfn_ENGINE_BOOSTER_TIME_UP += srcParamFactor.pfn_ENGINE_BOOSTER_TIME_UP;    // 부스터 시간 증가
    destParamFactor.pfn_ENGINE_MAX_SPEED_UP += srcParamFactor.pfn_ENGINE_MAX_SPEED_UP;    // 엔진 일반속도(최대) 증가    
    destParamFactor.pfn_ENGINE_MIN_SPEED_UP += srcParamFactor.pfn_ENGINE_MIN_SPEED_UP;    // 엔진 일반속도(최소) 증가
    destParamFactor.pfn_ENGINE_BOOSTER_SPEED_UP += srcParamFactor.pfn_ENGINE_BOOSTER_SPEED_UP;    // 엔진 부스터속도 증가
    destParamFactor.pfn_ENGINE_GROUND_SPEED_UP += srcParamFactor.pfn_ENGINE_GROUND_SPEED_UP;    // 엔진 지상속도 증가
    destParamFactor.pfn_RADAR_OBJECT_DETECT_RANGE += srcParamFactor.pfn_RADAR_OBJECT_DETECT_RANGE;    // 레이더 물체 감지 반경
    destParamFactor.pfm_PIERCE_UP_01 += srcParamFactor.pfm_PIERCE_UP_01;    // 기본무기 피어스율 증가 카드
    destParamFactor.pfm_PIERCE_UP_02 += srcParamFactor.pfm_PIERCE_UP_02;    // 고급무기 피어스율 증가 카드
    destParamFactor.pfm_ENGINE_ANGLE_UP += srcParamFactor.pfm_ENGINE_ANGLE_UP;    // 엔진 회전각 증가 카드
    destParamFactor.pfm_ENGINE_BOOSTERANGLE_UP += srcParamFactor.pfm_ENGINE_BOOSTERANGLE_UP;    // 엔진 부스터 회전각 증가 카드    
    // end 2008-09-26 by bhsohn 신규 인첸트 처리
}

void SetItem_ParamFactor( ITEM & item, CParamFactor &paramFactor )
{
    item.ReqGearStat.AttackPart += paramFactor.pfp_ATTACK_PART;// 공격 디바이스 파트
    item.ReqGearStat.DefensePart += paramFactor.pfp_DEFENSE_PART;// 내구 디바이스 파트
    item.ReqGearStat.FuelPart += paramFactor.pfp_FUEL_PART;// 연료 디바이스 파트
    item.ReqGearStat.SoulPart += paramFactor.pfp_SOUL_PART;// 감응 디바이스 파트
    item.ReqGearStat.ShieldPart += paramFactor.pfp_SHIELD_PART;// 방어 디바이스 파트
    item.ReqGearStat.DodgePart += paramFactor.pfp_DODGE_PART;    // 회피 디바이스 파트
    item.SkillHitRate += paramFactor.pfp_SKILLPROBABILITY;// 스킬공격확률
    item.FractionResistance += paramFactor.pfp_FACTIONRESISTANCE;// 속성저항력
//    item.AbilityMax += paramFactor.pfp_SPEED;// 이동속도
    item.MultiTarget += paramFactor.pfp_MULTITAGET;    // 멀티타겟
    item.ReactionRange += paramFactor.pfp_REACTION_RANGE;

    // 2005-03-28 by jschoi - 서버 요청으로 조건문 추가
    if(item.ReqMinLevel != 0)
    {
        item.ReqMinLevel = max(0, (int)item.ReqMinLevel + paramFactor.pfp_REQ_MIN_LEVEL);
    }
    // 2005-03-28 by jschoi - 서버 요청으로 조건문 추가
    if(item.ReqMaxLevel != 0)
    {
        item.ReqMaxLevel += paramFactor.pfp_REQ_MAX_LEVEL;
    }

    if(IS_PRIMARY_WEAPON(item.Kind))
    {
        item.Endurance += paramFactor.pfp_ENDURANCE_01;    // 내구도 01
        item.Charging += paramFactor.pfp_CHARGING_01;    // 장탄수 01
        item.AbilityMin *= (1.0f+paramFactor.pfm_MINATTACK_01);    // 최소 공격력 01, check: * 
        item.AbilityMax *= (1.0f+paramFactor.pfm_MAXATTACK_01);    // 최대 공격력 01, check: * 
        item.Defense += paramFactor.pfp_DEFENSE_01;    // 방어력 01
        item.HitRate += paramFactor.pfp_ATTACKPROBABILITY_01;    // 공격확률 01
        item.ReAttacktime *= (1.0f+paramFactor.pfm_REATTACKTIME_01);    // (*) 리어택타임 01 (- 증가)
        item.AbrasionRate += paramFactor.pfp_ABRASIONRATE_01;    // 마모율 01
        item.Range *= (1.0f+paramFactor.pfm_RANGE_01);    // (*) 유효거리 01
        item.RangeAngle += paramFactor.pfp_RANGEANGLE_01;    // 유효각도 01
        item.ShotNum += paramFactor.pfp_SHOTNUM_01;    // 일점사 당 발사 수
        item.MultiNum += paramFactor.pfp_MULTINUM_01;    // 동시 발사 수
        item.AttackTime += paramFactor.pfp_ATTACKTIME_01;// 처음 공격 시의 타임 01 (- 증가)
        item.Time *= (1.0f+paramFactor.pfm_TIME_01);    // (*) 지속 시간 01, check: + -> *
        item.ExplosionRange += paramFactor.pfp_EXPLOSIONRANGE_01;    // 폭발반경
        item.Weight *= (1.0f+paramFactor.pfm_WEIGHT_01);    // (*) 무게 01, check: 추가됨
    }
    else if (IS_SECONDARY_WEAPON(item.Kind))
    {
        item.Endurance += paramFactor.pfp_ENDURANCE_02;    // 내구도 02
        item.Charging += paramFactor.pfp_CHARGING_02;    // 장탄수 02
        item.AbilityMin *= (1.0f+paramFactor.pfm_MINATTACK_02);    // 최소 공격력 02, check: * 
        item.AbilityMax *= (1.0f+paramFactor.pfm_MAXATTACK_02);    // 최대 공격력 02, check: * 
        item.Defense += paramFactor.pfp_DEFENSE_02;    // 방어력 02
        item.HitRate += paramFactor.pfp_ATTACKPROBABILITY_02;    // 공격확률 02
        item.ReAttacktime *= (1.0f+paramFactor.pfm_REATTACKTIME_02);    // (*) 리어택타임 02 (- 증가)
        item.AbrasionRate += paramFactor.pfp_ABRASIONRATE_02;    // 마모율 02
        item.Range *= (1.0f+paramFactor.pfm_RANGE_02);    // (*) 유효거리 02
        item.RangeAngle += paramFactor.pfp_RANGEANGLE_02;    // 유효각도 02
        item.ShotNum += paramFactor.pfp_SHOTNUM_02;    // 일점사 당 발사 수
        item.MultiNum += paramFactor.pfp_MULTINUM_02;    // 동시 발사 수
        item.AttackTime += paramFactor.pfp_ATTACKTIME_02;    // 처음 공격 시의 타임 02 (- 증가)
        item.Time *= (1.0f+paramFactor.pfm_TIME_02);    // (*) 지속 시간 02, check: + -> *
        item.ExplosionRange += paramFactor.pfp_EXPLOSIONRANGE_02;    // 폭발반경
        item.Weight *= (1.0f+paramFactor.pfm_WEIGHT_02);    // (*) 무게 02, check: 추가됨
        item.RepeatTime *= (1.0f+paramFactor.pfm_WARHEAD_SPEED);    // 이동 속도
    }
//    case DES_NULL:                        // 대상 파라미터가 없는 경우 사용
//    case DES_BODYCONDITION:                // 몸상태
//    case DES_PROPENSITY    :                // 성향
//    case DES_HP            :                // 히트포인트, MAX HP를 +VALUE만큼 올려줌
//    case DES_SP            :                // 소울포인트
//    case DES_EP            :                // 엔진포인트
//    case DES_SPRECOVERY    :                // 소울포인트회복력
//    case DES_HPRECOVERY    :                // 에너지포인트회복력
//    case DES_DEFENSEPROBABILITY_01:        // 방어확률 01
//    case DES_DEFENSEPROBABILITY_02:        // 방어확률 02
//    case DES_FACTION_01    :                // 속성 01, check: 추가됨
//    case DES_FACTION_02    :                // 속성 02, check: 추가됨
//    case DES_TRANSPORT    :                // 운반력
//    case DES_MATERIAL    :                // 재질
//    case DES_UNIT    :                    // 유닛의 종류 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
//    case DES_REVISION:                    // 유닛의 보정값 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
//    case DES_FACTIONPROBABILITY_01:        // 속성에 대한 방어확률 01
//    case DES_FACTIONPROBABILITY_02:        // 속성에 대한 방어확률 02
//    case DES_OVERHITTIME_01:            // (*) 오버힛회복시간 01, check: 추가됨
//    case DES_OVERHITTIME_02:            // (*) 오버힛회복시간 02, check: 추가됨
//    case DES_UNITKIND    :                // 기체 업그레이드시 해당 기체
//    case DES_ITEMKIND    :                // 아이템의 종류(ITEMKIND_XXX)
//    case DES_SUMMON        :                // 몬스터 소환
//    case DES_GRADUAL_HP_UP:                // 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel
//    case DES_GRADUAL_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 부스터 켜거나 공격 받으면 cancel, check: 추가됨
//    case DES_GRADUAL_SP_UP:                // 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
//    case DES_GRADUAL_EP_UP:                // 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)동안 점진적으로 VALUE만큼 올려줌, 중간에 뭐(?)들어오면 cancel
//    case DES_IN_TIME_HP_UP:                // 에너지류, 현재 HP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
//    case DES_IN_TIME_DP_UP:                // 에너지류, 현재 DP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌, check: 추가됨
//    case DES_IN_TIME_SP_UP:                // 에너지류, 현재 SP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
//    case DES_IN_TIME_EP_UP:                // 에너지류, 현재 EP를 일정 시간(ITEM.Time에 저장)이 지난 후 VALUE만큼 올려줌
//    case DES_IMMEDIATE_HP_UP:            // 에너지류, 현재 HP를 즉시 VALUE만큼 올려줌
//    case DES_IMMEDIATE_DP_UP:            // 에너지류, 현재 DP를 즉시 VALUE만큼 올려줌, check: 추가됨
//    case DES_IMMEDIATE_SP_UP:            // 에너지류, 현재 SP를 즉시 VALUE만큼 올려줌
//    case DES_IMMEDIATE_EP_UP:            // 에너지류, 현재 EP를 즉시 VALUE만큼 올려줌
//    case DES_GROUNDMODE    :                // 폭격모드
//    case DES_SIEGEMODE    :                // 시즈모드
//    case DES_BEAMBULLET_01:                // (*) 빔소모총알량 01, check: 추가됨
//    case DES_BEAMBULLET_02    :            // (*) 빔소모총알량 02, check: 추가됨
//    case DES_PRIMARY_WEAPON    :            // 1형 무기, 범용
//    case DES_SECONDARY_WEAPON:            // 2형 무기, 범용
//    case DES_ALL_WEAPON    :                // 1,2형 무기 모두, 범용
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CompareIntRange(INT_RANGE &range, INT nValue)
/// \brief        최대/최소가 존재하는 값의 비교
/// \author        dhkwon
/// \date        2004-04-28 ~ 2004-04-28
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CompareIntRange(INT_RANGE &range, INT nValue)
{
    if( (range.Min == 0 ? TRUE : range.Min <= nValue) &&
        (range.Max == 0 ? TRUE : range.Max >= nValue) )
    {
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CompareBitFlag( USHORT nSource, USHORT nValue )
/// \brief        비트 플래그 비교
/// \author        dhkwon
/// \date        2004-04-28 ~ 2004-04-28
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CompareBitFlag( USHORT nSource, USHORT nValue )
{
    if(nSource == 0 ? TRUE : nSource & nValue )
    {
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CompareItemCount( ITEM_W_COUNT &itemCount )
/// \brief        Item count가 만족하는지 검사한다.
/// \author        dhkwon
/// \date        2004-04-29 ~ 2004-04-29
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CompareItemCount( ITEM_W_COUNT &itemCount )
{
    ASSERT_ASSERT(g_pStoreData);
    if( g_pStoreData->GetCurrentCountItemInInventoryByItemNum( itemCount.ItemNum ) >= itemCount.Count )
    {
        return TRUE;
    }
    return FALSE;
}

// 각 기어별 잔상 이펙트
int GetGearTrace(UINT nUnitKind)
{
    if(IS_BT(nUnitKind))
        return RC_EFF_BGEAR_TRACE;
    if(IS_OT(nUnitKind))
        return RC_EFF_MGEAR_TRACE;
    if(IS_DT(nUnitKind))
        return RC_EFF_AGEAR_TRACE;
    if(IS_ST(nUnitKind))
        return RC_EFF_IGEAR_TRACE;
    return 0;
}

char* GetGuildRankName( BYTE nGuildRank )
{
    switch(nGuildRank)
    {
    case GUILD_RANK_PRIVATE_NULL:
        return STRCMD_CS_GUILD_RANK_PRIVATE_NULL;
    case GUILD_RANK_COMMANDER:
        return STRCMD_CS_GUILD_RANK_COMMANDER;
    case GUILD_RANK_SUBCOMMANDER:
        return STRCMD_CS_GUILD_RANK_SUBCOMMANDER;        
    case GUILD_RANK_SQUAD_LEADER_1:
        return STRCMD_CS_GUILD_RANK_SQUAD_LEADER_1;
    case GUILD_RANK_PRIVATE_1: 
        return STRCMD_CS_GUILD_RANK_PRIVATE_1;
    case GUILD_RANK_SQUAD_LEADER_2:
        return STRCMD_CS_GUILD_RANK_SQUAD_LEADER_2;
    case GUILD_RANK_PRIVATE_2:
        return STRCMD_CS_GUILD_RANK_PRIVATE_2;
    case GUILD_RANK_SQUAD_LEADER_3:
        return STRCMD_CS_GUILD_RANK_SQUAD_LEADER_3;
    case GUILD_RANK_PRIVATE_3:
        return STRCMD_CS_GUILD_RANK_PRIVATE_3;
    case GUILD_RANK_SQUAD_LEADER_4:
        return STRCMD_CS_GUILD_RANK_SQUAD_LEADER_4;
    case GUILD_RANK_PRIVATE_4:
        return STRCMD_CS_GUILD_RANK_PRIVATE_4;
    case GUILD_RANK_SQUAD_LEADER_5:
        return STRCMD_CS_GUILD_RANK_SQUAD_LEADER_5;
    case GUILD_RANK_PRIVATE_5:
        return STRCMD_CS_GUILD_RANK_PRIVATE_5;
    }
    // end 2008-06-03 by bhsohn EP3 여단 관련 처리
    return NULL;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            int GGetUnitUpgradeLevel(int x) 
/// \brief        Unit의 업그레이드 단계를 리턴한다.
/// \author        dhkwon
/// \date        2004-03-24 ~ 2004-03-24
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
unsigned short GGetUnitUpgradeLevel(unsigned short i_nUnitKind) 
{
    FLOG( "GGetUnitUpgradeLevel(unsigned short i_nUnitKind)" );
    unsigned short nHexLevel = 1;    
    if(IS_BT (i_nUnitKind)) nHexLevel = i_nUnitKind - 0x0000;
    else if (IS_OT(i_nUnitKind)) nHexLevel = i_nUnitKind - 0x000F;
    else if (IS_DT(i_nUnitKind)) nHexLevel = i_nUnitKind - 0x00FF;
    else if (IS_ST(i_nUnitKind)) nHexLevel = i_nUnitKind - 0x0FFF;
    
    if (nHexLevel&1) return 1;
    else if (nHexLevel&2) return 2;
    else if (nHexLevel&4) return 3;
    else if (nHexLevel&8) return 4; 
    return 0;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            unsigned short GGetUnitKindHexToDeimal(unsigned short i_nUnitKind)
/// \brief        유닛카인드를 십진수로 변환한다.(0010 --> 21, 0400 --> 32...)
/// \author        dhkwon
/// \date        2004-03-24 ~ 2004-03-24
/// \warning    이펙트 번호, 아이템 번호 등은 유닛카인드가 10진수로 쓰인다.
///                두자리로 이루어지는데 앞번호는 유닛카인드, 뒷번호는 업그레이드레벨
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
unsigned short GGetUnitKindHexToDeimal(unsigned short i_nUnitKind)
{
/*    unsigned short nUpgradeLevel = GGetUnitUpgradeLevel( i_nUnitKind );

    if(IS_BT(i_nUnitKind)) return nUpgradeLevel + UNIT_KIND_SHIFT_B_GEAR;
    else if(IS_OT(i_nUnitKind)) return nUpgradeLevel + UNIT_KIND_SHIFT_M_GEAR;
    else if(IS_DT(i_nUnitKind)) return nUpgradeLevel + UNIT_KIND_SHIFT_A_GEAR;
    else if(IS_ST(i_nUnitKind)) return nUpgradeLevel + UNIT_KIND_SHIFT_I_GEAR;
*/
    switch (i_nUnitKind)
    {
    case UNITKIND_BT01:
        return 1;
    case UNITKIND_OT01:
        return 11;
    case UNITKIND_DT01:
        return 21;
    case UNITKIND_ST01:
        return 31;
    }
    return 0;

}

int GetUnitTypeDecimal(unsigned short nUnitKind) 
{
    if( IS_BT(nUnitKind) ) return 0;
    if( IS_OT(nUnitKind) ) return 1;
    if( IS_DT(nUnitKind) ) return 2;
    if( IS_ST(nUnitKind) ) return 3;
    return 0;
}

float Random(float max, float min)
{
    if (min == max) return max;

    if (min > max) return RANDF(max, min);

    return RANDF(min, max);
}

int GetCreateUnitWeapon11(int nUnitKind)
{
// 2005-11-25 by ispark, EP2에서는 무기 지급이 공통이므로 표준무기로 입력(1형 : 테일)
//    if(IS_BT(nUnitKind)) return 7002710;
//    if(IS_OT(nUnitKind)) return 7002750;
//    if(IS_DT(nUnitKind)) return 7002770;
//    if(IS_ST(nUnitKind)) return 7002740;
//    return 0;
    // 2006-01-02 by ispark, 오른쪽 (0으로 끝남)
    if(IS_DT(nUnitKind)) return 7000050;
    return 7001950;
}
int GetCreateUnitWeapon12(int nUnitKind)
{
    // 2006-01-02 by ispark, 왼쪽 (1로 끝남)
    if(IS_DT(nUnitKind)) return 7000050;
    return 7001950;
}
int GetCreateUnitEngine(int nUnitKind)
{
    if(IS_BT(nUnitKind)) return 7001230;
    if(IS_OT(nUnitKind)) return 7002580;
    if(IS_DT(nUnitKind)) return 7002590;
    if(IS_ST(nUnitKind)) return 7002570;
    return 0;
}


void MakeCurrencySeparator(char *pDest, char *pSrc, int nSepInterval, char cSepChar)
{
    int        nSrcLen=    strlen(pSrc);
    int        nDestLen;
    char    *pFindDot=    strchr(pSrc, '.');
    int        nDotPos;

    // Error
    if (!nSepInterval) return;

    // Initial
    if (pFindDot == NULL)
    {
        nDotPos= nSrcLen;
        nDestLen= nSrcLen + nSrcLen/nSepInterval;
    }
    else
    {
        nDotPos= pFindDot - pSrc;
        nDestLen= nSrcLen + nDotPos/nSepInterval;
    }

    memset(pDest, NULL, nDestLen+1);

    // Copy to destination buffer
    int        nCurDestPos= nDestLen - 1;
    int        nCount= 0;

    for (int i=nSrcLen-1 ; i>=0 ; i--)
    {
        if (!(nCount%nSepInterval) && nCount)
        {
            pDest[nCurDestPos]= cSepChar;
            nCurDestPos--;
        }

        pDest[nCurDestPos]= pSrc[i];

        if (i < nDotPos)    nCount++;
        nCurDestPos--;
    }

    // Adjusting
    if (*pDest == NULL)
    {
        memcpy(pDest, pDest+1, nDestLen+1);
    }
}


UINT GetUnitNum(int nDegree, int nDefenseItemNum, int nUnitKind, BOOL bCharacter)
{
    if (bCharacter)
    {
        // 캐릭터 일때
        // 1부터는 여자, 100이상은 남자
        switch (nUnitKind) 
        {
        case 0:
        case 1:        
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
#ifdef EP1_CHARACTER
        case 7:
#endif
        case 100:
        case 101:
        case 102:    
        case 103:    
        case 104:    
        case 105:
        case 106:
#ifdef EP1_CHARACTER
        case 107:
#endif
            break;
        default:
        if(nUnitKind >= 100)
                nUnitKind = 100;
        else
            nUnitKind = 0;
        }
        
        return 20000000 + (nUnitKind * 100);
    }
    

    // 유닛(기체) 일때
    if (nDegree < 2) nDegree = 2;

    if (nDefenseItemNum == 0)
    {
        if (nUnitKind != 0)
        {
            int nTemp = GGetUnitKindHexToDeimal(nUnitKind);

            // return default gear
            return 10000000 + nDegree*1000000 + nTemp * 100;
        }
            
        return 0;
    }

    auto nTemp = nDefenseItemNum - 8000000;
            
    return 10000000 + nDegree * 1000000 + nTemp;
    
}

char* GetWaterTypeAtMap(USHORT nMapIndex )
{
    // 2005-06-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);
    switch(pMapInfo->WaterType) 
    {
    case 0:
        return NULL;
    case 1:
        return "water.tex";
    case 2:
        return "black.tex";
    case 3:
        return "black.tex";            // 2008-06-26 by dgwoo 깔루아 다시 변경. black_g => black
    }
/*    switch( nMapIndex )
    {
    case 1001:
    case 2001:
    case 4001:
    case 4002:
    case 6001:
        return NULL;
    case 3013:
        return "black.tex";
    case 3002:
    case 3003:
    case 3004:
    case 3005:
    case 3006:
    case 3007:
    case 3008:
    case 3009:
    case 3014:
    case 3034:
    case 3060:
    case 3063:
    case 3067:
    case 3999:// 테스트 맵(공맵)
    case 5001:
    case 8001:
    case 9001:
    case 9002:
    default:
        return "water.tex";
    }
*/
    return NULL;
}
/*
float GetWaterTick(USHORT nMapIndex )
{

    switch( nMapIndex )
    {
    case 1001:
    case 2001:
    case 4001:
    case 4002:
    case 6001:
        return 0.0f;
    case 3013:
        return 0.1f;
    case 3002:
    case 3003:
    case 3004:
    case 3005:
    case 3006:
    case 3007:
    case 3008:
    case 3009:
    case 3014:
    case 3034:
    case 3060:
    case 3063:
    case 3067:
    case 3999:    // 테스트맵(공맵)
    case 5001:
    case 8001:
    case 9001:
    case 9002:
    default:
        return 0.1f;
    }
    return 0.0f;
}
*/
BOOL IsTileMapRenderEnable(USHORT nMapIndex )
{
    // 2005-06-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    // 2010. 03. 05 by jskim 맵 로딩 중 윈도우 화면 전환시 지형 사라지는 버그 수정
    // 2007-04-12 by bhsohn 포커스 잃어 버렸을때의 추가버그 처리
    //return pMapInfo->TileRenderingFlag;
    //BOOL bMapRender = pMapInfo->TileRenderingFlag;
    //if(bMapRender && g_pD3dApp->IsDeviceLost())
    //{
    //    bMapRender = FALSE;        
    //}
    //return bMapRender;    
    return pMapInfo->TileRenderingFlag;
    //end 2010. 03. 05 by jskim 맵 로딩 중 윈도우 화면 전환시 지형 사라지는 버그 수정
}

// 2007-04-19 by bhsohn 충돌 처리 문제
BOOL IsTileMapTickEnable(USHORT nMapIndex )
{    
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);    
    return pMapInfo->TileRenderingFlag;
    
}
// 2007-08-02 by dgwoo 맵의 BGM을 얻어온다.
MapIndex_t GetMapIndexBGM(USHORT nMapIndex)
{
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);    
    if(pMapInfo == NULL)
        return NULL;
    return pMapInfo->Bgm;
}
BOOL IsSkyRenderEnable( USHORT nMapIndex )
{
    // 2005-06-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    return pMapInfo->SkyRenderingFlag;
}

BOOL IsSunRenderEnable(USHORT nMapIndex)
{
    // 2005-06-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    return pMapInfo->SunRenderingFlag;
}

BOOL IsFogEnableMap(USHORT nMapIndex)
{
    // 2005-06-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    return pMapInfo->FogRenderingFlag;
}

BOOL IsPkEnableMap(USHORT nMapIndex)
{
    // 2005-06-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    return pMapInfo->PKZoneFlag;
}

int GetQuestIndexForWarp(USHORT nMapIndex)
{
    // 2006-09-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    return pMapInfo->QuestIndexForWarp;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        벡터간 각을 구한다. 
//#define RadToDeg    57.29577951f         // 라디안을 도로 바꾼다.
//#define DegToRad    0.017453293f         // 도를 라디안으로 바꾼다.
/// \author        // 2007-04-13 by bhsohn A기어 포대문제
/// \date        2007-04-13 ~ 2007-04-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float GetRadianVectorBetweenVector(D3DXVECTOR3 i_vecDir, D3DXVECTOR3 i_vecUp)
{
    float o_fRadian = 0.0f;
    
    float dotAv = D3DXVec3Dot(&i_vecDir, &i_vecUp);    
    o_fRadian = ACOS(dotAv);

    return o_fRadian;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        업벡터에서의 포대가 움직인 벡터
/// \author        // 2007-04-13 by bhsohn A기어 포대문제
/// \date        2007-04-13 ~ 2007-04-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 GetRadianVector(D3DXVECTOR3 i_vecOld, D3DXVECTOR3 i_vecUp, FLOAT i_fRadianMove)
{
    D3DXVECTOR3 o_vecDir;    
    
    D3DXVECTOR3 vtempWeaponVel;
    D3DXVECTOR3 vVelF = i_vecOld;
    D3DXVECTOR3 vVelS;
    D3DXMATRIX matRot;
    
    D3DXVec3Normalize(&vVelF, &vVelF);
    D3DXVec3Cross(&vVelS, &i_vecUp, &vVelF);

    vtempWeaponVel = i_vecUp;        

    D3DXMatrixRotationAxis(&matRot , &vVelS , i_fRadianMove);
    D3DXVec3Normalize(&vtempWeaponVel, &vtempWeaponVel);
    D3DXVec3TransformCoord(&vtempWeaponVel , &vtempWeaponVel , &matRot); // view * rot로 새로운 dst vector를 구한다.
    o_vecDir = vtempWeaponVel;
    D3DXVec3Normalize(&o_vecDir, &o_vecDir);
    
    return o_vecDir;
}

// Set Fog Level
#ifdef _DEBUG_MAPSETTING
extern DWORD g_dwDebugFogColor;
extern float g_pDebugFogStartDistance;
extern float g_pDebugFogEndDistance;
extern D3DXVECTOR3 g_vDebugLightDirection;
void SetFogLevel(USHORT nMapIndex,BOOL bDay)
{
    // 2006-07-05 by ispark, 
    SetOptionFogDistance(nMapIndex,bDay, g_pDebugFogStartDistance, g_pDebugFogEndDistance);
}

DWORD GetFogColor(USHORT nMapIndex, BOOL bDay)
{
    // 2006-07-05 by ispark, 
    return g_dwDebugFogColor;
}

D3DXVECTOR3 GetMapDirection(USHORT nMapIndex,BOOL bDay)
{
    return g_vDebugLightDirection;
}
void SetOptionFogDistance(USHORT nMapIndex,BOOL bDay, float fFogStart, float fFogEnd)
{
    // 옵션 설정에 따른 시야 변화(현재 오브젝트와 지형이 같게 변화 된다)

    float fFogE = fFogEnd;
    float fFogS = fFogStart;
    g_pScene->SetFogLevel( GetFogColor(nMapIndex, bDay), fFogS, fFogE);
    g_pScene->m_bChangeWeather = TRUE;
}
#else

void SetFogLevel(USHORT nMapIndex,BOOL bDay)
{
// 2005-06-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    if(pMapInfo == NULL)
    {
        DBGOUT("맵(%d)이 없습니다.\n", nMapIndex);
        return;
    }
    DBGOUT("DayFogST = %d, DayFogED = %d\n", pMapInfo->DayFogStartDistance, pMapInfo->DayFogEndDistance);
    DBGOUT("NightFogST = %d, NightFogED = %d\n", pMapInfo->NightFogStartDistance, pMapInfo->NightFogEndDistance);
    if(bDay)
    {
        g_pScene->SetFogLevel(pMapInfo->DayFogColor, pMapInfo->DayFogStartDistance, pMapInfo->DayFogEndDistance);
    }
    else
    {
        g_pScene->SetFogLevel(pMapInfo->NightFogColor, pMapInfo->NightFogStartDistance, pMapInfo->NightFogEndDistance);
    }
}

D3DXVECTOR3 GetMapDirection(USHORT nMapIndex,BOOL bDay)
{
    // 2005-06-29 by ispark
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    if(bDay)
    {
        return A2DX(pMapInfo->DaySunDirection);
    }
    else
    {
        return A2DX(pMapInfo->NightSunDirection);
    }
}

void SetOptionFogDistance(USHORT nMapIndex,BOOL bDay, float fFogStart, float fFogEnd)
{
    // 옵션 설정에 따른 시야 변화(현재 오브젝트와 지형이 같게 변화 된다)

    // 2005-04-29 by jschoi
    float fFogE, fFogS;
    if(IS_TUTORIAL_MAP_INDEX(nMapIndex))
    {
        fFogE = fFogEnd;
        fFogS = fFogStart;
    }
    else
    {
        fFogE = fFogEnd*(g_pSOption->sTerrainRender + 5)/(MAX_OPTION_VALUE + 5);
        fFogS = fFogStart*(g_pSOption->sTerrainRender + 5)/(MAX_OPTION_VALUE + 5);
    }

    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    
    if(bDay)
    {
        g_pScene->SetFogLevel(pMapInfo->DayFogColor, fFogS, fFogE);
        DBGOUT("Scene DayFogST = %d, DayFogED = %d\n", fFogS, fFogE);
    }
    else
    {
        g_pScene->SetFogLevel(pMapInfo->NightFogColor, fFogS, fFogE);
        DBGOUT("Scene NightFogST = %d, NightFogED = %d\n", fFogS, fFogE);
    }

    g_pScene->m_bChangeWeather = TRUE;

/*
    g_pD3dDev->SetRenderState( D3DRS_FOGENABLE, TRUE );
    g_pD3dDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
    g_pD3dDev->SetRenderState( D3DRS_FOGSTART,  FtoDW(fFogStart) ); //
    g_pD3dDev->SetRenderState( D3DRS_FOGEND,    FtoDW(fFogEnd) );
    g_pD3dDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
    g_pD3dDev->SetRenderState( D3DRS_RANGEFOGENABLE, TRUE );
    g_pD3dDev->SetVertexShader( D3DFVF_FOGVERTEX );
    ////////////////////////////////////////////////////////////////////////
    g_pD3dDev->SetRenderState( D3DRS_FOGCOLOR,  GetFogColor(nMapIndex, bDay) );
*/
}

// 2005-03-04 by jschoi
void CheckMouseReverse(POINT* pt)
{
    if(g_pD3dApp->m_dwGameState != _GAME)
    {
        return;
    }

    if(    g_pInterface && 
        g_pInterface->m_pTarget && 
        g_pInterface->m_pTarget->m_nMouseType == MOUSE_TYPE_0)    
    {
        return;    // 인터페이스 마우스 타입(헤더파일)
    }

    // 좌우반전 옵션
    if(g_pSOption && g_pSOption->sMouseReversLeft == TRUE)
    {
        pt->x = g_pD3dApp->GetBackBufferDesc().Width - pt->x;
    }
    // 상하반전 옵션
    if(g_pSOption && g_pSOption->sMouseReversUp == TRUE)
    {
        pt->y = g_pD3dApp->GetBackBufferDesc().Height - pt->y;
    }
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            GetCharacterHeight(int nPilotNum)
/// \brief        캐릭터 높이 가져옴 (카메라 LookAt, ID, 말풍선 위치)
/// \author        ispark
/// \date        2005-07-29 ~ 2005-07-29
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float GetCharacterHeight(int nPilotNum)
{
    switch(nPilotNum) 
    {
    case 20000000:                        // 여자
        //return 10.4f * 1.5f;            // 2007-07-18 by dgwoo 캐릭터 변경 
        return 11.26f * 1.5f;
    case 20000100:
        return 11.3f * 1.5f;
    case 20000200:
        //return 10.1f * 1.5f;                
        return 10.81f * 1.5f;
    case 20000300:
        //return 11.2f * 1.5f;                
        return 11.0f * 1.5f;                
    case 20000400:
        //return 11.2f * 1.5f;                
        return 11.28f * 1.5f;
    case 20000500:
        return 11.3f * 1.5f;
    case 20000600:
        return 11.2f * 1.5f;
#ifdef EP1_CHARACTER
    case 20000700:
        return 11.28f * 1.5f;
#endif        
    case 20010000:                        // 남자
        return 11.18f * 1.5f;
    case 20010100:
        //return 10.6f * 1.5f;
        return 11.18f * 1.5f;
    case 20010200:                        
        //return 11.4f * 1.5f;            // 2007-07-30 by dgwoo
        return 11.3f * 1.5f;
    case 20010300:                        
        //return 11.5f * 1.5f;
        return 11.45f * 1.5f;
    case 20010400:                        
        //return 11.2f * 1.5f;
        return 11.35f * 1.5f;
    case 20010500:
        return 11.5f * 1.5f;
    case 20010600:
        return 11.2f * 1.5f;
#ifdef EP1_CHARACTER
    case 20010700:
        return 11.35f * 1.5f;
#endif
    }

    return 0.0f;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            float GetCharacterSteps(int nPilotNum)
/// \brief        캐릭터 보폭
/// \author        ispark
/// \date        2006-11-02 ~ 2006-11-02
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float GetCharacterSteps(int nPilotNum)
{
// 2007-07-20 by dgwoo 모든 캐릭터의 보폭을 같게함.
//    switch(nPilotNum) 
//    {
//    case 0:                        // 여자
//        return 50.0f;
//    case 1:
//        return 36.0f;
//    case 2:
//        return 44.0f;                
//    case 3:
//        return 48.0f;                
//    case 4:
//        return 38.0f;                
//    case 100:                        // 남자
//        return 40.0f;
//    case 101:
//        return 40.0f;
//    case 102:                        
//        return 43.0f;
//    case 103:                        
//        return 43.0f;
//    case 104:                        
//        return 50.0f;
//    }
    return 42.0f;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL GetCharactorName(char* o_szCharatorName,INT FaceNum)
/// \brief        캐릭터 얼굴의 번호를 넣으면 캐릭터 이름을 가져온다.
/// \author        dgwoo
/// \date        2007-11-22 ~ 2007-11-22
/// \warning    
///
/// \param        
/// \return        없을경우엔 FALSE를 리턴한다.
///////////////////////////////////////////////////////////////////////////////
BOOL GetCharactorName(char* o_szCharatorName,INT i_nPilotNum)
{
    char buf[SIZE_MAX_CHARACTER_NAME];
    switch(i_nPilotNum)
    {
    case 0:                        // 여자
        wsprintf(buf,STRMSG_C_071122_0103);
        break;
    case 1:
        wsprintf(buf,STRMSG_C_071122_0105);
        break;
    case 2:
        wsprintf(buf,STRMSG_C_071122_0104);
        break;
    case 3:
        wsprintf(buf,STRMSG_C_071122_0101);
        break;
    case 4:
        wsprintf(buf,STRMSG_C_071122_0102);
        break;
    case 5:
        wsprintf(buf,STRMSG_C_080527_0100);
        break;
    case 6:
        wsprintf(buf,STRMSG_C_111228_0001);
        break;
#ifdef EP1_CHARACTER
    case 7:
        wsprintf(buf,STRMSG_C_111228_0003);
        break;
#endif
    case 100:                        // 남자
        wsprintf(buf,STRMSG_C_071122_0110);
        break;
    case 101:
        wsprintf(buf,STRMSG_C_071122_0107);
        break;
    case 102:
        wsprintf(buf,STRMSG_C_071122_0106);
        break;
    case 103:
        wsprintf(buf,STRMSG_C_071122_0109);
        break;
    case 104:
        wsprintf(buf,STRMSG_C_071122_0108);
        break;
    case 105:
        wsprintf(buf,STRMSG_C_080527_0101);
        break;
    case 106:
        wsprintf(buf,STRMSG_C_111228_0002);
        break;
#ifdef EP1_CHARACTER
    case 107:
        wsprintf(buf,STRMSG_C_111228_0004);
        break;
#endif
    default:
        return FALSE;
    }
    strcpy(o_szCharatorName,buf);
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            GetBaseChange3D(POINT pt)    
/// \brief        2D좌표를 3D 방향 벡터와 위치로 변환
/// \author        ispark
/// \date        2005-09-07 ~ 2005-09-07
/// \warning    
///
/// \param        
/// \return        D3DXVECTOR3
///////////////////////////////////////////////////////////////////////////////
void GetBaseChange3D(POINT pt, D3DXVECTOR3 *vOutPos, D3DXVECTOR3 *vOutDir)
{
    D3DXVECTOR3 v, v3DDir;
    D3DXMATRIX matProj,matView;
    matProj = g_pD3dApp->m_pCamera->GetProjMatrix();

    // Compute the vector of the pick ray in screen space
    v.x =  (((2.0f * pt.x) / g_pD3dApp->m_d3dsdBackBuffer.Width) - 1) / matProj._11;
    v.y = -(((2.0f * pt.y) / g_pD3dApp->m_d3dsdBackBuffer.Height) - 1) / matProj._22;
    v.z =  1.0f;

    // Get the inverse view matrix
    D3DXMATRIX  matTemp;
    matView = g_pD3dApp->m_pCamera->GetViewMatrix();
    D3DXMatrixInverse(&matTemp, NULL, &matView);

    // Transform the screen space pick ray into 3D space
    v3DDir.x  = v.x*matTemp._11 + v.y*matTemp._21 + v.z*matTemp._31;
    v3DDir.y  = v.x*matTemp._12 + v.y*matTemp._22 + v.z*matTemp._32;
    v3DDir.z  = v.x*matTemp._13 + v.y*matTemp._23 + v.z*matTemp._33;
//    D3DXVec3Normalize(&v3DDir,&v3DDir);

    *vOutPos = D3DXVECTOR3(matTemp._41, matTemp._42, matTemp._43);
    *vOutDir = v3DDir;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            GetPositionInvenScreenMode()
/// \brief        
/// \author        ispark
/// \date        2005-11-30 ~ 2005-11-30
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int GetPositionInvenScreenMode()
{
    // 2007-12-21 by dgwoo 창모드 지원
    switch(g_pD3dApp->GetHeight())
    {

        case RESOLUTION_FULL_H_720:
            {
                return 255;
            }
            break;
        case RESOLUTION_FULL_H_768:
            {
                return 255;
            }
            break;
        case RESOLUTION_FULL_H_800:
            {
                return 255;
            }
            break;
        case RESOLUTION_FULL_H_900:
            {
                return 388;
            }
            break;
        case RESOLUTION_FULL_H_960:
            {
                return 388;
            }
            break;
        case RESOLUTION_FULL_H_1024:
            {
                return 388;
            }
            break;
        case RESOLUTION_FULL_H_1200:
            {
                return 388;
            }
            break;
        // 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)
        case RESOLUTION_FULL_H_1050:
            {
                return 388;
            }
            break;

        case RESOLUTION_FULL_H_1080:
            {
                return 388;
            }
            break;
        // end 2009. 10. 19 by ckPark 해상도 추가(1680x1050, 1920x1080, 1920x1200)

    }
//    DbgOut("Window Height(%d)",g_pD3dApp->GetBackBufferDesc().Height);
//    switch(g_pD3dApp->GetBackBufferDesc().Height)
//    {
//    case INVEN_RESOLUTION_1:
//        return 173;
//    case INVEN_RESOLUTION_2:
//        return 311;
//    case INVEN_RESOLUTION_3:
//        {
//            if(g_pD3dApp->GetBackBufferDesc().Height == 960)
//                return 423;
//            // 2007-04-24 by bhsohn 와이드 모니터 관련 처리
//            else if(g_pD3dApp->GetBackBufferDesc().Height == 800)
//            {
//                return 311;
//            }
//            // 2007-08-23 by bhsohn 1280X720와이드 모니터 관련 처리
//            else if(720 == g_pD3dApp->GetBackBufferDesc().Height)
//            {
//                return 255;
//            }
//            // end 2007-08-23 by bhsohn 1280X720와이드 모니터 관련 처리
//            else
//                return 468;
//        }
//    case INVEN_RESOLUTION_4:
//        {
//            // 2007-04-24 by bhsohn 와이드 모니터 관련 처리
//            if(g_pD3dApp->GetBackBufferDesc().Height == 900)
//            {
//                return 388;
//            }
//        return 563;
//        }
//        break;
//        
//    case INVEN_RESOLUTION_W:
//        return 144;
//    // 2007-07-14 by dgwoo
//    case INVEN_RESOLUTION_WINDOW_1024:
//        return 233;
//    case INVEN_RESOLUTION_WINDOW_1280:
//        return 233;
//    case INVEN_RESOLUTION_WINDOW_1600:
//        return 233;
//    }

    return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            EnableRevivalMap(USHORT nInfluenceType)
/// \brief        다른세력의 맵이라 죽었을 경우 바로 도시로 가야하는 맵인가?
/// \author        ispark
/// \date        2006-01-02 ~ 2006-01-02
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL EnableRevivalMap(USHORT nInfluenceType, USHORT nMapIndex)
{
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    // 미세력, 바이제니유
    if(nInfluenceType == INFLUENCE_TYPE_NORMAL ||
        nInfluenceType == INFLUENCE_TYPE_VCN)
    {        
        if(pMapInfo->MapInfluenceType == MAP_INFLUENCE_ANI_START)
        {
            return FALSE;
        }        
        // 2013-03-04 by bhsohn 모선/거점전 워프관련 버그 수정
        else if (MAP_INFLUENCE_ANI_BASE == pMapInfo->MapInfluenceType)
        {
            return FALSE;
        }        
        // END 2013-03-04 by bhsohn 모선/거점전 워프관련 버그 수정
    }
    // 알링턴
    else if (nInfluenceType == INFLUENCE_TYPE_ANI)
    {
        if(pMapInfo->MapInfluenceType == MAP_INFLUENCE_VCN_START)
        {
            return FALSE;
        }
        // 2013-03-04 by bhsohn 모선/거점전 워프관련 버그 수정
        else if (MAP_INFLUENCE_VCN_BASE == pMapInfo->MapInfluenceType)
        {
            return FALSE;
        }        
        // END 2013-03-04 by bhsohn 모선/거점전 워프관련 버그 수정
        
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL IsInfluenceSameMap(USHORT nInfluenceType, USHORT nMapIndex)
/// \brief        자신의 세력과 맵의 세력이 다를경우 false 같거나 세력이 없을경우 true
/// \author        dgwoo
/// \date        2006-01-02 ~ 2006-01-02
/// \warning    EnableRevivalMap() 함수 수정 요망.
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL IsNotInfluenceSameMap(USHORT nInfluenceType, USHORT nMapIndex)
{
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);
    // 바이제니유.
    if(nInfluenceType == INFLUENCE_TYPE_VCN)
    {
        if(IS_MAP_INFLUENCE_ANI(pMapInfo->MapInfluenceType))
        {
            return FALSE;
        }
    }
    // 알링턴.
    else if (nInfluenceType == INFLUENCE_TYPE_ANI)
    {
        if(IS_MAP_INFLUENCE_VCN(pMapInfo->MapInfluenceType))
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL IsInfluenceCharacter(USHORT nMyInfluenceType, USHORT nEnemyInfuenceType, BYTE byType)
/// \brief        캐릭터 간의 세력 비교
/// \author        ispark
/// \date        2006-01-07 ~ 2006-01-07
/// \warning    
///
/// \param        
/// \return        BOOL ( TRUE:같은 세력, FALSE:다른세력 )
///////////////////////////////////////////////////////////////////////////////
#ifdef FREEWAR_
BOOL IsInfluenceCharacter(USHORT nMyInfluenceType, USHORT nEnemyInfuenceType, BYTE byType, UINT WarZone)
#else
BOOL IsInfluenceCharacter(USHORT nMyInfluenceType, USHORT nEnemyInfuenceType, BYTE byType)
#endif
{
#ifdef FREEWAR_
    if (isFreeForAllMap(WarZone)) return FALSE;
#endif

    if (byType == 0) return // true if there is a common influence
        
        COMPARE_INFLUENCE(nMyInfluenceType, nEnemyInfuenceType);

    if (byType == 1) return // true if any of us is NORMAL or there is a common influence

        COMPARE_INFLUENCE(nMyInfluenceType, INFLUENCE_TYPE_NORMAL) ||
        COMPARE_INFLUENCE(nEnemyInfuenceType, INFLUENCE_TYPE_NORMAL) ||
        COMPARE_INFLUENCE(nMyInfluenceType, nEnemyInfuenceType);
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            GetInfluenceString(char* pOutStr, USHORT nMyInfluenceType)
/// \brief        캐릭터 세력 스트링 가져오기
/// \author        ispark
/// \date        2006-01-07 ~ 2006-01-07
/// \warning    
///
/// \param        
/// \return        void
///////////////////////////////////////////////////////////////////////////////
void GetInfluenceString(char* pOutStr, USHORT nMyInfluenceType)
{
    if(nMyInfluenceType == INFLUENCE_TYPE_NORMAL)
    {
        strcpy(pOutStr, STRCMD_CS_INFLUENCE_TYPE_NORMAL);
    }
    else if (nMyInfluenceType == INFLUENCE_TYPE_VCN)
    {
        strcpy(pOutStr, STRCMD_CS_INFLUENCE_TYPE_VCN);
    }
    else if (nMyInfluenceType == INFLUENCE_TYPE_ANI)
    {
        strcpy(pOutStr, STRCMD_CS_INFLUENCE_TYPE_ANI);
    }
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            GetMapItemVisibleDistance(USHORT nMapIndex)
/// \brief        드랍 아이템 보이는 허용 거리 가져오기
/// \author        ispark
/// \date        2006-01-13 ~ 2006-01-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int GetMapItemVisibleDistance(USHORT nMapIndex)
{
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(nMapIndex);

    return pMapInfo->UserVisibleDistance;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            GetInvenUseEngineEffectNum(int nUnitKind)
/// \brief        1단계에서 4단계까지 쓰이는 엔진 이펙트 번호
/// \author        ispark
/// \date        2006-01-23 ~ 2006-01-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int GetInvenUseEngineEffectNum(int nUnitKind)
{
    if(IS_BT(nUnitKind)) return 6351600;        // B기어
    if(IS_OT(nUnitKind)) return 6357000;        // M기어
    if(IS_DT(nUnitKind)) return 6353100;        // A기어
    if(IS_ST(nUnitKind)) return 6350500;        // I기어
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            GetInvenUseEngineObjectNum(int nUnitKind)
/// \brief        엔진 오브젝트 넘버
/// \author        ispark
/// \date        2006-01-24 ~ 2006-01-24
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int GetInvenUseEngineObjectNum(int nUnitKind)
{
    if(IS_BT(nUnitKind)) return 6332800;        // B기어
    if(IS_OT(nUnitKind)) return 6336000;        // M기어
    if(IS_DT(nUnitKind)) return 6333400;        // A기어
    if(IS_ST(nUnitKind)) return 6332400;        // I기어
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            int GetMusicVolume(int nRate)
/// \brief        볼륨 계산
/// \author        ispark
/// \date        2006-03-22 ~ 2006-03-22
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int GetMusicVolume(int nRate)
{
    int nNewVol;
    float fPercent = (float)nRate*0.02f;
    float fLog     = (float)log(fPercent);
    nNewVol = (int)(fLog*1000.f);

    return nNewVol;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            GetFontColor( char chr )
/// \brief        폰트 색깔 가져오기
/// \author        ispark
/// \date        2006-04-17 ~ 2006-04-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
DWORD GetFontColor(char chr)
{
    switch (chr)
    {
    case RED_FONT:
        return HFONT_ARGB(0, 255, 0, 0);
    case GREEN_FONT:
        return HFONT_ARGB(0, 0, 255, 0);
    case BLUE_FONT:
        return HFONT_ARGB(0, 0, 0, 255);
    case YELLOW_FONT:
        return HFONT_ARGB(0, 255, 255, 0);
    case CYAN_FONT:
        return HFONT_ARGB(0, 0, 255, 255);
    case MAGENTA_FONT:
        return HFONT_ARGB(0, 255, 0, 255);
    case WHITE_FONT:
        return HFONT_ARGB(0, 255, 255, 255);
    case ENCHANT_FONT:
        return HFONT_ARGB(0, 245, 185, 48);
    case GRAY_FONT:
        return HFONT_ARGB(0, 208, 208, 208);
    case DARKBLUE_FONT:
        return HFONT_ARGB(0, 178, 190, 255);

        //2015-03-02 by St0rmy added new colors
    case OLIVE_FONT:
        return HFONT_ARGB(0, 119, 187, 34);
    case PINK_FONT:
        return HFONT_ARGB(0, 255, 102, 238);
    case LIGHTBLUE_FONT:
        return HFONT_ARGB(0, 0, 170, 255);
    case DARKGREY_FONT:
        return HFONT_ARGB(0, 119, 119, 119);
    case BLACK_FONT:
        return HFONT_ARGB(0, 1, 1, 1);
    default:
        return HFONT_ARGB(0, 0, 0, 0);
    }

}

///////////////////////////////////////////////////////////////////////////////
/// \fn            GetFontColor( char* str )
/// \brief        New chat color system
/// \author        killburne
/// \date        2015-03-22
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
DWORD GetFontColor(char* str)
{
    unsigned int colorParts[3];

    util::zero(colorParts);
    
    if (strlen(str) == 6)
    {
        sscanf_s(str, "%2hhx%2hhx%2hhx", &colorParts[0], &colorParts[1], &colorParts[2]);
        return (HFONT_ARGB(0x00, colorParts[0], colorParts[1], colorParts[2]));
    }
    return (HFONT_ARGB(0x00, (BYTE)0, (BYTE)0, (BYTE)0));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            ReSetRemoveString(int nType, char *pChar)
/// \brief        특정 문자 빼기
/// \author        ispark
/// \date        2006-04-17 ~ 2006-04-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL ReSetRemoveString(int nType, char *pChar)
{
    int nStrLen = strlen(pChar);
    char pSubStringTemp[260] = {0,};
    int nRemoveCount = 0, nInCount = 0;
    // 특수 문자 빼기
    if(nType == SPECIAL_STRING)
    {
        for(int i = 0; i < nStrLen; i++)
        {
            if((*(pChar+i) >= 33 && *(pChar+i) <= 47) ||        // ! ~ /
                (*(pChar+i) >= 58 && *(pChar+i) <= 64) ||        // : ~ @
                (*(pChar+i) >= 91 && *(pChar+i) <= 96) ||        // [ ~ `
                (*(pChar+i) >= 123 && *(pChar+i) <= 126))        // { ~ ~
            {
                nRemoveCount++;
            }
            else
            {
                pSubStringTemp[nInCount] = *(pChar+i);
                nInCount++;
            }
        }
    }

    if(nRemoveCount > 0)
    {
        memset(pChar, 0x00, nStrLen);
        strncpy(pChar,pSubStringTemp, nStrLen);
    }

    return nRemoveCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            GetCollReflect(D3DXVECTOR3 *pvOut, D3DXVECTOR3 *pvVel, D3DXVECTOR3 *pvNor, BOOL    bRight = TRUE)
/// \brief        충돌 벡터 계산
/// \author        ispark
/// \date        2006-05-09 ~ 2006-05-09
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float GetCollReflect(D3DXVECTOR3 *pvOut, D3DXVECTOR3 *pvVel, D3DXVECTOR3 *pvNor, BOOL    bRight)
{
    D3DXVECTOR3 vIVel;
    if(bRight)
        vIVel = -1.0f * *pvVel;
    else
        vIVel = *pvVel;

    float fDot1 = D3DXVec3Dot(&vIVel,pvNor);
    *pvOut = (2 * fDot1) * *pvNor + *pvVel;
    D3DXVec3Normalize(pvOut,pvOut);

    return fDot1;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void StringCullingUserData(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont)
/// \brief        
/// \author        ispark
/// \date        2006-08-07 ~ 2006-08-07
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void StringCullingUserData(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont)
{
    FLOG( "CINFGameMain::StringCullingUserData(char *strFullString, int nHeight, vector<string> *vecStorage)" );
    
    if(nCullStringNum>strlen(pLineString))
    {
        vecStorage->push_back(pLineString);
        return;
    }

    // 글자 나누기
    int        nStrPointGap = 0;
    char    *pStr = CharNext(pLineString);
    char    *pPreStr = NULL;
    nStrPointGap = pStr - pLineString;    
    while(nStrPointGap < nCullStringNum)
    {
        pPreStr = pStr;
        pStr = CharNext(&pLineString[nStrPointGap]);
        nStrPointGap = pStr - pLineString;
        if(strlen(pLineString)<nCullStringNum)
        {
            break;
        }
    }
    char temp[2][2048];
    memset(temp, 0x00, 2 * 2048);
    memcpy(temp[0], pLineString, strlen(pLineString) - strlen(pPreStr));
    vecStorage->push_back(temp[0]);
    memcpy(temp[1], pPreStr, strlen(pPreStr));
    if(strlen(temp[1])>nCullStringNum)
    {
        StringCullingUserData(temp[1], nCullStringNum, vecStorage);
    }
    else
    {
        vecStorage->push_back(temp[1]);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont)
/// \brief        한글 수정(단어 그냥 자르기) ~ processes string colors too
/// \author        ispark
/// \date        2006-08-07 ~ 2006-08-07
///////////////////////////////////////////////////////////////////////////////
BOOL StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont, BOOL bChRt /*= FALSE*/, BOOL bShop/*= FALSE*/)
{
    FLOG( "CINFGameMain::StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont)" );

    char *pStr;
    char chColor = 0;
    char chColorSave = 0;
    int nOldPoint = 0;
    int nPoint = 0;
    int nCheckPoint = 0;
    int nLineBreakPoint;                // 한 라인에 브레이크 포이트가 있나?
    int nLine = 0;
    int nLineMax = 100;
    char strNPCTalk[64][1024] { };
    BOOL bShopNextLine;                // 事?때 아이템 이름이 다음라인에 걸쳐져 있습니까 ?
    
    while (true)
    {
        bShopNextLine = FALSE; // 2013-04-05 by ssjung ?일때 아이템 이름 글자 수 넘어 갈때 처리 보정
        char strBuf[1024] = {0,};
        pStr = CharNext(&pLineString[nOldPoint]);
        nLineBreakPoint = nPoint;
        nPoint = pStr - (pLineString + nCheckPoint);
        memcpy(strBuf, pLineString + nCheckPoint, nPoint);
        int nBufSize = pHanFont->GetStringSize(strBuf).cx;

// 2013-04-05 by ssjung ?일때 아이템 이름 글자 수 넘어 갈때 처리 보정 ~ FALSE by default
        if (bShop)
        {
            int nListChar = strlen(strBuf) - 1;
            if(' ' == strBuf[nListChar] || '+' == strBuf[nListChar])
            {
                char* strTemp;
                int n_pStrMinusstrTemp;
                int nCount = 0;
                for(int i = 0; i<1000; ++i)
                {
                    strTemp = CharNext(&pStr[nCount]);
                    if(' ' == pStr[nCount] || '+' == pStr[nCount] || ']' == pStr[nCount] || NULL == pStr[nCount])
                    {
                        n_pStrMinusstrTemp  = pHanFont->GetStringSize(pStr).cx -  pHanFont->GetStringSize(strTemp).cx;
                        if(nBufSize+n_pStrMinusstrTemp >= nCullStringNum)
                        {
//                            memcpy(strNPCTalk[nLine], pLineString + nCheckPoint, ++nLineBreakPoint);
//                            nCheckPoint += nLineBreakPoint;
//                            nPoint = 0;
//                            nLine ++;
                            bShopNextLine = TRUE;
                        }
                        break;
                    }
                    nCount++;
                }
            }
        }
// 2013-04-05 by ssjung ?일때 아이템 이름 글자 수 넘어 갈때 처리 보정

        if (nBufSize >= nCullStringNum || bShopNextLine)
        {
            if(nLine >= nLineMax)
            {
                return FALSE;
            }            

            if(chColorSave)
            {                
                char strNPCTalkTemp[1024] = {0, };
                memcpy(strNPCTalkTemp, pLineString + nCheckPoint, nLineBreakPoint);
                sprintf(strNPCTalk[nLine], "\\%c%s", chColorSave, strNPCTalkTemp);                

                // 2007-07-27 by bhsohn 글씨 컬링 버그 수정
                char chFindColor = GetLineFindColor(strNPCTalkTemp);                
                // 다음 라인에 종료 글씨가 있다.면 초기화 
                if(chFindColor == chColorSave)            
                {                
                    chColorSave = 0;
                    chColor = 0;                
                }
                // end 2007-07-27 by bhsohn 글씨 컬링 버그 수정
            }
            else
            {
                if(bShopNextLine) ++nLineBreakPoint;                                     // 2013-04-05 by ssjung ?일때 아이템 이름 글자 수 넘어 갈때 처리 보정
                memcpy(strNPCTalk[nLine], pLineString + nCheckPoint, nLineBreakPoint);
            }

            nCheckPoint += nLineBreakPoint;
            nPoint = 0;
            nLine ++;

            // 색깔 저장(다음 라인에 입력
            if (chColor)
            {
                chColorSave = chColor;
                chColor = 0;
            }
//            continue;
        }
        else if (pLineString[nOldPoint] == NULL)
        {
            if (nPoint == 0)
            {
                nLine--;
                break;
            }

            if (nLine >= nLineMax) return FALSE;
            
            
            if (chColorSave)
            {
                char strNPCTalkTemp[1024] = {0, };
                memcpy(strNPCTalkTemp, pLineString + nCheckPoint, nPoint);
                sprintf(strNPCTalk[nLine], "\\%c%s", chColorSave, strNPCTalkTemp);
                chColorSave = 0;
            }
            else memcpy(strNPCTalk[nLine], pLineString + nCheckPoint, nPoint);
            
            break;
        }

        //*--------------------------------------------------------------------------*//
        // 색깔 체크
        int nLastCount = strlen(strBuf) - 1;
        if(strBuf[nLastCount - 1] == '\\')
        {
            if (CheckFontColor(strBuf[nLastCount]))
            {
                if (chColor == strBuf[nLastCount])
                
                    chColor = 0;
                
                else chColor = strBuf[nLastCount];
                
            }            
            // 2007-07-27 by bhsohn 채팅 enter관련 처리
            // 2007-11-15 by bhsohn 퀘스트설정할만 \n되게 변경
            else if('n' == strBuf[nLastCount] && bChRt)
            {
                if(nLine >= nLineMax)
                {
                    return FALSE;
                }            
                
                // 역슬래쉬 제거
                nLineBreakPoint--;
                if(chColorSave)
                {                
                    char strNPCTalkTemp[1024] = {0, };                    
                    memcpy(strNPCTalkTemp, pLineString + nCheckPoint, nLineBreakPoint);
                    sprintf(strNPCTalk[nLine], "\\%c%s", chColorSave, strNPCTalkTemp);                
                    
                    // 2007-07-27 by bhsohn 글씨 컬링 버그 수정
                    char chFindColor = GetLineFindColor(strNPCTalkTemp);                
                    // 다음 라인에 종료 글씨가 있다.면 초기화 
                    if(chFindColor == chColorSave)            
                    {                
                        chColorSave = 0;
                        chColor = 0;                
                    }
                    // end 2007-07-27 by bhsohn 글씨 컬링 버그 수정
                }
                else
                {
                    memcpy(strNPCTalk[nLine], pLineString + nCheckPoint, nLineBreakPoint);
                }
                nLineBreakPoint++;
                
                nCheckPoint += nLineBreakPoint;
                nCheckPoint++; // n제거 

                nPoint = 0;
                nLine ++;
                
                // 색깔 저장(다음 라인에 입력
                if(chColor)
                {
                    chColorSave = chColor;
                    chColor = 0;
                }
                
            }
            
        }
        //*--------------------------------------------------------------------------*//
        //
        nOldPoint = nCheckPoint + nPoint;
    }

    for (auto j = 0; j < nLine + 1; ++j)
        
        vecStorage->push_back(strNPCTalk[j]);
    

    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL GetLineFindColor(char chColorSave , char* pTxtTmp)
/// \brief        
/// \author        // 2007-07-27 by bhsohn 글씨 컬링 버그 수정
/// \date        2007-07-27 ~ 2007-07-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
char GetLineFindColor(char* pTxtTmp)
{
    int nCnt = 0;
    BOOL bFindSlash = FALSE;
    for(nCnt = 0;nCnt < strlen(pTxtTmp);nCnt++)
    {
        if(pTxtTmp[nCnt] == '\\')
        {
            bFindSlash = TRUE;
        }
        else if (TRUE  == bFindSlash)            
        {
            return pTxtTmp[nCnt];        
        }
        
    }
    return 0;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn            char GetFindColorOnTxt(char* pTxtTmp)
/// \brief        색의 위치를 리턴한다.
/// \author        // 2008-07-24 by bhsohn 컬링 관련 버그 수정
/// \date        2008-07-24 ~ 2008-07-24
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
char GetFindColorOnTxt(char* pTxtTmp)
{    
    int nFindColorPos = -1;
    char chColor = 0;
    int nCnt = 0;
    BOOL bFindSlash = FALSE;
    for(nCnt = 0;nCnt < strlen(pTxtTmp);nCnt++)
    {
        if(pTxtTmp[nCnt] == '\\')
        {
            bFindSlash = TRUE;
        }
        else if (TRUE  == bFindSlash )            
        {
            bFindSlash = FALSE;

            if(chColor == pTxtTmp[nCnt])
            {
                chColor = 0;
            }
            else
            {
                chColor = pTxtTmp[nCnt];
            }                        

            if(-1 == nFindColorPos)
            {
                nFindColorPos = nCnt;
            }
        }        
    }    

    return chColor;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL StringCullingUserData2( char* strTalk, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont)
/// \brief        단어 공백에서 자르기
/// \author        ispark
/// \date        2006-08-08 ~ 2006-08-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL StringCullingUserData2( char* strTalk, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont)
{
//    int i = 0;
//    int nPoint = 0;
//    int nCheckPoint = 0;
//    int nBreakPoint = 0;
//    int nLineBreakPoint = 0;                // 한 라인에 브레이크 포이트가 있나?
//    int nLine = 0;
//    int nLineMax = 100;
//    char strNPCTalk[64][512];
//    BOOL bSetChar = FALSE;
//    ZERO_MEMORY(strNPCTalk);
//    
//    if(strlen(strTalk) == 0)
//    {
//        DBGOUT("NPC Talk is NULL.(QuestIndex)\n");
//        return;
//    }
//    while(TRUE)
//    {
////        if(strTalk[i] == ' ' || (strTalk[i] == '.' && strTalk[i-3] != '[') || strTalk[i] == '!' || strTalk[i] == NULL || bSetChar == TRUE)
//        if(strTalk[i] == ' ' || strTalk[i] == '!' || strTalk[i] == NULL || bSetChar == TRUE)
//        {
//            bSetChar = FALSE;
//            char strBuf[512] = {0,};
//            memcpy(strBuf, strTalk + nCheckPoint, nPoint);
//            int nBufSize = pHanFont->GetStringSize(strBuf).cx;
////            if(nPoint >= nCullStringNum)
//            if(nBufSize >= nCullStringNum)
//            {
//                if(nLine >= nLineMax)
//                {
//                    return;
//                }
//                memcpy(strNPCTalk[nLine], strTalk + nCheckPoint, nBreakPoint+1);
//                nPoint -= nBreakPoint;
//                nCheckPoint += nBreakPoint+1;
//                nBreakPoint = nPoint-1;
//                nLineBreakPoint = 0;
//                nLine ++;
//                i++;
//                continue;
//            }
//            if(strTalk[i] == NULL)
//            {
//                if(nLine == 0 && i == 0)
//                {
//                    return;
//                }
//                if(nLine >= nLineMax)
//                {
//                    return;
//                }
//                memcpy(strNPCTalk[nLine], strTalk + nCheckPoint, nPoint);
//                break;
//            }
//            nBreakPoint = nPoint;
//            nLineBreakPoint = nBreakPoint;
//        }
//        else if(nLineBreakPoint == 0)
//        {
//            char strBuf[512] = {0,};
//            memcpy(strBuf, strTalk + nCheckPoint, nPoint);
//            int nBufSize = pHanFont->GetStringSize(strBuf).cx;
//            if(nBufSize >= nCullStringNum)
//            {
//                bSetChar = TRUE;
//                nBreakPoint = nPoint - 1;
//            }
//        }
//        i++;
//        nPoint++;
//    }
//
//    for(int j=0; j<nLine+1; j++)
//    {
//        vecStorage->push_back(strNPCTalk[j]);
//    }
    char *pStr;
    char chColor = 0;
    char chColorSave = 0;
    int nOldPoint = 0;
    int nPoint = 0;
    int nBreakPoint = 0;
    int nCheckPoint = 0;
    int nLineBreakPoint = 0;                // 한 라인에 브레이크 포이트가 있나?
    int nLine = 0;
    int nLineMax = 100;
    char strNPCTalk[64][1024];
    BOOL bSetChar = FALSE;
    ZERO_MEMORY(strNPCTalk);
    
    while(TRUE)
    {
        bSetChar = FALSE;
        char strBuf[1024] = {0,};
        pStr = CharNext(&strTalk[nOldPoint]);
        nPoint = pStr - (strTalk + nCheckPoint);
        if(strTalk[nOldPoint+1] == ' ' || strTalk[nOldPoint+1] == '!' || strTalk[nOldPoint+1] == NULL || bSetChar == TRUE)
        {
            memcpy(strBuf, strTalk + nCheckPoint, nPoint);
            int nBufSize = pHanFont->GetStringSize(strBuf).cx;
            if(nBufSize >= nCullStringNum)
            {
                if(nLine >= nLineMax || nBreakPoint == 0)
                {
                    return FALSE;
                }
                if(chColorSave)
                {
                    char strNPCTalkTemp[1024] = {0, };
                    memcpy(strNPCTalkTemp, strTalk + nCheckPoint, nBreakPoint);
                    sprintf(strNPCTalk[nLine], "\\%c%s", chColorSave, strNPCTalkTemp);
                    // 2007-07-27 by bhsohn 글씨 컬링 버그 수정
                    char chFindColor = GetLineFindColor(strNPCTalkTemp);                
                    // 다음 라인에 종료 글씨가 있다.면 초기화 
                    if(chColorSave == chFindColor)            
                    {                
                        chColorSave = 0;
                        chColor = 0;                
                    }
                    // end 2007-07-27 by bhsohn 글씨 컬링 버그 수정
                }
                else
                {
                memcpy(strNPCTalk[nLine], strTalk + nCheckPoint, nBreakPoint);
                }
                nCheckPoint += nBreakPoint;
                nPoint = 0;
                nBreakPoint = 0;
                nLine ++;

                // 색깔 저장(다음 라인에 입력
                if(chColor)
                {
                    chColorSave = chColor;
                    chColor = 0;
                }
            }
            else if (strTalk[nOldPoint+1] == NULL)
            {
                if(nPoint == 0)
                {
                    nLine--;
                    break;
                }

                if(nLine >= nLineMax)
                {
                    return FALSE;
                }

                if(chColorSave)
                {
                    char strNPCTalkTemp[1024] = {0, };
                    memcpy(strNPCTalkTemp, strTalk + nCheckPoint, nPoint);
                    sprintf(strNPCTalk[nLine], "\\%c%s", chColorSave, strNPCTalkTemp);
                    chColorSave = 0;
                }
                else
                {
                memcpy(strNPCTalk[nLine], strTalk + nCheckPoint, nPoint);
                }
                break;
            }

            nBreakPoint = nPoint;
        }

        //*--------------------------------------------------------------------------*//
        // 색깔 체크
        if(strTalk[nOldPoint] == '\\')
        {
            if(CheckFontColor(strTalk[nOldPoint+1]))
            {
                if(chColor == strTalk[nOldPoint+1])
                {
                    chColor = 0;
                }
                else
                {
                    chColor = strTalk[nOldPoint+1];
                }
            }
        }
        //*--------------------------------------------------------------------------*//
        
        nOldPoint = nCheckPoint + nPoint;
    }

    for(int j=0; j<nLine+1; j++)
    {
        vecStorage->push_back(strNPCTalk[j]);
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            StringCullingUserData3(char *pLineString, int nState, int nCullStringNum, vector<string> *vecStorage)
/// \brief        특수한 상태에 따라 문자 앞에서 자르고 싶을 때
/// \author        ispark
/// \date        2006-02-03 ~ 2006-02-03
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void StringCullingUserData3(char *pLineString, int nState, int nCullStringNum, vector<string> *vecStorage)
{
    FLOG( "CINFGameMain::StringCullingUserData(char *strFullString, int nHeight, vector<string> *vecStorage)" );
    
    // 글자 나누기
    int        nStrPointGap = 0;
    char    *pStr = CharNext(pLineString);
    char    *pPreStr = NULL;
    nStrPointGap = pStr - pLineString;    
    while(nStrPointGap < nCullStringNum)
    {
        pPreStr = pStr;
        pStr = CharNext(&pLineString[nStrPointGap]);
        nStrPointGap = pStr - pLineString;

        if(nState == _SHOP)
        {
            if(pLineString[nStrPointGap - 2] == ']')
            {
                break;
            }
        }
    }
    char temp[2][2048];
    memset(temp, 0x00, 2 * 2048);
    memcpy(temp[0], pLineString, strlen(pLineString) - strlen(pPreStr));
    vecStorage->push_back(temp[0]);
    memcpy(temp[1], pPreStr, strlen(pPreStr));
    if(strlen(temp[1])>nCullStringNum)
    {
        StringCullingUserData(temp[1], nCullStringNum, vecStorage);
    }
    else
    {
        vecStorage->push_back(temp[1]);
    }
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL StringCullingUserData_ToBlank( char* strTalk, int nCullPixel, vector<string> *vecStorage, CD3DHanFont* pHanFont)
/// \brief        단어 공백으로 글씨 자르기 
/// \author        // 2008-01-23 by bhsohn 공백으로 글씨 짜르는 함수 생성
/// \date        2008-01-23 ~ 2008-01-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL StringCullingUserData_ToBlank( char* strTalk, int nCullPixel, vector<string> *vecDstString, CD3DHanFont* pFont, BOOL bChRt /*= FALSE*/)
{
    int nCheckPoint=  0;
    char* pCheckPoint = nullptr;
    char chCullingBuff[2048], chCullingTmpBuff[2048];    
    char chColorSave = 0;
    bool bBreak = false;    
    bool bFindBlank = false;    
    int nBlankPos = 0;    
    
    int nCullingPos=0;
    int nLastStartPos = 0;    

    int nCullingLen =0;
    int nLastCullingLen = 0;    

    int nOldCheckPoint = 0;

    ZERO_MEMORY(chCullingBuff);
    ZERO_MEMORY(chCullingTmpBuff);
    
    while (true)
    {
        char *pNextStr = CharNext(&strTalk[nCheckPoint]);

        if (pNextStr == nullptr) break;

        nOldCheckPoint = nCheckPoint;

        int    nNextLen = pNextStr - (strTalk + nCheckPoint);

        bFindBlank = false;
        
        nCheckPoint += nNextLen;

        nCullingLen = pNextStr - (strTalk + nLastStartPos);
        strncpy(chCullingBuff, &strTalk[nLastStartPos], nCullingLen);                                                                

        if (pNextStr[0] == ' ') bFindBlank = true;

        else if (pNextStr[0] == '\0' || pNextStr == nullptr)
        {
            bFindBlank = true;            
            bBreak = true;            
        }        

        int nBufSize = pFont->GetStringSize(chCullingBuff).cx;
        if (nBufSize >= nCullPixel)            
        {            
            // 길이 추가
            int nStringSize = strlen(chCullingBuff);
            memset(chCullingBuff, 0x00, nStringSize + 1);
            if(nLastCullingLen)
            {
                // 다시 메모리를 다시 복사해온다.
                strncpy(chCullingBuff, &strTalk[nLastStartPos], nLastCullingLen);                
                nCheckPoint = nCullingPos;
                // 컬링 성공
                nLastStartPos = nCullingPos;                        
            }
            else
            {    
                nLastCullingLen = pNextStr - (strTalk + nLastStartPos);

                // 공백이 없다.
                strncpy(chCullingBuff, &strTalk[nLastStartPos], nLastCullingLen);                
                
                nLastStartPos = nCheckPoint;                
                nCheckPoint = nCheckPoint;
            }        

            chColorSave = StringCullingAddVector(chColorSave, chCullingBuff, vecDstString);
            ZERO_MEMORY(chCullingBuff);                
            
            if (bBreak)
            {
                // 공백을 못찾고 최대 픽셀수를 넘었다.
                nCullingLen = pNextStr - (strTalk + nLastStartPos);
                if(nCullingLen)
                {
                    strncpy(chCullingBuff, &strTalk[nLastStartPos], nCullingLen);            
                    chColorSave = StringCullingAddVector(chColorSave, chCullingBuff, vecDstString);
                    ZERO_MEMORY(chCullingBuff);    
                }
            }
            bFindBlank = false;            
            
            nLastCullingLen = 0;
            nCullingPos = 0;
        }        
        else if (bBreak)
        {
            // 공백을 못찾고 최대 픽셀수를 넘었다.
            nCullingLen = pNextStr - (strTalk + nLastStartPos);
            if(nCullingLen)
            {
                strncpy(chCullingBuff, &strTalk[nLastStartPos], nCullingLen);            
                chColorSave = StringCullingAddVector(chColorSave, chCullingBuff, vecDstString);            
            }
        }
        else if (bFindBlank)
        {    
            nLastCullingLen = nCullingLen;
            nCullingPos = nCheckPoint;
        }

        if (bBreak) break;
    }
    return TRUE;
}

char StringCullingAddVector(char chColorSave, char* pString, vector<string> *vecDstString)
{
    char chCullingTmpBuff[2048];
    // 2008-07-24 by bhsohn 컬링 관련 버그 수정
    char chFindColor = GetFindColorOnTxt(pString);                

    ZERO_MEMORY(chCullingTmpBuff);
    // 2008-07-24 by bhsohn 컬링 관련 버그 수정
    //if(0 == chFindColor && chColorSave )
    if(chColorSave )
    {
        // 칼러가 없으면 강제로 칼라 넣어줌
        strncpy(chCullingTmpBuff, pString, 2048);
        sprintf(pString, "\\%c%s", chColorSave, chCullingTmpBuff);
        chFindColor = GetFindColorOnTxt(pString);                
        chColorSave = chFindColor;
    }
    else if (0 != chFindColor)
    {
        // 라인컬러 저장
        chColorSave = chFindColor;
    }

    // 컬링을 원하는 픽셀보다 큰지 체크한다.
    vecDstString->push_back(pString);

    return chColorSave;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            ATUM_DATE_TIME GetServerDateTime()
/// \brief        서버시간에서 경과된 시간
/// \author        ispark
/// \date        2006-10-11 ~ 2006-10-11
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
ATUM_DATE_TIME GetServerDateTime()
{
    ATUM_DATE_TIME tempTime = g_ServerDateTime;
    
    int nPassSecondTime = (timeGetTime() - g_GameStartTime) / 1000;
    tempTime.AddDateTime(0, 0, 0, 0, 0, nPassSecondTime);

    return tempTime;
}
// 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
ATUM_DATE_TIME GetArenaServerDateTime()
{
    ATUM_DATE_TIME tempTime = g_ArenaServerDateTime;
    
    int nPassSecondTime = (timeGetTime() - g_ArenaStartTime) / 1000;
    tempTime.AddDateTime(0, 0, 0, 0, 0, nPassSecondTime);
    
    return tempTime;
}
// end 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정

///////////////////////////////////////////////////////////////////////////////
/// \fn            ATUM_DATE_TIME GetAtumDateTimeFormSecond(int nSecond)
/// \brief        초를 시간으로 변경
/// \author        ispark
/// \date        2006-10-11 ~ 2006-10-11
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////

// 2009. 06. 10 by ckPark 255일 넘는 아이템 사용 시간 제대로 나오지 않는 오류 수정
// ATUM_DATE_TIME GetAtumDateTimeFormSecond(int nSecond)
// {
//     ATUM_DATE_TIME tempTime;
//     BYTE byDay; 
//     BYTE byHour; 
//     BYTE byMinute; 
//     BYTE bySecond;
// 
//     // 2007-09-07 by bhsohn 시간제 마크 아이템 추가
// //    bySecond = (nSecond % 60);
// //    nSecond /= 60;
// //    byMinute = (nSecond % 60);
// //    nSecond /= 60;
// //    byHour = (nSecond % 60);
// //    nSecond /= 24;
// //    byDay = (nSecond % 24);
//     if(nSecond < 0)
//     {
//         tempTime.SetDateTime(0, 0, 0, 0, 0, 0);
//         return tempTime;
// 
//     }
//     // 일수를 구한다.
//     {
//         byDay = nSecond/86400;
//         nSecond -= (byDay*86400);
//     }
//     // 시
//     {
//         byHour = (nSecond / 3600);
//         nSecond -= (byHour*3600);
//     }
// 
//     // 분
//     {
//         byMinute = (nSecond / 60);
//         nSecond -= (byMinute*60);
//     }
// 
//     // 초
//     {        
//         if(nSecond < 0)
//         {
//             nSecond = 0;
//         }
//         bySecond = nSecond;
//     }
//     // end 2007-09-07 by bhsohn 시간제 마크 아이템 추가
// 
//     tempTime.SetDateTime(0, 0, byDay, byHour, byMinute, bySecond);
// 
//     return tempTime;
// }



void    GetAtumDateTimeFormSecond( const int nSecond, int* pDay, int* pHour, int* pMin, int* Sec )
{
    int nSec = nSecond;
    
    if(nSec < 0)
        *pDay = *pHour = *pMin = *Sec = 0;
    {
        // 일수를 구한다.
        *pDay    =    nSec / 86400;
        nSec    -=    *pDay * 86400;

        // 시
        *pHour    =    nSec / 3600;
        nSec    -=    *pHour * 3600;

        // 분
        *pMin    =    nSec / 60;
        nSec    -=    *pMin * 60;

        // 초
        if(nSec < 0)
            nSec = 0;
        *Sec = nSec;
    }
}
// end 2009. 06. 10 by ckPark 255일 넘는 아이템 사용 시간 제대로 나오지 않는 오류 수정


// 2010. 02. 11 by ckPark 발동류 장착아이템
#define ONEDAY_SEC        86400    // 1일
#define ONEHOUR_SEC        3600    // 1시간
#define ONEMIN_SEC        60        // 1분

void    GetString_TimeLeft( DWORD nSec, char* pBuff )
{
    int        nNum;

    if( nSec > ONEDAY_SEC )
    {
        // 일단위 표시
        nNum = nSec / ONEDAY_SEC;
        sprintf( pBuff, STRMSG_C_061011_0004, nNum );    // "%d일"
    }
    else if ( nSec > ONEHOUR_SEC )
    {
        // 시간단위 표시
        nNum = nSec / ONEHOUR_SEC;
        sprintf( pBuff, STRMSG_C_060425_0000, nNum );    // "%d시"
    }
    else if ( nSec >= ONEMIN_SEC )
    {
        // 분단위 표시
        nNum = nSec / ONEMIN_SEC;
        sprintf( pBuff, STRMSG_C_SKILL_0009, nNum );    // "%d분"
    }
    else
    {
        nNum = nSec;
        sprintf( pBuff, STRMSG_C_SKILL_0010, nNum );    // "%2d초"
    }
}

BOOL    GetString_CoolTime( CItemInfo* pItem, char* pBuff )
{
    if( pItem->GetCoolElapsedTime() > 0
        && pItem->GetCoolElapsedTime() < pItem->ItemInfo->ReAttacktime )
    {
        GetString_TimeLeft( (int)((pItem->ItemInfo->ReAttacktime - pItem->GetCoolElapsedTime()) / 1000), pBuff );
        return TRUE;
    }
    else
        return FALSE;
}
// end 2010. 02. 11 by ckPark 발동류 장착아이템


///////////////////////////////////////////////////////////////////////////////
/// \fn            char * GetStringDateTimeFormSecond(int nSecond);
/// \brief        초를 스트링으로 변경
/// \author        ispark
/// \date        2006-10-11 ~ 2006-10-11
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
char * GetStringDateTimeFormSecond(char * o_strDateTime, int nSecond)
{
    
    // 2009. 06. 10 by ckPark 255일 넘는 아이템 사용 시간 제대로 나오지 않는 오류 수정
//     ATUM_DATE_TIME tempTime = GetAtumDateTimeFormSecond(nSecond);
// 
//     if(tempTime.Day)
//     {
//         char strTemp[15] = {0,};
//         wsprintf(strTemp, STRMSG_C_061011_0004, tempTime.Day);
//         wsprintf(o_strDateTime, "%s ", strTemp);
//     }
//     if(tempTime.Hour)
//     {
//         char strTemp[15] = {0,};
//         wsprintf(strTemp, STRMSG_C_060425_0000, tempTime.Hour);
//         wsprintf(strTemp, "%s ", strTemp);
//         strcat(o_strDateTime, strTemp);
//     }
//     if(tempTime.Minute)
//     {
//         char strTemp[15] = {0,};
//         wsprintf(strTemp, STRMSG_C_SKILL_0009, tempTime.Minute);
//         wsprintf(strTemp, "%s ", strTemp);
//         strcat(o_strDateTime, strTemp);
//     }
//     if(tempTime.Second)
//     {
//         char strTemp[15] = {0,};
//         wsprintf(strTemp, STRMSG_C_SKILL_0010, tempTime.Second);
//         wsprintf(strTemp, "%s ", strTemp);
//         strcat(o_strDateTime, strTemp);
//     }


    int nDay, nHour, nMin, nSec;
    nDay = nHour = nMin = nSec = 0;
    GetAtumDateTimeFormSecond( nSecond, &nDay, &nHour, &nMin, &nSec );
    
    if(nDay)
    {
        char strTemp[15] = {0,};
        wsprintf(strTemp, STRMSG_C_061011_0004, nDay);
        wsprintf(o_strDateTime, "%s ", strTemp);
    }
    if(nHour)
    {
        char strTemp[15] = {0,};
        wsprintf(strTemp, STRMSG_C_060425_0000, nHour);
        wsprintf(strTemp, "%s ", strTemp);
        strcat(o_strDateTime, strTemp);
    }
    if(nMin)
    {
        char strTemp[15] = {0,};
        wsprintf(strTemp, STRMSG_C_SKILL_0009, nMin);
        wsprintf(strTemp, "%s ", strTemp);
        strcat(o_strDateTime, strTemp);
    }
    if(nSec)
    {
        char strTemp[15] = {0,};
        wsprintf(strTemp, STRMSG_C_SKILL_0010, nSec);
        wsprintf(strTemp, "%s ", strTemp);
        strcat(o_strDateTime, strTemp);
    }
    // end 2009. 06. 10 by ckPark 255일 넘는 아이템 사용 시간 제대로 나오지 않는 오류 수정

    return o_strDateTime;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            DWORD CheckFontColor( char chr )
/// \brief        색깔 체크
/// \author        ispark
/// \date        2006-10-23 ~ 2006-10-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CheckFontColor( char chr )
{
    switch (chr)
    {
    case RED_FONT:
    case GREEN_FONT:
    case BLUE_FONT:
    case YELLOW_FONT:
    case CYAN_FONT:
    case MAGENTA_FONT:
    case WHITE_FONT:
    case ENCHANT_FONT:
    case GRAY_FONT:
    case DARKBLUE_FONT:

        //2015-03-20 by St0rmy, added new colors
    case OLIVE_FONT:
    case PINK_FONT:
    case LIGHTBLUE_FONT:
    case DARKGREY_FONT:
    case BLACK_FONT:
        return true;

    default: return false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            DWORD CheckFontColor( char* str )
/// \brief        checks if it's a valid color
/// \author        killburne
/// \date        2015-03-22
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CheckFontColor(char* str)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F')))
        {
            return FALSE;
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void GetScale(float& fXScale, float& fYScale)
/// \brief        이미지의 크기를 입력하면 스케일의 값으로 변경.
/// \author        dgwoo
/// \date        2007-07-03 ~ 2007-07-03
/// \warning    입력값중에 0보다 작거나 0이면 리턴.    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void GetScale(float& fXScale, float& fYScale)
{
    if((fXScale*fYScale) <= 0)
        return;
    fXScale = (float)(g_pD3dApp->GetBackBufferDesc().Width)/fXScale;
    fYScale = (float)(g_pD3dApp->GetBackBufferDesc().Height)/fYScale;
}
DWORD GetCurrentColor(float fTime, DWORD dwType, float fTotal, float fDelay)
{    // fTime 전체 시간, dwType 알파타입, fTotal 페이드 인아웃 타임, fDelay 지연 시간
    DWORD dwColor = 0x00FFFFFF;
    DWORD dwAlpha;

    if(dwType == ALPHA_WAVE)
    {// 깜빡깜빡
        float fAlpha;
        fAlpha =  (fTime - (((int)(fTime/fTotal))*fTotal))/fTotal;
        if(fAlpha >= 0.5f)
        {
            dwAlpha = 255 * (2.0f - fAlpha*2.0f);
        }
        else
        {
            dwAlpha = 255 * (2.0f - (1.0f - fAlpha)*2.0f);
        }
    }
    else if (dwType == ALPHA_DELAY_WAVE)
    {
        float fAlpha;
        fTime = fTime - (((int)(fTime/fTotal))*fTotal);
        float fGap = (fTotal - fDelay)/2.0f;    // fGap + fDelay + fGap = fTotal
        
        if(fTime < fGap)
        {
            // 2006-07-18 by dgwoo 점점 진해진다.
            fAlpha =  fTime / fGap;
            dwAlpha = 255 * fAlpha;
        }
        else if (fTime >= fGap && fTime <= fGap + fDelay)
        {
            // 2006-07-18 by dgwoo 선명한 글씨 유지
            dwAlpha = 255;
        }
        else
        {
            // 2006-07-18 by dgwoo 점점 흐려진다.
            fAlpha =  (fTime - fDelay -fGap) / fGap;
            dwAlpha = 255 * (1.0f - fAlpha);            
        }

//        fAlpha =  (fTime - (((int)(fTime/fTotal))*fTotal))/fTotal;
//        if(fAlpha >= 0.5f)
//        {
//            dwAlpha = 255 * (2.0f - fAlpha*2.0f);
//        }
//        else
//        {
//            dwAlpha = 255 * (2.0f - (1.0f - fAlpha)*2.0f);
//        }        
    }
    else if (dwType == ALPHA_FADE_IN)
    {
        if(fTime < 0.0f)
        {
            dwAlpha = 0;
        }
        else if (fTime < fTotal)
        {
            dwAlpha = 255 * (fTime/fTotal);
        }
        else if (fTime > fTotal)
        {
            dwAlpha = 255;
        }
    }
    else if (dwType == ALPHA_FADE_OUT)
    {
        if(fTime < 0.0f)
        {
            dwAlpha = 255;
        }
        else if (fTime < fTotal)
        {
            dwAlpha = 255 * (1.0f - (fTime/fTotal));
        }
        else if (fTime > fTotal)
        {
            dwAlpha = 0;
        }
    }

    dwAlpha = dwAlpha << 24;
    dwColor |= dwAlpha;
    return dwColor;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL IsMousePosClientArea()
/// \brief        현재 마우스 영역이
/// \author        dgwoo
/// \date        2008-01-14 ~ 2008-01-14
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL IsMousePosClientArea()
{
    RECT rtClient;
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(g_pD3dApp->GetHwnd(),&pt);
    GetClientRect(g_pD3dApp->GetHwnd(),&rtClient);
    if(PtInRect(&rtClient,pt))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        소수점 둘째 자리 까지 정렬한다.
/// \author        // 2008-03-19 by bhsohn FLOAT형 재정렬 하여 사용
/// \date        2008-03-17 ~ 2008-03-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float FloatRangeDecimal(float fValue, int nDecimalPoint)
{
    float fDst = fValue;
    char chFloatBuff[256];
    memset(chFloatBuff, 0x00, 256);

    sprintf(chFloatBuff, "%f", fDst);
    char* pFind = strstr(chFloatBuff, ".");
    if(NULL == pFind)
    {
        return fDst;
    }
    int nPos = pFind - chFloatBuff;
    chFloatBuff[nPos+nDecimalPoint+1] = NULL;

    return atof(chFloatBuff);

}


///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        소수점 둘째 자리 까지 정렬한다.
/// \author        // 2008-10-20 by bhsohn 피어스율 올림으로 변경
/// \date        2008-10-20 ~ 2008-10-20
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float FloatSecRangeSharp(float fValue)
{    
    char chFloatBuff[256];
    memset(chFloatBuff, 0x00, 256);

    sprintf(chFloatBuff, "%.2f", fValue);

    return atof(chFloatBuff);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            INT ChangeChatButtonNumber(INT i_nMenuIndex)
/// \brief        메뉴 버튼과 메인창에 채팅 버튼 인덱스로 변경.
/// \author        dgwoo
/// \date        2008-05-19 ~ 2008-05-19
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
INT ChangeChatButtonNumber(INT i_nMenuIndex)
{
    INT nNum = 0;
    switch(i_nMenuIndex)
    {
    case CHAT_TAB_ALLUSER:
        nNum = 0;
        break;
    case CHAT_TAB_NORMAL:
        nNum = 1;
        break;
    case CHAT_TAB_WAR:
        nNum = 2;
        break;
    case CHAT_TAB_TRADE:
        nNum = 3;
        break;
    case CHAT_TAB_MAP:
        nNum = 4;
        break;
    case CHAT_TAB_GUILD:
        nNum = 5;
        break;
    case CHAT_TAB_PARTY:
        nNum = 6;
        break;
    case CHAT_TAB_ARENA:
        nNum = 7;
        break;
    case CHAT_TAB_CHATROOM:
        nNum = 8;
        break;
    }
    return nNum;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            FLOAT GetAmorGearHeight(INT i_nAmmor)
/// \brief        아머높이를 리턴.(아머키)
/// \author        dgwoo
/// \date        2008-07-07 ~ 2008-07-07
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
FLOAT GetAmorGearHeight(INT i_nAmmor, BOOL i_bSelect /* = FALSE*/)
{
    // 2009. 09. 03 by ckPark 로봇기어 버젼에 따라 높이적용 안되는 문제

    ITEM* pItem = g_pDatabase->GetServerItemInfo( i_nAmmor );
    if( pItem )
    {
        if( pItem->SourceIndex != i_nAmmor )
            i_nAmmor = pItem->SourceIndex;
    }

    // end 2009. 09. 03 bt ckPark 로봇기어 버젼에 따라 높이적용 안되는 문제
    
    
    
    switch(i_nAmmor)
    {
    case 8410100:
    case 8412100:
        if (i_bSelect) return 12.0f;
        return 35.0f;

    case 8050400:
        return 15.00f;

    // 2010. 06. 01 by jskim 월드컵 아머 높이 적용
    // 2009. 07. 16 by ckPark 수박아머 높이 조절
    case 8290600:
    case 8291600:
    case 8293600:
    case 8559400:
    case 8565500:
    case 8569700:
        return 10.85f;

    case 8292600:
    case 8567600:
        return 13.0f;

    // end 2009. 07. 16 by ckPark 수박아머 높이 조절
    //end 2010. 06. 01 by jskim 월드컵 아머 높이 적용    

    // 2010. 05. 03 by ckPark 로봇기어 높이 수정
    // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
//     case 8293800:
//         if( i_bSelect )
//             return 13.0f;
//         else
//             return 22.0f;
// 
//     case 8292800:
//         if( i_bSelect )
//             return 16.0f;
//         else
//             return 19.0f;
// 
//     case 8291800:
//         if( i_bSelect )
//             return 14.0f;
//         else
//             return 21.0f;
// 
//     case 8290800:
//         if( i_bSelect )
//             return 8.5f;
//         else
//             return 17.0f;
    // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    case 8293800:
    case 8990700:
        if (i_bSelect) return 13.0f;
        return 22.0f;
        
    case 8292800:
    case 8991700:
        if (i_bSelect) return 16.0f;
        return 19.0f;
        
    case 8291800:
    case 8992700:
        if (i_bSelect) return 14.0f;
        return 21.0f;
        
    case 8290800:
    case 8993700:
        if (i_bSelect) return 8.5f;
        return 17.0f;
    //end 2010. 05. 03 by ckPark 로봇기어 높이 수정

    default:
        return 7.087f;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        2바이트도 한글자로 인식한다.
/// \author        // 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date        2009-03-18 ~ 2009-03-18
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int GetStringBuffPos(const char* pTxt, int i_nCurSelPos)
{
    // 위치 0
    if (i_nCurSelPos == 0) return 0;

    int nCheckPoint = 0;
    int nStringPos = 0;
    BOOL bChColor = FALSE;


    for (auto nCnt = 0; nCnt < strlen(pTxt); nCnt++)
    {
        auto pNextStr = CharNext(&pTxt[nCheckPoint]);
        
        if (!pNextStr || pTxt[nCheckPoint] == '\0') break;
        
        if (!bChColor && pTxt[nCheckPoint] == '\\')
        {
            bChColor = TRUE;
#ifdef _EXTENDED_CHATCOLOR
            int nPos = 0;
            if (pTxt[nCheckPoint + 1] == '#')
            {
                bool hasHash = false;

                for (int i = 2; pTxt[nCheckPoint + i] != NULL && i <= 8; i++)
                {
                    if (i > 2 && pTxt[nCheckPoint + i] == '#')
                    {
                        hasHash = true;
                        break;
                    }
                    else if (nPos < 6)
                    {
                        nPos++;
                    }
                    else
                    {
                        break;
                    }
                }
                if (nPos == 6 && hasHash)
                {
                    nCnt += nPos;
                    bChColor = FALSE;
                    for (int m = 0; m < nPos + 2; m++)
                    {
                        nCheckPoint = pNextStr - pTxt;
                        pNextStr = CharNext(&pTxt[nCheckPoint]);
                    }

                    nPos++;
                }
                else
                {
                    nPos = 0;
                }
            }
#endif
        }
        else if (bChColor && (0 != GetFontColor( pTxt[nCheckPoint] )))
        {
            // 색이 글자는 체크안한다.
            bChColor = FALSE;            
        }
        else
        {
            // 색이 글자는 체크안한다.
            bChColor = FALSE;
            
            // \n 인식 못하게 함.
            nStringPos++;
        }

        nCheckPoint = pNextStr - pTxt;        
        if(i_nCurSelPos == nStringPos)
        {
            return nCheckPoint;

        }
    }
    
    return -1;// 못찾았다.
    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        2바이트도 한글자로 인식한다.
/// \author        // 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date        2009-03-18 ~ 2009-03-18
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int GetStringBuffLen(const char* pTxt)
{
    int nCheckPoint = 0;
    int nStringPos = 0;
    int nCnt = 0;
    BOOL bChColor = FALSE;
    int nMaxSize = strlen(pTxt);
    for(nCnt = 0;nCnt < strlen(pTxt); nCnt++)
    {
        char    *pNextStr = CharNext(&pTxt[nCheckPoint]);
        if(NULL == pNextStr)
        {
            break;
        }
        else if (NULL == pTxt[nCheckPoint])
        {
            break;
        }
        else if (!bChColor && ('\\' == pTxt[nCheckPoint]))
        {
            bChColor = TRUE;
#ifdef _EXTENDED_CHATCOLOR
            int nPos = 0;
            if (pTxt[nCheckPoint + 1] == '#')
            {
                bool hasHash = false;

                for (int i = 2; pTxt[nCheckPoint + i] != NULL && i <= 8; i++)
                {
                    if (i > 2 && pTxt[nCheckPoint + i] == '#')
                    {
                        hasHash = true;
                        break;
                    }
                    else if (nPos < 6)
                    {
                        nPos++;
                    }
                    else
                    {
                        break;
                    }
                }
                if (nPos == 6 && hasHash)
                {
                    nCnt += nPos;
                    bChColor = FALSE;
                    for (int m = 0; m < nPos + 2; m++)
                    {
                        nCheckPoint = pNextStr - pTxt;
                        pNextStr = CharNext(&pTxt[nCheckPoint]);
                    }
                    
                    nPos++;
                }
                else
                {
                    nPos = 0;
                }
            }
#endif
        }
        else if (bChColor && (0 != GetFontColor( pTxt[nCheckPoint] )))
        {
            // 색이 글자는 체크안한다.
            bChColor = FALSE;            
        }
        else
        {
            // 색이 글자는 체크안한다.
            bChColor = FALSE;
            
            // \n 인식 못하게 함.
            nStringPos++;
        }
        
        nCheckPoint = pNextStr - pTxt;                
    }
    return nStringPos;    
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        2바이트도 한글자로 인식한다.
/// \author        // 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date        2009-03-18 ~ 2009-03-18
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int NormalizeRect(RECT *prt)
{
    
    int t;    
    int SwapResult=0;    
    if (prt->left > prt->right) 
    {
        
        t=prt->left;        
        prt->left=prt->right;        
        prt->right=t;        
        SwapResult|=1;        
    }
    
    if (prt->top > prt->bottom) 
    {        
        t=prt->top;        
        prt->top=prt->bottom;        
        prt->bottom=t;        
        SwapResult|=2;        
    }    
    return SwapResult;    
}

// 2015-12-21 Future, int32 optimized method of determinating the number of digits in a number
int NumDigits(int x)
{
    if (x == 0) return 1;
    if (x < 0) return NumDigits(-x) + 1;

    if (x >= 10000) {
        if (x >= 10000000) {
            if (x >= 100000000) {
                if (x >= 1000000000)
                    return 10;
                return 9;
            }
            return 8;
        }
        if (x >= 100000) {
            if (x >= 1000000)
                return 7;
            return 6;
        }
        return 5;
    }
    if (x >= 100) {
        if (x >= 1000)
            return 4;
        return 3;
    }
    if (x >= 10)
        return 2;
    return 1;
}


#endif
