//Copyright[2002] MasangSoft
// ShuttleChild.cpp: implementation of the CShuttleChild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShuttleChild.h"
#include "AtumApplication.h"
#include "ChatMoveData.h"
#include "ItemData.h"
//#include "TraceData.h"
// remodeling, 2004-04-28 by dhkwon
#include "StoreData.h"    // Itemtable, enchant, store
#include "QuestData.h"    // Quest, CharacterQuest 관리

#include "EnemyData.h"
#include "INFQuest.h"
#include "MonsterData.h"
#include "AtumSJ.h"
#include "Background.h"
#include "SceneData.h"
#include "KeyBoardInput.h"
#include "JoyStickInput.h"                    // 2008-11-13 by bhsohn 조이스틱 작업
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "Camera.h"
#include "AtumDatabase.h"
#include "INFGameMainUnitInfoBar.h"
//#include "ObjectRender.h"
#include "AtumSound.h"
#include "Chat.h"
#include "RangeTime.h"
#include "INFUnitState.h"
#include "INFGameMainUnitInfoBar.h"
#include "Weapon.h"
#include "WeaponShieldData.h"
#include "ObjectChild.h"
#include "INFWeaponInfo.h"
//#include "INFNotice.h"
//#include "TraceRender.h"
// 2007-11-22 by bhsohn 아레나 통합서버
//#include "IMSocket.h"
#include "IMSocketManager.h"

#include "Interface.h"
#include "INFWindow.h"
//#include "MonsterRender.h"
//#include "INFShop.h"
#include "INFTarget.h"
#include "INFTrade.h"
#include "WeaponMineData.h"
#include "ItemInfo.h"
#include "Cinema.h"
#include "INFCityBase.h"
#include "INFCityWarp.h"
// 2008-03-19 by bhsohn Ep3옵션창
//#include "INFSystem.h"
#include "INFOptionSystem.h"

#include "WeaponItemInfo.h"
#include "Skill.h"
#include "UnitRender.h"
#include "ClientParty.h"
#include "ObjRender.h"
#include "MonRender.h"
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "TickTimeProcess.h"
#include "INFSkill.h"
#include "SkillEffect.h"
#include "dxutil.h"
#include "PkNormalTimer.h"
#include "AtumDefine.h"
#include "INFMp3Player.h"
#include "TutorialSystem.h"
#include "CharacterChild.h"
#include "INFMissionInfo.h"
#include "GlobalImage.h"

#include "INFSelect.h"

#include "ObjectAniData.h"
#include "EffectRender.h"
#include "INFGameArena.h"
#include "INFOpMain.h"
#include "INFMissionMain.h" // 2008-12-09 by dgwoo 미션마스터.

// 2009-02-25 by bhsohn 조이스틱 이용, 메뉴창 띠우면 A기어는 마우스로 조절
#include "INFCharacterInfoExtend.h"
#include "INFInvenExtend.h"
// end 2009-02-25 by bhsohn 조이스틱 이용, 메뉴창 띠우면 A기어는 마우스로 조절

#include "INFInfinity.h"        // 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

#include "PetManager.h"    // 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
#include "TimeMgr.h" // 2011-08-12 by hsson 편대 대열에 추가/탈퇴 시 딜레이를 삽입 (편대 동기화 버그를 줄임)
#define SELECT_MENU_ITEM_SHIFT(x)        (WEAPON_BODYCON_LOADING << (x*10))//10 = 기어가 증가하면서 bodycondition이 증가하는 값

// 2015-11-2, PanKJ FreeWar
#include "FreeWar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define TUTORIAL_MISSION_1            1
#define TUTORIAL_MISSION_2            2
#define TUTORIAL_MISSION_3            3
#define TUTORIAL_MISSION_4            4
#define TUTORIAL_MISSION_11            11

// 2008-07-07 by dgwoo GetAmorGearHeight() 변경.
//#define SHUTTLE_HEIGHT                7.087f

#define OVERHEAT_REPAIR_TIME        10.0f    // 10초
#define UNIT_STOP_DELAY                2.0f    // 2초            // 2005-09-26 by ispark

#define AIR_SIDERATE_MAX            0.7f
#define AIR_UPDOWNRATE_MAX            0.5f

//////////////////////////////////////////////////////////////////////
// 착륙시 이동관련 정의문 
// ydkim
// 2004.6.7
//////////////////////////////////////////////////////////////////////
#define KEYBOARD_INPUT_TIMER        0.5f
#define MOVEFRAME_TIMER                0.5f
#define SIGEMODE_MOUSE_FITPOINT     50.0f
#define A_GEAR_SIGEMODE_MOVETIMING  0.1f
#define ANIMATION_UNIT_TIMING        0.01f

#define SHUTTLE_CONNER_MOVE_SPEED    30

#define ANI_UPDOWN_FLAG                1
#define ANI_LEFT_FLAG                2
#define ANI_RIGHT_FLAG                3

#define ANI_REFLEXION_LEFT_FLAG        4
#define ANI_REFLEXION_RIGHT_FLAG    5

#define REFLEXION_TILE_MIN_SIZE        1

///////////////////////////////////////////////////////////////////////
///        스킬 관련 정의문
///        jschoi
///        2006.06.26
///////////////////////////////////////////////////////////////////////

#define TARGET_ON        1
#define TARGET_OFF        2

// 착륙장 패턴
#define AIR_PORT_LANDING_PATTERN_NONE    0
#define AIR_PORT_LANDING_PATTERN_START    1
#define AIR_PORT_LANDING_PATTERN_END    2
#define AIR_PORT_TAKEOFF_PATTERN_START    3
#define AIR_PORT_TAKEOFF_PATTERN_END    4

#define AIR_PORT_MIN_SPEED                150.0f

// 편대 대형시 이동 범위
#define FORMATION_UPDOWN_RANGE            0.1f        
#define FORMATION_SIDE_RANGE            0.1f        
#define FORMATION_MOVE_RATE                20

// 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
#define    UNIT_CHG_STATE_TIME                0.5f

// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#define    UNIT_SEARCHEYE_CHG_STATE_TIME    0.01f

// 2007-04-13 by bhsohn A기어 포대문제
#define        AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN            0.266f            // 최대 15도*DegToRad
#define        AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN            2.0943f         // 최소 120도*DegToRad
#define        AGEAR_SIEGEMODE_MIN_CAP                        0.001f             

// 2007-05-15 by bhsohn A기어 보완
#define        AGEAR_LANDING_SPEED                            45.0f            // 착륙 속도
#define        AGEAR_RASING_SPEED                            60.0f            // 이륙 속도
#define        AGEAR_RASING_TIME                            0.4f            // 이륙 시간

// 2007-12-05 by bhsohn 벽에 뚫고 들어가는 현상 처리
// 최대 각도 10도
#define        MAX_LADING_DO                            10.0f            

// 2008-11-13 by bhsohn 조이스틱 작업
#define        JOYSTIC_STEP_MAX_VALUE                    (1000.0f)

// 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음
#define        ATTACK_POSSIBLE_DISTANCE                (20.0f)
// end 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음

// 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화
#define        SEND_MOVE_VEL_CHECK_TIME                (1000)    // 1초에 한번씩 보낸다.
// end 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화

// 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정
#define        BGEAR_CHAGINGSHOT_SHOT_CAP_TIME            (1.2f)    
// end 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정

// 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
#define        FALLING_TIME    5000
// end 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가

// 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
#define        MAX_PETUSETITEM_CAP        300
// END 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정

#ifdef _DEBUG
void TempDataRenderLib() {}
#endif // _DEBUG_endif

CShuttleChild::CShuttleChild()
{
    FLOG( "CShuttleChild()" );
    m_pGameData = NULL;
    g_pShuttleChild = this;
    m_pCharacterInfo = NULL;
    m_pRender = g_pD3dApp->m_pUnitRender;
    m_dwPartType = _SHUTTLE;
    m_vBasisUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);            // Up Vector
    D3DXVECTOR3 vPos = D3DXVECTOR3(150.0f,30.0f,150.0f);
    SetPos(vPos);
    D3DXVec3Normalize(&m_vVel,&m_vVel);
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);          
    //m_dwState = _NORMAL;
    m_fSideRate = 0.0f;                                    // 좌우 회전 속도 조절
    m_fSideCheckRate = 0.0f;
    m_fCurrentTime = 0.0f;
    memset(&m_myShuttleInfo,0x00,sizeof(CHARACTER));
    
    // 2007-05-15 by bhsohn 기어 스탯 관련 처리
    memset(&m_myShuttleFullStatInfo,0x00,sizeof(GEAR_STAT));    // 전체 스탯정보     

    m_fAxisRate = 0.0f;
    m_nUDState = 0;

    m_bGetOtherInfo = FALSE;
    m_fGetOtherInfoCheckTime = 0.0f;
    m_nGetOtherIndex = 0;

    m_bColl = FALSE;
    m_vCollPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_fCheckCollTime = 0.3f;
    m_pMoveChat = NULL;
    m_pMoveChat = new CAtumNode;
    m_pIDChat = NULL;
    m_pIDChat = new CAtumNode;

    m_nBooster = BOOSTER_TYPE_STOP;
    m_nHyperBooster = HYPER_BOOSTER_NORMAL;
    m_vOldPos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);// edited by dhkwon 2003.8.20

    m_bIsUnderWater = FALSE;
    m_bIsWater = FALSE;

    m_fLRVelRate = 0.0f;

    m_bCollMap = FALSE;
    m_bOnObject = TRUE;

    m_bIsAir = FALSE;
    m_vNormal = m_vUp;

    m_fHeightRate = 0.0f;
    m_bShadowIsRender = TRUE;

    m_bSkillMoveIsUse = FALSE;
    m_bSkillMoveType = 0;
    m_nCurrentPatternNumber = 0;

    m_bCollShuttleCheck = 0;
    m_bCollMonsterCheck = 0;
    m_bCollObjectCheck = 0;
    m_bCollAttackCheck = 0;

    m_fObjectSize = 15.0f;

    m_bTurnCamera = FALSE;
    m_pTarget = NULL;
    m_pOrderTarget = NULL;

    ChangeEngine(0,0);

    m_bCheckBlend = FALSE;

    m_bEffectGround = 0;
    m_fExpPer = 0.0f;

    m_bMoveGroundType = 0;

    m_fDTEventCheckTime = 0.0f;

    m_bMoveInputType = 0;
    m_fShuttleSpeed = 0;
    m_fShuttleSpeedMax = 0;
    m_fShuttleSpeedMin = 0;
    m_fShuttleSpeedBoosterOn = 0;
    m_bySpeedPenalty = 0;                                // 2005-08-16 by ispark
    m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
    m_fChangeBodyForBoosterCheckTime = 0.2f;
    m_nShuttleBoosterState = 0;
    m_nShuttleStopState = 0;
    m_fNumpad0keyTime = 2.0f;                            // 2006-05-30 by ispark, 1.0f -> 2.0f
    m_fCancelSkillCheckTime = 0.0f;

    m_fMouseLRMoveRate = 0.0f;
    m_fMouseUDMoveRate = 0.0f;

    m_fDefenseTotal = 0.0f;    
    m_fDefenseForItem = 0.0f;    
    m_fDefenseForSkill = 0.0f;    
    m_fDefenseForShuttle = 0.0f;
    m_fTransportTotal = 0.0f;    
    m_fTransportCurrent = 0.0f;

    int i;
    for(i=0;i<MAX_CHAT_PTOP_CONT;i++)
        memset(m_strChatPToP[i],0x00,SIZE_MAX_CHARACTER_NAME);
    m_bCurPToPPos = 0;
    m_bOldPToPPos = 200;

    m_bRButtonState = FALSE;
    m_bLButtonState = FALSE;
    m_bMButtonState = FALSE;
    m_bReChargeBURN = TRUE;
    m_bUseBooster = TRUE;

    m_bUpButtonState = FALSE;
    m_bDownButtonState = FALSE;
    m_bLeftButtonState = FALSE;
    m_bRightButtonState = FALSE;

            
    m_fBURN = 1.0f;
    m_fCurrentBURN = 0.0f;

    m_ptOldPoint.x = 0;
    m_ptOldPoint.y = 0;
    m_bFirstStart = FALSE;

    m_bAutoLockon = FALSE;
    m_bAutoHeightControl = FALSE;

//    m_bEventReady = FALSE;

    m_fWeaponMaxDistance = 0.0f;

    m_nMonsterCount = 0;
    m_bAttackMode = _AIR;
//    m_fEventCheckTime = 2.0f;


    m_fNextHP = 0.0f;
    m_fNextEP = 0.0f;
    m_fNextSP = 0.0f;
    m_fNextDP = 0.0f;
    m_fNextHPRate = 0.0f;
    m_fNextEPRate = 0.0f;
    m_fNextSPRate = 0.0f;
    m_fNextDPRate = 0.0f;

    m_bIsSetFlighting = FALSE;
    m_fCollSendDamageCheckTime = 0.0f;

    m_vMouseDir = D3DXVECTOR3(0,0,0);
    m_fMouseRate = 0.5f;
    m_pt.x = 0;
    m_pt.y = 0;
    m_ptDir.x = 0;
    m_ptDir.y = 0;

    m_fOldUpRate = 0.0f;
    m_vCollCenter = D3DXVECTOR3(0,0,0);
    m_bBeginnerQuestDegree = 0;

    m_bMouseLock = FALSE;

    m_bOrderMove = FALSE;
    m_bOrderMoveType = FALSE;
    m_vTargetPos = D3DXVECTOR3(0,0,0);
    m_vTargetVel = D3DXVECTOR3(1,0,0);
    m_fLandingCheckTime = 0.0f;

    m_fRasingCheckTime = 0.0f;

    m_pStoreData = NULL;
    m_fKeyboardInputTimer = 0;

    m_fFrameLerp          = 0;                            // 이동 애니메이션 러프
    m_fAniFlag            = 0;                            // 이동 애니메이션 플레그
    m_bAniKeyBlock        = FALSE;                        // 애니메이션중 키블럭값
    m_bAgearPortflag      = FALSE;                        // A 기어 착륙장 플레그
    m_bReflexionKeyBlock  = FALSE;                        // 벽에부디칠때 키블럭 값 
    m_bLandingOldCamtypeFPS         = FALSE;

    m_fAnimationMoveTimer         = ANIMATION_UNIT_TIMING;
    m_fSinMove = 0;
    m_dwLastAttackTick            = 0;        
    m_bIsCameraPattern = FALSE;                // 2004.06.28 jschoi 카메라패턴 사용중인가?
    m_pCinemaUnitPattern = NULL;            // 2004.06.17 jschoi 유닛(스킬,연출) 패턴
    m_pCinemaCamera = NULL;                    // 2005.06.17 jschoi 스킬 카메라
    m_nEventType = EVENT_WARP_IN;
    m_pCurrentObjEvent = NULL;
    m_pCurrentObjEventOld = NULL;

    m_pPrimaryWeapon = NULL;
    m_pSecondaryWeapon = NULL;

    m_pSkill = NULL;
    m_pClientParty = NULL;

    m_pGuildWarInfo = NULL;
    m_fWarpOutDistance = 0;

    // 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수값 변경.
    //memset(    &m_wearItemParamFactor, 0x00, sizeof(WEAR_ITEM_PARAM_FACTOR)*WEAR_ITEM_NUMBER );
    memset(    &m_wearItemParamFactor, 0x00, sizeof(WEAR_ITEM_PARAM_FACTOR)*WEAR_ITEM_NUMBER );
    memset(    &m_wearPreRareItemParamFactor, 0x00, sizeof(WEAR_ITEM_PARAM_FACTOR)*MAX_EQUIP_POS );
    memset(    &m_wearSufRareItemParamFactor, 0x00, sizeof(WEAR_ITEM_PARAM_FACTOR)*MAX_EQUIP_POS );
    // End 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수값 변경.
    
    m_nOldMapIndex = 0;
//#ifdef _DEBUG
//    m_nDebugWarpMove = 0;
//    m_nDebugSendPacketMove = 0;
//    m_fDebugWarpCheckTime = 0;
//    m_fDebugSendCheckTime = 0;
//#endif
//    m_fMissileFireTime = 0;
    
    // 2005-02-15 by jschoi  -  PK Delay Timer
    m_pPkNormalTimer = new CPkNormalTimer;
    
    m_bCtrlKey = FALSE;

    m_bWarpGateZone = FALSE;
    
    m_byCityWarTeamType = CITYWAR_TEAM_TYPE_NORMAL;
    m_fCurrentAngle = 0;
    m_fCurrentSideRate = 0;

    m_bMouseMoveLock = FALSE;                // 기본적으로 막음    // 2005-07-05 by ispark

    // 2005-07-05 by ispark
    // 롤링 시스템 초기화
    InitRolling();

    m_nEventTypeAirPort = AIR_PORT_LANDING_PATTERN_NONE;
    m_nEventIndex = 0;
    m_bKeyBlock = FALSE;
    m_bWarpLink = FALSE;
    m_bUnitStop = FALSE;
    m_bPenaltyUnitStop = FALSE;            // 2005-08-09 by ispark

    m_pRadarItemInfo = NULL;            // 2005-08-16 by ispark
    m_bLandingField = FALSE;
    m_bSkipPattern = FALSE;
    m_bAttack = TRUE;                    //공격이 가능한 모드로 세팅.
    InitUnitStop();
    m_fRotationX = SHUTTLE_ROTATION_DEFAULT_X;
    m_fRotationY = 0.0f;
    m_fRotationZ = SHUTTLE_ROTATION_DEFAULT_Z;
    m_fRotationA = 0.0f;    
    
    // 2008-08-22 by bhsohn EP3 인벤토리 처리
    //D3DXMatrixIdentity(&m_pMatInven);
    //m_bInvenRender = FALSE;

    m_pTexturesBackInven = NULL;
    m_bRenderInven = FALSE;

    m_bSkillAirMove = FALSE;
    m_fCheckWearItemTime = 0.0f;
    m_nCheckDamage = 0;

    m_fAdvanceTime = 0.0f;
    m_fCheckDead = 0.0f;

    m_bObserve = FALSE;
    m_bOperation = FALSE;
    // 2008-08-22 by bhsohn EP3 인벤토리 처리
//    for(i = 0; i < 4; i++)
//    {
//        D3DXMatrixIdentity(&m_pMatInvenWeaponSetPosition[i]);
//        D3DXMatrixIdentity(&m_pMatInvenWeaponOrgPosition[i]);
//    }
    // end 2008-08-22 by bhsohn EP3 인벤토리 처리

    m_nArmorColorIndex = 0;
    m_nInvenArmorColorIndex = 0;

    // 2007-11-09 by dgwoo 무기 메쉬를 배열에서 벡터로 변경.
    m_vectInvenWeaponMesh.clear();
    for(i = 0 ; i < OBSERVE_MAX_QUICK ; i++)
    {
        m_ObserveQuick[i] = 0;
    }

    m_byFormationFlightType = 0;

    // 2007-04-17 by bhsohn 가까운 거리에서의 시즈모드시 데이지 안들어가는 현상 처리
    m_bAGearFollowEnemy = FALSE;

    m_fUnitX = m_fUnitY = m_fUnitZ = m_fScaling = 0;
    m_bNowMousePosWindowArea = TRUE;
    m_bOldMousePosWindowArea = TRUE;    
    m_fAmorHeight = 7.087f;

    // 2008-11-13 by bhsohn 조이스틱 작업
    m_bSetCursorPos = FALSE;
    m_nJoyStickRolling = 0;    

    // 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화
    m_dwCheckTimeSendMoveVel = 0;
    // end 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화

    // 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정
    m_fDelCapChagingSkill = 0.0f;
    // end 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정

    // 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
    m_bDeadMessageEnable    = TRUE;

    m_dwDeadTime    = 0;
    // end 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
    // 2010. 05. 27 by jskim 시네마 적용 카메라 구현
    SetShowCinema ( FALSE );
    //end 2010. 05. 27 by jskim 시네마 적용 카메라 구현

    
    // 2011-06-30 by jhahn  인피3차 시네마 스킵금지
    m_InfiState        = TRUE;
    //end 2011-06-30 by jhahn  인피3차 시네마 스킵금지

// 2012-09-20 by jhahn 인비지블에서 파트너 아이템 사용가능
    HpCharge = FALSE;                 //2011-10-06 by jhahn 파트너 성장형 시스템
    ShieldCharge = FALSE;             //2011-10-06 by jhahn 파트너 성장형 시스템
    SpCharge = FALSE;                 //2011-10-06 by jhahn 파트너 성장형 시스템
//end 2012-09-20 by jhahn 인비지블에서 파트너 아이템 사용가능

    // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
    m_dwPetUseItemTime_Spell    = 0;
    m_dwPetUseItemTime_Hp        = 0;
    m_dwPetUseItemTime_Dp        = 0;
    m_dwPetUseItemTime_Sp        = 0;
    // END 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정

    // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
    m_fReverserIgnoreTime = 0.0f;
    // END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리

    m_bLendSoundPlay = FALSE;                // 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
}

CShuttleChild::~CShuttleChild()
{
    FLOG( "~CShuttleChild()" );

    util::del(m_pGameData);

    if(m_pCharacterInfo)//추가
    {
        m_pCharacterInfo->InvalidateDeviceObjects();
        m_pCharacterInfo->DeleteDeviceObjects();
        util::del(m_pCharacterInfo);
    }

    util::del( m_pCinemaUnitPattern );            // 2004.06.17 jschoi 스킬 패턴
    util::del( m_pCinemaCamera );            // 2004.06.17 jschoi 스킬 카메라

    util::del( m_pPrimaryWeapon );
    util::del( m_pSecondaryWeapon );

    util::del(m_pStoreData);    // m_pPrimaryWeapon, m_pSecondaryWeapon 뒤에 지워야 한다.

    util::del(m_pSkill);        // 2004-07-23 jschoi 스킬
    util::del(m_pClientParty);    // 2004-08-19 jschoi 파티

    util::del(m_pGuildWarInfo);
//    InvalidateShadow();

    util::del(m_pPkNormalTimer);
    
    g_pShuttleChild = NULL;
    m_bTargetChange = FALSE;

    // 2007-11-09 by dgwoo 무기 메쉬를 배열에서 벡터로 변경.
    m_vectInvenWeaponMesh.clear();
}

void CShuttleChild::SetPos(const D3DXVECTOR3 &vPos)
{
    FLOG( "CShuttleChild::SetPos(const D3DXVECTOR3 &vPos)" );
    m_vPos = vPos;
}

HRESULT CShuttleChild::InitDeviceObjects()
{
    FLOG( "CShuttleChild::InitDeviceObjects()" );
    if(m_pIDChat && !m_pIDChat->m_pChild)
    {
        CChatMoveData * pData = new CChatMoveData(m_pIDChat,m_myShuttleInfo.CharacterName, COLOR_CHARACTER_ID00,9,256,32);
        pData->InitDeviceObjects(); // Restore는 CShuttleChild::RestoreDeviceObjects()에서 한다.
        pData = (CChatMoveData *)m_pIDChat->AddChild(pData);
    }

    // remodling, 2004-03-21 by dhkwon
    m_pStoreData = new CStoreData;
    m_pSkill = new CSkill;
    m_pClientParty = new CClientParty;

    char strPath[256];
    g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "select.tex");
    SetResourceFile(strPath);
    
    DataHeader    * pDataHeader = NULL;
    pDataHeader = FindResource("inven");
//    m_pImgBack->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
    D3DXCreateTextureFromFileInMemory( g_pD3dDev, pDataHeader->m_pData, pDataHeader->m_DataSize,
                                         &m_pTexturesBackInven );

//    D3DUtil_CreateTexture( g_pD3dDev, pDataHeader->m_pData, &m_pTexturesBackInven );
    // 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
    m_pPetManager    = new CPetManager;
    //end 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리

    // 2013-05-28 by bhsohn 아머 컬렉션 시스템
    if(g_pDatabase)
    {
        g_pDatabase->InitArmorCollectionInfo();
    }
    // END 2013-05-28 by bhsohn 아머 컬렉션 시스템

    return S_OK;
}

HRESULT CShuttleChild::SetResourceFile(char* szFileName)
{
    FLOG( "CShuttleChild::SetResourceFile(char* szFileName)" );
    util::del(m_pGameData);
    m_pGameData = new CGameData;
    if(!m_pGameData->SetFile(szFileName, FALSE, NULL, 0))
    {
        util::del(m_pGameData);
        return E_FAIL;
    }

    return S_OK;
}

DataHeader * CShuttleChild::FindResource(char* szRcName)
{
    FLOG( "CShuttleChild::FindResource(char* szRcName)" );
    DataHeader* pHeader = NULL;
    if(m_pGameData)
    {
        pHeader = m_pGameData->Find(szRcName);
    }
    return pHeader;
}

HRESULT CShuttleChild::RestoreDeviceObjects()
{
    if(m_pIDChat)
        m_pIDChat->RestoreDeviceObjects();
    if(m_pMoveChat)
        m_pMoveChat->RestoreDeviceObjects();
    //RestoreShadow();
    return CUnitData::RestoreDeviceObjects();
}

/*
void CShuttleChild::RestoreChat()
{
    FLOG( "CShuttleChild::RestoreChat()" );
    if(m_pIDChat)
        m_pIDChat->RestoreDeviceObjects();
    if(m_pMoveChat)
        m_pMoveChat->RestoreDeviceObjects();
    RestoreShadow();
}
*/
//void CShuttleChild::InvalidateChat()
//{
//    FLOG( "CShuttleChild::InvalidateChat()" );
//    if(m_pIDChat)
//        m_pIDChat->InvalidateDeviceObjects();
//    if(m_pMoveChat)
//        m_pMoveChat->InvalidateDeviceObjects();
//    InvalidateShadow();
//}
HRESULT CShuttleChild::InvalidateDeviceObjects()
{
    FLOG( "CShuttleChild::InvalidateChat()" );
    if(m_pIDChat)
        m_pIDChat->InvalidateDeviceObjects();
    if(m_pMoveChat)
        m_pMoveChat->InvalidateDeviceObjects();

    return CUnitData::InvalidateDeviceObjects();
}

HRESULT CShuttleChild::DeleteDeviceObjects()
{
    FLOG( "CShuttleChild::DeleteDeviceObjects()" );

    util::del(m_pGameData);
    if(m_pIDChat)// && m_pIDChat->m_pChild)
    {
        m_pIDChat->DeleteDeviceObjects();
        util::del(m_pIDChat);
    }
    if(m_pMoveChat)
    {
        m_pMoveChat->DeleteDeviceObjects();
        util::del(m_pMoveChat);
    }
    if(m_pCharacterInfo)//추가
    {
        m_pCharacterInfo->InvalidateDeviceObjects();
        m_pCharacterInfo->DeleteDeviceObjects();
        util::del(m_pCharacterInfo);
    }

    util::del(m_pSkill);
    util::del(m_pClientParty);
    // 삭제전 미리 이펙트 포인트 체크
//    if(m_pWeapon1_1)
//    {
//        m_pWeapon1_1->m_bUsing = FALSE;
//        m_pWeapon1_1 = NULL;
//    }
    if(m_pWeapon1_1_1)
    {
        m_pWeapon1_1_1->m_bUsing = FALSE;
        m_pWeapon1_1_1 = NULL;
    }
    if(m_pWeapon1_1_2)
    {
        m_pWeapon1_1_2->m_bUsing = FALSE;
        m_pWeapon1_1_2 = NULL;
    }
    if(m_pWeapon1_2)
    {
        m_pWeapon1_2->m_bUsing = FALSE;
        m_pWeapon1_2 = NULL;
    }
//    if(m_pWeapon2_1)
//    {
//        m_pWeapon2_1->m_bUsing = FALSE;
//        m_pWeapon2_1 = NULL;
//    }
    if(m_pWeapon2_1_1)
    {
        m_pWeapon2_1_1->m_bUsing = FALSE;
        m_pWeapon2_1_1 = NULL;
    }
    if(m_pWeapon2_1_2)
    {
        m_pWeapon2_1_2->m_bUsing = FALSE;
        m_pWeapon2_1_2 = NULL;
    }
    if(m_pWeapon2_2)
    {
        m_pWeapon2_2->m_bUsing = FALSE;
        m_pWeapon2_2 = NULL;
    }

    // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    if( m_pRobotAGearWeapon_1 )
    {
        m_pRobotAGearWeapon_1->m_bUsing        = FALSE;
        m_pRobotAGearWeapon_1                = NULL;
    }
    // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

    if(m_pEngine)
    {
        m_pEngine->m_bUsing = FALSE;
        m_pEngine = NULL;
    }
    if(m_pRadar)
    {
        m_pRadar->m_bUsing = FALSE;
        m_pRadar = NULL;
    }
//    if(m_pArmor)
//    {
//        m_pArmor->m_bUsing = FALSE;
//        m_pArmor = NULL;
//    }
    if(m_pContainer)
    {
        m_pContainer->m_bUsing = FALSE;
        m_pContainer = NULL;
    }
    if(m_pAccessories)
    {
        m_pAccessories->m_bUsing = FALSE;
        m_pAccessories = NULL;
    }
    // 2010-06-08 by dgwoo, 펫시스템 추가. 
    if(m_pPartner)
    {
        m_pPartner->m_bUsing = FALSE;
        m_pPartner = NULL;
    }
    if(m_pPartner1)
    {
        m_pPartner1->m_bUsing = FALSE;
        m_pPartner1 = NULL;
    }

    if(m_pDummyPartner)
    {
        m_pDummyPartner->m_bUsing = FALSE;
        m_pDummyPartner = NULL;
    }
    
    if(m_pDummyPartner1)
    {
        m_pDummyPartner1->m_bUsing = FALSE;
        m_pDummyPartner1 = NULL;
    }
    // 2010-06-08 by dgwoo, 펫시스템 추가. 
    if(m_pWingIn)
    {
        m_pWingIn->m_bUsing = FALSE;
        m_pWingIn = NULL;
    }    
    SAFE_RELEASE( m_pTexturesBackInven );

    // 2007-11-09 by dgwoo 무기 메쉬를 배열에서 벡터로 변경.
    m_vectInvenWeaponMesh.clear();

    // 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
    if(m_pPetManager)
        util::del(m_pPetManager);
    //end 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
    return S_OK;
}


void CShuttleChild::UnitInfoBarSmoothRepair(float fElapsedTime)
{
    // HP - 회복시 서서히 부드럽게 회복
    if(m_fNextHP > m_myShuttleInfo.CurrentHP)
    {
        m_myShuttleInfo.CurrentHP += m_fNextHPRate*fElapsedTime;
        if(m_myShuttleInfo.CurrentHP > m_fNextHP)
            m_myShuttleInfo.CurrentHP = m_fNextHP;
        if(g_pGameMain->m_pUnitInfoBar)
        {
            g_pGameMain->m_pUnitInfoBar->SetHP(m_myShuttleInfo.CurrentHP, (float)m_myShuttleInfo.HP);
        }
    }
    // EP - 회복시 서서히 부드럽게 회복
    if(m_fNextEP > m_myShuttleInfo.CurrentEP)
    {
        m_myShuttleInfo.CurrentEP += m_fNextEPRate*fElapsedTime;
        if(m_myShuttleInfo.CurrentEP > m_fNextEP)
            m_myShuttleInfo.CurrentEP = m_fNextEP;
        if(g_pGameMain->m_pUnitInfoBar)
        {
            g_pGameMain->m_pUnitInfoBar->SetEP(m_myShuttleInfo.CurrentEP, (float)m_myShuttleInfo.EP);
        }
    }
    // SP - 회복시 서서히 부드럽게 회복
    if(m_fNextSP > m_myShuttleInfo.CurrentSP)
    {// SP의 경우 SHORT 형이기 때문에 아래의 코드 추가
        float fTempSP = m_myShuttleInfo.CurrentSP;
        fTempSP += m_fNextSPRate*fElapsedTime;
        m_myShuttleInfo.CurrentSP = (SHORT)fTempSP;
        if((float)m_myShuttleInfo.CurrentSP < fTempSP)
            m_myShuttleInfo.CurrentSP++;
        
        if(m_myShuttleInfo.CurrentSP > m_fNextSP)
            m_myShuttleInfo.CurrentSP = m_fNextSP;
        if(g_pGameMain->m_pUnitInfoBar)
        {
            g_pGameMain->m_pUnitInfoBar->SetSP((float)m_myShuttleInfo.CurrentSP, (float) m_myShuttleInfo.SP);
        }
    }
    // HP - 회복시 서서히 부드럽게 회복
    if(m_fNextDP > m_myShuttleInfo.CurrentDP)
    {
        m_myShuttleInfo.CurrentDP += m_fNextDPRate*fElapsedTime;
        if(m_myShuttleInfo.CurrentDP > m_fNextDP)
            m_myShuttleInfo.CurrentDP = m_fNextDP;
        if(g_pGameMain->m_pUnitInfoBar)
        {
            g_pGameMain->m_pUnitInfoBar->SetDP(m_myShuttleInfo.CurrentDP, (float)m_myShuttleInfo.DP);
        }
    }
}

void CShuttleChild::TurnOnWall(float fElapsedTime)
{
    // 2005-02-02 by jschoi - 우주맵은 이 함수 적용 안됨.
    // 2007-01-04 by bhsohn 무브 스킬로 맵 타일에 부딪혔을 에 대한 버그처리
    //if(IS_SPACE_MAP_INDEX(m_myShuttleInfo.MapChannelIndex.MapIndex) == TRUE)
    // 우주맵 판단은 이제 타일맵이 있고 없냐로 판단한다.
    if(FALSE == IsTileMapTickEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
    {
        return;
    }
    // 타일 정보 찾기 
    D3DXVECTOR3 Vp = D3DXVECTOR3(0,1,0);                // 유닛의 앞뒷면을 찾기위한 함수 
    float ads = D3DXVec3Dot(&Vp, &m_vUp);    

    int nmyXCheck = (int)(m_vPos.x / TILE_SIZE);
    int nmyYCheck = (int)(m_vPos.z / TILE_SIZE);
    
    int max_x = ((int)g_pGround->m_projectInfo.sXSize - REFLEXION_TILE_MIN_SIZE);
    int max_y = ((int)g_pGround->m_projectInfo.sYSize - REFLEXION_TILE_MIN_SIZE);
    
//    if(m_fShuttleSpeed > 0)
    // 2005-08-01 by ispark
    // 맵 벽에 충돌시 마우스가 이상한 행동 버그 수정 충돌후 떨어졌을 때 else 소스 부분 사용 안함
    if(m_dwState == _FALLEN || m_dwState == _FALLING)
    {
        m_bReflexionKeyBlock = FALSE;
    }
    else
    {
        if(m_nCurrentPatternNumber == 0)
        {
            if( nmyXCheck <= REFLEXION_TILE_MIN_SIZE+10 )
            { // 타일끝 경고 메세지
                g_pGImage->SetImageInfo(OUT_ZONE);
            }
            else if( nmyYCheck <= REFLEXION_TILE_MIN_SIZE+10 )
            { // 타일끝 경고 메세지
                g_pGImage->SetImageInfo(OUT_ZONE);
            }
            else if( nmyXCheck >= max_x-10 )
            { // 타일끝 경고 메세지
                g_pGImage->SetImageInfo(OUT_ZONE);
            }
            else if( nmyYCheck >= max_y-10 )
            { // 타일끝 경고 메세지
                g_pGImage->SetImageInfo(OUT_ZONE);
            }
        }

        if( nmyXCheck <= REFLEXION_TILE_MIN_SIZE )// X축타일이 1 보다 작을 경우  
        {
            D3DXVECTOR3 BottomVec = D3DXVECTOR3(0,0,-1);
            float reS = D3DXVec3Dot(&m_vVel, &BottomVec);
            
            POINT pt,pt2;
            int nX;
            GetCursorPos(&pt);
            pt2 = pt;
            ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
            CheckMouseReverse(&pt);
            
            if(reS*100 <= -1)
            {
                if(IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind))
                {
                        if(ads > 0 )
                            m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                        else 
                            m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;

//                        m_fShuttleSpeed = SHUTTLE_CONNER_MOVE_SPEED;            // 2005-07-11 by ispark    // 쓰지 않아서 지움
                        
                        nX = pt2.x - pt.x;
                        nX += g_pD3dApp->GetBackBufferDesc().Width/2 + 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
                else
                {
                    if(ads > 0) 
                        m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                    else 
                        m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;
                        
                    nX = pt2.x - pt.x;
                    nX += g_pD3dApp->GetBackBufferDesc().Width/2 + 100*g_pD3dApp->GetBackBufferDesc().Width/800;            
                }
                
            }
            else                    
            {
                if(IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind))
                {
                    
                        if(ads > 0 )m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;
                            
//                    if(m_fShuttleSpeed < 0) m_fShuttleSpeed = -20;                // 2005-07-11 by ispark    // 쓰지 않아서 지움
//                    else m_fShuttleSpeed = SHUTTLE_CONNER_MOVE_SPEED;
                        nX = pt2.x - pt.x;
                        nX += g_pD3dApp->GetBackBufferDesc().Width/2 - 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                    
                }
                else
                {
                    if(ads > 0 )m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;
                            
                    nX = pt2.x - pt.x;
                    nX += g_pD3dApp->GetBackBufferDesc().Width/2 - 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
            }
            // 2008-11-13 by bhsohn 조이스틱 작업
            m_bSetCursorPos = TRUE;

            SetCursorPos(nX,pt2.y);        
            SetUnitMovingAnimation(fElapsedTime);
//            m_bReflexionKeyBlock = TRUE;                                // 2006-09-27 by ispark
        }
        else if( nmyYCheck <= REFLEXION_TILE_MIN_SIZE )// Z축타일이 1 보다 작을 경우  
        {
            D3DXVECTOR3 BottomVec = D3DXVECTOR3(1,0,0);
            float reS = D3DXVec3Dot(&m_vVel, &BottomVec);
            
            POINT pt,pt2;
            int nX;
            GetCursorPos(&pt);
            pt2 = pt;
            ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
            CheckMouseReverse(&pt);
            
            if(reS*100 <= -1)
            {
                if(IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind))
                {
                        if(ads > 0 )m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;

//                    if(m_fShuttleSpeed < 0) m_fShuttleSpeed = -20;                // 2005-07-11 by ispark    // 쓰지 않아서 지움
//                    else m_fShuttleSpeed = SHUTTLE_CONNER_MOVE_SPEED;

                        nX = pt2.x - pt.x;
                        nX += g_pD3dApp->GetBackBufferDesc().Width/2 + 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
                else
                {
                    if(ads > 0 )m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                        else m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;

                    nX = pt2.x - pt.x;
                    nX += g_pD3dApp->GetBackBufferDesc().Width/2 + 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
            }
            else 
            {
                if(IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind))
                {
                        if(ads > 0 )m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;
                            
//                    if(m_fShuttleSpeed < 0) m_fShuttleSpeed = -20;                // 2005-07-11 by ispark    // 쓰지 않아서 지움
//                    else m_fShuttleSpeed = SHUTTLE_CONNER_MOVE_SPEED;

                        nX = pt2.x - pt.x;
                        nX += g_pD3dApp->GetBackBufferDesc().Width/2 - 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                        
                }
                else
                {
                    if(ads > 0 )m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;
                            
                    nX = pt2.x - pt.x;
                    nX += g_pD3dApp->GetBackBufferDesc().Width/2 - 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
            }        
            // 2008-11-13 by bhsohn 조이스틱 작업
            m_bSetCursorPos = TRUE;

            SetCursorPos(nX,pt2.y);        
            SetUnitMovingAnimation(fElapsedTime);
//            m_bReflexionKeyBlock = TRUE;                            // 2006-09-27 by ispark    
        }
        else if( nmyXCheck >= max_x )// X축타일을 255 넘길경우 
        {
            D3DXVECTOR3 BottomVec = D3DXVECTOR3(0,0,1);
            float reS = D3DXVec3Dot(&m_vVel, &BottomVec);
            
            POINT pt,pt2;
            int nX;
            GetCursorPos(&pt);
            pt2 = pt;
            ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
            CheckMouseReverse(&pt);
            
            if(reS*100 <= -1)
            {
//                DBGOUT("Dot(%f)\n",reS);
                if(IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind))
                {
                        if(ads > 0 )m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;
                        
//                    if(m_fShuttleSpeed < 0) m_fShuttleSpeed = -20;                // 2005-07-11 by ispark    // 쓰지 않아서 지움
//                    else m_fShuttleSpeed = SHUTTLE_CONNER_MOVE_SPEED;
                    
                        nX = pt2.x - pt.x;
                        nX += g_pD3dApp->GetBackBufferDesc().Width/2 + 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
                else
                {
                    if(ads > 0 ) m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                        else m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;

                    nX = pt2.x - pt.x;
                    nX += g_pD3dApp->GetBackBufferDesc().Width/2 + 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
            }
            else 
            {
                if(IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind))
                {
                        if(ads > 0 )m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;
                            
//                    if(m_fShuttleSpeed < 0) m_fShuttleSpeed = -20;                // 2005-07-11 by ispark    // 쓰지 않아서 지움
//                    else m_fShuttleSpeed = SHUTTLE_CONNER_MOVE_SPEED;
                    
                        nX = pt2.x - pt.x;
                        nX += g_pD3dApp->GetBackBufferDesc().Width/2 - 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
                else
                {
                    if(ads > 0 )m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;
                            
                    nX = pt2.x - pt.x;
                    nX += g_pD3dApp->GetBackBufferDesc().Width/2 - 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
            }
            // 2008-11-13 by bhsohn 조이스틱 작업
            m_bSetCursorPos = TRUE;

            SetCursorPos(nX,pt2.y);        
            SetUnitMovingAnimation(fElapsedTime);
//            m_bReflexionKeyBlock = TRUE;                                // 2006-09-27 by ispark
//            DBGOUT("Dot(%f)\n",reS);
        }
        else if( nmyYCheck >= max_y )// Z축타일을 255 넘길경우 
        {
            D3DXVECTOR3 BottomVec = D3DXVECTOR3(-1,0,0);
            float reS = D3DXVec3Dot(&m_vVel, &BottomVec);
            
            POINT pt,pt2;
            int nX;
            GetCursorPos(&pt);
            pt2 = pt;
            ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
            CheckMouseReverse(&pt);
            
            if(reS*100 <= -1)
            {
                if(IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind))
                {
                        if(ads > 0 )m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;
    
//                    if(m_fShuttleSpeed < 0) m_fShuttleSpeed = -20;                // 2005-07-11 by ispark    // 쓰지 않아서 지움
//                    else m_fShuttleSpeed = SHUTTLE_CONNER_MOVE_SPEED;
                    
                        nX = pt2.x - pt.x;
                        nX += g_pD3dApp->GetBackBufferDesc().Width/2 + 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
                else
                {
                    if(ads > 0 )m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                        else m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;

                    nX = pt2.x - pt.x;
                    nX += g_pD3dApp->GetBackBufferDesc().Width/2 + 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
            }
            else 
            {
                if(IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind))
                {
                        if(ads > 0 )m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;
                            
//                    if(m_fShuttleSpeed < 0) m_fShuttleSpeed = -20;                // 2005-07-11 by ispark    // 쓰지 않아서 지움
//                    else m_fShuttleSpeed = SHUTTLE_CONNER_MOVE_SPEED;
                    
                        nX = pt2.x - pt.x;
                        nX += g_pD3dApp->GetBackBufferDesc().Width/2 - 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
                else
                {
                    if(ads > 0 )m_fAniFlag = ANI_REFLEXION_LEFT_FLAG;    // 셔틀의 앞뒷면을 구분해서 리플렉션 방향설정 
                            else m_fAniFlag = ANI_REFLEXION_RIGHT_FLAG;
                            
                    nX = pt2.x - pt.x;
                    nX += g_pD3dApp->GetBackBufferDesc().Width/2 - 100*g_pD3dApp->GetBackBufferDesc().Width/800;
                }
            }
            if(m_fAniFlag)
            { 
                // 2008-11-13 by bhsohn 조이스틱 작업
                m_bSetCursorPos = TRUE;

                SetCursorPos(nX,pt2.y);        
                SetUnitMovingAnimation(fElapsedTime);
//                m_bReflexionKeyBlock = TRUE;                        // 2006-09-27 by ispark
            }
//            DBGOUT("Dot(%f)\n",reS);
        }
        else
        {
            m_bReflexionKeyBlock = FALSE;        
        }
    }

}
void CShuttleChild::TickCity(float fElapsedTime)
{
    switch(m_dwState)
    {
    case _NORMAL:
        break;
    case _LANDING:
        {
            EVENT_POINT UnitPoint;
            BOOL bResult = FALSE;
            if(m_pCinemaUnitPattern != NULL)
            {
                bResult = m_pCinemaUnitPattern->SkillTick();
                UnitPoint=m_pCinemaUnitPattern->GetCurrentCinemaPoint();
                m_vPos = UnitPoint.vPosition;                // 위치
                m_vVel = UnitPoint.vDirection;                // 방향
                m_vUp = UnitPoint.vUpVector;
                m_vTargetPosition = UnitPoint.vTarget;            
                int nPatternType = m_pCinemaUnitPattern->GetHeader().nPatternType;
                switch(nPatternType)
                {
                case TARGET_ON:    // 타켓 방향이 있는 패턴
                    m_vVel = m_vTargetPosition - m_vPos;
                    D3DXVec3Normalize(&m_vVel,&m_vVel);    
                    break;
                case TARGET_OFF: // 타켓 방향이 없는 패턴
                    break;
                }

            }
            if( m_vPos.y <= 19 )//bResult)
            {
                ChangeSingleBodyCondition(BODYCON_LANDED_MASK);
                m_vPos = MAP_TYPE_CITY_UNIT_POS;
                m_vVel.y = 0;
                DeletePattern();
                ChangeUnitState(_LANDED);
                SendPacket(T_FC_MOVE_LANDING_DONE);
                DBGOUT("T_FC_MOVE_LANDING_DONE\n");
                // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리
                if(IS_DT(m_myShuttleInfo.UnitKind))
                {
                    // A기어 라면 움직임 초기화
                    InitUnitStop();
                }
                // end 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리
            }
        }
        break;
    case _LANDED:
        {
        }
        break;
    case _TAKINGOFF:
        {
            EVENT_POINT UnitPoint;
            BOOL bResult = FALSE;
            if(m_pCinemaUnitPattern != NULL)
            {
                bResult = m_pCinemaUnitPattern->SkillTick();
                UnitPoint=m_pCinemaUnitPattern->GetCurrentCinemaPoint();
                m_vPos = UnitPoint.vPosition;                // 위치
                m_vVel = UnitPoint.vDirection;                // 방향
                m_vUp = UnitPoint.vUpVector;
                m_vTargetPosition = UnitPoint.vTarget;            
                int nPatternType = m_pCinemaUnitPattern->GetHeader().nPatternType;
                switch(nPatternType)
                {
                case TARGET_ON:    // 타켓 방향이 있는 패턴
                    m_vVel = m_vTargetPosition - m_vPos;
                    D3DXVec3Normalize(&m_vVel,&m_vVel);    
                    break;
                case TARGET_OFF: // 타켓 방향이 없는 패턴
                    break;
                }

            }
            if(bResult)
            {
                if(m_nEventType == EVENT_CITY_OUTDOOR)
                {
                    g_pInterface->m_pCityBase->SendEventRequestWarpToOutdoor();
                }
                else if(m_nEventType == EVENT_CITY_OUT_MOVE)
                {
                    CINFCityWarp* pCityWarp = g_pInterface->m_pCityBase->GetCityWarp();
                    ASSERT_ASSERT(pCityWarp);
                    pCityWarp->SendFieldSocketRequestShopWarp();
                }
                DeletePattern();
                ChangeUnitState(_NORMAL);
            }
        }
        break;
    }
    TickBurn(fElapsedTime);
    CheckAniTime();
    D3DXMatrixLookAtRH( &m_mMatrix, &m_vPos, &(m_vPos + m_vVel), &m_vUp);
    D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
    if(m_pCharacterInfo)
    {
        m_pCharacterInfo->SetAllBodyConditionMatrix( m_mMatrix );
        m_pCharacterInfo->Tick(fElapsedTime);
    }
    CheckShadowPos();

    // 2005-03-18 by jschoi - For ToolTip
    GetCursorPos(&m_pt);
    ScreenToClient(g_pD3dApp->GetHwnd(), &m_pt);                
    CheckMouseReverse(&m_pt);


//    const int nKeyMove = 1; // 키보드 감도
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_LEFT))
//    {
//        g_pCamera->SetCameraRotate(nKeyMove,0);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_RIGHT))
//    {
//        g_pCamera->SetCameraRotate(-nKeyMove,0);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_UP))
//    {
//        g_pCamera->SetCameraRotate(0,nKeyMove);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_DOWN))
//    {
//        g_pCamera->SetCameraRotate(0,-nKeyMove);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_PRIOR))
//    {
//        g_pCamera->SetCameraZoomInOut(nKeyMove);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_NEXT))
//    {
//        g_pCamera->SetCameraZoomInOut(-nKeyMove);
//    }


}

void CShuttleChild::MoveLandingStation(float fElapsedTime)
{
    
    if(g_pTutorial->IsTutorialMode() == TRUE &&
        //g_pTutorial->GetLesson() != L1
        g_pTutorial->GetChapterState() == TUTORIAL_STATE_READY)
        return;

    int            nXCheck, nYCheck;
    if(m_bKeyBlock)        return;        // 착륙중이면 키블럭
    
    m_fKeyboardInputTimer -= fElapsedTime;         // 키입력 타이머 

    // 2008-11-13 by bhsohn 조이스틱 작업
//    if( (g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S)          // 키입력시에만 계산 (필요없는 딜레이 삭제)     
//            || g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W)
//            || g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_A)
//            || g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_D)) 
    if( (g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S)          // 키입력시에만 계산 (필요없는 딜레이 삭제)     
            || g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W)
            || g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_A)
            || g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_D)) 
        && m_bReflexionKeyBlock == FALSE 
        && m_dwState != _SHOP)
    {    
        nXCheck = (int)(m_vPos.x / TILE_SIZE);
        nYCheck = (int)(m_vPos.z / TILE_SIZE);
        m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize + nYCheck].bEnableLand;            // 타일정보가 착륙장일때만 실행                 

        if(m_bMoveGroundType == 1)
        {                
            if(m_fKeyboardInputTimer < 0)
            {
                m_fKeyboardInputTimer = KEYBOARD_INPUT_TIMER;
                // 2008-11-13 by bhsohn 조이스틱 작업
                //if(!g_pD3dApp->m_bChatMode && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S) && !m_bAniKeyBlock)
                if(!g_pD3dApp->m_bChatMode && g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S) && !m_bAniKeyBlock)
                {   // 뒤로 이동
                    //m_vPos.x = (int(m_vPos.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;                                // 자신의 중심점 위치 계산
                    //m_vPos.z = (int(m_vPos.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
                    
                    D3DXVECTOR3 temp;

                    temp = m_vPos - m_vVel * TILE_SIZE;
                    temp.x = (int(temp.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;                                // 자신의 중심점 위치 계산
                    temp.z = (int(temp.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
                    
                    D3DXVec3Normalize(&m_vVel, &(m_vPos - temp));

                    m_bAniKeyBlock = TRUE;
                    m_vAniFrame = m_vPos;
                    m_vAniFrame = m_vPos - m_vVel * TILE_SIZE;
                    m_vAniFrame.x = (int(m_vAniFrame.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
                    m_vAniFrame.z = (int(m_vAniFrame.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;                            
                    
                    nXCheck = (int)(m_vAniFrame.x / TILE_SIZE);
                    nYCheck = (int)(m_vAniFrame.z / TILE_SIZE);                                
                    m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize + nYCheck].bEnableLand;                                
                    
                    BOOL Een = ScanEnemyFrontBAckAndSpace();
                    if(m_bMoveGroundType == 1 && !Een)
                    {    
                        m_fAniFlag = ANI_UPDOWN_FLAG;
                        m_vMyOldPos = m_vPos;                                    
                    }
                    else
                    {
                        m_bAniKeyBlock = FALSE;
                    }
                    //else if(IS_DT(m_myShuttleInfo.UnitKind))
                    //{
                    //    m_fAniFlag = 1;
                    //    m_vMyOldPos = m_vPos;
                    //}
                }
                // 2008-11-13 by bhsohn 조이스틱 작업
                //else if(!g_pD3dApp->m_bChatMode && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W) && !m_bAniKeyBlock)
                else if(!g_pD3dApp->m_bChatMode && g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W) && !m_bAniKeyBlock)
                {   // 앞으로 이동
                    
                    D3DXVECTOR3 temp;

                    temp = m_vPos + m_vVel * TILE_SIZE;
                    temp.x = (int(temp.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;                                // 자신의 중심점 위치 계산
                    temp.z = (int(temp.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
                    
                    D3DXVec3Normalize(&m_vVel, &(temp - m_vPos));
                    
                    m_bAniKeyBlock = TRUE;
                    m_vAniFrame = m_vPos;                                
                    m_vAniFrame = m_vAniFrame + m_vVel * TILE_SIZE;
                    m_vAniFrame.x = (int(m_vAniFrame.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
                    m_vAniFrame.z = (int(m_vAniFrame.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
                    
                    nXCheck = (int)(m_vAniFrame.x / TILE_SIZE);
                    nYCheck = (int)(m_vAniFrame.z / TILE_SIZE);                                
                    m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize + nYCheck].bEnableLand;
                    
                    BOOL Een = ScanEnemyFrontBAckAndSpace();
                    if(m_bMoveGroundType == 1 && !Een) // 고쳐야할부분 A기어 관련 이동 
                    {
                        m_fAniFlag = ANI_UPDOWN_FLAG;
                        m_vMyOldPos = m_vPos;                                    
                    }
                    else
                    {
                        m_bAniKeyBlock = FALSE;
                    }
                    //else if(m_myShuttleInfo.UnitKind == 256)
                    //{
                    //    m_fAniFlag = 1;
                    //    m_vMyOldPos = m_vPos;
                    //}
                    
                }
                // 2008-11-13 by bhsohn 조이스틱 작업
                //else if(!g_pD3dApp->m_bChatMode && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_A) && !m_bAniKeyBlock)
                else if(!g_pD3dApp->m_bChatMode && g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_A) && !m_bAniKeyBlock)
                {   // 좌로 회전    
                    m_bAniKeyBlock = TRUE;
                    m_fAniFlag = ANI_LEFT_FLAG;    
                    SetTargetVectorforLandedMove();
                    
                }
                // 2008-11-13 by bhsohn 조이스틱 작업
                //else if(!g_pD3dApp->m_bChatMode && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_D) && !m_bAniKeyBlock)
                else if(!g_pD3dApp->m_bChatMode && g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_D) && !m_bAniKeyBlock)
                {   // 우로 이동
                    m_bAniKeyBlock = TRUE;
                    m_fAniFlag = ANI_RIGHT_FLAG;    
                    SetTargetVectorforLandedMove();
                }
                                    
            }
        }
    }
    if(m_fAniFlag) SetUnitMovingAnimation(fElapsedTime);            
}


void CShuttleChild::TickBurn(float fElapsedTime)
{
    ASSERT_ASSERT(m_fBURN>0.0f);
    
    // 2006-07-05 by ispark, 프리미엄 개념 변경
    // 2006-07-06 by ispark, 무한 부스터 삭제
//    if(    m_myShuttleInfo.CurrentEP > 0 && 
//        g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1 )
//    {
//        // 2005-03-28 by jschoi - 연료가 있는 슈퍼 프리미엄 사용자는 무한 부스터
//        m_fCurrentBURN = m_fBURN;
//        if(g_pGameMain->m_pUnitInfoBar)
//        {
//            g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
//        }
//        m_bReChargeBURN = TRUE;
//        m_bUseBooster = TRUE;
//        return;
//    }
    
//    float fOldBURN = m_fCurrentBURN;
    if(m_nBooster == BOOSTER_TYPE_MAX && m_nHyperBooster == HYPER_BOOSTER_NORMAL)
    {
        if(m_fCurrentBURN > 0.0f)
        {
// 2013-01-31 by mspark, 게임포지 부스터 사용 중 추락 시 부스터가 소비되는 문제 수정
#ifdef C_FALL_OF_BOOSTER_USE_STOP_MSPARK
            if(m_dwState != _FALLING)
            {
            m_fCurrentBURN -= fElapsedTime;
            }
#else
            m_fCurrentBURN -= fElapsedTime;
#endif
// end 2013-01-31 by mspark, 게임포지 부스터 사용 중 추락 시 부스터가 소비되는 문제 수정
            
            if(g_pGameMain->m_pUnitInfoBar)
            {
                g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN); 
            }
        }
        if(m_fCurrentBURN <= 0.0f)     // 2012-06-29 by jhahn 무한 부트터 오류 수정
        {
            m_fCurrentBURN = 0.0f;
            m_bReChargeBURN = FALSE;
        }
    }
    else
    {
//        if(!m_bReChargeBURN)
//        {
//            m_fCurrentBURN += (m_fBURN*fElapsedTime)/BOOSTER_OVERHEAT_REPAIR_TIME;
//        }
//        else
//        {
//            m_fCurrentBURN += fElapsedTime/2.0f;
//        }
        if(m_fCurrentBURN < m_fBURN)
        {
            m_fCurrentBURN += (m_fBURN*fElapsedTime)/BOOSTER_OVERHEAT_REPAIR_TIME;
            if(m_fCurrentBURN/m_fBURN >= 0.9f)
            {
                m_bReChargeBURN = TRUE;
            }
            if(m_fCurrentBURN > m_fBURN)
            {
                m_fCurrentBURN = m_fBURN;
            }
            if(g_pGameMain->m_pUnitInfoBar)
            {
                g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
            }
        }
    }

    // 2005-07-11 by ispark    
    // 중력 계산을 안했음. 그래서 맵 경계선에서 부스터 버그가 있었음
//    if( m_bUseBooster == FALSE && m_fShuttleSpeed <= m_fShuttleSpeedMax)
//    if( m_bUseBooster == FALSE && m_fShuttleSpeed <= m_fRealShuttleSpeedMax)
    if( m_bUseBooster == FALSE && m_fShuttleSpeed - m_fTempGravity <= m_fRealShuttleSpeedMax)
    {
        m_bUseBooster = TRUE;
    }
//    if(m_fCurrentBURN <= 0.0f || m_fBURN == 0.0f)
//        m_bReChargeBURN = FALSE;
//    else if(m_fCurrentBURN/m_fBURN >= 0.9f)
//        m_bReChargeBURN = TRUE;
//    if(fOldBURN != m_fCurrentBURN)
//    {
//        ASSERT_ASSERT(m_fBURN>0.0f);
//        if(g_pGameMain->m_pUnitInfoBar)
//        {
//            g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
//        }
//    }

}

void CShuttleChild::TickWeapon(float fElapsedTime)
{
     TickChangeClientArea();
    if(m_pPrimaryWeapon)
    {
//        m_PrimaryAttack.fCheckAutoMaticTimer -= fElapsedTime;
//        m_PrimaryAttack.fCheckAttackTime -= fElapsedTime;

        // 2005-08-02 by ispark
        // 착륙때나 이륙때는 발싸를 막음
//        if( IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster))
        if( IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster) && m_bWarpLink == FALSE)
        {
            m_pPrimaryWeapon->Tick( fElapsedTime, m_bLButtonState );
        }
        else
        {
            m_pPrimaryWeapon->Tick( fElapsedTime, FALSE );
        }
    }
    if( m_pSecondaryWeapon )
    {
//        m_SecondaryAttack.fCheckAutoMaticTimer -= fElapsedTime;
//        m_SecondaryAttack.fCheckAttackTime -= fElapsedTime;
//        if(m_fMissileFireTime > 0)
//        {
//            m_fMissileFireTime -= fElapsedTime;
//            if(m_fMissileFireTime <= 0)
//            {
//                m_fMissileFireTime = 0;
//            }
//        }

        // 2005-08-02 by ispark
        // 착륙때나 이륙때는 발싸를 막음
//        if( IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster) )
        if( IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster) && m_bWarpLink == FALSE)
        {
            m_pSecondaryWeapon->Tick( fElapsedTime, m_bRButtonState );
        }
        else
        {
            m_pSecondaryWeapon->Tick( fElapsedTime, FALSE );
        }
    }
    // 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
    if( m_pPetManager &&
        m_pPetManager->GetWeaponItemInfo() ) 
    {
        if(IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster) && m_bWarpLink == FALSE)
        {
            m_pPetManager->GetWeaponItemInfo()->Tick( fElapsedTime, m_bLButtonState );
        }
        else
        {
            m_pPetManager->GetWeaponItemInfo()->Tick( fElapsedTime, FALSE );
        }
    }
    // end 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리

}



void CShuttleChild::Tick(float fElapsedTime)
{
    FLOG( "CShuttleChild::Tick(float fElapsedTime)" );

    // 2007-05-28 by bhsohn 에어 시즈 무조건 데미지 들어가는 버그 수정
    m_bAGearFollowEnemy = FALSE;

    // 2008-11-13 by bhsohn 조이스틱 작업
    m_bSetCursorPos = FALSE;

    // 테스트 코드
// 2005-08-08 by ispark
// 기존 F11키를 없앤다.
//#ifdef _DEBUG
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_F11))
//    {// 저속
//        m_bUnitStop = !m_bUnitStop;
//    }
//
//    if(m_bUnitStop)
//    {
//        m_fShuttleSpeed = 0.0f;
//        m_fTempGravity = 0.0f;
//        m_fRealShuttleSpeedMin = 0.0f;
//    }
//#endif
    // 2007-03-29 by dgwoo 옵저버 모드일때 단축키값에 따라 유저를 변경.
    if(m_bObserve)
    {
        TickObserve(fElapsedTime);
    }
    if(m_bOperation)
    {
        TickOperation(fElapsedTime);
    }
//    if(m_pSkillEffect)
//    {
//        m_pSkillEffect->Tick(fElapsedTime);
//    }
    
    // Ground Attack
    if(m_pSecondaryWeapon && m_pSecondaryWeapon->GetAttackMode() == ATT_TYPE_GROUND_BOMBING_SEC)
    {
        SetGroundTargetPos();
    }
    // 2006-12-01 by ispark, 공중 폭격
    else if(m_pSecondaryWeapon && m_pSecondaryWeapon->GetAttackMode() == ATT_TYPE_AIR_BOMBING_SEC)
    {
        SetAirGroundTargetPos();
    }

    // 파티 Tick
    if(m_pClientParty->IsParty())
    {
        m_pClientParty->Tick(fElapsedTime);
    }
    
    if(m_dwState == _INITIALIZING)
    {
        return; // initializing
    }

    // 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결
    // 2012-11-07 by bhsohn 베트남 패킷 처리
#ifdef C_CLIENT_LIVE_TIME
    if(g_pD3dApp->m_bClientQuit || g_pD3dApp->GetClientLogOff())
    {
//         if(g_pD3dApp->m_bClientQuit)
//         {
//             DBGOUT("g_pD3dApp->m_bClientQuit[%d] \n", g_pD3dApp->m_bClientQuit);
//         }
//         if(g_pD3dApp->GetClientLogOff())
//         {
//             DBGOUT("g_pD3dApp->GetClientLogOff() [%d] \n", g_pD3dApp->GetClientLogOff());
// 
//         }
        
    }
    else if(g_pD3dApp->IsLockMode() == FALSE)
    {
        return;
    }
#endif
    // END 2012-11-07 by bhsohn 베트남 패킷 처리

    // 2005-02-15 by jschoi - Pk Delay Timer
    m_pPkNormalTimer->Tick(fElapsedTime);

    // 옵저버 모드시에는 자동으로 올라가는걸 막는다.
    if(!IsObserverMode())
    {
        UnitInfoBarSmoothRepair(fElapsedTime);
    }
    CheckDamageBodyCondition();
    UsedItemTick(fElapsedTime);
    m_pSkill->Tick(fElapsedTime);
    // 클라이언트 타이머 계산
    m_timeProcess.Tick(fElapsedTime);

    if(g_pScene->m_byMapType == MAP_TYPE_CITY)
    {
        TickCity(fElapsedTime);
    }
    else
    {
        TurnOnWall(fElapsedTime);
        if(m_dwState == _LANDED && !IS_DT(m_myShuttleInfo.UnitKind) && m_bLandedMove)        // 착륙중에는 사용 금지                            
        {
            MoveLandingStation(fElapsedTime);
        }
        if(g_pScene->m_byMapType == MAP_TYPE_TUTORIAL)
        {
            CheckClientQuestList();
        }

        if(m_fCollSendDamageCheckTime >= 0.0f)
        {
            m_fCollSendDamageCheckTime -= fElapsedTime;
        }
        if(m_fGetOtherInfoCheckTime >= 0.0f)
        {
            m_fGetOtherInfoCheckTime -= fElapsedTime;
        }
        if(m_fGetOtherInfoCheckTime < 0.0f && m_bGetOtherInfo)
        {
            m_bGetOtherInfo = FALSE;
            m_nGetOtherIndex = 0;
        }

        D3DXVECTOR3  vUpTemp;
        D3DXMATRIX    matTemp;
        m_bIsUnderWater = FALSE;
        m_vOldPos = m_vPos;

        HandleInput(fElapsedTime);
        CheckMouseDir();

        // 이벤트를 체크하여 적용한다.
        if(m_dwState == _NORMAL || m_bSkillAirMove)
        {
            CheckEventType();
            CheckEventTypeObject();
        }

        // 착륙장에서 다른장소로 텔레포트시..
        if(m_nEventIndex != 0)
        {
            int nSpeed = (int)(m_fShuttleSpeed - m_fTempGravity);
            if(nSpeed <= 0 || m_bSkipPattern == TRUE)
            {
//                CObjectChild* pObj = g_pScene->FindObjectByIndex(m_nEventIndex);
                CObjectChild* pObj = g_pScene->FindRandomObjectByEventType(EVENT_TYPE_OBJ_ENTER_BUILDING, TRUE);
                m_nEventIndex = 0;
                m_bKeyBlock = FALSE;
                m_bSkipPattern = FALSE;
                if(pObj)
                {
                    // 위치 이동
                    g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
                    // 렌덤 위치 지정
//                    m_vPos = pObj->m_vPos;//CAtumSJ::GetRandomXZVec3(&pObj->m_vPos, SIZE_CITY_POSITION_RANDOM_XZ_RADIUS);
                    m_vPos = CAtumSJ::GetRandomXZVec3(&pObj->m_vPos, SIZE_CITY_POSITION_RANDOM_XZ_RADIUS);
                    g_pCharacterChild->m_vPos = m_vPos;
                    // 2010. 01. 25 by jskim 워프 이동시 맵 컬링 잘못하는 버그 수정 
                    g_pD3dApp->SetCamPosInit();                    
                    //end 2010. 01. 25 by jskim 워프 이동시 맵 컬링 잘못하는 버그 수정 
                    DBGOUT("GetRandomXZVec3 %d, %d, %d\n", (int)m_vPos.x, (int)m_vPos.y, (int)m_vPos.z);

                    // 메세지 처리
                    INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE,
                        T_FC_CHARACTER_CHANGE_CHARACTER_MODE, pSMode, SendBuf);
                    pSMode->CharacterMode0 = TRUE;
                    pSMode->PositionAVec3 = m_vPos;
                    pSMode->TargetAVec3 = m_vVel*1000.0f;
                    g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE));
                    // 2005-11-03 by ispark
                    // 편대 비행 중이었따면 해체
                    if(    m_pClientParty &&
                        m_pClientParty->IsFormationFlightMove())
                    {
                        SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
                        m_pClientParty->FormationFlightClear();
                        g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PARTY_0001,COLOR_SYSTEM);//"편대비행에서 벗어났습니다."
                    }
                    // 2005-07-28 by ispark
                    // 캐릭터 초기 바디 컨디션 보냄. 그 이후는 보내지 않는다. Client가 스스로 바디컨디션을 판단한다.
                    SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, BODYCON_SET_OR_CLEAR_MASK);
                    // 캐릭터 모드 변환
                    g_pD3dApp->m_bCharacter = TRUE;
                    // 2007-02-22 by dgwoo 맵셋팅.
                    g_pInterface->m_pGameMain->InitWarp();
                    m_bLandingField = FALSE;
                    m_pTarget = NULL;
                    m_pOrderTarget = NULL;
                    g_pCharacterChild->InitCharacterData();
                }                
            }            
        }

        if(m_bSkillMoveIsUse )//&& m_bEventReady == FALSE)
        {
            UseSkillMove(fElapsedTime);
        }
        else
        {
            // 2005-06-01 by jschoi
            if(m_bIsAir == FALSE && IS_DT(m_myShuttleInfo.UnitKind))
            {
                Move_Ground(fElapsedTime);
            }
            else
            {
                // 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
                if(m_bLendSoundPlay)
                {
                    g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );
                    m_bLendSoundPlay = FALSE;
                }
                // end 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
                Move_Air(fElapsedTime);
            }
        }

        // 2006-01-18 by ispark, Dead 중간 정검, 
        // 만약에 서버로부터 4초동안 메세지를 받지 못하면 강제로 _FALLING을 한다.
        if(m_fCheckDead >= 1.0f)
        {
            m_fCheckDead += fElapsedTime;
            if(m_fCheckDead > 5.0f)
            {
                if(!UNIT_STATE_DEAD(m_dwState))
                {
                    DBGOUT("My State[%d] --> Dead State\n", m_dwState);
                    ChangeUnitStateDead();
                }
                else
                {
                    m_fCheckDead = 0.0f;
                }
            }
        }

        // Burn
        TickBurn(fElapsedTime);
        // 2006-11-27 by dgwoo 공격이 가능할때만 Weapon함수로 들어간다.
        if(m_bAttack)
            TickWeapon(fElapsedTime);              
        
        PetUseItme();      // 2012-09-20 by jhahn 인비지블에서 파트너 아이템 사용가능
        
        CheckTarget();


        // 2005-03-31 by jschoi
        SetEffectPos(fElapsedTime);

        CheckGroundEffect();
        CheckOnObject();            // 오브젝트 위인지 체크하는 함수
//        m_bOnObject = FALSE;        // 오브젝트 위인지 변수를 초기화 -> 이변수는 오브젝트의 Tick에서 결정된다.
        CheckAniTime();
        m_pMoveChat->Tick();
        m_pIDChat->Tick();
        // 1~2형의 발사시 락온 이펙트를 위한 변수 초기화
//        m_bPrimaryAttackFire = FALSE;
//        m_bSecondaryAttackFire = FALSE;

//        for(int i = 0;i<MAX_MINE_NUM;i++)
//        {
//            m_pCurrentMine[i] = NULL;
//        }
//        m_nCurrentMineCont = 0;

        // 2006-12-04 by ispark, 체프 사출
        ChaffDataTick();
    }

    // 2007-04-05 by dgwoo Move Air 아래로 위치 변경.
    if(m_pSkillEffect)
    {
        m_pSkillEffect->Tick(fElapsedTime);
        // 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정
        CCharacterInfo*  pCharacterInfo = m_pSkillEffect->FindSkillEffect(BGEAR_STATE_CHARINGSHOT);
        if(m_pSkill && pCharacterInfo)
        {                
            if(!m_pSkill->IsSkillUse(BGEAR_STATE_CHARINGSHOT))
            {
                m_fDelCapChagingSkill += g_pD3dApp->GetCheckElapsedTime();
                if(m_fDelCapChagingSkill > BGEAR_CHAGINGSHOT_SHOT_CAP_TIME)
                {                    
                    // 차징 스킬 제거
                    m_pSkillEffect->DeleteChargingStateSkillEffect();
                    m_fDelCapChagingSkill = 0.0f;
                }                                    
            }
            else
            {
                m_fDelCapChagingSkill = 0.0f;
            }
        }
        // end 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정
    }
    SetUnitState();

    // 2005-12-29 by ispark, 체크 장착 아이템
    CheckWearItem(fElapsedTime);

//    const int nKeyMove = 1; // 키보드 감도
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_LEFT))
//    {
//        g_pCamera->SetControlCamSideAngle(nKeyMove);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_RIGHT))
//    {
//        g_pCamera->SetControlCamSideAngle(-nKeyMove);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_UP))
//    {
//        g_pCamera->SetControlCamAngle(nKeyMove);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_DOWN))
//    {
//        g_pCamera->SetControlCamAngle(-nKeyMove);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_PRIOR))
//    {
//        g_pCamera->SetControlCamPos(nKeyMove);
//    }
//    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_NEXT))
//    {
//        g_pCamera->SetControlCamPos(-nKeyMove);
//    }

    // 2005-03-08 by jschoi
    if((IsPkEnableMap(m_myShuttleInfo.MapChannelIndex.MapIndex) ||
        m_pPkNormalTimer->IsCityWar()) &&
        g_pScene->FindWarpGateByPosition(m_vPos,CHECK_WARP_GATE_DISTANCE))
    {
        m_bWarpGateZone = TRUE;
    }
    else
    {
        m_bWarpGateZone = FALSE;
    }

    // 2008-11-13 by bhsohn 조이스틱 작업
    if(g_pD3dApp->IsUseJoyStick())
    {        
        if(!m_bSetCursorPos)
        {
            SetCursorJostickPos();
        }
        CheckJoystickKeyDown();
    }
    
    // end 2008-11-13 by bhsohn 조이스틱 작업

    
    // 2009. 02. 18 by ckPark 시즈 모드 해제시 부스터 이펙트 버그
    if( IS_DT(m_myShuttleInfo.UnitKind)
        && ( GetCurrentBodyCondition() & BODYCON_SIEGE_OFF_MASK ) )
    {
        if(GetCurrentAnimationTime() >= GetCurrentBodyConditionEndAnimationTime())
            ChangeSingleBodyCondition(BODYCON_LANDED_MASK);
    }
    // end 2009. 02. 18 by ckPark 시즈 모드 해제시 부스터 이펙트 버그
    // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음
    TickCheckGearAttackStat();    
    // end 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CShuttleChild::GetAttackClientArea()
/// \brief        마우스가 공격할수 있는 클라이언트 영역에 있는가를 체크.
/// \author        dgwoo
/// \date        2008-01-14 ~ 2008-01-14
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::TickChangeClientArea()
{
    BOOL bMousePosArea = TRUE;
    //ScreenToClient()
    m_bNowMousePosWindowArea =  IsMousePosClientArea();
    if(!m_bNowMousePosWindowArea)
    {// 마우스가 클라이언트 영역을 벗어났을경우.
        bMousePosArea = FALSE;
        // 2008-03-13 by dgwoo  무기의 셋팅을 초기화.
        m_bLButtonState = FALSE;
        m_bRButtonState = FALSE;
    }
    if(m_bNowMousePosWindowArea != m_bOldMousePosWindowArea)
    {
        //DBGOUT("m_bNowMouseArea = %d, m_bOldMouseArea = %d",m_bNowMousePosWindowArea,m_bOldMousePosWindowArea);
        m_bOldMousePosWindowArea = m_bNowMousePosWindowArea;
        // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음
        //if(GetKeyState(VK_LBUTTON) & 0x8000)
        if((GetKeyState(VK_LBUTTON) & 0x8000) && IsPossibleAttackStat()) 
        {
            m_bLButtonState = TRUE;
        }
        else
        {
            m_bLButtonState = FALSE;
        }
        // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음                            
        //if(GetKeyState(VK_RBUTTON) & 0x8000)
        if((GetKeyState(VK_RBUTTON) & 0x8000) && IsPossibleAttackStat())
        {
            m_bRButtonState = TRUE;
        }
        else
        {
            m_bRButtonState = FALSE;
        }
        //DBGOUT("m_bLButton = %d(%x), m_bRButton = %d(%x)\n",m_bLButtonState,GetKeyState(VK_LBUTTON),m_bRButtonState,GetKeyState(VK_RBUTTON));
    }
    return bMousePosArea;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::SetUnitState()
/// \brief        화면 상단에 유닛의 상태 표시
/// \author        dhkwon
/// \date        2004-06-01 ~ 2004-06-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetUnitState()
{
    float fWeight = g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo);
    if(fWeight > 0.9f)
    {
        g_pGameMain->m_pUnitState->InsertState(INF_UW_FULLITEM3_MASK);
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM2_MASK);
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM1_MASK);
    }
    else if(fWeight > WEIGHT_RATE_VERY_HEAVY)
    {
        g_pGameMain->m_pUnitState->InsertState(INF_UW_FULLITEM2_MASK);
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM3_MASK);
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM1_MASK);
    }
    else if(fWeight > WEIGHT_RATE_HEAVY)
    {
        g_pGameMain->m_pUnitState->InsertState(INF_UW_FULLITEM1_MASK);
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM2_MASK);
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM3_MASK);
    }
    else
    {
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM1_MASK);
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM2_MASK);
        g_pGameMain->m_pUnitState->OffState(INF_UW_FULLITEM3_MASK);
    }
    if(!m_bReChargeBURN) // 부스터 과열
    {
        g_pGameMain->m_pUnitState->InsertState(INF_UW_OVERHEAT_MASK);
    }
    else
    {
        g_pGameMain->m_pUnitState->OffState(INF_UW_OVERHEAT_MASK);
    }
    if(m_nBooster == BOOSTER_TYPE_AIRBREAK) // 공중 부양
    {
        g_pGameMain->m_pUnitState->SetState(INF_US_STOP_MASK);
    }
    else
    {
        g_pGameMain->m_pUnitState->OffState(INF_US_STOP_MASK);
    }
    if(m_bMouseLock && m_bUnitStop == FALSE)                            // 2005-08-08 by ispark 유닛정지시 마우스를 막는다. 그러나 STATE가 UNIT ROCK 상태가 아니다.
    {
//        g_pGameMain->m_pUnitState->OffState(INF_US_STOP_MASK);
//        g_pGameMain->m_pUnitState->SetState(INF_US_LOCK_MASK);
    }
    else
    {
        g_pGameMain->m_pUnitState->OffState(INF_US_LOCK_MASK);
    }
    if( m_dwState == _TAKINGOFF )    // 이륙
    {
        g_pGameMain->m_pUnitState->SetState(INF_US_TAKEOFF_MASK);
    }
    else
    {
        g_pGameMain->m_pUnitState->OffState(INF_US_TAKEOFF_MASK);
    }
    if( m_dwState == _LANDED )    // 착륙
    {
        // 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
        // 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
        //g_INFCnt = 0;
        g_pD3dApp->SetINFCnt(0);
        // end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
        // end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
        g_pGameMain->m_pUnitState->SetState(INF_US_LAND_MASK);        
    }
    else
    {
        g_pGameMain->m_pUnitState->OffState(INF_US_LAND_MASK);
    }
    if( m_pClientParty->IsFormationFlightMove() ) // 편대비
    {
        g_pGameMain->m_pUnitState->SetState(INF_US_FORM_MASK);
    }
    else
    {
        g_pGameMain->m_pUnitState->OffState(INF_US_FORM_MASK);
    }
    // 2005-07-11 by ispark
    // 이동 가능한 최대 높이 자동 셋팅
    if(m_vPos.y > g_pScene->m_nMaxAtitudeHeight + g_pGround->m_projectInfo.fWaterHeight - 100)
    {
        g_pGameMain->m_pUnitState->InsertState(INF_UW_TOOHIGH_MASK);
    }
    else
    {
        g_pGameMain->m_pUnitState->OffState(INF_UW_TOOHIGH_MASK);
    }
}
void CShuttleChild::CheckMapRegion()
{
    FLOG( "CShuttleChild::CheckMapRegion()" );
    // 맵을 벗어나면 못나가게 셋팅 체크
    if(g_pScene->m_byMapType == MAP_TYPE_TUTORIAL)
    {
        if(m_vPos.x < 2660.0f)
            m_vPos.x = 2660.0f;
        if(m_vPos.x > 3760.0f)
            m_vPos.x = 3760.0f;
        if(m_vPos.z < 67.0f)
            m_vPos.z = 67.0f;
        if(m_vPos.z > 967)
            m_vPos.z = 967;
        // 버그 발생으로 무한 높이로 올라갔다면 내린다.
        if(m_vPos.y < 0.0f)// || m_vPos.y < -1000.0f)
            m_vPos.y = 0.0f;
        if(m_vPos.y > 400.0f)// || m_vPos.y < -1000.0f)
            m_vPos.y = 400.0f;
    }
    else
    {
        if(m_vPos.x < 40.0f)
            m_vPos.x = 40.0f;
        if(m_vPos.x > (g_pGround->m_projectInfo.sXSize-1)*TILE_SIZE)
            m_vPos.x = (g_pGround->m_projectInfo.sXSize-1)*TILE_SIZE;
        if(m_vPos.z < 40.0f)
            m_vPos.z = 40.0f;
        if(m_vPos.z > (g_pGround->m_projectInfo.sYSize-1)*TILE_SIZE)
            m_vPos.z = (g_pGround->m_projectInfo.sYSize-1)*TILE_SIZE;
        // 버그 발생으로 무한 높이로 올라갔다면 내린다. 셔틀의 최대 높이
//        if(m_vPos.y > g_pGround->m_projectInfo.fHeightMax + WEATHER_SUNNY_FOG_END)// || m_vPos.y < -1000.0f)
//            m_vPos.y = g_pGround->m_projectInfo.fHeightMax + WEATHER_SUNNY_FOG_END;


        // 2005-02-02 by jschoi
        // 우주맵의 상하 이동 가능 반경을 확장
        // ALTITUDE_SAPCE_ALTITUDE 값 만큼 위,아래로 확장
        // 2006-04-12 by ispark, DB에서 맵 높이를 셋팅 하므로 우주맵이라고 해서 별도로 높일 이유가 없다.
//        if(IS_SPACE_MAP_INDEX(m_myShuttleInfo.MapChannelIndex.MapIndex) == TRUE)
//        {// 우주맵이라면
//            // 2005-07-11 by ispark
//            // 이동 가능한 최대 높이 자동 셋팅
//            if(m_vPos.y > g_pScene->m_nMaxAtitudeHeight + ALTITUDE_SAPCE_ALTITUDE + g_pGround->m_projectInfo.fWaterHeight)
//            {
//                m_vPos.y = g_pScene->m_nMaxAtitudeHeight + ALTITUDE_SAPCE_ALTITUDE + g_pGround->m_projectInfo.fWaterHeight;
//            }
////            else if(m_vPos.y < ALTITUDE_APPLY_MIN - ALTITUDE_SAPCE_ALTITUDE + g_pGround->m_projectInfo.fWaterHeight)
////            {
////                m_vPos.y = ALTITUDE_APPLY_MIN - ALTITUDE_SAPCE_ALTITUDE + g_pGround->m_projectInfo.fWaterHeight;
////            }
//        }
//        else
//        {// 우주맵이 아니라면
            // 2005-07-11 by ispark
            // 이동 가능한 최대 높이 자동 셋팅
            if(m_vPos.y > g_pScene->m_nMaxAtitudeHeight + g_pGround->m_projectInfo.fWaterHeight)
            {
                m_vPos.y = g_pScene->m_nMaxAtitudeHeight + g_pGround->m_projectInfo.fWaterHeight;
            }
//        }

        // 2005-12-12 by ispark, 지형을 그리지 않는 맵이라면 높이 0 이하로 가지 못하게 한다.
        if(!IsTileMapRenderEnable(m_myShuttleInfo.MapChannelIndex.MapIndex))
        {
            if(m_vPos.y <= 0)
                m_vPos.y = 0;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::CheckBazaarInfo(MSG_FC_BAZAAR_INFO_OK* pMsg)
/// \brief        개인상점 정보
/// \author        ispark
/// \date        2006-07-28 ~ 2006-07-28
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckBazaarInfo(MSG_FC_BAZAAR_INFO_OK* pMsg)
{
    if(m_myShuttleInfo.ClientIndex == pMsg->clientIndex0)
    {
        return;
    }
    else
    {
        CEnemyData * pEnemy = NULL;
        CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.find(pMsg->clientIndex0);
        if(itEnemy != g_pScene->m_mapEnemyList.end())
        {
            pEnemy = itEnemy->second;
        }

        if(pEnemy)
        {
            pEnemy->m_byBazaarType = pMsg->byBazaarType;
            if(pMsg->byBazaarType == 1)
            {
                wsprintf(pEnemy->m_szBazaarName, STRMSG_C_060811_0000, pMsg->szBazaarName);
                pEnemy->m_pMoveChatShop = new CChatMoveData(nullptr, pEnemy->m_szBazaarName,COLOR_SKILL_USE,11,500,32,240,TRUE, 0x0001);
                pEnemy->m_pMoveChatShop->InitDeviceObjects();
                pEnemy->m_pMoveChatShop->RestoreDeviceObjects();
            }
            else if(pMsg->byBazaarType == 2)
            {
                wsprintf(pEnemy->m_szBazaarName, STRMSG_C_060811_0001, pMsg->szBazaarName);
                pEnemy->m_pMoveChatShop = new CChatMoveData(nullptr, pEnemy->m_szBazaarName,GUI_FONT_COLOR_YM,11,500,32,240,TRUE, 0x0001);
                pEnemy->m_pMoveChatShop->InitDeviceObjects();
                pEnemy->m_pMoveChatShop->RestoreDeviceObjects();
            }
        }
    }
}

void CShuttleChild::CheckIndexList(MSG_FC_CHARACTER_GET_OTHER_INFO_OK* pMsg)
{
    FLOG( "CShuttleChild::CheckIndexList(MSG_FC_CHARACTER_GET_OTHER_INFO_OK* pMsg)" );
    // 2013-03-06 by bhsohn 복귀 유저 시스템
//      if(m_myShuttleInfo.ClientIndex == pMsg->CharacterInfo.ClientIndex)
//         return;
    if(m_myShuttleInfo.ClientIndex == pMsg->CharacterInfo.ClientIndex)
    {
        // 복귀 유저 정보는 갱신해주자        
//        g_pShuttleChild->SetRtnGameUser(pMsg->CharacterInfo.UsingReturnItem);
        return;
    }    
    else
    {
        if(pMsg->CharacterInfo.ClientIndex == m_nGetOtherIndex)
        {
            m_bGetOtherInfo = FALSE;
            m_fGetOtherInfoCheckTime = 0.0f;
            m_nGetOtherIndex = 0;
        }
        CEnemyData * pEnemy = NULL;
        CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.find(pMsg->CharacterInfo.ClientIndex);
        if(itEnemy != g_pScene->m_mapEnemyList.end())
        {
            if(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber == pMsg->CharacterInfo.CharacterUniqueNumber)
            {
                pEnemy = itEnemy->second;
            }
        }
        if(pEnemy)
        {
            // 2006-07-05 by ispark 상대 세력 멘트 삭제
#ifdef FREEWAR_
            if (!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pMsg->CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
#else
            if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pMsg->CharacterInfo.InfluenceType))
#endif
            {
                memset(pMsg->CharacterInfo.szCharacterMent, 0x00, sizeof(pMsg->CharacterInfo.szCharacterMent));
            }
            pEnemy->m_bIsInit = TRUE;
            pEnemy->m_infoCharacter.CharacterInfo = pMsg->CharacterInfo;
            pEnemy->m_dwPartType = (COMPARE_RACE(pMsg->CharacterInfo.Race, RACE_GAMEMASTER) ? _ADMIN:_ENEMY);
//            pEnemy->m_infoCharacter.UnitKind = pMsg->CharacterInfo.UnitKind;
//            pEnemy->m_infoCharacter.Race = pMsg->CharacterInfo.Race;
//            pEnemy->m_infoCharacter.GuildUniqueNumber = pMsg->CharacterInfo.GuildUniqueNumber;
//            pEnemy->m_infoCharacter.BodyCondition = pMsg->CharacterInfo.BodyCondition;
//            pEnemy->m_infoCharacter.ClientIndex = pMsg->CharacterInfo.ClientIndex;
            pEnemy->m_vPos = A2DX(pMsg->CharacterInfo.PositionVector);
            pEnemy->m_vNextPos = A2DX(pMsg->CharacterInfo.PositionVector);
            pEnemy->m_bEnemyCharacter = pMsg->CharacterInfo.CharacterMode0;            // 2005-12-30 by ispark
            // 2005-07-28 by ispark
            D3DXVec3Normalize(&pEnemy->m_vVel, &A2DX(pMsg->CharacterInfo.TargetVector));
            pEnemy->m_infoCharacter.CharacterRenderInfo = pMsg->CharacterRenderInfo;
            // 10. 03. 18 by jskim 몬스터변신 카드
            pEnemy->SetMonsterTransformer(pMsg->CharacterRenderInfo.RI_MonsterUnitKind_ForTransformer);
            //end 10. 03. 18 by jskim 몬스터변신 카드
            if(!pEnemy->m_pCharacterInfo)
            {
                // 2005-11-24 by ispark, ItemNum -> SourceIndex으로 변경


                // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
                //ITEM * pItem = g_pDatabase->GetServerItemInfo(pEnemy->m_infoCharacter.CharacterRenderInfo.RI_Center);
                // 외형아이템번호가 없다면 원래 아이템 번호를 사용한다
                int nItemNum = 0;
                if( pEnemy->m_infoCharacter.CharacterRenderInfo.RI_Center_ShapeItemNum )
                    nItemNum = pEnemy->m_infoCharacter.CharacterRenderInfo.RI_Center_ShapeItemNum;
                else
                    nItemNum = pEnemy->m_infoCharacter.CharacterRenderInfo.RI_Center;

                ITEM * pItem = g_pDatabase->GetServerItemInfo( nItemNum );
                // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
                // 10. 03. 18 by jskim 몬스터변신 스킬
                BOOL bMonTras = FALSE;
                if(pEnemy->GetMonsterTransformer())
                {
                    // 10. 03. 18 by jskim 몬스터변신 스킬
                    pEnemy->ChangeUnitCharacterInfo(nItemNum, pEnemy->m_infoCharacter.CharacterInfo.UnitKind, pMsg->CharacterInfo.BodyCondition, FALSE);
                    pEnemy->SetMonsterTransPrimarySidePos(pEnemy->m_PrimaryAttack.vSidePos);
                    pEnemy->SetMonsterTransSecondarySidePos(pEnemy->m_SecondaryAttack.vSidePos);
                }
                //end 10. 03. 18 by jskim 몬스터변신 스킬
                if(pItem)
                {
                    // 10. 03. 18 by jskim 몬스터변신 스킬
                    if(pEnemy->GetMonsterTransformer())
                    {
                        bMonTras = TRUE;                        
                        MEX_MONSTER_INFO * pTempIn = g_pGameMain->CheckMonsterInfo( pEnemy->GetMonsterTransformer() );
                        nItemNum = pTempIn->SourceIndex;            
                        pEnemy->ChangeUnitCharacterInfo(nItemNum, 
                            pEnemy->m_infoCharacter.CharacterInfo.UnitKind, pMsg->CharacterInfo.BodyCondition, FALSE , bMonTras);
                    }    
                    else
                    {
                        pEnemy->ChangeUnitCharacterInfo(pItem->SourceIndex, 
                        pEnemy->m_infoCharacter.CharacterInfo.UnitKind, pMsg->CharacterInfo.BodyCondition, FALSE , bMonTras);
                    }
                    //end 10. 03. 18 by jskim 몬스터변신 스킬
                }
                else
                {
                    // 디폴트
                    ChangeUnitCharacterInfo(0, 
                        pEnemy->m_infoCharacter.CharacterInfo.UnitKind, pMsg->CharacterInfo.BodyCondition, FALSE);
                }
                
                // 2005-07-28 by ispark
                // 캐릭터 렌더링 번호 입력
                pEnemy->SetPilotNumber(pEnemy->m_infoCharacter.CharacterInfo.PilotFace);
            }
            pEnemy->ChangeUnitBodyCondition(pMsg->CharacterInfo.BodyCondition);
            // 아이템 렌더링을 위한 정보 저장
            memcpy(&pEnemy->m_infoCharacter.CharacterRenderInfo,&pMsg->CharacterRenderInfo,sizeof(CHARACTER_RENDER_INFO));
        }
        else
        {
            // 2006-07-05 by ispark 상대 세력 멘트 삭제
#ifdef FREEWAR_
            if (!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pMsg->CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
#else
             if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pMsg->CharacterInfo.InfluenceType))
#endif
            {
                memset(pMsg->CharacterInfo.szCharacterMent, 0x00, sizeof(pMsg->CharacterInfo.szCharacterMent));
            }
            pEnemy = new CEnemyData(pMsg);
            g_pScene->InsertToBlockData(pEnemy);
//            pEnemy->m_infoCharacter.CharacterInfo = pMsg->CharacterInfo;
//            pEnemy->m_infoCharacter.ClientIndex = pMsg->CharacterInfo.ClientIndex;
//            pEnemy->m_infoCharacter.CharacterUniqueNumber = pMsg->CharacterInfo.CharacterUniqueNumber;
//            strcpy(pEnemy->m_infoCharacter.CharacterName,pMsg->CharacterInfo.CharacterName);//,sizeof(SIZE_MAX_CHARACTER_NAME));
//            pEnemy->m_infoCharacter.UnitKind = pMsg->CharacterInfo.UnitKind;
//            pEnemy->m_infoCharacter.Race = pMsg->CharacterInfo.Race;
//            pEnemy->m_infoCharacter.GuildUniqueNumber = pMsg->CharacterInfo.GuildUniqueNumber;
//            pEnemy->m_infoCharacter.BodyCondition = pMsg->CharacterInfo.BodyCondition;

//            pEnemy->m_vPos = A2DX(pMsg->CharacterInfo.PositionVector);
//            pEnemy->m_vNextPos = A2DX(pMsg->CharacterInfo.PositionVector);
//            if(COMPARE_BODYCON_BIT(pMsg->CharacterInfo.BodyCondition,BODYCON_DEAD_MASK))
//            {
//                pEnemy->ChangeUnitState( _FALLEN );
//                pMsg->CharacterInfo.BodyCondition = BODYCON_DEAD_MASK | BODYCON_DAMAGE1_MASK | BODYCON_DAMAGE2_MASK | BODYCON_DAMAGE3_MASK;
//            }
//            pEnemy->m_infoCharacter.CharacterRenderInfo = pMsg->CharacterRenderInfo;
            pEnemy->Init();
#ifdef FREEWAR_
            if (isFreeForAllMap(pEnemy->m_infoCharacter.CharacterInfo.MapChannelIndex.MapIndex))
            {
                strcpy(((CChatMoveData *)pEnemy->m_pIDChat->m_pChild)->m_szString, "");
            }
            else
            {
                strcpy(((CChatMoveData *)pEnemy->m_pIDChat->m_pChild)->m_szString, pEnemy->m_infoCharacter.CharacterInfo.CharacterName);
            }
#else
            strcpy(((CChatMoveData *)pEnemy->m_pIDChat->m_pChild)->m_szString, pEnemy->m_infoCharacter.CharacterInfo.CharacterName);
#endif
            
            // 2007-11-22 by bhsohn 아레나 통합서버
            // 아레나 맵에서는 '\'뒤는 서버이름이므로 빠릴 필요가없다.
            g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), ((CChatMoveData *)pEnemy->m_pIDChat->m_pChild)->m_szString);
            
            g_pScene->m_mapEnemyList[pEnemy->m_infoCharacter.CharacterInfo.ClientIndex] = pEnemy;
            pEnemy->ChangeBodyConditionFromServer(pMsg->CharacterInfo.BodyCondition);
        }
        pEnemy->InitItemPoint();
         PARTYENEMYINFO* pPartyEnemtInfo = m_pClientParty->PartyFindMemberByUniqueNumber(pMsg->CharacterInfo.CharacterUniqueNumber);
        if(pPartyEnemtInfo)
        {
            if(pPartyEnemtInfo->m_bPartyType == _PARTYOTHERMAPMASTER)
            {
                pPartyEnemtInfo->m_bPartyType = _PARTYMASTER;
            }
            else if(pPartyEnemtInfo->m_bPartyType == _PARTYOTHERMAPMEMBER)
            {
                pPartyEnemtInfo->m_bPartyType = _PARTYMEMBER;
            }
            pEnemy->m_nPartyID = m_pClientParty->GetPartyInfo().PartyID;
            pPartyEnemtInfo->m_pEnemyData = pEnemy;
            // 파티 정보 요청;
            MSG_FC_PARTY_GET_MEMBER sMsg;
            sMsg.CharacterUniqueNumber = pMsg->CharacterInfo.CharacterUniqueNumber;
            g_pFieldWinSocket->SendMsg(T_FC_PARTY_GET_MEMBER, (char*)&sMsg, sizeof(sMsg));
        }
    }
}

void CShuttleChild::CheckIndexList(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK* pMsg)
{
    FLOG( "CShuttleChild::CheckIndexList(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK* pMsg)" );
    CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.find(pMsg->MonsterIndex);
    if(itMonster != g_pScene->m_mapMonsterList.end())
    {
        memcpy(&itMonster->second->m_info,pMsg,sizeof(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK));
        return;
    }
    if(pMsg->MonsterIndex == m_nGetOtherIndex)
    {
        m_bGetOtherInfo = FALSE;
        m_fGetOtherInfoCheckTime = 0.0f;
        m_nGetOtherIndex = 0;
    }
    CMonsterData * pMonster = new CMonsterData(pMsg);
//    DBGOUT("MONSTER[%d] RENDER_INFO_OK Distance[%d]\n",pMsg->ClientIndex,
//            (int)g_pD3dApp->Distance3DTo2D(m_vPos, A2DX(pMsg->PositionVector)));
    g_pScene->InsertToBlockData(pMonster);
    // 바디컨디션 정리(new에서 다 처리됨)
/*    pMonster->ChangeBodyConditionFromServer(pMsg->BodyCondition);
    if(pMonster->m_pCharacterInfo)
    {
        pMonster->m_pCharacterInfo->SetBodyCondition(pMsg->BodyCondition);
    }
    if(COMPARE_BODYCON_BIT(pMonster->GetCurrentBodyCondition(),BODYCON_DEAD_MASK))
    {
        //pMonster->m_dwState = _EXPLODED;
        pMonster->ChangeUnitState( _EXPLODED );
    }
*/    g_pScene->m_mapMonsterList[pMsg->MonsterIndex] = pMonster;
}

void CShuttleChild::CheckMove(MSG_FC_MOVE_OK* pMsg)
{
    FLOG( "CShuttleChild::CheckMove(MSG_FC_MOVE_OK* pMsg)" );
    if(m_myShuttleInfo.ClientIndex == pMsg->ClientIndex)
    {
        return;
    }
    else
    {
        if(pMsg->ClientIndex < 10000)
        {
            CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.find(pMsg->ClientIndex);
            // 2005-12-30 by ispark, Enemy가 리스트에 없을 때 정보 요청
            if(itEnemy != g_pScene->m_mapEnemyList.end())
            {
                if(m_pClientParty->FormationFlightFindMemberByClientIndex(pMsg->ClientIndex) == NULL)
                {
                    itEnemy->second->SetPosition(pMsg);
                }
                
                // 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
                // 현재 적기 상태와 우리의 상태가 변경시 강제로 적기 셋팅
                UpdateEnemyState(itEnemy->second, pMsg);
                // end 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
            }
            else
            {
                // 2005-12-30 by ispark, 자주 요청 할 가망성이 있음 
//                if(!m_bGetOtherInfo)
                {
                     m_bGetOtherInfo = TRUE;
                    m_fGetOtherInfoCheckTime = 0.5f;
                    m_nGetOtherIndex = pMsg->ClientIndex;

                    // 2006-06-15 by ispark, 일정시간에 패킷을 보내는 것으로 변경
//                    MSG_FC_CHARACTER_GET_OTHER_INFO sMsg; 
//                    memset(&sMsg,0x00,sizeof(sMsg));
//                    char buffer[SIZE_MAX_PACKET];
//                    sMsg.ClientIndex = pMsg->ClientIndex;
//                    int nType = T_FC_CHARACTER_GET_OTHER_INFO;
//                    memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//                    memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//                    g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
                    SetClientIndexDataByVector(g_pD3dApp->m_vecCharacterOtherInfo, pMsg->ClientIndex);

                    if( UNIT_STATE_NO_MOVE(m_dwState))
                    {
//                        MSG_FC_MOVE_TARGET sMsg2;
//                        sMsg2.ClientIndex = pMsg->ClientIndex;
//                        g_pFieldWinSocket->SendMsg( T_FC_MOVE_TARGET, (char*)&sMsg2, sizeof(sMsg2));
                        SetClientIndexDataByVector(g_pD3dApp->m_vecMoveTarget, pMsg->ClientIndex);
                    }
                }
            }
        }
        else
        {
            CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.find(pMsg->ClientIndex);
            if(itMonster != g_pScene->m_mapMonsterList.end())
            {
                itMonster->second->SetPosition(pMsg);
            }
            else
            {
                // 리스트에 없으면 클라이언트 정보 요청
                if(!m_bGetOtherInfo)
                {
                    m_bGetOtherInfo = TRUE;
                    m_fGetOtherInfoCheckTime = 0.5f;
                    m_nGetOtherIndex = pMsg->ClientIndex;

                    // 2006-06-15 by ispark, 일정시간에 패킷을 보내는 것으로 변경
//                    MSG_FC_CHARACTER_GET_OTHER_INFO sMsg;
//                    memset(&sMsg,0x00,sizeof(sMsg));
//                    char buffer[SIZE_MAX_PACKET];
//                    sMsg.ClientIndex = pMsg->ClientIndex;
//                    int nType = T_FC_CHARACTER_GET_OTHER_INFO;
//                    memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//                    memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//                    g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
                    SetClientIndexDataByVector(g_pD3dApp->m_vecCharacterOtherInfo, pMsg->ClientIndex);
                }
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        적기의 상태를 갱신해준다. 
/// \author        // 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
/// \date        2007-04-02 ~ 2007-04-02
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::UpdateEnemyState(CEnemyData *pEnemyData, MSG_FC_MOVE_OK* pMsg)
{
    if(g_pShuttleChild->m_myShuttleInfo.ClientIndex == pMsg->ClientIndex)
    {
        // 자기 자신이다. 
        return;
    }
    // 적기가 아니다. 
    if (!IS_CHARACTER_CLIENT_INDEX(pMsg->ClientIndex))
    {
        return;
    }
    
    // Enemy skill 발동시
    CUnitData* pUnit = g_pScene->FindUnitDataByClientIndex( pMsg->ClientIndex );
    if(NULL == pUnit)
    {
        return;
    }
    float fElaspedTime = g_pD3dApp->GetCheckElapsedTime();
    float fSkillTime = 0;
    INT ItemNum = 0;
    bool bTmp = false;

    // 스킬 래벨 1 로 강제 셋팅
    INT nSkillUid[UNIT_STATE_MAX] = 
    {
        BGEAR_SKILL_BASENUM_INVISIBLE + 1,        // 인비지블
        BGEAR_STATE_CHARINGSHOT + 1,            // 차징샷
        AGEAR_STATE_HYPERSHOT + 1,                // 하이퍼샷
        AGEAR_STATE_BARRIER    + 1,                // 배리어    // 2013-01-30 by mspark, A기어 배리어 스킬 워프후에 상대방에게 보이지 않는 문제 수정
        MGEAR_STATE_INVINCIBLE + 1                // 무적        // 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
    };
    
    // 인비지블
    bTmp = pUnit->IsUnitState(UNIT_STATE_INVISIBLE);
    if(pMsg->moveBitFlag.Invisible0 != bTmp)
    {
        pUnit->AddSkillChgTime(UNIT_STATE_INVISIBLE, fElaspedTime); // 시간 추가
        fSkillTime = pUnit->GetSkillChgTime(UNIT_STATE_INVISIBLE);

        if(fSkillTime > UNIT_CHG_STATE_TIME)
        {
            pUnit->AddSkillChgTime(UNIT_STATE_INVISIBLE, -1);    // 변수 초기화 
            bTmp ^= TRUE;    //==(bTmp = !bTmp)
            pUnit->SetUnitSkillState(UNIT_STATE_INVISIBLE, bTmp);
            if(bTmp)
            {
                // ON                
                SetEmenyState(pUnit, nSkillUid[UNIT_STATE_INVISIBLE]);
            }
            else
            {
                ClearEmenyState(pUnit, nSkillUid[UNIT_STATE_INVISIBLE]);
            }

        }        
    }
    else
    {
        pUnit->AddSkillChgTime(UNIT_STATE_INVISIBLE, -1);    // 변수 초기화 
    }
    
    // 차징샷    
    bTmp = pUnit->IsUnitState(UNIT_STATE_CHARINGSHOT);
    if(pMsg->moveBitFlag.ChargingSkill != bTmp)
    {
        pUnit->AddSkillChgTime(UNIT_STATE_CHARINGSHOT, fElaspedTime);
        fSkillTime = pUnit->GetSkillChgTime(UNIT_STATE_CHARINGSHOT);
        
        if(fSkillTime > UNIT_CHG_STATE_TIME)
        {
            pUnit->AddSkillChgTime(UNIT_STATE_CHARINGSHOT, -1);    // 변수 초기화             
            bTmp ^= TRUE;
            pUnit->SetUnitSkillState(UNIT_STATE_CHARINGSHOT, bTmp);
            if(bTmp)
            {
                // ON                
                SetEmenyState(pUnit, nSkillUid[UNIT_STATE_CHARINGSHOT]);                
            }
            else
            {
                ClearEmenyState(pUnit, nSkillUid[UNIT_STATE_CHARINGSHOT]);
            }        
        }
    }        
    else
    {
        pUnit->AddSkillChgTime(UNIT_STATE_CHARINGSHOT, -1);    // 변수 초기화             
    }
    
    // 하이퍼샷
    bTmp = pUnit->IsUnitState(UNIT_STATE_HYPERSHOT);
    if(pMsg->moveBitFlag.HyperShot != bTmp)
    {
        pUnit->AddSkillChgTime(UNIT_STATE_HYPERSHOT, fElaspedTime);
        fSkillTime = pUnit->GetSkillChgTime(UNIT_STATE_HYPERSHOT);
        
        if(fSkillTime > UNIT_CHG_STATE_TIME)
        {
            pUnit->AddSkillChgTime(UNIT_STATE_HYPERSHOT, -1);    // 변수 초기화     
            bTmp ^= TRUE;
            pUnit->SetUnitSkillState(UNIT_STATE_HYPERSHOT, bTmp);
            if(bTmp)
            {
                // ON                
                SetEmenyState(pUnit, nSkillUid[UNIT_STATE_HYPERSHOT]);                
            }
            else
            {
                ClearEmenyState(pUnit, nSkillUid[UNIT_STATE_HYPERSHOT]);
            }
        }
    }
    else
    {
        pUnit->AddSkillChgTime(UNIT_STATE_HYPERSHOT, -1);    // 변수 초기화     
    }
    
    // 2013-01-30 by mspark, A기어 배리어 스킬 워프후에 상대방에게 보이지 않는 문제 수정
    bool bTemp = pUnit->IsUnitState(UNIT_STATE_BARRIER);
    if(pMsg->moveBitFlag.bUsingBarialSkill != bTemp)
    {
        pUnit->AddSkillChgTime(UNIT_STATE_BARRIER, fElaspedTime);
        fSkillTime = pUnit->GetSkillChgTime(UNIT_STATE_BARRIER);
        
        if(fSkillTime > UNIT_SEARCHEYE_CHG_STATE_TIME)
        {
            pUnit->AddSkillChgTime(UNIT_STATE_BARRIER, -1);    // 변수 초기화     
            bTemp ^= TRUE;
            pUnit->SetUnitSkillState(UNIT_STATE_BARRIER, bTemp);
            if(bTemp)
            {
                // ON                
                SetEmenyState(pUnit, nSkillUid[UNIT_STATE_BARRIER]);                
            }
            else
            {
                ClearEmenyState(pUnit, nSkillUid[UNIT_STATE_BARRIER]);
            }
        }
    }
    else
    {
        pUnit->AddSkillChgTime(UNIT_STATE_BARRIER, -1);    // 변수 초기화     
    }
    // end 2013-01-30 by mspark, A기어 배리어 스킬 워프후에 상대방에게 보이지 않는 문제 수정

    // 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
    bool bTemp2 = pUnit->IsUnitState(UNIT_STATE_INVINCIBLE);
    if(pMsg->moveBitFlag.bUsingInvicibleSkill != bTemp2)
    {
        pUnit->AddSkillChgTime(UNIT_STATE_INVINCIBLE, fElaspedTime);
        fSkillTime = pUnit->GetSkillChgTime(UNIT_STATE_INVINCIBLE);
        
        if(fSkillTime > UNIT_SEARCHEYE_CHG_STATE_TIME)
        {
            pUnit->AddSkillChgTime(UNIT_STATE_INVINCIBLE, -1);    // 변수 초기화     
            bTemp2 ^= TRUE;
            pUnit->SetUnitSkillState(UNIT_STATE_INVINCIBLE, bTemp2);
            if(bTemp2)
            {
                // ON                
                SetEmenyState(pUnit, nSkillUid[UNIT_STATE_INVINCIBLE]);                
            }
            else
            {
                ClearEmenyState(pUnit, nSkillUid[UNIT_STATE_INVINCIBLE]);
            }
        }
    }
    else
    {
        pUnit->AddSkillChgTime(UNIT_STATE_INVINCIBLE, -1);    // 변수 초기화     
    }
    // end 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정

// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#ifdef C_SEARCHEYE_SHUTTLE_POSITION_MSPARK
    bool byTmp = pUnit->IsUnitState(UNIT_STATE_SEARCHEYE);
    if(pMsg->moveBitFlag.bSearchEye_1 != byTmp)
    {
        pUnit->AddSkillChgTime(UNIT_STATE_SEARCHEYE, fElaspedTime);
        fSkillTime = pUnit->GetSkillChgTime(UNIT_STATE_SEARCHEYE);
        
        if(fSkillTime > UNIT_SEARCHEYE_CHG_STATE_TIME)
        {
            pUnit->AddSkillChgTime(UNIT_STATE_SEARCHEYE, -1);    // 변수 초기화     
            byTmp ^= TRUE;
            pUnit->SetUnitSkillState(UNIT_STATE_SEARCHEYE, byTmp);

            if(byTmp)
            {                                
                g_pD3dApp->SetPlaySearchEyeEffect(pUnit, RC_EFF_SEARCHEYE_SHUTTLE_POSITION, 0, false);
            }
            else
            {
                g_pD3dApp->ClearPlaySearchEyeEffect(pUnit, RC_EFF_SEARCHEYE_SHUTTLE_POSITION);
            }
        }
    }
    else
    {
        pUnit->AddSkillChgTime(UNIT_STATE_SEARCHEYE, -1);    // 변수 초기화     
    }

    bool byTmp2 = pUnit->IsUnitState(UNIT_STATE_SEARCHEYE2);
    if(pMsg->moveBitFlag.bSearchEye_2 != byTmp2)
    {
        pUnit->AddSkillChgTime(UNIT_STATE_SEARCHEYE2, fElaspedTime);
        fSkillTime = pUnit->GetSkillChgTime(UNIT_STATE_SEARCHEYE2);
                
        if(fSkillTime > UNIT_SEARCHEYE_CHG_STATE_TIME)
        {            
            pUnit->AddSkillChgTime(UNIT_STATE_SEARCHEYE2, -1);    // 변수 초기화     
            byTmp2 ^= TRUE;
            pUnit->SetUnitSkillState(UNIT_STATE_SEARCHEYE2, byTmp2);
            
            if(byTmp2)
            {                                
                g_pD3dApp->SetPlaySearchEyeEffect(pUnit, RC_EFF_SEARCHEYE_SHUTTLE_POSITION2, 0, false);
            }
            else
            {
                g_pD3dApp->ClearPlaySearchEyeEffect(pUnit, RC_EFF_SEARCHEYE_SHUTTLE_POSITION2);
            }
        }
    }
    else
    {
        pUnit->AddSkillChgTime(UNIT_STATE_SEARCHEYE2, -1);    // 변수 초기화     
    }
#endif
// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
    
}    

void CShuttleChild::SetEmenyState(CUnitData* pUnit, INT ItemNum)
{
    if(NULL == pUnit)
    {
        return;
    }
    // Skill Prepare
    if(pUnit->m_pSkillEffect)
    {
        pUnit->m_pSkillEffect->CreateSkillEffect(ItemNum, BODYCON_CHARGING_MASK);
    }

    // USE_SKILL_OK
    CEnemyData *pEnemyData = ((CEnemyData *)pUnit);    
    
    pEnemyData->UseSkillFromServer(ItemNum);
    ITEM* pItem = g_pDatabase->GetServerItemInfo(ItemNum);
    
    if(pUnit->m_pSkillEffect)
    {
        if(pUnit->m_pSkillEffect->FindSkillEffect(ItemNum))
        { 
            // 이미 생성된 스킬이 있다면..
            pUnit->m_pSkillEffect->ChangeBodyCondition(ItemNum, BODYCON_USE_SKILL_MASK);
        }
        else
        { // 생성된 스킬이 없다면 생성한다.
            
            // 순간이펙트 - 나중에 바꾸자..
            // 2005-08-23 by ispark
            // 2013-03-13 by bhsohn 스킬 번호 구조 수정
//            CAppEffectData* pEff = new CAppEffectData(pUnit,SKILL_BASE_NUM(pItem->SourceIndex),D3DXVECTOR3(0,0,0));
            CAppEffectData* pEff = new CAppEffectData(pUnit,SKILL_BASE_SOURCEINDEX(pItem->SourceIndex),D3DXVECTOR3(0,0,0));
            if(pEff->m_bUsing)
            {
                pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
            }
            else
            {
                delete pEff;
            }
        }
    }
}

void CShuttleChild::ClearEmenyState(CUnitData* pUnit, INT ItemNum)
{
    if(NULL == pUnit)
    {
        return;
    }            
    
    CEnemyData* pEnemy = ((CEnemyData *)pUnit);
    pEnemy->CancelUsingSkill(ItemNum);
}


void CShuttleChild::HandleInput(float fElapsedTime)
{
    FLOG( "CShuttleChild::HandleInput(float fElapsedTime)" );
    if(g_pD3dApp->GetHwnd()!=GetActiveWindow())
        return;

    static float fNkeyTime = 0.0f;
    static float fLockonkeyTime = 1.0f;

    static float fFkeyTime = 0.0f;
    static float fVkeyTime = 0.0f;
    static float fTabkeyTime = 0.0f;
//    static float fInsertKeyCheckTime = 0.0f;

    if(fFkeyTime >= 0.0f)
        fFkeyTime -= fElapsedTime;
    if(fVkeyTime >= 0.0f)
        fVkeyTime -= fElapsedTime;
    if(fTabkeyTime >= 0.0f)
        fTabkeyTime -= fElapsedTime;

//    if(fInsertKeyCheckTime >= 0.0f)
//        fInsertKeyCheckTime -= fElapsedTime;

    static float fDeletekeyTime = 0.0f;
    if(fDeletekeyTime >= 0.0f)
        fDeletekeyTime -= fElapsedTime;

    if(m_fCancelSkillCheckTime >= 0.0f)
        m_fCancelSkillCheckTime -= fElapsedTime;
    if(m_fChangeBodyForBoosterCheckTime >= 0.0f)
        m_fChangeBodyForBoosterCheckTime -= fElapsedTime;
    if(m_fNumpad0keyTime >= 0.0f)
        m_fNumpad0keyTime -= fElapsedTime;

    if( FAILED( g_pD3dApp->m_pKeyBoard->ReadImmediateData( g_pD3dApp->GetHwnd() ) ) )
    {
        return;
    }
#ifdef _DEBUG
    if(COMPARE_RACE(m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER| RACE_MONITOR))
    {
        if ( !g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LCONTROL )
            && !g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LMENU )
            && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_ADD ) )
        {
            m_fMouseRate += 0.1f*fElapsedTime;
        }
        if ( !g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LCONTROL ) 
            && !g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LMENU )
            && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_SUBTRACT ) )
        {
            m_fMouseRate -= 0.1f*fElapsedTime;
            if(m_fMouseRate < 0.0f)
            {
                m_fMouseRate = 0.0f;
            }
        }
        if ( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LCONTROL ) 
            && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_ADD ) )
        {
            g_pD3dApp->m_pCamera->SetDetailGround(0,true);
        }
        if ( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LCONTROL ) 
            && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_SUBTRACT ) )
        {
            g_pD3dApp->m_pCamera->SetDetailGround(0);
        }
        if ( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LMENU ) 
            && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_ADD ) )
        {
            g_pD3dApp->m_pCamera->SetDetailGround(1,true);
        }
        if ( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LMENU ) 
            && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_SUBTRACT ) )
        {
            g_pD3dApp->m_pCamera->SetDetailGround( 1 );
        }
    }
#endif // _DEBUG_endif
/*    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_TAB) && fTabkeyTime <= 0.0f)
    {// 강제 락온을 다른 타겟으로 바꾼다
        bool bCheck = false;
//        if(m_bPKMode)
        if(IsPkEnable())
        {
            CVecEnemyIterator itEnemy = g_pScene->m_vecEnemyRenderList.begin();
            while(itEnemy != g_pScene->m_vecEnemyRenderList.end())
            {
//                if(bCheck && (*itEnemy)->m_dwAttackEnemy != PK_NONE)
                if(bCheck && (*itEnemy)->IsPkEnable() && ATTACK_AVAILABLE_STATE((*itEnemy)->m_dwState))
                {
                    m_pOrderTarget = (*itEnemy);
                    break;
                }
                if((*itEnemy) == m_pOrderTarget)
                {
                    bCheck = true;
                    m_pOrderTarget = NULL;
                }
                itEnemy++;
            }
        }
        if(!bCheck || !m_pOrderTarget)
        {
            CVecMonsterIterator itMon = g_pScene->m_vecMonsterRenderList.begin();
            while(itMon != g_pScene->m_vecMonsterRenderList.end())
            {
                if(bCheck && ATTACK_AVAILABLE_STATE((*itMon)->m_dwState))
                {
                    m_pOrderTarget = (*itMon);
                    break;
                }
                if(*itMon == m_pOrderTarget)
                {
                    bCheck = true;
                    m_pOrderTarget = NULL;
                }
                itMon++;
            }
        }
        if(!bCheck || !m_pOrderTarget)
        {// 강제 락온 걸 타겟이 없을때 기존 락온을 해제시키는기능을 하자
            CVecMonsterIterator itMon = g_pScene->m_vecMonsterRenderList.begin();
            if(itMon != g_pScene->m_vecMonsterRenderList.end() && ATTACK_AVAILABLE_STATE((*itMon)->m_dwState))
            {
                if(m_pOrderTarget != *itMon)
                {
                    m_pOrderTarget = *itMon;
                    g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LOCKON_PRIMARY, g_pShuttleChild->m_vPos, FALSE);
                }
            }
        }
        fTabkeyTime = 0.5f;
    }

*/
/*    
    if(COMPARE_RACE(m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER))
    {
        if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LCONTROL) && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_F12))
        {
            if(!m_bSkillMoveIsUse)
            {
                m_bSkillMoveIsUse = TRUE;
                m_vSkillMoveVel = m_vVel;
                m_fSkillMoveTime = 0.0f;
                m_bSkillMoveType++;
                if(m_bSkillMoveType > 9)
                    m_bSkillMoveType = 0;
            }
        }
        if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_LCONTROL) && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_L))
        {// 지상 폭격 모드 전환
            if(fDeletekeyTime <= 0.0f)
            {
                m_bSecondaryAttackMode++;
                if(m_bSecondaryAttackMode == 2)
                    m_bSecondaryAttackMode = 0;
                fDeletekeyTime = 1.0f;
            }
        }
    }
*/
    // 2009-04-23 by bhsohn 에디트창 커서 이동
    //if(m_dwState != _LANDING && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_DELETE))
    if(m_dwState != _LANDING && !g_pD3dApp->m_bChatMode && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_DELETE))
    {
        if(g_pGameMain->m_pMp3Player->m_bMp3PlayListShow == FALSE)
        {
            // 2005-04-07 by jschoi - Tutorial
            if(g_pTutorial->IsTutorialMode() == FALSE)
            {
                if(fDeletekeyTime <= 0.0f)
                {
                    fDeletekeyTime = 1.0f;
                    // 2009-04-07 by bhsohn 에어 시즈시, 시점 안바뀌게 함                                        
                    //if(m_bIsAir == TRUE)
                    if((m_bIsAir == TRUE) && (_SIEGE != m_bAttackMode) && m_InfiState)    //2011-07-20 by jhahn    인피3차 시네마 중  시점변경 금지
                    {                
                        g_pD3dApp->m_pCamera->ChangeCamType();
                        g_pInterface->SaveOptionFile();
                    }
                }
            }
        }
    }

/*    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_INSERT))
    {
        if(fInsertKeyCheckTime <= 0.0f)
        {
            fInsertKeyCheckTime = 0.5f;
            m_bMoveInputType++;
            if(m_bMoveInputType == 2)
                m_bMoveInputType = 0;
            g_pSOption->InputType  = m_bMoveInputType ;
            if(g_pSOption->InputType)
             strcpy(g_pGameMain->m_pSystem->m_strSelectModeComboData[4], "액티브");
            else
             strcpy(g_pGameMain->m_pSystem->m_strSelectModeComboData[4],  "오토");
            g_pInterface->m_pGageBox->SetControlMode(m_bMoveInputType);
        }
    }
*/    
//    if(fLockonkeyTime >= 0.0f)
//        fLockonkeyTime -= fElapsedTime;
//    if( (m_bAutoLockon || 
//        g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_LCONTROL) ) && 
//        fLockonkeyTime < 0.0f )
//    {
//        if(m_pOrderTarget)
//        {
//            if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_LCONTROL) )
//            {
//                if(    m_pOrderTarget->m_dwPartType == _ENEMY &&                        // 1. 오더타겟이 Enemy였고,
//                    ((CEnemyData*)m_pOrderTarget)->GetPkState() == PK_NORMAL &&        // 2. 순수 강제 타겟이었다면.
//                    g_pShuttleChild->m_pPkNormalTimer->IsCityWar() == FALSE)        // 3. 도시 점령전중에는 보내지 않는다.
//                {    // 서버로 보냄
//                    MSG_FC_MOVE_UNLOCKON sMsg;
//                    sMsg.AttackIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
//                    sMsg.TargetIndex = ((CEnemyData*)m_pOrderTarget)->m_infoCharacter.CharacterInfo.ClientIndex;    
//                    g_pFieldWinSocket->SendMsg( T_FC_MOVE_UNLOCKON, (char*)&sMsg, sizeof(sMsg) );                
//                }
//                g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0001,COLOR_SKILL_USE);
////                m_pTarget = NULL;
//                m_pOrderTarget = NULL;
//                fLockonkeyTime = 0.5f;
//                m_pPkNormalTimer->ReleasePkDelay();
//            }
//        }
//        else if(IsPkEnableMap(m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE )
//        {
//            CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
//            if(itEnemy != g_pScene->m_mapEnemyList.end() && )
//            {
//                g_pD3dApp->m_pChat->CreateChatChild("PK 락온 작동",COLOR_SKILL_USE);
//                m_pOrderTarget = itEnemy->second;
//                fLockonkeyTime = 0.5f;
//                g_pD3dApp->m_pSound->PlayD3DSound(300012,m_vPos);
//            }
//            if(!m_pOrderTarget)
//            {
//                CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.begin();
//                while(itMonster != g_pScene->m_mapMonsterList.end())
//                {
//                    if(itMonster->second->m_dwState == _NORMAL )//&& 
//                    {
//                        g_pD3dApp->m_pChat->CreateChatChild("락온 작동",COLOR_SKILL_USE);
//                        m_pOrderTarget = itMonster->second;
//                        fLockonkeyTime = 0.5f;
//                        g_pD3dApp->m_pSound->PlayD3DSound(300012,m_vPos);
//                        break;
//                    }
//                    itMonster++;
//                }
//            }
//        }
//    }
/*
    if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_PRIOR))
    {
        if(g_pD3dApp->m_pCamera->m_bIsCamControl)
        {
            g_pD3dApp->m_pCamera->SetControlCamAngle(10);
        }
    }
    else if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_NEXT))
    {
        if(g_pD3dApp->m_pCamera->m_bIsCamControl)
        {
            g_pD3dApp->m_pCamera->SetControlCamAngle(-10);
        }
    }
    else
    {
        g_pD3dApp->m_pCamera->SetControlCamAngle(0,FALSE);
    }
*/
}
void CShuttleChild::Render()
{
    FLOG( "CShuttleChild::Render()" );
    // 2007-03-26 by dgwoo 옵저버 모드시 기어를 그리지 않는다.
    if(IsOperAndObser())
    {
        return;
    }

    // 2007-03-07 by bhsohn 시즈 모드 버그 수정    
    BOOL bCameraStatus = (g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL) && m_bAttackMode != _SIEGE;
    if(FALSE == bCameraStatus)
    {
        BOOL bCamContorl = g_pCamera->m_bIsCamControl || m_bTurnCamera;
        // 카메라 모드인지 판단. 
        if( bCamContorl && _SIEGE == m_bAttackMode )
        {
            // 시즈면서 카메라 제어모드 
            bCameraStatus = TRUE;
        }
    }    
//    if( (g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL && 
//        m_bAttackMode != 2 ) ||
//        m_bIsCameraPattern == TRUE ||
//        g_pD3dApp->m_dwGameState == _CITY)// 1인칭인 경우, 시즈모드인 경우 렌더링 안함, 도시에서는 무조건 렌더링 함
    if( bCameraStatus
        || m_bIsCameraPattern == TRUE 
        ||     g_pD3dApp->m_dwGameState == _CITY)// 1인칭인 경우, 시즈모드인 경우 렌더링 안함, 도시에서는 무조건 렌더링 함
    {
        ((CUnitRender*)m_pRender)->Render(this, m_nArmorColorIndex);
        
        // 인벤토리 렌더링
//        m_bInvenRender = FALSE;
//        if(g_pGameMain->m_pInfWindow->m_nLeftWindowInfo == 1 && m_bRenderInven == TRUE)
//        {
//            m_bInvenRender = TRUE;
//        }
    }

//    RenderMirror();
}


// 2008-08-22 by bhsohn EP3 인벤토리 처리
void CShuttleChild::RenderMirror(POINT *pMirrorPos/*=NULL*/)
{    
    g_pGameMain->RenderMirror(pMirrorPos);        
}
//{
////    if(g_pGameMain->m_pInfWindow->m_nLeftWindowInfo == 1 && m_bRenderInven == TRUE)
//    {
//        m_bInvenRender = TRUE;
////        g_pCamera->SetCamType(CAMERA_TYPE_NORMAL);
//        
//        int nAmorNum = GetUnitNum();
//        CEffectInfo* pEffectEngine = NULL;
//        char chEngineObjectNum[20] = {0,};
//
//        if(g_pD3dApp->m_bCharacter)
//        {
//            if(g_pD3dApp->m_dwGameState != _SHOP)
//            {
//                CItemInfo* pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
//                nAmorNum = GetUnitNumFromCharacter((pItem?pItem->ItemInfo->SourceIndex:0), m_myShuttleInfo.UnitKind, 0, 0, 2);
//            }
//            else if(g_pD3dApp->m_dwGameState == _SHOP)
//            {
//                CItemInfo* pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
//                if(pItem && g_pGameMain->m_pItemInfoWear[POS_CENTER] == NULL)
//                    nAmorNum = GetUnitNumFromCharacter((pItem?pItem->ItemInfo->SourceIndex:0), m_myShuttleInfo.UnitKind, 0, 0, 2);
//                else
//                    nAmorNum = GetUnitNumFromCharacter((g_pGameMain->m_pItemInfoWear[POS_CENTER]?g_pGameMain->m_pItemInfoWear[POS_CENTER]->ItemInfo->SourceIndex:0), m_myShuttleInfo.UnitKind, 0, 0, 2);            
//            }
//        
//            GUIUnitRenderInfo pInfo;
//            memset(&pInfo, 0x00, sizeof(GUIUnitRenderInfo));
//            CAppEffectData    *pEffectTemp;
//            pEffectTemp = CUnitData::CreateWearItemEffectAtSelectMenu(&pInfo, nAmorNum, BODYCON_LANDED_MASK);
//
//            // 무기 메트릭스
//            m_pMatInvenWeaponOrgPosition[0] = pEffectTemp->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_RIGHT_POSITION);
//            m_pMatInvenWeaponOrgPosition[1] = pEffectTemp->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_PRIMARY_LEFT_POSITION);
//            m_pMatInvenWeaponOrgPosition[2] = pEffectTemp->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_RIGHT_POSITION);
//            m_pMatInvenWeaponOrgPosition[3] = pEffectTemp->m_pCharacterInfo->GetEffectMatrix(BODYCON_WEAPON_POSITION_MASK, RC_EFF_SECONDARY_LEFT_POSITION);
//
//            // 엔진 오브젝트
//            CBodyConditionInfo* pBodyTemp = pEffectTemp->m_pCharacterInfo->FindBodyConditionInfo(BODYCON_LANDED_MASK);
//            if(pBodyTemp)
//            {
//                char chEffNum[20] = {0,};
//                wsprintf(chEffNum, "0%d.eff", GetInvenUseEngineEffectNum(m_myShuttleInfo.UnitKind));
//                pEffectEngine = pBodyTemp->GetEffectInfo(chEffNum);
//                if(pEffectEngine)
//                {
//                    wsprintf(chEngineObjectNum, "0%d.obj", GetInvenUseEngineObjectNum(m_myShuttleInfo.UnitKind));
//                }
//            }
//
//            if(g_pD3dApp->m_pEffectList)
//            {
//                g_pD3dApp->m_pEffectList->DeleteChild((CAtumNode*)pEffectTemp);
//            }
//        }
//        else
//        {
//            // 무기 메트릭스
//            m_pMatInvenWeaponOrgPosition[0] = m_pMatWeaponPosition[0];
//            m_pMatInvenWeaponOrgPosition[1] = m_pMatWeaponPosition[1];
//            m_pMatInvenWeaponOrgPosition[2] = m_pMatWeaponPosition[2];
//            m_pMatInvenWeaponOrgPosition[3] = m_pMatWeaponPosition[3];
//
//            // 엔진 오브젝트
//            CBodyConditionInfo* pBodyTemp = m_pCharacterInfo->FindBodyConditionInfo(BODYCON_LANDED_MASK);
//            if(pBodyTemp)
//            {
//                char chEffNum[20] = {0,};
//                wsprintf(chEffNum, "0%d.eff", GetInvenUseEngineEffectNum(m_myShuttleInfo.UnitKind));
//                pEffectEngine = pBodyTemp->GetEffectInfo(chEffNum);
//                if(pEffectEngine)
//                {
//                    wsprintf(chEngineObjectNum, "0%d.obj", GetInvenUseEngineObjectNum(m_myShuttleInfo.UnitKind));
//                }
//            }
//        }
//        
//        // 기어
//        CSkinnedMesh* pSkinnedMesh = ((CUnitRender*)m_pRender)->GetUnitMesh(nAmorNum);
//        if(pSkinnedMesh == NULL)
//            return;
//        // 엔진
//        CSkinnedMesh* pSkinnedMeshEngine = NULL;
//        if(pEffectEngine != NULL)
//            pSkinnedMeshEngine = g_pD3dApp->m_pEffectRender->LoadObject(chEngineObjectNum);
//        float fUnitScaling;
//        float fUnitPosX;
//        float fUnitPosY;
//        float fUnitPosZ;
//
//        GetResolutionPos(i_dwKind,
//            fUnitPosX,fUnitPosY,fUnitPosZ,fUnitScaling);
//        
//        fUnitScaling += m_fScaling;
//        fUnitPosX     += m_fUnitX;
//        fUnitPosY     += m_fUnitY;
//        fUnitPosZ     += m_fUnitZ;
//
//        //////////////////////////////////////////////////////////////////////////
//        //
//        D3DXMATRIX pMatOldView, pMatOldProj, pMatPresView, pMatPresProj, pMatrix;
//        D3DXMatrixIdentity(&pMatOldView);
//        D3DXMatrixIdentity(&pMatOldProj);
//        D3DXMatrixIdentity(&pMatPresView);
//        D3DXMatrixIdentity(&pMatPresProj);
//        D3DXMatrixIdentity(&pMatrix);
//        
//        D3DXMATRIX pTemp, pMatRotX, pMatRotZ, pMatScaling;
//        D3DXMatrixIdentity(&pTemp);
//        D3DXMatrixIdentity(&pMatRotX);
//        D3DXMatrixIdentity(&pMatRotZ);
//        D3DXMatrixIdentity(&pMatScaling);
//        
//        // 현재 뷰와 프로젝션을 가져온다
//        g_pD3dDev->GetTransform( D3DTS_VIEW,       &pMatOldView );
//        g_pD3dDev->GetTransform( D3DTS_PROJECTION, &pMatOldProj );    
//        
//        D3DXMatrixScaling(&pMatScaling, fUnitScaling, fUnitScaling+0.002f, fUnitScaling);
//        D3DXMatrixTranslation(&pTemp, fUnitPosX, fUnitPosY, fUnitPosZ);
//        D3DXMatrixRotationX(&pMatRotX, m_fRotationX);
//        D3DXMatrixRotationY(&pMatRotZ, m_fRotationZ);
//        pMatrix = pMatRotZ*pMatRotX*pTemp*pMatScaling;
//
//        // 2006-01-16 by ispark, 무기
//        m_pMatInvenWeaponSetPosition[0] = m_pMatInvenWeaponOrgPosition[0]*pMatrix;
//        m_pMatInvenWeaponSetPosition[1] = m_pMatInvenWeaponOrgPosition[1]*pMatrix;
//        m_pMatInvenWeaponSetPosition[2] = m_pMatInvenWeaponOrgPosition[2]*pMatrix;
//        m_pMatInvenWeaponSetPosition[3] = m_pMatInvenWeaponOrgPosition[3]*pMatrix;
//        
//        g_pD3dDev->SetTransform( D3DTS_VIEW,        &pMatPresView);
//        g_pD3dDev->SetTransform( D3DTS_PROJECTION,    &pMatPresProj);    
//        
//        // 2008-01-21 by bhsohn 창고에서 아머색이 안바뀌는 버그 수정
//        GUI_BUILDINGNPC* pCurrentBuildingNpc = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
//
//        // 기체
//        // 2006-02-17 by ispark
//        if(g_pD3dApp->m_dwGameState != _SHOP 
//            || (pCurrentBuildingNpc && IS_STORE_SHOP_TYPE(pCurrentBuildingNpc->buildingInfo.BuildingKind)))// 2008-01-21 by bhsohn 창고에서 아머색이 안바뀌는 버그 수정
//        {
//            m_nInvenArmorColorIndex = m_nArmorColorIndex;
//        }
//        pSkinnedMesh->m_unTexSelectColor = m_nInvenArmorColorIndex;
//        pSkinnedMesh->SetWorldMatrix(pMatrix);
//        pSkinnedMesh->AnotherTexture(1);
//        pSkinnedMesh->Render(FALSE, _SHUTTLE);
////      pSkinnedMesh->SetDecalCount(0);
//        m_pMatInven = pMatrix;
//
//        // 엔진
//        if(pSkinnedMeshEngine != NULL)
//        {
//            pSkinnedMeshEngine->SetWorldMatrix(pMatrix);
//            pSkinnedMeshEngine->AnotherTexture(1);
//            pSkinnedMeshEngine->Render();
//        }
//
//        // 2007-11-09 by dgwoo 무기 메쉬를 배열에서 벡터로 변경.
//        for(int i = 0; i < m_vectInvenWeaponMesh.size(); i++)
//        {
//            if(m_vectInvenWeaponMesh[i].pInvenWeaponMesh)
//            {        
//                DWORD dwSrc,dwDest,dwColorOp;
//                DWORD dwFogValue = FALSE;    
//                DWORD dwLightColorOp = 0;                    
//                CEffectInfo* pEffectInfo = GetEffectWeapon(i);
//                
//                if(pEffectInfo     && (EFFECT_TYPE_OBJECT == pEffectInfo->m_nEffectType))// 오브젝트만 랜더
//                {
//                    CObjectAni* pEffect = (CObjectAni*)pEffectInfo->m_pEffect;
//                    
//                    //06800370                                
//                    {                    
//                        g_pD3dDev->GetRenderState(D3DRS_SRCBLEND,&dwSrc);
//                        g_pD3dDev->GetRenderState(D3DRS_DESTBLEND,&dwDest);
//                        g_pD3dDev->GetTextureStageState(0,D3DTSS_COLOROP,&dwColorOp);
//                        
//                        g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,   pEffect->m_bAlphaTestEnble);//pEffect->m_bAlphaTestEnble
//                        g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
//                        
//                        g_pD3dDev->SetRenderState( D3DRS_ZENABLE, pEffect->m_bZbufferEnable);
//                        if( pEffect->m_bZWriteEnable == FALSE)
//                        {
//                            g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, pEffect->m_bZWriteEnable );
//                        }
//                        g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  pEffect->m_bAlphaBlending );
//                        if(pEffect->m_bAlphaBlending)
//                        {
//                            g_pD3dDev->GetRenderState( D3DRS_FOGENABLE,  &dwFogValue );
//                            
//                            g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,pEffect->m_nSrcBlend);
//                            g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,pEffect->m_nDestBlend);
//                        }
//                        g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,pEffect->m_nTextureRenderState);
//                        g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );                    
//                        
//                        if(pEffect->m_bLightMapUse)
//                        {
//                            g_pD3dDev->GetTextureStageState( 1, D3DTSS_COLOROP, &dwLightColorOp );
//                            g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP, pEffect->m_nLightMapRenderState );
//                            g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );
//                            
//                            g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//                            g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
//                        }
//                        
//                    }                    
//                }            
//                m_vectInvenWeaponMesh[i].pInvenWeaponMesh->SetWorldMatrix(m_pMatInvenWeaponSetPosition[m_vectInvenWeaponMesh[i].nMatIndex]);
//                m_vectInvenWeaponMesh[i].pInvenWeaponMesh->Render();
//                if(pEffectInfo     && (EFFECT_TYPE_OBJECT == pEffectInfo->m_nEffectType))// 오브젝트만 랜더
//                {
//                    g_pD3dDev->SetRenderState(D3DRS_SRCBLEND,dwSrc);
//                    g_pD3dDev->SetRenderState(D3DRS_DESTBLEND,dwDest);
//                    g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,dwColorOp);
//
//                    CObjectAni* pEffect = (CObjectAni*)pEffectInfo->m_pEffect;
//                    
//                    //g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
//                    if(!pEffect->m_bUseEnvironmentLight)
//                    {
//                        g_pD3dDev->LightEnable( 2, FALSE );
//                    }
//                    if(pEffect->m_bLightMapUse)
//                    {
//                        g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP, dwLightColorOp );
//                    }
//                    if(pEffect->m_nSrcBlend == D3DBLEND_ONE && pEffect->m_nDestBlend == D3DBLEND_ONE && dwFogValue == TRUE)
//                    {
//                        g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  dwFogValue );
//                    }
//                    if(pEffect->m_bAlphaTestEnble)
//                    {
//                        g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
//                    }
//                    if( pEffect->m_bZWriteEnable == FALSE)
//                    {
//                        g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
//                    }
//                }                
//            }
//          
//            // 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
//            // 파티클 랜더            
//            //if(m_stInvenWeaponMesh[i].pInvenParticle)
//            {                
//                //g_pD3dApp->m_pEffectRender->ParticleInvenRender();
//                
//                g_pD3dApp->m_pEffectRender->RenderParticleInvenVector(m_vectInvenWeaponMesh[i].nMatIndex, 
//                        pMatrix, 
//                        m_pMatInvenWeaponSetPosition[m_vectInvenWeaponMesh[i].nMatIndex], 
//                        fUnitScaling);
//                
//            }            
//        }
//        // 랜더링 버퍼 초기화
//        g_pD3dApp->m_pEffectRender->ResetContentInvneParticle();
//
//        // 복구
//        g_pD3dDev->SetTransform( D3DTS_VIEW,        &pMatOldView );
//        g_pD3dDev->SetTransform( D3DTS_PROJECTION,    &pMatOldProj );
//        g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
//    }
//}
// end 2008-08-22 by bhsohn EP3 인벤토리 처리


void CShuttleChild::CheckShadowPos()
{
    FLOG( "CShuttleChild::CheckShadowPos()" );
    D3DXVECTOR3 vTempShadowPos[4];
    if( IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE &&
        m_bOnObject == FALSE)
    {
        m_bShadowIsRender = FALSE;
        return;
    }
    else
    {
        m_bShadowIsRender = TRUE;
    }
//    if( g_pSOption->sShadowState == 0 )
//    {
//        return;
//    }
    D3DXVECTOR3 vVel, vSideVel;
    D3DXMATRIX matTemp;
    if(IS_DT(m_myShuttleInfo.UnitKind) && !m_bIsAir)
    {
        vVel = m_vVel;
    }
    else
    {
        D3DXMatrixRotationAxis( &matTemp, &m_vSideVel, m_fAxisRate);
        D3DXVec3TransformCoord( &vVel, &m_vVel, &matTemp );
    }
    if(m_vUp.y < 0)
    {
        vSideVel = -m_vSideVel;
    }
    else
    {
        vSideVel = m_vSideVel;
    }
    m_vShadowPos1 = m_vPos+m_fObjectSize*(vVel-vSideVel);
    m_vShadowPos2 = m_vPos+m_fObjectSize*(vVel+vSideVel);
    m_vShadowPos3 = m_vPos-m_fObjectSize*(vVel-vSideVel);
    m_vShadowPos4 = m_vPos-m_fObjectSize*(vVel+vSideVel);

    FLOAT fDist;
    int i,j;
    i = (int)(m_vShadowPos1.x/TILE_SIZE);
    j = (int)(m_vShadowPos1.z/TILE_SIZE);
    D3DXVECTOR3 v,orig,dir,vPos;//,v2;
    D3DXVECTOR3 vSide;
    orig = m_vShadowPos1;
    dir = D3DXVECTOR3(0,-1,0);
    vSide = D3DXVECTOR3(0,0,1);
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
    {
        if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            m_vShadowPos1 = orig + dir*fDist;
            if(CheckIsWater(orig))
            {
                if(m_vShadowPos1.y < g_pGround->m_projectInfo.fWaterHeight)
                    m_vShadowPos1.y = g_pGround->m_projectInfo.fWaterHeight;
            }
            vTempShadowPos[0] = m_vShadowPos1;
            float fTempHeight = DEFAULT_COLLISION_DISTANCE;// = 100.0f;
            if(m_bOnObject)
            {
                D3DXMatrixLookAtLH(&matTemp,&orig,&(orig + dir),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
                if(m_vShadowPos1.y < orig.y - fTempHeight)
                    m_vShadowPos1.y = orig.y - fTempHeight;
                else if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
                    m_bShadowIsRender = FALSE;
            }
        }
        else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            m_vShadowPos1 = orig + dir*fDist;
            if(CheckIsWater(orig))
            {
                if(m_vShadowPos1.y < g_pGround->m_projectInfo.fWaterHeight)
                    m_vShadowPos1.y = g_pGround->m_projectInfo.fWaterHeight;
            }
            vTempShadowPos[0] = m_vShadowPos1;
            float fTempHeight = DEFAULT_COLLISION_DISTANCE;// = 100.0f;
            if(m_bOnObject && 
                g_pScene &&
                g_pScene->m_pObjectRender)
            {
                D3DXMatrixLookAtLH(&matTemp,&orig,&(orig + dir),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
                if(m_vShadowPos1.y < orig.y - fTempHeight)
                    m_vShadowPos1.y = orig.y - fTempHeight;
                else if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
                    m_bShadowIsRender = FALSE;
            }
        }
    }
    i = (int)(m_vShadowPos2.x/TILE_SIZE);
    j = (int)(m_vShadowPos2.z/TILE_SIZE);
    orig = m_vShadowPos2;
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
    {
        vPos = m_vShadowPos2;
        if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            m_vShadowPos2 = orig + dir*fDist;
            if(CheckIsWater(orig))
            {
                if(m_vShadowPos2.y < g_pGround->m_projectInfo.fWaterHeight)
                    m_vShadowPos2.y = g_pGround->m_projectInfo.fWaterHeight;
            }
            vTempShadowPos[1] = m_vShadowPos2;
            float fTempHeight = DEFAULT_COLLISION_DISTANCE;// = 100.0f;
            if(m_bOnObject && g_pScene->m_pObjectRender)
            {
                D3DXMatrixLookAtLH(&matTemp,&orig,&(orig + dir),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
                if(m_vShadowPos2.y < orig.y - fTempHeight)
                    m_vShadowPos2.y = orig.y - fTempHeight;
                else if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
                    m_bShadowIsRender = FALSE;
            }
        }
        else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            m_vShadowPos2 = orig + dir*fDist;
            if(CheckIsWater(orig))
            {
                if(m_vShadowPos2.y < g_pGround->m_projectInfo.fWaterHeight)
                    m_vShadowPos2.y = g_pGround->m_projectInfo.fWaterHeight;
            }
            vTempShadowPos[1] = m_vShadowPos2;
            float fTempHeight = DEFAULT_COLLISION_DISTANCE;// = 100.0f;
            if(m_bOnObject && g_pScene->m_pObjectRender)
            {
                D3DXMatrixLookAtLH(&matTemp,&orig,&(orig + dir),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
                if(m_vShadowPos2.y < orig.y - fTempHeight)
                    m_vShadowPos2.y = orig.y - fTempHeight;
                else if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
                    m_bShadowIsRender = FALSE;
            }
        }
    }
    i = (int)(m_vShadowPos3.x/TILE_SIZE);
    j = (int)(m_vShadowPos3.z/TILE_SIZE);
    orig = m_vShadowPos3;
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
    {
        vPos = m_vShadowPos3;
        if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            m_vShadowPos3 = orig + dir*fDist;
            if(CheckIsWater(orig))
            {
                if(m_vShadowPos3.y < g_pGround->m_projectInfo.fWaterHeight)
                    m_vShadowPos3.y = g_pGround->m_projectInfo.fWaterHeight;
            }
            vTempShadowPos[2] = m_vShadowPos3;
            float fTempHeight = DEFAULT_COLLISION_DISTANCE;// = 100.0f;
            if(m_bOnObject && g_pScene->m_pObjectRender)
            {
                D3DXMatrixLookAtLH(&matTemp,&orig,&(orig + dir),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
                if(m_vShadowPos3.y < orig.y - fTempHeight)
                    m_vShadowPos3.y = orig.y - fTempHeight;
                else if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
                    m_bShadowIsRender = FALSE;
            }
        }
        else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            m_vShadowPos3 = orig + dir*fDist;
            if(CheckIsWater(orig))
            {
                if(m_vShadowPos3.y < g_pGround->m_projectInfo.fWaterHeight)
                    m_vShadowPos3.y = g_pGround->m_projectInfo.fWaterHeight;
            }
            vTempShadowPos[2] = m_vShadowPos3;
            float fTempHeight = DEFAULT_COLLISION_DISTANCE;// = 100.0f;
            if(m_bOnObject && g_pScene->m_pObjectRender)
            {
                D3DXMatrixLookAtLH(&matTemp,&orig,&(orig + dir),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
                if(m_vShadowPos3.y < orig.y - fTempHeight)
                    m_vShadowPos3.y = orig.y - fTempHeight;
                else if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
                    m_bShadowIsRender = FALSE;
            }
        }
    }
    i = (int)(m_vShadowPos4.x/TILE_SIZE);
    j = (int)(m_vShadowPos4.z/TILE_SIZE);
    orig = m_vShadowPos4;
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
    {
        vPos = m_vShadowPos4;
        if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            m_vShadowPos4 = orig + dir*fDist;
            if(CheckIsWater(orig))
            {
                if(m_vShadowPos4.y < g_pGround->m_projectInfo.fWaterHeight)
                    m_vShadowPos4.y = g_pGround->m_projectInfo.fWaterHeight;
            }
            vTempShadowPos[3] = m_vShadowPos4;
            float fTempHeight = DEFAULT_COLLISION_DISTANCE;// = 100.0f;
            if(m_bOnObject && g_pScene->m_pObjectRender)
            {
                D3DXMatrixLookAtLH(&matTemp,&orig,&(orig + dir),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
                if(m_vShadowPos4.y < orig.y - fTempHeight)
                    m_vShadowPos4.y = orig.y - fTempHeight;
                else if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
                    m_bShadowIsRender = FALSE;
            }
        }
        else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            m_vShadowPos4 = orig + dir*fDist;
            if(CheckIsWater(orig))
            {
                if(m_vShadowPos4.y < g_pGround->m_projectInfo.fWaterHeight)
                    m_vShadowPos4.y = g_pGround->m_projectInfo.fWaterHeight;
            }
            vTempShadowPos[3] = m_vShadowPos4;
            float fTempHeight = DEFAULT_COLLISION_DISTANCE;// = 100.0f;
            if(m_bOnObject && g_pScene->m_pObjectRender)
            {
                D3DXMatrixLookAtLH(&matTemp,&orig,&(orig + dir),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
                if(m_vShadowPos4.y < orig.y - fTempHeight)
                    m_vShadowPos4.y = orig.y - fTempHeight;
                else if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
                    m_bShadowIsRender = FALSE;
            }
        }
    }
    if(m_bOnObject && g_pScene->m_pObjectRender &&
        (D3DXVec3Length(&(m_vShadowPos4-m_vShadowPos2)) > m_fObjectSize*6 ||
        D3DXVec3Length(&(m_vShadowPos1-m_vShadowPos3)) > m_fObjectSize*6))
    {// 그림자가 오프젝트에 걸쳐 늘어지면 지형과 물을 기준으로 그림자 위치를 세팅한다.
        m_vShadowPos1 = vTempShadowPos[0];
        m_vShadowPos2 = vTempShadowPos[1];
        m_vShadowPos3 = vTempShadowPos[2];
        m_vShadowPos4 = vTempShadowPos[3];
    }
    SPRITEVERTEX* pV;
    m_pVBShadow->Lock( 0, 0, (void**)&pV,    0 );
    pV[0].p = m_vShadowPos4; 
    pV[1].p = m_vShadowPos1; 
    pV[2].p = m_vShadowPos3; 
    pV[3].p = m_vShadowPos2;
    m_pVBShadow->Unlock();
}

FLOAT CShuttleChild::CheckMap(D3DXVECTOR3 vPos)
{
    FLOG( "CShuttleChild::CheckMap(D3DXVECTOR3 vPos)" );
    // 2007-04-19 by bhsohn 충돌 처리 문제
    //if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
    if(IsTileMapTickEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
    {
        if(vPos.y <= g_pGround->m_projectInfo.fWaterHeight + 1.0f)
        {
            m_bCollMap = TRUE;
        }
        return g_pGround->m_projectInfo.fWaterHeight;
    }
    FLOAT fDist,fMaxPoint;
    int i,j;
    i = (int)(vPos.x/TILE_SIZE);
    j = (int)(vPos.z/TILE_SIZE);
    D3DXVECTOR3 v,dir;
    dir = D3DXVECTOR3(0,-1,0);
    m_bIsWater = CheckIsWater(vPos);
    // 2005-09-14 by ispark
    // 만약에 외곽이라면 안쪽 지형으로 검사
    if((i < 0 || i > g_pGround->m_projectInfo.sXSize) || (j < 0 || j > g_pGround->m_projectInfo.sYSize))
    {
        if(i < 0)
        {
            vPos.x = 0;
            i = 0;
        }
        else if(i > g_pGround->m_projectInfo.sXSize)
        {
            i = g_pGround->m_projectInfo.sXSize - 1;
            vPos.x = i * TILE_SIZE;
        }
        if(j < 0)
        {
            vPos.z = 0;
            j = 0;
        }
        else if(j > g_pGround->m_projectInfo.sYSize)
        {
            j = g_pGround->m_projectInfo.sYSize - 1;
            vPos.z = j * TILE_SIZE;
        }
    }
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
    {
        if(g_pGround->IntersectTriangle(vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            // 세점중 최고 높은점을 저장한다.
            fMaxPoint = g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos.y;
            if(fMaxPoint < g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos.y)
                fMaxPoint = g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos.y;
            if(fMaxPoint < g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos.y)
                fMaxPoint = g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos.y;
            // 지형의 노멀벡터를 구한다.
            D3DXVec3Cross(&m_vNormal,&(g_pGround->m_pTileVertexArray[(i)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos
                -g_pGround->m_pTileVertexArray[(i)*(g_pGround->m_projectInfo.sYSize+1)+j].pos),
                &(g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos
                -g_pGround->m_pTileVertexArray[(i)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos));
            D3DXVec3Normalize(&m_vNormal,&m_vNormal);
            v = vPos + dir*fDist;
            SetMoveType(m_vNormal,fMaxPoint);
            if(m_bIsWater)
            {
                if(v.y > g_pGround->m_projectInfo.fWaterHeight)
                {
                    if(vPos.y <= v.y + 1.0f)
                        m_bCollMap = TRUE;
                    return v.y;
                }
                else
                {
                    m_bCheckMove = 3;
                    if(vPos.y <= g_pGround->m_projectInfo.fWaterHeight + 1.0f)
                    {
                        m_bCollMap = TRUE;
                    }
                    return g_pGround->m_projectInfo.fWaterHeight;
                }
            }
            else
            {
                if(vPos.y <= v.y + 1.0f)
                    m_bCollMap = TRUE;
                return v.y;
            }
        }
        else if(g_pGround->IntersectTriangle(vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            // 세점중 최고 높은점을 저장한다.
            fMaxPoint = g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos.y;
            if(fMaxPoint < g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos.y)
                fMaxPoint = g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos.y;
            if(fMaxPoint < g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos.y)
                fMaxPoint = g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos.y;
            // 지형의 노멀벡터를 구한다.
            D3DXVec3Cross(&m_vNormal,&(g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos
                -g_pGround->m_pTileVertexArray[(i)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos),
                &(g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos
                -g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos));
            D3DXVec3Normalize(&m_vNormal,&m_vNormal);
            v = vPos + dir*fDist;
            SetMoveType(m_vNormal,fMaxPoint);
            if(m_bIsWater)
            {
                if(v.y > g_pGround->m_projectInfo.fWaterHeight)
                {
                    if(vPos.y <= v.y + 1.0f)
                        m_bCollMap = TRUE;
                    return v.y;
                }
                else
                {
                    m_bCheckMove = 3;
                    if(vPos.y <= g_pGround->m_projectInfo.fWaterHeight + 1.0f)
                    {
                        m_bCollMap = TRUE;
                    }
                    return g_pGround->m_projectInfo.fWaterHeight;
                }
            }
            else
            {
                if(vPos.y <= v.y + 1.0f)
                    m_bCollMap = TRUE;
                return v.y;
            }
        }
    }
    return 0.0f;
}

FLOAT CShuttleChild::CheckMapIsColl(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel,int cont)
{
    FLOG( "CShuttleChild::CheckMapIsColl(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel,int cont)" );
    if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
    {
        return vPos.y - g_pGround->m_projectInfo.fWaterHeight;
    }
    FLOAT fDist;
    D3DXVECTOR3 vTempPos;
    int i,j;
    for(int k=0;k<cont;k++)
    {
        vTempPos = vPos + TILE_SIZE*vVel*k;
        i = (int)(vTempPos.x/TILE_SIZE);
        j = (int)(vTempPos.z/TILE_SIZE);
        if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
        {
            if(g_pGround->IntersectTriangle(vPos,vVel,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
                g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
                g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
                &fDist))//, &fBary1, &fBary2 ))
            {
                return fDist;
            }
            if(g_pGround->IntersectTriangle(vPos,vVel,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
                g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
                g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
                &fDist))//, &fBary1, &fBary2 ))
            {
                return fDist;
            }
        }
    }
    return 0.0f;
}

BOOL CShuttleChild::CheckMapIsColl(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel)
{
    FLOG( "CShuttleChild::CheckMapIsColl(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel)" );
    if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
    {
        return TRUE;
    }
    FLOAT fDist,fMaxPoint;
    int i,j;
    i = (int)(vPos.x/TILE_SIZE);
    j = (int)(vPos.z/TILE_SIZE);
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
    {
        if(g_pGround->IntersectTriangle(vPos,vVel,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            // 세점중 최고 높은점을 저장한다.
            fMaxPoint = g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos.y;
            if(fMaxPoint < g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos.y)
                fMaxPoint = g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos.y;
            if(fMaxPoint < g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos.y)
                fMaxPoint = g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos.y;
            // 지형의 노멀벡터를 구한다.
            D3DXVec3Cross(&m_vNormal,&(g_pGround->m_pTileVertexArray[(i)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos
                -g_pGround->m_pTileVertexArray[(i)*(g_pGround->m_projectInfo.sYSize+1)+j].pos),
                &(g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos
                -g_pGround->m_pTileVertexArray[(i)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos));
            D3DXVec3Normalize(&m_vNormal,&m_vNormal);
            if(m_vNormal.y > 0.5f) // 경사 체크하여 상승 할것인지 하지 않을것인지? TRUE:상승 FALSE:상승안함
                return TRUE;
            else
                return FALSE;
        }
        if(g_pGround->IntersectTriangle(vPos,vVel,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            // 세점중 최고 높은점을 저장한다.
            fMaxPoint = g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos.y;
            if(fMaxPoint < g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos.y)
                fMaxPoint = g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos.y;
            if(fMaxPoint < g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos.y)
                fMaxPoint = g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos.y;
            // 지형의 노멀벡터를 구한다.
            D3DXVec3Cross(&m_vNormal,&(g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos
                -g_pGround->m_pTileVertexArray[(i)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos),
                &(g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos
                -g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos));
            D3DXVec3Normalize(&m_vNormal,&m_vNormal);
            if(m_vNormal.y > 0.5f)
                return TRUE;
            else
                return FALSE;
        }
    }
    return FALSE;
}

void CShuttleChild::SetMoveType(D3DXVECTOR3 vPos,float fHeight)
{
    FLOG( "CShuttleChild::SetMoveType(D3DXVECTOR3 vPos,float fHeight)" );
    if(IS_DT(m_myShuttleInfo.UnitKind) && !m_bIsAir)
    {
        if(vPos.y > 0.9f) // - ?
            m_bCheckMove = 3;
        else if(vPos.y > 0.866f) // - 30
            m_bCheckMove = 2;
        else if(vPos.y > 0.7f) // 30-45
            m_bCheckMove = 1;
        else // 45도 이상
            m_bCheckMove = 0;
    }
    else
    {
        if(vPos.y > 0.866f)
            m_bCheckMove = 3; // 30도 미만
        else if(vPos.y > 0.7f) //30-45
            m_bCheckMove = 2;
        else if(vPos.y > 0.5f) // 45-60
            m_bCheckMove = 1;
        else // 60도 이상
        {
            if(fHeight - m_vPos.y < 20.0f)
                m_bCheckMove = 1;    // 경사가 급해도 작은 언덕이라면 넘어가자
            else
                m_bCheckMove = 0;
        }
    }
}

void CShuttleChild::ResortingItem()
{
    FLOG( "CShuttleChild::ResortingItem()" );
    if(m_pStoreData)
    {
        m_pStoreData->ResortingItemInInventory();
    }
}

BOOL CShuttleChild::CheckIsWater(D3DXVECTOR3 vPos)
{
    FLOG( "CShuttleChild::CheckIsWater(D3DXVECTOR3 vPos)" );

    // 2005-02-11 by jschoi 물 오브젝트 먼저 검사
    D3DXMATRIX mat;
    D3DXVECTOR3 vSide(0,0,1),vUp(0,1,0);
    D3DXMatrixLookAtLH(&mat,&m_vPos,&(m_vPos - m_vUp),&vSide);        // 아래
        
    if(    g_pScene &&
        g_pScene->m_pObjectRender)
    {    
        COLLISION_RESULT collResult = g_pScene->m_pObjectRender->CheckCollMeshWaterObject(mat,m_vPos);
        if(collResult.fDist != DEFAULT_COLLISION_DISTANCE)
        {
            return TRUE;
        }
    }

    int i, j;
    i = (int)(vPos.x / TILE_SIZE);
    j = (int)(vPos.z / TILE_SIZE);
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
    {
        int k = (i*g_pGround->m_projectInfo.sYSize+j);

        // 2005-04-06 by jschoi
//        return g_pGround->m_pTileInfo[k].useWater;
        return g_pGround->m_pTileInfo[k].dwWaterType == 1 ? TRUE : FALSE ;
    }

    return FALSE;
}

//void CShuttleChild::UpdateEngine(int nEngineType)
//{
//    FLOG( "CShuttleChild::UpdateEngine(int nEngineType)" );
//    ITEM * item = g_pDatabase->GetServerItemInfo(nEngineType);
//    if(item)
//    {
//        m_fShuttleSpeed = item->AbilityMin;
//        m_fShuttleSpeedMax = item->AbilityMax;
//        m_fShuttleSpeedBoosterOn = (float)item->Range;
//        m_fShuttleSpeedMin = item->AbilityMin;
//        m_fBURN = (float)item->Time/1000.0f;
//        m_fMouseRate = item->RangeAngle;
//        ASSERT_ASSERT(m_fBURN>0.0f);
//        if(g_pGameMain->m_pUnitInfoBar)
//        {
//            g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
//            g_pGameMain->m_pUnitInfoBar->SetSpeed(m_fShuttleSpeed, item->AbilityMin, (float)item->Range);
//        }
//
//    }
//}

//void CShuttleChild::ChangeEngine(int nEngineType, int nCurrentBURN)
//{
//    FLOG( "CShuttleChild::ChangeEngine(int nEngineType, int nCurrentBURN)" );
//    if(m_pEngine)
//    {
//        m_pEngine->m_bUsing = FALSE;
//        m_pEngine = NULL;
//    }
//    if(nEngineType && g_pD3dApp->m_pEffectList)
//    {
//        D3DXVECTOR3 vTempPos = D3DXVECTOR3(0,0,0);
//        m_pEngine = new CAppEffectData(this,nEngineType,vTempPos);
//        m_pEngine = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(m_pEngine);
//    }
//
//    ITEM * item = g_pDatabase->GetServerItemInfo(nEngineType);
//    if(item)
//    {
//        m_fShuttleSpeed = item->AbilityMin;
//        m_fShuttleSpeedMax = item->AbilityMax;
//        m_fShuttleSpeedBoosterOn = (float)item->Range;
//        m_fShuttleSpeedMin = item->AbilityMin;
//        m_fBURN = (float)item->Time/1000.0f;
//        m_fCurrentBURN = nCurrentBURN;
//        m_fMouseRate = item->RangeAngle;
//    }
//    else
//    {
//        m_fShuttleSpeed = 0.0f;
//        m_fShuttleSpeedMax = 10.0f;
//        m_fShuttleSpeedBoosterOn = 10.0f;
//        m_fShuttleSpeedMin = 0.0f;
//        m_fBURN = 1;
//        m_fCurrentBURN = 1;
//        m_fMouseRate = 0.5f;
//    }
//    ASSERT_ASSERT(m_fBURN>0.0f);
//    if(g_pGameMain && g_pGameMain->m_pUnitInfoBar)
//    {
//        g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
//        g_pGameMain->m_pUnitInfoBar->SetSpeed(m_fShuttleSpeed, m_fShuttleSpeedMin, m_fShuttleSpeedBoosterOn);
//    }
//}

void CShuttleChild::UpdateEngine(CItemInfo* pItemInfo)
{
    FLOG( "CShuttleChild::UpdateEngine(int nEngineType)" );
    ASSERT_ASSERT(pItemInfo);
    ITEM* pITEM = pItemInfo->GetRealItemInfo();
    m_fShuttleSpeed = pITEM->AbilityMin;
    m_fShuttleSpeedMax = pITEM->AbilityMax;
    m_fShuttleSpeedBoosterOn = (float)pITEM->Range;
    m_fShuttleSpeedMin = pITEM->AbilityMin;
    m_bySpeedPenalty = pITEM->SpeedPenalty;                    // 2005-08-16 by ispark
    m_fBURN = (float)pITEM->Time/1000.0f;

    // 2008-09-26 by bhsohn 신규 인첸트 처리
    // 최소 속도
    {        
        float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_MIN_SPEED_UP);
        m_fShuttleSpeedMin += fTime;

    }
    // 최대 속도
    {        
        float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_MAX_SPEED_UP);
        m_fShuttleSpeedMax += fTime;        
    }

    // 지상 속도
    {        
        float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_GROUND_SPEED_UP);
        m_fShuttleGroundSpeedMax += fTime;        
    }

    // 부스터 속도
    {        
        float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_BOOSTER_SPEED_UP);
        m_fShuttleSpeedBoosterOn += fTime;        
    }
    // 부스터 추가 시간 
    float fTimeUp = GetEnchantDesParam(pItemInfo, DES_ENGINE_BOOSTER_TIME_UP);
    m_fBURN += fTimeUp;

    // 2008-03-19 by bhsohn FLOAT형 재정렬 하여 사용
    m_fBURN = FloatRangeDecimal(m_fBURN, 2);

//    m_fMouseRate = pITEM->RangeAngle;
    CParamFactor*    pRefEnchant = pItemInfo->GetEnchantParamFactor();    
    if(pRefEnchant)
    {
        m_paramFactor.pfm_ENGINE_ANGLE_UP = pRefEnchant->pfm_ENGINE_ANGLE_UP;
        m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = pRefEnchant->pfm_ENGINE_BOOSTERANGLE_UP;
    }
    else
    {
        // 초기화        
        m_paramFactor.pfm_ENGINE_ANGLE_UP = m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = 0.0f;
    }

    m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pITEM, &m_paramFactor);
    ASSERT_ASSERT(m_fBURN>0.0f);
    if(g_pGameMain->m_pUnitInfoBar)
    {
        g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
        g_pGameMain->m_pUnitInfoBar->SetSpeed(m_fShuttleSpeed, m_fShuttleSpeedMin, m_fShuttleSpeedBoosterOn);
    }

}

void CShuttleChild::ChangeEngine(CItemInfo* pItemInfo, int nCurrentBURN)
{
    FLOG( "CShuttleChild::ChangeEngine(int nEngineType, int nCurrentBURN)" );
    if(pItemInfo == NULL)
    {
        m_fShuttleSpeed = 0.0f;
        m_fShuttleSpeedMax = 10.0f;
        m_fShuttleSpeedBoosterOn = 10.0f;
        m_fShuttleSpeedMin = 0.0f;
        m_bySpeedPenalty = 0;                            // 2005-08-16 by ispark
        m_fBURN = 1;
        m_fCurrentBURN = 1;
        m_fMouseRate = 0.5f;
        return;
    }
    if(m_pEngine)
    {
        m_pEngine->m_bUsing = FALSE;
        m_pEngine = NULL;
    }
    D3DXVECTOR3 vTempPos = D3DXVECTOR3(0,0,0);
//    m_pEngine = new CAppEffectData(this,pItemInfo->ItemNum,vTempPos);                // 2005-08-23 by ispark
    // 2005-10-25 by ispark
    if(m_dwPartType != _ADMIN)
    {
        m_pEngine = new CAppEffectData(this,pItemInfo->ItemInfo->SourceIndex,vTempPos, ENGINE_POSITION);
        m_pEngine = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(m_pEngine);
    }

    ITEM* pITEM = pItemInfo->GetRealItemInfo();

// 2010. 04. 15 by jskim 부스터 최소 속도, 최고 속도 2배 되는 버그 수정
//     // 2010. 03. 25 by jskim Speed, Angle 추가 계산
// 
// //     // 2004-11-04 by jschoi
// //     // 2005-11-03 by ispark
// //     // 정지 상태에서 엔진을 교환할 때 앞으로 이동하지 않도록 한다.
// //     if(m_fShuttleSpeed != 0.0f)
// //     {
// // //        m_fShuttleSpeed = pITEM->AbilityMin;
// //         m_fShuttleSpeed = 0.0f;
// //     }
// //     m_fShuttleSpeedMax = pITEM->AbilityMax;
// //     m_fShuttleSpeedBoosterOn = (float)pITEM->Range;
// //     m_fShuttleSpeedMin = pITEM->AbilityMin;
// //     m_fShuttleGroundSpeedMax = pITEM->Charging;            // 2005-11-26 by ispark
// //     m_bySpeedPenalty = pITEM->SpeedPenalty;                // 2005-08-16 by ispark
// //     m_fBURN = (float)pITEM->Time/1000.0f;
// // 
// //     // 2008-09-26 by bhsohn 신규 인첸트 처리
// //     // 최소 속도
// //     {        
// //         float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_MIN_SPEED_UP);
// //         m_fShuttleSpeedMin += fTime;
// // 
// //     }
// //     // 최대 속도
// //     {        
// //         float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_MAX_SPEED_UP);
// //         m_fShuttleSpeedMax += fTime;        
// //     }
// // 
// //     // 지상 속도
// //     {        
// //         float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_GROUND_SPEED_UP);
// //         m_fShuttleGroundSpeedMax += fTime;        
// //     }
// // 
// //     // 부스터 속도
// //     {        
// //         float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_BOOSTER_SPEED_UP);
// //         m_fShuttleSpeedBoosterOn += fTime;        
// //     }
// //     // 부스터 추가 시간 
// //     float fTimeUp = GetEnchantDesParam(pItemInfo, DES_ENGINE_BOOSTER_TIME_UP);
// //     m_fBURN += fTimeUp;
// //     
// //     // 2008-03-19 by bhsohn FLOAT형 재정렬 하여 사용
// //     m_fBURN = FloatRangeDecimal(m_fBURN, 2);
// // 
// //     //DbgOut("#1 m_fCurrentBURN[%.2f] nCurrentBURN[%d] m_fBURN[%.2f] \n", m_fCurrentBURN, nCurrentBURN, m_fBURN);
// //     if((m_fCurrentBURN == 0) ||                            // 2005-12-10 by ispark, 현재 부스터 게이지가 0이라면 셋팅. 자주 리셋 되는 경우 때문에 이렇게 함
// //         (m_fCurrentBURN > m_fBURN))                        // 2005-12-15 by ispark, 현재 부스터 게이지가 셋팅되는 엔진 부스터 최대보다 크면 다시 리셋
// //     {
// //         m_fCurrentBURN = nCurrentBURN;
// //     }
// // //    m_fMouseRate = pITEM->RangeAngle;
// //     CParamFactor*    pRefEnchant = pItemInfo->GetEnchantParamFactor();
// //     if(pRefEnchant)
// //     {
// //         m_paramFactor.pfm_ENGINE_ANGLE_UP = pRefEnchant->pfm_ENGINE_ANGLE_UP;
// //         m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = pRefEnchant->pfm_ENGINE_BOOSTERANGLE_UP;
// //     }
// //     else
// //     {
// //         // 초기화        
// //         m_paramFactor.pfm_ENGINE_ANGLE_UP = m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = 0.0f;
// //     }
// // 
// //     m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pITEM, &m_paramFactor);
// 
//     // 정지 상태에서 엔진을 교환할 때 앞으로 이동하지 않도록 한다.
//     if(m_fShuttleSpeed != 0.0f)
//         m_fShuttleSpeed = 0.0f;
// 
//     m_fShuttleSpeedMax            = pITEM->AbilityMax + GetEnchantDesParam(pItemInfo, DES_ENGINE_MAX_SPEED_UP);
//     m_fShuttleSpeedBoosterOn    = (float)pITEM->Range + GetEnchantDesParam(pItemInfo, DES_ENGINE_BOOSTER_SPEED_UP);
//     m_fShuttleSpeedMin            = pITEM->AbilityMin    + GetEnchantDesParam(pItemInfo, DES_ENGINE_MIN_SPEED_UP);
//     m_fShuttleGroundSpeedMax    = pITEM->Charging + GetEnchantDesParam(pItemInfo, DES_ENGINE_GROUND_SPEED_UP);
//     m_bySpeedPenalty            = pITEM->SpeedPenalty;
//     m_fBURN                        = (float)pITEM->Time/1000.0f + GetEnchantDesParam(pItemInfo, DES_ENGINE_BOOSTER_TIME_UP);
//     m_fBURN                        = FloatRangeDecimal(m_fBURN, 2);
// 
//     CParamFactor*    pRefEnchant = pItemInfo->GetEnchantParamFactor();
//     if(pRefEnchant)
//     {
//         m_paramFactor.pfm_ENGINE_ANGLE_UP            = pRefEnchant->pfm_ENGINE_ANGLE_UP;
//         m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP    = pRefEnchant->pfm_ENGINE_BOOSTERANGLE_UP;
//     }
//     else
//         m_paramFactor.pfm_ENGINE_ANGLE_UP            = m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = 0.0f;
// 
//     m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pITEM, &m_paramFactor);
// 
//     for( int i=POS_PROW; i <= POS_PET; ++i )
//     {
//         CItemInfo*    pEquipItem = g_pStoreData->FindItemInInventoryByWindowPos( i );
// 
//         if( !pEquipItem || i == POS_REAR )
//             continue;
// 
//         // 부스터 추가 시간
//         m_fBURN                += GetEnchantDesParam( pEquipItem, DES_ENGINE_BOOSTER_TIME_UP )
//                                + pEquipItem->GetRealItemInfo()->GetParameterValue( DES_ENGINE_BOOSTER_TIME_UP );
//         FloatRangeDecimal( m_fBURN, 2 );
// 
//         // 최소 속도
//         m_fShuttleSpeedMin    += GetEnchantDesParam( pEquipItem, DES_ENGINE_MIN_SPEED_UP )
//                                + pEquipItem->GetRealItemInfo()->GetParameterValue( DES_ENGINE_MIN_SPEED_UP );
// 
//         // 최대 속도
//         m_fShuttleSpeedMax    += GetEnchantDesParam( pEquipItem, DES_ENGINE_MAX_SPEED_UP )
//                                + pEquipItem->GetRealItemInfo()->GetParameterValue( DES_ENGINE_MAX_SPEED_UP );
// 
//         // 지상 속도
//         m_fShuttleGroundSpeedMax    += GetEnchantDesParam( pEquipItem, DES_ENGINE_GROUND_SPEED_UP )
//                                        + pEquipItem->GetRealItemInfo()->GetParameterValue( DES_ENGINE_GROUND_SPEED_UP );
// 
//         // 부스터 시간
//         m_fShuttleSpeedBoosterOn    += GetEnchantDesParam( pEquipItem, DES_ENGINE_BOOSTER_SPEED_UP )
//                                        + pEquipItem->GetRealItemInfo()->GetParameterValue( DES_ENGINE_BOOSTER_SPEED_UP );
// 
//         // 선회각
//         m_paramFactor.pfm_ENGINE_ANGLE_UP    += GetEnchantDesParam( pEquipItem, DES_ENGINE_ANGLE_UP )
//                                                + pEquipItem->GetRealItemInfo()->GetParameterValue( DES_ENGINE_ANGLE_UP );
// 
//         // 부스터시 선회각
//         m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP    += GetEnchantDesParam( pEquipItem, DES_ENGINE_BOOSTERANGLE_UP )
//                                                        + pEquipItem->GetRealItemInfo()->GetParameterValue( DES_ENGINE_BOOSTERANGLE_UP );
// 
//         m_fMouseRate    = CAtumSJ::GetEngineRangeAngle(pITEM, &m_paramFactor);
//     }
// 
// 
//     if((m_fCurrentBURN == 0) ||            // 현재 부스터 게이지가 0이라면 셋팅. 자주 리셋 되는 경우 때문에 이렇게 함
//         (m_fCurrentBURN > m_fBURN))        // 현재 부스터 게이지가 셋팅되는 엔진 부스터 최대보다 크면 다시 리셋
//     {
//         m_fCurrentBURN = nCurrentBURN;
//     }
// 
//     // end 2010. 03. 25 by jskim Speed, Angle 추가 계산

     if(m_fShuttleSpeed != 0.0f)
     {
        m_fShuttleSpeed = pITEM->AbilityMin;
           m_fShuttleSpeed = 0.0f;
     }
     m_fShuttleSpeedMax = pITEM->AbilityMax;
     m_fShuttleSpeedBoosterOn = (float)pITEM->Range;
     m_fShuttleSpeedMin = pITEM->AbilityMin;
     m_fShuttleGroundSpeedMax = pITEM->Charging;            // 2005-11-26 by ispark
     m_bySpeedPenalty = pITEM->SpeedPenalty;                // 2005-08-16 by ispark
     m_fBURN = (float)pITEM->Time/1000.0f;
 
     // 2008-09-26 by bhsohn 신규 인첸트 처리
     // 최소 속도
     {        
         float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_MIN_SPEED_UP);
         m_fShuttleSpeedMin += fTime; 
     }
     // 최대 속도
     {        
         float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_MAX_SPEED_UP);
         m_fShuttleSpeedMax += fTime;        
     }
 
     // 지상 속도
     {        
         float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_GROUND_SPEED_UP);
         m_fShuttleGroundSpeedMax += fTime;        
     }
 
     // 부스터 속도
     {        
        float fTime = GetEnchantDesParam(pItemInfo, DES_ENGINE_BOOSTER_SPEED_UP);
         m_fShuttleSpeedBoosterOn += fTime;        
     }
     // 부스터 추가 시간 
     float fTimeUp = GetEnchantDesParam(pItemInfo, DES_ENGINE_BOOSTER_TIME_UP);
     m_fBURN += fTimeUp;
     
     // 2008-03-19 by bhsohn FLOAT형 재정렬 하여 사용
     m_fBURN = FloatRangeDecimal(m_fBURN, 2);

    // 2010. 04. 27 by jskim 보급 상점에서 이륙시 부스터 0되는 버그 수정
    for(int i = POS_PROW; i <= POS_PET; i++)
    {
        CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( i ); 
        
        if(!pAllItem || i == POS_REAR )
            continue;
        
        // 2010. 04. 21 by jskim 신규 럭키 머신 구현
        //// 부스터 추가 시간 
        //m_fBURN += GetEnchantDesParam(pAllItem, DES_ENGINE_BOOSTER_TIME_UP) + pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_BOOSTER_TIME_UP);
        float tempBURN = 0.0f;
        if(pAllItem && i == POS_CENTER)
        {
            CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( i );
            
            for(int j=0; j <SIZE_DES_PARAM_PER_RARE_ITEM_INFO; j++)
            {
                if(pAllItem->GetPrefixRareInfo())
                {
                    if(pAllItem->GetPrefixRareInfo()->DesParameter[j] == DES_ENGINE_BOOSTER_TIME_UP)
                    {
                        tempBURN += pAllItem->GetPrefixRareInfo()->ParameterValue[j];
                    }
                }
                if(pAllItem->GetSuffixRareInfo())
                {
                    if(pAllItem->GetSuffixRareInfo()->DesParameter[j] == DES_ENGINE_BOOSTER_TIME_UP)
                    {
                        tempBURN += pAllItem->GetSuffixRareInfo()->ParameterValue[j];
                    }
                }
            }
        }    
        // 부스터 추가 시간 
        m_fBURN += GetEnchantDesParam(pAllItem, DES_ENGINE_BOOSTER_TIME_UP) + pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_BOOSTER_TIME_UP) + tempBURN;
        //end 2010. 04. 21 by jskim 신규 럭키 머신 구현
        m_fBURN = FloatRangeDecimal(m_fBURN, 2);
    }
    
    // 사용류 아이템 기능 가져오기
    vector<ItemFontInfo*> ItemInfo = g_pGameMain->m_pInfSkill->GetItemFontInfo();
    vector<ItemFontInfo*>::iterator it = ItemInfo.begin();
    while(it != ItemInfo.end())
    {
        // 부스터 추가 시간 
        if((*it)->pItemInfo)
        {
            m_fBURN += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_BOOSTER_TIME_UP);
            // FLOAT형 재정렬 하여 사용
            m_fBURN = FloatRangeDecimal(m_fBURN, 2);
        }
        it++;
     }
    //end 2010. 04. 27 by jskim 보급 상점에서 이륙시 부스터 0되는 버그 수정
 
     //DbgOut("#1 m_fCurrentBURN[%.2f] nCurrentBURN[%d] m_fBURN[%.2f] \n", m_fCurrentBURN, nCurrentBURN, m_fBURN);
     if((m_fCurrentBURN == 0) ||                            // 2005-12-10 by ispark, 현재 부스터 게이지가 0이라면 셋팅. 자주 리셋 되는 경우 때문에 이렇게 함
         (m_fCurrentBURN > m_fBURN))                        // 2005-12-15 by ispark, 현재 부스터 게이지가 셋팅되는 엔진 부스터 최대보다 크면 다시 리셋
     {
         m_fCurrentBURN = nCurrentBURN;
     }
 //    m_fMouseRate = pITEM->RangeAngle;
     CParamFactor*    pRefEnchant = pItemInfo->GetEnchantParamFactor();
     if(pRefEnchant)
     {
         m_paramFactor.pfm_ENGINE_ANGLE_UP = pRefEnchant->pfm_ENGINE_ANGLE_UP;
         m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = pRefEnchant->pfm_ENGINE_BOOSTERANGLE_UP;
     }
     else
     {
         // 초기화        
         m_paramFactor.pfm_ENGINE_ANGLE_UP = m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = 0.0f;
     }
      m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pITEM, &m_paramFactor);
//end 2010. 04. 15 by jskim 부스터 최소 속도, 최고 속도 2배 되는 버그 수정

    ASSERT_ASSERT(m_fBURN>0.0f);

    if(g_pGameMain && g_pGameMain->m_pUnitInfoBar)
    {
        g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
//        g_pGameMain->m_pUnitInfoBar->SetSpeed(m_fShuttleSpeed, m_fShuttleSpeedMin, m_fShuttleSpeedBoosterOn);
        g_pGameMain->m_pUnitInfoBar->SetSpeed(m_fShuttleSpeed, m_fShuttleSpeedMin, m_fShuttleSpeedBoosterOn);
    }
}

void CShuttleChild::InitEffect()
{
    FLOG( "CShuttleChild::InitEffect()" );

    if(COMPARE_RACE(m_myShuttleInfo.Race,RACE_GAMEMASTER))
    {
        //m_dwPartType = _ADMIN;
    }

//    if(m_dwPartType == _ADMIN)
//    {
//        if(!m_pCharacterInfo)
//        {
//            char strFileName[16];
//            wsprintf( strFileName, "10000000");
//            LoadCharacterEffect(strFileName);
//            if(m_pCharacterInfo)
//            {
//                m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
//                m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_CHARACTER_ANIMATION_TIME);
//                m_pCharacterInfo->SetBodyCondition(m_myShuttleInfo.BodyCondition);
//            }
//            m_nUnitNum = 10000000;
//        }
//    }
//    else
//    {
        CItemInfo* pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
        // 2005-07-20 by ispark
        // 캐릭터 렌더링 번호를 입력한다. 여기에 이펙트 포함

        // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
        //ChangeUnitCharacterInfo((pItem?pItem->ItemInfo->SourceIndex:0), m_myShuttleInfo.UnitKind, GetCurrentBodyCondition(), FALSE);

        int nDefenseNum = 0;

        if( !pItem )
            nDefenseNum = 0;
        else
        {
#ifdef _REWORKED_COLORSHOP
            if (pItem->ColorCode)
            {
                nDefenseNum = pItem->ColorCode;
            }
            else
#endif
            {
                if (!pItem->ShapeItemNum)
                    nDefenseNum = pItem->ItemInfo->SourceIndex;
                else
                {
                    ITEM* pShapeItem = g_pDatabase->GetServerItemInfo(pItem->ShapeItemNum);
                    if (pShapeItem)
                        nDefenseNum = pShapeItem->SourceIndex;
                    else
                        nDefenseNum = pItem->ItemInfo->SourceIndex;
                }
            }

        }
        // 2010. 03. 18 by jskim 몬스터변신 카드
        //ChangeUnitCharacterInfo( nDefenseNum, m_myShuttleInfo.UnitKind, GetCurrentBodyCondition(), FALSE );
        BOOL bMonTras = FALSE;

        if( m_MonsterTransformer )
        {
            ChangeUnitCharacterInfo(nDefenseNum, m_myShuttleInfo.UnitKind, GetCurrentBodyCondition(), FALSE);
            SetMonsterTransPrimarySidePos(m_PrimaryAttack.vSidePos);
            SetMonsterTransSecondarySidePos(m_SecondaryAttack.vSidePos);
        }

        if( m_MonsterTransformer )
        {
            bMonTras = TRUE;
            MEX_MONSTER_INFO * pTempIn = g_pGameMain->CheckMonsterInfo( m_MonsterTransformer );
            if(pTempIn)
                nDefenseNum = pTempIn->SourceIndex;
        }
        ChangeUnitCharacterInfo( nDefenseNum, m_myShuttleInfo.UnitKind, GetCurrentBodyCondition(), FALSE, bMonTras );
        //end 2010. 03. 18 by jskim 몬스터변신 카드
        // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//    }
}

void CShuttleChild::CheckGroundEffect()
{
    FLOG( "CShuttleChild::CheckGroundEffect()" );
    if( g_pScene->m_byMapType == MAP_TYPE_CITY || 
        m_dwState == _EXPLODED || 
        m_dwState == _FALLEN || 
        m_dwState == _LANDED)
    {
        CAppEffectData * pData = (CAppEffectData*)g_pD3dApp->m_pEffectList->m_pChild;
        while(pData)
        {
            if(pData->m_pParent == this && !COMPARE_BODYCON_BIT(pData->m_bodyCondition,BODYCON_EXPLOSION_MASK))
            {
                if(pData->m_nType == RC_EFF_GROUND_01 || pData->m_nType == RC_EFF_GROUND_02)
                {
                    pData->ChangeBodyCondition(BODYCON_EXPLOSION_MASK);
//                    pData->m_bodyCondition = BODYCON_EXPLOSION_MASK;
//                    if(pData->m_pCharacterInfo)
//                        pData->m_pCharacterInfo->ChangeBodyCondition(pData->m_bodyCondition);
                }
            }
            pData = (CAppEffectData*)pData->m_pNext;
        }
        m_bEffectGround = 0;
    }
    else// if(m_dwState != _EXPLODED)
    {
        BYTE bOldEffectGround = m_bEffectGround;
        m_bEffectGround = 0;
        FLOAT fDist,fHeight,fCheckHeight;
        if(m_bOnObject)
            fCheckHeight = 15.0f;
        else
            fCheckHeight = 30.0f;
        int i,j;
        i = (int)(m_vPos.x/TILE_SIZE);
        j = (int)(m_vPos.z/TILE_SIZE);
        D3DXVECTOR3 v,dir;
        dir = D3DXVECTOR3(0,-1,0);
        m_bIsWater = CheckIsWater(m_vPos);

        // 2005-02-11 by jschoi
        D3DXMATRIX mat;
        D3DXVECTOR3 vSide(0,0,1),vUp(0,1,0);
        D3DXMatrixLookAtLH(&mat,&m_vPos,&(m_vPos - m_vUp),&vSide);        // 아래
        if(    m_bIsWater &&
            g_pScene &&
            g_pScene->m_pObjectRender)
        {    
            COLLISION_RESULT collResult = g_pScene->m_pObjectRender->CheckCollMeshWaterObject(mat,m_vPos);
            if(collResult.fDist < fCheckHeight)
            {
                fHeight = m_vPos.y - collResult.fDist + 1.0f;
                m_bEffectGround = 1;
            }
        }

        if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize)
        {
            if(g_pGround->IntersectTriangle(m_vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
                g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
                g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
                &fDist))
            {
                v = m_vPos + dir*fDist;
                if(m_bIsWater)
                {
                    if(v.y > g_pGround->m_projectInfo.fWaterHeight)
                    {//땅
                        if(m_vPos.y <= v.y + fCheckHeight)
                        {
                            fHeight = v.y + 1.0f;
                            m_bEffectGround = 2;
                        }
                    }
                    else
                    {//물
                        if(    m_vPos.y <= g_pGround->m_projectInfo.fWaterHeight + fCheckHeight )
                        {
                            fHeight = g_pGround->m_projectInfo.fWaterHeight + 1.0f;
                            m_bEffectGround = 1;
                        }
                    }
                }
                else
                {//땅
                    if(m_vPos.y <= v.y + fCheckHeight)
                    {
                        fHeight = v.y + 1.0f;
                        m_bEffectGround = 2;
                    }
                }
            }
            else if(g_pGround->IntersectTriangle(m_vPos,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
                g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
                g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
                &fDist))
            {
                v = m_vPos + dir*fDist;
                if(m_bIsWater)
                {
                    if(v.y > g_pGround->m_projectInfo.fWaterHeight)
                    {//땅
                        if(m_vPos.y <= v.y + fCheckHeight)
                        {
                            fHeight = v.y + 1.0f;
                            m_bEffectGround = 2;
                        }
                    }
                    else
                    {//물                        
                        if(m_vPos.y <= g_pGround->m_projectInfo.fWaterHeight + fCheckHeight)
                        {
                            fHeight = g_pGround->m_projectInfo.fWaterHeight + 1.0f;
                            m_bEffectGround = 1;
                        }
                    }
                }
                else
                {//땅
                    if(m_vPos.y <= v.y + fCheckHeight)
                    {
                        fHeight = v.y + 1.0f;
                        m_bEffectGround = 2;
                    }
                }
            }
        }
        // 2008-12-04 by bhsohn 최소화 모드시, 물 랜더링 처리
//        BYTE bEffectGround = m_bEffectGround;
//        if(g_pSOption->sLowQuality)
//        {
//            // 이 부분을 수정하면 최소 7프레임 이상 보장~~~
//            // 꼭, 수정하자~~~~!
//            m_bEffectGround = 0;
//        }
        // end 2008-12-04 by bhsohn 최소화 모드시, 물 랜더링 처리
        if( IsTileMapRenderEnable(m_myShuttleInfo.MapChannelIndex.MapIndex) == TRUE &&
            bOldEffectGround != m_bEffectGround)
        {
            CAppEffectData * pData = (CAppEffectData*)g_pD3dApp->m_pEffectList->m_pChild;
            while(pData)
            {
                if(pData->m_pParent == this && !COMPARE_BODYCON_BIT(pData->m_bodyCondition,BODYCON_EXPLOSION_MASK))
                {
                    if(pData->m_nType == RC_EFF_GROUND_01 || pData->m_nType == RC_EFF_GROUND_02)
                    {
                        pData->ChangeBodyCondition(BODYCON_EXPLOSION_MASK);
//                        pData->m_bodyCondition = BODYCON_EXPLOSION_MASK;
//                        if(pData->m_pCharacterInfo)
//                            pData->m_pCharacterInfo->ChangeBodyCondition(pData->m_bodyCondition);
                    }
                }
                pData = (CAppEffectData*)pData->m_pNext;
            }
            if(m_bEffectGround == 1)
            {
                D3DXVECTOR3 vTargetPos;
                vTargetPos.x = 0.0f;
                vTargetPos.y = fHeight;
                vTargetPos.z = 0.0f;
                pData = new CAppEffectData(this,RC_EFF_GROUND_01,vTargetPos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
            }
            else if(m_bEffectGround == 2)
            {
                D3DXVECTOR3 vTargetPos;
                vTargetPos.x = 7.0f;
                vTargetPos.y = -20.0f;
                vTargetPos.z = -3.0f;
                pData = new CAppEffectData(this,RC_EFF_GROUND_02,vTargetPos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
                vTargetPos.x = -7.0f;
                vTargetPos.y = -20.0f;
                vTargetPos.z = -3.0f;
                pData = new CAppEffectData(this,RC_EFF_GROUND_02,vTargetPos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
                vTargetPos.x = 0.0f;
                vTargetPos.y = -20;
                vTargetPos.z = 0.0f;
                pData = new CAppEffectData(this,RC_EFF_GROUND_02,vTargetPos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
            }
        }
    }
}

//void CShuttleChild::AutoControlHeight()
//{// 자동 고도 조절 - Enemy,Monster 검색
//    FLOG( "CShuttleChild::AutoControlHeight()" );
//    float fDist;
//    if(m_pTarget && 
//        m_pTarget->m_dwState == _NORMAL && 
//        m_pTarget->m_dwPartType == _MONSTER)
//    {
//        fDist = m_pTarget->m_vPos.y - m_vPos.y;
//        if(fDist > 10.0f)
//            fDist = 10.0f; // 이동 속도 제한
//        m_vPos.y += fDist*g_pD3dApp->GetElapsedTime();
//    }
//}

void CShuttleChild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    FLOG( "CShuttleChild::WndProc()" );
    float fElapsedTime = g_pD3dApp->GetElapsedTime();

    if(!g_pD3dApp->m_bChatMode)// && m_bMoveInputType == 0)    
    {    
        switch(uMsg)
        {
        case WM_KEYDOWN:
            {
                switch( wParam )
                {
                case VK_LEFT:
                    m_bLeftButtonState = TRUE;
                    
                    // 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
//                     if((g_pTutorial->IsTutorialMode() == FALSE || 
//                         (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() >= L3)) &&
//                         m_bIsAir == TRUE &&
//                         m_dwState == _NORMAL)
//                     {
//                         if(m_fADoubleKeyTime < DOUBLEKEY_TIME 
//                             && m_bKeyDownACheck == FALSE
//                             // 2007-02-28 by dgwoo 부스터 사용시에는 롤링을 저장하지 않는다.
//                             && m_nBooster != BOOSTER_TYPE_MAX)
//                         {
//                             DbgOut("CShuttleChild::WndProc() : m_bKeyDownADouble = TRUE\n");
//                             m_bKeyDownADouble = TRUE;
//                             m_bKeyDownDDouble = FALSE;
//                         }
//                         else
//                         {
//                             m_fADoubleKeyTime = 0.0f;
//                         }
//                     }
                    ActiveKeyDownADouble();
                    // end 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
                    break;
                case VK_UP:
                    m_bUpButtonState = TRUE;
                    break;
                case VK_RIGHT:
                    m_bRightButtonState = TRUE;

                    // 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
//                     if((g_pTutorial->IsTutorialMode() == FALSE || 
//                         (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() >= L3)) &&
//                         m_bIsAir == TRUE &&
//                         m_dwState == _NORMAL)
//                     {
//                         if(m_fDDoubleKeyTime < DOUBLEKEY_TIME 
//                             && m_bKeyDownDCheck == FALSE
//                             // 2007-02-28 by dgwoo 부스터 사용시에는 롤링을 저장하지 않는다.
//                             && m_nBooster != BOOSTER_TYPE_MAX)
//                         {
//                             DbgOut("CShuttleChild::WndProc() : m_bKeyDownDDouble = TRUE\n");
//                             m_bKeyDownDDouble = TRUE;
//                             m_bKeyDownADouble = FALSE;
//                         }
//                         else
//                         {
//                             m_fDDoubleKeyTime = 0.0f;
//                         }
//                     }
                    ActiveKeyDownDDouble();
                    // end 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
                    break;
                case VK_DOWN:
                    m_bDownButtonState = TRUE;
                    break;
//                case VK_END:
//                    {
//                        // 2006-05-03 by ispark, 백뷰 마우스 휠클릭과 R버튼으로 변경
//                        // 2005-04-07 by jschoi - Tutorial
//                        if(g_pTutorial->IsTutorialMode() == FALSE)
//                        {
//                            m_bTurnCamera = TRUE;
//                            m_bMouseLock  = TRUE;                    
//                            m_fOldUpRate = 0.0f;                            
//                        }
//                    }
//                    break;
                case VK_CONTROL:
                    {
                        if( IsPkEnableMap(m_myShuttleInfo.MapChannelIndex.MapIndex) )
                        {
                            SetPKMode( TRUE );
                        }
                        m_bCtrlKey = TRUE;
                    }
                    break;
                case VK_SHIFT:
                    {
                        SetUnLockOn();        // 2005-10-20 by ispark

                        if( IsPkEnableMap(m_myShuttleInfo.MapChannelIndex.MapIndex) )
                        {
                            SetPKMode( FALSE );
                        }                        
                    }
                    break;
                }
                switch(lParam)
                {
                case WM_KEYDOWN_LPARAM_E:
                    {
                        // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음                    
                        //m_bRButtonState = TRUE;
                        if(IsPossibleAttackStat())
                        {
                            m_bRButtonState = TRUE;
                        }                        
                        // end 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음                    
                        
                    }
                    break;
                case WM_KEYDOWN_LPARAM_Z:
                    {
                    // 2016-09-20 panoskj removed these
//                        if( m_pClientParty->GetNtoNCallState() == TRUE ||
//                            g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState() == TRUE)
//                        {
//                            // 2008-10-14 by bhsohn VOIP개선
////                            COPYDATASTRUCT    copyData;
////                            memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
////                            AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK, 0);
////                            copyData.dwData = (DWORD)voIP;
////                            copyData.lpData = NULL;
////                            copyData.cbData = 0;
////                            ::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//                            g_pGameMain->ChangeVOIPMode();
//                        }
                    }
                    break;
                case WM_KEYDOWN_LPARAM_B:
                    {
                        // 2005-08-08 by ispark
                        // 유닛정지 시스템
                        // 관리자, 운영자 모드에서만 사용
                        // // 2005-08-10 by ispark 그외 제외 되는 상태 (스킬 포함)
                        // 2005-09-26 by ispark 부로 상황 종료
//                        if(!COMPARE_RACE(m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER) || 
//                            m_bSkillMoveIsUse ||
//                            m_dwState == _FALLEN || 
//                            m_dwState == _EXPLODED || 
//                            m_dwState == _LANDED ||
//                            m_dwState == _LANDING || 
//                            m_dwState == _TAKINGOFF)
//                            break;
                        // 2006-10-24 by dgwoo 기어의 상태가 죽은 상황일때는 B키는 막는다.
                        if(CheckUnitState() == BREAKDOWN)
                            break;
                    
                        if(m_bIsAir == FALSE && IS_DT(m_myShuttleInfo.UnitKind)            //A기어가 땅에 있을때
                            || g_pD3dApp->m_dwGameState == _SHOP                        //상점에 있을때
                            || m_bAttackMode == _SIEGE)                                    //시즈 모드일때 
                            break;
                        // 2005-10-06 by ispark
                        // 튜토리얼일때는 정지 사용 안함
                        // 편대 비행일때 편대장 이외는 정시 사용 금지
                        // 기어가 LANDED일때 정지 사용 안함
                        // 패턴 사용중에는 정지 사용 안함
                        if((g_pTutorial->IsTutorialMode() == FALSE || 
                            (g_pTutorial->GetLesson() == L2 && g_pTutorial->GetChapter() == L2_2 && g_pTutorial->GetChapterState() == TUTORIAL_STATE_MOTION)) &&
                            m_pClientParty->IsFormationFlightMove() == FALSE &&
                            m_bLandedMove == FALSE &&
                            m_bSkillMoveIsUse == FALSE)
                        {
                            if(m_bUnitStop == FALSE)    // 정지시 메세지를 물어본다.
                            {
                                g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_050809_0001, //"몬스터의 공격을 받을 수 있습니다. 정지 하시겠습니까?"
                                _Q_UNIT_STOP);
                            }
                            else if(m_bSkillGoMove)    // 출발 할 때
                            {
                                // 2008-07-03 by bhsohn EP3 버그 리포트 수정
                                if(g_pGameMain->m_pCommunity->IsShowCommunityWindow())
                                {
                                    // 출발시 강제로 창닫음
                                    g_pGameMain->m_pCommunity->ShowCommunityWindow(FALSE);
                                }
                                if(g_pGameMain->m_pSystem && g_pGameMain->m_pSystem->IsOptionShow())
                                {
                                    g_pGameMain->PopupSystemOptinWnd();
                                }
                                // end 2008-07-03 by bhsohn EP3 버그 리포트 수정
                                
                                m_bUnitStop = FALSE;
                                m_bPenaltyUnitStop = TRUE;
                                m_bMouseMoveLock = FALSE;
                                m_nBooster = BOOSTER_TYPE_MIN;
                                g_pInterface->m_pGameMain->InitShowWindow();
                                // 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달

                                SendFieldSocketChangeBodyCondition( m_myShuttleInfo.ClientIndex, ShuttleChangeBodyCondition());        // 2013-06-25 by ssjung 데미지 이펙트가 안나오는 현상 수정
//                                SendFieldSocketChangeBodyCondition( m_myShuttleInfo.ClientIndex, BODYCON_SET_OR_CLEAR_MASK );
                                //end 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달    
                            }                        
                        }
                    }
                    break;
                case WM_KEYDOWN_LPARAM_A:
                    {
                        // 2005-10-06 by ispark
                        // 1. 튜토리얼이 아니다
                        // 2. 튜토리얼이면서 레슨5 이다
                        // 3. 비행중이다
                        // 4. 비행상태가 _NORMAL
                        // 사용한다.
                        // 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
//                         if((g_pTutorial->IsTutorialMode() == FALSE || 
//                             (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() >= L3)) &&
//                             m_bIsAir == TRUE &&
//                             m_dwState == _NORMAL)
//                         {
// //                            DBGOUT("A Push %f\n", m_fADoubleKeyTime);
// //                            m_bKeyDownAPush = TRUE;
//                             if(m_fADoubleKeyTime < DOUBLEKEY_TIME 
//                                 && m_bKeyDownACheck == FALSE 
//                                 // 2007-02-28 by dgwoo 부스터 사용시에는 롤링을 저장하지 않는다.
//                                 && m_nBooster != BOOSTER_TYPE_MAX)
//                             {
//                                 DbgOut("CShuttleChild::WndProc() : m_bKeyDownADouble = TRUE\n");
//                                 m_bKeyDownADouble = TRUE;
//                                 m_bKeyDownDDouble = FALSE;
//                             }
//                             else
//                             {
//                                 m_fADoubleKeyTime = 0.0f;
//                             }
//                         }
                        ActiveKeyDownADouble();
                        // end 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
                    }
                    break;
                case WM_KEYDOWN_LPARAM_D:
                    {
                        // 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
//                         if((g_pTutorial->IsTutorialMode() == FALSE || 
//                             (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() >= L3)) &&
//                             m_bIsAir == TRUE &&
//                             m_dwState == _NORMAL)
//                         {
// //                            DBGOUT("D Push %f\n", m_fDDoubleKeyTime);
// //                            m_bKeyDownDPush = TRUE;
//                             if(m_fDDoubleKeyTime < DOUBLEKEY_TIME 
//                                 && m_bKeyDownDCheck == FALSE
//                                 // 2007-02-28 by dgwoo 부스터 사용시에는 롤링을 저장하지 않는다.
//                                 && m_nBooster != BOOSTER_TYPE_MAX)
//                             {
//                                 m_bKeyDownDDouble = TRUE;
//                                 m_bKeyDownADouble = FALSE;
//                             }
//                             else
//                             {
//                                 m_fDDoubleKeyTime = 0.0f;
//                             }
//                         }
                        ActiveKeyDownDDouble();
                        // end 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
                    }
                    break;
                case WM_KEYDOWN_LPARAM_R:
                    {
                        // 2006-05-03 by ispark, 백뷰
                        SetBackView(TRUE);
                    }
                    break;
                // 2008-10-23 by bhsohn 자기 자신한테 힐 단축키 추가
                case WM_KEYDOWN_LPARAM_FLOW:
                    {
                        if(    m_pSkill && m_pSkill->GetSkillTargetState() == TRUE )
                        {
                            m_pSkill->FindTargetForSkill_ToMe();
                        }
                    }
                    break;
                // end 2008-10-23 by bhsohn 자기 자신한테 힐 단축키 추가
                    // 2008-11-13 by bhsohn 조이스틱 작업
                case WM_KEYDOWN_LPARAM_KEYPAD_PLUS:
                    {
                        if(!g_pD3dApp->IsUseJoyStick())
                        {
                            return;
                        }
                        g_pJoysticOp->nJoysticSense++;
                        if(g_pJoysticOp->nJoysticSense >= MAX_JOYSTICK_SENSITY)
                        {
                            g_pJoysticOp->nJoysticSense = MAX_JOYSTICK_SENSITY-1;
                        }
                        else
                        {
                            char buf[256];
                            // 2008-12-23 by bhsohn 조이스틱 추가 요청 처리
                            wsprintf(buf, STRMSG_C_081120_0200, g_pJoysticOp->nJoysticSense-MAX_JOYSTICK_DEFAULTSENSITY );
                            g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);
                        }
                    }
                    break;
                case WM_KEYDOWN_LPARAM_KEYPAD_MINUS:
                    {
                        if(!g_pD3dApp->IsUseJoyStick())
                        {
                            return;
                        }

                        g_pJoysticOp->nJoysticSense--;
                        if(g_pJoysticOp->nJoysticSense < 0)
                        {
                            g_pJoysticOp->nJoysticSense = 0;
                        }
                        else
                        {
                            char buf[256];
                            wsprintf(buf, STRMSG_C_081120_0201, g_pJoysticOp->nJoysticSense-MAX_JOYSTICK_DEFAULTSENSITY );
                            g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);
                        }
                        
                    }
                    break;
                    // end 2008-11-13 by bhsohn 조이스틱 작업
                }    
            }
            break;
        case WM_KEYUP:
            {
                switch( wParam )
                {
                    case VK_LEFT:
                        m_bLeftButtonState = FALSE;
                        break;
                    case VK_UP:
                        m_bUpButtonState = FALSE;
                        break;
                    case VK_RIGHT:
                                m_bRightButtonState = FALSE;
                        break;
                    case VK_DOWN:
                        m_bDownButtonState = FALSE;
                        break;
    //                case VK_END:
    //                    {
    //                        // 2006-05-03 by ispark, 백뷰 마우스 휠클릭과 R버튼으로 변경
    //                        // 2005-04-07 by jschoi - Tutorial
    //                        if(g_pTutorial->IsTutorialMode() == FALSE)
    //                        {
    //                            m_bTurnCamera = FALSE;
    ////                            g_pCamera->SetCamType(CAMERA_TYPE_NORMAL);
    //                            //m_bCamType = CAMERA_TYPE_NORMAL;
    //                            m_bMouseLock  = FALSE;
    //                            m_fOldUpRate = 0.0f;
    //                        }
    //                    }
    //                    break;
                    case VK_CONTROL:
                        {
                            // 2005-10-20 by ispark
    //                        if( IsPkEnableMap(m_myShuttleInfo.MapChannelIndex.MapIndex) )
    //                        {
    //                            SetPKMode( FALSE );
    //                        }
                            
                            m_bCtrlKey = FALSE;
                        }
                        break;
                }

                switch(lParam)
                {
                    case WM_KEYUP_LPARAM_E:
                        {
                            m_bRButtonState = FALSE;
                            m_SecondaryAttack.AttackData.TargetInfo.SetNullTarget();
    //                        if(m_pSecondaryWeapon)
    //                        {
    //                            m_pSecondaryWeapon->SetUseToggleWeapon(TRUE);
    //                        }
                        }
                        break;
                    case WM_KEYUP_LPARAM_Z:
                        {
                            // 2008-10-14 by bhsohn VOIP개선                            
    //                        if( m_pClientParty->GetNtoNCallState() == TRUE ||
    //                            g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState() == TRUE)
    //                        {
    //                            COPYDATASTRUCT    copyData;
    //                            memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
    //                            AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK_CANCEL, 0);
    //                            copyData.dwData = (DWORD)voIP;
    //                            copyData.lpData = NULL;
    //                            copyData.cbData = 0;
    //                            ::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
    //                        }
                            // end 2008-10-14 by bhsohn VOIP개선                            
                        }
                        break;
                    case WM_KEYUP_LPARAM_R:
                        {
                            // 2006-05-03 by ispark, 백뷰
                            SetBackView(FALSE);
                        }
                        break;
                }
            }
            break;
/*        case WM_MBUTTONDBLCLK:
            {
                m_bTurnCamera = !m_bTurnCamera;
            }
            break;
*/        case WM_MBUTTONDOWN:
            {
                // 2010. 05. 27 by jskim 시네마 적용 카메라 구현
                if( IsShowCinema() )
                {
                    break;
                }
                //end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
                m_bMButtonState = TRUE;
                // 2005-08-08 by ispark
                // M버튼을 막는다. 그전에 있어서 롤링시 M버튼 사용금지도 추가
//                if(m_bRollUsed == FALSE)
//                {
//                    SetShuttleFlight();
//                }
// 2005-08-03 by ispark
// 사용 금지
//#ifdef _DEBUG
//                DeletePattern();
//                CObjectChild * pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPosition(EVENT_TYPE_OBJ_ENTER_BUILDING, m_vPos, 7000.0f);                
//                if(pCurrentObjEvent)
//                {
//                    CObjectChild* pObj = g_pScene->FindObjectByIndex(pCurrentObjEvent->m_sEventIndexFrom);                    
//                    if(pObj)
//                    {
//                        // 위치 이동
//                        m_vPos = pObj->m_vPos;
//                        g_pCharacterChild->m_vPos = pObj->m_vPos;
//                        
//                        // 메세지 처리
//                        INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE,
//                            T_FC_CHARACTER_CHANGE_CHARACTER_MODE, pSMode, SendBuf);
//                        pSMode->CharacterMode0 = TRUE;
//                        pSMode->PositionAVec3 = m_vPos;
//                        pSMode->TargetAVec3 = m_vVel*1000.0f;
//                        g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE));
//                        
//                        // 캐릭터 모드 변환
//                        g_pD3dApp->m_bCharacter = TRUE;
//                        g_pCharacterChild->InitCharacterData();              
//                    }
//                }
//#endif
                // 2006-05-03 by ispark, 백뷰
                SetBackView(TRUE);
            }
            break;
        case WM_MBUTTONUP:
            {
                m_bMButtonState = FALSE;
                // 2006-05-03 by ispark, 백뷰
                SetBackView(FALSE);
            }
            break;
        case WM_MOUSEWHEEL:
            {
//                if( m_dwState != _NORMAL ||
//                    g_pGameMain->IsShowing() ||
//                    m_bAttackMode == _SIEGE ||
//                    m_bMouseLock == TRUE ||
//                    m_nBooster == BOOSTER_TYPE_AIRBREAK ||
//                    g_pTutorial->IsTutorialMode() == TRUE )
//                {
//                    break;
//                }
//                if( (int)wParam > 0 )
//                {
//                    m_fShuttleSpeed += 10;
//                    if(m_fShuttleSpeed > m_fRealShuttleSpeedMax)
//                        m_fShuttleSpeed = m_fRealShuttleSpeedMax;
//                }
//                else
//                {
//                    m_fShuttleSpeed -= 10;
//                    if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
//                        m_fShuttleSpeed = m_fRealShuttleSpeedMin;
//                }
            }
            break;
        case WM_LBUTTONDOWN:
            {
                // 2010. 05. 27 by jskim 시네마 적용 카메라 구현
                if( IsShowCinema() )
                {
                    break;
                }
                //end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
                //    내가 지금 타겟 스킬을 사용하기 위해 타겟을 찾는 중이라면..
                //    총알이 발사되지 않고, 대신에 스킬 클래스에 있는 타겟을 찾는 함수를 호출한다.
                if(    m_pSkill->GetSkillTargetState() == TRUE )
                {
                    m_pSkill->FindTargetForSkill();
                }
                else
                {
                    // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음
                    //if(IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster))
                    if(IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster) && IsPossibleAttackStat())                    
                    {
                        m_bLButtonState = TRUE;
                    }                
                }
                //첫발사가 안보이는 경우가 생겨 이 코드를 추가
    //            m_pPrimaryAttackTarget = NULL;
            }
            break;
        case WM_LBUTTONUP:
            {
                m_bLButtonState = FALSE;
                m_PrimaryAttack.AttackData.TargetInfo.SetNullTarget();
                m_bTargetChange = TRUE;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
                // 2011-11-01 by jhahn EP4 필드맵 상점 클릭
                GUI_BUILDINGNPC* pCurrentBuildingNPC = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
        
                if(pCurrentBuildingNPC)
                    return ;
                if (m_bUnitStop )
                {
                    POINT pt;
                    GetCursorPos(&pt);
                    ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
                    CheckMouseReverse(&pt);
                    
                    
                    if(g_pGameMain->m_bMenuLock || g_pD3dApp->IsLockMode() == FALSE)
                        break;
                    
                
                    if(g_pInterface->m_bShowInterface && (pt.x >= 0 && pt.x <= g_pD3dApp->GetBackBufferDesc().Width) &&
                        (pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 39) || ((pt.y >= g_pD3dApp->GetBackBufferDesc().Height - 51) && 
                        (pt.x >= g_pD3dApp->GetBackBufferDesc().Width / 2 - 117 && pt.x <= g_pD3dApp->GetBackBufferDesc().Width / 2 + 117)))
                    {
                        break;
                    }        
                    
                    
                    COLLISION_RESULT Coll_Result_Pick;
                    Coll_Result_Pick = g_pScene->m_pObjectRender->CheckPickMesh(pt.x, pt.y);
                
                    float fSearchLength = 50.0f;

                    COLLISION_OBJ_RESULT stObject;                    
                    stObject = g_pScene->m_pObjectRender->GetPickObject(pt.x, pt.y);

                
                
                    if(stObject.pObject )
                    {
                        
                        float fDistanceScene = D3DXVec3Length(&(m_vPos - stObject.stCollision.vPicking));
                    
                        if(stObject.pObject->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC && fDistanceScene<100.0f)
                        {
                            if( g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->m_bShopConcurrent + CINFInvenExtend::SHOP_CONCURRENT_DELAY < timeGetTime() )
                            {
                                
                                g_pD3dApp->m_pInterface->m_pGameMain->m_pInven->m_bShopConcurrent = timeGetTime();
                                
                                MSG_FC_EVENT_CHARACTERMODE_ENTER_BUILDING sMsg;
                                sMsg.nBuildingIndex0 = stObject.pObject->m_sEventIndexTo;

                                g_pFieldWinSocket->SendMsg( T_FC_EVENT_CHARACTERMODE_ENTER_BUILDING, (char*)&sMsg, sizeof(sMsg) );                
                                g_pD3dApp->m_bRequestEnable = FALSE;    
                                
                                                
                            }

    
                        }
                    }
                

                }
                //end 2011-11-01 by jhahn EP4 필드맵 상점 클릭
#endif
            }
            break;
        case WM_RBUTTONDOWN:
            {
                // 2010. 05. 27 by jskim 시네마 적용 카메라 구현
                if( IsShowCinema() )
                {
                    break;
                }
                //end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
                if( m_dwState == _FALLEN || 
                    m_dwState == _EXPLODED || 
                    m_dwState == _LANDED || 
//                    m_nBooster == BOOSTER_TYPE_AIRBREAK || 
//                    m_bFormationFlight || 
                    m_bMouseLock)
                {// 카메라 조절 모드로 전환
                    if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL && m_bAttackMode != _SIEGE)
                    {
                        g_pCamera->m_bIsCamControl = TRUE;
                        POINT pt;
                        pt.x = LOWORD(lParam);
                        pt.y = HIWORD(lParam);
                        CheckMouseReverse(&pt);
    
                        m_pOldMousePoint = pt;
                    }
                }
                // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음                    
                //m_bRButtonState = TRUE;
                if(IsPossibleAttackStat())
                {
                    m_bRButtonState = TRUE;
                }
                // end 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음                    
                
//                if(IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster))
//                {
//                    m_bRButtonState = TRUE;
//                }
            }
            break;
        case WM_RBUTTONDBLCLK:
        case WM_LBUTTONDBLCLK:
            {
                if(IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster) && m_myShuttleInfo.Level < 6)
                {
//                    g_pGameMain->m_pNotice->SetNotice( "공격은 마우스 좌/우 버튼을 누르고 있으면 자동으로 됩니다.");
                    g_pGameMain->SetNotice( 14 );
                }
            }
            break;
        case WM_RBUTTONUP:
            {
                // 카메라 전환 모드 해제
                if(g_pD3dApp->m_pCamera->m_bIsCamControl)
                {
                    g_pD3dApp->m_pCamera->m_bIsCamControl = FALSE;
                }
                m_bRButtonState = FALSE;
                m_SecondaryAttack.AttackData.TargetInfo.SetNullTarget();
//                if(m_pSecondaryWeapon)
//                {
//                    m_pSecondaryWeapon->SetUseToggleWeapon(TRUE);
//                }
            }
            break;
        }
    }
    // 2008-09-02 by dgwoo MouseMove이벤트는 채팅 입력중에도 이벤트 처리.
    switch(uMsg)
    {
        case WM_MOUSEMOVE:
            {
                POINT pt;
                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);
                CheckMouseReverse(&pt);

                if((m_dwState == _FALLEN || 
                    m_dwState == _EXPLODED || 
                    m_dwState == _LANDED || 
                    m_pClientParty->IsFormationFlightMove() || 
                    m_bMouseLock) &&
                    m_bPenaltyUnitStop == FALSE)                // 페널티 상태가 아닐때 움직임 허용                    
//                    ((IS_OT(m_myShuttleInfo.UnitKind) || IS_DT(m_myShuttleInfo.UnitKind)) && m_bMouseLock))                    
                {// 카메라 조절 모드로 전환
                    if(g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL)
                    {
                        if( g_pCamera->GetCamType() == CAMERA_TYPE_NORMAL &&
                            g_pD3dApp->m_pCamera->m_bIsCamControl && 
                            m_bRButtonState)
                        {
                            // 2005-08-19 by ispark
                            if(m_pOldMousePoint.y - pt.y != 0)
                                g_pCamera->SetControlCamAngle(pt.y - m_pOldMousePoint.y);
                            if(m_pOldMousePoint.x - pt.x != 0)
                                g_pCamera->SetControlCamSideAngle(pt.x - m_pOldMousePoint.x);
                        }
                        m_pOldMousePoint = pt;    
                    }
                }
                m_pOldMousePoint = pt;
            }
            break;
        }
}

void CShuttleChild::SetShuttleData(BYTE type)
{
    FLOG( "CShuttleChild::SetShuttleData(BYTE type)" );
//    map<int, ITEM_GENERAL*>::iterator it = NULL;
    switch(type)
    {
    case 0:
        {
            m_fDefenseTotal = 0.0f;
            m_fDefenseForItem = 0.0f;
            m_fDefenseForSkill = 0.0f;
            m_fDefenseForShuttle = 0.0f;
            m_fTransportTotal = 0.0f;
            m_fTransportCurrent = 0.0f;

            // 2007-07-27 by dgwoo 세력 선택을 한 유저인가?
            if(FALSE == IS_NORMAL_INFLUENCE_TYPE(m_myShuttleInfo.InfluenceType))
            {
                g_pD3dApp->m_bInfregular = TRUE;
            }
            else
            {
                g_pD3dApp->m_bInfregular = FALSE;
            }

            // 방어력 적용
            // 착용 아이템에 따른 방어력
            for(int i=0;i<POS_INVALID_POSITION;i++)
            {
                CItemInfo* pItemInfo = m_pStoreData->FindItemInInventoryByWindowPos(i);
                if( pItemInfo && 
                    pItemInfo->Kind == ITEMKIND_DEFENSE &&
                    pItemInfo->Wear == WEAR_ATTACHED)
                {
                    ITEM* pITEM = pItemInfo->GetRealItemInfo();
                    m_fDefenseForItem += pITEM->AbilityMax;
                    m_fTransportCurrent += pITEM->Weight;
                }
            }
//            it = m_pStoreData->m_mapItemInInventory.begin();
//            while(it != m_pStoreData->m_mapItemInInventory.end())
//            {
//                ITEM *item = g_pDatabase->GetServerItemInfo(it->second->ItemNum);
//                if(item)
//                {
//                    if(it->second->Kind == ITEMKIND_DEFENSE && it->second->Wear == WEAR_ATTACHED)
//                    {
//                        m_fDefenseForItem += item->AbilityMax;
//                    }
//                    // 현재 기체에 실려 있는 총중량->Energy의 경우 무게 적용 고려해야함
//                    m_fTransportCurrent += item->Weight;
//                }
//                it++;
//            }
            // 사용중인(적용중) 마인드 컨트롤에 따른 방어력

            // 기체의 능력에 따른 방어력
            m_fDefenseForShuttle = AtumSJ::GetDefenseC(&m_myShuttleInfo, &m_paramFactor, TRUE);
            // 총방어력 적용
            m_fDefenseTotal = m_fDefenseForShuttle + m_fDefenseForItem + m_fDefenseForSkill;
            
            // 중량 적용
            // 기체의 능력에 따른 총가능중량
            m_fTransportTotal = AtumSJ::GetTransport(&m_myShuttleInfo);
        }
        break;
    case 1:
        {// 착용한 아이템에 따른 방어력 적용
            m_fDefenseTotal -= m_fDefenseForItem;
            m_fDefenseForItem = 0.0f;
            for(int i=0;i<POS_INVALID_POSITION;i++)
            {
                CItemInfo* pItemInfo = m_pStoreData->FindItemInInventoryByWindowPos(i);
                if( pItemInfo && 
                    pItemInfo->Kind == ITEMKIND_DEFENSE &&
                    pItemInfo->Wear == WEAR_ATTACHED)
                {
                    ITEM* pITEM = pItemInfo->GetRealItemInfo();
                    m_fDefenseForItem += pITEM->AbilityMax;
                }
            }
//            it = m_pStoreData->m_mapItemInInventory.begin();
//            while(it != m_pStoreData->m_mapItemInInventory.end())
//            {
//                ITEM *item = g_pDatabase->GetServerItemInfo(it->second->ItemNum);
//                if(item)
//                {
//                    if(it->second->Kind == ITEMKIND_DEFENSE && it->second->Wear == WEAR_ATTACHED)
//                    {
//                        m_fDefenseForItem += item->AbilityMax;
//                    }
//                }
//                it++;
//            }
            m_fDefenseTotal += m_fDefenseForItem;
        }
        break;
    case 2:
        {// 사용중인(적용중) 마인드 컨트롤에 따른 방어력 적용
            m_fDefenseTotal -= m_fDefenseForSkill;
            m_fDefenseTotal += m_fDefenseForSkill;
        }
        break;
    case 3:
        {// 기체 능력에 따른 방어력 적용
            m_fDefenseTotal -= m_fDefenseForShuttle;
//            m_fDefenseForShuttle = g_pAtumSJ->GetMaxDefense(&m_myShuttleInfo);
            m_fDefenseForShuttle = AtumSJ::GetDefenseC(&m_myShuttleInfo, &m_paramFactor, TRUE);
            m_fDefenseTotal += m_fDefenseForShuttle;
        }
        break;
    case 4:
        {// 총가능중량 적용
            m_fTransportTotal = AtumSJ::GetTransport(&m_myShuttleInfo);
        }
        break;
    case 5:
        {// 현재 소유한 아이템 총중량
            m_fTransportCurrent = 0.0f;
            m_fTransportCurrent = m_pStoreData->GetTotalWeight();
//            it = m_pStoreData->m_mapItemInInventory.begin();
//            while(it != m_pStoreData->m_mapItemInInventory.end())
//            {
//                ITEM *item = g_pDatabase->GetServerItemInfo(it->second->ItemNum);
//                if(item)
//                {
//                    // 현재 기체에 실려 있는 총중량->Energy의 경우 무게 적용 고려해야함
//                    m_fTransportCurrent += item->Weight;
//                }
//                it++;
//            }
        }
        break;
    }
}

void CShuttleChild::CheckEventType()
{
    FLOG( "CShuttleChild::CheckEventType()" );
    // 이벤트 지역 체크 - Shop
//    if(m_bPKMode)
//        return;
    if(IsPkState(PK_PVP) || IsPkState(PK_FVF))    // 1:1결투거나 편대전인 경우에는 이벤트를 받지 않는다.
        return;
    int nCheckX = ((int)m_vPos.x)/TILE_SIZE;
    int nCheckZ = ((int)m_vPos.z)/TILE_SIZE;
    if(m_myShuttleInfo.Level < 6)
    {
        if(!g_pGround->m_pTileInfo[m_ptOldPoint.x*g_pGround->m_projectInfo.sYSize + m_ptOldPoint.y].bEnableLand &&
            g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEnableLand &&
            m_fLandingDist < 180.0f)
        {
//            g_pGameMain->m_pNotice->SetNotice("현재 착륙장 상공입니다. 착륙키는 'C'키입니다.");
            g_pGameMain->SetNotice( 15 );
        }
    }

    if((g_pGround->m_pTileInfo[m_ptOldPoint.x*g_pGround->m_projectInfo.sYSize + m_ptOldPoint.y].bEventType 
        != g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType)
        && m_vPos.y < g_pGround->m_projectInfo.fHeightMax - 20.0f)
    {
        switch(g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType)
        {
        case EVENT_TYPE_WARP:
            {
                MSG_FC_MOVE sMsg;
                memset(&sMsg,0x00,sizeof(sMsg));
                char buffer[SIZE_MAX_PACKET];
                sMsg.ClientIndex = m_myShuttleInfo.ClientIndex;
                sMsg.TimeGap = g_pD3dApp->m_dwSpeedHackTimer;
                sMsg.PositionVector = m_vPos;
                sMsg.TargetVector = m_vVel*1000.0f;
                D3DXVECTOR3 vUp;
                vUp.x = m_mMatrix._21;
                vUp.y = m_mMatrix._22;
                vUp.z = m_mMatrix._23;
                sMsg.UpVector = vUp*1000.0f;
                int nType = T_FC_MOVE;
                memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
                memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
                g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
                g_pD3dApp->m_vClientOldPos = m_vPos;
                g_pD3dApp->m_dwSpeedHackTimer  = 0;
                
                if(m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
                {
                    m_pClientParty->FSendPartyRequestPartyWarp();
                }
                else
                {
                    // 워프를 요청
                    if(!m_pClientParty->IsFormationFlightMove())
                    {
                        MSG_FC_EVENT_REQUEST_WARP sMsg2;
                        memset(&sMsg2,0x00,sizeof(sMsg2));
                        memset(buffer,0x00,SIZE_MAX_PACKET);
                        sMsg2.ClientIndex = m_myShuttleInfo.ClientIndex;
                        nType = T_FC_EVENT_REQUEST_WARP;
                        memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
                        memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg2, sizeof(sMsg2));
                        g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg2));
                        DBGOUT("Request Warp : MSG_FC_EVENT_REQUEST_WARP\n");
                    }
                }                
            }
            break;
        case EVENT_TYPE_WARP_TARGET:
            {// 클라이언트 현재 사용안함
            }
            break;
        case EVENT_TYPE_ENTER_BUILDING:
            {// 상점 Object
                //SendPacket(T_FC_MOVE_LANDING_DONE);
//                m_bEventReady = TRUE;
            }
            break;
        case EVENT_TYPE_LANDING:
            {// 착륙 Object
            }
            break;
        case EVENT_TYPE_RACING_CHECK_POTINT:
            {// 레이싱 체크 포인트
            }
            break;
        case EVENT_TYPE_OBJECT_MONSTER:
            {// 오브젝트 몬스터 - 아무것도 안함.
            }
            break;
        case EVENT_TYPE_PATTERN_POINT:
            {// 캐릭터 워프시 패턴의 시작/끝
            }
            break;
        case EVENT_TYPE_LANDING_PATTERN_START:
            {// 패턴타입 상점 착륙 시작 
            }
            break;
        case EVENT_TYPE_LANDING_PATTERN_END:
            {// 패턴타입 상점 착륙 끝
            }
            break;
        case EVENT_TYPE_TAKEOFF_PATTERN_START:
            {// 패턴타입 상점 이륙 시작
            }
            break;
        case EVENT_TYPE_TAKEOFF_PATTERN_END:
            {// 패턴타입 상점 이륙 끝
            }
            break;
        }
    }
}
BOOL CShuttleChild::ISSetAutoPos()
{
    FLOG( "CShuttleChild::ISSetAutoPos()" );
    switch(m_myShuttleInfo.UnitKind)
    {
    case UNITKIND_BT01:
    case UNITKIND_BT02:
    case UNITKIND_BT03:
    case UNITKIND_BT04:
        {
            if(!m_nSideRotateDir && 
                (m_nFBDir && m_nBooster == BOOSTER_TYPE_MIN) && 
                !m_nLRDir)
                return TRUE;
        }
        break;
    case UNITKIND_OT01:
    case UNITKIND_OT02:
    case UNITKIND_OT03:
    case UNITKIND_OT04:
        {
            if(!m_nSideRotateDir && !m_nFBDir && !m_nLRDir)
                return TRUE;
        }
        break;
    case UNITKIND_DT01:
    case UNITKIND_DT02:
    case UNITKIND_DT03:
    case UNITKIND_DT04:
        {
            if(!m_nSideRotateDir && !m_nFBDir && !m_nLRDir)
                return TRUE;
        }
        break;
    case UNITKIND_ST01:
    case UNITKIND_ST02:
    case UNITKIND_ST03:
    case UNITKIND_ST04:
        {
            if(!m_nSideRotateDir && 
                (m_nFBDir && m_nBooster == BOOSTER_TYPE_MIN) && 
                !m_nLRDir)
                return TRUE;
        }
        break;
    }
    return FALSE;
}

void CShuttleChild::SetWeaponData(BYTE bType,BOOL bReset)//,BOOL bWear)
{
    FLOG( "CShuttleChild::SetWeaponData(BYTE bType,BOOL bReset)" );
    byte bWeaponType = 0;
    D3DXVECTOR3 vTempPos = D3DXVECTOR3(0,0,0);
    InitInvenMesh();                    // 2006-04-04 by ispark
    if(bType == 0) // 1형
    {
        if(bReset)
        {
            DeleteWearItem( WEAR_ITEM_KIND_WEAPON_11 );
            DeleteWearItem( WEAR_ITEM_KIND_WEAPON_12 );
            
            // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
            // A기어 포대이펙트도 지운다
            DeleteRobotAGearWeapon();
            // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

            if(g_pGameMain)
            {
                g_pGameMain->m_pWeaponInfo->ChangeWeapon(1);
            }
            if(m_pPrimaryWeapon == NULL)
            {
                // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
                //CreatePrimaryWeaponItem(NULL);
                CreatePrimaryWeaponItem( NULL, NULL );
                // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
            }
            else
            {

                // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
                //CreatePrimaryWeaponItem(m_pPrimaryWeapon->GetRealItemInfo());

                ITEM*    pShapeItem    = g_pDatabase->GetServerItemInfo( m_pPrimaryWeapon->GetItemGeneral()->GetShapeItemNum() ) ;
                CreatePrimaryWeaponItem( m_pPrimaryWeapon->GetRealItemInfo(), pShapeItem );

                // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

                if(IS_PRIMARY_WEAPON_1(m_pPrimaryWeapon->GetRealItemInfo()->Kind))
                {
                    if(m_pPrimaryWeapon->GetItemGeneral()->CurrentCount <= 0)
                    {
                        ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD);
                    }
                }
                else if (IS_PRIMARY_WEAPON_2(m_pPrimaryWeapon->GetRealItemInfo()->Kind))
                {
                    if(m_myShuttleInfo.CurrentEP <= 0)
                    {
                        ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD);
                    }
                }
            }
        }
        else
        {
            if(m_pPrimaryWeapon)
            {
                if(IS_PRIMARY_WEAPON_1(m_pPrimaryWeapon->GetRealItemInfo()->Kind))
                {
                    if(m_pPrimaryWeapon->GetItemGeneral()->CurrentCount <= 0)
                    {
                        ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD);
                    }
                }
                else if(IS_PRIMARY_WEAPON_2(m_pPrimaryWeapon->GetRealItemInfo()->Kind))
                {
                    if(m_myShuttleInfo.CurrentEP <= 0)
                    {
                        ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD);
                    }
                }
                // 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가
                else
                {
                    ResetPetWeaponBodyCondition(WEAPON_BODYCON_RELOAD);                    
                }
                // end 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가

            }
        }
    }
    else if(bType == 1) // 2형
    {
        if(bReset)
        {
            DeleteWearItem( WEAR_ITEM_KIND_WEAPON_21 );
            DeleteWearItem( WEAR_ITEM_KIND_WEAPON_22 );
            if(g_pGameMain)
            {
                g_pGameMain->m_pWeaponInfo->ChangeWeapon(2);
            }
            if(m_pSecondaryWeapon == NULL)
            {
                // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
                //CreateSecondaryWeaponItem(NULL);
                CreateSecondaryWeaponItem( NULL, NULL );
                // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
            }
            else
            {
                // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
                //CreateSecondaryWeaponItem(m_pSecondaryWeapon->GetRealItemInfo());
                ITEM*    pShapeItem    = g_pDatabase->GetServerItemInfo( m_pSecondaryWeapon->GetItemGeneral()->GetShapeItemNum() ) ;
                
                CreateSecondaryWeaponItem( m_pSecondaryWeapon->GetRealItemInfo(), pShapeItem );
                // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

                if(m_pSecondaryWeapon->GetItemGeneral()->CurrentCount <= 0)
                {
                    ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD);
                    ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD, FALSE);
                }
            }
        }
        else
        {
            if(m_pSecondaryWeapon)
            {
                if(m_pSecondaryWeapon->GetItemGeneral()->CurrentCount <= 0)
                {
                    ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD);
                    ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD, FALSE);
                }
            }
        }
    }
    float fRange1 , fRange2;
    fRange1 = fRange2 = 0;
    if(m_pPrimaryWeapon)
        fRange1 = m_pPrimaryWeapon->GetRealItemInfo()->Range;
    if(m_pSecondaryWeapon)
        fRange2 = m_pSecondaryWeapon->GetRealItemInfo()->Range;
    m_fWeaponMaxDistance = max(fRange1,fRange2);
}

void CShuttleChild::SetShuttleFirstPosition(BOOL bMode)
{
    FLOG( "CShuttleChild::SetShuttleFirstPosition(BOOL bMode)" );
    float fElapsedTime = g_pD3dApp->GetElapsedTime();
    float fTempHeight1 = DEFAULT_COLLISION_DISTANCE;
    D3DXVECTOR3 vPos = m_vPos;
//    if(m_bOnObject || bMode)
//    {
        D3DXMATRIX matTemp;
        D3DXVECTOR3 vSide;
        vSide = D3DXVECTOR3(0,0,1);
        D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos - m_vBasisUp),&vSide);
        if(g_pScene->m_pObjectRender)
            fTempHeight1 = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos, FALSE).fDist;
        D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos + m_vBasisUp),&vSide);
        float fTempCheck = DEFAULT_COLLISION_DISTANCE;
        if(g_pScene->m_pObjectRender)
            fTempCheck = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos, FALSE).fDist;
        if(fTempHeight1 != DEFAULT_COLLISION_DISTANCE && fTempCheck != DEFAULT_COLLISION_DISTANCE)
        {
            if(vPos.y - fTempHeight1 < vPos.y + fTempCheck)
                fTempHeight1 = vPos.y + fTempCheck;
            else
                fTempHeight1 = vPos.y - fTempHeight1;
        }
        else
        {
            fTempHeight1 = vPos.y - fTempHeight1;
        }
//    }
    float fTempHeight2 = CheckMap(m_vPos);
    if(!bMode)
    {
        if(fTempHeight2 < fTempHeight1)
        {
            if(m_vPos.y > fTempHeight1 + m_fAmorHeight)
            {
                m_vPos.y -= (m_vPos.y - fTempHeight1 - m_fAmorHeight)*0.5f*fElapsedTime;
                if(m_vPos.y < fTempHeight1 + m_fAmorHeight)
                    m_vPos.y = fTempHeight1 + m_fAmorHeight;
            }
            else if(m_vPos.y < fTempHeight1 + m_fAmorHeight)
            {
                m_vPos.y -= (m_vPos.y - fTempHeight1 - m_fAmorHeight)*0.5f*fElapsedTime;
                if(m_vPos.y > fTempHeight1 + m_fAmorHeight)
                    m_vPos.y = fTempHeight1 + m_fAmorHeight;
            }
        }
        else// if(fTempHeight2 > fTempHeight1)
        {
            if(m_vPos.y > fTempHeight2 + m_fAmorHeight)
            {
                m_vPos.y -= (m_vPos.y - fTempHeight2 - m_fAmorHeight)*0.5f*fElapsedTime;
                if(m_vPos.y < fTempHeight2 + m_fAmorHeight)
                    m_vPos.y = fTempHeight2 + m_fAmorHeight;
            }
            else if(m_vPos.y < fTempHeight2 + m_fAmorHeight)
            {
                m_vPos.y -= (m_vPos.y - fTempHeight2 - m_fAmorHeight)*0.5f*fElapsedTime;
                if(m_vPos.y > fTempHeight2 + m_fAmorHeight)
                    m_vPos.y = fTempHeight2 + m_fAmorHeight;
            }
        }
    }
    else
    {
        if(fTempHeight2 < fTempHeight1)
        {
            if(m_vPos.y > fTempHeight1 + m_fAmorHeight)
            {
                m_vPos.y = fTempHeight1 + m_fAmorHeight;
            }
            else if(m_vPos.y < fTempHeight1 + m_fAmorHeight)
            {
                m_vPos.y = fTempHeight1 + m_fAmorHeight;
            }
        }
        else// if(fTempHeight2 > fTempHeight1)
        {
            if(m_vPos.y > fTempHeight2 + m_fAmorHeight)
            {
                m_vPos.y = fTempHeight2 + m_fAmorHeight;
            }
            else if(m_vPos.y < fTempHeight2 + m_fAmorHeight)
            {
                m_vPos.y = fTempHeight2 + m_fAmorHeight;
            }
        }
    }
//    if(m_dwState == _NORMAL)
//        m_vPos.y += m_fAmorHeight;// 지형보다 약간 높게 띄운다
//        m_vPos.y += 300.0f;// 지형보다 약간 높게 띄운다
    
}

void CShuttleChild::InitShuttleData(BOOL bState)
{
    FLOG( "CShuttleChild::InitShuttleData()" );
    DBGOUT("CShuttleChild::InitShuttleData()\n");
    D3DXMatrixIdentity(&m_mMatrix);
    m_bRButtonState = FALSE;
    m_bLButtonState = FALSE;
    m_bUpButtonState = FALSE;
    m_bDownButtonState = FALSE;
    m_bLeftButtonState = FALSE;
    m_bRightButtonState = FALSE;
    m_bMButtonState = FALSE;
    ChangeUnitState( _NORMAL );
    //m_dwState = _NORMAL;
    m_fShuttleSpeed = 0.0f;
    m_fLRVelRate = 0.0f;
    m_pTarget = NULL;
    m_pOrderTarget = NULL;
    m_bOnObject = TRUE;
    D3DXVECTOR3 vTempPos;
    vTempPos.x = g_pGround->m_projectInfo.sXSize*TILE_SIZE/2.0f+RAND100();
    vTempPos.y = m_vPos.y;
    vTempPos.z = g_pGround->m_projectInfo.sYSize*TILE_SIZE/2.0f+RAND100();
    D3DXVec3Normalize(&m_vVel,&(vTempPos-m_vPos));
    DBGOUT("ShuttleChild,m_vPos(%.2f, %.2f, %.2f)\n", m_vPos.x, m_vPos.y, m_vPos.z);
    DBGOUT("ShuttleChild,m_vVel(%.2f, %.2f, %.2f)\n", m_vVel.x, m_vVel.y, m_vVel.z);
    m_vUp = D3DXVECTOR3(0,1,0);
    m_vBasisUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXVec3Normalize(&m_vVel,&m_vVel);
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);          
    m_vWeaponVel = m_vVel;

    // 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태
    m_vPetLeftVel = m_vVel;
    m_vPetRightVel = m_vVel;
    // end 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태
    m_bIsCameraPattern = FALSE;
    m_bMouseMoveLock = FALSE;
    m_nMissileCount = 0;                                            // 2005-08-10 by ispark
    m_bMissileWarning = FALSE;
    m_bSkillMissileWarning = FALSE;
    m_bSkillGoMove            = TRUE;
    m_bWarpLink = FALSE;
    m_bLandingField = FALSE;
    m_bKeyDownA = FALSE;                                            // 2005-10-13 by ispark
    m_bKeyDownD = FALSE;
    m_bRollUsed = FALSE;
    m_bUnitStop = FALSE;
    m_bLandedMove = FALSE;
    m_bPenaltyUnitStop = FALSE;
    m_fCurrentSideRate = 0.0f;
    m_fCurrentAngle = 0.0f;
    
    // 2005-08-02 by ispark
    // 기본 부스터는 MIN이다.
    m_nBooster = BOOSTER_TYPE_MIN;
    
    if(g_pScene->m_byMapType == MAP_TYPE_TUTORIAL)
    {
        m_vPos = D3DXVECTOR3(2702,178,516.5);
        m_vVel = D3DXVECTOR3(1,0,0);
    }
    if(g_pScene->m_byMapType == MAP_TYPE_CITY)
    {
        m_nBooster = BOOSTER_TYPE_STOP;
        m_vPos = MAP_TYPE_CITY_UNIT_POS;
        m_myShuttleInfo.PositionVector = MAP_TYPE_CITY_UNIT_POS;
    }
    else
    {
// 2005-08-03 by ispark, 예전 2001번 맵 처리 사항
//        if(m_nOldMapIndex == 1001 && m_myShuttleInfo.MapChannelIndex.MapIndex == 2001)
//        {
//            m_vVel.y = 0;
//            m_vPos.y = 400.0f;
//            if(m_vPos.x/TILE_SIZE > 120.0f)
//            {
//                m_vVel.x = 0.7f;
//            }
//            else
//            {
//                m_vVel.x = -0.7f;
//            }
//            if(m_vPos.z/TILE_SIZE > 120.0f)
//            {
//                m_vVel.z = 0.7f;
//            }
//            else
//            {
//                m_vVel.z = -0.7f;
//            }
//        }
//        else if(m_nOldMapIndex != 0 && m_myShuttleInfo.MapChannelIndex.MapIndex == 2001)
//        {
//            m_vPos.y = 400.0f;
//        }
        // 2005-08-02 by ispark
        // 부스터를 끄는 원인이 여기다.
        // 워프로 들어오는 거라면 상관이 없다. 워프에서 수정이 된다.
        // 그러나, 그외는 안쨈? 2001번으로 들어올때 그렇다.
//        if(IS_BT(m_myShuttleInfo.UnitKind) || IS_ST(m_myShuttleInfo.UnitKind))
//        {
//            m_nShuttleStopState = 3;
//            m_nBooster = BOOSTER_TYPE_AIRBREAK;
//        }
//        else if(IS_DT(m_myShuttleInfo.UnitKind))
        if(IS_DT(m_myShuttleInfo.UnitKind))
        {
            m_bIsAir = TRUE;//시작시 가라앉지 않게..// 2004-06-22 by dhkwon
        }
    }
    m_vNextPos = m_vPos;
//    m_fOldUpRate = 0;
//    m_fSideRate = 0;
    m_fSideCheckRate = 0;

    D3DXMatrixLookAtRH( &m_mMatrix, &m_vPos, &(m_vPos + m_vVel), &m_vUp);
    D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
    g_pD3dApp->SetCamPosInit();
    g_pD3dApp->m_bFixUnitDirMode = FALSE;

    // 최초 시작 지점 체크
    int x = ((int)m_vPos.x)/TILE_SIZE;
    int z = ((int)m_vPos.z)/TILE_SIZE;
    // 2005-08-03 by ispark
    m_ptOldPoint.x = x;
    m_ptOldPoint.y = z;

    // 2005-08-22 by ispark
    // 들어왔을 때 착륙지점에서 착륙 못하게 막음. 정상적인 착륙이 아니면 날게 한다.
//    if(g_pGround->m_pTileInfo[x*g_pGround->m_projectInfo.sYSize + z].bEnableLand && bState == FALSE)
//    {
////        SendPacket(T_FC_MOVE_LANDING);
//        // 2004-11-04 by jschoi
//        // 최초 시작 지점 체크에서 현재 위치가 착륙지점이라면 OnObject인지 검사를 위해 m_vPos 를 m_fAmorHeight 만큼 올려준다.
//        m_vPos.y += m_fAmorHeight;
//        if(SetOrderMoveTargetPos() == TRUE )
//        {
//            m_vPos = m_vTargetPos;
//            m_vNextPos = m_vTargetPos;
//            //m_dwState = _LANDED;
//            ChangeUnitState( _LANDED );
//            m_bIsAir = FALSE;
////            SendPacket(T_FC_MOVE);
////            g_pD3dApp->SendMovePacket(FALSE);
//            g_fSendMovePacket = max(g_fSendMovePacket,5.0f);
//        }
//    }
//    else
//    {
//        //m_vPos.y += 300.0f;// 지형보다 약간 높게 띄운다, 왜 올릴까?
//    }
    // 착륙전 시점이 1인칭이였으면 다시 1인칭으로 변환 
    if(m_bLandingOldCamtypeFPS)
    {
        g_pD3dApp->m_pCamera->ChangeCamType();
        m_bLandingOldCamtypeFPS = FALSE;
    }
    
    SetCursorInit();
    
    m_bDegree = 2;// 2004-05-06 by dhkwon
    
//    SetShuttleFirstPosition(TRUE);
    InitEffect();
//    g_pScene->SetDay();
    InitItemPoint();
    InitRolling();

    // 편대대형 초기화
    m_pClientParty->FormationFlightClear();
    SetShuttleSpeed();
    
    m_byCityWarTeamType = CITYWAR_TEAM_TYPE_NORMAL;

    // 2005-10-06 by ispark
    // 튜토리얼 모드라면 창 초기화 실행 안함
    if(g_pInterface->m_pGameMain)
    {
        g_pInterface->m_pGameMain->InitShowWindow();            // 2005-08-05 by ispark
    }

    // 2008-06-20 by bhsohn EP3 옵션관련 처리
    {
        //g_pInterface->m_bShowInterface = TRUE;                        // 2005-08-30 by ispark
        //BOOL bShowInter = !g_pD3dApp->IsOptionEtc(OPTION_RADIO_INTERFACE_HIDE);
        //g_pInterface->SetShowInterface(bShowInter);        
    }
    // end 2008-06-20 by bhsohn EP3 옵션관련 처리
    if(g_pInterface->m_pCityBase)
    {
        g_pInterface->m_pCityBase->CloseCurrentEnterBuilding();    // 2005-11-11 by ispark 사용중이던 상점 닫기
    }

    // 핵 검사를 위한 좌표 초기화
    g_pD3dApp->m_vShuttleOldPos = m_vPos;

    m_fCheckDead = 0.0f;
    m_bKeyBlock = FALSE;

    // 2006-11-14 by ispark, 우주맵에서 죽으면 수평 유지가 사라지는 버그 수정
    m_bCollMap = FALSE;
}

void CShuttleChild::CheckClientQuestList()
{
    FLOG( "CShuttleChild::CheckClientQuestList()" );
    static float fMonsterMoveCheckTime = 0.2f;
    static float fDegreeCheckTime = 0.0f; // 퀘스트 시작시 부터 완료시까지 타임을 누적하여 점수를 구할때 쓴다.
    float fElapsedTime = g_pD3dApp->GetElapsedTime();
    int cont = 0;
    if(fMonsterMoveCheckTime > 0.0f)
        fMonsterMoveCheckTime -= fElapsedTime;
    map<int, MEX_QUEST_INFO*>::iterator itQuest = g_pQuestData->m_mapCharacterQuest.begin();
    while(itQuest != g_pQuestData->m_mapCharacterQuest.end())
    {
        int nState = itQuest->second->QuestState;
        if(itQuest->second->QuestIndex == TUTORIAL_MISSION_1 || itQuest->second->QuestIndex == TUTORIAL_MISSION_2)
        {
            //CMapObjectIterator itObj = g_pScene->m_mapObjectList.begin();
            vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
            while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
            {
                (*itObj)->m_fCurrentTime = 0.0f;
                itObj++;
            }
        }
        if(nState == QUEST_STATE_IN_PROGRESS)
        {
            if(itQuest->second->QuestIndex == TUTORIAL_MISSION_1)
            {// 설명을 듣는 퀘스트 - 이동안 셔틀은 움직이지 않는다.
                m_bOrderMove = TRUE;
                return;
            }
            else if(itQuest->second->QuestIndex == TUTORIAL_MISSION_2)
            {
                if(m_bOrderMove)
                {// 강제 이동 체크
                    if(MoveOrder(m_bOrderMoveType))
                        m_bOrderMove = FALSE;
                }
                if(m_bBeginnerQuestDegree == 1)
                {
                    fDegreeCheckTime = 0.0f;
                }
                else
                {
                    fDegreeCheckTime += fElapsedTime;
                }
                // 1단계 몬스터 소환
/*                m_nMonsterCount = 0;
                CMapMonsterIterator itMon = g_pScene->m_mapMonsterList.begin();
                while(itMon != g_pScene->m_mapMonsterList.end())
                {
                    if(itMon->second->m_dwState == _NORMAL)
                    {
                        m_nMonsterCount++;
                    }
                    itMon++;
                }
                if(!m_nMonsterCount)
*/                if(g_pScene->m_mapMonsterList.empty())
                {// bQuestDegree 단계 몬스터 소환
                    if(m_bBeginnerQuestDegree < 6)
                    {
                        vector<MONSTERINFO *>::iterator itResMon = g_pGround->m_vecMonsterResList.begin();
                        while(itResMon != g_pGround->m_vecMonsterResList.end())
                        {
                            MONSTERINFO * pData = *itResMon;
                            if(pData->sMaxMon == m_bBeginnerQuestDegree)
                            {
                                MSG_FC_CHARACTER_GET_MONSTER_INFO_OK info;
                                info.MonsterIndex = 10000+cont+1;
                                info.MonsterUnitKind = pData->nMonType;
                                info.BodyCondition = BODYCON_CREATION_MASK;
                                
                                info.PositionVector.x = pData->sStartx*40.0f + RANDI(0, 39);
                                info.PositionVector.y = pData->sResTime;
                                info.PositionVector.z = pData->sStartz*40.0f + RANDI(0, 39);
                                info.CurrentHP = 45;            // 몬스터의 CurrentHP는 SHORT
                                CMonsterData * pMon = new CMonsterData(&info);
                                if(RANDBOOL())
                                    pMon->m_nSideDir = 1;
                                else
                                    pMon->m_nSideDir = -1;
                                g_pScene->InsertToBlockData(pMon);
                                if(pMon->m_pCharacterInfo)
                                {
                                    pMon->m_pCharacterInfo->SetBodyCondition(info.BodyCondition);
                                }
                                memcpy(&pMon->m_ResInfo,pData,sizeof(MONSTERINFO));
                                g_pScene->m_mapMonsterList[info.MonsterIndex] = pMon;
                                cont++;
                            }
                            itResMon++;
                        }
                        if(cont)
                        {
                            char buf[256];
                            wsprintf(buf,STRMSG_C_MONSTER_0001,cont);//"%d기의 비트가 소환 되었습니다"
                            g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);
                            if(m_bBeginnerQuestDegree == 1)
                            {
//                                g_pGameMain->SetNotice("목표물의 조준은 마우스를 목표물에 가져다 되면 됩니다.");
                            }
                            else if(m_bBeginnerQuestDegree == 2)
                            {
//                                g_pGameMain->SetNotice("기체 회전은 마우스 이동으로 가능하며 공격은 마우스 왼쪽 버튼 입니다.");
                            }
                        }
                        m_bBeginnerQuestDegree++;
                    }
                    else
                    {// 퀘스트 완료 
                        MSG_FC_QUEST_REQUEST_SUCCESS sMsg;
                        memset(&sMsg,0x00,sizeof(sMsg));
                        char buffer[SIZE_MAX_PACKET];
                        sMsg.QuestIndex = TUTORIAL_MISSION_2;
                        if(fDegreeCheckTime <= 20.0f)
                        {// A  index : 10
                            sMsg.QuestResult = 10;
                        }
                        else if(fDegreeCheckTime <= 40.0f)
                        {// B  index : 11
                            sMsg.QuestResult = 11;
                        }
                        else if(fDegreeCheckTime <= 80.0f)
                        {// C  index : 12
                            sMsg.QuestResult = 12;
                        }
                        else
                        {// D  index : 13
                            sMsg.QuestResult = 13;
                        }
                        int nType = T_FC_QUEST_REQUEST_SUCCESS;
                        memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
                        memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
                        g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
                        char buf[256];
                        wsprintf(buf,STRMSG_C_MONSTER_0002);//"비트 소탕 완료"
                        g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SKILL_USE);
                        m_bOrderMove = TRUE;    // 강제 이동에 들어간다.
                        if(IS_DT(m_myShuttleInfo.UnitKind))
                        {
//                            m_vTargetPos = D3DXVECTOR3(2690,10,507);
                            m_bIsAir = TRUE;
                        }
//                        else
                            m_vTargetPos = D3DXVECTOR3(2702,178,516.5);
                        m_vTargetVel = D3DXVECTOR3(1,0,0);
                        m_bOrderMoveType = 0;
                        itQuest->second->QuestState = QUEST_STATE_COMPLETED;
//                        g_pGameMain->SetNotice("착륙은 숫자판의 'C'키를 누르면 됩니다.");
                    }
                }
                else
                {
                    if(fMonsterMoveCheckTime <= 0.0f)
                    {
                        map<INT,CMonsterData *>::iterator itMonster = g_pScene->m_mapMonsterList.begin();
                        while(itMonster != g_pScene->m_mapMonsterList.end())
                        {
                            if(!m_pOrderTarget && ATTACK_AVAILABLE_STATE(itMonster->second->m_dwState))
                            {
                                if(m_pOrderTarget != itMonster->second)
                                {
                                    m_pOrderTarget = itMonster->second;
                                    g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LOCKON_PRIMARY, g_pShuttleChild->m_vPos, FALSE);
                                }
                            }
                            CheckMonsterMove(itMonster->second);
                            itMonster++;
                        }
                        fMonsterMoveCheckTime = 0.2f;
                    }
                }
            }
            else if(itQuest->second->QuestIndex == TUTORIAL_MISSION_3)
            {
                if(m_bOrderMove)
                {// 강제 이동 체크
                    if(!MoveOrder(m_bOrderMoveType))
                        return;
                    m_bOrderMove = FALSE;
//                    g_pGameMain->SetNotice("키보드 'W'는 속도를 올리고 'S'는 속도를 내립니다.");
                }

                if(m_bBeginnerQuestDegree <= 6)
                {
                    m_bBeginnerQuestDegree = 6;
                }
                if(m_bBeginnerQuestDegree == 6)
                {
                    fDegreeCheckTime = 0.0f;
                }
                else
                {
                    fDegreeCheckTime += fElapsedTime;
                }
//                if(m_bBeginnerQuestDegree > 3)
//                {// 2단계 몬스터 소환
/*                    m_nMonsterCount = 0;
                    CMapMonsterIterator itMon = g_pScene->m_mapMonsterList.begin();
                    while(itMon != g_pScene->m_mapMonsterList.end())
                    {
                        if(itMon->second->m_dwState == _NORMAL)
                        {
                            m_nMonsterCount++;
                        }
                        itMon++;
                    }
                    if(!m_nMonsterCount)
*/                    if(g_pScene->m_mapMonsterList.empty())
                    {// bQuestDegree 단계 몬스터 소환
                        if(m_bBeginnerQuestDegree < 20)
                        {
                            vector<MONSTERINFO *>::iterator itResMon = g_pGround->m_vecMonsterResList.begin();
                            while(itResMon != g_pGround->m_vecMonsterResList.end())
                            {
                                MONSTERINFO * pData = *itResMon;
                                if(pData->sMaxMon == m_bBeginnerQuestDegree)
                                {
                                    MSG_FC_CHARACTER_GET_MONSTER_INFO_OK info;
                                    info.MonsterIndex = 10000+cont+1;
                                    info.MonsterUnitKind = pData->nMonType;
                                    info.BodyCondition = BODYCON_CREATION_MASK;
                                    
                                    info.PositionVector.x = pData->sStartx*40.0f + RANDI(0, 39);
                                    if(m_bBeginnerQuestDegree == 5)
                                        info.PositionVector.y = pData->sResTime + RAND100();
                                    else
                                        info.PositionVector.y = pData->sResTime;
                                    info.PositionVector.z = pData->sStartz*40.0f + RANDI(0, 39);
                                    info.CurrentHP = 45;            // 몬스터의 CurrentHP는 SHORT
                                    CMonsterData * pMon = new CMonsterData(&info);
                                    if(RANDBOOL())
                                        pMon->m_nSideDir = 1;
                                    else
                                        pMon->m_nSideDir = -1;
                                    g_pScene->InsertToBlockData(pMon);
                                    if(pMon->m_pCharacterInfo)
                                    {
                                        pMon->m_pCharacterInfo->SetBodyCondition(info.BodyCondition);
                                    }
                                    memcpy(&pMon->m_ResInfo,pData,sizeof(MONSTERINFO));
                                    g_pScene->m_mapMonsterList[info.MonsterIndex] = pMon;
                                    cont++;
                                }
                                itResMon++;
                            }
                            if(cont)
                            {
                                char buf[256];
                                wsprintf(buf,STRMSG_C_MONSTER_0001,cont);//"%d기의 비트가 소환 되었습니다"
                                g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SYSTEM);
                                if(m_bBeginnerQuestDegree == 7)
                                {
//                                    g_pGameMain->SetNotice("아이템 사용 단축키는 1~8 번입니다.");
                                }
                            }
                            m_bBeginnerQuestDegree++;
                        }
                        else
                        {// 퀘스트 완료 
                            MSG_FC_QUEST_REQUEST_SUCCESS sMsg;
                            memset(&sMsg,0x00,sizeof(sMsg));
                            char buffer[SIZE_MAX_PACKET];
                            sMsg.QuestIndex = TUTORIAL_MISSION_3;
                            if(fDegreeCheckTime <= 60.0f)
                            {// A  index : 10
                                sMsg.QuestResult = 10;
                            }
                            else if(fDegreeCheckTime <= 90.0f)
                            {// B  index : 11
                                sMsg.QuestResult = 11;
                            }
                            else if(fDegreeCheckTime <= 120.0f)
                            {// C  index : 12
                                sMsg.QuestResult = 12;
                            }
                            else
                            {// D  index : 13
                                sMsg.QuestResult = 13;
                            }
                            int nType = T_FC_QUEST_REQUEST_SUCCESS;
                            memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
                            memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
                            g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
                            char buf[256];
                            wsprintf(buf,STRMSG_C_MONSTER_0002);//"비트 소탕 완료"
                            g_pD3dApp->m_pChat->CreateChatChild(buf, COLOR_SKILL_USE);
                            itQuest->second->QuestState = QUEST_STATE_COMPLETED;
                        }
                    }
                    else
                    {
                        if(g_pScene->m_mapMonsterList.size() == 3 && m_bBeginnerQuestDegree == 7)
                        {
//                            g_pGameMain->SetNotice("전방에 있는 목표물만 마우스를 이용하여 조준이 가능합니다.");
                        }
                        if(g_pScene->m_mapMonsterList.size() == 1 && m_bBeginnerQuestDegree == 7)
                        {
//                            g_pGameMain->SetNotice("기체의 제자리에서 좌우 이동은 키보드 'A' , 'D' 버튼입니다.");
                        }
                        if(g_pScene->m_mapMonsterList.size() == 3 && m_bBeginnerQuestDegree == 8)
                        {
//                            g_pGameMain->SetNotice("목표물을 변경하고 싶을때는 Tab 버튼을 누르시기 바랍니다.");
                        }
                        if(fMonsterMoveCheckTime <= 0.0f)
                        {
                            map<INT,CMonsterData *>::iterator itMonster = g_pScene->m_mapMonsterList.begin();
                            while(itMonster != g_pScene->m_mapMonsterList.end())
                            {
                                if(!m_pOrderTarget && ATTACK_AVAILABLE_STATE(itMonster->second->m_dwState))
                                {
                                    if(m_pOrderTarget != itMonster->second)
                                    {
                                        m_pOrderTarget = itMonster->second;
                                        g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LOCKON_PRIMARY, g_pShuttleChild->m_vPos, FALSE);
                                    }
                                }
                                CheckMonsterMove(itMonster->second);
                                itMonster++;
                            }
                            fMonsterMoveCheckTime = 0.2f;
                        }
                    }
//                    CMapObjectIterator itObj = g_pScene->m_mapObjectList.begin();
                    vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorCollisionObjectPtrList.begin());
                    while(itObj != g_pScene->m_vectorCollisionObjectPtrList.end())
                    {
                        if((*itObj)->m_fCurrentTime > 450.0f)
                            (*itObj)->m_fCurrentTime = 450.0f;
                        itObj++;
                    }
//                }
            }
            break;
        }
        itQuest++;
    }
}

void CShuttleChild::SendFieldSocketRequestEventObjectWarpIn(CObjectChild * pObj)
{
    if(m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
    {
        m_pClientParty->FSendPartyRequestPartyObjectEvent(pObj);
//        char buffer[SIZE_MAX_PACKET];
//        MSG_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT sMsg;
//        memset(&sMsg,0x00,sizeof(sMsg));
//        memset(buffer,0x00,SIZE_MAX_PACKET);
//        sMsg.ObjectType = pObj->m_nCode;
//        sMsg.ObjectPosition = pObj->m_vPos;
//        int nType = T_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT;
//        memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//        int cont = 0;
//        for(int i=0;i<MAX_PARTY_NUMBER - 1;i++)
//        {
//            if(m_nCharactUniqueNumber[i])
//            {
//                memcpy(buffer+SIZE_FIELD_TYPE_HEADER+sizeof(sMsg)+(cont)*sizeof(UINT), &m_nCharactUniqueNumber[i], sizeof(UINT));
//                cont++;
//            }
//        }
//        sMsg.nPartyMembers = cont;
//        memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//        g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg) + cont*sizeof(UINT));
    }
    else
    {
//        if(!m_infoParty.bFormationFlyingPositionType)
        if(!m_pClientParty->IsFormationFlightMove())
        {
            MSG_FC_EVENT_REQUEST_OBJECT_EVENT sMsg;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
// 2011-02-08 by jhahn EP4 트리거 시스템
//             if ((pObj->m_nCode/10000000) == 9)
//             {
//                 pObj->m_nCode =  pObj->m_nCode - 90000000;
//             }
//end 2011-02-08 by jhahn EP4 트리거 시스템
#endif
            sMsg.ObjectType = pObj->m_nCode;
            sMsg.ObjectPosition = pObj->m_vPos;
            g_pFieldWinSocket->SendMsg( T_FC_EVENT_REQUEST_OBJECT_EVENT, (char*)&sMsg, sizeof(sMsg) );
            DBGOUT("Request Warp : MSG_FC_EVENT_REQUEST_OBJECT_EVENT(5974)\n");
        }
    }

}

void CShuttleChild::CheckEventTypeObject()
{
    FLOG( "CShuttleChild::CheckEventTypeObject()" );
    if(IsPkState(PK_PVP) || IsPkState(PK_FVF))    // 1:1결투거나 편대전인 경우에는 이벤트를 받지 않는다.
        return;
    if(g_pScene->m_pObjectRender)
    {
//        CObjectChild * pObj = (CObjectChild *)g_pScene->m_pObjectRender->CheckCollMeshEvent();
        m_pCurrentObjEvent = (CObjectChild *)g_pScene->m_pObjectRender->CheckCollMeshEvent(500.0f);
        
        if(!m_pCurrentObjEvent)
            return;
        else if(m_bSkillAirMove)
        {
            DeletePattern();
        }
        switch(m_pCurrentObjEvent->m_bEventType)
        {
        case EVENT_TYPE_WARP:
            {// 워프지역(입구)
                if(m_nCurrentPatternNumber == PATTERN_UNIT_WARP_IN)    // 워프인 패턴 중에만 워프가 가능하다.
                {
                    DeletePattern();
                    m_nEventType = EVENT_WARP_IN;
//                    m_bEventReady = TRUE;
//                    m_fEventCheckTime = 2.0f;
                    ChangeUnitState( _WARP );
                    if(g_pD3dApp->m_pEffectList)
                    {
                        CAppEffectData *pData = new CAppEffectData(this,RC_EFF_OTHER_WARP,D3DXVECTOR3(0,0,0));
                        g_pD3dApp->m_pEffectList->AddChild(pData);
                    }
                    // 2007-10-04 by dgwoo 워프 통과로 인한 프롤로그가 아닌 모든 미션을 완료하였을경우 
                    // 프롤로그를 보여준다.
//                    if(g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() == L10)
//                    {
//                        g_pTutorial->EndTutorial();
//                        // 2006-09-07 by ispark
//                        STAGE_EFFECT_DATA stStageData;
//                        stStageData.pWarpObject = (int *)m_pCurrentObjEvent;
//                        g_pInterface->InitSoundScript("Prolog.tex", &stStageData, TUTORIAL_COMPLETE, TRUE);
//                        break;
//                    }

//                    if( InitCinemaUnit(PATTERN_UNIT_WARP_IN) == FALSE ||
//                        InitCinemaCamera(PATTERN_CAMERA_WARP_IN) == FALSE )
//                    {// 패턴 생성 실패시 그냥 보낸다.
                    SendFieldSocketRequestEventObjectWarpIn(m_pCurrentObjEvent);
//                     }
//                    else
//                    {
//                        CObjectChild* pWarpgate = g_pScene->FindWarpGateByPosition( m_vPos );
//                        if( pWarpgate )
//                        {
//                            pWarpgate->ChangeBodycondition( BODYCON_WARP_MASK );
//                        }
//                    }
                }
                break;
            }
        case EVENT_TYPE_ENTER_BUILDING:
            {// 상점 Object
                if(g_pD3dApp->m_dwGameState != _SHOP)
                {
                    MSG_FC_EVENT_REQUEST_OBJECT_EVENT sMsg;
                    memset(&sMsg,0x00,sizeof(sMsg));
                    char buffer[SIZE_MAX_PACKET];
                    int nType = T_FC_EVENT_REQUEST_OBJECT_EVENT;
//                    sMsg.ObjectIndex = pObj->m_nObjectCont;
                    sMsg.ObjectType = m_pCurrentObjEvent->m_nCode;
                    sMsg.ObjectPosition = m_pCurrentObjEvent->m_vPos;
                    memcpy(buffer,&nType,SIZE_FIELD_TYPE_HEADER);
                    memcpy(buffer+SIZE_FIELD_TYPE_HEADER,&sMsg,sizeof(sMsg));
                    g_pD3dApp->m_pFieldWinSocket->Write(buffer,SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//                    m_dwState = _LANDED;
                    DBGOUT("Request Warp : MSG_FC_EVENT_REQUEST_OBJECT_EVENT(5997)\n");
//                    m_bEventReady = TRUE;
//                    m_fEventCheckTime = 2.0f;
                }
            }
            break;
        case EVENT_TYPE_WARP_TARGET:
            {// 클라이언트 현재 사용안함
            }
            break;
        case EVENT_TYPE_LANDING:
            {// 착륙 Object
            }
            break;
        case EVENT_TYPE_RACING_CHECK_POTINT:
            {// 레이싱 체크 포인트
            }
            break;
        case EVENT_TYPE_OBJECT_MONSTER:
            {// 오브젝트 몬스터 - 아무것도 안함
            }
            break;
        case 7:
            {// 바뀔예정
            }
            break;
        case EVENT_TYPE_PATTERN_POINT:
            {// 캐릭터 워프시 패턴의 시작/끝
                if(m_nEventTypeAirPort == AIR_PORT_TAKEOFF_PATTERN_START && 
                    m_bWarpLink == TRUE)// 2006-05-04 by ispark, 이륙시 워프를 만나면 워프한다.
                {
                    // 이륙처리 한다.
                    m_nEventTypeAirPort = AIR_PORT_TAKEOFF_PATTERN_END;
                    DeletePattern();
                    ChangeUnitState( _NORMAL );
                    m_bWarpLink = FALSE;
                    m_bKeyBlock = FALSE;                                        // 2005-10-26 by ispark
                    // 2007-06-12 by bhsohn 미션 인터페이스 수정안                    
//                    if(g_pGameMain->m_pMissionInfo->GetMissionMapIndex())
//                    {
//                        INIT_MSG_WITH_BUFFER(MSG_FC_QUEST_MOVE_QUEST_MAP, T_FC_QUEST_MOVE_QUEST_MAP,
//                                            pSQuestMove, SendBuf);
//                        pSQuestMove->QuestIndex0 = g_pGameMain->m_pMissionInfo->GetMissionIndex()->QuestIndex;
//                        g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_QUEST_MOVE_QUEST_MAP));
//                    }
                    SendGoMissionMap();                    
                    
                }
                  if(m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT)    // 워프아웃 패턴 중이면 패턴이 종료된다.
                {
                    DeletePattern();
                }
                else if(m_nCurrentPatternNumber == 0)                        // 현재 진행중인 패턴이 없으면 워프인 패턴을 생성한다.
                {
                    // 2007-04-05 by bhsohn 빅붐 버그 처리
                    BOOL bWarpSuccess = (g_pTutorial->IsTutorialMode() == FALSE);
                    if(bWarpSuccess && m_pSkill)
                    {
                        // 빅붐을 쓰면서 포탈을 쓰지 못하게 변경
                        if(SKILL_BASE_NUM(m_pSkill->GetPreSkillItemNum()) == BGEAR_SKILL_BASENUM_BIG_BOOM)
                        {                            
                            bWarpSuccess = FALSE;
                        }                        
                    }
//                    if(g_pTutorial->IsTutorialMode() == FALSE || 
//                        (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() == L8))
                    if( bWarpSuccess
                        || (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() == L10))
                    {
                        if(g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() == L10)
                        {
                            // 튜토리얼을 끝내고서 워프를 했음. 
                            g_pTutorial->SetTutorialEd(TRUE);
                        }

                        // 패턴 위프 오브젝트의 방향과 셔틀의 방향 사이의 각을 구한다.
                        CObjectChild* pObj = g_pScene->FindEventObjectByIndex(m_pCurrentObjEvent->m_vPos, m_pCurrentObjEvent->m_nNextEventIndex);
                        if(pObj)
                        {
                            D3DXVECTOR3 vPatternVel = pObj->m_vPos - m_pCurrentObjEvent->m_vPos; // 패턴 방향
                            D3DXVec3Normalize(&vPatternVel,&vPatternVel);
                            float fAngle = ACOS(D3DXVec3Dot(&m_vVel,&vPatternVel));    
                            if( fAngle > 0 &&
                                fAngle < PI/2 &&
                                !m_pClientParty->IsFormationFlightMove())            // 2006-11-02 by ispark, 편대 비행이면 워프 안탐
                            {
                                PatternWarpIn();
                            }
                        }
                    }
                }
            }
            break;
        case EVENT_TYPE_LANDING_PATTERN_START:
            {                
                // 이벤트 워프 패턴 스타트 ( 착륙시 )
                CObjectChild* pObj = g_pScene->FindObjectByIndex(m_pCurrentObjEvent->m_nNextEventIndex);
                if(m_nCurrentPatternNumber == PATTERN_UNIT_AIRPORT_IN)    // 워프아웃 패턴 중이면 패턴이 종료된다.
                {
                }
                else if(pObj )
                {
                    // 스킵을 위한 변수
                    if(m_pCurrentObjEvent)
                        m_pCurrentObjEventOld = m_pCurrentObjEvent;

                    D3DXVECTOR3 vPatternVel = pObj->m_vPos - m_pCurrentObjEvent->m_vPos; // 패턴 방향
                    D3DXVec3Normalize(&vPatternVel,&vPatternVel);
                    float fAngle = ACOS(D3DXVec3Dot(&m_vVel,&vPatternVel));    
                    if( fAngle > 0 &&
                        fAngle < PI/2)    
                    {
                        m_bWarpLink = TRUE;
                        m_nEventTypeAirPort = AIR_PORT_LANDING_PATTERN_START;
                        D3DXVECTOR3 vPatternVel = pObj->m_vPos - m_pCurrentObjEvent->m_vPos; // 패턴 방향
                        D3DXVec3Normalize(&vPatternVel,&vPatternVel);
                        float fAngle = ACOS(D3DXVec3Dot(&m_vVel,&vPatternVel));
                        m_fShuttleSpeed = AIR_PORT_MIN_SPEED;
                        // 2005-08-05 by ispark
                        g_pInterface->m_pGameMain->InitShowWindow();
                        m_nBooster = BOOSTER_TYPE_MIN;
                        CheckBoosterAni();
                        
                        D3DXVECTOR3 vVel,vUp;
                        vVel = m_vPos - pObj->m_vPos;
                        vUp.x = m_mMatrix._21;
                        vUp.y = m_mMatrix._22;
                        vUp.z = m_mMatrix._23;
                        if(vVel == vUp)
                        {// 매트릭스가 깨지지 않도록
                            m_mMatrix._21 += 0.001f;
                            m_mMatrix._22 -= 0.001f;
                            m_mMatrix._23 += 0.002f;
                        }
                        m_mMatrix._31 = vVel.x;
                        m_mMatrix._32 = vVel.y;
                        m_mMatrix._33 = vVel.z;
                        
                        int nPattNum = PATTERN_UNIT_AIRPORT_IN + m_pCurrentObjEvent->m_sEventIndexFrom;
                        if(InitCinemaUnit(nPattNum, FALSE) == TRUE)
                        {
                            m_bSkillMoveIsUse = TRUE;
                            m_nCurrentPatternNumber = nPattNum ;
                            InitCinemaCamera(PATTERN_CAMERA_AIRPORT_IN);
                            m_bKeyBlock = TRUE;
                        }
                        else if(InitCinemaUnit(PATTERN_UNIT_AIRPORT_IN, FALSE) == TRUE)
                        {
                            m_bSkillMoveIsUse = TRUE;
                            m_nCurrentPatternNumber = PATTERN_UNIT_AIRPORT_IN;
                            InitCinemaCamera(PATTERN_CAMERA_AIRPORT_IN);
                            m_bKeyBlock = TRUE;
                        }
                        else
                        {
                            DBGOUT("[PATTERN_UNIT_AIRPORT_IN] : Pattern File Non Exsistent.\n");
                        }
                    }
                }
            }
            break;
        case EVENT_TYPE_LANDING_PATTERN_END:
            {
                if(m_pCurrentObjEvent && TRUE == m_bWarpLink)
                {
                    m_nEventTypeAirPort = AIR_PORT_LANDING_PATTERN_END;
                    DeletePattern();
                    ChangeUnitState( _LANDING );
                    m_nEventIndex = m_pCurrentObjEvent->m_nNextEventIndex;
                    m_bWarpLink = FALSE;
                    m_bLandingField = TRUE;
                }
            }
            break;
        case EVENT_TYPE_TAKEOFF_PATTERN_START:
            {
                // 이벤트 워프 패턴 스타트 ( 이륙시 )
                if(m_bWarpLink == TRUE)
                {
                    CObjectChild* pObj = g_pScene->FindObjectByIndex(m_pCurrentObjEvent->m_nNextEventIndex);
                    if(m_nCurrentPatternNumber == PATTERN_UNIT_AIRPORT_OUT)    // 워프아웃 패턴 중이면 패턴이 종료된다.
                    {
                    }
                    else if(pObj )
                    {
                        //                    ChangeUnitState( _LANDING );
                        m_nEventTypeAirPort = AIR_PORT_TAKEOFF_PATTERN_START;
                        D3DXVECTOR3 vPatternVel = pObj->m_vPos - m_pCurrentObjEvent->m_vPos; // 패턴 방향
                        D3DXVec3Normalize(&vPatternVel,&vPatternVel);
                        float fAngle = ACOS(D3DXVec3Dot(&m_vVel,&vPatternVel));
                        m_fShuttleSpeed = m_fShuttleSpeedBoosterOn;
                        
                        // 메세지 처리
                        INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE,
                            T_FC_CHARACTER_CHANGE_CHARACTER_MODE, pSMode, SendBuf);
                        pSMode->CharacterMode0 = FALSE;
                        pSMode->PositionAVec3 = m_vPos;
                        pSMode->TargetAVec3 = m_vVel*1000.0f;
                        g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE));
                        
                        // 2005-07-29 by ispark
                        // 출발 바디컨디션 보냄
                        SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, BODYCON_FLY_MASK|BODYCON_SET_OR_CLEAR_MASK);
                        SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, BODYCON_BOOSTER1_MASK|BODYCON_SET_OR_CLEAR_MASK);
                        CheckBodyConditionToServer(m_myShuttleInfo.BodyCondition);
                        m_nBooster = BOOSTER_TYPE_MIN;        // -1:후진부스터 0:중지 1:기본부스터 2:가속부스터
                        ChangeSingleBodyCondition(BODYCON_BOOSTER1_MASK);

                         g_pD3dApp->SetCharacterChangeCurrentHPAndDamageBodyCondition(m_myShuttleInfo.CurrentHP);        // 2013-06-25 by ssjung 데미지 이펙트가 안나오는 현상 수정

                        D3DXVECTOR3 vVel,vUp;
                        vVel = m_vPos - pObj->m_vPos;
                        vUp.x = m_mMatrix._21;
                        vUp.y = m_mMatrix._22;
                        vUp.z = m_mMatrix._23;
                        if(vVel == vUp)
                        {// 매트릭스가 깨지지 않도록
                            m_mMatrix._21 += 0.001f;
                            m_mMatrix._22 -= 0.001f;
                            m_mMatrix._23 += 0.002f;
                        }
                        m_mMatrix._31 = vVel.x;
                        m_mMatrix._32 = vVel.y;
                        m_mMatrix._33 = vVel.z;
                        
                        // 2005-08-01 by ispark
                        // 좌표 설정 오류
                        m_mMatrix._41 = m_vPos.x;
                        m_mMatrix._42 = m_vPos.y;
                        m_mMatrix._43 = m_vPos.z;


                        int nPattNum = PATTERN_UNIT_AIRPORT_OUT + m_pCurrentObjEvent->m_sEventIndexFrom;
                        if(InitCinemaUnit(nPattNum, FALSE) == TRUE)
                        {
                            m_bSkillMoveIsUse = TRUE;
                            m_nCurrentPatternNumber = nPattNum ;

                            // 2007-06-13 by bhsohn 아카데미맵 시네마 추가
                            //InitCinemaCamera(PATTERN_CAMERA_AIRPORT_OUT);            // 2005-08-01 by ispark 임시로 쓰는 번호
                            int nCameraType = PATTERN_CAMERA_AIRPORT_OUT;
                            // 2009. 10. 14 by jskim 프리스카 제거 
//                             if(IS_NORMAL_CITY_MAP_INDEX(g_pD3dApp->GetMyShuttleMapChannelIndex().MapIndex))
//                             {
//                                 nCameraType = PATTERN_CAMERA_AIRPORT_NORMALCITY_OUT;
//                             }
                            //end 2009. 10. 14 by jskim 프리스카 제거 
                            InitCinemaCamera(nCameraType);            // 2005-08-01 by ispark 임시로 쓰는 번호
                            
                            m_bKeyBlock = TRUE;
                        }
                        else if(InitCinemaUnit(PATTERN_UNIT_AIRPORT_OUT, FALSE) == TRUE)
                        {
                            m_bSkillMoveIsUse = TRUE;
                            m_nCurrentPatternNumber = PATTERN_UNIT_AIRPORT_OUT;

                            // 2007-06-13 by bhsohn 아카데미맵 시네마 추가
                            //InitCinemaCamera(PATTERN_CAMERA_AIRPORT_OUT);            // 2005-08-01 by ispark
                            int nCameraType = PATTERN_CAMERA_AIRPORT_OUT;
                            // 2009. 10. 14 by jskim 프리스카 제거 
//                             if(IS_NORMAL_CITY_MAP_INDEX(g_pD3dApp->GetMyShuttleMapChannelIndex().MapIndex))
//                             {
//                                 nCameraType = PATTERN_CAMERA_AIRPORT_NORMALCITY_OUT;
//                             }
                            //end 2009. 10. 14 by jskim 프리스카 제거 
                            InitCinemaCamera(nCameraType);        

                            m_bKeyBlock = TRUE;
                        }
                        else
                        {
                            DBGOUT("[PATTERN_UNIT_AIRPORT_OUT] : Pattern File Non Exsistent.\n");
                        }
                    }
                }
            }
            break;
        case EVENT_TYPE_TAKEOFF_PATTERN_END:
            {
                if(m_bWarpLink == TRUE && m_nEventTypeAirPort != AIR_PORT_LANDING_PATTERN_START)
                {
                    m_nEventTypeAirPort = AIR_PORT_TAKEOFF_PATTERN_END;
                    DeletePattern();
                    ChangeUnitState( _NORMAL );
                    m_bWarpLink = FALSE;
                    m_bKeyBlock = FALSE;                                        // 2005-10-26 by ispark
                    // 2007-06-12 by bhsohn 미션 인터페이스 수정안
//                    if(g_pGameMain->m_pMissionInfo->GetMissionMapIndex())
//                    {
//                        INIT_MSG_WITH_BUFFER(MSG_FC_QUEST_MOVE_QUEST_MAP, T_FC_QUEST_MOVE_QUEST_MAP,
//                                            pSQuestMove, SendBuf);
//                        pSQuestMove->QuestIndex0 = g_pGameMain->m_pMissionInfo->GetMissionIndex()->QuestIndex;
//                        g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_QUEST_MOVE_QUEST_MAP));
//                    }    
                    SendGoMissionMap();
                }
            }
            break;
        case EVENT_TYPE_TUTORIAL_GATE_POINT:
            if(g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() == L1)
            {
                g_pTutorial->MakeDisableTutorialGate(m_pCurrentObjEvent->m_vPos);
            }
            break;
        case EVENT_TYPE_OBJ_QUEST_OBJECT:
            {
                MSG_FC_EVENT_REQUEST_OBJECT_EVENT sMsg;
                sMsg.ObjectType        = m_pCurrentObjEvent->m_nCode;
                sMsg.ObjectPosition = m_pCurrentObjEvent->m_vPos;
                g_pFieldWinSocket->SendMsg( T_FC_EVENT_REQUEST_OBJECT_EVENT, (char*)&sMsg, sizeof(sMsg) );
                
                DBGOUT("EVENT_TYPE_OBJ_QUEST_OBJECT\n");
            }
            break;
        }
    }
}

void CShuttleChild::SetWeaponCountPrimary(INT nClientIndex, INT nCount)
{
    FLOG( "CShuttleChild::SetWeaponCountPrimary(INT nClientIndex, INT nCount)" );
    if(m_pPrimaryWeapon)
    {
        m_pPrimaryWeapon->SetWeaponCount( nCount );
    }
}

void CShuttleChild::SetWeaponCountSecondary(INT nClientIndex,INT nCount)
{
    FLOG( "CShuttleChild::SetWeaponCountSecondary(INT nClientIndex,INT nCont)" );
    if(m_pSecondaryWeapon)
    {
        m_pSecondaryWeapon->SetWeaponCount( nCount );
    }
}

void CShuttleChild::InitItemPoint()
{
    FLOG( "CShuttleChild::InitItemPoint()" );
    ITEM_GENERAL* pItem;
    DeleteItemPoint();
    // 2006-07-21 by ispark, 캐릭터와 기어 이펙트 초기화
    /*--------------------------------------------------------------------------*/
    // 기어 이펙트
    if(g_pD3dApp->m_bCharacter == FALSE)
    {
        
        // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//         CreatePrimaryWeaponItem(m_pPrimaryWeapon ? m_pPrimaryWeapon->GetRealItemInfo() : NULL);
//         CreateSecondaryWeaponItem(m_pSecondaryWeapon ? m_pSecondaryWeapon->GetRealItemInfo() : NULL);

        if( !m_pPrimaryWeapon )
            CreatePrimaryWeaponItem( NULL, NULL );
        else
        {
            // 추가적인 외형아이템
            ITEM*    pShapeItem    = g_pDatabase->GetServerItemInfo( m_pPrimaryWeapon->GetItemGeneral()->GetShapeItemNum() );
            CreatePrimaryWeaponItem( m_pPrimaryWeapon->GetRealItemInfo(), pShapeItem  );
        }

        if( !m_pSecondaryWeapon )
            CreateSecondaryWeaponItem( NULL, NULL );
        else
        {
            // 추가적인 외형아이템
            ITEM*    pShapeItem    = g_pDatabase->GetServerItemInfo( m_pSecondaryWeapon->GetItemGeneral()->GetShapeItemNum() );
            
            // 2009. 10. 21 by ckPark 맵 이동시 2형 무기장착 안나오는 현상
            //CreatePrimaryWeaponItem( m_pSecondaryWeapon->GetRealItemInfo(), pShapeItem );
            CreateSecondaryWeaponItem( m_pSecondaryWeapon->GetRealItemInfo(), pShapeItem );
            // end 2009. 10. 21 by ckPark 맵 이동시 2형 무기장착 안나오는 현상
        }
        // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

        pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_PROW );
        if( pItem )
        {
            CreateWearItem( WEAR_ITEM_KIND_PROW, EFFECT_NUMBER_WEAR_RADAR(pItem->ItemInfo->SourceIndex) );
        }
        pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_REAR );
        if(pItem)
        {
            CreateWearItem( WEAR_ITEM_KIND_ENGINE, pItem->ItemInfo->SourceIndex );
        }
    }
    /*--------------------------------------------------------------------------*/
    // 캐릭터 이펙트
    else if(g_pD3dApp->m_bCharacter == TRUE)
    {
        // 2007-12-17 by bhsohn 전진 기지 맵에서 무기 안나오는 현상처리

        // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//         if(m_pPrimaryWeapon)
//         {
//             CreatePrimaryWeaponItem(m_pPrimaryWeapon ? m_pPrimaryWeapon->GetRealItemInfo() : NULL);
//         }        
// 
//         if(m_pSecondaryWeapon)
//         {
//             CreateSecondaryWeaponItem(m_pSecondaryWeapon ? m_pSecondaryWeapon->GetRealItemInfo() : NULL);
//         }        

        if( !m_pPrimaryWeapon )
            CreatePrimaryWeaponItem( NULL, NULL );
        else
        {
            // 추가적인 외형아이템
            ITEM*    pShapeItem    = g_pDatabase->GetServerItemInfo( m_pPrimaryWeapon->GetItemGeneral()->GetShapeItemNum() );
            CreatePrimaryWeaponItem( m_pPrimaryWeapon->GetRealItemInfo(), pShapeItem );
        }
        
        if( !m_pSecondaryWeapon )
            CreateSecondaryWeaponItem( NULL, NULL );
        else
        {
            // 추가적인 외형아이템
            ITEM*    pShapeItem    = g_pDatabase->GetServerItemInfo( m_pSecondaryWeapon->GetItemGeneral()->GetShapeItemNum() );

            // 2009. 10. 21 by ckPark 맵 이동시 2형 무기장착 안나오는 현상
            //CreatePrimaryWeaponItem( m_pSecondaryWeapon->GetRealItemInfo(), pShapeItem );
            CreateSecondaryWeaponItem( m_pSecondaryWeapon->GetRealItemInfo(), pShapeItem );
            // end 2009. 10. 21 by ckPark 맵 이동시 2형 무기장착 안나오는 현상
        }
        // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

        pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_PROW );
        if(pItem)
        {
            CreateWearItem( WEAR_ITEM_KIND_PROW, EFFECT_NUMBER_WEAR_RADAR(pItem->ItemInfo->SourceIndex) );
        }
        
        pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_REAR );
        if(pItem)
        {
            CreateWearItem( WEAR_ITEM_KIND_ENGINE, pItem->ItemInfo->SourceIndex );
        }
        // end 2007-12-17 by bhsohn 전진 기지 맵에서 무기 안나오는 현상처리
    }

    /*--------------------------------------------------------------------------*/
    // 전체 이펙트
    // 2006-07-21 by ispark, 이펙트 캐쉬 아이템
    pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_WINGIN );
    if(pItem)
    {
        CreateWearItem( WEAR_ITEM_KIND_WINGIN, pItem->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter );
    }

    pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_ACCESSORY_UNLIMITED );
    if(pItem)
    {
        CreateWearItem( WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, pItem->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter );
    }
    pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_ACCESSORY_TIME_LIMIT );
    if(pItem)
    {
        CreateWearItem( WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, pItem->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter );
    }
    // 2010-06-08 by dgwoo, 펫시스템 추가. 
    pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_PET );
    if(pItem)
    {
        CreateWearItem( WEAR_ITEM_KIND_PET, GetPetManager()->GetSourceIndex_PetCurrentData( pItem->UniqueNumber , g_pD3dApp->m_bCharacter == TRUE ? true : false ) , g_pD3dApp->m_bCharacter );
    }                                        
    // 2010-06-08 by dgwoo, 펫시스템 추가. 


    /*--------------------------------------------------------------------------*/
    pItem = m_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
    // 우선 기체에 관한 것이므로 FALSE로 설정
    // 2005-08-23 by ispark

    // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
    //ChangeUnitCharacterInfo((pItem?pItem->ItemInfo->SourceIndex:0), m_myShuttleInfo.UnitKind, m_myShuttleInfo.BodyCondition, FALSE );

    int nDefenseNum = 0;

    if( !pItem )
        nDefenseNum = 0;
    else
    {
#ifdef _REWORKED_COLORSHOP
        //If Colorcode is applied render ColorCode instead of COntour
        if (pItem->ColorCode)
        {
            nDefenseNum = pItem->ColorCode;
        }
        else
        {
#endif
            if (!pItem->ShapeItemNum)
                nDefenseNum = pItem->ItemInfo->SourceIndex;
            else
            {
                ITEM* pShapeItem = g_pDatabase->GetServerItemInfo(pItem->ShapeItemNum);
                if (pShapeItem)
                    nDefenseNum = pShapeItem->SourceIndex;
                else
                    nDefenseNum = pItem->ItemInfo->SourceIndex;
            }
#ifdef _REWORKED_COLORSHOP
        }
#endif
    }
    // 2010. 03. 18 by jskim 몬스터변신 카드
    //ChangeUnitCharacterInfo( nDefenseNum, m_myShuttleInfo.UnitKind, m_myShuttleInfo.BodyCondition, FALSE );
    BOOL bMonTras = FALSE;

    if( m_MonsterTransformer )
    {
        ChangeUnitCharacterInfo(nDefenseNum, m_myShuttleInfo.UnitKind, m_myShuttleInfo.BodyCondition, FALSE);
        SetMonsterTransPrimarySidePos(m_PrimaryAttack.vSidePos);
        SetMonsterTransSecondarySidePos(m_SecondaryAttack.vSidePos);
    }

    if( m_MonsterTransformer )
    {
        bMonTras = TRUE;
        MEX_MONSTER_INFO * pTempIn = g_pGameMain->CheckMonsterInfo( m_MonsterTransformer );
        if(pTempIn)
            nDefenseNum = pTempIn->SourceIndex;
    }
    ChangeUnitCharacterInfo( nDefenseNum, m_myShuttleInfo.UnitKind, m_myShuttleInfo.BodyCondition, FALSE, bMonTras );
    //end 2010. 03. 18 by jskim 몬스터변신 카드
    // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
}
/*
///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)
/// \brief        Primary weapon의 action을 수행한다.
/// \author        dhkwon
/// \date        2004-03-21 ~ 2004-03-21
/// \warning    CUnitData의 PrimaryAttackAction함수를 오버라이딩(virtual function)하여 재구성
///                ((CUnitData*)p)->PrimaryAttackAction을 수행하더라도 자식의 것이 수행됨
///                이 부분은 CEnemyData,CShuttleChild간에 약간의 차이가 있어 따로 구현되었다.
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)
{
    FLOG( "CShuttleChild::CheckPrimaryAttack(BOOL bReset)" );
    if(m_pPrimaryAttackTarget && 
        (!m_pPrimaryAttackTarget->m_bUsing || 
        ATTACK_AVAILABLE_STATE(m_pPrimaryAttackTarget->m_dwState) == FALSE))
//        m_pPrimaryAttackTarget->m_dwState != _NORMAL))
    {
        m_pPrimaryAttackTarget = NULL;    // 데이타가 지워질 것인지 체크하여 타겟포인터를 NULL셋팅
    }

    if( m_pPrimaryWeapon && m_pPrimaryWeapon->GetRealItemInfo() )
    {
        CUnitData::CheckPrimaryAttack( bReset, m_pPrimaryWeapon->GetRealItemInfo() );
    }
}
*/

// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//void CShuttleChild::CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM)
void CShuttleChild::CheckAttack( ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM /* = NULL */, ITEM* pEffectItem /* = NULL */ )
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
{
    if(IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType))
    {
        if(m_pPrimaryWeapon)
        {
            // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
            //CUnitData::CheckAttack( m_PrimaryAttack, bReset, m_pPrimaryWeapon->GetRealItemInfo() );

            // 추가적인 이펙트 아이템
            ITEM* pEffectItem = g_pDatabase->GetServerItemInfo(    m_pPrimaryWeapon->GetItemGeneral()->ColorCode );
            CUnitData::CheckAttack( m_PrimaryAttack, bReset, m_pPrimaryWeapon->GetRealItemInfo(), pEffectItem );
            // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
        }
    }
    else if(IS_SECONDARY_ATT_TYPE(attackData.AttackData.AttackType))
    {
        if(m_pSecondaryWeapon)
        {
            // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
            //CUnitData::CheckAttack( m_SecondaryAttack, bReset, m_pSecondaryWeapon->GetRealItemInfo() );

            // 추가적인 이펙트 아이템
            ITEM* pEffectItem = g_pDatabase->GetServerItemInfo(    m_pSecondaryWeapon->GetItemGeneral()->ColorCode );
            CUnitData::CheckAttack( m_SecondaryAttack, bReset, m_pSecondaryWeapon->GetRealItemInfo(), pEffectItem );
            // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
        }
    }
    // 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
    else if(ATT_TYPE_PET_GENERAL == attackData.AttackData.AttackType)
    {
        if(GetPetManager()->GetWeaponItemInfo())
        {
            CUnitData::CheckAttack( m_SecondaryAttack, bReset, m_pSecondaryWeapon->GetRealItemInfo() );
        }
    }
    // end 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::CreateSecondaryShieldDamage(D3DXVECTOR3 vCollPos)
/// \brief        쉴드 데미지가 서버에서 왔을 경우 이펙트 생성
/// \author        dhkwon
/// \date        2004-05-18 ~ 2004-05-18
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CreateSecondaryShieldDamage(D3DXVECTOR3 vCollPos)
{
//    ITEM_GENERAL *pItem = g_pStoreData->FindItemInInventoryByWindowPos(POS_WINGOUT);
//    if(!pItem || pItem->Kind != ITEMKIND_SHIELD)
//    {
//        pItem = g_pStoreData->FindItemInInventoryByWindowPos(POS_WINGIN);
//    }
//    if(pItem && pItem->Kind == ITEMKIND_SHIELD)
//    {
        //CWeaponShieldData* pShield = new CWeaponShieldData(this, pItem->ItemNum, vCollPos );
        CWeaponShieldData* pShield = new CWeaponShieldData(this, RC_EFF_SHIELD_HIT, vCollPos );
        g_pScene->m_pWeaponData->AddChild(pShield);
//    }
}


void CShuttleChild::SetShuttleFlight()
{
    FLOG( "CShuttleChild::SetShuttleFlight()" );
    if(g_pD3dApp->GetHwnd()!=GetActiveWindow())
        return;

    SetCursorInit();
    m_bIsSetFlighting = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::Move_AirSiege(float fElapsedTime)
/// \brief        에어 시즈만을 따로 처리한다.
/// \author        dgwoo
/// \date        2006-11-21 ~ 2006-11-21
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::Move_AirSiege(float fElapsedTime)
{
    D3DXVec3Normalize(&m_vVel,&m_vVel);
    D3DXVec3Normalize(&m_vUp,&m_vUp);
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
    m_fAdvanceTime -= fElapsedTime;
    if(m_fAdvanceTime <= 0.0f)
        m_fAdvanceTime = 0.0f;
    
    m_bLandedMove = FALSE;
    m_bIsAir = TRUE;
    int nSideMove = 0;    
    m_nBooster = BOOSTER_TYPE_STOP;
    CheckMoveRate(fElapsedTime,nSideMove);
    if(m_myShuttleInfo.CurrentEP)
        m_vPos += (m_vNextPos - m_vPos)*fElapsedTime;
    else 
        m_vPos += 0.5f * (m_vNextPos - m_vPos) * fElapsedTime;
    CheckCollForMap();
    CheckCollForObject(fElapsedTime);
    // 2007-03-14 by dgwoo 에어 시즈시 평행을 유지하지 않기로 변경.
    //SetShuttleFlightInit(fElapsedTime);
    if(    (g_pSOption->sMenuBalance == TRUE ||
        g_pGameMain->m_pSystem->m_bQuitGameMessage == TRUE)&&            // 메뉴사용시 유닛정지 옵션 체크
        (g_pD3dApp->m_bMessageBox == TRUE || 
        g_pD3dApp->m_bFixUnitDirMode == TRUE) )
    {// 인터페이스 창이 떠있는 경우 기어를 서서히 감속한다.
        m_nShuttleStopState = 4;
    }
    CheckBoosterAni();
    CheckMapRegion();
    int nXCheck,nYCheck;
    nXCheck = (int)(m_vPos.x/TILE_SIZE);
    nYCheck = (int)(m_vPos.z/TILE_SIZE);
    m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize+nYCheck].bMove;
    if( m_dwState == _FALLEN || 
        m_dwState == _EXPLODED || 
//        g_pGameMain->IsShowing() ||                // 2006-11-07 by ispark, 창이 떠도 움직이게 수정
        m_bOrderMove || 
        m_bMouseLock || 
        //g_pD3dApp->m_bChatMode ||                // 2008-09-02 by dgwoo 채팅중에도 마우스 컨트롤 가능하게.
        g_pD3dApp->m_bFixUnitDirMode == TRUE)    // 유닛 진행 방향 고정 모드
    {
        m_fLRVelRate = 0.0f;
        m_fSideRate = 0.0f;
    }

    SetMatrix_Move_Ground(fElapsedTime);//    SetEffectPos_B_Gear(fElapsedTime);
    SiegeTarget();

    // 2010-12-21 by hsson 펫시스템 - 시즈 모드시 펫 숨기기 
    // 2010-06-15 by shcho&hslee 펫시스템 - 시즈 모드시 펫 숨기기 
    //HidePet(TRUE);
    // end 2010-06-15 by shcho&hslee 펫시스템 - 시즈 모드시 펫 숨기기 
    // end 2010-12-21 by hsson 펫시스템 - 시즈 모드시 펫 숨기기
    
}
void CShuttleChild::ObserveUpdateGage(MSG_FC_CHARACTER_OBSERVER_INFO *pMsg)
{
    m_myShuttleInfo.HP = pMsg->HP;
    m_myShuttleInfo.SP = pMsg->SP;
    m_myShuttleInfo.DP = pMsg->DP;
    m_myShuttleInfo.EP = pMsg->EP;
    if(pMsg->CurrentHP < 1)
        m_myShuttleInfo.CurrentHP = 1;
    else
        m_myShuttleInfo.CurrentHP = pMsg->CurrentHP;

    if(pMsg->CurrentSP < 1)
        m_myShuttleInfo.CurrentSP = 1;
    else
        m_myShuttleInfo.CurrentSP = pMsg->CurrentSP;

    if(pMsg->CurrentDP < 1)
        m_myShuttleInfo.CurrentDP = 1;
    else
        m_myShuttleInfo.CurrentDP = pMsg->CurrentDP;

    if(pMsg->CurrentEP < 1)
        m_myShuttleInfo.CurrentEP = 1;
    else
        m_myShuttleInfo.CurrentEP = pMsg->CurrentEP;
}

void CShuttleChild::Move_Observe()
{
    D3DXVec3Normalize(&g_pShuttleChild->m_vVel,&m_stObserve.vObserveLook);
    g_pShuttleChild->m_vUp                = m_stObserve.vObserveUp;
    g_pShuttleChild->m_fOldUpRate        = 0;
    g_pShuttleChild->m_vPos                = m_stObserve.vObservePos;
    g_pShuttleChild->m_vNextPos            = m_stObserve.vObserveNextPos;
    g_pShuttleChild->m_vSideVel            = m_stObserve.vObserveSide;
    return;
}

void CShuttleChild::Observe_PosSeting()
{
    m_stObserve.vObserveLook = m_stObserve.pEnemyData->m_vVel;
    m_stObserve.vObservePos = m_stObserve.pEnemyData->m_vPos;
    m_stObserve.vObserveSide = m_stObserve.pEnemyData->m_vSideVel;
    // 2007-03-27 by bhsohn 옵저버 모드 업벡터 고정
    m_stObserve.vObserveUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);            
    m_stObserve.vObserveNextPos = m_stObserve.pEnemyData->m_vNextPos;
}


void CShuttleChild::Move_Air(float fElapsedTime)    //void CShuttleChild::Move_B_Gear(float fElapsedTime)
{
    // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
    D3DXVECTOR3 vAirUp = m_vUp;
    D3DXVECTOR3 vAirVel = m_vVel;
    // END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
    char message[256];
    CheckRollKeyDown(fElapsedTime);                        // 2005-07-05 by ispark

    // 2007-03-20 by dgwoo 옵저버
    if(IsOperAndObser() && IsObserverMode())
    {
        Observe_PosSeting();
        Move_Observe();
    }
    //    if(m_bFormationFlight)
    if(m_pClientParty->IsFormationFlightMove())
    {
//        DBGOUT("편대\n");
        m_pClientParty->SetMasterInfo();
        m_pClientParty->FormationFlightMoveForShuttleChild();
        m_pClientParty->FormationFlightMoveForEnemy();

        // 2005-11-21 by ispark
        //  편대비행시 타겟을 위한 비행 회전
        SetFormationMove(fElapsedTime);

        if(D3DXVec3Length(&(m_vNextPos - m_vPos)) > 1000.0f ||
            m_pClientParty->GetPartyInfo().bFormationFlyingType == FLIGHT_FORM_NONE) // 자유비행은 해제
        {
            SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
            m_pClientParty->FormationFlightClear();
//            m_infoParty.bFormationFlyingPositionType = 0;
//            m_bFormationFlight = FALSE; // 편대 비행을 켄슬하고 본인의 의지대로 비행한다.
            wsprintf(message,STRMSG_C_PARTY_0001);//"편대비행에서 벗어났습니다."
            g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);

            // 해제시 방향값 초기화
            m_fCurrentSideRate = 0.0f;
            m_fCurrentAngle = 0.0f;
        }
    }
    else if(m_bAttackMode == _SIEGE)
    {// 2006-11-20 by dgwoo 시즈 모드로 변경시.

        // 2010-12-21 by hsson 펫시스템 - 시즈 모드시 펫 숨기기 
        HidePet(TRUE);
        // end 2010-12-21 by hsson 펫시스템 - 시즈 모드시 펫 숨기기 

        Move_AirSiege(fElapsedTime);

        // 2008-03-04 by bhsohn 시즈모드후 게임종료면 멈추는 현상처리
        if((m_dwState == _WARP)
            && (EVENT_GAME_END  == m_nEventType))
        {
            SetSelectCharacterItem();
            g_pD3dApp->SendFieldSocketCharacterGameEnd(FALSE);    // 2015-09-26 Future, standardized Game closing
        }
        // end 2008-03-04 by bhsohn 시즈모드후 게임종료면 멈추는 현상처리
        return ;
    }
    else
    {
        // 2010-12-21 by hsson 펫시스템 - 시즈 모드시 펫 숨기기 
        HidePet(FALSE);
        // end 2010-12-21 by hsson 펫시스템 - 시즈 모드시 펫 숨기기 

        // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리        
        D3DXVECTOR3            vOldUpTmp = m_vUp;
        // END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리

        // 2005-08-10 by ispark
        // 유닛정지로 인하여 위치를 바꾼다.
        D3DXVec3Normalize(&m_vVel,&m_vVel);
        D3DXVec3Normalize(&m_vUp,&m_vUp);
        // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
        UpdateSafeUpVector(vOldUpTmp);        
        // END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리

        D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);

        // 2005-07-05 by ispark
        // 롤링 시스템 사용시 마우스 움직임을 막는다.
        // 2010. 12. 06. by jskim 윈도우 비활성화 일때 마우스 처리 안하게 함.
        //if(!m_bMouseMoveLock)
        if(!m_bMouseMoveLock && g_pD3dApp->m_bHasFocus)
        // end 2010. 12. 06. by jskim 윈도우 비활성화 일때 마우스 처리 안하게 함.
        {
            D3DXMATRIX  matTemp;
            int nx,ny;
            nx = g_pD3dApp->GetBackBufferDesc().Width/2;
            ny = g_pD3dApp->GetBackBufferDesc().Height/2;

            // 2005-11-28 by ispark, 엔진 아이템 가져오기
            ITEM* pItemInfo = g_pStoreData->GetItemInfoFromItemGeneral(g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR ));

            // 평면 방정식을 구한다.
            // a*x + b*y + c*z + d = 0 << 평면 방정식
            float fTemp = -((m_vUp.x*m_vMouseDir.x + m_vUp.y*m_vMouseDir.y + m_vUp.z*m_vMouseDir.z)/(m_vUp.x*m_vUp.x + m_vUp.y*m_vUp.y + m_vUp.z*m_vUp.z));
            D3DXVECTOR3 vNew;
            vNew.x = m_vUp.x*fTemp + m_vMouseDir.x;
            vNew.y = m_vUp.y*fTemp + m_vMouseDir.y;
            vNew.z = m_vUp.z*fTemp + m_vMouseDir.z;
            D3DXVec3Normalize(&vNew,&vNew);
            float fAngle = (float)(AIR_UPDOWNRATE_MAX * m_ptDir.y) / ny;
            if(fAngle > AIR_UPDOWNRATE_MAX)
                fAngle = AIR_UPDOWNRATE_MAX;
            else if(fAngle < -AIR_UPDOWNRATE_MAX)
                fAngle = -AIR_UPDOWNRATE_MAX;

            if( abs(ny - m_pt.y) > 3*((float)g_pD3dApp->GetBackBufferDesc().Height/(float)600) && 
                m_dwState == _NORMAL && 
                m_nBooster != BOOSTER_TYPE_AIRBREAK && 
//                !g_pGameMain->IsShowing() &&                // 2006-11-07 by ispark, 창이 떠도 움직이게 수정
                !m_bMouseLock && !m_bOrderMove && 
                //!g_pD3dApp->m_bChatMode &&                // 2008-09-02 by dgwoo 채팅중에도 마우스 컨트롤 가능하게.
                g_pD3dApp->m_bFixUnitDirMode == FALSE)        // 유닛 진행 방향 고정 모드
            {
                // 2005-09-06 by ispark
                // 선회할 수 있는 최대 수치로 점점 증가
                // 2005-11-28 by ispark, 하이퍼무빙 스킬 추가
                float fSpeedPenalty = 0.0f;
                if(pItemInfo)
                {
                    fSpeedPenalty = CAtumSJ::GetCircularAngle(pItemInfo, &m_paramFactor);
                }
//                m_fCurrentAngle += m_bySpeedPenalty * fElapsedTime * fAngle * 0.1f;
                m_fCurrentAngle += fSpeedPenalty * fElapsedTime * fAngle * 0.1f;
                if(fAngle > 0 && m_fCurrentAngle > fAngle)
                    m_fCurrentAngle = fAngle;
                else if(fAngle < 0 && m_fCurrentAngle < fAngle)
                    m_fCurrentAngle = fAngle;
                    D3DXMatrixRotationAxis(&matTemp,&m_vSideVel, m_fMouseRate*m_fCurrentAngle*fElapsedTime);
                    D3DXVec3TransformCoord(&m_vVel,&m_vVel, &matTemp );
            }
            float fSideRateTemp = m_fSideRate;
            m_fSideRate = (float)(AIR_SIDERATE_MAX * m_ptDir.x) / nx;
            if(m_fSideRate > AIR_SIDERATE_MAX)
                m_fSideRate = AIR_SIDERATE_MAX;
            else if(m_fSideRate < -AIR_SIDERATE_MAX)
                m_fSideRate = -AIR_SIDERATE_MAX;
            
            if( abs(nx - m_pt.x) > 3*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800) && 
                (m_dwState == _NORMAL || m_dwState == _LANDING) && 
                !g_pD3dApp->m_pCamera->m_bIsCamControl && 
//                !g_pGameMain->IsShowing() &&                // 2006-11-07 by ispark, 창이 떠도 움직이게 수정 
                !m_bMouseLock && !m_bOrderMove &&
                //!g_pD3dApp->m_bChatMode  &&                    // 2008-09-02 by dgwoo 채팅중에도 마우스 컨트롤 가능하게.
                 g_pD3dApp->m_bFixUnitDirMode == FALSE)        // 유닛 진행 방향 고정 모드
            {
                // 2005-09-06 by ispark
                // 선회할 수 있는 최대 수치로 점점 증가
                // 2005-11-28 by ispark, 하이퍼무빙 스킬 추가
                float fSpeedPenalty = 0.0f;
                if(pItemInfo)
                {
                    fSpeedPenalty = CAtumSJ::GetCircularAngle(pItemInfo, &m_paramFactor);
                }
//                m_fCurrentSideRate += m_bySpeedPenalty * fElapsedTime * m_fSideRate * 0.1f;
                m_fCurrentSideRate += fSpeedPenalty * fElapsedTime * m_fSideRate * 0.1f;
                if(m_fSideRate > 0 && m_fCurrentSideRate > m_fSideRate)
                    m_fCurrentSideRate = m_fSideRate;
                else if(m_fSideRate < 0 && m_fCurrentSideRate < m_fSideRate)
                    m_fCurrentSideRate = m_fSideRate;
                
                D3DXMatrixRotationAxis( &matTemp, &m_vUp, m_fMouseRate*m_fCurrentSideRate*fElapsedTime);
                D3DXVec3TransformCoord( &m_vVel, &m_vVel, &matTemp );
            }    
            //
            else if(!m_bIsSetFlighting && g_pSOption->sAutoBalance == FALSE)
            {// 수평 유지중이 아닐때만-좌우 회전 및 기울임이없을때 수평 자동 유지 되도록 하자(좌우 수평만)-서서히
                SetShuttleFlightInit(fElapsedTime, false);
             }

            
            // 2005-02-23 by jschoi - 자동 수평 유지 옵션 추가
            if(g_pSOption->sAutoBalance == TRUE)        // 자동 수평 유지 옵션 체크
            {
                // 2005-01-27 by jschoi - 수평 유지를 위해 새로운 알고리즘 적용(B기어, I기어)
// 2007-08-07 by dgwoo 해상도가 큰 PC에선 회전각이 더 커진다.
//                float fDistanceToCenter = D3DXVec2Length(&(D3DXVECTOR2(nx - m_pt.x,ny - m_pt.y)));    // 마우스 포인터와 중심과의 거리
//                float fDistanceToMaxView = D3DXVec2Length(&(D3DXVECTOR2(nx,ny)));                    // 화면 끝과 중심과의 거리
//                float fAutoRecoveryShuttle = (1.0f - fDistanceToCenter/fDistanceToMaxView);            // 수평 자동 유지 정도 조정
//                SetShuttleFlightInit(fElapsedTime * fAutoRecoveryShuttle , false);
                SetShuttleFlightInit(fElapsedTime, false);
            }

            if(m_bIsSetFlighting)
            {
                if((abs(ny - m_pt.y) > 7 || abs(nx - m_pt.x) > 7) && 
                    m_dwState == _NORMAL &&
                    m_nBooster != BOOSTER_TYPE_AIRBREAK)
                {
                    m_bIsSetFlighting = FALSE;
                }
            }
            if(m_bIsSetFlighting)
                m_fSideRate = fSideRateTemp;
        }    // 2005-07-05 by ispark        // 여기까지 마우스 움직임 막기

        // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리        
        D3DXVECTOR3            vOldUpVec = m_vUp;
        // END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
        D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
        D3DXVec3Cross(&m_vUp,&m_vVel,&m_vSideVel);

        // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
        CheckUpNVelVector(vAirVel, vOldUpVec);
        // END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리

        if(m_pClientParty->IsFormationFlight())
        {        
            m_pClientParty->SetMasterInfo();
            m_pClientParty->FormationFlightMoveForEnemy();
        }
    }

    if(m_dwState==_NORMAL)
    {
        // 2006-01-06 by ispark, AdvanceTime
        m_fAdvanceTime -= fElapsedTime;
        if(m_fAdvanceTime <= 0.0f)
            m_fAdvanceTime = 0.0f;
        
        m_bLandedMove = FALSE;
        m_bIsAir = TRUE;
        int nSideMove = 0;    
        
        if(!g_pD3dApp->m_bChatMode && m_bReflexionKeyBlock == FALSE &&
            g_pD3dApp->m_bFixUnitDirMode == FALSE &&                    // 유닛 진행 방향 고정 모드
            m_bUnitStop == FALSE &&
            (g_pTutorial->IsTutorialMode() == FALSE || g_pTutorial->GetLesson() >= L2) )    // 튜토리얼 레슨 2부터
        {
            int nOldBoosterType = m_nBooster;
            m_nBooster = BOOSTER_TYPE_MIN;        // -1:후진부스터 0:중지 1:기본부스터 2:가속부스터
            bool bFormationFlight = true;
            if(g_pTutorial->IsTutorialMode() == FALSE || g_pTutorial->GetLesson() >= L3)    // 튜토리얼 레슨 3부터
            {
                // 2008-11-13 by bhsohn 조이스틱 작업
                // 2010. 03. 26 by jskim 출격 중 키보드 클릭 못하도록 수정
                //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_D) || m_bKeyDownD || m_bRightButtonState)
                //if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_D) || m_bKeyDownD || m_bRightButtonState)
                if((g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_D) || m_bKeyDownD || m_bRightButtonState) && !IS_CITY_MAP_INDEX(m_myShuttleInfo.MapChannelIndex.MapIndex))
                //end 2010. 03. 26 by jskim 출격 중 키보드 클릭 못하도록 수정
                {// 우로 이동
                    nSideMove++;
                    bFormationFlight = false;
                    g_pTutorial->m_Lesson3State.bRMove = TRUE;
                }
                // 2008-11-13 by bhsohn 조이스틱 작업
                // 2010. 03. 26 by jskim 출격 중 키보드 클릭 못하도록 수정
                //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_A) || m_bKeyDownA || m_bLeftButtonState)
                if((g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_A) || m_bKeyDownA || m_bLeftButtonState) && !IS_CITY_MAP_INDEX(m_myShuttleInfo.MapChannelIndex.MapIndex))
                //end 2010. 03. 26 by jskim 출격 중 키보드 클릭 못하도록 수정
                {// 좌로 이동
                    nSideMove--;
                    bFormationFlight = false;
                    g_pTutorial->m_Lesson3State.bLMove = TRUE;
                }
            }
            // 2008-11-13 by bhsohn 조이스틱 작업
            //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W) || m_bUpButtonState)
            if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W) || m_bUpButtonState)
            {
            }
            // 2008-11-13 by bhsohn 조이스틱 작업
            // 2010. 03. 26 by jskim 출격 중 키보드 클릭 못하도록 수정
            //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S) || m_bDownButtonState)
            if((g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S) || m_bDownButtonState) && !IS_CITY_MAP_INDEX(m_myShuttleInfo.MapChannelIndex.MapIndex))
            //end 2010. 03. 26 by jskim 출격 중 키보드 클릭 못하도록 수정
            {// 저속
                m_nBooster = BOOSTER_TYPE_STOP;                // 2006-01-19 by ispark, 저속 이펙트
                bFormationFlight = false;
            }
            if(!g_pD3dApp->m_bChatMode && !m_bReflexionKeyBlock &&
                // 2008-11-13 by bhsohn 조이스틱 작업
                //(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE)) &&
                (g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE)) &&
                (!m_bKeyDownA && !m_bKeyDownD) &&
                m_bPenaltyUnitStop == FALSE)                // 2005-08-09 by ispark 유닛정지 페널티가 사용중이 아니라면
            {
                // 2005-04-07 by jschoi
                if(g_pTutorial->IsTutorialMode() == FALSE || g_pTutorial->GetLesson() >= L4)
                {
                    if( m_myShuttleInfo.CurrentEP && 
                        m_bReChargeBURN &&
                        m_bUseBooster &&
                        (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo)) < WEIGHT_RATE_VERY_HEAVY)
                    {
                        m_nShuttleStopState = 0;
                        m_nBooster = BOOSTER_TYPE_MAX;
//                        nFBMove = 1;
                        bFormationFlight = false;
                        m_fAdvanceTime = 10.0f;
                    }
                }
            }
//            if(m_bFormationFlight && !bFormationFlight)
            // 2006-01-06 by ispark, 롤링 사용시 편대 해제
            if(m_bKeyDownA || m_bKeyDownD)
            {
//                if(m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER &&
//                    m_pClientParty->IsFormationFlight())
//                {
//                    g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(FLIGHT_FORM_NONE);
//                }
                bFormationFlight = false;
            }

            if(    m_pClientParty &&
                m_pClientParty->IsFormationFlightMove() &&
                !bFormationFlight)
            {
                // 2011-08-12 by hsson 편대 대열에 추가/탈퇴 시 딜레이를 삽입 (편대 동기화 버그를 줄임)
                //if( g_cTimeMgr.CheckTime( 1, 200 ) )
                {
                    SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
                    m_pClientParty->FormationFlightClear();
                    //m_infoParty.bFormationFlyingPositionType = 0;
                    //m_bFormationFlight = FALSE; // 편대 비행을 켄슬하고 본인의 의지대로 비행한다.
                    wsprintf(message,STRMSG_C_PARTY_0001);//"편대비행에서 벗어났습니다."
                    g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
                    
                    // 해제시 방향값 초기화 
                    m_fCurrentSideRate = 0.0f;
                    m_fCurrentAngle = 0.0f;
                }
                // end 2011-08-12 by hsson 편대 대열에 추가/탈퇴 시 딜레이를 삽입 (편대 동기화 버그를 줄임)
            }
        }
        else
        {// 채팅 모드일때 에어 브레이크가 아니라면 부스터의 상태를 일반 키를 뗀 상태로 바꾼다
            // 2005-08-11 by ispark
            // 정지 페널티 출발시 STOP부스터 쓰지 않게
            if(m_nBooster != BOOSTER_TYPE_AIRBREAK && m_bPenaltyUnitStop == FALSE)
            {
                m_nBooster = BOOSTER_TYPE_STOP;
            }
        }

        // 유닛 정지 시스템 처리
        // 2006-06-23 by ispark, 캐쉬 상점 이용시 사용 금지
        // 2008-11-13 by bhsohn 조이스틱 작업
        //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W) && 
        if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W) && 
            !g_pD3dApp->m_bChatMode && 
            g_pD3dApp->m_dwGameState != _SHOP &&
            (!g_pTutorial->IsTutorialMode() ||
            (g_pTutorial->IsTutorialMode() && g_pTutorial->GetChapterState() != TUTORIAL_STATE_READY && g_pTutorial->GetLesson() != L6)))
        {
            // W키를 누렀을 때 유닛 정지를 푼다.
            if(m_bUnitStop == TRUE && m_bSkillGoMove )
            {
                // 2008-07-03 by bhsohn EP3 버그 리포트 수정
                if(g_pGameMain->m_pCommunity->IsShowCommunityWindow())
                {
                    // 출발시 강제로 창닫음
                    g_pGameMain->m_pCommunity->ShowCommunityWindow(FALSE);
                }
                if(g_pGameMain->m_pSystem && g_pGameMain->m_pSystem->IsOptionShow())
                {
                    g_pGameMain->PopupSystemOptinWnd();
                }
                // end 2008-07-03 by bhsohn EP3 버그 리포트 수정

                m_bUnitStop = FALSE;
                m_bPenaltyUnitStop = TRUE;
                m_bMouseMoveLock = FALSE;
                m_nBooster = BOOSTER_TYPE_MIN;
                // 2010. 07. 07 by jskim 홀드 맞을시 정지 풀때 안풀리는 버그 수정
                if( m_pSkill->IsExistDesParamSKill( DES_SKILL_MON_HOLD ) )
                {
                    m_bMouseLock = FALSE;
                    g_pD3dApp->m_bFixUnitDirMode = FALSE;
                }
                //end 2010. 07. 07 by jskim 홀드 맞을시 정지 풀때 안풀리는 버그 수정
                g_pInterface->m_pGameMain->InitShowWindow();            // 2005-08-05 by ispark
                // 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달    
                SendFieldSocketChangeBodyCondition( m_myShuttleInfo.ClientIndex, ShuttleChangeBodyCondition());        // 2013-06-25 by ssjung 데미지 이펙트가 안나오는 현상 수정
                //end 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달    
            }
        }
        else if(m_bUnitStop == TRUE)
        {
            // 유닛 정지시 부스터 상태
            m_nBooster = BOOSTER_TYPE_AIRBREAK;
        }
            
        if( g_pGameMain && 
            g_pGameMain->m_pMission && 
            g_pGameMain->m_pMission->m_bShow)
        {
            m_nBooster = BOOSTER_TYPE_AIRBREAK;
        }
        if(m_bOrderMove && m_nBooster == BOOSTER_TYPE_MAX)
        {
            m_bOrderMove = FALSE;
        }
        if(!m_pClientParty->IsFormationFlightMove() && 
//            m_nBooster != BOOSTER_TYPE_AIRBREAK &&
            !m_bOrderMove)
        {
//            CheckMoveRate(fElapsedTime,nSideMove,nFBMove);
            // 2007-03-27 by dgwoo 옵저버 모드시 움직임을 막는다.
            if(!IsObserverMode())
            {
                CheckMoveRate(fElapsedTime,nSideMove);
            }
            if(m_myShuttleInfo.CurrentEP)
                m_vPos += (m_vNextPos - m_vPos)*fElapsedTime;
            else    // EP가 없을때-속도를 반으로 줄인다.
                m_vPos += 0.5f*(m_vNextPos - m_vPos)*fElapsedTime;
        }
        if(g_pD3dApp->m_pCamera->m_bIsCamControl)
        {
            m_fSideCheckRate = 0.0f;
        }
        else if(m_nBooster == BOOSTER_TYPE_AIRBREAK)
        {
            if(m_fSideCheckRate > 0.5f)
                m_fSideCheckRate -= 0.5f*fElapsedTime;
            if(m_fSideCheckRate < 0.5f)
                m_fSideCheckRate = 0.5f;
        }
        else
        {
            m_fSideCheckRate = 2.0f;    // 기울임 조절변수 1.0 으로 셋팅
        }
        CheckCollForMap();        // 맵과의 충돌 검사
        CheckCollForObject(fElapsedTime);    // 오브젝트와 충돌 검사 b기어
        if( !g_pD3dApp->m_bChatMode && m_nBooster != BOOSTER_TYPE_MAX &&
            // 2008-11-13 by bhsohn 조이스틱 작업
            //((g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_C) &&    // 20s05-11-07 by ispark 푼다
            ((g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_C) &&    // 20s05-11-07 by ispark 푼다
            (!m_bKeyDownA && !m_bKeyDownD)) ||
//            ((g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_C) &&
//            IS_DT(m_myShuttleInfo.UnitKind))||        // 2005-08-03 by ispark A기어 이외는 C버튼 사용 금지
            g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_NUMPAD0) || 
            g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_BACKSLASH)) &&
            g_pD3dApp->m_dwGameState != _SHOP &&
            m_bSkillGoMove)
        {
            TryLandingState();
            // 2007-07-26 by dgwoo C버튼은 착륙시에 사용.
            //InitUnitStop();
        }

        // 2005-11-16 by ispark    
        // B버튼 이외는 수평 유지 삭제
//        if(m_bMButtonState ||                            // 2005-08-09 by ispark 마우스 M버튼 삭제
//        if( m_bIsSetFlighting || 
//            g_pD3dApp->m_bChatMode || 
//            m_nBooster == BOOSTER_TYPE_AIRBREAK ||
//            g_pD3dApp->m_bFixUnitDirMode == TRUE)        // 유닛 진행 방향 고정 모드
        if(g_pD3dApp->m_bFixUnitDirMode == TRUE)
        {// 평형 마추기
            SetShuttleFlightInit(fElapsedTime);
        }

        // 2005-02-01 by jschoi
        if(    (g_pSOption->sMenuBalance == TRUE ||
            g_pGameMain->m_pSystem->m_bQuitGameMessage == TRUE)&&            // 메뉴사용시 유닛정지 옵션 체크
            (g_pD3dApp->m_bMessageBox == TRUE || 
            g_pD3dApp->m_bFixUnitDirMode == TRUE) )
        {// 인터페이스 창이 떠있는 경우 기어를 서서히 감속한다.
            m_nShuttleStopState = 4;
        }

        // 2005-08-22 by ispark
        // 비행중 연료가 0일 때 추락
        if(0 >= m_myShuttleInfo.CurrentEP)
        {
            m_nBooster = BOOSTER_TYPE_AIRBREAK;
            ChangeUnitStateDead();
        }

        
        // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
        //CheckBoosterAni();

        // 로봇기어 롤링애니메이션시 부스터 애니메이션이 덮어쓰므로 롤링시 부스터 애니메이션 체크를 안한다
        if( !( m_bKeyDownA || m_bKeyDownD ) )
            CheckBoosterAni();
        // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    }
    else if(m_dwState == _LANDING)
    {
        // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리
        BOOL bLandingDone = FALSE;

        // 2005-11-11 by ispark
        // 대결중에는 지우지 않음
        if(m_pOrderTarget &&
            m_dwPkState != PK_PVP &&
            m_dwPkState != PK_FVF && 
            m_pOrderTarget->m_dwPartType != _ENEMY &&
            !IS_DT(m_myShuttleInfo.UnitKind))                // 2006-05-19 by ispark, A기어라면 타겟 삭제 제외
        {
            m_pTarget = NULL;
            m_pOrderTarget = NULL;
        }
        m_nBooster = BOOSTER_TYPE_STOP;
        m_bMouseMoveLock = TRUE;                            // 2005-08-05 by ispark 착륙시 움직임 막는다.

        // 착륙시 1인칭이라면 3인칭으로 변경
        if(g_pD3dApp->m_pCamera->GetCamType() == CAMERA_TYPE_FPS)
        {
            g_pD3dApp->m_pCamera->SetCamType(CAMERA_TYPE_NORMAL);
            g_pShuttleChild->m_bLandingOldCamtypeFPS = TRUE;
        }        
        if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_LANDING_MASK))
        {
            ChangeSingleBodyCondition(BODYCON_LANDING_MASK);
        }
        if(m_bOrderMove)
        {
            D3DXVECTOR3 vtempPos = m_vPos;
            if(MoveOrder(m_bOrderMoveType))
            {
                m_bOrderMove = FALSE;
            }
            m_vMoveVel = m_vPos - vtempPos;
            D3DXVec3Normalize(&m_vMoveVel, &m_vMoveVel);            // 2006-07-13 by ispark, 기어 이동 방향
        }
        else
        {
            if(m_fSideCheckRate > 0.0f)
                m_fSideCheckRate -= 0.5f*fElapsedTime;
            if(m_fSideCheckRate < 0.0f)
                m_fSideCheckRate = 0.0f;
            // 평형 마추기
            SetShuttleFlightInit(fElapsedTime);
            CheckMoveRate(fElapsedTime,0);
            if(m_myShuttleInfo.CurrentEP)
                m_vPos += (m_vNextPos - m_vPos)*fElapsedTime;
            else    // EP가 없을때-속도를 반으로 줄인다.
                m_vPos += 0.5f*(m_vNextPos - m_vPos)*fElapsedTime;

            // 2007-05-15 by bhsohn A기어 보완
            //m_vPos.y -= 15.0f*fElapsedTime;
            if(IS_DT(m_myShuttleInfo.UnitKind))
            {
                m_vPos.y -= AGEAR_LANDING_SPEED*fElapsedTime;
            }
            else
            {
                m_vPos.y -= 15.0f*fElapsedTime;
            }

            float fTempHeight = 100.0f;
            // 오브젝트 검사
            if(m_bOnObject)
            {
                D3DXMATRIX matTemp;
                D3DXVECTOR3 vSide;
                vSide = D3DXVECTOR3(0,0,1);
                D3DXMatrixLookAtLH(&matTemp,&m_vPos,&(m_vPos - m_vBasisUp),&vSide);
                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vPos, FALSE).fDist;
            }
            float fCheckMapHeight = CheckMap(m_vPos);
            if(m_vPos.y <= fCheckMapHeight+m_fAmorHeight || m_fAmorHeight >= fTempHeight)
            {
                if(fCheckMapHeight + m_fAmorHeight > m_vPos.y + m_fAmorHeight - fTempHeight)
                    m_vPos.y = fCheckMapHeight + m_fAmorHeight;
                else
                    m_vPos.y += m_fAmorHeight - fTempHeight;

                // 2011-02-25 by shcho&hsSon, A기어 기체와 카메라가 뒤집히던 문제 수정
                // 2011-04-20 by hsson A기어 자동수평 유지 옵션을 껏을 때 착륙시 공격 못 하던 버그
                if( 0 > m_vUp.y )
                {
                    // 음수면 기체가 뒤집혀 있는것이다. 뒤집혀 있는면 y축을 천천히 보정해 준다.
                    m_vUp.y += 0.01f;
                }
                else
                {
                    // 양수일경우 제대로 서있으므로 괜찮다.
                    SendPacket(T_FC_MOVE_LANDING_DONE);
                    DBGOUT("T_FC_MOVE_LANDING_DONE\n");
                    ChangeUnitState( _LANDED );
                }
                // end 2011-04-20 by hsson A기어 자동수평 유지 옵션을 껏을 때 착륙시 공격 못 하던 버그
                // end 2011-02-25 by shcho&hsSon, A기어 기체와 카메라가 뒤집히던 문제 수정

                // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리
                bLandingDone = TRUE;

                // 카메라 복구
                m_bIsCameraPattern = FALSE;
            }
            // 물속인지 체크
            if(m_bIsWater && m_vPos.y < g_pGround->m_projectInfo.fWaterHeight)
                m_bIsUnderWater = TRUE;
            // 2008-11-13 by bhsohn 조이스틱 작업
            //if(!m_bKeyBlock && !g_pD3dApp->m_bChatMode && !m_bReflexionKeyBlock && ( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE)))
            if(!m_bKeyBlock && !g_pD3dApp->m_bChatMode && !m_bReflexionKeyBlock && ( g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE)))
            {
                // 2005-04-07 by jschoi
                if(g_pTutorial->IsTutorialMode() == FALSE || g_pTutorial->GetLesson() >= L4)
                {
                    if( m_myShuttleInfo.CurrentEP && 
                        m_bReChargeBURN && 
                        m_bUseBooster &&
                        (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo)) < 0.8f)
                    {
                        m_nShuttleStopState = 0;
                        m_nBooster = BOOSTER_TYPE_MAX;
                    }
                }
            }
        }

        // 2005-10-26 by ispark
        CheckCollForObject(fElapsedTime);    // 오브젝트와 충돌 검사 b기어

        // 착륙중 다시 이륙
        if( !m_bKeyBlock && 
            !g_pD3dApp->m_bChatMode &&
            !m_bReflexionKeyBlock &&
            // 2008-11-13 by bhsohn 조이스틱 작업
            //(m_bUpButtonState || g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_SPACE )))
            (m_bUpButtonState || g_pD3dApp->GetAsyncKeyState_DIK_DIJ( DIK_SPACE )))
        {
            // 2005-04-07 by jschoi
            if(g_pTutorial->IsTutorialMode() == FALSE)
            {
                if(m_fNumpad0keyTime <= 0.0f)
                {
                    // 착륙전 시점이 1인칭이였으면 다시 1인칭으로 변환 
                    if(m_bLandingOldCamtypeFPS)
                    {
                        g_pD3dApp->m_pCamera->ChangeCamType();
                        m_bLandingOldCamtypeFPS = FALSE;
                    }

                    SendPacket(T_FC_MOVE_TAKEOFF);
                    DBGOUT("T_FC_MOVE_TAKEOFF\n");
                    g_pCamera->Init(m_vPos - 10.0f*m_vVel,m_vVel, 3.14f/13.0f, 60.0f);
                    m_fNumpad0keyTime = 2.0f;
                    ChangeUnitState(_TAKINGOFF);
                    // 2007-05-15 by bhsohn A기어 보완
                    //m_fRasingCheckTime = 1.0f;
                    m_fRasingCheckTime = GetRasingCheckTime();
                }
            }
        }
        // _LANDING 카메라 패턴 적용
        if(m_bIsCameraPattern == TRUE)
        {
            EVENT_POINT CameraPoint;
            m_pCinemaCamera->SkillTick();
            D3DXVECTOR3 vCameraPos, vCameraVel, vCameraUp;
            CameraPoint=m_pCinemaCamera->GetCurrentCinemaPoint();
            vCameraPos = CameraPoint.vPosition;
            vCameraVel = m_vPos - CameraPoint.vPosition;
            vCameraUp = CameraPoint.vUpVector;
            
            // 카메라의 위치 셋팅 2004.06.24 jschoi
            g_pD3dApp->m_pCamera->Tick(vCameraPos,vCameraVel,vCameraUp);
            
        }
        // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리        
        if(bLandingDone && IS_DT(m_myShuttleInfo.UnitKind))
        {
            // A기어 라면 움직임 초기화
            InitUnitStop();
        }        
        // end 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리
    }
    else if(m_dwState == _LANDED)
    {
        // 2005-11-11 by ispark
        // 대결중에는 지우지 않음
        if(m_pOrderTarget &&
            m_dwPkState != PK_PVP &&
            m_dwPkState != PK_FVF && 
            m_pOrderTarget->m_dwPartType != _ENEMY &&
            !IS_DT(m_myShuttleInfo.UnitKind))                // 2006-05-19 by ispark, A기어라면 타겟 삭제 제외
        {
            m_pTarget = NULL;
            m_pOrderTarget = NULL;
        }
        m_nBooster = BOOSTER_TYPE_BACK;
        m_bMouseMoveLock = TRUE;                            // 2005-08-05 by ispark

        if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_LANDED_MASK))
        {
            ChangeSingleBodyCondition(BODYCON_LANDED_MASK);
        }
        if(m_fSideCheckRate > 0.0f)
            m_fSideCheckRate -= 0.5f*fElapsedTime;
        if(m_fSideCheckRate < 0.0f)
            m_fSideCheckRate = 0.0f;
        SetShuttleFlightInit(fElapsedTime);
        CheckMoveRate(fElapsedTime,0);
        if(m_myShuttleInfo.CurrentEP)
            m_vPos += (m_vNextPos - m_vPos)*fElapsedTime;
        else    // EP가 없을때-속도를 반으로 줄인다.
            m_vPos += 0.5f*(m_vNextPos - m_vPos)*fElapsedTime;

        float fTempHeight = 100.0f;
        if(m_bOnObject)
        {
            D3DXMATRIX matTemp;
            D3DXVECTOR3 vSide;
            vSide = D3DXVECTOR3(0,0,1);
            D3DXMatrixLookAtLH(&matTemp,&m_vPos,&(m_vPos - m_vBasisUp),&vSide);
            fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vPos, FALSE).fDist;
        }
        float fCheckMapHeight = CheckMap(m_vPos);
        if(fCheckMapHeight + m_fAmorHeight > m_vPos.y + m_fAmorHeight - fTempHeight)
        {
            m_vPos.y = fCheckMapHeight + m_fAmorHeight;
        }
        else
        {
            if((m_fAmorHeight - fTempHeight) <= 1.0f && (m_fAmorHeight - fTempHeight) >= -1.0f)
            {
                m_vPos.y += m_fAmorHeight - fTempHeight;
            }
            else
            {
                if(m_fAmorHeight - fTempHeight > 0) 
                {
                    m_vPos.y += m_fAmorHeight - fTempHeight;
                }
                else
                {
                    float fRealObjectHeight = m_vPos.y - fTempHeight;
                    m_vPos.y -= fElapsedTime*50.0f;        // 착륙된 상태에서 바닥으로 떨어지는 량            
                    if( fRealObjectHeight + m_fAmorHeight > m_vPos.y )
                    {
                        m_vPos.y = fRealObjectHeight + m_fAmorHeight;            // 기어가 땅(오브젝트)속으로는 들어갈 수 없다.
                    }
                }
            }
        }
        
        // 지형 및 오브젝트의 높이를구하여 셔틀의 높이를 적용한다.    
        m_bFirstStart = FALSE;
        // 물속인지 체크
        if(m_bIsWater && m_vPos.y < g_pGround->m_projectInfo.fWaterHeight)
            m_bIsUnderWater = TRUE;
        // 2008-11-13 by bhsohn 조이스틱 작업
        //if(!m_bKeyBlock && !g_pD3dApp->m_bChatMode && !m_bReflexionKeyBlock && ( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE)))
        if(!m_bKeyBlock && !g_pD3dApp->m_bChatMode && !m_bReflexionKeyBlock && ( g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE)))
        {
            // 2005-04-07 by jschoi
            if(g_pTutorial->IsTutorialMode() == FALSE || g_pTutorial->GetLesson() == L4)
            {
                if( m_myShuttleInfo.CurrentEP && 
                    m_bReChargeBURN && 
                    m_bUseBooster &&
                    (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo)) < 0.8f)
                {
                    m_nShuttleStopState = 0;
                    m_nBooster = BOOSTER_TYPE_MAX;
                }
            }
        }
        // 2006-11-02 by dgwoo 도시 맵에선 C버튼을 막음으로서 
        // 버그수정(착륙시 C버튼을 누르고 있을면 보급창이 안뜬다.)
        if( !IS_CITY_MAP_INDEX(m_myShuttleInfo.MapChannelIndex.MapIndex) &&
            !g_pD3dApp->m_bChatMode &&
            !m_bReflexionKeyBlock &&
            !m_bKeyBlock &&
            // 2008-11-13 by bhsohn 조이스틱 작업
            //(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_C) || 
            (g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_C) || 
            m_bUpButtonState || 
            // 2008-11-13 by bhsohn 조이스틱 작업
            //g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_SPACE )))
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ( DIK_SPACE )))
        {
            m_bLandedMove = FALSE;
            g_pInterface->m_pGameMain->InitShowWindow();
            // 2005-04-07 by jschoi
            if(g_pTutorial->IsTutorialMode() == FALSE)
            {
                if(m_fNumpad0keyTime <= 0.0f)
                {
                    // 착륙전 시점이 1인칭이였으면 다시 1인칭으로 변환 
                    if(m_bLandingOldCamtypeFPS)
                    {
                        g_pD3dApp->m_pCamera->ChangeCamType();
                        m_bLandingOldCamtypeFPS = FALSE;
                    }

                    SendPacket(T_FC_MOVE_TAKEOFF);
                    DBGOUT("T_FC_MOVE_TAKEOFF\n");
                    g_pCamera->Init(m_vPos - 10.0f*m_vVel,m_vVel, 3.14f/13.0f, 60.0f);
                    m_fNumpad0keyTime = 2.0f;
                    ChangeUnitState(_TAKINGOFF);                    
                    // 2007-05-15 by bhsohn A기어 보완
                    //m_fRasingCheckTime = 1.0f;
                    m_fRasingCheckTime = GetRasingCheckTime();

                }
            }
        }

        // 2005-10-26 by ispark
        if(m_bLandedMove == FALSE)
            CheckCollForObject(fElapsedTime);    // 오브젝트와 충돌 검사 b기어

        // _LANDING 카메라 패턴 적용
        if(m_bIsCameraPattern == TRUE)
        {
            EVENT_POINT CameraPoint;
            m_pCinemaCamera->SkillTick();
            D3DXVECTOR3 vCameraPos, vCameraVel, vCameraUp;
            CameraPoint=m_pCinemaCamera->GetCurrentCinemaPoint();
            vCameraPos = CameraPoint.vPosition;
            vCameraVel = m_vPos - CameraPoint.vPosition;
            vCameraUp = CameraPoint.vUpVector;
            
            // 카메라의 위치 셋팅 2004.06.24 jschoi
            g_pD3dApp->m_pCamera->Tick(vCameraPos,vCameraVel,vCameraUp);
            
        }
        if(m_fShuttleSpeed <= 0)
        {
            m_bIsCameraPattern = FALSE;
        }
    }
    else if(m_dwState == _TAKINGOFF)
    {
        InitRolling();
        m_bMouseMoveLock = FALSE;                // 2005-08-05 by ispark        
        m_bIsCameraPattern = FALSE;

        if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_TAKEOFF_MASK))
        {
            ChangeSingleBodyCondition(BODYCON_TAKEOFF_MASK);
        }
        if(m_fRasingCheckTime > 0.0f)
            m_fRasingCheckTime -= fElapsedTime;
        if(m_fRasingCheckTime <= 0.0f)
        {
            m_fRasingCheckTime = 0.0f;
            ChangeUnitState( _NORMAL );
            // 2007-07-26 by dgwoo 이륙시 위치에 도달하면 앞으로 전진.
            InitUnitStop();
        }
        D3DXVECTOR3 vtempPos = m_vPos;
        
        // 2007-05-15 by bhsohn A기어 보완
//        m_vPos.y += 30.0f*fElapsedTime;                
//        m_vPos += 30.0f*m_vVel*fElapsedTime;
        if(IS_DT(m_myShuttleInfo.UnitKind))
        {
            m_vPos.y += AGEAR_RASING_SPEED*fElapsedTime;                
            m_vPos += AGEAR_RASING_SPEED*m_vVel*fElapsedTime;
        }
        else
        {
            m_vPos.y += 30.0f*fElapsedTime;                
            m_vPos += 30.0f*m_vVel*fElapsedTime;
        }

        m_vMoveVel = m_vPos - vtempPos;
        D3DXVec3Normalize(&m_vMoveVel, &m_vMoveVel);            // 2006-07-13 by ispark, 기어 이동 방향
        //m_myShuttleInfo.BodyCondition = BODYCON_TAKEOFF_MASK;
        // 2005-01-19 by jschoi - 이륙 시 충돌 처리
        CheckCollForMap();        // 맵과의 충돌 검사
        CheckCollForObject(fElapsedTime);    // 오브젝트와 충돌 검사 b기어
    }
    else if(m_dwState == _FALLING)
    {
        D3DXVECTOR3 vtempPos = m_vPos;
        if(m_pClientParty->IsFormationFlightMove())
        {
            SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
            m_pClientParty->FormationFlightClear();
            wsprintf(message,STRMSG_C_PARTY_0001);//"편대비행에서 벗어났습니다."
            g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
        }
        EVENT_POINT UnitPoint, CameraPoint;
        if(m_pCinemaUnitPattern)
        {
            m_pCinemaUnitPattern->SkillTick();
            UnitPoint=m_pCinemaUnitPattern->GetCurrentCinemaPoint();
            m_vPos = UnitPoint.vPosition;                // 위치
            m_vVel = UnitPoint.vDirection;                // 방향
            m_vUp = UnitPoint.vUpVector;
            m_vTargetPosition = UnitPoint.vTarget;
        }

        // 2005-08-01 by ispark
        // 카메라 패턴 사용 안함
//        if(m_bIsCameraPattern == TRUE)
//        {
//            m_pCinemaCamera->SkillTick();
//            D3DXVECTOR3 vCameraPos, vCameraVel, vCameraUp;
//            CameraPoint=m_pCinemaCamera->GetCurrentCinemaPoint();
//            vCameraPos = CameraPoint.vPosition;
//            vCameraVel = m_vPos - CameraPoint.vPosition;
//            vCameraUp = CameraPoint.vUpVector;
//            
//            // 카메라의 위치 셋팅 2004.06.24 jschoi
//            g_pCamera->Tick(vCameraPos,vCameraVel,vCameraUp);
//            
//        }
        m_vMoveVel = m_vPos - vtempPos;
        D3DXVec3Normalize(&m_vMoveVel, &m_vMoveVel);            // 2006-07-13 by ispark, 기어 이동 방향


        // 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가

//         // 추락시 충돌 처리 2004.06.19 jschoi
//         BOOL bResult = CheckCollMapObject();
//         if(m_vPos.y < CheckMap(m_vPos) + 2.0f || bResult)
//         {
//             ChangeUnitState( _FALLEN );
//             g_fSendMovePacket = max(g_fSendMovePacket,5.0f);
// 
//             // 2005-08-22 by ispark
//             // 연료가 0일 때 추락후 보여줄 메시지 박스와 메시지 처리
//             // 2005-12-16 by ispark
//             // 죽은상태가 아닌 경우만 FUELALLIN 처리
//             if(FALSE == COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_DEAD_MASK)
//                 && 0 >= m_myShuttleInfo.CurrentEP)// && 
//             //    !COMPARE_BODYCON_BIT(g_pShuttleChild->m_myShuttleInfo.BodyCondition,BODYCON_DEAD_MASK))
//             {
//                 ChangeUnitState( _FUELALLIN );
//                 // 메세지 처리
//                 g_pFieldWinSocket->WriteMessageType(T_FC_CHARACTER_FALLING_BY_FUEL_ALLIN);
//                 g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_050819_0100, COLOR_ERROR);
//             }
//             else
//             {
//                 // 2005-03-28 by jschoi - 죽었을 때 부활 카드 사용
//                 CItemInfo* pItemInfo = g_pStoreData->SerchRevivalUseItem();
//                 MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
// // 2005-12-05 by ispark, 
// //                if(g_pD3dApp->m_bDeadByP2PPK == TRUE)// 1:1결투하다가 적에게 죽은 경우
//                 // 2006-01-02 by ispark, 다른 세력맵이면
//                 // 2006-04-10 by ispark, 부활 카드 수정, 1:1 결투는 제외
//                 // 2006-11-17 by ispark, 이벤트 맵에서는 제외
//                 // 2007-05-15 by dgwoo, 아레나 맵에서 제외.
//                 if(g_pD3dApp->m_bDeadByP2PPK == FALSE && 
//                     pItemInfo &&
//                     IsRevivalUseItem())
//                 {
//                     char strRevivalMsg[512] = {0,};
//                     // 2009-04-21 by bhsohn 아이템 DesParam추가
//                     //sprintf(strRevivalMsg, STRMSG_C_060410_0003, pItemInfo->ItemInfo->ItemName, pItemInfo->ItemInfo->ParameterValue1);    // "%s를 사용하시겠습니까? 확률은 %.0f입니다."
//                     sprintf(strRevivalMsg, STRMSG_C_060410_0003, pItemInfo->ItemInfo->ItemName, pItemInfo->ItemInfo->ArrParameterValue[0]);    // "%s를 사용하시겠습니까? 확률은 %.0f입니다."
//                     g_pGameMain->m_pInfWindow->AddMsgBox(strRevivalMsg, _Q_USE_NORMAL_RESTORE,0,0,0,pItemInfo->UniqueNumber);
//                 }
//                 else
//                 {
//                     // 2006-11-17 by ispark, 부활 카드가 있지만 이벤트 맵이라면
//                     if(pItemInfo && IS_MAP_INFLUENCE_EVENT_AREA(pMapInfo->MapInfluenceType))
//                     {
//                         char chMsg[512] = {0,};
//                         wsprintf(chMsg, STRMSG_C_061117_0000, pItemInfo->ItemInfo->ItemName);    // "이벤트맵에서는 %s 아이템을 사용할 수 없습니다."
//                         g_pD3dApp->m_pChat->CreateChatChild(chMsg, COLOR_ERROR);
//                     }
// 
//                     SendDeadMsg();
//                 }
// 
// 
// // 2005-12-05 by ispark, 부활 카드 삭제
// //                else if(pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(ITEM_NUM_CASH_CASH_NORMAL_RESTORE))
// //                {
// //                    g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_CARD_0001, _Q_USE_NORMAL_RESTORE,0,0,0,pItemInfo->UniqueNumber);//"일반 부활 카드를 사용하시겠습니까? (취소시 격납고로 이동합니다.)"
// //                }
// //                else if(pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(ITEM_NUM_CASH_CASH_SUPER_RESTORE))
// //                {
// //                    g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_CARD_0002, _Q_USE_SUPER_RESTORE,0,0,0,pItemInfo->UniqueNumber);//"슈퍼 부활 카드를 사용하시겠습니까? (취소시 격납고로 이동합니다.)"
// //                }
// //                else
// //                {// 부활 카드가 없다면
// //                    g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_CARD_0003, _Q_DEAD_RETURN);    //"격납고로 이동합니다."
// //                }
// 
//                 ResetBodyCondition(BODYCON_EXPLOSION_MASK);
//             }
//         }
        

        // 추락시 충돌 처리
        BOOL bResult = CheckCollMapObject();
        if( m_vPos.y < CheckMap(m_vPos) + 2.0f || bResult )
        {
            ChangeUnitState( _FALLEN );
            g_fSendMovePacket = max(g_fSendMovePacket,5.0f);

            if( m_bDeadMessageEnable )
            {
                m_bDeadMessageEnable    = FALSE;

                // 연료가 0일 때 추락후 보여줄 메시지 박스와 메시지 처리
                // 죽은상태가 아닌 경우만 FUELALLIN 처리
                if( FALSE == COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_DEAD_MASK)
                    && 0 >= m_myShuttleInfo.CurrentEP )
                {
                    ChangeUnitState( _FUELALLIN );
                    // 메세지 처리
                    g_pFieldWinSocket->WriteMessageType(T_FC_CHARACTER_FALLING_BY_FUEL_ALLIN);
                    g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_050819_0100, COLOR_ERROR);
                }
                else
                {
                    // 죽었을 때 부활 카드 사용
                    CItemInfo* pItemInfo    = g_pStoreData->SerchRevivalUseItem();
                    MAP_INFO* pMapInfo        = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
                    // 1:1결투하다가 적에게 죽은 경우
                    // 다른 세력맵이면
                    // 부활 카드 수정, 1:1 결투는 제외
                    // 이벤트 맵에서는 제외
                    // 아레나 맵에서 제외.
                    if(g_pD3dApp->m_bDeadByP2PPK == FALSE && 
                        pItemInfo &&
                        IsRevivalUseItem())
                    {
                        char strRevivalMsg[512] = {0,};
                        // "%s를 사용하시겠습니까? 확률은 %.0f입니다."
                        sprintf(strRevivalMsg, STRMSG_C_060410_0003, pItemInfo->ItemInfo->ItemName, pItemInfo->ItemInfo->ArrParameterValue[0]);
                        g_pGameMain->m_pInfWindow->AddMsgBox(strRevivalMsg, _Q_USE_NORMAL_RESTORE,0,0,0,pItemInfo->UniqueNumber);
                    }
                    else
                    {
                        if(pItemInfo && IS_MAP_INFLUENCE_EVENT_AREA(pMapInfo->MapInfluenceType))
                        {
                            char chMsg[512] = {0,};
                            // "이벤트맵에서는 %s 아이템을 사용할 수 없습니다."
                            wsprintf(chMsg, STRMSG_C_061117_0000, pItemInfo->ItemInfo->ItemName);
                            g_pD3dApp->m_pChat->CreateChatChild(chMsg, COLOR_ERROR);
                        }

                        SendDeadMsg();
                    }

                    ResetBodyCondition(BODYCON_EXPLOSION_MASK);
                }
            }
        }

        if( m_dwDeadTime + FALLING_TIME < timeGetTime() )
        {
            if( m_bDeadMessageEnable )
            {
                m_bDeadMessageEnable    = FALSE;

                // 연료가 0일 때 추락후 보여줄 메시지 박스와 메시지 처리
                // 죽은상태가 아닌 경우만 FUELALLIN 처리
                if( FALSE == COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_DEAD_MASK)
                    && 0 >= m_myShuttleInfo.CurrentEP )
                {
                    ChangeUnitState( _FUELALLIN );
                    // 메세지 처리
                    g_pFieldWinSocket->WriteMessageType(T_FC_CHARACTER_FALLING_BY_FUEL_ALLIN);
                    g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_050819_0100, COLOR_ERROR);
                }
                else
                {
                    // 죽었을 때 부활 카드 사용
                    CItemInfo* pItemInfo    = g_pStoreData->SerchRevivalUseItem();
                    MAP_INFO* pMapInfo        = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
                    // 1:1결투하다가 적에게 죽은 경우
                    // 다른 세력맵이면
                    // 부활 카드 수정, 1:1 결투는 제외
                    // 이벤트 맵에서는 제외
                    // 아레나 맵에서 제외.
                    if(g_pD3dApp->m_bDeadByP2PPK == FALSE && 
                        pItemInfo &&
                        IsRevivalUseItem())
                    {
                        char strRevivalMsg[512] = {0,};
                        // "%s를 사용하시겠습니까? 확률은 %.0f입니다."
                        sprintf(strRevivalMsg, STRMSG_C_060410_0003, pItemInfo->ItemInfo->ItemName, pItemInfo->ItemInfo->ArrParameterValue[0]);
                        g_pGameMain->m_pInfWindow->AddMsgBox(strRevivalMsg, _Q_USE_NORMAL_RESTORE,0,0,0,pItemInfo->UniqueNumber);
                    }
                    else
                    {
                        if(pItemInfo && IS_MAP_INFLUENCE_EVENT_AREA(pMapInfo->MapInfluenceType))
                        {
                            char chMsg[512] = {0,};
                            // "이벤트맵에서는 %s 아이템을 사용할 수 없습니다."
                            wsprintf(chMsg, STRMSG_C_061117_0000, pItemInfo->ItemInfo->ItemName);
                            g_pD3dApp->m_pChat->CreateChatChild(chMsg, COLOR_ERROR);
                        }

                        SendDeadMsg();
                    }
                }
            }
        }

        // end 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가


        // 물속인지 체크
        if(CheckIsWater(m_vPos) && m_vPos.y < g_pGround->m_projectInfo.fWaterHeight)
            m_bIsUnderWater = TRUE;
    }
    else if( m_dwState == _FALLEN)
    {
        if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_DEAD_MASK))
        {
            m_pTarget = NULL;
            m_pOrderTarget = NULL;
            m_nBooster = BOOSTER_TYPE_BACK;
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition(BODYCON_DAMAGE1_MASK,TRUE);
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition(BODYCON_DAMAGE2_MASK,TRUE);
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition(BODYCON_DAMAGE3_MASK,TRUE);
            ChangeSingleBodyCondition(BODYCON_DEAD_MASK);//BODYCON_DEAD_MASK가 있는지 확인
        }
    }
    else if(m_dwState==_EXPLODING)
    {
        m_pTarget = NULL;
        m_pOrderTarget = NULL;
        ChangeUnitState( _EXPLODED );
        //m_dwState = _EXPLODED;
    }
    else if(m_dwState==_EXPLODED)
    {
        m_pTarget = NULL;
        m_pOrderTarget = NULL;
    }
    else if(m_dwState == _WARP)
    {
        EVENT_POINT UnitPoint, CameraPoint;
        BOOL bResult = FALSE;
        if(m_pCinemaUnitPattern != NULL)
        {
            bResult = m_pCinemaUnitPattern->SkillTick();
            UnitPoint=m_pCinemaUnitPattern->GetCurrentCinemaPoint();
            m_vPos = UnitPoint.vPosition;                // 위치
            m_vVel = UnitPoint.vDirection;                // 방향
            m_vUp = UnitPoint.vUpVector;
            m_vTargetPosition = UnitPoint.vTarget;            
        }

        if(m_bIsCameraPattern == TRUE)
        {
            bResult = m_pCinemaCamera->SkillTick();
            D3DXVECTOR3 vCameraPos, vCameraVel, vCameraUp;
            CameraPoint=m_pCinemaCamera->GetCurrentCinemaPoint();
            vCameraPos = CameraPoint.vPosition;
            vCameraVel = m_vPos - CameraPoint.vPosition;
            vCameraUp = CameraPoint.vUpVector;
            g_pCamera->Tick(vCameraPos,vCameraVel,vCameraUp);
            
        }
//        m_fEventCheckTime = 2.0f;
        if(bResult)    // 카메라 패턴이 끝나던 아니면 유닛 패턴이 끝나면 종료한다.
        {
            switch(m_nEventType)
            {
            case EVENT_WARP_IN:
                {
                    if(m_pCurrentObjEvent)
                    {
                        SendFieldSocketRequestEventObjectWarpIn(m_pCurrentObjEvent);
                        m_pCurrentObjEvent = NULL;
                    }
                }
                break;
            case EVENT_WARP_OUT:
                {
                    m_bIsCameraPattern = FALSE;
                    ChangeUnitState( _NORMAL );
                }
                break;
            case EVENT_GAME_START:
                {
                    m_bIsCameraPattern = FALSE;
                    ChangeUnitState( _NORMAL );
                }
                break;
            case EVENT_GAME_END:
                {
                    SetSelectCharacterItem();
                    g_pD3dApp->SendFieldSocketCharacterGameEnd(FALSE);    // 2015-09-26 Future, standardized Game closing
                }
                break;
            }
//            m_bEventReady = FALSE; WARP_OK에서 한다.
            
        }
    }

    CheckMapRegion();
    // 2006-08-08 by ispark, 우주맵이면 한번더 충돌 검사한다.
    // 맵 끝에서 오브젝트를 뚫어버리는 버그 수정
    if(!IsTileMapRenderEnable(m_myShuttleInfo.MapChannelIndex.MapIndex) && m_dwState == _NORMAL)
    {        
        m_vMoveVel = m_vPos - m_vOldPos;
        CheckCollForObject(fElapsedTime);
    }

    // 못가는 지역체크하여 빽시킨다.
    int nXCheck, nYCheck;
    nXCheck = (int)(m_vPos.x / TILE_SIZE);
    nYCheck = (int)(m_vPos.z / TILE_SIZE);
    m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize + nYCheck].bMove;
    if(m_bMoveGroundType == 2)
    {
        m_fShuttleSpeed *= -0.5f;
        m_fLRVelRate *= 0.8f;

        // 최초 접속시 이동 불가 지역인경우 버그 생김 edited by dhkwon 2003.8.20
        if(m_vOldPos.x != -1 && m_vOldPos.y != -1 && m_vOldPos.z != -1)
        {
            m_vPos = m_vPos + 20.0f*(m_vPos-m_vOldPos)*fElapsedTime;
        }
    }
    ////////////////////////여기까지 클라이언트 좌표 설정(이동)//////////////////////////////////////////////////////

    if( m_dwState == _FALLEN || 
        m_dwState == _EXPLODED || 
//        g_pGameMain->IsShowing() ||                // 2006-11-07 by ispark, 창이 떠도 움직이게 수정
        m_bOrderMove || 
        m_bMouseLock || 
        //g_pD3dApp->m_bChatMode ||                // 2008-09-02 by dgwoo 채팅중에도 마우스 컨트롤 가능하게.
        g_pD3dApp->m_bFixUnitDirMode == TRUE)    // 유닛 진행 방향 고정 모드
    {
        m_fLRVelRate = 0.0f;
        m_fSideRate = 0.0f;
    }

    SetMatrix_Move_Air(fElapsedTime);//    SetEffectPos_B_Gear(fElapsedTime);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        UpVector 깨지는 부분 방지
/// \author        // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
/// \date        2013-07-01 ~ 2013-07-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::UpdateSafeUpVector(D3DXVECTOR3    vOldUpTmp)
{
    D3DXVECTOR3            vZeroVel(0,0,0);
    if(vZeroVel != m_vUp)
    {
        // 현재 안전하다.
        return;
    }
    if(vOldUpTmp.x > 0)
    {
        m_vUp.x = 1;
    }
    else
    {
        m_vUp.x = -1;
    }
    
    if(vOldUpTmp.y > 0)
    {
        m_vUp.y = 1;
    }
    else
    {
        m_vUp.y= -1;
    }
    if(vOldUpTmp.z > 0)
    {
        m_vUp.z = 1;
    }
    else
    {
        m_vUp.z = -1;
    }            
    
    D3DXVec3Normalize(&m_vUp,&m_vUp);        
}

void CShuttleChild::CheckUpNVelVector(D3DXVECTOR3    vAirVel, D3DXVECTOR3    vOldUpVec)
{
    D3DXVECTOR3            vZeroVel(0,0,0);
    D3DXVECTOR3            vTmpUp = m_vUp;
    
    if(vZeroVel != vTmpUp)
    {
        // 현재 안전하다.
        return;
    }    
    
    D3DXVec3Normalize(&vTmpUp,&vTmpUp);        
    
    m_vUp = vOldUpVec;
    m_vVel = vAirVel;
    
    // 안전하게 
    m_vVel.x +=0.1f;
    m_vVel.y -=0.1f;
    m_vVel.z +=0.1f;
    
    D3DXVec3Normalize(&m_vVel,&m_vVel);        
    
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
    D3DXVec3Cross(&m_vUp,&m_vVel,&m_vSideVel);
    
    vTmpUp = m_vUp;
    D3DXVec3Normalize(&vTmpUp,&vTmpUp);        
    if(vZeroVel == vTmpUp)
    {
        DBGOUT("vOldUpVec #2 ERROR \n");
    }
    
}

void CShuttleChild::SetMatrix_Move_Air(float fElapsedTime) //void CShuttleChild::SetEffectPos_B_Gear(float fElapsedTime)
{
    D3DXVECTOR3 vVirtualVel, vVirtualUp,vSide;
    D3DXMATRIX    matVirtualTemp;
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
    

    // 2005-07-06 by ispark
    // 롤링 시스템으로 인하여 기울임 제한을 풀기 위해 조건을 비교
    if(m_bKeyDownA || m_bKeyDownD)
    {
//        float fUpRate = m_fOldUpRate + (-2.0f*m_fSideRate*m_fSideCheckRate - m_fOldUpRate)*fElapsedTime;

        float fUpRate = RollingSystem(fElapsedTime);        // 2005-07-05 by ispark

        // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

        // 로봇아머일 경우 롤링 회전을 하지 않는다
         if( IsRobotArmor() )
             fUpRate = 0.0f;
        // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

        D3DXMatrixRotationAxis(&matVirtualTemp, &m_vVel, fUpRate);
        m_fOldUpRate = fUpRate;

    }
    else if((m_fLRVelRate <= 0.0f && m_fSideRate <= m_fLRVelRate*0.5f) 
            || (m_fLRVelRate >= 0.0f && m_fSideRate >= m_fLRVelRate*0.5f))
    {
        float fUpRate = m_fOldUpRate + (-2.0f*m_fSideRate*m_fSideCheckRate - m_fOldUpRate)*fElapsedTime;

        // 2005-05-26 by jschoi - 기체 최대 기울임 제한
        if(m_dwState == _LANDED)
        {
            fUpRate = 0.0f;
        }
        else if(fUpRate > D3DX_PI/2*0.8f)
        {
            fUpRate = D3DX_PI/2*0.8f;
        }
        else if(fUpRate < -D3DX_PI/2*0.8f)
        {
            fUpRate = -D3DX_PI/2*0.8f;
        }

        D3DXMatrixRotationAxis(&matVirtualTemp, &m_vVel, fUpRate);
        m_fOldUpRate = fUpRate;
    }
    else
    {
        float fUpRate = m_fOldUpRate + (-2.0f*(m_fLRVelRate*0.5f)*m_fSideCheckRate - m_fOldUpRate)*fElapsedTime;

        // 2005-05-26 by jschoi - 기체 최대 기울임 제한
        if(m_dwState == _LANDED)
        {
            fUpRate = 0.0f;
        }
        else if(fUpRate > D3DX_PI/2*0.8f)
        {
            fUpRate = D3DX_PI/2*0.8f;
        }
        else if(fUpRate < -D3DX_PI/2*0.8f)
        {
            fUpRate = -D3DX_PI/2*0.8f;
        }
        
        D3DXMatrixRotationAxis(&matVirtualTemp, &m_vVel, fUpRate);
        m_fOldUpRate = fUpRate;
    }
    D3DXVec3TransformCoord(&vVirtualUp, &m_vUp, &matVirtualTemp );
    D3DXMatrixLookAtRH(&m_mMatrix, &m_vPos, &(m_vPos+m_vVel), &vVirtualUp);
    D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix);

    vSide.x = m_mMatrix._11;
    vSide.y = m_mMatrix._12;
    vSide.z = m_mMatrix._13;

    // 4.0f
    D3DXVec3TransformCoord(&m_vLWPos, &m_PrimaryAttack.vSidePos, &m_mMatrix);
    m_vLWPos += 3.0f*m_vVel;
    D3DXVECTOR3 vPrimaryRightPos = D3DXVECTOR3(-m_PrimaryAttack.vSidePos.x, m_PrimaryAttack.vSidePos.y, m_PrimaryAttack.vSidePos.z);
    D3DXVec3TransformCoord(&m_vRWPos, &vPrimaryRightPos, &m_mMatrix);
    m_vRWPos += 3.0f*m_vVel;

    D3DXVec3TransformCoord(&m_vLWSecondaryPos, &m_SecondaryAttack.vSidePos, &m_mMatrix);
    D3DXVECTOR3 vSecondaryRightPos = D3DXVECTOR3(-m_SecondaryAttack.vSidePos.x, m_SecondaryAttack.vSidePos.y, m_SecondaryAttack.vSidePos.z);
    D3DXVec3TransformCoord(&m_vRWSecondaryPos, &vSecondaryRightPos, &m_mMatrix);

    // 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치
    D3DXVec3TransformCoord(&m_vPetLeftPos, &m_PetAttack.vSidePos, &m_mMatrix);
    D3DXVECTOR3 vPetRightPos = D3DXVECTOR3(-m_PetAttack.vSidePos.x,
                                            m_PetAttack.vSidePos.y, 
                                            m_PetAttack.vSidePos.z);
    D3DXVec3TransformCoord(&m_vPetRightPos, &vPetRightPos, &m_mMatrix);
    // end 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치

    m_vWeaponVel.x = m_mMatrix._31;
    m_vWeaponVel.y = m_mMatrix._32;
    m_vWeaponVel.z = m_mMatrix._33;
    D3DXVec3Normalize(&m_vWeaponVel,&-m_vWeaponVel);

    // 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태
    m_vPetLeftVel.x        = m_mMatrix._31; 
    m_vPetLeftVel.y        = m_mMatrix._32;
    m_vPetLeftVel.z        = m_mMatrix._33;

    m_vPetRightVel.x    = m_mMatrix._31; 
    m_vPetRightVel.y    = m_mMatrix._32;
    m_vPetRightVel.z    = m_mMatrix._33;

    D3DXVec3Normalize(&m_vPetLeftVel,&-m_vPetLeftVel);
    D3DXVec3Normalize(&m_vPetRightVel,&-m_vPetRightVel);
    // end 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태

    if(IS_DT(m_myShuttleInfo.UnitKind))
    {

        // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

////    D3DXVec3TransformCoord(&m_vLWBackPos, &m_PrimaryAttack.vSideBackPos, &m_mMatrix);
//         D3DXVec3TransformCoord(&m_vLWBackPos, &m_PrimaryAttack.vSidePos, &m_mMatrix);
//         D3DXVECTOR3 vTempPrimary = m_PrimaryAttack.vSideBackPos - m_PrimaryAttack.vSidePos;
//         float fLen = D3DXVec3Length(&vTempPrimary);
//         D3DXVec3Normalize(&m_vWeaponVel,&m_vWeaponVel);
//         m_vLWPos = m_vLWBackPos + fLen*m_vWeaponVel;
// 
//         D3DXVECTOR3 vPrimaryRightBackPos = D3DXVECTOR3(-m_PrimaryAttack.vSideBackPos.x, m_PrimaryAttack.vSideBackPos.y, m_PrimaryAttack.vSideBackPos.z);
//         D3DXVECTOR3 vRWBackPos;
//         D3DXVec3TransformCoord(&vRWBackPos, &vPrimaryRightBackPos, &m_mMatrix);
//         m_vRWPos = vRWBackPos + fLen*m_vWeaponVel;

        // 발사 위치의 vSideBackPos에서 가져오나
        // y축은 아머 위치에서 가져온다(vSidePos)
        D3DXVECTOR3 vPrimaryLeftBackPos = m_PrimaryAttack.vSideBackPos;
        vPrimaryLeftBackPos.y = m_PrimaryAttack.vSidePos.y;

        // 왼쪽 발사 위치
        D3DXVec3TransformCoord( &m_vLWBackPos, &vPrimaryLeftBackPos, &m_mMatrix );
        D3DXVECTOR3 vTempPrimary = vPrimaryLeftBackPos - m_PrimaryAttack.vSidePos;

        float fLen = D3DXVec3Length( &vTempPrimary );
        D3DXVec3Normalize( &m_vWeaponVel,&m_vWeaponVel );
        m_vLWPos = m_vLWBackPos + fLen * m_vWeaponVel;
        
        // 오른쪽 발사 위치
        D3DXVECTOR3 vPrimaryRightBackPos = vPrimaryLeftBackPos;
        vPrimaryRightBackPos.x = -vPrimaryRightBackPos.x;

        D3DXVECTOR3 vRWBackPos;
        D3DXVec3TransformCoord( &vRWBackPos, &vPrimaryRightBackPos, &m_mMatrix) ;
        m_vRWPos = vRWBackPos + fLen * m_vWeaponVel;

        // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

        D3DXVec3TransformCoord(&m_vLWSecondaryPos, &m_SecondaryAttack.vSidePos, &m_mMatrix);
        D3DXVECTOR3 vSecondaryRightPos = D3DXVECTOR3(-m_SecondaryAttack.vSidePos.x, m_SecondaryAttack.vSidePos.y, m_SecondaryAttack.vSidePos.z);
        D3DXVec3TransformCoord(&m_vRWSecondaryPos, &vSecondaryRightPos, &m_mMatrix);    
    }

}
void CShuttleChild::Move_Ground(float fElapsedTime)        //void CShuttleChild::Move_A_Gear(float fElapsedTime)
{
    if(m_dwState == _LANDED || m_dwState == _NORMAL)
    {
        ChangeUnitState( _NORMAL );
        //m_dwState = _NORMAL;
        // 카메라 복구
        m_bIsCameraPattern = FALSE;
        m_bMouseMoveLock = FALSE;                // 2005-08-05 by ispark        

        // 2005-08-19 by ispark
        // A기어 연료가 0일 때 
        if(0 >= m_myShuttleInfo.CurrentEP)
        {
            m_fShuttleSpeed = 0.0f;
            g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_050819_0101, _Q_AGEAR_FUEL_ALLIN);    //"연료가 소모되어 도시로 워프됩니다."
            ChangeUnitState( _CLOSE );        // 다른처리 못하도록 한다.
        }

        // 2005-08-31 by ispark
        // 롤링중이었다면 롤링 푼다.
        if(m_bRollUsed)
        {
            m_bKeyDownA = m_bKeyDownD = FALSE;
            m_bKeyDownACheck = m_bKeyDownDCheck = FALSE;
            m_bRollUsed = FALSE;
            m_fOldUpRate = 0.0f;            // 기울인 채로 이륙해서 롤링이었다면 기울임을 초기화 시킨다.
        }
    }
    else
    {
        m_bIsAir = TRUE;        
        
        // 2005-08-19 by ispark
        // A기어 연료가 0일 때 
        if(0 >= m_myShuttleInfo.CurrentEP)
        {
            g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );    // 2005-08-19 by ispark
            m_bIsAir = FALSE;
        }

    }
    
    //        if( m_bAttackMode != _SIEGE &&
    //            !COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_EXPLOSION_MASK) && 
    //            !COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_LANDED_MASK))
    //        {
    //            ChangeSingleBodyCondition(BODYCON_LANDED_MASK);
    //        }
    
    D3DXMATRIX  matTemp;
    D3DXVec3Normalize(&m_vVel,&m_vVel);
    D3DXVec3Normalize(&m_vUp,&m_vUp);
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
    //        int nx,ny,nz;
    //        g_pD3dApp->CalcObjectSourceScreenCoords(m_vPos+10000.0f*m_vVel, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
    //            nx, ny, nz );
    
    
    /*        // 평면 방정식을 구한다.
    // a*x + b*y + c*z + d = 0 << 평면 방정식
    float fTemp = -((m_vUp.x*m_vMouseDir.x + m_vUp.y*m_vMouseDir.y + m_vUp.z*m_vMouseDir.z)/(m_vUp.x*m_vUp.x + m_vUp.y*m_vUp.y + m_vUp.z*m_vUp.z));
    D3DXVECTOR3 vNew;
    vNew.x = m_vUp.x*fTemp + m_vMouseDir.x;
    vNew.y = m_vUp.y*fTemp + m_vMouseDir.y;
    vNew.z = m_vUp.z*fTemp + m_vMouseDir.z;
    D3DXVec3Normalize(&vNew,&vNew);
    
      m_fSideRate = ACOS(D3DXVec3Dot(&m_vVel, &vNew));
      if(nx > m_pt.x)
      m_fSideRate *= -1.0f;
      if(abs(nx - m_pt.x) > 3*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800) && !g_pGameMain->IsShowing()
      && !m_bMouseLock && !m_bOrderMove)
      {
      D3DXVECTOR3 vVel,vSide;
      vVel = m_vVel;
      vVel.y = 0.0f;
      D3DXVec3Normalize(&vVel,&vVel);
      float fAngleTemp = ACOS(D3DXVec3Dot(&m_vVel,&vVel));
      if(m_vVel.y > 0.0f)
      fAngleTemp *= -1.0f;
      
        D3DXMatrixRotationAxis(&matTemp, &m_vBasisUp, m_fMouseRate*m_fSideRate*fElapsedTime);
        D3DXVec3TransformCoord(&vVel, &vVel, &matTemp);
        
          D3DXVec3Cross(&vSide,&m_vBasisUp,&vVel);
          D3DXMatrixRotationAxis(&matTemp, &vSide, fAngleTemp);
          D3DXVec3TransformCoord(&m_vVel, &vVel, &matTemp);
          }
          D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
          D3DXVec3Cross(&m_vUp,&m_vVel,&m_vSideVel);
    */    
    int nSideMove = 0;
//    int nFBMove = 0;
    if(    m_bLandingField == FALSE &&
        g_pD3dApp->m_bFixUnitDirMode == FALSE &&        // 유닛 진행 방향 고정 모드
        !g_pD3dApp->m_bChatMode && 
        m_bReflexionKeyBlock == FALSE && 
        m_bAttackMode != _SIEGE &&
        g_pD3dApp->m_dwGameState != _SHOP)
    //    (g_pTutorial->IsTutorialMode() == FALSE || g_pTutorial->GetLesson() >= L2) )
    {
        int nOldBoosterType = m_nBooster;
        m_nBooster = BOOSTER_TYPE_MIN;
        //            if( m_pItemSecondary && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_E) &&
        //                m_fSecondaryWeaponRACheckTime >= ((float)m_pItemSecondary->ReAttacktime)/1000.0f)
        //            {// 2형 무기 발사
        //                m_bSecondaryAttackOrder = TRUE;
        //            }
        // 2008-11-13 by bhsohn 조이스틱 작업
        //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_D) || m_bRightButtonState)
        if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_D) || m_bRightButtonState)
        {// 우로 이동->회전으로 변경
            D3DXMatrixRotationAxis(&matTemp, &m_vUp, 1.0f*fElapsedTime);
            D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
            //                nSideMove++;
        }
        // 2008-11-13 by bhsohn 조이스틱 작업
        //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_A) || m_bLeftButtonState)
        if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_A) || m_bLeftButtonState)
        {// 좌로 이동->회전으로 변경
            D3DXMatrixRotationAxis(&matTemp, &m_vUp, -1.0f*fElapsedTime);
            D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
            //                nSideMove--;
        }
        // 2008-11-13 by bhsohn 조이스틱 작업
        //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W) || m_bUpButtonState)
        if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W) || m_bUpButtonState)
        {// 가속
            //if(m_fShuttleSpeed == 0)
            // 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
            if(0 == m_fShuttleSpeed || 0 == g_pShuttleChild->m_nShuttleBoosterState)
            {
                g_pD3dApp->m_pSound->PlayD3DSound( SOUND_GROUND_MOVING_A_GEAR, m_vPos, FALSE);
                m_bLendSoundPlay = TRUE;
            }
            // end 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
            // 2007-05-15 by bhsohn 위장 스킬후 움직일시 위장 안풀리게 변경
//            if(GetUnitAlphaState())
//                m_pSkill->ReleseSkill(DES_SKILL_CAMOUFLAGE);
            //if((g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo)) < 0.9f)
            //{
//            nFBMove++;
            CheckBoosterState(fElapsedTime,true,nOldBoosterType);
            //}
            //                if(g_pD3dApp->m_dwGameState == _SHOP &&
            //                    g_pGameMain->m_pShop->m_pNPCWindow->m_buildingInfo.BuildingIndex != 0)
            //                {
            //                    SendPacket(T_FC_EVENT_LEAVE_BUILDING);
            //                    //더이상 패킷을 안보내기 위해 미리 나온다.
            //                    g_pD3dApp->ChangeGameState(_GAME);
            //                    m_fEventCheckTime = 2.0f;
            //                    m_bEventReady = TRUE;
            //                    ChangeUnitState( _NORMAL );
            //                    g_pGameMain->m_pMission->m_bShow = FALSE;
            //                }
        }
        else
        {
            CheckBoosterState(fElapsedTime,false,nOldBoosterType);
        }
        // 2008-11-13 by bhsohn 조이스틱 작업
        //if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S) || m_bDownButtonState && !m_bAniKeyBlock)
        if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S) || m_bDownButtonState && !m_bAniKeyBlock)
        {// 저속
//             if(m_fShuttleSpeed == 0)
            // 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
            if(0 == m_fShuttleSpeed || 0 == g_pShuttleChild->m_nShuttleBoosterState)
            {
                g_pD3dApp->m_pSound->PlayD3DSound( SOUND_GROUND_MOVING_A_GEAR, m_vPos, FALSE);
                m_bLendSoundPlay = TRUE;
            }
            // end 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
            // 2007-05-15 by bhsohn 위장 스킬후 움직일시 위장 안풀리게 변경
//            if(GetUnitAlphaState())
//                m_pSkill->ReleseSkill(DES_SKILL_CAMOUFLAGE);
//            nFBMove--;
//            CheckAirBreakState(fElapsedTime,true);
        }
//        else
//        {
//            CheckAirBreakState(fElapsedTime,false);
//        }
    }
    else
    {// 채팅 모드일때 에어 브레이크가 아니라면 부스터의 상태를 일반 키를 뗀 상태로 바꾼다
        if(m_nBooster != BOOSTER_TYPE_AIRBREAK)
        {
            m_nBooster = BOOSTER_TYPE_STOP;
        }
    }
    //        if(!g_pD3dApp->m_bChatMode &&
    //            !m_bReflexionKeyBlock && 
    //            (g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE))&&
    //            m_myShuttleInfo.CurrentEP > 0 &&
    //            m_bAttackMode != _SIEGE &&
    //            m_bReChargeBURN )
    //        {
    //            m_nShuttleStopState = 0;
    //            m_nBooster = BOOSTER_TYPE_MAX;
    //            nFBMove = 1;
    //        }
    //        if(m_nBooster == BOOSTER_TYPE_MAX)
    //        {
    //            // 이륙하게 된다.
    //            SendPacket(T_FC_MOVE_TAKEOFF);
    //            m_bIsAir = TRUE;
    //        }
    // 2006-11-02 by dgwoo A기어는 여기서 처리.
    if(!IS_CITY_MAP_INDEX(m_myShuttleInfo.MapChannelIndex.MapIndex) &&
        m_bLandingField == FALSE &&
        !g_pD3dApp->m_bChatMode &&
        !m_bReflexionKeyBlock && 
        m_bAttackMode != _SIEGE &&
        m_myShuttleInfo.CurrentEP > 0 &&
        // 2008-11-13 by bhsohn 조이스틱 작업
        //( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_C) ||
        ( g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_C) ||
        //            m_bUpButtonState || 
        // 2008-11-13 by bhsohn 조이스틱 작업
        //g_pD3dApp->m_pKeyBoard->GetAsyncKeyState( DIK_SPACE )))
        g_pD3dApp->GetAsyncKeyState_DIK_DIJ( DIK_SPACE )))
    {
        m_bLandedMove = FALSE;
        g_pInterface->m_pGameMain->InitShowWindow();

        // 2005-04-07 by jschoi
        if(g_pTutorial->IsTutorialMode() == FALSE)
        {
            if(m_fNumpad0keyTime <= 0.0f)
            {
                // 착륙전 시점이 1인칭이였으면 다시 1인칭으로 변환 
                if(m_bLandingOldCamtypeFPS)
                {
                    g_pD3dApp->m_pCamera->ChangeCamType();
                    m_bLandingOldCamtypeFPS = FALSE;
                }
                
                SendPacket(T_FC_MOVE_TAKEOFF);
                DBGOUT("T_FC_MOVE_TAKEOFF\n");
                g_pCamera->Init(m_vPos - 10.0f*m_vVel,m_vVel, 3.14f/13.0f, 60.0f);
                m_fNumpad0keyTime = 2.0f;
                ChangeUnitState(_TAKINGOFF);                
                // 2007-05-15 by bhsohn A기어 보완
                //m_fRasingCheckTime = 1.0f;
                m_fRasingCheckTime = GetRasingCheckTime();
            }
        }
    }
    // 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
    if(0 == g_pShuttleChild->m_nShuttleBoosterState && TRUE == m_bLendSoundPlay)
    {
        g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );
        m_bLendSoundPlay = FALSE;
    }
    // end 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
// 2013-02-04 by mspark, A기어 보급상점에 랜딩 시 이동 버튼을 누를경우, 보급상점 창이 닫히도록 작업
#ifdef C_AGEAR_REFINERYSHOP_CLOSE_MSPARK
    if(g_pD3dApp->m_dwGameState == _SHOP)
    {
        if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W) ||
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S) ||
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_A) ||
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_D) ||
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_UP) ||
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_DOWN) ||
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_LEFT) ||
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_RIGHT))
        {
            g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
        }
    }    
#endif
// end 2013-02-04 by mspark, A기어 보급상점에 랜딩 시 이동 버튼을 누를경우, 보급상점 창이 닫히도록 작업
    if(g_pGameMain->m_pMission->m_bShow)
    {
        m_nBooster = BOOSTER_TYPE_STOP;
        nSideMove = 0;
//        nFBMove = 0;
    }
    //        CheckMoveRate(fElapsedTime,nSideMove,nFBMove);
//    CheckMoveRate(fElapsedTime,0,nFBMove);
    CheckMoveRate(fElapsedTime,0);
    // 타 캐릭 체크
    //        if(m_bSecondaryFireType == 1)
    //            CheckEnemyType1(1);
    //        else if(m_bSecondaryFireType == 2)
    //            CheckEnemyType2(1);
    m_fSideRate = 0.0f;
    
    D3DXVECTOR3  vUpTemp;
    D3DXVECTOR3 vFront, vBack, vLeft, vRight;
    vFront = m_vPos + 6.0f*m_vVel;
    vBack = m_vPos - 3.0f*m_vVel;
    vLeft = m_vPos - 6.0f*m_vSideVel;
    vRight = m_vPos + 6.0f*m_vSideVel;
    m_bCheckMove = 4;
    m_bCollMap = FALSE;
    BYTE bTemp;
    BYTE bCollCont = 0;
    // Front
    bTemp = m_bCheckMove;
    CheckMap(vFront);
    if(bTemp < m_bCheckMove)
        m_bCheckMove = bTemp;
    if(m_bCollMap)
    {
        if(g_pD3dApp->m_pEffectList)
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vFront);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
        bCollCont++;
    }
    // Back
    m_bCollMap = FALSE;
    bTemp = m_bCheckMove;
    CheckMap(vBack);
    if(bTemp < m_bCheckMove)
        m_bCheckMove = bTemp;
    if(m_bCollMap)
    {
        if(g_pD3dApp->m_pEffectList)
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vBack);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
        bCollCont++;
    }
    // Left
    m_bCollMap = FALSE;
    bTemp = m_bCheckMove;
    CheckMap(vLeft);
    if(bTemp < m_bCheckMove)
        m_bCheckMove = bTemp;
    if(m_bCollMap)
    {
        if(g_pD3dApp->m_pEffectList)
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vLeft);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
        bCollCont++;
    }
    // Right    
    m_bCollMap = FALSE;
    bTemp = m_bCheckMove;
    CheckMap(vRight);
    if(bTemp < m_bCheckMove)
        m_bCheckMove = bTemp;
    if(m_bCollMap)
    {
        if(g_pD3dApp->m_pEffectList)
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vRight);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
        bCollCont++;
    }
    
    float fRate = 1.0f;
    // A기어 지상 이동시 지형에 대한 기어의 높낮이 세팅
    float fHeight = 0.0f;
    
    float fObjHeight = -DEFAULT_COLLISION_DISTANCE;
    
    // 맵 높이 산출
    // 2004-11-04 by jschoi
    if(m_bOnObject)
    {
        float fTempDistance;
        D3DXMATRIX matTemp;
        D3DXMatrixLookAtLH(&matTemp,&m_vPos,&(m_vPos - D3DXVECTOR3(0,1,0)),&D3DXVECTOR3(0,0,1));        // 아래
        fTempDistance = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vPos, FALSE).fDist;
        if(fTempDistance != DEFAULT_COLLISION_DISTANCE)
        {
            fHeight = CheckMap(m_vPos);
            if( m_vPos.y - fTempDistance > fHeight)
            {
                fHeight = m_vPos.y - fTempDistance;
            }
        }
        else
        {
            fHeight = CheckMap(m_vPos);
        }
    }
    else
    {
        fHeight = CheckMap(m_vPos);
    }
    
    if((m_vPos.y - fHeight) < 5.0f)
    {
        switch(m_bCheckMove)
        {
        case 0:
            {// 못가는 지역
                D3DXVECTOR3 vTempvel;
                vTempvel = m_vNormal;
                vTempvel.y = 0.0f;
                D3DXVec3Normalize(&vTempvel,&vTempvel);
                // 방향 틀어짐 
                D3DXVec3Normalize(&m_vVel,&(20.0f*m_vVel+vTempvel-100.0f*m_vBasisUp*fElapsedTime));
                D3DXVec3Normalize(&vTempvel,&(m_vVel+1.5f*vTempvel));
                if(m_vPos.y < m_vNextPos.y || bCollCont)
                {
                    if(m_fShuttleSpeed > 0.0f)    // 앞방향으로 전진하며 부딪힐때
                        m_vNextPos = m_vPos + D3DXVec3Length(&(m_vPos - m_vNextPos))*vTempvel;
                    else    // 뒤로 가며 부딪힐때
                    {
                        m_fShuttleSpeed *= -0.8f;
                        m_vNextPos = m_vPos + 20.0f*D3DXVec3Length(&(m_vPos - m_vNextPos))*vTempvel;
                    }
                    if(m_fLRVelRate > 0.0f)
                        m_fLRVelRate *= -0.3f;
                    else
                    {
                        m_fLRVelRate *= -0.3f;
                    }
                }
                break;
            }
        case 1:
            {// 2차감속
            }
        case 2:
            {// 1차감속
            }
        case 3:
            {// 갈수있는 지역
                fRate = 1.0f;
                break;
            }
        }
    }
    // 포지션을 적용
    if(m_myShuttleInfo.CurrentEP)
        m_vPos += fRate*(m_vNextPos - m_vPos)*fElapsedTime;
    else
        m_vPos += 0.5f*fRate*(m_vNextPos - m_vPos)*fElapsedTime;
    // 비행높이 산출
    
    // 2004-11-04 by jschoi
    float fHeightDistance = m_vPos.y - fHeight - 10.0f;
    
    if(fHeightDistance > 0.2f)
    {
        if(fHeightDistance > 10.0f)
        {
            fHeightDistance = 10.0f;        // 최대값
        }
        m_vPos.y -= 5.0f*fHeightDistance*fElapsedTime;        // A기어 지상 이동시 떨어지는 높이
        if( fHeight + m_fAmorHeight > m_vPos.y )
        {
            m_vPos.y = fHeight + m_fAmorHeight;            // 기어가 땅(오브젝트)속으로는 들어갈 수 없다.
        }
    }
    else if(fHeightDistance < -0.2f)
    {
        if(fHeightDistance < -10.0f)
        {
            fHeightDistance = -10.0f;        // 최대값
        }
        m_vPos.y -= 10.0f*fHeightDistance*fElapsedTime;        // A기어 지상 이동시 기어 올라가는 높이
    }

    // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    // 땅에 착륙시 아머를 갈아낄 시 기어가 파 묻히는것을 방지하기 위해 한번 더 체크
    if( fHeight + m_fAmorHeight > m_vPos.y )
        m_vPos.y = fHeight + m_fAmorHeight;            // 기어가 땅(오브젝트)속으로는 들어갈 수 없다.
    // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    
    //        if((m_vPos.y - (fHeight+10.0f)) > 0.2f)
    //        {
    //            m_vPos.y -= 10.0f*fElapsedTime;
    //        }
    //        else if((m_vPos.y - (fHeight+10.0f)) < -0.2f)
    //        {
    //            m_vPos.y += 10.0f*((fHeight+10.0f) - m_vPos.y)*fElapsedTime;
    //        }
    
    
    FLOAT fDist;
    int i,j;
    D3DXVECTOR3 v,orig,dir;
    D3DXVECTOR3 vSide,vVel;
    D3DXVECTOR3 v1,v2,v3;
    D3DXVECTOR3 vNextVel, vNextUp,vPos;
    float fDistTemp;
    v = D3DXVECTOR3(0,0,0);
    vSide = D3DXVECTOR3(0,0,1);
    //        orig = m_vPos+10.0f*m_vVel;
    float fTempHeight = 100.0f;
    //        i = (int)(orig.x/TILE_SIZE);
    //        j = (int)(orig.z/TILE_SIZE);
    dir = D3DXVECTOR3(0,-1,0);
    
    orig = m_vPos + 5.0f*m_vUp + 6.0f*m_vVel;        // Front
    D3DXMatrixLookAtLH(&matTemp,&orig,&(orig - m_vBasisUp),&vSide);
    fDistTemp = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
    //        if(fDistTemp > 14.0f)
    //            fDistTemp = 14.0f;
    v1 = orig - m_vBasisUp*fDistTemp;
    
    i = (int)(orig.x/TILE_SIZE);
    j = (int)(orig.z/TILE_SIZE);
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize
        &&IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))                    // 2006-06-07 by ispark
    {
        if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            v = orig + dir*fDist;
        }
        else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            v = orig + dir*fDist;
        }
        if(CheckIsWater(v))
        {
            if(v.y < g_pGround->m_projectInfo.fWaterHeight)
                v.y = g_pGround->m_projectInfo.fWaterHeight;
        }
    }
    if(v1.y <= v.y)
        v1 = v;
    
    orig = m_vPos + 5.0f*m_vUp + 6.0f*m_vSideVel;    // Left
    D3DXMatrixLookAtLH(&matTemp,&orig,&(orig - m_vBasisUp),&vSide);
    fDistTemp = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
    //        if(fDistTemp > 14.0f)
    //            fDistTemp = 14.0f;
    v2 = orig - m_vBasisUp*fDistTemp;
    v = D3DXVECTOR3(0,0,0);
    i = (int)(orig.x/TILE_SIZE);
    j = (int)(orig.z/TILE_SIZE);
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize
        && IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))                    // 2006-06-07 by ispark
    {
        if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            v = orig + dir*fDist;
        }
        else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            v = orig + dir*fDist;
        }
        if(CheckIsWater(v))
        {
            if(v.y < g_pGround->m_projectInfo.fWaterHeight)
                v.y = g_pGround->m_projectInfo.fWaterHeight;
        }
    }
    if(v2.y <= v.y)
        v2 = v;
    
    orig = m_vPos + 5.0f*m_vUp - 6.0f*m_vSideVel;    // Right
    D3DXMatrixLookAtLH(&matTemp,&orig,&(orig - m_vBasisUp),&vSide);
    fDistTemp = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,orig, FALSE).fDist;
    //        if(fDistTemp > 14.0f)
    //            fDistTemp = 14.0f;
    v3 = orig - m_vBasisUp*fDistTemp;
    v = D3DXVECTOR3(0,0,0);
    i = (int)(orig.x/TILE_SIZE);
    j = (int)(orig.z/TILE_SIZE);
    if(i >= 0 && i < g_pGround->m_projectInfo.sXSize && j >= 0 && j < g_pGround->m_projectInfo.sYSize
        &&IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))                    // 2006-06-07 by ispark
    {
        if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j].pos,
            g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+(j+1)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            v = orig + dir*fDist;
        }
        else if(g_pGround->IntersectTriangle(orig,dir,g_pGround->m_pTileVertexArray[i*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+(j)].pos,
            g_pGround->m_pTileVertexArray[(i+1)*(g_pGround->m_projectInfo.sYSize+1)+j+1].pos,
            &fDist))//, &fBary1, &fBary2 ))
        {
            v = orig + dir*fDist;
        }
        if(CheckIsWater(v))
        {
            if(v.y < g_pGround->m_projectInfo.fWaterHeight)
                v.y = g_pGround->m_projectInfo.fWaterHeight;
        }
    }
    if(v3.y <= v.y)
        v3 = v;
    
    if(abs(v1.y - v2.y) > 50.0f || abs(v2.y - v3.y) > 50.0f)
    {// 
        vNextUp = m_vUp;
    }
    else
    {
        D3DXVec3Cross(&vNextUp,&(v3-v2),&(v1-v2));
        D3DXVec3Normalize(&vNextUp,&vNextUp);
    }
    // 2008-01-09 by dgwoo UP vector가 0,0,0이면 수평유지!
    if(vNextUp == D3DXVECTOR3(0,0,0))
    {
        vNextUp = D3DXVECTOR3(0,1,0);
    }
    
    v1.y += 5.0f;
    D3DXVec3Normalize(&vNextVel,&(v1-m_vPos));
    //////////////////////////////////////////////////////    
    if(m_bOnObject)
    {
        D3DXMatrixLookAtLH(&matTemp,&m_vPos,&(m_vPos - m_vBasisUp),&vSide);
        fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vPos, FALSE).fDist;
    }
    
    if(fTempHeight <= m_fAmorHeight && fHeight + m_fAmorHeight < m_vPos.y + m_fAmorHeight - fTempHeight)
    {
        // 오브젝트위에 있을때
        fHeight = m_vPos.y + m_fAmorHeight - fTempHeight;
        m_vPos.y += m_fAmorHeight - fTempHeight;
    }
    else if((m_vPos.y - fHeight) > m_fAmorHeight+10.0f)//10.0f : A기어 유닛 차체 높이
    {
        // 공중에 있을때
        vNextUp = D3DXVECTOR3(0,1,0);
        vNextVel.y = 0.0f;
        D3DXVec3Normalize(&vNextVel,&vNextVel);
    }
    
    m_vUp += 1.0f*(vNextUp-m_vUp)*fElapsedTime;
    //        if(m_vUp.y < 0.0f)
    //            m_vUp.y *= -1.0f;
    //        m_vVel += 1.0f*(vNextVel-m_vVel)*fElapsedTime;
    
    /// 2004.06.11 jschoi - 충돌체크 추가
    CheckCollForObject(fElapsedTime);        // 착륙 상태의 충돌 체크 
    
    // 못가는 지역체크하여 빽시킨다.
    CheckMapRegion();
    
    
    
    int nXCheck, nYCheck;
    nXCheck = (int)(m_vPos.x / TILE_SIZE);
    nYCheck = (int)(m_vPos.z / TILE_SIZE);
    m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize + nYCheck].bMove;
    if(m_bMoveGroundType == 2)
    {
        m_fShuttleSpeed *= -0.5f;
        m_fLRVelRate *= 0.8f;
        
        // 최초 접속시 이동 불가 지역인경우 버그 생김 edited by dhkwon 2003.8.20
        if(m_vOldPos.x != -1 && m_vOldPos.y != -1 && m_vOldPos.z != -1)
        {
            m_vPos = m_vPos + 20.0f*(m_vPos-m_vOldPos)*fElapsedTime;
        }
    }
    ////////////////////////여기까지 클라이언트 좌표 설정(이동)//////////////////////////////////////////////////////
    
    // 2005-04-01 by jschoi
    SetMatrix_Move_Ground(fElapsedTime);    //    SetEffectPos_A_Gear(fElapsedTime);
        
    // 이벤트 지역 체크 - Shop
    int nCheckX = ((int)m_vPos.x)/TILE_SIZE;
    int nCheckZ = ((int)m_vPos.z)/TILE_SIZE;
    if((g_pGround->m_pTileInfo[m_ptOldPoint.x*g_pGround->m_projectInfo.sYSize + m_ptOldPoint.y].bEventType 
        != g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType)
        && (g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType == EVENT_TYPE_ENTER_BUILDING
        || g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEventType == EVENT_TYPE_LANDING))
    {
        // 상점으로 들어감을 알린다
//        if(m_fDTEventCheckTime <= 0.0f && m_vPos.y <= fHeight + m_fAmorHeight)
        {
            // 강제로 Move를 전송한다.
            //                SendPacket(T_FC_MOVE);
            //                g_pD3dApp->SendMovePacket(FALSE);
            
            g_pD3dApp->m_vClientOldPos = m_vPos;
            //                g_pD3dApp->m_dwLastTickTime = g_pD3dApp->m_dwCurTickTime;
            
            m_fDTEventCheckTime = 0.01f;
            
            SendPacket(T_FC_MOVE_LANDING_DONE);
            DBGOUT("T_FC_MOVE_LANDING_DONE\n");

            // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리                                
            if(IS_DT(m_myShuttleInfo.UnitKind))
            {
                // A기어 라면 움직임 초기화
                InitUnitStop();
            }                
            // end 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리

            //                m_bEventReady = TRUE;            
        }
    }
    else if(g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEnableLand)
    {
        if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_LANDED_MASK) 
            && m_fDTEventCheckTime <= 0.0f && m_vPos.y <= fHeight + m_fAmorHeight)
        {
            // 강제로 Move를 전송한다.
            //                SendPacket(T_FC_MOVE);
            //                g_pD3dApp->SendMovePacket(FALSE);
            g_pD3dApp->m_vClientOldPos = m_vPos;
            //                g_pD3dApp->m_dwLastTickTime = g_pD3dApp->m_dwCurTickTime;
            
            m_fDTEventCheckTime = 0.01f;
            SendPacket(T_FC_MOVE_LANDING_DONE);
            DBGOUT("T_FC_MOVE_LANDING_DONE\n");
            //                m_bEventReady = TRUE;
            
            ChangeSingleBodyCondition(BODYCON_LANDED_MASK);
            
            // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리                                
            if(IS_DT(m_myShuttleInfo.UnitKind))
            {
                // A기어 라면 움직임 초기화
                InitUnitStop();
            }
            // end 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리
        }
    }
    else
    {
        //            //  상점 부분 : 구현해야 한다.
        //            if( g_pD3dApp->m_dwGameState == _SHOP &&
        //                g_pGameMain->m_pShop->m_pNPCWindow->m_buildingInfo.BuildingIndex != 0)
        //            {
        //                SendPacket(T_FC_EVENT_LEAVE_BUILDING);
        //                //더이상 패킷을 안보내기 위해 미리 나온다.
        //                g_pD3dApp->ChangeGameState(_GAME);
        //                m_fEventCheckTime = 2.0f;
        //                m_bEventReady = TRUE;
        //                ChangeUnitState( _NORMAL );
        //                //m_dwState = _NORMAL;
        //                g_pGameMain->m_pMission->m_bShow = FALSE;
        //
        //                //SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_TAKEOFF_MASK);
        //                if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_TAKEOFF_MASK))
        //                {
        //                    ChangeSingleBodyCondition(BODYCON_TAKEOFF_MASK);
        //                }
        //
        //            }
        /*            else if(COMPARE_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_LANDED_MASK) && )
        {
        SendPacket(T_FC_MOVE_TAKEOFF);
        
          SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_TAKEOFF_MASK);
          if(m_pCharacterInfo)
          m_pCharacterInfo->ChangeBodyCondition(m_myShuttleInfo.BodyCondition);
          if(m_pEngine && m_pEngine->m_pCharacterInfo)
          {
          m_pEngine->ChangeBodyCondition(BODYCON_BOOSTER1_MASK);
          m_pEngine->m_bodyCondition = BODYCON_BOOSTER1_MASK;
          m_pEngine->m_pCharacterInfo->ChangeBodyCondition(m_pEngine->m_bodyCondition);
          }
          }
        */            if(m_fDTEventCheckTime >= 0.0f)
        m_fDTEventCheckTime -= fElapsedTime;
    }
    
    // 2007-04-17 by bhsohn 가까운 거리에서의 시즈모드시 데이지 안들어가는 현상 처리
    m_bAGearFollowEnemy = FALSE;

    // 시즈모드
    if(m_bAttackMode == _SIEGE)
    {
        SiegeTarget();
        // 2010-06-15 by shcho&hslee 펫시스템 - 시즈 모드시 펫 숨기기 
        HidePet(TRUE);
        // end 2010-06-15 by shcho&hslee 펫시스템 - 시즈 모드시 펫 숨기기 
    }
    // 일반모드
    else
    {
        // 2006-01-03 by ispark, 일반 A기어 무브 그라운드 상태 (타겟이 있던, 없던)
        m_vWeaponVel = m_vMouseDir;
        // 2010-06-15 by shcho&hslee 펫시스템 - 시즈 모드시 펫 숨기기 
        m_vPetLeftVel    = m_vMouseDir;
        m_vPetRightVel    = m_vMouseDir;
        HidePet(FALSE);
        // end 2010-06-15 by shcho&hslee 펫시스템 - 시즈 모드시 펫 숨기기 
    }            

//    m_pAutoTargetOldMousePos = pt;
    
    // 유닛 락상태일때 포이동 없음
    if(m_bMouseLock == TRUE) m_vWeaponVel = m_vVel;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::SiegeTarget(float fElapsedTime)
/// \brief        시즈 모드시 타겟을 잡은 상태로 계속 때릴 수 있도록.
/// \author        dgwoo
/// \date        2006-11-16 ~ 2006-11-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SiegeTarget()
{
    POINT pt;                            // 현재 마우스 포인터
    POINT pt2;                            // 몬스터의 2D위치.
    BOOL  pointmoveflag = FALSE;
    GetCursorPos(&pt);
    // Order타겟이 있을 때
    if(m_pOrderTarget)
    {
        // 타겟을 따라가기
        //--------------------------------------------------------------------------//
        // 2006-11-03 by dgwoo
        pointmoveflag = TRUE;

        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
//         pt2.x = m_pOrderTarget->m_nObjScreenX;
//         pt2.y = m_pOrderTarget->m_nObjScreenY;
        D3DXVECTOR3 temp;
        // 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
        //if( ((CMonsterData*)m_pOrderTarget)->m_vecvmultiData.size() )
        if( m_pOrderTarget &&
            m_pOrderTarget->m_dwPartType == _MONSTER &&
            ((CMonsterData*)m_pOrderTarget)->m_vecvmultiData.size() )
        // end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
        {
            // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            temp = ((CMonsterData*)m_pOrderTarget)->GetMultiPos( ((CMonsterData*)m_pOrderTarget)->m_nMultiIndex );
            int tempX, tempY, tempW;

            ((CMonsterData*)m_pOrderTarget)->GetScreenPos( temp, tempX, tempY, tempW ); 
            pt2.x = tempX;
            pt2.y = tempY;
        }
        else
        {
            temp = m_pOrderTarget->m_vPos;
            pt2.x = m_pOrderTarget->m_nObjScreenX;
            pt2.y = m_pOrderTarget->m_nObjScreenY;
        }
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템


        ClientToScreen(g_pD3dApp->GetHwnd(), &pt2);
        ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
        CheckMouseReverse(&pt);
        ClientToScreen(g_pD3dApp->GetHwnd(),&pt);
        
        if(pt2.x + SIGEMODE_MOUSE_FITPOINT < pt.x ||
            pt2.x - SIGEMODE_MOUSE_FITPOINT > pt.x)
        {
            pointmoveflag = FALSE;
            // 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
            //// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            //((CMonsterData*)m_pOrderTarget)->m_bSiegeMode = FALSE;
            //// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            if( m_pOrderTarget && m_pOrderTarget->m_dwPartType == _MONSTER )
            {
            ((CMonsterData*)m_pOrderTarget)->m_bSiegeMode = FALSE;
            }            
            // end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
        }
        if(pt2.y + SIGEMODE_MOUSE_FITPOINT < pt.y ||
            pt2.y - SIGEMODE_MOUSE_FITPOINT > pt.y)
        {
            pointmoveflag = FALSE;
            // 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
            //// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            //((CMonsterData*)m_pOrderTarget)->m_bSiegeMode = FALSE;
            //// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            if( m_pOrderTarget && m_pOrderTarget->m_dwPartType == _MONSTER )
            {
            ((CMonsterData*)m_pOrderTarget)->m_bSiegeMode = FALSE;
            }            
            // end 2011. 09. 28 by jskim 넌 타겟 시스템 버그 수정( 케스팅 문제 )
        }
        pt2 = pt;

        ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
        //CheckMouseReverse(&pt);
        //--------------------------------------------------------------------------//
        // 클라이언트와 스크린 좌표의 차를 구한다.
        int nX = pt2.x - pt.x;
        int nY = pt2.y - pt.y;
        
        // 그 차만큼 클라이언트의 중심좌표를 nX,nY에 넣어준다.
        nX += g_pD3dApp->GetBackBufferDesc().Width/2 + g_pD3dApp->GetBackBufferDesc().Width/800;
        nY += g_pD3dApp->GetBackBufferDesc().Height/2 + g_pD3dApp->GetBackBufferDesc().Height/600;    
        if(pointmoveflag) 
        {
            // 2008-11-13 by bhsohn 조이스틱 작업
            m_bSetCursorPos = TRUE;

            SetCursorPos(nX,nY);
            // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            //D3DXVec3Normalize(&m_vWeaponVel,&(m_pOrderTarget->m_vPos - m_vPos));
             D3DXVec3Normalize(&m_vWeaponVel,&(temp - m_vPos));

            // 포의 일정각도 아래로 내려가지 않는다.
            float dot2 = D3DXVec3Dot(&m_vUp, &m_vWeaponVel);
            if(dot2 < -0.3f && (m_pt.y > (g_pD3dApp->m_d3dsdBackBuffer.Height / 2)))
            {
                double dRadian = 0.0f;
                D3DXVECTOR3 vtempWeaponVel;
                D3DXVECTOR3 vVelF = m_vWeaponVel;
                D3DXVECTOR3 vVelS;
                D3DXMATRIX matRot;
                D3DXVec3Normalize(&vVelF, &vVelF);
                D3DXVec3Cross(&vVelS, &m_vUp, &vVelF);
                vtempWeaponVel = m_vUp;
                dRadian = ACOS(-0.37);
                D3DXMatrixRotationAxis(&matRot , &vVelS , dRadian);
                D3DXVec3Normalize(&vtempWeaponVel, &vtempWeaponVel);
                D3DXVec3TransformCoord(&vtempWeaponVel , &vtempWeaponVel , &matRot); // view * rot로 새로운 dst vector를 구한다.
                m_vWeaponVel = vtempWeaponVel;
                D3DXVec3Normalize(&m_vWeaponVel, &m_vWeaponVel);
            }        
        }        
    }

    // 타겟이 없거나 타겟에 멀리 떨어져 있을 때
    // 2008-12-16 by bhsohn A기어 공중시지 기능 업그레이드                
//    if(pointmoveflag == FALSE)
//    {    
//        D3DXVECTOR3 Av, Uv;                    // 포이동 속도관련 
//        D3DXMATRIX matV;
//        
//        D3DXVECTOR3 v, MouseD;                // 위치관련 마우스 관련 
//        D3DXMATRIX matProj,matView,matTemp;
//        
//        float m_fAnimationShotMove = A_GEAR_SIGEMODE_MOVETIMING;
//        D3DXVec3Lerp(&Av, &m_vWeaponVel, &m_vMouseDir, m_fAnimationShotMove);                
//        
//        // 2007-04-13 by bhsohn A기어 포대문제
//        D3DXVec3Normalize(&Av, &Av);
//        // 각(라디안)을 구한다.         
//        float fAngleRadian = GetRadianVectorBetweenVector(m_vUp, m_vWeaponVel);
//        if(fAngleRadian < AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN)        
//        {        
//            // 포의 일정각도 위로 올라 가지 않는다.
//            m_vWeaponVel = GetRadianVector(m_vWeaponVel, m_vUp, AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN+AGEAR_SIEGEMODE_MIN_CAP);            
//                }
//        else if(fAngleRadian > AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN)    
//        {
//                // 포의 일정각도 아래로 내려가지 않는다.
//            m_vWeaponVel = GetRadianVector(m_vWeaponVel, m_vUp, AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN-AGEAR_SIEGEMODE_MIN_CAP);
//            }
//            else
//            {
//            float fAngleAv = GetRadianVectorBetweenVector(Av, m_vUp);
//            if(fAngleAv < AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN)        
//            {
//                // 포의 일정각도 위로 올라 가지 않는다.
//                Av = GetRadianVector(Av, m_vUp, AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN);                
//            }
//            else if(fAngleAv > AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN)    
//            {
//                // 포의 일정각도 아래로 내려가지 않는다.
//                Av = GetRadianVector(Av, m_vUp, AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN);
//            }            
//                m_vWeaponVel = Av;
//            }                                    
//    }    
    if(pointmoveflag == FALSE)
    {    
        if(m_bIsAir == FALSE)
        {            
            GetSiegeUpVelVector(&m_vWeaponVel, &m_vUp);
        }
        else
        {
            GetAirSiegeUpVelVector(&m_vWeaponVel, &m_vUp);
        }        
    }    
    // end 2008-12-16 by bhsohn A기어 공중시지 기능 업그레이드

    // 2007-04-17 by bhsohn 가까운 거리에서의 시즈모드시 데이지 안들어가는 현상 처리
    m_bAGearFollowEnemy = pointmoveflag;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        A기어 공중시지 기능 업그레이드
/// \author        // 2008-12-16 by bhsohn A기어 공중시지 기능 업그레이드
/// \date        2008-12-16 ~ 2008-12-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::GetSiegeUpVelVector(D3DXVECTOR3    *o_vWeaponVel,    D3DXVECTOR3* o_vWeaponUp)
{
    D3DXVECTOR3    vTmpWeaponVel = (*o_vWeaponVel);
    D3DXVECTOR3    vTmpWeaponUp = (*o_vWeaponUp);
    
    D3DXVECTOR3 Av, Uv;                    // 포이동 속도관련 
    D3DXMATRIX matV;
    
    D3DXVECTOR3 v, MouseD;                // 위치관련 마우스 관련 
    D3DXMATRIX matProj,matView,matTemp;
    
    float fAnimationShotMove = A_GEAR_SIGEMODE_MOVETIMING;
    D3DXVec3Lerp(&Av, &vTmpWeaponVel, &m_vMouseDir, fAnimationShotMove);                
    
    // 2007-04-13 by bhsohn A기어 포대문제
    D3DXVec3Normalize(&Av, &Av);
    // 각(라디안)을 구한다.         
    float fAngleRadian = GetRadianVectorBetweenVector(vTmpWeaponUp, vTmpWeaponVel);
    if(fAngleRadian < AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN)        
    {        
        // 포의 일정각도 위로 올라 가지 않는다.
        vTmpWeaponVel = GetRadianVector(vTmpWeaponVel, vTmpWeaponUp, AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN+AGEAR_SIEGEMODE_MIN_CAP);            
    }
    else if(fAngleRadian > AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN)    
    {
        // 포의 일정각도 아래로 내려가지 않는다.
        vTmpWeaponVel = GetRadianVector(vTmpWeaponVel, vTmpWeaponUp, AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN-AGEAR_SIEGEMODE_MIN_CAP);
    }
    else
    {
        float fAngleAv = GetRadianVectorBetweenVector(Av, vTmpWeaponUp);
        if(fAngleAv < AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN)        
        {
            // 포의 일정각도 위로 올라 가지 않는다.
            Av = GetRadianVector(Av, vTmpWeaponUp, AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN);                
        }
        else if(fAngleAv > AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN)    
        {
            // 포의 일정각도 아래로 내려가지 않는다.
            Av = GetRadianVector(Av, vTmpWeaponUp, AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN);
        }            
        vTmpWeaponVel = Av;
    }            

    (*o_vWeaponVel) = vTmpWeaponVel;
    (*o_vWeaponUp) = vTmpWeaponUp;
    
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        A기어 공중시지 기능 업그레이드
/// \author        // 2008-12-16 by bhsohn A기어 공중시지 기능 업그레이드
/// \date        2008-12-16 ~ 2008-12-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::GetAirSiegeUpVelVector(D3DXVECTOR3    *o_vWeaponVel,    D3DXVECTOR3* o_vWeaponUp)
{
    D3DXVECTOR3    vTmpWeaponVel = (*o_vWeaponVel);
    D3DXVECTOR3    vTmpWeaponUp = (*o_vWeaponUp);

    BOOL bRefreshUpVector = FALSE;
    BOOL bRefreshVelVector = FALSE;
    float fAnimationShotMove = A_GEAR_SIGEMODE_MOVETIMING;
    
    D3DXVECTOR3 Av, Uv;                    // 포이동 속도관련 
    D3DXMATRIX matV;
    
    D3DXVECTOR3 v, MouseD;                // 위치관련 마우스 관련 
    D3DXMATRIX matProj,matView,matTemp;
    
    
    D3DXVec3Lerp(&Av, &vTmpWeaponVel, &m_vMouseDir, fAnimationShotMove);                
    
    // 2007-04-13 by bhsohn A기어 포대문제
    D3DXVec3Normalize(&Av, &Av);
    // 각(라디안)을 구한다.         
    float fAngleRadian = GetRadianVectorBetweenVector(vTmpWeaponUp, vTmpWeaponVel);
    if(fAngleRadian < AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN)        
    {    
        // 포의 일정각도 위로 올라 가지 않는다.
        //vTmpWeaponVel = GetRadianVector(vTmpWeaponVel, vTmpWeaponUp, AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN+AGEAR_SIEGEMODE_MIN_CAP);            
        bRefreshUpVector= TRUE;
        bRefreshVelVector = TRUE;
    }
    else if(fAngleRadian > AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN)    
    {
        // 포의 일정각도 아래로 내려가지 않는다.
        //vTmpWeaponVel = GetRadianVector(vTmpWeaponVel, vTmpWeaponUp, AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN-AGEAR_SIEGEMODE_MIN_CAP);
        bRefreshUpVector= TRUE;
    }
    else
    {
        float fAngleAv = GetRadianVectorBetweenVector(Av, vTmpWeaponUp);
        if(fAngleAv < AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN)        
        {
            // 포의 일정각도 위로 올라 가지 않는다.
            //Av = GetRadianVector(Av, vTmpWeaponUp, AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN);                
            bRefreshUpVector= TRUE;
            bRefreshVelVector = TRUE;
        }
        else if(fAngleAv > AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN)    
        {
            // 포의 일정각도 아래로 내려가지 않는다.
            //Av = GetRadianVector(Av, vTmpWeaponUp, AGEAR_SIEGEMODE_MIN_ANGLE_RADIAN);
            bRefreshUpVector= TRUE;
        }            
        vTmpWeaponVel = Av;
    }    
    if(bRefreshUpVector)
    {
        // 강제로 기체 업벡터를 변경시킨다.
        D3DXVECTOR3 vUpTmp;
        D3DXVec3Lerp(&vUpTmp, &vTmpWeaponUp, &m_vMouseDir, fAnimationShotMove);
        D3DXVec3Normalize(&vUpTmp, &vUpTmp);
        vTmpWeaponUp = vUpTmp;
        
        if(bRefreshVelVector)
        {
            vTmpWeaponVel = GetRadianVector(vTmpWeaponVel, vTmpWeaponUp, 
                AGEAR_SIEGEMODE_MAX_ANGLE_RADIAN+AGEAR_SIEGEMODE_MIN_CAP);
        }
    }
    (*o_vWeaponVel) = vTmpWeaponVel;
    (*o_vWeaponUp) = vTmpWeaponUp;    
}
//*/
void CShuttleChild::SetMatrix_Move_Ground(float fElapsedTime) // void CShuttleChild::SetEffectPos_A_Gear(float fElapsedTime)
{
    D3DXVec3Normalize(&m_vUp,&m_vUp);
    D3DXVec3Normalize(&m_vVel,&m_vVel);
    D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
    D3DXVec3Cross(&m_vVel,&m_vSideVel,&m_vUp);

    D3DXMatrixLookAtRH(&m_mMatrix, &m_vPos, &(m_vPos+m_vVel), &m_vUp);
    D3DXMatrixInverse(&m_mMatrix, NULL, &m_mMatrix );

}

//void CShuttleChild::Move_I_Gear(float fElapsedTime)
//{
//    FLOG( "CShuttleChild::Move_I_Gear(float fElapsedTime)" );
//    // B-Gear 와 I-Gear 는 구동방식이 같음
//    Move_B_Gear(fElapsedTime);
//}

void CShuttleChild::CheckBoosterState(float fElapsedTime,bool bSet,int nOldBoosterType)
{
    FLOG( "CShuttleChild::CheckBoosterState(float fElapsedTime,bool bSet,int nOldBoosterType)" );
    if(bSet)
    {
        m_nShuttleStopState = 0;
        if(m_nShuttleBoosterState == 0)
        {
            m_nShuttleBoosterState = 1;
        }
        else if(m_nShuttleBoosterState == 1)
        {
            m_nBooster = BOOSTER_TYPE_MIN;
            m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
        }
        else if(m_nShuttleBoosterState == 2)
        {
            m_nShuttleBoosterState = 3;
            m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
        }
        else if(m_nShuttleBoosterState == 3 )
        {
            m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
//                if( m_myShuttleInfo.CurrentEP && 
//                    m_bReChargeBURN && 
//                    m_bUseBooster &&
//                    (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo)) < 0.9f)
//                {
//                    m_nBooster = BOOSTER_TYPE_MAX;
//                }
//                else
//                {
//                    m_nBooster = BOOSTER_TYPE_MIN;
//                }
        }
    }
    else
    {
        if(m_nShuttleBoosterState == 1)
        {
            m_nShuttleBoosterState = 2;
        }
        else if(m_nShuttleBoosterState == 2)
        {
            if(m_fBoosterCheckTime >= 0.0f)
                m_fBoosterCheckTime -= fElapsedTime;
            if(m_fBoosterCheckTime <= 0.0f)
            {
                m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
                m_nShuttleBoosterState=0;
            }
        }
        else if(m_nShuttleBoosterState == 3)
        {
            if(m_fBoosterCheckTime >= 0.0f)
                m_fBoosterCheckTime -= fElapsedTime;
            if(m_fBoosterCheckTime <= 0.0f)
            {
                m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
                m_nShuttleBoosterState = 0;
            }
        }
    }
}

//void CShuttleChild::CheckAirBreakState(float fElapsedTime,bool bSet)
//{
//    FLOG( "CShuttleChild::CheckAirBreakState(float fElapsedTime,bool bSet)" );
//
//    // 2005-05-06 by jschoi
//    if(    m_nShuttleStopState == 3 &&
//        (IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind)) )
//    {
//        m_nShuttleStopState = 1;
//    }
//
//    if(bSet)
//    {
//        m_nBooster = BOOSTER_TYPE_STOP;
//        if(m_nShuttleStopState == 0)
//        {
//            m_nShuttleStopState = 1;
//        }
//        else if(m_nShuttleStopState == 1)
//        {
//            m_nBooster = BOOSTER_TYPE_STOP;
//            m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
//        }
//        else if(m_nShuttleStopState == 2)
//        {
//            m_nShuttleStopState = 3;
//            m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
//        }
//        else if(m_nShuttleStopState == 3 )
//        {
//            m_fShuttleSpeed = 0.0f;
////            m_fShuttleSpeed -= m_fShuttleSpeed*fElapsedTime;
////            if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
////            {
////                m_fShuttleSpeed += (m_fRealShuttleSpeedMin - m_fShuttleSpeed)*fElapsedTime;
////            }
//            if(m_fShuttleSpeed < 0)
//                m_fShuttleSpeed = 0;
//            m_nBooster = BOOSTER_TYPE_AIRBREAK;
//            m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
//        }
//    }
//    else
//    {
//        if(m_nShuttleStopState == 1)
//        {
//            m_nShuttleStopState = 2;
//        }
//        else if(m_nShuttleStopState == 2)
//        {
//            if(m_fBoosterCheckTime >= 0.0f)
//                m_fBoosterCheckTime -= fElapsedTime;
//            if(m_fBoosterCheckTime <= 0.0f)
//            {
//                m_fBoosterCheckTime = BOOSTER_KEY_CHECK_TIME;
//                m_nShuttleStopState = 0;
//            }
//        }
//        else if(m_nShuttleStopState == 3)
//        {
//            m_fShuttleSpeed = 0.0f;
////            m_fShuttleSpeed -= m_fShuttleSpeed*fElapsedTime;
////            if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
////            {
////                m_fShuttleSpeed += (m_fRealShuttleSpeedMin - m_fShuttleSpeed)*fElapsedTime;
////            }
//            if(m_fShuttleSpeed < 0)
//                m_fShuttleSpeed = 0;
//            m_nBooster = BOOSTER_TYPE_AIRBREAK;
//        }
//    }
//}

void CShuttleChild::SendPacket(int nType,UINT64 n)
{
    FLOG( "CShuttleChild::SendPacket(int nType,UINT64 n)" );
    char buffer[SIZE_MAX_PACKET];
    switch(nType)
    {
    case T_FC_MOVE_LANDING:
        {
            MSG_FC_MOVE_LANDING sMsg;
            memset(&sMsg,0x00,sizeof(sMsg));
            int nType = T_FC_MOVE_LANDING;
            sMsg.ClientIndex = m_myShuttleInfo.ClientIndex;
            sMsg.Position = m_vPos;
            memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
            memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
            g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
        }
        break;
    case T_FC_CHARACTER_APPLY_COLLISION_DAMAGE:
        {
            MSG_FC_CHARACTER_APPLY_COLLISION_DAMAGE sMsg;
            memset(&sMsg,0x00,sizeof(sMsg));
            sMsg.CollisionType = n;
              sMsg.SpeedOfCollision = m_fShuttleSpeed - m_fTempGravity;            // 2005-07-08 by ispark
            int nType = T_FC_CHARACTER_APPLY_COLLISION_DAMAGE;
            memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
            memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
            g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
        }
        break;
    case T_IC_PARTY_CANCEL_FLIGHT_POSITION:
        {
            MSG_IC_PARTY_CANCEL_FLIGHT_POSITION sMsg;
            memset(&sMsg,0x00,sizeof(sMsg));
            int nType = T_IC_PARTY_CANCEL_FLIGHT_POSITION;
            // 2007-11-22 by bhsohn 아레나 통합서버
            //sMsg.CharacterUniqueNumber = m_myShuttleInfo.CharacterUniqueNumber;
            sMsg.CharacterUniqueNumber = GetCharacterUniqueNumber();
            memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
            memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
            // 2007-11-22 by bhsohn 아레나 통합서버
            g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
        }
        break;
    case T_FC_MOVE_LANDING_DONE:
        {
            MSG_FC_MOVE_LANDING_DONE sMsg;
            memset(&sMsg,0x00,sizeof(sMsg));
            int nType = T_FC_MOVE_LANDING_DONE;
            sMsg.Position = m_vPos;
            memcpy(buffer,&nType,SIZE_FIELD_TYPE_HEADER);
            memcpy(buffer+SIZE_FIELD_TYPE_HEADER,&sMsg,sizeof(sMsg));
            g_pD3dApp->m_pFieldWinSocket->Write(buffer,SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
            m_bIsAir = FALSE;
            ChangeSingleBodyCondition(BODYCON_LANDED_MASK);
//            SendPacket(T_FC_MOVE);
//            g_pD3dApp->SendMovePacket(FALSE);
            g_fSendMovePacket = max(g_fSendMovePacket,5.0f);
            g_pD3dApp->m_pSound->StopD3DSound( SOUND_LANDING );
            g_pD3dApp->m_pSound->PlayD3DSound( SOUND_LANDING_DONE, m_vPos, FALSE );

        }
        break;
    case T_FC_CHARACTER_CHANGE_BODYCONDITION:
        {
            MSG_FC_CHARACTER_CHANGE_BODYCONDITION sMsg;
            memset(&sMsg,0x00,sizeof(sMsg));
            int nType = T_FC_CHARACTER_CHANGE_BODYCONDITION;
            sMsg.ClientIndex = m_myShuttleInfo.ClientIndex;
            sMsg.BodyCondition = n;
            memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
            memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
            g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
        }
        break;
    case T_FC_MOVE_TAKEOFF:
        {
            m_bAniKeyBlock = FALSE;
            MSG_FC_MOVE_TAKEOFF sMsg;
            memset(&sMsg,0x00,sizeof(sMsg));
            int nType = T_FC_MOVE_TAKEOFF;
            sMsg.ClientIndex = m_myShuttleInfo.ClientIndex;
            sMsg.Position = m_vPos;
            memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
            memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
            g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
            m_bIsAir = TRUE;
            if(g_pD3dApp->m_dwGameState == _SHOP)
            {
                g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
            }
            g_pD3dApp->m_pSound->PlayD3DSound( SOUND_TAKINGOFF, m_vPos, FALSE);
        }
        break;
    case T_FC_MOVE:
        {
            MSG_FC_MOVE sMsg;
            memset(&sMsg,0x00,sizeof(sMsg));
            sMsg.ClientIndex = m_myShuttleInfo.ClientIndex;
//            g_pD3dApp->m_dwCurTickTime = timeGetTime();
//            sMsg.TimeGap = g_pD3dApp->m_dwCurTickTime - g_pD3dApp->m_dwLastTickTime;
            sMsg.TimeGap = g_pD3dApp->m_dwSpeedHackTimer;
            //float fDistance = D3DXVec3Length(&(m_vPos-g_pD3dApp->m_vClientOldPos));
            //if(fDistance > 255)
            //    fDistance = 255.0f; 
            //sMsg.DistanceGap = (BYTE)fDistance;
            sMsg.PositionVector = m_vPos;
            sMsg.TargetVector = m_vVel*1000.0f;
            D3DXVECTOR3 vUp;
            vUp.x = m_mMatrix._21;
            vUp.y = m_mMatrix._22;
            vUp.z = m_mMatrix._23;
            sMsg.UpVector = vUp*1000.0f;
            int nType = T_FC_MOVE;
            memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
            memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
            g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
            g_pD3dApp->m_vClientOldPos = m_vPos;
//            g_pD3dApp->m_dwLastTickTime = g_pD3dApp->m_dwCurTickTime;
            g_pD3dApp->m_dwSpeedHackTimer = 0;
//#ifdef _DEBUG
//            m_nDebugSendPacketMove++;
//#endif
        }
        break;
    case T_FC_EVENT_LEAVE_BUILDING:
        {
            MSG_FC_EVENT_LEAVE_BUILDING sMsg;
            memset(&sMsg,0x00,sizeof(sMsg));
//            sMsg.BuildingIndex = g_pGameMain->m_pShop->m_pNPCWindow->m_buildingInfo.BuildingIndex;
            int nType = T_FC_EVENT_LEAVE_BUILDING;
            memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
            memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
            g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
        }
        break;
    }
}

//void CShuttleChild::CheckMoveRate(float fElapsedTime,int nSideMove, int nFBMove)
void CShuttleChild::CheckMoveRate(float fElapsedTime,int nSideMove)
{
    FLOG( "CShuttleChild::CheckMoveRate(float fElapsedTime,int nSideMove, int nFBMove)" );
    if(nSideMove)
    {
        // Ship의 좌표 변화...이동[좌,우]
        m_fLRVelRate += 10.0f*nSideMove*fElapsedTime;
        if(m_fLRVelRate < -1.0f)
            m_fLRVelRate = -1.0f;
        else if(m_fLRVelRate > 1.0f)
            m_fLRVelRate = 1.0f;
    }
    else
    {
        if(m_fLRVelRate > 0.0f)
        {
            m_fLRVelRate -= fElapsedTime;
            if(m_fLRVelRate < 0.0f)
                m_fLRVelRate = 0.0f;
        }
        else if(m_fLRVelRate < 0.0f)
        {
            m_fLRVelRate += fElapsedTime;
            if(m_fLRVelRate > 0.0f)
                m_fLRVelRate = 0.0f;
        }
    }
//    m_vNextPos = m_vPos + 0.5f*m_fLRVelRate*m_vSideVel*m_fShuttleSpeedMax; //  좌우 이동은 엔진의 최대 성능에 영향을 받자
    m_vNextPos = m_vPos + 0.5f*m_fLRVelRate*m_vSideVel*m_fRealShuttleSpeedMax; //  좌우 이동은 엔진의 최대 성능에 영향을 받자
    // ship의 움직임[앞,뒤] 키가 안눌러졌을때
    if(m_dwState == _NORMAL)
    {
        // 2005-06-03 by jschoi - 비행 시스템 변경
                
        if(IS_DT(m_myShuttleInfo.UnitKind) && !m_bIsAir )
        {// Move_Ground 
            // 2006-09-22 by ispark 튜토리얼만 연관성 있음
            if(g_pTutorial && g_pTutorial->IsTutorialMode() && g_pTutorial->GetLesson() == L0)
            {
                return;
            }

            if(m_bAttackMode != _SIEGE && m_bUnitStop == FALSE)
            {
                // 2008-11-13 by bhsohn 조이스틱 작업
                //if((g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W)
//                || g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_UP))&& 
//                    !g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S) && 
                if((g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W)
                    || g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_UP))&& 
                    !g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S) && 
                    m_bLandingField == FALSE 
                    &&    g_pD3dApp->m_bChatMode == FALSE)            // 2008-11-04 by dgwoo 착륙시 채팅중이 아니여만 조종이 가능.
                {
                    // 2005-09-28 by ispark 엔진 속도 0으로 인한 m_fRealShuttleSpeedMin -> m_fShuttleGroundSpeed으로 변경
                    m_fShuttleSpeed += (m_fShuttleGroundSpeed - m_fShuttleSpeed)*fElapsedTime;
                }
                // 2008-11-13 by bhsohn 조이스틱 작업
//                else if((g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S) 
//                          || g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_DOWN)) &&
//                         !g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W) && 
                else if((g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S) 
                          || g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_DOWN)) &&
                         !g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_W) && 
                         m_bLandingField == FALSE
                         && g_pD3dApp->m_bChatMode == FALSE)            // 2008-11-04 by dgwoo 착륙시 채팅중이 아니여만 조종이 가능.
                {
                    if(m_fShuttleSpeed > 0)
                    {
                        m_fShuttleSpeed -= (m_fShuttleSpeed + m_fShuttleGroundSpeed/2)*fElapsedTime;
                    }
                    else
                    {
                        m_fShuttleSpeed -= (m_fShuttleGroundSpeed/2 - m_fShuttleSpeed)*fElapsedTime;
                    }

                    if(m_fShuttleSpeed < -m_fShuttleGroundSpeed/2)
                    {
                        m_fShuttleSpeed = -m_fShuttleGroundSpeed/2;
                    }
                }
                else
                {

                    float fAddSpeed = m_fShuttleSpeed*fElapsedTime;

                    if ( fabs(fAddSpeed) < 0.005f )
                        m_fShuttleSpeed /= 2;
                    else
                        m_fShuttleSpeed -= fAddSpeed;

                    // 기준값 임의 처리. 소숫점 2자리이하 0일 경우 점감처리에서 바로 0으로 셋팅. 2010. 08. 24 by hsLee.
                    if ( (int)(m_fShuttleSpeed * 100.0f) == 0 )
                        m_fShuttleSpeed = .0f;
                    
                }
            }
            else
            {
                m_fShuttleSpeed = 0.0f;
            }

            if(m_bPenaltyUnitStop == TRUE && m_bUnitStop == FALSE)
            {
                m_bPenaltyUnitStop = FALSE;
                m_bMouseLock = FALSE;
                m_bMouseMoveLock = FALSE;
                g_pD3dApp->m_bFixUnitDirMode = FALSE;        
            }
        }    
//        else if(IS_DT(m_myShuttleInfo.UnitKind) && (m_bAttackMode == _SIEGE))
//        {// 2006-11-16 by dgwoo 에어시즈 모드일때.
//            //m_fShuttleSpeed -= m_fShuttleSpeed * fElapsedTime * 4.0f;
//            m_fShuttleSpeed = 0;
//            m_fTempGravity = 0.0f;
//            
//        }

        else
        {// Move_Air
            
            // 이 부분은 새로 만들어진 부분
            // 코드에서 아래쪽에 주석으로 된 부분이 예전 코드 
            // 유닛의 움직임에 대해 매우 민감한 부분이므로 반드시 확인 해야 함.

            // 2005-05-31 by jschoi - 유닛이 상하 이동 시 속도 차이를 두고 하강 시 빠른 속도로 이동
            float GRAVITY = 98.0f;        // 짝퉁 중력 가속도 
            float fTempAngle = ACOS(D3DXVec3Dot(&m_vVel,&D3DXVECTOR3(0,1,0)))/PI;
            float fTempGravity;
            if(fTempAngle > 0 && fTempAngle < 0.5f)
            {// 상승 중 
                fTempGravity = GRAVITY*(0.5f - fTempAngle)*2.0f;
            }
            else if(fTempAngle > 0.5f && fTempAngle < 1.0f)
            {// 하강 중 
                fTempGravity = -GRAVITY*(fTempAngle - 0.5f)*2.0f;
            }
            else
            {
                fTempGravity = 0;
            }

            if(m_fTempGravity > fTempGravity)
            {
                m_fTempGravity -= (m_fTempGravity - fTempGravity)*fElapsedTime;
            }
            else if(m_fTempGravity < fTempGravity)
            {
                m_fTempGravity += (fTempGravity - m_fTempGravity)*fElapsedTime;
            }
                 

            if(m_fTempGravity < -GRAVITY)
            {
                m_fTempGravity = -GRAVITY;
            }
            else if(m_fTempGravity > GRAVITY)
            {
                m_fTempGravity = GRAVITY;
            }

            // 2005-08-10 by ispark
            if((m_fRealShuttleSpeedMin > (m_fShuttleSpeed - m_fTempGravity)) && m_bPenaltyUnitStop == FALSE)
            {
                m_fTempGravity = m_fShuttleSpeed - m_fRealShuttleSpeedMin;
            }

            // 2005-06-03 by jschoi - S키 기능 재정의
            const float AIR_BREAK = 4.0f;    // Default 1.0f - 이 값이 크면 클수록 브레이크가 빠르게 적용된다.
            if(m_bAttackMode == _SIEGE)
            {
                m_fShuttleSpeed = 0;
                m_fTempGravity = 0.0f;
                if(m_fShuttleSpeed < 0.0f)
                {
                    m_fShuttleSpeed = 0.0f;
                }
            }
            // 2008-11-13 by bhsohn 조이스틱 작업
//            else if(((g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_S) &&
//                !g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE))|| // 스페이스바 안 눌렀을 때
            else if(((g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_S) &&
                !g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE))|| // 스페이스바 안 눌렀을 때
                m_bOrderMove || 
//                g_pD3dApp->m_bChatMode ||                // 2005-11-16 by ispark 채팅시 에어브레이크 삭제
                g_pD3dApp->m_bFixUnitDirMode) &&
                m_bUnitStop == FALSE &&
                m_bPenaltyUnitStop == FALSE)                                // 2005-08-08 by ispark 유닛정지시 사용 안함
            {

                float fAddSpeed = (m_fShuttleSpeed - m_fRealShuttleSpeedMin)*fElapsedTime*AIR_BREAK;

                if ( fabs(fAddSpeed) < 0.005f )
                    m_fShuttleSpeed /= 2;
                else
                    m_fShuttleSpeed -= fAddSpeed;

                // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
//                 if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
//                 {
//                     m_fShuttleSpeed = m_fRealShuttleSpeedMin;
//                 }                
                if(m_fReverserIgnoreTime <= 0)
                {
                    if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
                    {
                        m_fShuttleSpeed = m_fRealShuttleSpeedMin;
                    }
                }
                else
                {
                    m_fReverserIgnoreTime -= g_pD3dApp->GetCheckElapsedTime();
                    float fRealShuttleSpeedMin = abs(m_fRealShuttleSpeedMin);
                    if(m_fShuttleSpeed < fRealShuttleSpeedMin)
                    {
                        m_fShuttleSpeed = fRealShuttleSpeedMin;
                    }
                }                
                // END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
            }
            else if(m_bKeyDownA || m_bKeyDownD)
            {
                // 2005-07-07 by ispark
                // 롤잉 시스템 사용시 속도 변화
                // 서서히 원래 속도의 2/3으로 줄어든다. (좌,우 거리에 따라 줄어들게 하였음)
                float fLegth = D3DXVec3Length(&(m_vRollOldPos - m_vPos));    // 전 좌표와 비교하여 어느정도 지났는지 계산

                if(fLegth)
                {
                    if(80 > D3DXVec3Length(&(m_vRollStart - m_vPos)))        // 처음 시작후 좌, 우 이동 거리와 계산
                    {
                        // 속도를 거리에 따라 서서히 줄어 줌
                        m_fShuttleSpeed -= (80.0f / m_fRollDownSpeed) * (80.0f / fLegth);
                        
                        m_vRollOldPos = m_vPos;
                    }
                    else
                    {
                        // 거리 초과시 원래 속도의 2/3으로 줄어들게 함
                        m_fShuttleSpeed = m_fRollDownSpeed;
                    }
                }

                // 계단된 속도가 최저보다 작다면 작은 속도로 수정
                if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
                {
                    m_fShuttleSpeed = m_fRealShuttleSpeedMin;
                }
            }
            // 2008-11-13 by bhsohn 조이스틱 작업
            //else if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE)
            else if(g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE)
                && m_fShuttleSpeed < m_fRealShuttleSpeedBoosterOn
                && m_nBooster == BOOSTER_TYPE_MAX
                && m_bPenaltyUnitStop == FALSE)                    // 2005-07-11 by ispark // 부스터 사용중이라면 계산하라
            {
                m_fShuttleSpeed += (m_fRealShuttleSpeedBoosterOn - m_fShuttleSpeed)*fElapsedTime;
            }
            else if(m_bUnitStop == TRUE)                            // 2005-08-08 by ispark 유닛정지 추가
            {
                m_fShuttleSpeed = 0;
                m_fTempGravity = 0.0f;
                if(m_fShuttleSpeed < 0.0f)
                {
                    m_fShuttleSpeed = 0.0f;
                }
            }
            else if(m_bPenaltyUnitStop == TRUE)                        // 2005-08-12 by ispark 정지후 추발시 페널티 적용
            {
                float fPenaltySpeedMax;
                if((g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo)) > 0.9f)
                {
                    fPenaltySpeedMax = m_fRealShuttleSpeedMin;
                }
                else
                {
                    fPenaltySpeedMax = m_fRealShuttleSpeedMax;
                }
                // 2013-03-25 by bhsohn 홀드 스킬 버그 처리
                if( m_pSkill && m_pSkill->IsExistDesParamSKill( DES_SKILL_MON_HOLD ) )
                {
                    // 홀드스킬 걸리면 바로 이동 턴가능하게
                    fPenaltySpeedMax = 0.0f;
                }
                // END 2013-03-25 by bhsohn 홀드 스킬 버그 처리

                m_fShuttleSpeed += fElapsedTime * (fPenaltySpeedMax / UNIT_STOP_DELAY);
                // 2013-03-25 by bhsohn 홀드 스킬 버그 처리
//                if(m_fShuttleSpeed > fPenaltySpeedMax)
                if(m_fShuttleSpeed >= fPenaltySpeedMax)
                {
                    // 2005-11-17 by ispark
                    InitUnitStop();    
                    m_fShuttleSpeed = fPenaltySpeedMax;
                    m_fCurrentSideRate = 0.0f;

                    // 2005-10-12 by ispark    튜토리얼
                    if(g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() >= L2)
                    {
                        g_pTutorial->m_Lesson2State.nBKey = 2;
                    }
                }
            
//                SetCursorInit();
            }
            else
            {
                if(m_fShuttleSpeed < m_fRealShuttleSpeedMax)
                {
                    m_fShuttleSpeed += (m_fRealShuttleSpeedMax - m_fShuttleSpeed)*fElapsedTime;
                }
                else if(m_fShuttleSpeed > m_fRealShuttleSpeedMax)
                {
                    m_fShuttleSpeed -= (m_fShuttleSpeed - m_fRealShuttleSpeedMax)*fElapsedTime;
                }
            }
        }
/*
        // 2004-11-30 by jschoi
        if(m_nShuttleStopState == 4)    // 워프 아웃 패턴 종료시 에어 브레이크 서서히 속력을 줄인다.
        {
            m_fShuttleSpeed -= (m_fRealShuttleSpeedMin - m_fShuttleSpeed)*fElapsedTime;
            if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
            {
                m_fShuttleSpeed = m_fRealShuttleSpeedMin;
            }

//            if(m_fShuttleSpeed > 60.0f)
//            {
//                m_fShuttleSpeed -= m_fShuttleSpeed*fElapsedTime/2;
//            }
//            else
//            {
//                m_fShuttleSpeed -= 30.0f*fElapsedTime;
//            }
//
//            if(m_fShuttleSpeed < 0)
//            {
//                m_fShuttleSpeed = 0;
//
//                // 2005-05-11 by jschoi
////                m_nShuttleStopState = 3;
//                if(    IS_DT(m_myShuttleInfo.UnitKind) || IS_OT(m_myShuttleInfo.UnitKind) )
//                {// M,A 기어
//                    m_nShuttleStopState = 1;
//                }
//                else
//                {// B,I 기어
//                    m_nShuttleStopState = 3;
//                    m_nBooster = BOOSTER_TYPE_AIRBREAK;
//                    g_pD3dApp->m_pSound->StopD3DSound( SOUND_HIGH_BOOSTER );
//                }            
//                
//            }
        }
        else if(nFBMove == 0)
        {
            if(g_pD3dApp->m_dwGameState == _SHOP)
            {// 상점 이벤트 중일때 속도는 줄인다.
                if(m_fShuttleSpeed > 1.0f)
                {
                    m_fShuttleSpeed -= 50.0f*fElapsedTime;
                    if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
                    {
                        m_fShuttleSpeed = m_fRealShuttleSpeedMin;
                    }
//                    if(m_fShuttleSpeed < 0.0f)
//                    {
//                        m_fShuttleSpeed = 0.0f;
//                    }
                }
                else if(m_fShuttleSpeed < -1.0f)
                {
                    m_fShuttleSpeed += 50.0f*fElapsedTime;
                    if(m_fShuttleSpeed > 0.0f)
                    {
                        m_fShuttleSpeed = 0.0f;
                    }
                }
                else
                {
                    m_fShuttleSpeed = 0.0f;
                }
            }
            else
            {
                if(IS_BT(m_myShuttleInfo.UnitKind) || IS_ST(m_myShuttleInfo.UnitKind))
                {
                    if(m_nBooster == BOOSTER_TYPE_AIRBREAK)
                    {
//                        m_fShuttleSpeed = 0.0f;
                        m_fShuttleSpeed -= m_fShuttleSpeed*fElapsedTime;
                        if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
                        {
                            m_fShuttleSpeed = m_fRealShuttleSpeedMin;
                        }
//                        if(m_fShuttleSpeed < 0)
//                            m_fShuttleSpeed = 0;
                    }
                    else
                    {
                        // 속도가 오버 되었는지 체크
                        if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
                            m_fShuttleSpeed = m_fRealShuttleSpeedMin;
//                        else if(m_fShuttleSpeed > m_fShuttleSpeedBoosterOn)
//                            m_fShuttleSpeed = m_fShuttleSpeedBoosterOn;
                        else if(m_fShuttleSpeed > m_fRealShuttleSpeedBoosterOn)
                            m_fShuttleSpeed = m_fRealShuttleSpeedBoosterOn;
//                        else if(m_fShuttleSpeed > m_fShuttleSpeedMax)
                        else if(m_fShuttleSpeed > m_fRealShuttleSpeedMax)
                            m_fShuttleSpeed -= 30.0f*fElapsedTime;
                    }
                }
                else
                {
                    if(IS_DT(m_myShuttleInfo.UnitKind) && !m_bIsAir)
                    {
                        if(m_fShuttleSpeed > 1.0f)
                        {
                            m_fShuttleSpeed -= 20.0f*fElapsedTime;
                            if(m_fShuttleSpeed < 0.0f)
                            {
                                g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );
                                m_fShuttleSpeed = 0.0f;
                            }
                        }
                        else if(m_fShuttleSpeed < -1.0f)
                        {
                            m_fShuttleSpeed += 20.0f*fElapsedTime;
                            if(m_fShuttleSpeed > 0.0f)
                            {
                                g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );
                                m_fShuttleSpeed = 0.0f;
                            }
                        }
                        else
                        {
                            g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );
                            m_fShuttleSpeed = 0.0f;
                        }
                    }
                
                    // 앞 키가 안눌렀을때 속도 증감이 없다 현재 속도 유지
                    if(m_fShuttleSpeed < -1.0f)
                    {
                        m_fShuttleSpeed += 20.0f*fElapsedTime;
                        if(m_fShuttleSpeed > 0.0f)
                        {
                            g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );
                            m_fShuttleSpeed = 0.0f;
                        }
                    }
                    if((g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo)) > 0.9f)
                    {
                        if(m_fShuttleSpeed < 0)
                        {
                            g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );
                            m_fShuttleSpeed = 0;
                        }
                        else if(m_fShuttleSpeed > (m_fRealShuttleSpeedMin<0 ? 10 :m_fRealShuttleSpeedMin))
                        {
                            m_fShuttleSpeed = (m_fRealShuttleSpeedMin<0 ? 10 :m_fRealShuttleSpeedMin);
                        }
                    }
                    // 속도가 오버 되었는지 체크
                    if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
                        m_fShuttleSpeed = m_fRealShuttleSpeedMin;
//                    else if(m_fShuttleSpeed > m_fShuttleSpeedBoosterOn)
//                        m_fShuttleSpeed = m_fShuttleSpeedBoosterOn;
                    else if(m_fShuttleSpeed > m_fRealShuttleSpeedBoosterOn)
                        m_fShuttleSpeed = m_fRealShuttleSpeedBoosterOn;
//                    else if(m_fShuttleSpeed > m_fShuttleSpeedMax)
                    else if(m_fShuttleSpeed > m_fRealShuttleSpeedMax)
                        m_fShuttleSpeed -= 30.0f*fElapsedTime;
                }
            }
        }
        else
        {    // ship의 움직임[앞,뒤] 키가 눌러졌을때
            // 아래 3줄 ... 부스터 일때는 부스터 의 속도 만큼 가속도가 적용된다-일반비행시는 일반속도만큼.
            if(m_nBooster == BOOSTER_TYPE_AIRBREAK)
            {
//                m_fShuttleSpeed = 0.0f;
                m_fShuttleSpeed -= m_fShuttleSpeed*fElapsedTime;
                if(m_fShuttleSpeed < 0)
                {
                    g_pD3dApp->m_pSound->StopD3DSound( SOUND_GROUND_MOVING_A_GEAR );
                    m_fShuttleSpeed = 0;
                }
            }
            else
            {
                if(nFBMove == -1)
                {
                    if(m_fShuttleSpeed > m_fRealShuttleSpeedMin)
                        m_fShuttleSpeed -= 70.0f*fElapsedTime;
                    else if(m_fShuttleSpeed < m_fRealShuttleSpeedMin)
                        m_fShuttleSpeed = m_fRealShuttleSpeedMin;
                }
                else
                {
                    if(m_nBooster == BOOSTER_TYPE_MAX)
                    {
//                        if(m_fShuttleSpeed < m_fShuttleSpeedBoosterOn)
                        if(m_fShuttleSpeed < m_fRealShuttleSpeedBoosterOn)
                        {
//                            float fGap = m_fShuttleSpeedBoosterOn - m_fShuttleSpeed;
                            float fGap = m_fRealShuttleSpeedBoosterOn - m_fShuttleSpeed;
                            m_fShuttleSpeed += fGap*fElapsedTime;
//                            if(m_fShuttleSpeed > m_fShuttleSpeedBoosterOn)
//                                m_fShuttleSpeed = m_fShuttleSpeedBoosterOn;
                            if(m_fShuttleSpeed > m_fRealShuttleSpeedBoosterOn)
                                m_fShuttleSpeed = m_fRealShuttleSpeedBoosterOn;
                        }
                    }
                    else
                    {
//                        if(m_fShuttleSpeed < m_fShuttleSpeedMax)
                        if(m_fShuttleSpeed < m_fRealShuttleSpeedMax)
                        {
//                            float fGap = m_fShuttleSpeedMax - m_fShuttleSpeed;
                            float fGap = m_fRealShuttleSpeedMax - m_fShuttleSpeed;
                            m_fShuttleSpeed += fGap*fElapsedTime;
//                            if(m_fShuttleSpeed > m_fShuttleSpeedMax)
//                                m_fShuttleSpeed = m_fShuttleSpeedMax;
                            if(m_fShuttleSpeed > m_fRealShuttleSpeedMax)
                                m_fShuttleSpeed = m_fRealShuttleSpeedMax;
                        }
//                        else if(m_fShuttleSpeed > m_fShuttleSpeedMax)
                        else if(m_fShuttleSpeed > m_fRealShuttleSpeedMax)
                        {
//                            float fGap = m_fShuttleSpeed - m_fShuttleSpeedMax;
                            float fGap = m_fShuttleSpeed - m_fRealShuttleSpeedMax;
                            m_fShuttleSpeed -= fGap*fElapsedTime;
//                            if(m_fShuttleSpeed < m_fShuttleSpeedMax)
//                                m_fShuttleSpeed = m_fShuttleSpeedMax;
                            if(m_fShuttleSpeed < m_fRealShuttleSpeedMax)
                                m_fShuttleSpeed = m_fRealShuttleSpeedMax;
                        }
                    }
                }
            }
        }

*/

    }
    else if(m_dwState == _LANDING)
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        D3DXVECTOR3 myShutllePos, myShutlleNextPos;

        myShutllePos = m_vPos + m_vVel * TILE_SIZE;
        
        D3DXVec3Normalize(&m_vVel, &(myShutllePos - m_vPos));
                
        myShutlleNextPos = m_vPos;                                
        myShutlleNextPos = myShutlleNextPos + m_vVel * TILE_SIZE;
        myShutlleNextPos.x = (int(myShutlleNextPos.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
        myShutlleNextPos.z = (int(myShutlleNextPos.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
        
        int nXCheck = (int)(myShutlleNextPos.x / TILE_SIZE);
        int nYCheck = (int)(myShutlleNextPos.z / TILE_SIZE);                                
        m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize + nYCheck].bEnableLand;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        if(m_fShuttleSpeed > 1.0f)
        {
            m_fShuttleSpeed -= 30.0f*fElapsedTime;            // 2005-10-26 by ispark 속도가 빠른 관계로 빨리 줄임
            if(m_fShuttleSpeed < 0.0f)
            {
                m_fShuttleSpeed = 0.0f;
            }
        }
        else if(m_fShuttleSpeed < -1.0f)
        {
            m_fShuttleSpeed += 5.0f*fElapsedTime;
            if(m_fShuttleSpeed > 0.0f)
            {
                m_fShuttleSpeed = 0.0f;
            }
        }
        else
        {
            m_fShuttleSpeed = 0.0f;
        }

        if(m_bMoveGroundType != 1) // 추가 코드 ydkim
        {
            m_fShuttleSpeed = 0.0f;
        }

        // 2005-06-03 by jschoi
        m_fTempGravity = 0.0f;
    }
    else
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        D3DXVECTOR3 myShutllePos, myShutlleNextPos;

        myShutllePos = m_vPos + m_vVel * TILE_SIZE;
        
        D3DXVec3Normalize(&m_vVel, &(myShutllePos - m_vPos));
                
        myShutlleNextPos = m_vPos;                                
        myShutlleNextPos = myShutlleNextPos + m_vVel * TILE_SIZE;
        myShutlleNextPos.x = (int(myShutlleNextPos.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
        myShutlleNextPos.z = (int(myShutlleNextPos.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
        
        int nXCheck = (int)(myShutlleNextPos.x / TILE_SIZE);
        int nYCheck = (int)(myShutlleNextPos.z / TILE_SIZE);                                
        m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize + nYCheck].bEnableLand;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        if(m_fShuttleSpeed > 1.0f)
        {
            m_fShuttleSpeed -= 80.0f*fElapsedTime;            // 2005-10-26 by ispark 속도가 빠른 관계로 빨리 줄임
            if(m_fShuttleSpeed < 0.0f)
            {
                m_bLandedMove = TRUE;
                m_fShuttleSpeed = 0.0f;
            }
        }
        else if(m_fShuttleSpeed < -1.0f)
        {
            m_fShuttleSpeed += 30.0f*fElapsedTime;
            if(m_fShuttleSpeed > 0.0f)
            {
                m_bLandedMove = TRUE;
                m_fShuttleSpeed = 0.0f;
            }
        }
        else
        {
            m_bLandedMove = TRUE;
            m_fShuttleSpeed = 0.0f;
        }

        if(m_bMoveGroundType != 1) // 추가 코드 ydkim TEST
        {
            m_fShuttleSpeed = 0.0f;
        }

        // 2005-06-03 by jschoi
        m_fTempGravity = 0.0f;
    }

    if((g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo)) > 0.9f)
    {
        if(m_fShuttleSpeed < 0)
        {
            m_fShuttleSpeed = 0;
        }
        else if(m_fShuttleSpeed > (m_fRealShuttleSpeedMin<0 ? 10 :m_fRealShuttleSpeedMin))
        {
            m_fShuttleSpeed = (m_fRealShuttleSpeedMin<0 ? 10 :m_fRealShuttleSpeedMin);
        }
    }

    if(m_dwState == _NORMAL)
    {
        // 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

        //m_vNextPos += m_vVel*(m_fShuttleSpeed - m_fTempGravity);
        // 홀드 스킬 걸리면 중력 계산하지 않는다
        if( !m_pSkill->IsExistDesParamSKill( DES_SKILL_MON_HOLD ) )
            m_vNextPos += m_vVel*(m_fShuttleSpeed - m_fTempGravity);
        else
            m_vNextPos += m_vVel*m_fShuttleSpeed;

        // end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
    }
    else
    {
        m_vNextPos += m_vVel*m_fShuttleSpeed;
    }

    m_vMoveVel = m_vNextPos - m_vPos;
    D3DXVec3Normalize(&m_vMoveVel, &m_vMoveVel);            // 2006-07-13 by ispark, 기어 이동 방향

// 2005-08-08 by ispark
// 기존 F11키를 없앤다.
//#ifdef _DEBUG
//    if(m_bUnitStop)
//    {
//        m_fShuttleSpeed = 0.0f;
//        m_fTempGravity = 0.0f;
//        m_fRealShuttleSpeedMin = 0.0f;
//    }
//#endif
}

#define SHUTTLE_MAP_COLL_RATE            5.0f
#define SHUTTLE_AUTO_FLIGHTINIT_HEIGHT    30.0f
#define SHUTTLE_MAX_MOUSE_MOVE            100

void CShuttleChild::CheckCollForMap()
{
    FLOG( "CShuttleChild::CheckCollForMap()" );
#ifdef _NOCLIP
    if (g_pD3dApp->m_bNoclip == TRUE)
        return;
#endif
    // 2005-02-02 by jschoi
    // 우주맵은 지형과 충돌검사를 하지 않는다.
    // 2005-12-12 by ispark, 지형을 그리지 않는다면 충돌검사를 안한다. (동굴, 우주맵)
    // 2007-06-18 by bhsohn 알트텝 관련 버그 처리
    //if(!IsTileMapRenderEnable(m_myShuttleInfo.MapChannelIndex.MapIndex))
    if(!IsTileMapTickEnable(m_myShuttleInfo.MapChannelIndex.MapIndex))
    {
        // 2007-01-04 by bhsohn 무브 스킬로 맵 타일에 부딪혔을 에 대한 버그처리
        m_bCollMap = FALSE;

        return;
    }

    float fElapsedTime = g_pD3dApp->GetElapsedTime();
    // 맵과의 충돌 검사-셔틀의 중요 4부분의 충돌 검사를 하자 앞 뒤 좌 우(4 귀퉁이 좌표를 맞추는 작업도 해줘야할 것이다)
    D3DXMATRIX    m;

    D3DXVECTOR3 vSide, vVel;
//    float fAngle;

    vVel = m_vVel;
    vVel.y = 0.0f;
    D3DXVec3Normalize(&vVel,&vVel);

    vSide.x = m_mMatrix._11;
    vSide.y = m_mMatrix._12;
    vSide.z = m_mMatrix._13;
    vSide *= -1.0f;
    
    D3DXVECTOR3 vFront, vBack, vLeft, vRight, vParticlePos;
    vFront = m_vPos + 8.0f*m_vVel;
    vBack = m_vPos - 3.0f*m_vVel;
    vLeft = m_vPos - 7.0f*vSide;
    vRight = m_vPos + 7.0f*vSide;
    
    m_bCheckMove = 4;
    float fMapHeightTemp;
    BOOL bIsColl = FALSE;
    m_bCollMap = FALSE;
    fMapHeightTemp = CheckMap(vFront);
    if(m_bCollMap)
    {// 앞부분 충돌 - 좌표 이동후 위쪽으로 방향이 틀림
        vParticlePos = vFront;
        if(g_pD3dApp->m_pEffectList && !g_pScene->FindEffect(RC_EFF_COLL_01))
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }

//        m_vPos += SHUTTLE_MAP_COLL_RATE*m_vNormal*fElapsedTime;
        m_vPos += (m_fShuttleSpeed / 2.0f)*m_vNormal*fElapsedTime;
        D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
//        m_fShuttleSpeed = m_fRealShuttleSpeedMin>30 ? m_fRealShuttleSpeedMin : 30;
        if(m_vUp.y >= 0.0f)// 바로 비행할때
        {
            // 2005-12-01 by ispark, 저비행에서는 충돌시 방향전환이 약해서 강해게 함
//            if(m_fShuttleSpeed < 80.0f)
//            {
//                D3DXMatrixRotationAxis(&m,&vSide,-5.0f*fElapsedTime);
//            }
//            else
//            {
//                D3DXMatrixRotationAxis(&m,&vSide,-0.05f*m_fShuttleSpeed*fElapsedTime);
//            }
            // 2006-05-09 by ispark, 충돌 반사 코드 변경
            GetCollReflect(&m_vVel, &m_vVel, &m_vNormal);
        }
        else// 거꾸로 비행할때
        {
//            if(m_fShuttleSpeed < 80.0f)
//            {
//                D3DXMatrixRotationAxis(&m,&vSide,-5.0f*fElapsedTime);
//            }
//            else
//            {
//                D3DXMatrixRotationAxis(&m,&vSide,0.05f*m_fShuttleSpeed*fElapsedTime);
//            }
            // 2006-05-09 by ispark, 충돌 반사 코드 변경
            GetCollReflect(&m_vVel, &m_vVel, &m_vNormal, FALSE);
        }
//        D3DXVec3TransformCoord(&m_vVel,&m_vVel,&m);
//        D3DXVec3TransformCoord(&m_vUp,&m_vUp,&m);
        // 마우스 좌표 위로 이동 - 50만큼
//        POINT pt,pt2;
//        GetCursorPos(&pt);
//        pt2 = pt;
//        ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
//        CheckMouseReverse(&pt);
//
//        int nY = pt2.y - pt.y;
//        nY += g_pD3dApp->GetBackBufferDesc().Height/2 - SHUTTLE_MAX_MOUSE_MOVE*g_pD3dApp->GetBackBufferDesc().Height/600;
//        SetCursorPos(pt2.x,nY);
        bIsColl = TRUE;
    }
    m_bCollMap = FALSE;
    fMapHeightTemp = CheckMap(vBack);
    if(m_bCollMap)
    {// 뒤부분 충돌
        vParticlePos = vBack;
        if(g_pD3dApp->m_pEffectList && !g_pScene->FindEffect(RC_EFF_COLL_01))
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
//        m_fShuttleSpeed = m_fRealShuttleSpeedMin>30 ? m_fRealShuttleSpeedMin : 30;
        m_vPos += SHUTTLE_MAP_COLL_RATE*m_vNormal*fElapsedTime;
        // 2006-05-09 by ispark, 충돌 반사 코드 변경
        GetCollReflect(&m_vVel, &m_vVel, &m_vNormal);
        bIsColl = TRUE;
    }
    m_bCollMap = FALSE;
    fMapHeightTemp = CheckMap(vLeft);
    if(m_bCollMap)
    {// 좌측 부분 충돌
        vParticlePos = vLeft;
        if(g_pD3dApp->m_pEffectList && !g_pScene->FindEffect(RC_EFF_COLL_01))
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
        m_vPos += SHUTTLE_MAP_COLL_RATE*m_vNormal*fElapsedTime;
//        m_fShuttleSpeed = m_fRealShuttleSpeedMin>30 ? m_fRealShuttleSpeedMin : 30;
//        D3DXMatrixRotationAxis(&m,&m_vUp,0.05f*m_fShuttleSpeed*fElapsedTime);
//        D3DXVec3TransformCoord(&m_vVel,&m_vVel,&m);
        // 2006-05-09 by ispark, 충돌 반사 코드 변경
        GetCollReflect(&m_vVel, &m_vVel, &m_vNormal);
        //마우스 좌표 우로 이동 - 50만큼
//        POINT pt,pt2;
//        GetCursorPos(&pt);
//        pt2 = pt;
//        ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
//        CheckMouseReverse(&pt);
//
//        int nX = pt2.x - pt.x;
//        nX += g_pD3dApp->GetBackBufferDesc().Width/2 + SHUTTLE_MAX_MOUSE_MOVE*g_pD3dApp->GetBackBufferDesc().Width/800;
//        SetCursorPos(nX,pt2.y);
        bIsColl = TRUE;
    }
    m_bCollMap = FALSE;
    fMapHeightTemp = CheckMap(vRight);
    if(m_bCollMap)
    {// 우측 부분 충돌
        vParticlePos = vRight;
        if(g_pD3dApp->m_pEffectList && !g_pScene->FindEffect(RC_EFF_COLL_01))
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
        m_vPos += SHUTTLE_MAP_COLL_RATE*m_vNormal*fElapsedTime;
//        m_fShuttleSpeed = m_fRealShuttleSpeedMin>30 ? m_fRealShuttleSpeedMin : 30;
//        D3DXMatrixRotationAxis(&m,&m_vUp,-0.05f*m_fShuttleSpeed*fElapsedTime);
//        D3DXVec3TransformCoord(&m_vVel,&m_vVel,&m);
        // 2006-05-09 by ispark, 충돌 반사 코드 변경
        GetCollReflect(&m_vVel, &m_vVel, &m_vNormal);
        //마우스 좌표 좌로 이동 - 50만큼
//        POINT pt,pt2;
//        GetCursorPos(&pt);
//        pt2 = pt;
//        ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
//        CheckMouseReverse(&pt);
//
//        int nX = pt2.x - pt.x;
//        nX += g_pD3dApp->GetBackBufferDesc().Width/2 - SHUTTLE_MAX_MOUSE_MOVE*g_pD3dApp->GetBackBufferDesc().Width/800;
//        SetCursorPos(nX,pt2.y);
        bIsColl = TRUE;
    }
    m_bCollMap = FALSE;
    fMapHeightTemp = CheckMap(m_vPos - m_fAmorHeight*m_vUp);
    if(m_bCollMap)
    {// 아래부분 충돌
        vParticlePos = m_vPos - m_fAmorHeight*m_vUp;
        if(g_pD3dApp->m_pEffectList && !g_pScene->FindEffect(RC_EFF_COLL_01))
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
        m_vPos += SHUTTLE_MAP_COLL_RATE*m_vNormal*fElapsedTime;
        D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
//        m_fShuttleSpeed = m_fRealShuttleSpeedMin>30 ? m_fRealShuttleSpeedMin : 30;
//        D3DXMatrixRotationAxis(&m,&vSide,-0.05f*m_fShuttleSpeed*fElapsedTime);
//        D3DXVec3TransformCoord(&m_vVel,&m_vVel,&m);
//        D3DXVec3TransformCoord(&m_vUp,&m_vUp,&m);
        // 2006-05-09 by ispark, 충돌 반사 코드 변경
        GetCollReflect(&m_vVel, &m_vVel, &m_vNormal);
        //마우스 좌표 위로 이동 - 50만큼
//        POINT pt,pt2;
//        GetCursorPos(&pt);
//        pt2 = pt;
//        ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
//        CheckMouseReverse(&pt);
//
//        int nY = pt2.y - pt.y;
//        nY += g_pD3dApp->GetBackBufferDesc().Height/2 - SHUTTLE_MAX_MOUSE_MOVE*g_pD3dApp->GetBackBufferDesc().Height/600;
//        SetCursorPos(pt2.x,nY);
        bIsColl = TRUE;
    }
    m_bCollMap = FALSE;
    fMapHeightTemp = CheckMap(m_vPos + m_fAmorHeight*m_vUp);
    if(m_bCollMap)
    {// 위부분 충돌
        vParticlePos = m_vPos + m_fAmorHeight*m_vUp;
        if(g_pD3dApp->m_pEffectList && !g_pScene->FindEffect(RC_EFF_COLL_01))
        {
            CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
            pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
        }
        m_vPos += SHUTTLE_MAP_COLL_RATE*m_vNormal*fElapsedTime;
        D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
//        m_fShuttleSpeed = m_fRealShuttleSpeedMin>30 ? m_fRealShuttleSpeedMin : 30;
//        D3DXMatrixRotationAxis(&m,&vSide,0.05f*m_fShuttleSpeed*fElapsedTime);
//        D3DXVec3TransformCoord(&m_vVel,&m_vVel,&m);
//        D3DXVec3TransformCoord(&m_vUp,&m_vUp,&m);
        // 2006-05-09 by ispark, 충돌 반사 코드 변경
        GetCollReflect(&m_vVel, &m_vVel, &m_vNormal);
        //마우스 좌표 아래로 이동 - 50만큼
//        POINT pt,pt2;
//        GetCursorPos(&pt);
//        pt2 = pt;
//        ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
//        CheckMouseReverse(&pt);
//
//        int nY = pt2.y - pt.y;
//        nY += g_pD3dApp->GetBackBufferDesc().Height/2 + SHUTTLE_MAX_MOUSE_MOVE*g_pD3dApp->GetBackBufferDesc().Height/600;
//        SetCursorPos(pt2.x,nY);
        bIsColl = TRUE;
    }

    // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    //if(bIsColl)
    
    // 로봇기어 평상시와 이륙시 높이 차이가 많이나 굴곡있는 지형에서 이륙시
    // 충돌 처리로 인해 에너지가 깍이므로 A기어 이륙중일시 충돌패킷 보내지 않음
    if( bIsColl
        && !( IS_DT( m_myShuttleInfo.UnitKind ) && ( GetCurrentBodyCondition() & BODYCON_TAKEOFF_MASK ) ) )
    // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

    {
        // 지형과 충돌시
        m_fShuttleSpeed = m_fRealShuttleSpeedMin>60 ? m_fRealShuttleSpeedMin : 60;
        g_pCamera->ChangeCamCollType();
        if(m_fCollSendDamageCheckTime < 0.0f)
        {
            if(m_myShuttleInfo.CurrentHP > 0.0f && m_myShuttleInfo.Level > 5 && m_fShuttleSpeed != 0)
            {
                // 관리자, 운영자, 데모계정 데미지 없음
//                if(!COMPARE_RACE(m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER|RACE_DEMO))
                {
                    SendPacket(T_FC_CHARACTER_APPLY_COLLISION_DAMAGE, COLLISION_TYPE_LAND);
                    m_fCollSendDamageCheckTime = 0.5f;

                    // 2008-11-13 by bhsohn 조이스틱 작업
                    // 지형충돌
                    g_pD3dApp->SetJoyStickFeedback(TRUE, JOY_FEED_COLLISION_DAMAGE);
                    // end 2008-11-13 by bhsohn 조이스틱 작업
                }
            }
        }
//        if(m_bFormationFlight)
        if(m_pClientParty->IsFormationFlightMove())
        {
            SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
            m_pClientParty->FormationFlightClear();
//            m_infoParty.bFormationFlyingPositionType = 0;
//            m_bFormationFlight = FALSE;
            g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_PARTY_0001,COLOR_SYSTEM);//"충돌이 발생하여 편대비행에서 벗어났습니다."
        }

    }
//    if(bSetShuttleFlightInit)
//        SetShuttleFlightInit(fElapsedTime,true);
}

void CShuttleChild::CheckCollForObject(float fElapsedTime)
{
    FLOG( "CShuttleChild::CheckCollForObject(float fElapsedTime)" );

#ifdef _NOCLIP
    if (g_pD3dApp->m_bNoclip == TRUE)
        return;
#endif
    // 2007-03-21 by dgwoo 옵저버 모드시 충돌처리는 하지 않는다.
    if(IsObserverMode())
        return;
    //*--------------------------------------------------------------------------*//
    // 2006-11-08 by ispark
    if(CheckCollForObjectFrontByDistance(fElapsedTime, m_vPos, m_vOldPos))
        return;
    //*--------------------------------------------------------------------------*//

    // 2007-01-04 by bhsohn 기체 착지 후 몬스터와 충돌 처리 안되는 문제 처리
//    if(m_bOrderMoveType)
//    {
//        return;
//    }
    // end 2007-01-04 by bhsohn 기체 착지 후 몬스터와 충돌 처리 안되는 문제 처리
    
    m_vCollCenter = D3DXVECTOR3(0,0,0);
    // 오브젝트 및 몬스터, 타캐릭터 와의 충돌 검출
    
    COLLISION_RESULT collResult;
    
    D3DXMATRIX matArray[6];
    D3DXVECTOR3 vSide,vVel,vParticlePos;
    
    D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
    
    D3DXMatrixLookAtLH(&matArray[COLL_FRONT],&m_vPos,&(m_vPos + m_vVel),&m_vUp);    // 앞
    D3DXMatrixLookAtLH(&matArray[COLL_BACK],&m_vPos,&(m_vPos - m_vVel),&m_vUp);    // 뒤
    D3DXMatrixLookAtLH(&matArray[COLL_LEFT],&m_vPos,&(m_vPos - vSide),&m_vUp);        // 좌
    D3DXMatrixLookAtLH(&matArray[COLL_RIGHT],&m_vPos,&(m_vPos + vSide),&m_vUp);        // 우
    D3DXMatrixLookAtLH(&matArray[COLL_UP],&m_vPos,&(m_vPos + m_vUp),&vSide);        // 위
    D3DXMatrixLookAtLH(&matArray[COLL_DOWN],&m_vPos,&(m_vPos - m_vUp),&vSide);        // 아래
    
    float fFrontMove = m_fShuttleSpeed*fElapsedTime;
//    float fSideMove = m_fLRVelRate*m_fRealShuttleSpeedMax*fElapsedTime;
    float size[6] = { 10+fFrontMove,10-fFrontMove,10,10,5,5 };    // 유닛 크기    앞,뒤,좌,우,위,아래
    
    // 오브젝트와 충돌 처리 2004.06.18 jschoi
    
    if(    /*m_bCollObjectCheck && */
        g_pScene &&
        g_pScene->m_byMapType != MAP_TYPE_TUTORIAL &&
        g_pScene->m_pObjectRender)
    {
        collResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray,size);
        
        // 2006-06-29 by ispark 충돌 버그 수정
        vVel = collResult.vNormalVector;
//        D3DXVECTOR3 vIVel = -1.0f * m_vVel;
        D3DXVECTOR3 vIVel = -1.0f * m_vMoveVel;
        D3DXVECTOR3 vNewVel;
        float fDot1 = D3DXVec3Dot(&vIVel,&vVel);
//        if(collResult.nCollType != COLL_NONE)
//            DBGOUT("방향 %f %f %f(dot = %f)%f %f %f\n", m_vMoveVel.x, m_vMoveVel.y, m_vMoveVel.z, fDot1, vVel.x, vVel.y, vVel.z);
//        DBGOUT("거리 %f(Type = %d)\n", collResult.fDist, collResult.nCollType);

        if((collResult.nCollType != COLL_NONE) && fDot1 > 0) // 충돌이 있다면..
        {
            if(m_myShuttleInfo.Level > 5 && m_fCollSendDamageCheckTime < 0.0f && m_fShuttleSpeed != 0)
            {
                // 관리자, 운영자, 데모계정 데미지 없음
//                if(!COMPARE_RACE(m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER|RACE_DEMO))
                {
                    SendPacket(T_FC_CHARACTER_APPLY_COLLISION_DAMAGE, COLLISION_TYPE_BUILDING);
                    m_fCollSendDamageCheckTime = 1.0f;
                    // 2008-11-13 by bhsohn 조이스틱 작업
                    // 지형충돌
                    g_pD3dApp->SetJoyStickFeedback(TRUE, JOY_FEED_COLLISION_DAMAGE);
                    // end 2008-11-13 by bhsohn 조이스틱 작업
                }
            }
            m_fCheckCollTime = 0.0f;

            m_fReverserIgnoreTime = 0.5f; // 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리

            // 2006-07-13 by ispark, 비행시에는 최저 속도로 변경하지만, 땅에서 다닐경우 20 속도로한다.
            // 땅에서는 속도가 매우 크지 않기 때문이다.
            if(m_bIsAir == TRUE)
            {
                m_fShuttleSpeed = m_fRealShuttleSpeedMin>60 ? m_fRealShuttleSpeedMin : 60;
            }
            else
            {
                m_fShuttleSpeed = 20.0f;
            }

            g_pCamera->ChangeCamCollType();
            g_pCamera->SetSmoothMove();
//            DBGOUT("충돌--------------> %f %f %f\n", vVel.x, vVel.y, vVel.z);
            m_vPos = m_vOldPos + 0.5f*m_fShuttleSpeed*vVel*fElapsedTime;
//            float fAngleDegree = ACOS(D3DXVec3Dot(&m_vVel,&vVel))/PI;
//            D3DXVec3Normalize(&m_vVel,&(m_vVel + fAngleDegree*vVel));
            // 2006-08-08 by ispark, 맵 끝에서 충돌
            if(m_vPos.y > g_pScene->m_nMaxAtitudeHeight + g_pGround->m_projectInfo.fWaterHeight)
            {
                vNewVel.x = g_pGround->m_projectInfo.sXSize*TILE_SIZE/2.0f+RAND100();
                vNewVel.y = 0.0;
                vNewVel.z = g_pGround->m_projectInfo.sYSize*TILE_SIZE/2.0f+RAND100();
            }
            else
            {
                // 2006-05-06 by ispark, 충돌 반사 코드 변경
                vNewVel = (2 * fDot1) * vVel + m_vVel;
            }

            D3DXVec3Normalize(&m_vVel,&vNewVel);
            D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
            D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
            vParticlePos = m_vPos + 8.0f*m_vVel;
            
            if(collResult.nCollType == COLL_WHOLE)
            {
                // 이곳으로 들어온다면 나중에 뭔가 처리해 주자.
                DBGOUT("Whole Collision \n");
            }
            if(g_pD3dApp->m_pEffectList)
            {
                CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
            }
            SetCursorInit();

//            if(m_bFormationFlight)
            if(m_pClientParty->IsFormationFlightMove())
            {
                SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
                m_pClientParty->FormationFlightClear();
//                m_infoParty.bFormationFlyingPositionType = 0;
//                m_bFormationFlight = FALSE;
                g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_PARTY_0001,COLOR_SYSTEM);
            }

            // 2005-11-17 by ispark 페널티 상황 종료
            if(m_bPenaltyUnitStop == TRUE)
            {
                InitUnitStop();
            }
        }
    }
    
    
    // 맵과의 충돌이 없을때만 몬스터 셔틀과의 충돌 검사를 한다.
    if(collResult.nCollType == COLL_NONE && !m_bCollMap)
    {
        if(collResult.nCollType == COLL_NONE && m_bCollMonsterCheck)
        {
            float fColledMonsterSize = 0;
            ClientIndex_t monsterIndex = 0;
            collResult = g_pScene->m_pMonsterRender->CheckCollMesh(matArray,size,&fColledMonsterSize, &monsterIndex);
            //2011-06-01 by jhahn    충돌처리 시간체크 변경
            //if(collResult.nCollType != COLL_NONE && m_fCollSendDamageCheckTime < 0.0f) // 충돌이 있다면..    
            if(collResult.nCollType != COLL_NONE) // 충돌이 있다면..
            // end 2011-06-01 by jhahn    충돌처리 시간체크 변경
            {
                char buffer[512];
                INIT_MSG(MSG_FC_CHARACTER_APPLY_COLLISION_DAMAGE, T_FC_CHARACTER_APPLY_COLLISION_DAMAGE, pSColl, buffer);
                pSColl->CollisionType    = COLLISION_TYPE_MONSTER;
                pSColl->SpeedOfCollision = m_fShuttleSpeed - m_fTempGravity;            // 2005-07-11 by ispark    // 충돌처리
                pSColl->UnitIndex        = monsterIndex;
                g_pD3dApp->m_pFieldWinSocket->Write(buffer, MSG_SIZE(MSG_FC_CHARACTER_APPLY_COLLISION_DAMAGE));

                // 2008-11-13 by bhsohn 조이스틱 작업
                // 지형충돌
                g_pD3dApp->SetJoyStickFeedback(TRUE, JOY_FEED_COLLISION_DAMAGE);
                // end 2008-11-13 by bhsohn 조이스틱 작업

                // 2004-11-24 by ydkim
                //SendPacket(T_FC_CHARACTER_APPLY_COLLISION_DAMAGE, COLLISION_TYPE_MONSTER);

                m_fCollSendDamageCheckTime = 1.0f;
                m_fCheckCollTime = 0.0f;

                m_fShuttleSpeed = m_fRealShuttleSpeedMin>30 ? m_fRealShuttleSpeedMin : 30;
   
                g_pCamera->ChangeCamCollType();
                g_pCamera->SetSmoothMove();
                vVel = collResult.vNormalVector;
                // 2004-10-22 by jschoi 충돌 시 회전 값 설정
                fColledMonsterSize = fColledMonsterSize/100;
                if(fColledMonsterSize < 0.2f)
                {    
                    fColledMonsterSize = 0.2f;
                }
                else if(fColledMonsterSize > 1.0f)
                {
                    fColledMonsterSize = 1.0f;
                    m_vPos = m_vOldPos + 0.5f*m_fShuttleSpeed*vVel*fElapsedTime;
                    float fColledMonsterSize = ACOS(D3DXVec3Dot(&m_vVel,&vVel))/PI;
                }
                D3DXVec3Normalize(&m_vVel,&(m_vVel + fColledMonsterSize*vVel));
                D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
                D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
                vParticlePos = m_vPos;
                
                if(collResult.nCollType == COLL_WHOLE)
                {
                    // 이곳으로 들어온다면 나중에 뭔가 처리해 주자.
                    DBGOUT("Whole Collision \n");
                }
                if(g_pD3dApp->m_pEffectList)
                {
                    CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
                    pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
                }
                SetCursorInit();

                // 2005-11-17 by ispark 페널티 상황 종료
                if(m_bPenaltyUnitStop == TRUE)
                {
                    InitUnitStop();
                }
            }
        }
        // 셔틀 충돌 현재 사용 안함
        //    else if(nCollType == COLL_NONE && m_bCollShuttleCheck)
        //    {
        //        nCollType = g_pD3dApp->m_pShuttleRender->CheckCollMesh(matArray,size);
        //    }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CShuttleChild::CheckCollForObjectFrontByDistance(float fElapsedTime, D3DXVECTOR3 vNewPos D3DXVECTOR3 vOldPos)
/// \brief        오브젝트 거리로 판단
/// \author        ispark
/// \date        2006-11-08 ~ 2006-11-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////        
BOOL CShuttleChild::CheckCollForObjectFrontByDistance(float fElapsedTime, D3DXVECTOR3 vNewPos, D3DXVECTOR3 vOldPos)
{
    FLOG( "CShuttleChild::CheckCollForObjectFrontByDistance(float fElapsedTime, D3DXVECTOR3 vNewPos, D3DXVECTOR3 vOldPos)" );
    // 2008-10-16 by bhsohn A기어 충돌 버그 수정
    if((m_dwState == _LANDING) && (IS_DT(m_myShuttleInfo.UnitKind))    )
    {
        // A기어 착륙중에는 거리에 따른 랜딩 체크안함.
        return FALSE;
    }
    // end 2008-10-16 by bhsohn A기어 충돌 버그 수정
    
        
    COLLISION_RESULT collResult;
    
    D3DXMATRIX    matColl;
    D3DXVECTOR3 vSide,vVel,vParticlePos;

    D3DXVECTOR3 vDistVel    = vNewPos - vOldPos;                                        // 방향
    float        fDist        = D3DXVec3Length(&vDistVel);                                // 거리
    
    D3DXVec3Normalize(&vDistVel, &vDistVel);
    D3DXMatrixLookAtLH(&matColl,&vOldPos,&(vOldPos + vDistVel),&m_vUp);                    // 앞

    // 충돌 검사
    if(    /*m_bCollObjectCheck && */
        g_pScene &&
        g_pScene->m_byMapType != MAP_TYPE_TUTORIAL &&
        g_pScene->m_pObjectRender)
    {
        collResult = g_pScene->m_pObjectRender->CheckCollMesh(matColl, vOldPos);
        
        vVel = collResult.vNormalVector;
        D3DXVECTOR3 vNewVel;
        D3DXVECTOR3 vTempDistVel = -1.0f * vDistVel;
        float fDot1 = D3DXVec3Dot(&vTempDistVel,&vVel);
//        DBGOUT("전 %f %f %f, 현 %f %f %f\n", vOldPos.x, vOldPos.y, vOldPos.z, vNewPos, vNewPos.x, vNewPos.y, vNewPos.z);

        // 거리 비교, 오브젝트 노멀 방향과 관계 없다.
        // 내가 움직인 거리 사이에 오브젝트가 있다면 충돌 처리
        // 2007-06-12 by bhsohn A기어 마을에 착륙시 데미지 들어가는 현상 처리
        //if(collResult.fDist < fDist)
        if(collResult.fDist < fDist && (FALSE == g_pD3dApp->IsMyShuttleCharacter()))
        {
//            DBGOUT("원본 거리 = %f, 충돌 거리 = %f\n", fDist, collResult.fDist);
            if(m_myShuttleInfo.Level > 5 && m_fCollSendDamageCheckTime < 0.0f && m_fShuttleSpeed != 0)
            {
                SendPacket(T_FC_CHARACTER_APPLY_COLLISION_DAMAGE, COLLISION_TYPE_BUILDING);
                m_fCollSendDamageCheckTime = 1.0f;
                
                // 2008-11-13 by bhsohn 조이스틱 작업
                // 지형충돌
                g_pD3dApp->SetJoyStickFeedback(TRUE, JOY_FEED_COLLISION_DAMAGE);
                // end 2008-11-13 by bhsohn 조이스틱 작업
            }
            m_fCheckCollTime = 0.0f;

            if(m_bIsAir == TRUE)
            {
                m_fShuttleSpeed = m_fRealShuttleSpeedMin>60 ? m_fRealShuttleSpeedMin : 60;
            }
            else
            {
                m_fShuttleSpeed = 20.0f;
            }

            g_pCamera->ChangeCamCollType();
            g_pCamera->SetSmoothMove();
//            DBGOUT("충돌--------------> %f %f %f\n", vVel.x, vVel.y, vVel.z);
            m_vPos = m_vOldPos + 0.5f*m_fShuttleSpeed*vVel*fElapsedTime;
            // 2006-08-08 by ispark, 맵 끝에서 충돌
            if(m_vPos.y > g_pScene->m_nMaxAtitudeHeight + g_pGround->m_projectInfo.fWaterHeight)
            {
                vNewVel.x = g_pGround->m_projectInfo.sXSize*TILE_SIZE/2.0f+RAND100();
                vNewVel.y = 0.0;
                vNewVel.z = g_pGround->m_projectInfo.sYSize*TILE_SIZE/2.0f+RAND100();
            }
            else
            {
                // 2006-05-06 by ispark, 충돌 반사 코드 변경
                vNewVel = (2 * fDot1) * vVel + m_vVel;
            }

            D3DXVec3Normalize(&m_vVel,&vNewVel);
            D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
            D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
            vParticlePos = m_vPos + 8.0f*m_vVel;
            
            if(collResult.nCollType == COLL_WHOLE)
            {
                // 이곳으로 들어온다면 나중에 뭔가 처리해 주자.
                DBGOUT("Whole Collision \n");
            }
            if(g_pD3dApp->m_pEffectList)
            {
                CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
            }
            SetCursorInit();

            if(m_pClientParty->IsFormationFlightMove())
            {
                SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
                m_pClientParty->FormationFlightClear();
                g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_PARTY_0001,COLOR_SYSTEM);
            }

            // 2005-11-17 by ispark 페널티 상황 종료
            if(m_bPenaltyUnitStop == TRUE)
            {
                InitUnitStop();
            }

            return TRUE;
        }
    }
    
    return FALSE;
}

void CShuttleChild::SetShuttleFlightInit(float fElapsedTime,bool bType)                            // 셔틀의 평형마추기
{
    FLOG( "CShuttleChild::SetShuttleFlightInit(float fElapsedTime,bool bType)" );
    D3DXVECTOR3 vVel,v,v2;
    D3DXMATRIX m;
    // 충돌 있을시에 유닛고정없음
    // 2010. 05. 27 by jskim 시네마 적용 카메라 구현
    //if(m_bCollMap)
    if(m_bCollMap || IsShowCinema() )
    //end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
        return;

    float fAngle1, fAngle2,fAngleRate;
    fAngleRate = 1.0f;
    if(bType)
    {
        vVel = m_vVel;
        vVel.y = 0.0f;
        if(vVel.x == 0.0f && vVel.z == 0.0f)
        {// 에러
            vVel.x = 1.0f;
        }
        D3DXVec3Normalize(&vVel,&vVel);
        fAngle1 = ACOS(D3DXVec3Dot(&vVel,&m_vVel));
        D3DXVec3Cross(&m_vSideVel,&m_vUp,&m_vVel);
        if(m_vUp.y > 0.0f)
        {
            if(m_vVel.y > 0.0f)
            {
                D3DXMatrixRotationAxis(&m, &m_vSideVel, fAngleRate*fAngle1*fElapsedTime);
                D3DXVec3TransformCoord(&m_vVel, &m_vVel, &m);
                D3DXVec3TransformCoord(&m_vUp, &m_vUp, &m);
                if(m_vVel.y < 0.0f)
                {
                    m_vVel.y = 0.0f;
                }
            }
            else
            {
                D3DXMatrixRotationAxis(&m, &m_vSideVel, -fAngleRate*fAngle1*fElapsedTime);
                D3DXVec3TransformCoord(&m_vVel, &m_vVel, &m);
                D3DXVec3TransformCoord(&m_vUp, &m_vUp, &m);
                if(m_vVel.y > 0.0f)
                {
                    m_vVel.y = 0.0f;
                }
            }
        }
        else
        {
            if(m_vVel.y > 0.0f)
            {
                D3DXMatrixRotationAxis(&m, &m_vSideVel, -fAngleRate*fAngle1*fElapsedTime);
                if(m_vVel.y < 0.0f)
                {
                    m_vVel.y = 0.0f;
                }
            }
            else
            {
                D3DXMatrixRotationAxis(&m, &m_vSideVel, fAngleRate*fAngle1*fElapsedTime);
                if(m_vVel.y > 0.0f)
                {
                    m_vVel.y = 0.0f;
                }
            }
        }
        D3DXVec3TransformCoord(&m_vVel, &m_vVel, &m);
        D3DXVec3TransformCoord(&m_vUp, &m_vUp, &m);
        if(abs(m_vVel.y) < 0.0001f)
        {
            m_vVel.y = 0.0f;
        }
    }
    v = D3DXVECTOR3(0,1,0);
    D3DXVec3Cross(&v2,&m_vUp,&m_vVel);
    fAngle2 = ACOS(D3DXVec3Dot(&v,&m_vUp));
    if(v2.y > 0.0f)
    {
        D3DXMatrixRotationAxis(&m, &m_vVel, -fAngleRate*fAngle2*fElapsedTime);
        D3DXVec3TransformCoord(&m_vUp, &m_vUp, &m);
        D3DXVec3Cross(&v2,&m_vUp,&m_vVel);
        if(v2.y < 0.0f)
            m_vUp = v;
    }
    else
    {
        D3DXMatrixRotationAxis(&m, &m_vVel, fAngleRate*fAngle2*fElapsedTime);
        D3DXVec3TransformCoord(&m_vUp, &m_vUp, &m);
        D3DXVec3Cross(&v2,&m_vUp,&m_vVel);
        if(v2.y > 0.0f)
            m_vUp = v;
    }
    
    if(bType)
    {
        if(m_fSideRate > 0.0f)
        {
            m_fSideRate -= 3.0f*fElapsedTime;
            if(m_fSideRate < 0.0f)
                m_fSideRate = 0.0f;
        }
        else if(m_fSideRate < 0.0f)
        {
            m_fSideRate += 3.0f*fElapsedTime;
            if(m_fSideRate > 0.0f)
                m_fSideRate = 0.0f;
        }
        if(fAngle1 < 0.01f && fAngle2 < 0.01f && abs(m_fSideRate) < 0.01f)
            m_bIsSetFlighting = FALSE;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::ChangeUnitStateDead()
/// \brief        죽었을 때 딱한번 처리
/// \author        dhkwon
/// \date        2004-06-30 ~ 2004-06-30
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::ChangeUnitStateDead()
{
    // 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현
    if( g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ) )
    {
        g_pInterface->m_pINFOpMain->GetOpInfo()->SetOperatorAction(1,4);
    }
    // end 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 구현
    // 2009. 09. 24 by jskim 필드 창고 캐쉬 아이템
    g_pInterface->m_pCityBase->CloseCurrentEnterBuilding();
    //end 2009. 09. 24 by jskim 필드 창고 캐쉬 아이템

    // 2007-04-05 by bhsohn 빅붐 버그 처리
    // 죽었을시 과거 스킬초기화
    m_pSkill->InitPreSkillItemNum();

    // 2008-11-04 by bhsohn 죽었을시, 타켓형 스킬 해제
    m_pSkill->SetSkillTargetState(FALSE);        // 스킬 마우스 포인터 해제

    // 2009-02-19 by bhsohn 거래중 유닛 죽었을때에 대한 처리
    if(g_pGameMain->m_pTrade)
    {
        if(g_pGameMain->m_pTrade->m_bTrading)
        {
            // 거래중이면 취소를 보낸다.
            g_pGameMain->m_pTrade->CancelTrade();
            if(g_pD3dApp->m_pChat)
            {
                g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_TRADE_0015,COLOR_TRADE);//"거래가 취소 되었습니다."
            }
        }
    }
    // end 2009-02-19 by bhsohn 거래중 유닛 죽었을때에 대한 처리

    // 2007-10-16 by bhsohn 거래 상에 죽었을시, 키 안먹는 문제처리
    // 거래중이라면 거래도 푼다.
    g_pGameMain->SetTradeInfo(FALSE);

    m_fShuttleSpeed = 0;
    m_pTarget = NULL;
    m_pOrderTarget = NULL;
    m_mMatrix._32 = 0;
    m_mMatrix._21 = 0;
    m_mMatrix._22 = 1;
    m_mMatrix._23 = 0;
    // 2005-08-01 by ispark
    // 카메라 패턴 사용 안함
    m_bIsCameraPattern = FALSE;
    // Permanent를 제외한 사용중인 모든 스킬 해제
    // 2008-12-29 by bhsohn 사망 시 버프 스킬 유지 시스템
    //m_pSkill->ReleaseAllUsingSkillWithOutPermanent();
    m_pSkill->ReleaseStatDEAD_AllUsingSkillWithOutPermanent();


    // 2008. 12. 16 by ckPark 아이템 사용 지연
    // 죽었을 시 딜레이 아이템 초기화
    g_pGameMain->ReleaseDelayItem();
    // end 2008. 12. 16 by ckPark 아이템 사용 지연


    // 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
    // 죽었을 시 딜레이 스킬 초기화
    g_pGameMain->ReleaseDelaySkill();
    // end 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연


    // 2005-08-22 by ispark
    // 연료가 0이었다면 FALLEN으로. FALLING 처리는 이미 했음
    if( InitCinemaUnit(PATTERN_UNIT_FALLING, TRUE) == FALSE || m_dwState == _FUELALLIN)
     {
        // 2005-03-28 by jschoi - 죽었을 때 부활 카드 사용
        CItemInfo* pItemInfo = g_pStoreData->SerchRevivalUseItem();
        MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
// 2005-12-05 by ispark
//        if(g_pD3dApp->m_bDeadByP2PPK == TRUE)// 1:1결투하다가 적에게 죽은 경우
        // 2006-01-02 by ispark, 다른 세력맵인지 판단 
        // 2006-04-10 by ispark, 부활 카드 수정, 1:1 결투는 제외
        // 2006-11-17 by ispark, 이벤트 맵에서는 제외
        if(g_pD3dApp->m_bDeadByP2PPK == FALSE && 
            pItemInfo &&
            IsRevivalUseItem())
        {
            char strRevivalMsg[512] = {0,};
            // 2009-04-21 by bhsohn 아이템 DesParam추가
            //sprintf(strRevivalMsg, STRMSG_C_060410_0003, pItemInfo->ItemInfo->ItemName, pItemInfo->ItemInfo->ParameterValue1);    // "%s를 사용하시겠습니까? 확률은 %.0f입니다."
            sprintf(strRevivalMsg, STRMSG_C_060410_0003, pItemInfo->ItemInfo->ItemName, pItemInfo->ItemInfo->ArrParameterValue[0]);    // "%s를 사용하시겠습니까? 확률은 %.0f입니다."
            g_pGameMain->m_pInfWindow->AddMsgBox(strRevivalMsg, _Q_USE_NORMAL_RESTORE,0,0,0,pItemInfo->UniqueNumber);
        }
        else
        {
            // 2006-11-17 by ispark, 부활 카드가 있지만 이벤트 맵이라면
            if(pItemInfo && IS_MAP_INFLUENCE_EVENT_AREA(pMapInfo->MapInfluenceType))
            {
                g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061117_0000, COLOR_ERROR);// "이벤트맵에서는 %s 아이템을 사용할 수 없습니다."
            }

            SendDeadMsg();
        }

//        else if(pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(ITEM_NUM_CASH_CASH_NORMAL_RESTORE))
//        {
//            g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_CARD_0004, _Q_USE_NORMAL_RESTORE,0,0,0,pItemInfo->UniqueNumber);//"일반 부활 카드를 사용하시겠습니까?"
//        }
//        else if(pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(ITEM_NUM_CASH_CASH_SUPER_RESTORE))
//        {
//            g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_CARD_0005, _Q_USE_SUPER_RESTORE,0,0,0,pItemInfo->UniqueNumber);//"슈퍼 부활 카드를 사용하시겠습니까?"
//        }
//        else
//        {// 부활 카드가 없다면
//            g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_CARD_0003, _Q_DEAD_RETURN);//"격납고로 이동합니다."
//        }

        ChangeUnitState( _FALLEN );
        g_fSendMovePacket = max(g_fSendMovePacket,5.0f);

        ResetBodyCondition(BODYCON_EXPLOSION_MASK);
    }
    else
    {
        m_bSkillMoveIsUse = FALSE;            // 2006-01-10 by ispark, 스킬 무브를 쓰지 않는다.(_FALLING에 있음)
        RemoveEnemyTarget();                // 2006-01-13 by ispark, 적 타겟 삭제
        ChangeUnitState( _FALLING );
        g_pGameMain->InitShowWindow();        // 2006-08-07 by ispark, 창 초기화

        // 2008-11-13 by bhsohn 조이스틱 작업
        // 격추 진동
        g_pD3dApp->SetJoyStickFeedback(TRUE, JOY_FEED_SHUTTLE_EXPLODED);
        // end 2008-11-13 by bhsohn 조이스틱 작업
    }

    // 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
    if( m_bDeadMessageEnable )
        m_dwDeadTime = timeGetTime();
    // end 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
}

void CShuttleChild::ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition)
{
    //캐릭터가 죽은 경우
    if(    COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_DEAD_MASK) )
    {
          if( !COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_EXPLOSION_MASK) )
        {
//            m_myShuttleInfo.CurrentHP = 0.0f;
//            m_pOrderTarget = NULL;
//            ChangeUnitStateDead();

            ChangeSingleBodyCondition(BODYCON_EXPLOSION_MASK);
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE1_MASK, FALSE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE2_MASK, FALSE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE3_MASK, TRUE );
            /////////////////////////////////////////////////////////////////
//            m_vNextPos = m_vPos - 70.0f*m_vVel + 100.0f*m_vUp;
            /////////////////////////////////////////////////////////////////
            m_fCurrentTime = 186.0f;
            // 이캐릭을 타겟으로 셋팅하고 있는 것들을 타겟초기화 시킨다.
//            CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
//            while(itEnemy != g_pScene->m_mapEnemyList.end())
//            {
//                itEnemy->second->m_bAttackEnemy = FALSE;
//                itEnemy->second->SetPkState( PK_ALL, FALSE );
//                SetAttackAvaliableEnemyIndex( itEnemy->second->m_infoCharacter.ClientIndex, FALSE );
//                itEnemy->second->m_nEnemyTypePrimary = ENEMYDATA_ENEMYLIST;
//                itEnemy->second->m_nEnemyTypeSecondary = ENEMYDATA_ENEMYLIST;
//                itEnemy++;
//            }
//            m_vecAttackAvailableEnemyIndex.clear();
//            CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.begin();
//            while(itMonster != g_pScene->m_mapMonsterList.end())
//            {
//                itMonster->second->m_nMonsterTypePrimary = MONSTERDATA_MONSTERLIST;
//                itMonster->second->m_nMonsterTypeSecondary = MONSTERDATA_MONSTERLIST;
//                itMonster++;
//            }
            // 2006-01-18 by ispark, 지금 내상태 검사, 폭발이 맞는지...
            if(!UNIT_STATE_DEAD(m_dwState))
            {
                if(m_fCheckDead == 0.0f)
                    m_fCheckDead = 1.0f;
//                DBGOUT("My State[%d] --> Dead BodyCondition Check On\n", m_dwState);
            }
        }

        return;
    }
    else
    {
        // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
        
        // 롤링시 부스터 애니메이션이 덮어씌워지므로 막는다(체인롤링시나 롤링시 렉문제)
        if( COMPARE_BODYCON_BIT(hyBodyCondition, BODYCON_BOOSTER1_MASK | BODYCON_BOOSTER2_MASK | BODYCON_BOOSTER3_MASK | BODYCON_BOOSTER4_MASK | BODYCON_BOOSTER5_MASK) )
        {
            if( IsRobotArmor() && (m_bKeyDownA || m_bKeyDownD) )
                return;
        }

        // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
        
        // 부스터가 켜진 경우
        if(    COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_BOOSTER3_MASK) )
        {
            if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER3_MASK))
            {
                // 2008-01-16 by bhsohn A기어 워프 실패시 위장,그라운드 엑셀 취소 시킴
                //m_bIsAir = TRUE;
                SetShuttleAirMode(TRUE);
                // end 2008-01-16 by bhsohn A기어 워프 실패시 위장,그라운드 엑셀 취소 시킴

                if(COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_LANDED_MASK))
                {
                    m_nBooster = BOOSTER_TYPE_MIN;
                    ChangeSingleBodyCondition(BODYCON_BOOSTER1_MASK);
                }
                else
                {
                    m_nBooster = BOOSTER_TYPE_MAX;
                    ChangeSingleBodyCondition(BODYCON_BOOSTER3_MASK);
                    // mouse rate 조절
                    ITEM* pItemInfo = g_pStoreData->GetItemInfoFromItemGeneral(g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR ));
                    if(pItemInfo)
                    {
//                        m_fMouseRate = pItemInfo->BoosterAngle;
                        m_fMouseRate = CAtumSJ::GetEngineBoosterRangeAngle(pItemInfo, &m_paramFactor);
                    }
                    // 스크린 이펙트
                    if(g_pD3dApp->m_pEffectList)
                    {
                        CAppEffectData* pEffect = g_pScene->FindEffect( RC_EFF_BOOSTER );
                        if(pEffect == NULL)
                        {
                            pEffect = new CAppEffectData(g_pCamera,RC_EFF_BOOSTER,100);
                            g_pD3dApp->m_pEffectList->AddChild(pEffect);
                        }
                    }
                    if(IS_BT(g_pShuttleChild->m_myShuttleInfo.UnitKind) || IS_ST(g_pShuttleChild->m_myShuttleInfo.UnitKind))
                    {
//                        g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_B_I_GEAR );
                        g_pD3dApp->m_pSound->PlayD3DSound( SOUND_HIGH_BOOSTER_START, m_vPos, FALSE);
                        g_pD3dApp->m_pSound->PlayD3DSound( SOUND_HIGH_BOOSTER, m_vPos, FALSE);
                    }
                    else
                    {
//                        g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_M_A_GEAR );
                        g_pD3dApp->m_pSound->PlayD3DSound( SOUND_LOW_BOOSTER_START, m_vPos, FALSE);
                        g_pD3dApp->m_pSound->PlayD3DSound( SOUND_LOW_BOOSTER, m_vPos, FALSE);
                    }
                
                }
            }
            return;
        }
        // 부스터가 꺼진 경우
        else if( COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER3_MASK) && 
            !COMPARE_BODYCON_BIT(hyBodyCondition,BODYCON_BOOSTER3_MASK))
        {
            ChangeSingleBodyCondition(BODYCON_BOOSTER4_MASK);
//            DBGOUT("CShuttleChild::ChangeBodyConditionFromServer(BODYCON_BOOSTER4_MASK)\n");
            return;
        }
        else
        {
            ChangeUnitBodyCondition(hyBodyCondition);
//            DBGOUT("CShuttleChild::ChangeBodyConditionFromServer(%016I64X)\n",hyBodyCondition);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::CheckBodyConditionToServer(BodyCond_t hyBodyCondition)
/// \brief        바디컨디션을 분류후 하나씩서버로 보낸다.
///                필요시에는 하나씩 추가 바람
/// \author        ispark
/// \date        2005-07-28 ~ 2005-07-28
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckBodyConditionToServer(BodyCond_t hyBodyCondition)
{
    if(COMPARE_BODYCON_BIT(hyBodyCondition, BODYCON_DAMAGE1_MASK))
    {
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE1_MASK, TRUE );
    }
    else
    {
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE1_MASK, FALSE );

    }
    if(COMPARE_BODYCON_BIT(hyBodyCondition, BODYCON_DAMAGE2_MASK))
    {
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE2_MASK, TRUE );
    }    
    else
    {
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE2_MASK, FALSE );

    }
    if(COMPARE_BODYCON_BIT(hyBodyCondition, BODYCON_DAMAGE3_MASK))
    {
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE3_MASK, TRUE );
    }
    else
    {
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE3_MASK, FALSE );

    }
}



// 유닛의 바디컨디션이 유지될때, 애니메이션 시간을 가져옴
#define BODYCON_CLIENT_KEEPING_ANIMATION_MASK        (BODYCON_BOOSTER_EX_STATE_CLEAR_MASK | BODYCON_LANDING_MASK | BODYCON_LANDED_MASK | BODYCON_TAKEOFF_MASK)
void CShuttleChild::CheckAniTime()
{
    FLOG( "CShuttleChild::CheckAniTime()" );

    m_fCurrentTime = GetCurrentAnimationTime();
//    sprintf( g_pD3dApp->m_strDebug, "0x%016I64x %.3f",GetCurrentBodyCondition(), m_fCurrentTime );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::SendFieldSocketChangeBodyCondition(ClientIndex_t nClientIndex, BodyCond_t hyBody)
/// \brief        MSG_FC_CHARACTER_CHANGE_BODYCONDITION
/// \author        dhkwon
/// \date        2004-03-28 ~ 2004-03-28
/// \warning    APP에도 같은 기능을 하는 함수가 있다.참고할것.
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SendFieldSocketChangeBodyCondition(ClientIndex_t nClientIndex, BodyCond_t hyBody)
{
    FLOG("CShuttleChild::SendFieldSocketChangeBodyCondition(ClientIndex_t nClientIndex, BodyCond_t hyBody)");
//    DBGOUT("CShuttleChild::SendFieldSocketChangeBodyCondition(%016I64X)\n",hyBody);
    MSG_FC_CHARACTER_CHANGE_BODYCONDITION sMsg;
    sMsg.ClientIndex = nClientIndex;
    sMsg.BodyCondition = hyBody;
    g_pD3dApp->m_pFieldWinSocket->SendMsg( T_FC_CHARACTER_CHANGE_BODYCONDITION, (char*)&sMsg, sizeof(sMsg));
}
void CShuttleChild::CheckBoosterAni()
{
    FLOG( "CShuttleChild::CheckBoosterAni()" );
    switch(m_nBooster)
    {
    case BOOSTER_TYPE_AIRBREAK:// 역추진 부스터
        {
            if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER5_MASK))//역추진
            {
                SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, 
                        BODYCON_BOOSTER5_MASK | BODYCON_SET_OR_CLEAR_MASK);
                //SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_BOOSTER5_MASK);//역추진
                ChangeSingleBodyCondition(BODYCON_BOOSTER5_MASK);
            }
//            m_fAttackDelayCheckTime = 1.0f;
        }
        break;
    case BOOSTER_TYPE_STOP://중지
        {
            if(COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER3_MASK))//고속 부스터
            {
                //SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_BOOSTER4_MASK);//고속을 끌때 
                SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, 
                        BODYCON_BOOSTER4_MASK | BODYCON_SET_OR_CLEAR_MASK);
                ChangeSingleBodyCondition(BODYCON_BOOSTER4_MASK);
                // 2006-12-11 by dgwoo S키 누른 상태와 동시에 부스터사용시 작동안됨
                 //m_bUseBooster = FALSE;
            }
            else
            {
                if(COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER4_MASK))//고속을 끌때
                {
                    if(m_pCharacterInfo)
                    {
                        map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_pCharacterInfo->m_mapBodyCondition.find(BODYCON_BOOSTER4_MASK);
                        if(it != m_pCharacterInfo->m_mapBodyCondition.end())
                        {
                            if((it->second)->m_fCurrentAnimationTime >= (it->second)->m_fEndAnimationTime)
                            {
                                SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, 
                                        BODYCON_BOOSTER1_MASK | BODYCON_SET_OR_CLEAR_MASK);
                                //SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_BOOSTER1_MASK);//키를 안누르고 비행(평균속도 이하, 속도증가키 안누를때)
                                ChangeSingleBodyCondition(BODYCON_BOOSTER1_MASK);
                            }
                        }
                    }
                }
                else
                {
                    if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER1_MASK))//키를 안누르고 비행(평균속도 이하, 속도증가키 안누를때)
                    {
//                        DBGOUT("CheckBoosterAni(%016I64X) SendFieldSocketChangeBodyCondition(BOOSTER_TYPE_STOP)\n",GetCurrentBodyCondition());
                        SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, 
                                BODYCON_BOOSTER1_MASK | BODYCON_SET_OR_CLEAR_MASK);
                        //SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_BOOSTER1_MASK);
                        ChangeSingleBodyCondition(BODYCON_BOOSTER1_MASK);
//                        DBGOUT("CheckBoosterAni(%016I64X) SendFieldSocketChangeBodyCondition(BOOSTER_TYPE_STOP)\n",GetCurrentBodyCondition());
                    }
                }
            }
        }
        break;
    case BOOSTER_TYPE_MIN://기본부스터 
        {
            if(COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER3_MASK))//고속 부스터
            {
                //SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_BOOSTER4_MASK);//고속을 끌때
                ChangeSingleBodyCondition(BODYCON_BOOSTER4_MASK);
            }
            else
            {
                if(COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER4_MASK))//고속을 끌때
                {
                    if(m_pCharacterInfo)
                    {
                        map<BodyCond_t,CBodyConditionInfo*>::iterator it = m_pCharacterInfo->m_mapBodyCondition.find(BODYCON_BOOSTER4_MASK);
                        if(it != m_pCharacterInfo->m_mapBodyCondition.end())
                        {
                            if((it->second)->m_fCurrentAnimationTime >= (it->second)->m_fEndAnimationTime)
                            {
                                SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, 
                                        BODYCON_BOOSTER2_MASK | BODYCON_SET_OR_CLEAR_MASK);
                                //SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_BOOSTER2_MASK);
                                ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
                            }
                        }
                    }
                }
                else
                {
                    if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER2_MASK))// 전진키 누르는 상태(평균속도 이상, 속도증가키 누를때)
                    {
                        SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, 
                                BODYCON_BOOSTER2_MASK | BODYCON_SET_OR_CLEAR_MASK);
                        //SET_BODYCON_BIT(m_myShuttleInfo.BodyCondition,BODYCON_BOOSTER2_MASK);
                        ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
                    }
                }
            }
        }
        break;
    case BOOSTER_TYPE_MAX://가속부스터
        {
            if(!COMPARE_BODYCON_BIT(GetCurrentBodyCondition(),BODYCON_BOOSTER3_MASK))//고속 부스터
            {
                m_nBooster = BOOSTER_TYPE_MIN;
                if(m_fChangeBodyForBoosterCheckTime < 0.0f)
                {
                    m_fChangeBodyForBoosterCheckTime = 0.2f;
                    // 서버 전송후 OK 를 받아야지만 부스터 상태가 켜지게 된다. 현재는 부스터 사용전이다.
                    SendFieldSocketChangeBodyCondition(m_myShuttleInfo.ClientIndex, 
                            BODYCON_BOOSTER3_MASK | BODYCON_SET_OR_CLEAR_MASK);
                }
            }
        }
        break;
    }
}

void CShuttleChild::CheckMouseDir()
{
    FLOG( "CShuttleChild::CheckMouseDir()" );

    GetCursorPos(&m_pt);
    ScreenToClient(g_pD3dApp->GetHwnd(), &m_pt);
    CheckMouseReverse(&m_pt);

    D3DXVECTOR3 v;
    D3DXMATRIX matProj,matView;
    matProj = g_pD3dApp->m_pCamera->GetProjMatrix();

    // Compute the vector of the pick ray in screen space
    v.x =  (((2.0f * m_pt.x) / g_pD3dApp->m_d3dsdBackBuffer.Width) - 1) / matProj._11;
    v.y = -(((2.0f * m_pt.y) / g_pD3dApp->m_d3dsdBackBuffer.Height) - 1) / matProj._22;
    v.z =  1.0f;

    // Get the inverse view matrix
    D3DXMATRIX  matTemp;
    matView = g_pD3dApp->m_pCamera->GetViewMatrix();
    D3DXMatrixInverse(&matTemp, NULL, &matView);

    // Transform the screen space pick ray into 3D space
    m_vMouseDir.x  = v.x*matTemp._11 + v.y*matTemp._21 + v.z*matTemp._31;
    m_vMouseDir.y  = v.x*matTemp._12 + v.y*matTemp._22 + v.z*matTemp._32;
    m_vMouseDir.z  = v.x*matTemp._13 + v.y*matTemp._23 + v.z*matTemp._33;
    D3DXVec3Normalize(&m_vMouseDir,&m_vMouseDir);
    if(m_bTurnCamera)
        m_vMouseDir *= -1.0f;

    // 2010-06-15 by shcho&hslee 펫시스템 - 마우스 위치
    m_vMousePos = D3DXVECTOR3(matTemp._41, matTemp._42, matTemp._43);
    // end 2010-06-15 by shcho&hslee 펫시스템 - 마우스 위치
    // 2005-09-05 by ispark
    // 마우스 방향
    int iCenterX = g_pD3dApp->GetBackBufferDesc().Width / 2;
    int iCenterY = g_pD3dApp->GetBackBufferDesc().Height / 2;
    
    // 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)

//     m_ptDir.x = m_pt.x - iCenterX;
//     m_ptDir.y = m_pt.y - iCenterY;
    if( !m_pSkill->IsExistDesParamSKill( DES_SKILL_REVERSECONTROL ) )
    {
        m_ptDir.x = m_pt.x - iCenterX;
        m_ptDir.y = m_pt.y - iCenterY;
    }
    else
    {
        m_ptDir.x = -(m_pt.x - iCenterX);
        m_ptDir.y = -(m_pt.y - iCenterY);
    }

    // end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
}

void CShuttleChild::CheckTarget()
{
    // 2005-08-16 by ispark
    // 레이더 미장착시 타겟 검사 안한다.
    if(m_pRadarItemInfo == NULL || IsObserverMode())
    {
        return;
    }

    if(g_pD3dApp->m_dwTargetting == NEW_TARGETTING)
    {
        NewCheckTarget();
    }
    else if(g_pD3dApp->m_dwTargetting == OLD2_TARGETTING)
    {
        Old2CheckTarget();
    }
    else
    {// OLD_TARGETTING
        OldCheckTarget();
    }
}

void CShuttleChild::OldCheckTarget()
{
    // 2005-03-24 by jschoi - 기존의 타겟팅 방식
    FLOG( "CShuttleChild::CheckTarget()" );
    if(m_pSkill->GetSkillTargetState() == TRUE)
    {
        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_11);
        return;
    }

    D3DXVECTOR2 v1, v2, v3;
    v1 = D3DXVECTOR2(m_pt.x,m_pt.y);
    v3 = D3DXVECTOR2(g_pD3dApp->GetBackBufferDesc().Width/2,g_pD3dApp->GetBackBufferDesc().Height/2);

    // 2006-02-15 by ispark
    if(//g_pGameMain->IsShowing() ||                    // 2006-11-07 by ispark
        g_pGameMain->m_bMenuLock || 
        g_pGameMain->m_bChangeMousePoint == TRUE || 
        g_pD3dApp->m_dwGameState == _SHOP || 
        m_bUnitStop == TRUE ||
        m_dwState == _LANDED)
    {
        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_0);
    }
    else if(m_pPrimaryWeapon)
    {
        float fLength;//, fLength2;

        GetCursorPos(&m_pt);
        ScreenToClient(g_pD3dApp->GetHwnd(), &m_pt);
        CheckMouseReverse(&m_pt);

        m_pTarget = NULL;

        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        //vector<INT>::iterator it = m_pPrimaryWeapon->m_vecTargetIndex.begin();
        //while(it != m_pPrimaryWeapon->m_vecTargetIndex.end())
        vector<TARGET_DATA>::iterator it = m_pPrimaryWeapon->m_vecTargetIndexData.begin();
        while(it != m_pPrimaryWeapon->m_vecTargetIndexData.end())
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        {
            // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            //CUnitData* pUnit = g_pScene->FindUnitDataByClientIndex( (*it) );
            CUnitData* pUnit = g_pScene->FindUnitDataByClientIndex( (*it).nTargetIndex );
            // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            ASSERT_ASSERT(pUnit);
            if(pUnit)
            {
                v2 = D3DXVECTOR2(pUnit->m_nObjScreenX,pUnit->m_nObjScreenY);
                if(IS_DT(m_myShuttleInfo.UnitKind))    // A기어인 경우
                {
                    if( D3DXVec2Length(&(v1 - v2)) < SHUTTLE_TARGET_MOUSE_CIRCLE_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800) )
                    {
                        m_pTarget = pUnit;
//                        if(m_bLButtonState)
//                        {// 공격중(클릭됨)
//                            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_4);
//                        }
//                        else
//                        {// 공격하지 않음(노 클릭)
//                            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_2);
//                        }
//                        break;
                    }
                    else
                    {
//                        if(m_bLButtonState)
//                        {// 공격중(클릭됨)
//                            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_5);
//                        }
//                        else
//                        {// 공격하지 않음(노 클릭)
//                            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_3);
//                        }
//                        break;
                    }
                }
                else if(D3DXVec2Length(&(v1 - v3)) < SHUTTLE_ATTACK_CENTER_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800))// 마우스 포인터가 화망 안에 있는 경우
                {
                    fLength = D3DXVec2Length(&(v1-v2));
                    if( fLength < SHUTTLE_TARGET_MOUSE_CIRCLE_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800))
                    {
                        m_pTarget = pUnit;
                        if(m_bLButtonState)
                        {// 공격중(클릭됨)
                            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_4);
                        }
                        else
                        {// 공격하지 않음(노 클릭)
                            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_2);
                        }
                        break;
                    }
                    else
                    {
                        if(m_bLButtonState)
                        {// 공격중(클릭됨)
                            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_5);
                        }
                        else
                        {// 공격하지 않음(노 클릭)
                            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_3);
                        }
                        break;
                    }
                }
                else
                {
                    g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_1);
                    if(m_pOrderTarget == NULL || m_pOrderTarget == pUnit)
                    {
                        m_pTarget = pUnit;
                        break;
                    }
                    
                }
            }
            it++;
        } 
        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        //if(it == m_pPrimaryWeapon->m_vecTargetIndex.end())
        if(it == m_pPrimaryWeapon->m_vecTargetIndexData.end())
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        {
            if( IS_DT(m_myShuttleInfo.UnitKind) ||    // A기어인 경우
                D3DXVec2Length(&(v1 - v3)) < SHUTTLE_ATTACK_CENTER_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800))// 마우스 포인터가 화망 안에 있는 경우
            {
                if(m_bLButtonState)
                {// 공격중(클릭됨)
                    g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_5);
                }
                else
                {// 공격하지 않음(노 클릭)
                    g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_3);
                }
            }
            else
            {
                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_1);
            }
        }
        if( m_pTarget && m_pOrderTarget == NULL )
        {
            // 2005-02-15 by jschoi - 오더 타겟을 잡는 부분
            m_pOrderTarget = m_pTarget;

            // 2005-10-19 by ispark
            // Enemy 타겟에 있는지 검사
            vector<PK_TARGET *>::iterator itEnemy = m_vecPKTarget.begin();
            while(itEnemy != m_vecPKTarget.end())
            {
                CEnemyData* pTarget = (CEnemyData*)g_pScene->FindUnitDataByClientIndex( (*itEnemy)->nTargetIndex );
                if(m_pOrderTarget == pTarget)
                {
                    // 있음
                    return;
                }
                itEnemy++;
            }

            if(    m_pTarget->m_dwPartType == _ENEMY &&                        // 1. 타겟이 Enemy이고,
//                ((CEnemyData*)m_pTarget)->GetPkState() == PK_NORMAL &&        // 2. 순수 강제 타겟이면 // 2006-01-07 by ispark, 삭제
                g_pShuttleChild->m_pPkNormalTimer->IsCityWar() == FALSE)    // 3. 도시 점령전중에는 보내지 않는다.
            {
                // 서버로 보낸다.
                MSG_FC_MOVE_LOCKON sMsg;
                sMsg.AttackIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
                sMsg.TargetIndex = ((CEnemyData*)m_pTarget)->m_infoCharacter.CharacterInfo.ClientIndex;    
                g_pFieldWinSocket->SendMsg( T_FC_MOVE_LOCKON, (char*)&sMsg, sizeof(sMsg) );                
            }
            g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LOCKON_PRIMARY, g_pShuttleChild->m_vPos, FALSE);
        }
    }
    else
    {
        // 1형 무기를 장착하지 않은 경우
        if( IS_DT(m_myShuttleInfo.UnitKind) ||    // A기어인 경우
            D3DXVec2Length(&(v1 - v3)) < SHUTTLE_ATTACK_CENTER_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800))// 마우스 포인터가 화망 안에 있는 경우
        {
            if(m_bLButtonState)
            {// 공격중(클릭됨)
                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_5);
            }
            else
            {// 공격하지 않음(노 클릭)
                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_3);
            }
        }
        else
        {
            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_1);
        }
    }
}

void CShuttleChild::Old2CheckTarget()
{
    // 2005-03-29 by jschoi - 기존 타겟 방식 변경
    FLOG( "CShuttleChild::CheckTarget()" );
    if(m_pSkill->GetSkillTargetState() == TRUE)
    {
        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_11);
        return;
    }

    D3DXVECTOR2 v1, v2, v3;
    v1 = D3DXVECTOR2(m_pt.x,m_pt.y);
    v3 = D3DXVECTOR2(g_pD3dApp->GetBackBufferDesc().Width/2,g_pD3dApp->GetBackBufferDesc().Height/2);

    // 2006-02-15 by ispark
    if(//g_pGameMain->IsShowing() ||                        // 2006-11-07 by ispark
        g_pGameMain->m_bMenuLock || 
        g_pGameMain->m_bChangeMousePoint == TRUE || 
        g_pD3dApp->m_dwGameState == _SHOP || 
        m_bUnitStop == TRUE ||
        m_dwState == _LANDED)
    {
        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_0);
    }
    else if(m_pPrimaryWeapon)
    {
        float fLength;//, fLength2;

        GetCursorPos(&m_pt);
        ScreenToClient(g_pD3dApp->GetHwnd(), &m_pt);
        CheckMouseReverse(&m_pt);

        m_pTarget = NULL;

        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
//         vector<int>::iterator it = m_pPrimaryWeapon->m_vecTargetIndex.begin();
//         while(it != m_pPrimaryWeapon->m_vecTargetIndex.end())
        vector<TARGET_DATA>::iterator it = m_pPrimaryWeapon->m_vecTargetIndexData.begin();
        while(it != m_pPrimaryWeapon->m_vecTargetIndexData.end())        
        // end011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        {
            // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            //CUnitData* pUnit = g_pScene->FindUnitDataByClientIndex( *it );
            CUnitData* pUnit = g_pScene->FindUnitDataByClientIndex( (*it).nTargetIndex );
            // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            ASSERT_ASSERT(pUnit);
            #ifdef C_EPSODE4_UI_CHANGE_JSKIM
            // 2011-11-03 by jhahn EP4 중립지역 아이디 색 변경
#ifndef FREEWAR_
            if ((((m_myShuttleInfo.MapChannelIndex.MapIndex - 4000) / 100) == 1) && (pUnit->m_dwPartType == _ENEMY))
            {
                return;
            }
#endif

            //end 2011-11-03 by jhahn EP4 중립지역 아이디 색 변경
            #endif
            if(pUnit)
            {
                v2 = D3DXVECTOR2(pUnit->m_nObjScreenX,pUnit->m_nObjScreenY);
                if(IS_DT(m_myShuttleInfo.UnitKind) && m_bIsAir == FALSE)    // A기어인 경우
                {// 2007-07-27 by dgwoo A기어의 경우 땅에서 따로 처리해준다.

                    if( D3DXVec2Length(&(v1 - v2)) < SHUTTLE_TARGET_MOUSE_CIRCLE_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800) )
                    {
                        m_pTarget = pUnit;
                        float fDistance = D3DXVec3Length(&(m_vPos - m_pTarget->m_vPos));
                        if(    (GetTargetClientIndext() == GetTargetToClientIdx(m_pTarget))
                            && g_pShuttleChild->m_pRadarItemInfo && CAtumSJ::GetPrimaryRadarRange(g_pShuttleChild->m_pRadarItemInfo->ItemInfo, &g_pShuttleChild->m_paramFactor) > fDistance)
                            //&& (CAtumSJ::GetAttackRange(m_pPrimaryWeapon->GetRealItemInfo(), &m_paramFactor ) > fDistance))
                        {
                            if(m_bLButtonState)
                            {
                                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_4);
                            }
                            else
                            {
                                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_2);
                            }
                        }
                        else
                        {
                            if(m_bLButtonState)
                            {
                                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_5);
                            }
                            else
                            {
                                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_3);
                            }
                        }
                        break;
                    }
                    else
                    {
                        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_3);
                    }
                }
                // 2005-03-29 by jschoi - 가운데로 타겟일 때 여기 부터 주석
                else if(D3DXVec2Length(&(v1 - v3)) < SHUTTLE_ATTACK_CENTER_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800))// 마우스 포인터가 화망 안에 있는 경우
                {
                    fLength = D3DXVec2Length(&(v1-v2));
                    if( fLength < SHUTTLE_TARGET_MOUSE_CIRCLE_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800) ||
                        D3DXVec2Length(&(v1 - v3)) < SHUTTLE_TARGET_MOUSE_CIRCLE_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800))
                    {
                         m_pTarget = pUnit;
                        if(m_bLButtonState)
                        {// 공격중(클릭됨) 공격중 1형 타겟 변경
                            // 마우스 업버튼 이면 타겟 채인지
                            // 2006-01-19 by ispark
                            if(TRUE == m_bTargetChange && m_pOrderTarget == NULL)
                            {
                                // 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
                                // BELL_INFINITY_DEFENSE_MONSTER 인 몬스터는 타겟이 안잡히도록 한다
                                // BODYCON_CREATION_MASK 인 몬스터는 타겟이 안잡히도록 한다
                                if( m_pTarget->m_dwPartType == _MONSTER )
                                {
                                    // 2010. 03. 24 by ckPark 리젠중인 몬스터중 10초 생성 애니메이션이 있는 몬스터만 타겟 안잡히도록 변경
//                                     if( ((CMonsterData*)(m_pTarget))->m_pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER
//                                         || COMPARE_BODYCON_BIT( ((CMonsterData*)(m_pTarget))->GetCurrentBodyCondition(), BODYCON_CREATION_MASK ) )
                                    if( ((CMonsterData*)(m_pTarget))->m_pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER
                                        ||
                                        ( COMPARE_MPOPTION_BIT(((CMonsterData*)(m_pTarget))->m_pMonsterInfo->MPOption, MPOPTION_BIT_PATTERN_MONSTER )
                                        && COMPARE_BODYCON_BIT(((CMonsterData*)(m_pTarget))->GetCurrentBodyCondition(), BODYCON_CREATION_MASK) ) )
                                    // end 2010. 03. 24 by ckPark 리젠중인 몬스터중 10초 생성 애니메이션이 있는 몬스터만 타겟 안잡히도록 변경
                                    {
                                        break;
                                    }
                                }
                                // end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


                                m_pOrderTarget = m_pTarget;
                                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_4);
                            }
                        }
                        else
                        {// 공격하지 않음(노 클릭)
                            g_pInterface->m_pTarget->SetMouseType(g_pInterface->m_pTarget->m_nTargetColor);
                        }
                        break;
                    }
                    else
                    {

                        break;
                    }
                }
                else
                {
                    g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_1);
                    if(m_pOrderTarget == NULL || m_pOrderTarget == pUnit)
                    {
                        m_pTarget = pUnit;
                        break;
                    }
                    
                }
            }
            it++;
        }

        // 2005-03-29 by jschoi - 가운데로 타겟일 때 여기 부터 주석
        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        //if(it == m_pPrimaryWeapon->m_vecTargetIndex.end())
        if(it == m_pPrimaryWeapon->m_vecTargetIndexData.end())
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        {
            if( (IS_DT(m_myShuttleInfo.UnitKind) && m_bIsAir == FALSE) ||    // A기어인 경우
                D3DXVec2Length(&(v1 - v3)) < SHUTTLE_ATTACK_CENTER_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800))// 마우스 포인터가 화망 안에 있는 경우
            {
                if(m_bLButtonState)
                {// 공격중(클릭됨)
                    g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_5);
                }
                else
                {// 공격하지 않음(노 클릭)
                    g_pInterface->m_pTarget->SetMouseType(g_pInterface->m_pTarget->m_nTargetColor);
                }
            }
            else
            {
                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_1);
            }
        }

        // 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
        // if( m_pTarget && m_pOrderTarget == NULL )
        BOOL bTargetColl = TRUE;
        if(NULL == m_pOrderTarget )
        {
            bTargetColl = IsCheckObjectColl(m_pTarget);
        }
        
        if( (FALSE == bTargetColl )&& m_pTarget && m_pOrderTarget == NULL )
        {
            // 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
            // BELL_INFINITY_DEFENSE_MONSTER 인 몬스터는 타겟이 안잡히도록 한다
            // BODYCON_CREATION_MASK 인 몬스터는 타겟이 안잡히도록 한다
            if( m_pTarget->m_dwPartType == _MONSTER )
            {
                // 2010. 03. 24 by ckPark 리젠중인 몬스터중 10초 생성 애니메이션이 있는 몬스터만 타겟 안잡히도록 변경
//                 if( ((CMonsterData*)(m_pTarget))->m_pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER
//                     || COMPARE_BODYCON_BIT( ((CMonsterData*)(m_pTarget))->GetCurrentBodyCondition(), BODYCON_CREATION_MASK ) )
                if( ((CMonsterData*)(m_pTarget))->m_pMonsterInfo->Belligerence == BELL_INFINITY_DEFENSE_MONSTER
                    ||
                    ( COMPARE_MPOPTION_BIT(((CMonsterData*)(m_pTarget))->m_pMonsterInfo->MPOption, MPOPTION_BIT_PATTERN_MONSTER )
                    && COMPARE_BODYCON_BIT(((CMonsterData*)(m_pTarget))->GetCurrentBodyCondition(), BODYCON_CREATION_MASK) ) )
                // end 2010. 03. 24 by ckPark 리젠중인 몬스터중 10초 생성 애니메이션이 있는 몬스터만 타겟 안잡히도록 변경
                {
                    return;
                }
            }
            // end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

            // 2005-02-15 by jschoi - 오더 타겟을 잡는 부분
            m_pOrderTarget = m_pTarget;

            // 2005-10-19 by ispark
            // Enemy 타겟에 있는지 검사
            vector<PK_TARGET *>::iterator itEnemy = m_vecPKTarget.begin();
            while(itEnemy != m_vecPKTarget.end())
            {
                CEnemyData* pTarget = (CEnemyData*)g_pScene->FindUnitDataByClientIndex( (*itEnemy)->nTargetIndex );
                if(m_pOrderTarget == pTarget)
                {
                    // 있음
                    return;
                }
                itEnemy++;
            }

            if(    m_pTarget->m_dwPartType == _ENEMY &&                        // 1. 타겟이 Enemy이고,
//                ((CEnemyData*)m_pTarget)->GetPkState() == PK_NORMAL &&        // 2. 순수 강제 타겟이면 // 2006-01-07 by ispark, 삭제
                g_pShuttleChild->m_pPkNormalTimer->IsCityWar() == FALSE)    // 3. 도시 점령전중에는 보내지 않는다.
            {
                // 서버로 보낸다.
                MSG_FC_MOVE_LOCKON sMsg;
                sMsg.AttackIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
                sMsg.TargetIndex = ((CEnemyData*)m_pTarget)->m_infoCharacter.CharacterInfo.ClientIndex;    
                g_pFieldWinSocket->SendMsg( T_FC_MOVE_LOCKON, (char*)&sMsg, sizeof(sMsg) );                
            }
            g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LOCKON_PRIMARY, g_pShuttleChild->m_vPos, FALSE);
        }
    }
    else
    {
        // 1형 무기를 장착하지 않은 경우
        if( (IS_DT(m_myShuttleInfo.UnitKind) && m_bIsAir == FALSE) ||    // A기어인 경우
            D3DXVec2Length(&(v1 - v3)) < SHUTTLE_ATTACK_CENTER_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800))// 마우스 포인터가 화망 안에 있는 경우
        {
            if(m_bLButtonState)
            {// 공격중(클릭됨)
                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_4);
            }
            else
            {// 공격하지 않음(노 클릭)
                g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_3);
            }
        }
        else
        {
            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_1);
        }
    }
}

void CShuttleChild::NewCheckTarget()
{
    // 2005-03-24 by jschoi - 새로운 타겟팅 방식

    // 타겟팅 선택(엠기어 속성스킬)
    if(m_pSkill->GetSkillTargetState() == TRUE)
    {
        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_11);
        return;
    }

    // 인터페이스 모드
    // 2006-02-15 by ispark
    if(//g_pGameMain->IsShowing() ||                            // 2006-11-07 by ispark
        g_pGameMain->m_bMenuLock || 
        g_pGameMain->m_bChangeMousePoint == TRUE || 
        g_pD3dApp->m_dwGameState == _SHOP || 
        m_bUnitStop == TRUE ||
        m_dwState == _LANDED)
    {
        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_0);
        return;
    }

    if(m_pPrimaryWeapon)
    {
        GetCursorPos(&m_pt);
        ScreenToClient(g_pD3dApp->GetHwnd(), &m_pt);
        CheckMouseReverse(&m_pt);

        m_pTarget = NULL;

        if(m_bLButtonState)
        {// 공격중(클릭됨)
            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_5);
        }
        else
        {// 공격하지 않음(노 클릭)
            g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_3);
        }

        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
//         vector<int>::iterator it = m_pPrimaryWeapon->m_vecTargetIndex.begin();
//         DBGOUT(" m_vecTargetIndex : %d\n",m_pPrimaryWeapon->m_vecTargetIndex.size());
//         while(it != m_pPrimaryWeapon->m_vecTargetIndex.end())
        vector<TARGET_DATA>::iterator it = m_pPrimaryWeapon->m_vecTargetIndexData.begin();
        DBGOUT(" m_vecTargetIndexData : %d\n",m_pPrimaryWeapon->m_vecTargetIndexData.size());
        while(it != m_pPrimaryWeapon->m_vecTargetIndexData.end())
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        {
            // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            //CUnitData* pUnit = g_pScene->FindUnitDataByClientIndex( (*it) );
            CUnitData* pUnit = g_pScene->FindUnitDataByClientIndex( (*it).nTargetIndex );
            // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
            ASSERT_ASSERT(pUnit);
            if(pUnit)
            {
                D3DXVECTOR2 v1, v2, v3;
                v1 = D3DXVECTOR2(m_pt.x,m_pt.y);
                v2 = D3DXVECTOR2(pUnit->m_nObjScreenX,pUnit->m_nObjScreenY);
                v3 = D3DXVECTOR2(g_pD3dApp->GetBackBufferDesc().Width/2,g_pD3dApp->GetBackBufferDesc().Height/2);

                if( D3DXVec2Length(&(v1 - v2)) < SHUTTLE_TARGET_MOUSE_CIRCLE_RADIUS*((float)g_pD3dApp->GetBackBufferDesc().Width/(float)800) ||
                    CheckPickingTarget(pUnit))
                {
                    m_pTarget = pUnit;
                    if(m_bLButtonState)
                    {// 공격중(클릭됨)
                        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_4);
                    }
                    else
                    {// 공격하지 않음(노 클릭)
                        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_2);
                    }
                    break;
                }
            }
            it++;
        }

        if( m_pTarget && m_pOrderTarget == NULL )
        {
            // 2005-02-15 by jschoi - 오더 타겟을 잡는 부분
             m_pOrderTarget = m_pTarget;

            // 2005-10-19 by ispark
            // Enemy 타겟에 있는지 검사
            vector<PK_TARGET *>::iterator itEnemy = m_vecPKTarget.begin();
            while(itEnemy != m_vecPKTarget.end())
            {
                CEnemyData* pTarget = (CEnemyData*)g_pScene->FindUnitDataByClientIndex( (*itEnemy)->nTargetIndex );
                if(m_pOrderTarget == pTarget)
                {
                    // 있음
                    return;
                }
                itEnemy++;
            }
            
            if(    m_pTarget->m_dwPartType == _ENEMY &&                        // 1. 타겟이 Enemy이고,
//                ((CEnemyData*)m_pTarget)->GetPkState() == PK_NORMAL &&        // 2. 순수 강제 타겟이면 // 2006-01-07 by ispark, 삭제
                g_pShuttleChild->m_pPkNormalTimer->IsCityWar() == FALSE)    // 3. 도시 점령전중에는 보내지 않는다.
            {
                // 서버로 보낸다.
                MSG_FC_MOVE_LOCKON sMsg;
                sMsg.AttackIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
                sMsg.TargetIndex = ((CEnemyData*)m_pTarget)->m_infoCharacter.CharacterInfo.ClientIndex;    
                g_pFieldWinSocket->SendMsg( T_FC_MOVE_LOCKON, (char*)&sMsg, sizeof(sMsg) );                
            }
            g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LOCKON_PRIMARY, g_pShuttleChild->m_vPos, FALSE);
        }
    }
    else
    {
        // 1형 무기를 장착하지 않은 경우
        g_pInterface->m_pTarget->SetMouseType(MOUSE_TYPE_1);
    }
}

void CShuttleChild::CheckMonsterMove(CMonsterData * pMon)
{
    FLOG( "CShuttleChild::CheckMonsterMove(CMonsterData * pMon)" );
    float fElapsedTime = g_pD3dApp->GetElapsedTime();
    D3DXVECTOR3 vVel;
    D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0);
    D3DXVECTOR3 vSide = D3DXVECTOR3(0,0,1);
    int nSpeed;
    MSG_FC_MONSTER_MOVE_OK info;
    memset(&info,0x00,sizeof(MSG_FC_MONSTER_MOVE_OK));
    info.MonsterIndex = pMon->m_info.MonsterIndex;
    switch(pMon->m_ResInfo.bMonType)
    {
    case 0:
        {// 움직임 없음
            info.PositionVector = pMon->m_vPos;
        }
        break;
    case 1:
        {// 상하 움직임
//            if(pMon->m_vPos.y >= g_pGround->m_projectInfo.fHeightMax - 20.0f
//                || pMon->m_vPos.y <= 20.0f)
            if(pMon->m_vPos.y >= 250.0f)
            {
                pMon->m_nUpDir = -1;
            }
            else if(pMon->m_vPos.y <= 80.0f)
            {
                pMon->m_nUpDir = 1;
            }
            nSpeed = RANDI(70, 269);
            info.PositionVector = pMon->m_vPos + pMon->m_nUpDir*nSpeed*vUp*fElapsedTime;
        }
        break;
    case 2:
        {// 좌우 움직임
//            if(pMon->m_vPos.x <= 2670.0f || pMon->m_vPos.x >= 3750.0f)// || pMon->m_vPos.z <= 77.0f|| pMon->m_vPos.z >= 957.0f)
            if(pMon->m_vPos.z <= 77.0f)// || pMon->m_vPos.z <= 77.0f|| pMon->m_vPos.z >= 957.0f)
            {
                pMon->m_nSideDir = 1;
            }
            else if(pMon->m_vPos.z >= 957.0f)
            {
                pMon->m_nSideDir = -1;
            }
            nSpeed = RANDI(100, 299);
            info.PositionVector = pMon->m_vPos + pMon->m_nSideDir*nSpeed*vSide*fElapsedTime;
        }
        break;
    case 3:
        {// 상하 좌우 움직임
//            if(pMon->m_vPos.x <= 2670.0f || pMon->m_vPos.x >= 3750.0f)// || pMon->m_vPos.z <= 77.0f|| pMon->m_vPos.z >= 957.0f)
            if(pMon->m_vPos.z <= 77.0f)// || pMon->m_vPos.z <= 77.0f|| pMon->m_vPos.z >= 957.0f)
            {
                pMon->m_nSideDir = 1;
            }
            else if(pMon->m_vPos.z >= 957.0f)
            {
                pMon->m_nSideDir = -1;
            }
            nSpeed = RANDI(120, 319);
            info.PositionVector = pMon->m_vPos + pMon->m_nSideDir*nSpeed*vSide*fElapsedTime;
//            if(pMon->m_vPos.y >= g_pGround->m_projectInfo.fHeightMax - 20.0f
//                || pMon->m_vPos.y <= 20.0f)
            if(pMon->m_vPos.y >= 250.0f)
            {
                pMon->m_nUpDir = -1;
            }
            else if(pMon->m_vPos.y <= 80.0f)
            {
                pMon->m_nUpDir = 1;
            }
            nSpeed = RANDI(150, 349);
            info.PositionVector = pMon->m_vPos + pMon->m_nUpDir*nSpeed*vUp*fElapsedTime;
        }
        break;
    case 4:
        {
        }
        break;
    }
    D3DXVec3Normalize(&vVel,&(m_vPos - pMon->m_vPos));
    info.TargetVector = vVel;
    pMon->m_vVel = vVel;
    pMon->SetMonPosition(&info);
}

BOOL CShuttleChild::MoveOrder(BYTE bType)
{
    FLOG( "CShuttleChild::MoveOrder(BYTE bType)" );
    float fElapsedTime = g_pD3dApp->GetElapsedTime();
    D3DXVECTOR3 vVel, vOldPos, vSide, vSide2 ;
    D3DXMATRIX mat;
    float fAngle;
    D3DXVECTOR2 vPos1, vPos2, vVel2,vOldPos2;
    
    D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);

    switch(bType) 
    {
    case 0:
        {
            SetShuttleFlightInit(fElapsedTime);
            if(D3DXVec3Length(&(m_vPos - m_vTargetPos)) < 0.5f && ACOS(D3DXVec3Dot(&m_vTargetVel,&m_vVel)) < 0.01f 
                && vSide.y < 0.01f)
            {// 강제 이동 해제
                int nCheckX = ((int)m_vPos.x)/TILE_SIZE;
                int nCheckZ = ((int)m_vPos.z)/TILE_SIZE;
                if(g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEnableLand)
                {
                    if(SetOrderMoveTargetPos() == TRUE )
                    {
                        m_fNumpad0keyTime = 2.0f;
                        SendPacket(T_FC_MOVE_LANDING);
                    }
                }
                return TRUE;
            }

            vPos1.x = m_vPos.x;
            vPos1.y = m_vPos.z;
            vPos2.x = m_vTargetPos.x;
            vPos2.y = m_vTargetPos.z;
            
            vOldPos2 = vPos1;    
            vOldPos = m_vPos;

            if(D3DXVec2Length(&(vPos1 - vPos2)) > 0.5f)
            {// 목적지 까지 가는 중에 가는 곳을 바라보게 한다.
                D3DXVec2Normalize(&vVel2,&(vPos2 - vPos1));
                vVel.x = vVel2.x;
                vVel.z = vVel2.y;
                vVel.y = m_vVel.y;

                if(m_vTargetPos.y + 20.0f - m_vPos.y < 10.0f)
//                    vPos1 += m_fShuttleSpeedMax*vVel2*fElapsedTime;
                    vPos1 += m_fRealShuttleSpeedMax*vVel2*fElapsedTime;

                m_vPos.x = vPos1.x;
                m_vPos.z = vPos1.y;
                m_vPos.y += (m_vTargetPos.y + 20.0f - m_vPos.y)*fElapsedTime;
                if(D3DXVec2Length(&(vOldPos2 - vPos2)) < D3DXVec2Length(&(vPos1 - vPos2)))
                {// 타겟 지점을 지나가게 될때 타겟 지점으로 이동시킨다.
                    m_vPos.x = m_vTargetPos.x;
                    m_vPos.z = m_vTargetPos.z;
                }

                fAngle = ACOS(D3DXVec3Dot(&m_vVel,&vVel));
                D3DXVec3Cross(&vSide,&m_vVel,&vVel);
                if(fAngle > 0.01f)
                {
                    D3DXMatrixRotationAxis(&mat,&vSide,fAngle*fElapsedTime);
                }
                else
                {
                    D3DXMatrixRotationAxis(&mat,&vSide,fAngle);
                }
                D3DXVec3TransformCoord(&m_vVel,&m_vVel,&mat);

                D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
                D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
                vSide2 = vSide;
                vSide2.y = 0.0f;
                D3DXVec3Normalize(&vSide2, &vSide2);
                fAngle = ACOS(D3DXVec3Dot(&vSide2,&vSide));
                D3DXVec3Cross(&vVel,&vSide,&vSide2);
                if(fAngle > 0.01f)
                {
                    D3DXMatrixRotationAxis(&mat,&vVel,fAngle*fElapsedTime);
                }
                else
                {
                    D3DXMatrixRotationAxis(&mat,&vVel,fAngle);
                }
                D3DXVec3TransformCoord(&m_vUp,&m_vUp,&mat);
            }
            else
            {// 목적지 까지 도착후 설정한 방향으로 보게 한다.
                m_vPos.y += 0.5f*(m_vTargetPos.y - m_vPos.y)*fElapsedTime;
                
                vVel = m_vTargetVel;
                fAngle = ACOS(D3DXVec3Dot(&m_vVel,&vVel));
                D3DXVec3Cross(&vSide,&m_vVel,&vVel);
                if(fAngle > 0.01f)
                {
                    D3DXMatrixRotationAxis(&mat,&vSide,fAngle*fElapsedTime);
                }
                else
                {
                    D3DXMatrixRotationAxis(&mat,&vSide,fAngle);
                }
                D3DXVec3TransformCoord(&m_vVel,&m_vVel,&mat);

                D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
                D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
                vSide2 = vSide;
                vSide2.y = 0.0f;
                D3DXVec3Normalize(&vSide2, &vSide2);
                fAngle = ACOS(D3DXVec3Dot(&vSide2,&vSide));
                D3DXVec3Cross(&vVel,&vSide,&vSide2);
                if(fAngle > 0.01f)
                {
                    D3DXMatrixRotationAxis(&mat,&vVel,fAngle*fElapsedTime);
                }
                else
                {
                    D3DXMatrixRotationAxis(&mat,&vVel,fAngle);
                }
                D3DXVec3TransformCoord(&m_vUp,&m_vUp,&mat);
            }
        }
        break;
    case 1:// 직선 착륙
        {
            if(m_fSideCheckRate > 0.0f)
                m_fSideCheckRate -= 0.5f*fElapsedTime;
            if(m_fSideCheckRate < 0.0f)
                m_fSideCheckRate = 0.0f;
            // 평형 마추기
            // 2005-12-07 by ispark, 뒤로 움직이고 있었다면 스피드 0으로 만들어서 정지
            if(m_fShuttleSpeed < 0)
                m_fShuttleSpeed = 0.0f;
            SetShuttleFlightInit(fElapsedTime);
//            CheckMoveRate(fElapsedTime,0,-1);
            CheckMoveRate(fElapsedTime,0);
            m_vPos += m_fShuttleSpeed*m_vVel*fElapsedTime;
            if(m_fSinMove < 1.0f)
            {
                m_fSinMove += fElapsedTime;
            }
            m_vPos.y -= 50.0f*fElapsedTime*m_fSinMove;          // 착륙시 높이 감소량 세팅
            float fTempHeight = 100.0f;
            COLLISION_RESULT collResult;
            if(m_bOnObject)
            {
                D3DXMATRIX matTemp;
                D3DXVECTOR3 vSide;
                vSide = D3DXVECTOR3(0,0,1);
                D3DXMatrixLookAtLH(&matTemp,&m_vPos,&(m_vPos - m_vBasisUp),&vSide);
//                fTempHeight = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vPos).fDist;
                collResult = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,m_vPos, FALSE);
                if(collResult.fDist != DEFAULT_COLLISION_DISTANCE)
                {
                    fTempHeight = m_vPos.y - collResult.fDist;
                }
            }
            float fCheckMapHeight = CheckMap(m_vPos);
            if(collResult.fDist != DEFAULT_COLLISION_DISTANCE)
            {
                if(m_vPos.y <= fTempHeight+m_fAmorHeight)
                {
                    m_vPos.y = fTempHeight+m_fAmorHeight;
                    // 2007-12-05 by bhsohn 벽에 뚫고 들어가는 현상 처리
//                    SendPacket(T_FC_MOVE_LANDING_DONE);
//                    DBGOUT("T_FC_MOVE_LANDING_DONE\n");
//                    ChangeUnitState( _LANDED );
                    // 충돌이 있어났다.
                    if(FALSE == CheckObjLandingColl(collResult.vNormalVector))
                    {
                        if(CancelLandingMode())
                        {
                            return TRUE;
                        }                        
                    }
                    SendPacket(T_FC_MOVE_LANDING_DONE);
                    DBGOUT("T_FC_MOVE_LANDING_DONE\n");
                    ChangeUnitState( _LANDED );
                    // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리                                
                    if(IS_DT(m_myShuttleInfo.UnitKind))
                    {
                        // A기어 라면 움직임 초기화
                        InitUnitStop();
                    }                
                    // end 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리
                    
                    return TRUE;
                }
            }
            else
            {
                if(m_vPos.y <= fCheckMapHeight+m_fAmorHeight)
                {
                    // 2008-03-04 by bhsohn A기어가 아닌 기어가 땅에 착륙시 강제 출격시킴
                    if((g_pTutorial->IsTutorialMode() == FALSE)
                        && !IS_DT(m_myShuttleInfo.UnitKind))
                    {
                        if(CancelLandingMode())
                        {
                            return TRUE;
                        }
                    }
                    // end 2008-03-04 by bhsohn A기어가 아닌 기어가 땅에 착륙시 강제 출격시킴
                    m_vPos.y = fCheckMapHeight+m_fAmorHeight;
                    SendPacket(T_FC_MOVE_LANDING_DONE);
                    DBGOUT("T_FC_MOVE_LANDING_DONE\n");
                    ChangeUnitState( _LANDED );
                    // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리                                
                    if(IS_DT(m_myShuttleInfo.UnitKind))
                    {
                        // A기어 라면 움직임 초기화
                        InitUnitStop();
                    }                
                    // end 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리

                    return TRUE;
                }
            }

//            if(m_vPos.y <= fCheckMapHeight+m_fAmorHeight || m_vPos.y <= fTempHeight+m_fAmorHeight)
//            {
//                if(fCheckMapHeight + m_fAmorHeight > m_vPos.y + m_fAmorHeight - fTempHeight)
//                    m_vPos.y = fCheckMapHeight + m_fAmorHeight;
//                else
//                    m_vPos.y += m_fAmorHeight - fTempHeight;
//                SendPacket(T_FC_MOVE_LANDING_DONE);
//                ChangeUnitState( _LANDED );
//
//                return TRUE;
//            }
        }
        break;
    case 2:// 회전 착륙
        {
            SetShuttleFlightInit(fElapsedTime);
            if(m_fLandingCheckTime > 0.0f)
                m_fLandingCheckTime -= fElapsedTime;
            if(m_fLandingCheckTime > 0.0f)
            {
                vVel = m_vVel;
                vVel.y = 0.0f;
                D3DXVec3Normalize(&vVel,&vVel);
                D3DXMatrixRotationAxis(&mat,&m_vBasisUp,(D3DX_PI/2.0f)*fElapsedTime);
                D3DXVec3TransformCoord(&vVel,&vVel,&mat);
                D3DXVec3TransformCoord(&m_vVel,&m_vVel,&mat);
                D3DXVec3TransformCoord(&m_vUp,&m_vUp,&mat);
//                if(m_fShuttleSpeed > (m_fLandingCheckTime/4.0f)*100)
//                    m_vPos += (m_fLandingCheckTime/4.0f)*100*vVel*fElapsedTime;
//                else
//                    m_vPos += m_fShuttleSpeed*vVel*fElapsedTime;
                m_vPos += 100.0f*vVel*fElapsedTime;
                if(m_vPos.y > m_vTargetPos.y)
                {
                    m_vPos.y -= (m_fLandingDist/4.0f)*fElapsedTime;
                    if(m_vPos.y < m_vTargetPos.y)
                    {
                        m_vPos.y = m_vTargetPos.y;
                        m_fLandingCheckTime = 0.0f;
                    }
                }
                fAngle = ACOS(D3DXVec3Dot(&m_vVel,&vVel));
                if(fAngle != 0.0f)
                {
                    D3DXVec3Cross(&vSide,&m_vVel,&vVel);
                    D3DXMatrixRotationAxis(&mat,&vSide,fAngle*fElapsedTime);
                    D3DXVec3TransformCoord(&m_vVel,&m_vVel,&mat);
                    D3DXVec3TransformCoord(&m_vUp,&m_vUp,&mat);
//                    D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
                }
            }
            else
            {
//                if(D3DXVec3Length(&(m_vPos - m_vTargetPos)) < 5.0f)// && ACOS(D3DXVec3Dot(&m_vTargetVel,&m_vVel)) < 0.01f 
                if(m_vPos.y <= m_vTargetPos.y + 5.0f)// && ACOS(D3DXVec3Dot(&m_vTargetVel,&m_vVel)) < 0.01f 
//                    && vSide.y < 0.01f)
                {// 강제 이동 해제
                    m_vPos.y = m_vTargetPos.y;
                    SendPacket(T_FC_MOVE_LANDING_DONE);
                    DBGOUT("T_FC_MOVE_LANDING_DONE\n");
                    ChangeUnitState( _LANDED );
                    //m_dwState = _LANDED;
                    m_fShuttleSpeed = 100.0f;
                    
                    // 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리                                
                    if(IS_DT(m_myShuttleInfo.UnitKind))
                    {
                        // A기어 라면 움직임 초기화
                        InitUnitStop();
                    }                
                    // end 2007-01-02 by bhsohn A기어 b+Enter로 멈춘 후, 착륙하면 안 움직여지는 현상 처리
                    return TRUE;
                }
                m_vPos += 0.5f*(m_vTargetPos - m_vPos)*fElapsedTime;
                vVel = m_vTargetVel;
                fAngle = ACOS(D3DXVec3Dot(&m_vVel,&vVel));
                D3DXVec3Cross(&vSide,&m_vVel,&vVel);
                if(fAngle > 0.01f)
                {
                    D3DXMatrixRotationAxis(&mat,&vSide,fAngle*fElapsedTime);
                }
                else
                {
                    D3DXMatrixRotationAxis(&mat,&vSide,fAngle);
                }
                D3DXVec3TransformCoord(&m_vVel,&m_vVel,&mat);

                D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
                D3DXVec3Cross(&m_vUp,&m_vVel,&vSide);
                vSide2 = vSide;
                vSide2.y = 0.0f;
                D3DXVec3Normalize(&vSide2, &vSide2);
                fAngle = ACOS(D3DXVec3Dot(&vSide2,&vSide));
                D3DXVec3Cross(&vVel,&vSide,&vSide2);
                if(fAngle > 0.01f)
                {
                    D3DXMatrixRotationAxis(&mat,&vVel,fAngle*fElapsedTime);
                }
                else
                {
                    D3DXMatrixRotationAxis(&mat,&vVel,fAngle);
                }
                D3DXVec3TransformCoord(&m_vUp,&m_vUp,&mat);
            }
        }
        break;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CShuttleChild::SetOrderMoveTargetPos(BOOL bObjectLanding)
/// \brief        착륙 지점 계산
/// \author        dhkwon
/// \date        2004-06-29 ~ 2004-06-29
/// \warning    오브젝트가 있는 경우 유닛의 높이보다 높으면 착륙 실패
///                A기어는 경우 bObjectLanding=FALSE
///
///        추가    착륙 지점 계산에서 물 오브젝트는 제외한다.    // 2005-02-11 by jschoi    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::SetOrderMoveTargetPos(BOOL bObjectLanding)
{
    FLOG( "CShuttleChild::SetOrderMoveTargetPos()" );
    if( IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE &&
        m_bOnObject == FALSE )
    {
        return FALSE;
    }
    m_vTargetPos = m_vPos;
    m_vTargetVel = m_vVel;
    m_vTargetVel.y = 0.0f;
    D3DXVec3Normalize(&m_vTargetVel,&m_vTargetVel);
    float fTempHeight1 = -DEFAULT_COLLISION_DISTANCE;
    D3DXVECTOR3 vPos = m_vPos;

    D3DXMATRIX matTemp;
    D3DXVECTOR3 vSide;
    vSide = D3DXVECTOR3(0,0,1);
    D3DXMatrixLookAtLH(&matTemp,&vPos,&(vPos - m_vBasisUp),&vSide);
    COLLISION_RESULT collResult = g_pScene->m_pObjectRender->CheckCollMeshWithOutWaterObject(matTemp,vPos);
    if(collResult.fDist != DEFAULT_COLLISION_DISTANCE)
    {
        fTempHeight1 = m_vPos.y - collResult.fDist;
    }
    else if(bObjectLanding)
    {
        return FALSE;
    }

//        fTempHeight1 = g_pScene->m_pObjectRender->CheckCollMesh(matTemp,vPos).fDist;// object 충돌 높이
//        if(fTempHeight1 > vPos.y)
//        {
//            if(bObjectLanding)
//            {
//                return FALSE;
//            }
//            else
//            {
//                fTempHeight1 = vPos.y;
//            }
//        }
//        else
//        {
//            fTempHeight1 = vPos.y - fTempHeight1;
//        }
    float fTempHeight2 = CheckMap(m_vPos);// 맵 충돌 높이
    if(fTempHeight2 < fTempHeight1)
    {
        if(m_vPos.y > fTempHeight1 + m_fAmorHeight)
        {
            m_vTargetPos.y = fTempHeight1 + m_fAmorHeight;
        }
        else if(m_vPos.y < fTempHeight1 + m_fAmorHeight)
        {
            m_vTargetPos.y = fTempHeight1 + m_fAmorHeight;
        }
    }
    else// if(fTempHeight2 < fTempHeight1)
    {
        if(m_vPos.y > fTempHeight2 + m_fAmorHeight)
        {
            m_vTargetPos.y = fTempHeight2 + m_fAmorHeight;
        }
        else if(m_vPos.y < fTempHeight2 + m_fAmorHeight)
        {
            m_vTargetPos.y = fTempHeight2 + m_fAmorHeight;
        }
    }
//    m_fLandingDist = m_vPos.y - m_vTargetPos.y;
    m_fSinMove = 0.0f;
    return TRUE;
}

void CShuttleChild::SetCursorInit()
{
    // 2012-12-17 by jhjang 게임이 백그라운드일때, 마우스 포커스를 초기화 하지 않도록 수정
    //윈도우 포커스를 체크, 백 그라운드이면 커서 초기화를 하지 않도록 한다.
#ifdef C_MOUSEFOCUS_BACKGROUND_NOTRESET
    HWND hWnd = GetFocus();
    if(g_pD3dApp->GetHwnd() != hWnd)
        return;
#endif
    // end 2012-12-17 by jhjang 게임이 백그라운드일때, 마우스 포커스를 초기화 하지 않도록 수정

    FLOG( "CShuttleChild::SetCursorInit()" );
    POINT pt,pt2;
    GetCursorPos(&pt);
    pt2 = pt;
//    CheckMouseReverse(&pt2);
    ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
//    CheckMouseReverse(&pt);
    int nX = pt2.x - pt.x;
    int nY = pt2.y - pt.y;
    nX += g_pD3dApp->GetBackBufferDesc().Width/2;
    nY += g_pD3dApp->GetBackBufferDesc().Height/2;
    SetCursorPos(nX,nY);

    // 2008-11-13 by bhsohn 조이스틱 작업
    m_bSetCursorPos = TRUE;
}

void CShuttleChild::PrimaryBulletReloaded(MSG_FC_BATTLE_PRI_BULLET_RELOADED* pMsg)
{
    if(m_pPrimaryWeapon)
    {
        m_pPrimaryWeapon->BulletReloaded( pMsg );
    }
}

void CShuttleChild::SecondaryBulletReloaded(MSG_FC_BATTLE_SEC_BULLET_RELOADED* pMsg)
{
    if(m_pSecondaryWeapon)
    {
        g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SEC_BULLET_RELOADED, m_vPos, FALSE);
        m_pSecondaryWeapon->BulletReloaded( (MSG_FC_BATTLE_PRI_BULLET_RELOADED*)pMsg );
    }
}

USHORT CShuttleChild::GetShiftWeaponBodyconditionByUnitKind()
{
    if(IS_BT(m_myShuttleInfo.UnitKind))
        return UNIT_KIND_SHIFT_B_GEAR;
    if(IS_ST(m_myShuttleInfo.UnitKind))
        return UNIT_KIND_SHIFT_I_GEAR;
    if(IS_OT(m_myShuttleInfo.UnitKind))
        return UNIT_KIND_SHIFT_M_GEAR;
    if(IS_DT(m_myShuttleInfo.UnitKind))
        return UNIT_KIND_SHIFT_A_GEAR;
    return UNIT_KIND_SHIFT_DEFAULT;
}


void CShuttleChild::UseSkillMove(float fElapsedTime)
{
    FLOG( "CShuttleChild::UseSkillMove(float fElapsedTime)" );

    int nPatternType = TARGET_OFF;
    m_fAdvanceTime = 10.0f;

    D3DXVECTOR3 vOldPos;

    if(m_pCinemaUnitPattern != NULL)
    {
        nPatternType = m_pCinemaUnitPattern->GetHeader().nPatternType;
        BOOL bSkillStop = m_pCinemaUnitPattern->SkillTick();
        //    m_pCinemaCamera->CameraTick(); 
        
        EVENT_POINT SkillPoint, CameraPoint;
        SkillPoint=m_pCinemaUnitPattern->GetCurrentCinemaPoint();
        vOldPos = m_vPos;                            // 이전 위치
        m_vPos = SkillPoint.vPosition;                // 위치
        m_vVel = SkillPoint.vDirection;                // 방향
        m_vUp = SkillPoint.vUpVector;
        m_vTargetPosition = SkillPoint.vTarget;

        // 2005-12-02 by ispark, 속도 입력
        m_fShuttleSpeed = SkillPoint.fVelocity;

        if(m_bIsCameraPattern == TRUE)
        {
            m_pCinemaCamera->SkillTick();
            D3DXVECTOR3 vCameraPos, vCameraVel, vCameraUp;
            CameraPoint=m_pCinemaCamera->GetCurrentCinemaPoint();
            vCameraPos = CameraPoint.vPosition;
            vCameraVel = m_vPos - CameraPoint.vPosition;
            vCameraUp = CameraPoint.vUpVector;
            
            // 카메라의 위치 셋팅 2004.06.24 jschoi
            g_pD3dApp->m_pCamera->Tick(vCameraPos,vCameraVel,vCameraUp);
            
        }        
        
        if(bSkillStop)
        {

            if ( g_pShuttleChild->IsShowCinema() )
                g_pInterface->m_pInfinity->SendPacket_SkipEndingCinema( true );

            DeletePattern();
            m_nEventTypeAirPort = AIR_PORT_LANDING_PATTERN_NONE;
            // 2008-06-17 by bhsohn 편대 관련 처리
            // 무브 스킬이 끝났으면 과거 편대 정보로 변경
            m_pClientParty->RefreshFormationType();
            // end 2008-06-17 by bhsohn 편대 관련 처리
        }
    }

    // 착륙 패턴
    BOOL bLANDINGPattern = FALSE;
    switch(m_nEventTypeAirPort)
    {
    case AIR_PORT_LANDING_PATTERN_NONE:
        {
            m_nEventIndex = 0;
        }
        break;
    case AIR_PORT_LANDING_PATTERN_START:
        {
//            m_fShuttleSpeed -= g_pD3dApp->GetElapsedTime()*100;
//            if(m_fShuttleSpeed < AIR_PORT_MIN_SPEED)
//                m_fShuttleSpeed = AIR_PORT_MIN_SPEED;
//            SetShuttleFlightInit(fElapsedTime);
            m_nEventIndex = 0;
            bLANDINGPattern = TRUE;
        }
        break;
    case AIR_PORT_LANDING_PATTERN_END:
        {
            bLANDINGPattern = TRUE;
        }
        break;
    case AIR_PORT_TAKEOFF_PATTERN_START:
        {
            bLANDINGPattern = TRUE;
        }
        break;
    case AIR_PORT_TAKEOFF_PATTERN_END:
        {
            bLANDINGPattern = TRUE;
        }
        break;
    }

    switch(nPatternType)
    {
    case TARGET_ON:    // 타켓 방향이 있는 패턴
        {
            D3DXMatrixLookAtRH( &m_mMatrix, &m_vPos, &(m_vTargetPosition), &m_vUp);
            D3DXVec3Normalize( &m_vVel , &(m_vTargetPosition - m_vPos) ); 
        }
        break;
    case TARGET_OFF: // 타켓 방향이 없는 패턴
        {
            D3DXMatrixLookAtLH( &m_mMatrix, &m_vPos, &(m_vPos - m_vVel), &m_vUp);
        }
        break;
    }

    // 2005-11-29 by ispark
    // 오버부스터 스킬을 사용 중이라면 특별 이펙트(부스터) 실행
    if(m_pSkill->IsSkillOverBooster())
    {
        m_nBooster = BOOSTER_TYPE_MAX;
        CheckBoosterAni();        
    }

    D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );

    // 좌우 날개 엔진 위치 설정
    m_vSideVel.x = m_mMatrix._11;
    m_vSideVel.y = m_mMatrix._12;
    m_vSideVel.z = m_mMatrix._13;

    D3DXVec3TransformCoord(&m_vLWPos, &m_PrimaryAttack.vSidePos, &m_mMatrix);
    m_vLWPos += 3.0f*m_vVel+3.0f*m_vUp; // 3.0f*m_vVel;
    D3DXVECTOR3 vPrimaryRightPos = D3DXVECTOR3(-m_PrimaryAttack.vSidePos.x, m_PrimaryAttack.vSidePos.y, m_PrimaryAttack.vSidePos.z);
    D3DXVec3TransformCoord(&m_vRWPos, &vPrimaryRightPos, &m_mMatrix);
    m_vRWPos += 3.0f*m_vVel+3.0f*m_vUp; // 3.0f*m_vVel;
    D3DXVec3TransformCoord(&m_vLWSecondaryPos, &m_SecondaryAttack.vSidePos, &m_mMatrix);
    D3DXVECTOR3 vSecondaryRightPos = D3DXVECTOR3(-m_SecondaryAttack.vSidePos.x, m_SecondaryAttack.vSidePos.y, m_SecondaryAttack.vSidePos.z);
    D3DXVec3TransformCoord(&m_vRWSecondaryPos, &vSecondaryRightPos, &m_mMatrix);

    // 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치
    D3DXVec3TransformCoord(&m_vPetLeftPos, &m_PetAttack.vSidePos, &m_mMatrix);
    
    D3DXVECTOR3 vPetRightPos = D3DXVECTOR3(-m_PetAttack.vSidePos.x,
                                            m_PetAttack.vSidePos.y, 
                                            m_PetAttack.vSidePos.z);
    D3DXVec3TransformCoord(&m_vPetRightPos, &vPetRightPos, &m_mMatrix);

    m_vPetLeftVel = m_vVel;
    m_vPetRightVel = m_vVel;
    // end 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치
    // 무기 방향(에이 기어)

    if(IS_DT(m_myShuttleInfo.UnitKind))
    {
        // 2006-01-16 by ispark, 왜 A기어가 패턴 이동일 때 포를 움직이게 했을까? 
//        if(g_pTutorial->IsTutorialMode() == TRUE)
//        {
            m_vWeaponVel = m_vVel;
//        }
//        else
//        {
//            m_vWeaponVel = m_vMouseDir;
//        }
    }
    
    // 스크린상의 좌표를 구한다.
    g_pD3dApp->CalcObjectSourceScreenCoords(m_vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
        m_nObjScreenX, m_nObjScreenY,m_nObjScreenW);
    // 카메라로부터의 거리
    m_fDistanceCamera = D3DXVec3Length(&(m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));
    m_pMoveChat->Tick();
    m_pIDChat->Tick();


    // 2004-11-26 by jschoi - 충돌 처리
    // 패턴 이동 중 충돌 처리(단, 워프인, 워프아웃 패턴은 충돌처리 생략)
    if(    m_nCurrentPatternNumber != PATTERN_UNIT_WARP_IN &&
        m_nCurrentPatternNumber != PATTERN_UNIT_WARP_OUT &&
        bLANDINGPattern == FALSE)
    {
        // 맵과의 충돌 검사-셔틀의 중요 4부분의 충돌 검사를 하자 앞 뒤 좌 우(4 귀퉁이 좌표를 맞추는 작업도 해줘야할 것이다)
        D3DXVECTOR3 vFront, vBack, vLeft, vRight;
        D3DXVECTOR3 vParticlePos;
        vFront = m_vPos + 8.0f*m_vVel;
        vBack = m_vPos - 3.0f*m_vVel;
        vLeft = m_vPos - 7.0f*m_vSideVel;
        vRight = m_vPos + 7.0f*m_vSideVel;
        int nMapCheckType = 0;
        if(vFront.y < CheckMap(vFront))
        {
            nMapCheckType = 1;
            vParticlePos = vFront;
            if(g_pD3dApp->m_pEffectList)
            {
                CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
            }
        }
        else if(vBack.y < CheckMap(vBack))
        {
            nMapCheckType = 2;
            vParticlePos = vBack;
            if(g_pD3dApp->m_pEffectList)
            {
                CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
            }
        }
        else if(vLeft.y < CheckMap(vLeft))
        {
            nMapCheckType = 3;
            vParticlePos = vLeft;
            if(g_pD3dApp->m_pEffectList)
            {
                CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
            }
        }
        else if(vRight.y < CheckMap(vRight))
        {
            nMapCheckType = 4;
            vParticlePos = vRight;
            if(g_pD3dApp->m_pEffectList)
            {
                CAppEffectData * pData = new CAppEffectData(NULL,RC_EFF_COLL_01,vParticlePos);
                pData = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pData);
            }
        }
        if(nMapCheckType)
        {
            DeletePattern();
            m_vPos = vOldPos;            // 충돌 했으므로 이전 위치로
            m_bKeyBlock = FALSE;
        }

        int nCollType = COLL_NONE;    
        D3DXMATRIX matArray[6];
        D3DXVECTOR3 vSide,vVel;
        D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);
        D3DXMatrixLookAtLH(&matArray[COLL_FRONT],&m_vPos,&(m_vPos + m_vVel),&m_vUp);    // 앞
        D3DXMatrixLookAtLH(&matArray[COLL_BACK],&m_vPos,&(m_vPos - m_vVel),&m_vUp);    // 뒤
        D3DXMatrixLookAtLH(&matArray[COLL_LEFT],&m_vPos,&(m_vPos - vSide),&m_vUp);        // 좌
        D3DXMatrixLookAtLH(&matArray[COLL_RIGHT],&m_vPos,&(m_vPos + vSide),&m_vUp);        // 우
        D3DXMatrixLookAtLH(&matArray[COLL_UP],&m_vPos,&(m_vPos + m_vUp),&vSide);        // 위
        D3DXMatrixLookAtLH(&matArray[COLL_DOWN],&m_vPos,&(m_vPos - m_vUp),&vSide);        // 아래
        float fMoveDistance = D3DXVec3Length(&(m_vPos - vOldPos));
        float size[6] = { 10+fMoveDistance,10+fMoveDistance,10+fMoveDistance,10+fMoveDistance,5+fMoveDistance,5+fMoveDistance };    // 유닛 크기    앞,뒤,좌,우,위,아래    
        nCollType = g_pScene->m_pObjectRender->CheckCollMesh(matArray,size).nCollType;
        if(nCollType != COLL_NONE)
        {
            // 충돌 했다
            DeletePattern();
            m_vPos = vOldPos;            // 충돌 했으므로 이전 위치로
            m_bKeyBlock = FALSE;
        }

        // 2013-01-21 by jhjang 스킬 이동중 몬스터와의 충돌 처리 추가
        float fColledMonsterSize = 0;
        ClientIndex_t monsterIndex = 0;
        nCollType = g_pScene->m_pMonsterRender->CheckCollMesh(matArray,size,&fColledMonsterSize, &monsterIndex).nCollType;

        if(nCollType != COLL_NONE)
        {
            // 충돌 했다
            DeletePattern();
            m_vPos = vOldPos;            // 충돌 했으므로 이전 위치로
            m_bKeyBlock = FALSE;
        }
        // end 2013-01-21 by jhjang 스킬 이동중 몬스터와의 충돌 처리 추가
    }

    if(    m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT )
    {
        if(m_fWarpOutDistance < D3DXVec3Length(&(m_vPos - m_vWarpOutPosition)) )
        {
            // 워프 종료 해야할 위치를 지나쳤다.
            m_fWarpOutDistance = 0.0f;
            DeletePattern();
            m_bKeyBlock = FALSE;
        }
    }

    // 맵을 벗어나면 못나가게 셋팅 체크
    if(m_vPos.x < 0.0f || m_vPos.x > g_pGround->m_projectInfo.sXSize*TILE_SIZE || 
        m_vPos.z < 0.0f || m_vPos.z > g_pGround->m_projectInfo.sYSize*TILE_SIZE)
    {
        DeletePattern();
        m_vPos = vOldPos;            // 이전 위치로 
    }

    // 못가는 지역체크하여 빽시킨다.
    int nXCheck, nYCheck;
    nXCheck = (int)(m_vPos.x / TILE_SIZE);
    nYCheck = (int)(m_vPos.z / TILE_SIZE);
    m_bMoveGroundType = g_pGround->m_pTileInfo[nXCheck*g_pGround->m_projectInfo.sXSize + nYCheck].bMove;
    if(m_bMoveGroundType == 2)
    {
        DeletePattern();
        m_vPos = vOldPos;            // 이전 위치로 
    }

    // 그림자 위치 설정
//    CheckShadowPos();

    if(m_pCharacterInfo)
    {
        m_pCharacterInfo->SetAllBodyConditionMatrix( m_mMatrix );
        m_pCharacterInfo->Tick(fElapsedTime);
    }

}



void CShuttleChild::SetTargetVectorforLandedMove(void)
{
    D3DXVECTOR3 temp;
    m_vPos.x = (int(m_vPos.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;                            // 자신의 중심점 위치 계산
    m_vPos.z = (int(m_vPos.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;

    temp = m_vPos + m_vVel * TILE_SIZE;
    temp.x = (int(temp.x/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;                                // 자신의 중심점 위치 계산
    temp.z = (int(temp.z/TILE_SIZE))*TILE_SIZE + TILE_SIZE/2;
    
    D3DXVec3Normalize(&m_vVel, &(temp - m_vPos));
}

void CShuttleChild::SetUnitMovingAnimation(float fElapsedTime)
{
    //    if(m_fAnimationMoveTimer < 0)
    //    {
    //m_fAnimationMoveTimer = ANIMATION_UNIT_TIMING;        
        
    FLOAT        Yaw;    
    
    if(m_fAniFlag == ANI_UPDOWN_FLAG)                                                        /// 전후진 좌표계산 
    {    
    
        D3DXVec3Lerp( &m_vPos, &m_vMyOldPos, &m_vAniFrame, m_fFrameLerp );                        

        // 2005-10-26 by ispark
        // LANDED 오브젝트 검사
        CheckCollForObject2(fElapsedTime);
        
//        m_fFrameLerp += 0.05f;  //0.05    
        m_fFrameLerp += fElapsedTime;  // 2004-07-21 by dhkwon

        if(m_fFrameLerp >= 1.0f )
        {                 
            m_fAniFlag   = 0;
            m_fFrameLerp = 0;
            m_bAniKeyBlock = FALSE;                    
            SetTargetVectorforLandedMove();            
        }
    }
    else if(m_fAniFlag == ANI_LEFT_FLAG || m_fAniFlag == ANI_RIGHT_FLAG )                    /// 좌우 회전 좌표계산
    {        
        if(m_fAniFlag == ANI_LEFT_FLAG)
        {
            Yaw    = D3DX_PI * -5.0f / 90.0f;                                    
        }
        else if(m_fAniFlag == ANI_RIGHT_FLAG)
        {
            Yaw = D3DX_PI * 5.0f / 90.0f;    
        }

        m_fAnimationMoveTimer -= fElapsedTime;
        
        if(m_fAnimationMoveTimer < 0) 
        {
            m_fAnimationMoveTimer = ANIMATION_UNIT_TIMING;
            m_fFrameLerp += 0.1f;
//            m_fFrameLerp += fElapsedTime;  // 2004-07-21 by dhkwon
            
            D3DXMATRIX mat;
            D3DXQUATERNION    g_aniRot[2];                                            /// 회전(quaternion)키 값
            
            FLOAT Pitch    = 0;
            FLOAT Roll    = 0;
            D3DXQuaternionRotationYawPitchRoll( &g_aniRot[0], Yaw, Pitch, Roll );    /// 사원수 키(Y축90도)
            
            Yaw    = 0;
            Pitch    = 0;                                                            /// X축 0도 회전
            Roll    = 0;
            D3DXQuaternionRotationYawPitchRoll( &g_aniRot[1], Yaw, Pitch, Roll );    /// 사원수 키(X축90도)
            
            D3DXQUATERNION quat;
            D3DXQuaternionSlerp( &quat, &g_aniRot[0], &g_aniRot[1], m_fFrameLerp );
            D3DXMatrixRotationQuaternion( &mat, &quat );                            /// 사원수를 회전행렬값으로 변환
            D3DXVec3TransformCoord(&m_vVel, &m_vVel, &mat);
            
            
            if( m_fFrameLerp > 1.0f )
            {
                m_fFrameLerp = 0.0f;    
                m_fAniFlag   = 0;
                m_bAniKeyBlock = FALSE;
            }
        }
    }
    
    else if(m_fAniFlag == ANI_REFLEXION_RIGHT_FLAG || m_fAniFlag == ANI_REFLEXION_LEFT_FLAG)/// 벽을 만날때 회전 반경계산                                     /// 좌우 회전 좌표계산
    {        
        if(m_fAniFlag == ANI_REFLEXION_LEFT_FLAG)
        {
            if(IS_DT(m_myShuttleInfo.UnitKind) && !m_bIsAir)
            {
                // 2008-02-12 by dgwoo A기어가 땅에서 충돌시 방향전환을 다른기어와 조금 다르게 처리.
                D3DXMATRIX matTemp;    
                
                D3DXMatrixRotationAxis(&matTemp, &m_vUp, -0.08f);
                D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
                
                Yaw = D3DX_PI * -2.0f / 240.0f;    

            }
            
            else
            {
                D3DXMATRIX matTemp;    
                D3DXMatrixRotationAxis(&matTemp, &m_vUp, -0.1f);
                D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
                Yaw = D3DX_PI * -5.0f / 240.0f;            
            }
            
        }
        else
        {
            if(IS_DT(m_myShuttleInfo.UnitKind))
            {
                D3DXMATRIX matTemp;    
                
                D3DXMatrixRotationAxis(&matTemp, &m_vUp, 0.08f);
                D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
                
                Yaw = D3DX_PI * 2.0f / 240.0f;
            }
            else
            {

                D3DXMATRIX matTemp;    
                    
                D3DXMatrixRotationAxis(&matTemp, &m_vUp, 0.1f);
                D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);

                Yaw = D3DX_PI * 5.0f / 240.0f;
            }
        
//            m_fFrameLerp += 0.1f;
            m_fFrameLerp += fElapsedTime;// 2004-07-21 by dhkwon
            
            D3DXMATRIX mat;
            D3DXQUATERNION    g_aniRot[2];                                            /// 회전(quaternion)키 값
            
            FLOAT Pitch    = 0;
            FLOAT Roll    = 0;
            D3DXQuaternionRotationYawPitchRoll( &g_aniRot[0], Yaw, Pitch, Roll );    /// 사원수 키(Y축90도)
            
            Yaw    = 0;
            Pitch    = 0;                                                            /// X축 0도 회전
            Roll    = 0;
            D3DXQuaternionRotationYawPitchRoll( &g_aniRot[1], Yaw, Pitch, Roll );    /// 사원수 키(X축90도)
            
            D3DXQUATERNION quat;
            D3DXQuaternionSlerp( &quat, &g_aniRot[0], &g_aniRot[1], m_fFrameLerp );
            D3DXMatrixRotationQuaternion( &mat, &quat );                            /// 사원수를 회전행렬값으로 변환
            D3DXVec3TransformCoord(&m_vVel, &m_vVel, &mat);
            
            
            if( m_fFrameLerp > 1.0f )
            {
                m_fFrameLerp = 0.0f;    
                m_fAniFlag   = 0;
                m_bAniKeyBlock = FALSE;
            }
        }
        
            if( m_fShuttleSpeed <= 0)
            {
                m_fShuttleSpeed = 10;
            }
            if(m_nBooster == BOOSTER_TYPE_AIRBREAK)
            {
                m_nBooster = BOOSTER_TYPE_MIN;
        }
    }    
//    SendPacket(T_FC_MOVE);
    g_fSendMovePacket = max(g_fSendMovePacket,5.0f);
}    
        



BOOL CShuttleChild::ScanEnemyFrontBAckAndSpace(void)
{    
    float        LengthCenter;
         
    CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
    while(itEnemy != g_pScene->m_mapEnemyList.end())
    {
        LengthCenter  = D3DXVec3Length(&(itEnemy->second->m_vPos - m_vAniFrame));
        
        if(LengthCenter <= 5)
        {
            return TRUE;
        }    
                
        itEnemy++;
    }
    return FALSE;
}

BOOL CShuttleChild::InitCinemaSkill(int nSkillItemNumber)
{
    // 2005-12-07 by ispark, 에어스킬 사용한다.
    m_bSkillAirMove = TRUE;

    if(InitCinemaUnit(nSkillItemNumber, TRUE) == FALSE)
    {
        m_bSkillMoveIsUse = FALSE;
//        ITEM_SKILL* pItemSkill = m_pSkill->FindItemSkill(nSkillItemNumber);
//        map<LONGLONG, USED_ITEM*>::iterator    itUsedItem = m_mapUsedItem.find(pItemSkill->UniqueNumber);
//        if(itUsedItem != m_mapUsedItem.end())
//        {
//            m_mapUsedItem.erase(itUsedItem);
//        }
//        vector<USED_ITEM*>::iterator itUsedItem = m_vecUsedItem.begin();
//        while(itUsedItem != m_vecUsedItem.end())
//        {
//            if((*itUsedItem)->ItemNum == pItemSkill->ItemNum)
//            {
//                util::del(itUsedItem);
//                m_vecUsedItem.erase(itUsedItem);
//                break;
//            }
//            itUsedItem++;
//        }

        m_nCurrentPatternNumber = 0;
        return FALSE;
    }
    m_bSkillMoveIsUse = TRUE;
    m_nCurrentPatternNumber = nSkillItemNumber;
    return TRUE;
}

BOOL CShuttleChild::InitCinemaUnit(int nUnitPatternType, BOOL bUpUse)
{
    char str[32];
    sprintf(str,"%08d",nUnitPatternType);
    if(m_pCinemaUnitPattern != NULL)
    {
        util::del( m_pCinemaUnitPattern );
    }
     m_pCinemaUnitPattern = g_pScene->LoadCinemaData(str);
    if(m_pCinemaUnitPattern == NULL)
    {
        DBGOUT( "Cinema : Can't Find Pattern(Unit) File.(%s)\n",str);
        return FALSE;
    }
    EVENT_POINT ep;
    ep.vPosition = D3DXVECTOR3(m_mMatrix._41,m_mMatrix._42,m_mMatrix._43);
    ep.vDirection = -D3DXVECTOR3(m_mMatrix._31,m_mMatrix._32,m_mMatrix._33);
    if(bUpUse)    // 2005-08-12 by ispark
    {
        ep.vUpVector = D3DXVECTOR3(m_mMatrix._21,m_mMatrix._22,m_mMatrix._23);
    }
    else
    {
        // 강제 셋팅
        ep.vUpVector = D3DXVECTOR3(0, 1, 0);
    }
    D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
    ep.fVelocity = 100;
    ep.fCurvature = DEFAULT_CURVATURE;    // 초기값 셋팅
    m_pCinemaUnitPattern->InitCinemaData(ep);
    return TRUE;
}
// 2010. 05. 27 by jskim 시네마 적용 카메라 구현
BOOL CShuttleChild::InitCinemaUnit(int nUnitPatternType, BOOL bUpUse,D3DXVECTOR3 vPosition, D3DXVECTOR3 vDirection)
{
    char str[32];
    sprintf(str,"%08d",nUnitPatternType);
    if(m_pCinemaUnitPattern != NULL)
    {
        util::del( m_pCinemaUnitPattern );
    }
    m_pCinemaUnitPattern = g_pScene->LoadCinemaData(str);
    if(m_pCinemaUnitPattern == NULL)
    {
        DBGOUT( "Cinema : Can't Find Pattern(Unit) File.(%s)\n",str);
        return FALSE;
    }
    EVENT_POINT ep;
    ep.vPosition = D3DXVECTOR3(vPosition.x, vPosition.y,vPosition.z);
    ep.vDirection = D3DXVECTOR3(vDirection.x,vDirection.y,vDirection.z);
    //m_vPos = vTarget;
    if(bUpUse)    // 2005-08-12 by ispark
    {
        ep.vUpVector = D3DXVECTOR3(m_mMatrix._21,m_mMatrix._22,m_mMatrix._23);
    }
    else
    {
        // 강제 셋팅
        ep.vUpVector = D3DXVECTOR3(0, 1, 0);
    }
    D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
    ep.fVelocity = 100;
    ep.fCurvature = DEFAULT_CURVATURE;    // 초기값 셋팅
    m_pCinemaUnitPattern->InitCinemaData(ep);
    return TRUE;
}

void CShuttleChild::InfinityCinema(BOOL bSuccess)
{

    if ( TRUE == g_pInterface->m_pInfinity->GetEnableCinemaSkip() )
        return;

    CObjectChild * pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPositionIndex(EVENT_TYPE_INFI_CINEMA, EVENT_TYPE_PARAM_INFI_CINEMA);

    if(pCurrentObjEvent)
    {
        D3DXVECTOR3 vVel,vUp, vPos;
        vVel = m_vPos - pCurrentObjEvent->m_vPos;
        D3DXVec3Normalize(&vPos, &(pCurrentObjEvent->m_vPos - g_pShuttleChild->m_vPos));
        vUp.x = m_mMatrix._21;
        vUp.y = m_mMatrix._22;
        vUp.z = m_mMatrix._23;
        if(vVel == vUp)
        {// 매트릭스가 깨지지 않도록
            m_mMatrix._21 += 0.001f;
            m_mMatrix._22 -= 0.001f;
            m_mMatrix._23 += 0.002f;
        }
        m_mMatrix._31 = vVel.x;
        m_mMatrix._32 = vVel.y;
        m_mMatrix._33 = vVel.z;
        int succestype;
        if(bSuccess)
        {
            succestype = PATTERN_CAMERA_SUCCESS_INFINITY;
        }
        else
        {
            succestype = PATTERN_CAMERA_FAIL_INFINITY;
        }
    
        if(InitCinemaUnit(succestype, FALSE, pCurrentObjEvent->m_vPos, vPos) == TRUE)
        {
            m_bSkillMoveIsUse = TRUE;
            g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
            if(g_pInterface->m_bShowInterface)
            {                
                SetShowCinema ( TRUE );
                g_pInterface->SetScreenShotMode(TRUE);
                m_bKeyBlock = TRUE;
            }
        }    
        else
        {
            DBGOUT("[succestype] : Pattern File Non Exsistent.\n");

            g_pInterface->m_pInfinity->SendPacket_SkipEndingCinema();
        }
    }

}

BOOL CShuttleChild::InitCinemaCamera(int nCameraPattern, D3DXVECTOR3 vPosition, D3DXVECTOR3 vDirection)
{
    D3DXVECTOR3 vPos,vVel,vVel2,vUp;
    vVel = vDirection;
    vUp =  D3DXVECTOR3(0,1,0);
    vPos = vPosition;    
    char str[32];
    sprintf(str,"%08d",nCameraPattern);
    if(m_pCinemaCamera != NULL)
    {
        util::del( m_pCinemaCamera );
    }
    m_pCinemaCamera = g_pScene->LoadCinemaData(str);
    if(m_pCinemaCamera == NULL) // 해당 패턴 파일이 없는 경우
    {
        m_bIsCameraPattern = FALSE;
        DBGOUT( "Cinema : Can't Find Pattern(Camera) File.(%s)\n",str);
        return FALSE;
    }
    m_bIsCameraPattern = TRUE;
    EVENT_POINT ep;
    //    ep.vPosition = g_pShuttleChild->m_vPos;
    //    ep.vDirection = g_pShuttleChild->m_vVel;
    //    ep.vUpVector = D3DXVECTOR3(0,1,0);
    ep.vPosition = vPos; 
    ep.vDirection = vVel;
    ep.vUpVector = vUp;
    D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
    ep.fVelocity = 100;
    ep.fCurvature = DEFAULT_CURVATURE;    // 초기값 셋팅
    m_pCinemaCamera->InitCinemaData(ep);
    return TRUE;
}
//end 2010. 05. 27 by jskim 시네마 적용 카메라 구현 

BOOL CShuttleChild::InitCinemaCamera(int nCameraPattern)
{
    D3DXVECTOR3 vPos,vVel,vUp;
    vVel = m_vVel;
    vVel.y = 0;
    D3DXVec3Normalize(&vVel,&vVel);
    vUp =  D3DXVECTOR3(0,1,0);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM                                              
    vVel.x = vVel.x*100;    // 2011-11-28 by jhahn EP4 시네마 수정
    vPos = m_vPos - vVel + vUp*100;        // 2011-11-28 by jhahn EP4 시네마 수정
#else 
    vPos = m_vPos - vVel*100 + vUp*100;
#endif    
//    g_pCamera->SetViewParams( vPos , vVel , vUp );
    
    char str[32];
    sprintf(str,"%08d",nCameraPattern);    
    if(m_pCinemaCamera != NULL)
    {
        util::del( m_pCinemaCamera );
    }
    m_pCinemaCamera = g_pScene->LoadCinemaData(str);
    if(m_pCinemaCamera == NULL) // 해당 패턴 파일이 없는 경우
    {
        m_bIsCameraPattern = FALSE;
        DBGOUT( "Cinema : Can't Find Pattern(Camera) File.(%s)\n",str);
        return FALSE;
    }
    m_bIsCameraPattern = TRUE;
    EVENT_POINT ep;
//    ep.vPosition = g_pShuttleChild->m_vPos;
//    ep.vDirection = g_pShuttleChild->m_vVel;
//    ep.vUpVector = D3DXVECTOR3(0,1,0);
    ep.vPosition = vPos;
    ep.vDirection = vVel;
    ep.vUpVector = vUp;
    D3DXVec3Normalize(&ep.vDirection,&ep.vDirection);
    ep.fVelocity = 100;
    ep.fCurvature = DEFAULT_CURVATURE;    // 초기값 셋팅
    m_pCinemaCamera->InitCinemaData(ep);
    return TRUE;
}

// 충돌 검사 2004.06.19 jschoi 
// 이 함수는 단지 셔틀과 맵 오브젝트와의 충돌 상태만 본다.
BOOL CShuttleChild::CheckCollMapObject()
{
    float fElapsedTime = g_pD3dApp->GetElapsedTime();
    int nCollType = COLL_NONE;    
    
    D3DXMATRIX matArray[6];
    D3DXVECTOR3 vSide,vVel;

    D3DXVec3Cross(&vSide,&m_vUp,&m_vVel);

    D3DXMatrixLookAtLH(&matArray[COLL_FRONT],&m_vPos,&(m_vPos + m_vVel),&m_vUp);    // 앞
    D3DXMatrixLookAtLH(&matArray[COLL_BACK],&m_vPos,&(m_vPos - m_vVel),&m_vUp);    // 뒤
    D3DXMatrixLookAtLH(&matArray[COLL_LEFT],&m_vPos,&(m_vPos - vSide),&m_vUp);        // 좌
    D3DXMatrixLookAtLH(&matArray[COLL_RIGHT],&m_vPos,&(m_vPos + vSide),&m_vUp);        // 우
    D3DXMatrixLookAtLH(&matArray[COLL_UP],&m_vPos,&(m_vPos + m_vUp),&vSide);        // 위
    D3DXMatrixLookAtLH(&matArray[COLL_DOWN],&m_vPos,&(m_vPos - m_vUp),&vSide);        // 아래
    
    float fFrontMove = m_fShuttleSpeed*fElapsedTime;
//    float fSideMove = m_fLRVelRate*m_fRealShuttleSpeedMax*fElapsedTime;
    float size[6] = { 10+fFrontMove,10-fFrontMove,10,10,5,5 };    // 유닛 크기    앞,뒤,좌,우,위,아래

    nCollType = g_pScene->m_pObjectRender->CheckCollMesh(matArray,size).nCollType;
    
    if(nCollType == COLL_NONE)
    {
        // 충돌 안함
        return FALSE;
    }
    else
    {
        // 충돌 했다
        return TRUE;
    }
}

void CShuttleChild::DeletePattern()
{
    // 2005-07-18 by ispark
    // FALLING상태일때 카메라 패턴을 없애면 정지되는 버그
    if(m_bIsCameraPattern == TRUE && m_dwState == _FALLING)
    {
        return;
    }

    if(    m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT)// 워프 아웃 패턴 종료 시에는 엔진 스탑
    {
        m_nShuttleStopState = 4;    // Air_Break Type 추가
    }
    m_nCurrentPatternNumber = 0;
    m_fOldUpRate= 0;        // 이전 업벡터를 기록하여 부드럽게 기울인다(좌우 이동 기울임과 회전 기울임 조절) - jschoi
    m_fSkillMoveTime = 0.0f;
    SetCursorInit();
    m_bSkillMoveIsUse = FALSE;
    util::del(m_pCinemaUnitPattern);
    util::del(m_pCinemaCamera);
    m_bSkillAirMove = FALSE;        // 2005-12-07 by ispark

    if(m_bIsCameraPattern == TRUE)
    {
//        g_pD3dApp->m_pCamera->Tick(m_vPos , m_vVel, m_vUp);
        m_bIsCameraPattern = FALSE;                
    }
    // 2010. 05. 27 by jskim 시네마 적용 카메라 구현
    if( IsShowCinema() )
    {
        g_pInterface->SetScreenShotMode(FALSE);
        SetShowCinema ( FALSE );
        m_bKeyBlock = FALSE;
// 2011-06-30 by jhahn 인피3차 시점 1인칭 변경
        g_pD3dApp->m_pCamera->SetCamType( CAMERA_TYPE_NORMAL);   
        m_InfiState = TRUE;
//end 2011-06-30 by jhahn 인피3차 시점 1인칭 변경

//2011-07-20 by jhahn    인피3차 시네마플레이중 타캐릭터 안보이기

        CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();    
        while(itEnemy != g_pScene->m_mapEnemyList.end())
        {                             
            itEnemy->second->m_bRender = TRUE;    
            itEnemy->second->SetShowData();
            itEnemy++;
        }            

//end 2011-07-20 by jhahn    인피3차 시네마플레이중 타캐릭터 안보이기
    }
    //end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
}

//void CShuttleChild::SetAttackAvaliableEnemyIndex( int nTargetIndex, BOOL bSet )
//{
//    if(bSet)
//    {
//        vector<int>::iterator it = m_vecAttackAvailableEnemyIndex.begin();
//        while(it != m_vecAttackAvailableEnemyIndex.end() )
//        {
//            if( *it == nTargetIndex )
//            {
//                return;
//            }
//            it++;
//        }
//        m_vecAttackAvailableEnemyIndex.push_back( nTargetIndex );
//    }
//    else
//    {
//        vector<int>::iterator it = m_vecAttackAvailableEnemyIndex.begin();
//        while(it != m_vecAttackAvailableEnemyIndex.end() )
//        {
//            if( *it == nTargetIndex )
//            {
//                m_vecAttackAvailableEnemyIndex.erase( it );
//                break;
//            }
//            it++;
//        }
//    }
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::SetPKMode(BOOL bSet)
/// \brief        PK가능맵에서 Enemy들을 모두 m_vecAttackAvailableEnemyIndex에 넣고 뺀다.
/// \author        dhkwon
/// \date        2004-07-21 ~ 2004-07-21
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetPKMode(BOOL bSet)
{
    if(bSet)
    {
        CMapEnemyIterator itMap = g_pScene->m_mapEnemyList.begin();
        while( itMap != g_pScene->m_mapEnemyList.end() )
        {
            itMap->second->SetPkState( PK_NORMAL, TRUE );
//            vector<int>::iterator itVec = m_vecAttackAvailableEnemyIndex.begin();
//            while(itVec != m_vecAttackAvailableEnemyIndex.end() )
//            {
//                if( *itVec == (*itEnemy)->m_infoCharacter.ClientIndex )
//                {
//                    break;
//                }
//                itVec++;
//            }
//            if(itVec == m_vecAttackAvailableEnemyIndex.end())
//            {
//                m_vecAttackAvailableEnemyIndex.push_back((*itEnemy)->m_infoCharacter.ClientIndex);
//            }
            itMap++;
        }
    }
    else
    {
        CMapEnemyIterator itMap = g_pScene->m_mapEnemyList.begin();
        while( itMap != g_pScene->m_mapEnemyList.end() )
        {
            if( itMap->second->IsPkEnable() == TRUE )
            {
                itMap->second->SetPkState( PK_NORMAL, FALSE );
            }
            itMap++;
        }
//        vector<int>::iterator itVec = m_vecAttackAvailableEnemyIndex.begin();
//        while(itVec != m_vecAttackAvailableEnemyIndex.end() )
//        {
//            if( *itVec < MONSTER_CLIENT_INDEX_START_NUM )
//            {
//                CMapEnemyIterator itMap = g_pScene->m_mapEnemyList.find( *itVec );
//                if( itMap != g_pScene->m_mapEnemyList.end() )
//                {
//                    if( itMap->second->IsPkEnable() == FALSE )
//                    {
//                        m_vecAttackAvailableEnemyIndex.erase( itVec );
//                        continue;
//                    }
//                }
//                else
//                {
//                    m_vecAttackAvailableEnemyIndex.erase( itVec );
//                    continue;
//                }
//            }
//            itVec++;
//        }
    }
}


DWORD CShuttleChild::GetLastAttackTick()
{
    return max( m_pPrimaryWeapon ? m_pPrimaryWeapon->GetAttackTick() : 0, 
                m_pSecondaryWeapon ? m_pSecondaryWeapon->GetAttackTick() : 0);
}

void CShuttleChild::TryLandingState()
{
    // 2005-04-11 by jschoi - Tutorial
    if(g_pTutorial->IsTutorialMode() == TRUE &&
        g_pTutorial->GetLesson() != L1)
        return;

    if(m_fNumpad0keyTime <= 0.0f)
    {
        int nCheckX = ((int)m_vPos.x)/TILE_SIZE;
        int nCheckZ = ((int)m_vPos.z)/TILE_SIZE;
        if( g_pGround->m_pTileInfo[nCheckX*g_pGround->m_projectInfo.sYSize + nCheckZ].bEnableLand ||
            (IS_DT(m_myShuttleInfo.UnitKind) && m_bIsAir))
        {
            BOOL bObjectLanging = 
                (IS_DT(m_myShuttleInfo.UnitKind)==FALSE ||
                IsTileMapRenderEnable(m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE) ? TRUE : FALSE;

            if(SetOrderMoveTargetPos(bObjectLanging) == TRUE)// 유닛의 착륙지점 좌표 계산
            {
                m_fLandingDist = m_vPos.y - m_vTargetPos.y;
                if(m_fLandingDist < 180.0f)
                {
                    // 인칭 변환 착륙장에서 1인칭으로 착륙시 3인칭전환 
                    if(g_pD3dApp->m_pCamera->m_bCamType == CAMERA_TYPE_FPS)
                    {
                        g_pD3dApp->m_pCamera->ChangeCamType();
                        m_bLandingOldCamtypeFPS = TRUE;
                    }

                    m_fAniFlag = 0;
                    m_bAniKeyBlock = FALSE;
                    m_fNumpad0keyTime = 2.0f;
                    SendPacket(T_FC_MOVE_LANDING);
                    if(IS_DT(m_myShuttleInfo.UnitKind))
                    {
                        ChangeUnitState(_LANDING);
                    }
                }
            }
        }
    }
//    SetShuttleFlight();        // 2005-11-16 by ispark
//    if(m_bFormationFlight)
    if(m_pClientParty->IsFormationFlightMove())
    {
        SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
        m_pClientParty->FormationFlightClear();
//        m_infoParty.bFormationFlyingPositionType = 0;
//        m_bFormationFlight = FALSE;
        g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PARTY_0001,COLOR_SYSTEM);//"편대비행에서 벗어났습니다."
    }
}


void CShuttleChild::UsedItemTick(float fElapsedTime)
{
    vector<USED_ITEM*>::iterator itUsedItem = m_vecUsedItem.begin();
    while(itUsedItem != m_vecUsedItem.end())
    {
        (*itUsedItem)->fOverHeatCheckTime += fElapsedTime*((*itUsedItem)->fOverHeatTime/OVERHEAT_REPAIR_TIME);
        if((*itUsedItem)->fOverHeatCheckTime >= (*itUsedItem)->fOverHeatTime )
        {
            util::del(*itUsedItem);
            itUsedItem = m_vecUsedItem.erase(itUsedItem);
        }
        else
        {
            itUsedItem++;
        }
    }
}

void CShuttleChild::PutUsedItem(CWeaponItemInfo* pWeaponItemInfo)
{
    USED_ITEM* pUsedItem = new USED_ITEM;
    pUsedItem->UniqueNumber = pWeaponItemInfo->GetItemGeneral()->UniqueNumber;
    pUsedItem->bIsOverHeat = pWeaponItemInfo->IsOverHeat();
    pUsedItem->fOverHeatTime = pWeaponItemInfo->GetOverHeatTime();
    pUsedItem->fOverHeatCheckTime = pWeaponItemInfo->GetOverHeatCheckTime();
    m_vecUsedItem.push_back(pUsedItem);
}

USED_ITEM* CShuttleChild::GetUsedItem(LONGLONG UniqueNumber)
{
    vector<USED_ITEM*>::iterator itUsedItem = m_vecUsedItem.begin();
    while(itUsedItem != m_vecUsedItem.end())
    {
        if((*itUsedItem)->UniqueNumber == UniqueNumber)
        {
            return *itUsedItem;
        }
        itUsedItem++;
    }
    return NULL;
}

void CShuttleChild::DeleteUsedItem()
{
    vector<USED_ITEM*>::iterator itUsedItem = m_vecUsedItem.begin();
    while(itUsedItem != m_vecUsedItem.end())
    {
        util::del(*itUsedItem);
        itUsedItem++;
    }
    m_vecUsedItem.clear();
}

void CShuttleChild::SetWearItemParamFactor( int nPos, ITEM* pITEM )
{
    // 2009-04-21 by bhsohn 아이템 DesParam추가    
//     if(pITEM)
//     {
//         m_wearItemParamFactor[nPos].DestParameter1 = pITEM->DestParameter1;
//         m_wearItemParamFactor[nPos].DestParameter2 = pITEM->DestParameter2;
//         m_wearItemParamFactor[nPos].DestParameter3 = pITEM->DestParameter3;
//         m_wearItemParamFactor[nPos].DestParameter4 = pITEM->DestParameter4;
//         m_wearItemParamFactor[nPos].ParameterValue1 = pITEM->ParameterValue1;
//         m_wearItemParamFactor[nPos].ParameterValue2 = pITEM->ParameterValue2;
//         m_wearItemParamFactor[nPos].ParameterValue3 = pITEM->ParameterValue3;
//         m_wearItemParamFactor[nPos].ParameterValue4 = pITEM->ParameterValue4;
//     }
//     else
//     {
//         m_wearItemParamFactor[nPos].DestParameter1 = 0;
//         m_wearItemParamFactor[nPos].DestParameter2 = 0;
//         m_wearItemParamFactor[nPos].DestParameter3 = 0;
//         m_wearItemParamFactor[nPos].DestParameter4 = 0;
//         m_wearItemParamFactor[nPos].ParameterValue1 = 0;
//         m_wearItemParamFactor[nPos].ParameterValue2 = 0;
//         m_wearItemParamFactor[nPos].ParameterValue3 = 0;
//         m_wearItemParamFactor[nPos].ParameterValue4 = 0;
//     }
    int nArrParamCnt = 0;
    if(pITEM)
    {
        for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
        {
            m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] = pITEM->ArrDestParameter[nArrParamCnt];            
            m_wearItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] = pITEM->ArrParameterValue[nArrParamCnt];            
        }
    }
     else
    {
        for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
        {
            m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] = 0;
            m_wearItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] = 0;
        }        
    }
    // end 2009-04-21 by bhsohn 아이템 DesParam추가
}

// 2010. 04. 21 by jskim 신규 럭키 머신 구현
void CShuttleChild::SetWearRareItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo)
{
    if(nPos != POS_CENTER)
    {
        return;
    }
    for(int nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
    {
        if(pItemInfo->GetPrefixRareInfo())
        {
            m_wearPreRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] =        pItemInfo->GetPrefixRareInfo()->DesParameter[nArrParamCnt];
            m_wearPreRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] =    pItemInfo->GetPrefixRareInfo()->ParameterValue[nArrParamCnt];
        }
        else
        {
            m_wearPreRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] =        0;
            m_wearPreRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] =    0;
        }
        
        if(pItemInfo->GetSuffixRareInfo())
        {
            m_wearSufRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] =        pItemInfo->GetSuffixRareInfo()->DesParameter[nArrParamCnt];            
            m_wearSufRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] =    pItemInfo->GetSuffixRareInfo()->ParameterValue[nArrParamCnt];
        }
        else
        {
            m_wearSufRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] =        0;
            m_wearSufRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] =    0;
        }
    }
}
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

// 2010. 04. 21 by jskim 신규 럭키 머신 구현
//void CShuttleChild::SetWearItemToCharacterParamFactor(int nPos, ITEM* pITEM)        // ITEM의 ParamFactor를 CHARACTER에 적용한다.
void CShuttleChild::SetWearItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo)
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
{
    // 2010. 04. 21 by jskim 신규 럭키 머신 구현
    //SetWearItemParamFactor( nPos, pITEM );
    SetWearItemParamFactor( nPos, pItemInfo->GetRealItemInfo() );
    //end 2010. 04. 21 by jskim 신규 럭키 머신 구현        
    // 2009-04-21 by bhsohn 아이템 DesParam추가    
//     SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nPos].DestParameter1, m_wearItemParamFactor[nPos].ParameterValue1 );
//     SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nPos].DestParameter2, m_wearItemParamFactor[nPos].ParameterValue2 );
//     SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nPos].DestParameter3, m_wearItemParamFactor[nPos].ParameterValue3 );
//     SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nPos].DestParameter4, m_wearItemParamFactor[nPos].ParameterValue4 );
// 
//     if( m_wearItemParamFactor[nPos].DestParameter1 == DES_SHOTNUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter1 == DES_MULTINUM_01 ||
//         m_wearItemParamFactor[nPos].DestParameter1 == DES_REATTACKTIME_01 ||
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_SHOTNUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_MULTINUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_REATTACKTIME_01 ||
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_SHOTNUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_MULTINUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_REATTACKTIME_01 ||
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_SHOTNUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_MULTINUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_REATTACKTIME_01)
//     {
//         FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
//     }
//     else if( m_wearItemParamFactor[nPos].DestParameter1 == DES_SHOTNUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter1 == DES_MULTINUM_02 ||
//         m_wearItemParamFactor[nPos].DestParameter1 == DES_REATTACKTIME_02 ||
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_SHOTNUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_MULTINUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_REATTACKTIME_02 ||
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_SHOTNUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_MULTINUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_REATTACKTIME_02 ||
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_SHOTNUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_MULTINUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_REATTACKTIME_02)
//     {
//         FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
//     }
    // 2010. 04. 21 by jskim 신규 럭키 머신 구현
    
    SetWearRareItemToCharacterParamFactor(nPos, pItemInfo);
    int nArrParamCnt = 0;
    for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
    {
        SetParamFactorDesParam(m_paramFactor, 
                                m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt], 
                                m_wearItemParamFactor[nPos].ArrParameterValue[nArrParamCnt]);

    }
    // 2010. 04. 21 by jskim 신규 럭키 머신 구현    
    nArrParamCnt=0;
    for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
    {
        // 접두 능력치 적용
        SetParamFactorDesParam(m_paramFactor, 
        m_wearPreRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt], 
        m_wearPreRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt]);

        // 접미 능력치 적용
        SetParamFactorDesParam(m_paramFactor, 
            m_wearSufRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt], 
            m_wearSufRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt]);
    }
    //end 2010. 04. 21 by jskim 신규 럭키 머신 구현
    BOOL bBreak = FALSE;
    for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
    {
        if(m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_SHOTNUM_01 || 
            m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_MULTINUM_01 ||
            m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_REATTACKTIME_01)
        {
            FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
            bBreak = TRUE;
            break;
        }        
    }    
    for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
    {
        if(bBreak)
        {
            break;
        }
        if(m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_SHOTNUM_02 || 
            m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_MULTINUM_02 ||
            m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_REATTACKTIME_02)
        {
            FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
            break;
        }
    }
    // end 2009-04-21 by bhsohn 아이템 DesParam추가    
}

void CShuttleChild::ReleaseWearItemToCharacterParamFactor(int nPos)    // ITEM의 ParamFactor를 CHARACTER에서 뺀다.
{
    // 2009-04-21 by bhsohn 아이템 DesParam추가    
//     ReleaseParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nPos].DestParameter1, m_wearItemParamFactor[nPos].ParameterValue1 );
//     ReleaseParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nPos].DestParameter2, m_wearItemParamFactor[nPos].ParameterValue2 );
//     ReleaseParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nPos].DestParameter3, m_wearItemParamFactor[nPos].ParameterValue3 );
//     ReleaseParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nPos].DestParameter4, m_wearItemParamFactor[nPos].ParameterValue4 );
// //     SetWearItemParamFactor( nPos, NULL );
//     if( m_wearItemParamFactor[nPos].DestParameter1 == DES_SHOTNUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter1 == DES_MULTINUM_01 ||
//         m_wearItemParamFactor[nPos].DestParameter1 == DES_REATTACKTIME_01 ||
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_SHOTNUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_MULTINUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_REATTACKTIME_01 ||
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_SHOTNUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_MULTINUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_REATTACKTIME_01 ||
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_SHOTNUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_MULTINUM_01 || 
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_REATTACKTIME_01)
//     {
//         FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
//     }
//     else if( m_wearItemParamFactor[nPos].DestParameter1 == DES_SHOTNUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter1 == DES_MULTINUM_02 ||
//         m_wearItemParamFactor[nPos].DestParameter1 == DES_REATTACKTIME_02 ||
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_SHOTNUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_MULTINUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter2 == DES_REATTACKTIME_02 ||
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_SHOTNUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_MULTINUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter3 == DES_REATTACKTIME_02 ||
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_SHOTNUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_MULTINUM_02 || 
//         m_wearItemParamFactor[nPos].DestParameter4 == DES_REATTACKTIME_02)
//     {
//         FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
//     }
    int nArrParamCnt = 0;
    for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
    {
        ReleaseParamFactorDesParam(m_paramFactor, 
                                        m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt], 
                                        m_wearItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] );        
        
    }
    // 2010. 04. 21 by jskim 신규 럭키 머신 구현
    if(nPos == POS_CENTER)
    {
        for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
        {
            // 접두 능력치 해제
            ReleaseParamFactorDesParam(m_paramFactor, 
                m_wearPreRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt], 
                m_wearPreRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt]);
     
             // 접미 능력치 해제
             ReleaseParamFactorDesParam(m_paramFactor, 
                m_wearSufRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt], 
                 m_wearSufRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt]);
        }

        for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
        {
            m_wearPreRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] =        0;
            m_wearPreRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] =    0;
            m_wearSufRareItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] =        0;
            m_wearSufRareItemParamFactor[nPos].ArrParameterValue[nArrParamCnt] =    0;
        }
    }
    //end 2010. 04. 21 by jskim 신규 럭키 머신 구현
    

    BOOL bBreak = FALSE;    
    for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
    {
        if( m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_SHOTNUM_01 || 
            m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_MULTINUM_01 ||
            m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_REATTACKTIME_01)
        {
            FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
            bBreak = TRUE;    
            break;
        }

    }
    for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM;nArrParamCnt++)
    {
        if(bBreak)
        {
            break;
        }
        if(m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_SHOTNUM_02 
            || m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_MULTINUM_02 
            || m_wearItemParamFactor[nPos].ArrDestParameter[nArrParamCnt] == DES_REATTACKTIME_02 )
        {
            FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
            break;
        }

    }    
    // end 2009-04-21 by bhsohn 아이템 DesParam추가    
    SetWearItemParamFactor( nPos, NULL );
}
// 2010. 04. 21 by jskim 신규 럭키 머신 구현
//void CShuttleChild::ChangeWearItemToCharacterParamFactor(int nPos, ITEM* pITEM)
void CShuttleChild::ChangeWearItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo)
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
{
    ReleaseWearItemToCharacterParamFactor( nPos );
    // 2010. 04. 21 by jskim 신규 럭키 머신 구현
    //SetWearItemToCharacterParamFactor( nPos, pITEM );
    SetWearItemToCharacterParamFactor( nPos, pItemInfo );
    //end 2010. 04. 21 by jskim 신규 럭키 머신 구현
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            FieldSocketCharacterSendParamfactorInRange(int nPos)
/// \brief        주변의 캐릭에서 ShuttleChild의 1형 ParamFactor를 전송한다.
/// \author        dhkwon
/// \date        2004-07-31 ~ 2004-07-31
/// \warning    ShotNum, ReattackTime만 적용된다.
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::FieldSocketCharacterSendParamfactorInRange(int nPos)
{
    MSG_FC_CHARACTER_SEND_PARAMFACTOR_IN_RANGE sMsg;
    sMsg.ItemPosition = nPos;
    g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_SEND_PARAMFACTOR_IN_RANGE, (char*)&sMsg, sizeof(sMsg) );
}


void CShuttleChild::SetShuttleSpeed()
{
    // 2005-11-26 by ispark
    // A기어이면 땅에서 다니면 속도 셋팅
    if(IS_DT(m_myShuttleInfo.UnitKind))
    {
        

        // 2009. 08. 11 by ckPark 편대 대형 스킬

        //m_fShuttleGroundSpeed = m_fShuttleGroundSpeedMax*(1.0f + m_paramFactor.pfm_SPEED);
        // 스킬 데스 파람값도 적용
        m_fShuttleGroundSpeed = ( m_fShuttleGroundSpeedMax + GetParamFactor_DesParam( m_paramFactor, DES_ENGINE_GROUND_SPEED_UP ) ) * ( 1.0f + GetParamFactor_DesParam( m_paramFactor, DES_SPEED ) );

        // end 2009. 08. 11 by ckPark 편대 대형 스킬


        if(m_fRealShuttleSpeedMax < 0.0f)
        {
            m_fRealShuttleSpeedMax = 0.0f;    // 최소 속력
        }
    }

    // 2005-12-03 by ispark, 엔진과 아머가 장착되어 있는지 판단
    // 둘중에 하개라도 없다면... 스피드 셋팅 에러
    ITEM_GENERAL* pItemEngine = m_pStoreData->FindItemInInventoryByWindowPos( POS_REAR );
    ITEM_GENERAL* pItemArmor = m_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );
    if(pItemEngine && pItemArmor)
    {
    //    m_fRealShuttleSpeedMax = m_fShuttleSpeedMax*(1.0f + m_paramFactor.pfm_SPEED)+1000.0f;


        // 2009. 08. 11 by ckPark 편대 대형 스킬

        //m_fRealShuttleSpeedMax = m_fShuttleSpeedMax*(1.0f + m_paramFactor.pfm_SPEED);
        // 스킬 데스 파람값도 적용
        m_fRealShuttleSpeedMax = ( m_fShuttleSpeedMax + GetParamFactor_DesParam( m_paramFactor, DES_ENGINE_MAX_SPEED_UP ) ) * ( 1.0f + GetParamFactor_DesParam( m_paramFactor, DES_SPEED ) );
        
        // end 2009. 08. 11 by ckPark 편대 대형 스킬
        
        
        if(m_fRealShuttleSpeedMax < 40.0f)
        {
            m_fRealShuttleSpeedMax = 40.0f;    // 최소 속력
        }

    //    m_fRealShuttleSpeedBoosterOn = m_fShuttleSpeedBoosterOn*(1.0f + m_paramFactor.pfm_SPEED)+1000.0f;


        // 2009. 08. 11 by ckPark 편대 대형 스킬

        //m_fRealShuttleSpeedBoosterOn = m_fShuttleSpeedBoosterOn*(1.0f + m_paramFactor.pfm_SPEED);
        // 스킬 데스 파람값도 적용
        m_fRealShuttleSpeedBoosterOn = ( m_fShuttleSpeedBoosterOn + GetParamFactor_DesParam( m_paramFactor, DES_ENGINE_BOOSTER_SPEED_UP ) ) * ( 1.0f + GetParamFactor_DesParam( m_paramFactor, DES_SPEED ) );

        // end 2009. 08. 11 by ckPark 편대 대형 스킬



        if(m_fRealShuttleSpeedBoosterOn < 50.0f)
        {
            m_fRealShuttleSpeedBoosterOn = 50.0f;    // 최소 부스터 속력
        }

        if(m_pSkill->GetSmartSpeedState())
        {
            m_fRealShuttleSpeedMin = m_fRealShuttleSpeedMax * (-1);
        }
        else
        {

            // 2009. 08. 11 by ckPark 편대 대형 스킬

            //m_fRealShuttleSpeedMin = m_fShuttleSpeedMin;
            // 스킬 데스 파람값도 적용
            m_fRealShuttleSpeedMin = ( m_fShuttleSpeedMin + GetParamFactor_DesParam( m_paramFactor, DES_ENGINE_MIN_SPEED_UP ) );

            // 0보다 적으면 0으로 셋팅(역삼각 형태 : 정지 비행)
            if( m_fRealShuttleSpeedMin <= 0.0f )
                m_fRealShuttleSpeedMin = 0.0f;

            // end 2009. 08. 11 by ckPark 편대 대형 스킬
        }
    }
    else
    {
        // 2005-12-03 by ispark, 엔진이나 아머가 미창작이므로 속도 최저로 그리고 유닛 정지
        m_fRealShuttleSpeedMax = 40.0f;
        m_fRealShuttleSpeedMin = 40.0f;
        m_fRealShuttleSpeedBoosterOn = 0.0f;
    }

    // 2005-12-02 by ispark, 하이퍼 무빙, 부스터는 부스터 쓸 때
    ITEM* pItemInfo = g_pStoreData->GetItemInfoFromItemGeneral(g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR ));
    if(pItemInfo)
        m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pItemInfo, &m_paramFactor);


    // 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
    // 홀드 상태면 모든 속도 0
    if( m_pSkill->GetMonsterHoldState() )
    {
        m_fRealShuttleSpeedMax = 0.0f;
        m_fRealShuttleSpeedMin = 0.0f;
        m_fRealShuttleSpeedBoosterOn = 0.0f;
    }
    // end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
}

void CShuttleChild::SetParamFactorDesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue)
{
    float fpm_SPEED = paramFactor.pfm_SPEED;
    SetParamFactor_DesParam( paramFactor, nDestParam, fDestValue);
    if(fpm_SPEED == paramFactor.pfm_SPEED)
    {
        SetShuttleSpeed();
    }
}

void CShuttleChild::ReleaseParamFactorDesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue)
{
    float fpm_SPEED = paramFactor.pfm_SPEED;
    ReleaseParamFactor_DesParam( paramFactor, nDestParam, fDestValue);
    if(fpm_SPEED == paramFactor.pfm_SPEED)
    {
        SetShuttleSpeed();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::SetGroundTargetPos()
/// \author        jschoi
/// \date        2004-10-07 ~ 2004-10-07
/// \warning    지상 폭격을 할 위치를 셋팅한다.
///
/// \param        
/// \return        m_vGroundTargetPos : 지상 폭격 위치
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetGroundTargetPos()
{
    D3DXMATRIX mat;
    D3DXVECTOR3 vVel,vTargetVel;
    D3DXVECTOR3 vGroundTargetCheckObject,vGroundTargetCheckGround;
    vVel = m_vVel;
    vVel.y = 0;
    D3DXVec3Normalize(&vVel,&vVel);

    // 초기 셋팅
    float fRange = m_pSecondaryWeapon->GetRealItemInfo()->Range;                        // 무기의 사거리
    m_vGroundTargetPos = m_vPos + vVel * fRange + D3DXVECTOR3(0,-1,0) * (fRange/2) ;    // 타켓의 위치
    float fTargetDistance = D3DXVec3Length(&(m_vGroundTargetPos - m_vPos));                // 타켓과의 거리
    vGroundTargetCheckObject = m_vGroundTargetPos;
    vGroundTargetCheckGround = m_vGroundTargetPos;
    vTargetVel = m_vGroundTargetPos - m_vPos;                                            // 타켓의 방향
    D3DXVec3Normalize(&vTargetVel,&vTargetVel);

    // 1. 타켓방향으로  오브젝트와 충돌 검사
    D3DXMatrixLookAtLH(&mat,&m_vPos,&(m_vPos + vTargetVel),&D3DXVECTOR3(0,1,0));    // 셔틀 앞방향
    COLLISION_RESULT collResult = g_pScene->m_pObjectRender->CheckCollMesh(mat,m_vPos, FALSE);
    if(collResult.fDist < fTargetDistance) 
    { // 2형 무기의 사거리 안쪽에 오브젝트가 있다면 오브젝트에 충돌
        vGroundTargetCheckObject = m_vPos + vTargetVel*collResult.fDist;    
    }

    // 2. 타켓방향으로 지형과 충돌검사
    float fTempDistance = 0.0f;
    D3DXVECTOR3 vTempPos;
    while (fTempDistance < fTargetDistance)    // 지형 및 물과 충돌 검사
    {
        vTempPos = m_vPos + vTargetVel * fTempDistance;
        if(g_pGround->CheckHeightMap(vTempPos) > vTempPos.y)
        {    // 지형 및 물과 충돌 했다.
            vGroundTargetCheckGround = vTempPos;
            break;
        }
        fTempDistance += TILE_SIZE;
    }

    if(D3DXVec3Length(&(vGroundTargetCheckObject - m_vPos)) > D3DXVec3Length(&(vGroundTargetCheckGround - m_vPos))) 
    { // 지형과 먼저 충돌한다.
        m_vGroundTargetPos = vGroundTargetCheckGround;
    }
    else
    { // 오브젝트와 먼저 충돌한다.
        m_vGroundTargetPos = vGroundTargetCheckObject;
    }
    g_pGameMain->m_pInfSkill->SetGroundTarget(m_vGroundTargetPos);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::SetAirGroundTargetPos()
/// \brief        공중 폭격
/// \author        ispark
/// \date        2006-12-01 ~ 2006-12-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetAirGroundTargetPos()
{
    // 2009. 07. 21 by ckPark 공중폭격중에 레이더해제시 튕기는 문제
    if( !m_pRadarItemInfo )
    {
        for( vector<CSkillInfo*>::iterator it = m_pSkill->m_vecUsingSkill.begin();
             it != m_pSkill->m_vecUsingSkill.end();
             ++it )
        {
            if( SKILL_BASE_NUM((*it)->ItemNum) == BGEAR_SKILL_BASENUM_AIRBOMBINGMODE )
            {
                g_pGameMain->m_pCharacterInfo->SendUseSkill( *it );
                break;
            }
        }

        return;
    }
    // end 2009. 07. 21 by ckPark 공중폭격중에 레이더해제시 튕기는 문제
    
    D3DXMATRIX mat;
    D3DXVECTOR3 vVel,vTargetVel;
    D3DXVECTOR3 vGroundTargetCheckObject,vGroundTargetCheckGround;
    vVel = m_vVel;
    D3DXVec3Normalize(&vVel,&vVel);

    // 초기 셋팅
    // 레이더 2형 무기 사거리
    float fRange = CAtumSJ::GetSecondaryRadarRange(m_pRadarItemInfo->ItemInfo, &m_paramFactor);
    m_vGroundTargetPos = m_vPos + vVel * fRange;                                        // 타켓의 위치
    float fTargetDistance = D3DXVec3Length(&(m_vGroundTargetPos - m_vPos));                // 타켓과의 거리
    vGroundTargetCheckObject = m_vGroundTargetPos;
    vGroundTargetCheckGround = m_vGroundTargetPos;
    vTargetVel = m_vGroundTargetPos - m_vPos;                                            // 타켓의 방향
    D3DXVec3Normalize(&vTargetVel,&vTargetVel);

    // 1. 타켓방향으로  오브젝트와 충돌 검사
    D3DXMatrixLookAtLH(&mat,&m_vPos,&(m_vPos + vTargetVel),&D3DXVECTOR3(0,1,0));    // 셔틀 앞방향
    COLLISION_RESULT collResult = g_pScene->m_pObjectRender->CheckCollMesh(mat,m_vPos, FALSE);
    if(collResult.fDist < fTargetDistance) 
    { // 2형 무기의 사거리 안쪽에 오브젝트가 있다면 오브젝트에 충돌
        vGroundTargetCheckObject = m_vPos + vTargetVel*collResult.fDist;    
    }

    // 2. 타켓방향으로 지형과 충돌검사
    float fTempDistance = 0.0f;
    D3DXVECTOR3 vTempPos;
    while (fTempDistance < fTargetDistance)    // 지형 및 물과 충돌 검사
    {
        vTempPos = m_vPos + vTargetVel * fTempDistance;
        if(g_pGround->CheckHeightMap(vTempPos) > vTempPos.y)
        {    // 지형 및 물과 충돌 했다.
            vGroundTargetCheckGround = vTempPos;
            break;
        }
        fTempDistance += TILE_SIZE;
    }

    if(D3DXVec3Length(&(vGroundTargetCheckObject - m_vPos)) > D3DXVec3Length(&(vGroundTargetCheckGround - m_vPos))) 
    { // 지형과 먼저 충돌한다.
        m_vGroundTargetPos = vGroundTargetCheckGround;
    }
    else
    { // 오브젝트와 먼저 충돌한다.
        m_vGroundTargetPos = vGroundTargetCheckObject;
    }
    g_pGameMain->m_pInfSkill->SetAirGroundTarget(m_vGroundTargetPos);
}

void CShuttleChild::SendMoveWeaponVel()
{
    // 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화
    DWORD dwTime = g_pD3dApp->GetCurrentGameTime();        
    if(dwTime < m_dwCheckTimeSendMoveVel)        
    {        
        m_dwCheckTimeSendMoveVel = dwTime;
        return;
    }    
    else if((dwTime-m_dwCheckTimeSendMoveVel) < SEND_MOVE_VEL_CHECK_TIME)
    {
        return;
    }
    
    m_dwCheckTimeSendMoveVel = dwTime;
    // end 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화

    MSG_FC_MOVE_WEAPON_VEL sMsg;
    sMsg.WeaponVel = m_vWeaponVel;
    sMsg.PetLeftVel = m_vPetLeftVel;
    sMsg.PetRightVel = m_vPetRightVel;

    g_pFieldWinSocket->SendMsg( T_FC_MOVE_WEAPON_VEL, (char*)&sMsg, sizeof(sMsg) );
}

void CShuttleChild::CheckOnObject()
{
    D3DXMATRIX mat;
    D3DXVECTOR3 vSide(0,0,1),vUp(0,1,0);
    D3DXMatrixLookAtLH(&mat,&m_vPos,&(m_vPos - m_vUp),&vSide);        // 아래
        
    if(    g_pScene &&
        g_pScene->m_pObjectRender)
    {    
        COLLISION_RESULT collResult = g_pScene->m_pObjectRender->CheckCollMesh(mat,m_vPos, FALSE);
        if(collResult.fDist != DEFAULT_COLLISION_DISTANCE)
        {
            m_bOnObject = TRUE;
        }
        else
        {
             m_bOnObject = FALSE;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::PatternWarpIn()
/// \brief        패턴 워프 오브젝트에 진입시 패턴을 생성한다.
/// \author        jschoi
/// \date        2004-11-27 ~ 2004-11-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::PatternWarpIn()
{
    CObjectChild* pObj = g_pScene->FindEventObjectByIndex(m_pCurrentObjEvent->m_vPos, m_pCurrentObjEvent->m_nNextEventIndex);
    if(pObj)
    {
        D3DXVECTOR3 vVel,vUp;
        vVel = m_vPos - pObj->m_vPos;
        vUp.x = m_mMatrix._21;
        vUp.y = m_mMatrix._22;
        vUp.z = m_mMatrix._23;
        if(vVel == vUp)
        {// 매트릭스가 깨지지 않도록
            m_mMatrix._21 += 0.001f;
            m_mMatrix._22 -= 0.001f;
            m_mMatrix._23 += 0.002f;
        }
        m_mMatrix._31 = vVel.x;
        m_mMatrix._32 = vVel.y;
        m_mMatrix._33 = vVel.z;
        if(InitCinemaUnit(PATTERN_UNIT_WARP_IN, TRUE) == TRUE)
        {
            // 워프게이트 이펙트
            CObjectChild* pWarpgate = g_pScene->FindWarpGateByPosition( m_vPos );
            if( pWarpgate )
            {
                // 2008-06-24 by dgwoo 워프 게이트 추가.
                if(pWarpgate->m_pObjectInfo->Code == WARP_GATE_OBJECT_NUM_2||pWarpgate->m_pObjectInfo->Code == WARP_GATE_OBJECT_NUM_3)// 2012-06-13 by jhahn 워프 게이트 추가2.
                {
                    g_pD3dApp->m_pSound->PlayD3DSound(SOUND_WARP_2, pWarpgate->m_vPos, FALSE);
                }
                else
                {
                    g_pD3dApp->m_pSound->PlayD3DSound(SOUND_WARP, pWarpgate->m_vPos, FALSE);
                }
                pWarpgate->ChangeBodycondition( BODYCON_WARP_MASK );
                
                // 주변의 Enemy들에게 워프게이트 바디컨티션 Change를 전송한다.
                MSG_FC_OBJECT_CHANGE_BODYCONDITION sMsg;
                sMsg.AVec3Position = pWarpgate->m_vPos;
                sMsg.bodyCondition = BODYCON_WARP_MASK;
                sMsg.nObjectCodeNum = pWarpgate->m_nCode;
                g_pFieldWinSocket->SendMsg(T_FC_OBJECT_CHANGE_BODYCONDITION, (char*)&sMsg, sizeof(sMsg));
            }
            m_bSkillMoveIsUse = TRUE;
            m_nCurrentPatternNumber = PATTERN_UNIT_WARP_IN;
            InitCinemaCamera(PATTERN_CAMERA_WARP_IN);
        }
        else
        {
            DBGOUT("[PATTERN_UNIT_WARP_IN] : Pattern File Non Exsistent.\n");
        }
    }
    m_fShuttleSpeed = m_fShuttleSpeedBoosterOn;
    if(!COMPARE_BODYCON_BIT(m_pCharacterInfo->m_nCurrentBodyCondition,BODYCON_BOOSTER3_MASK))
    {
        ChangeBodyConditionFromServer(BODYCON_BOOSTER3_MASK);
    }

    m_nBooster = BOOSTER_TYPE_MIN;                        // 2006-09-27 by ispark, 부스터 게이지 달지 않게 하기
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::PatternWarpOut()
/// \brief        워프게이트 이벤트 오브젝트에서 워프가 완료되면, 워프아웃 패턴을 생성한다.
/// \author        jschoi
/// \date        2004-11-27 ~ 2004-11-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::PatternWarpOut(BOOL bOtherMap)
{
    // 2004-11-26 by jschoi - 위프아웃 시 패턴 적용
    CObjectChild * pCurrentObj = g_pScene->FindEventObjectByTypeAndPosition(EVENT_TYPE_WARP,m_vPos);
    if(pCurrentObj == NULL)
    {
        pCurrentObj = g_pScene->FindEventObjectByTypeAndPosition(EVENT_TYPE_WARP_TARGET,m_vPos);
    }
    if(pCurrentObj)
    {
        CObjectChild* pObjTarget = g_pScene->FindEventObjectByIndex(pCurrentObj->m_vPos, pCurrentObj->m_nNextEventIndex);
        if(pObjTarget)
        {
            if(g_pD3dApp->m_pEffectList)
            {
                CAppEffectData *pData = new CAppEffectData(pCurrentObj,RC_EFF_OTHER_WARP,D3DXVECTOR3(0,0,0));
                g_pD3dApp->m_pEffectList->AddChild(pData);
            }

            // 2006-04-25 by ispark, 이벤트 오브젝트 Up벡터를 가지고 셋팅
//            D3DXVECTOR3 vVel,vUp(0,1,0);
            D3DXVECTOR3 vVel,vUp = pCurrentObj->m_vUp;
            vVel = m_vPos - pObjTarget->m_vPos;
            m_fWarpOutDistance = D3DXVec3Length(&vVel);
            m_vWarpOutPosition = pCurrentObj->m_vPos;
            D3DXVec3Normalize(&vVel,&vVel);
            if(vVel == vUp)
            {
                vUp.x += 0.001f;
                vUp.y -= 0.002f;
                vUp.z += 0.001f;
                D3DXVec3Normalize(&vUp,&vUp);
            }
            m_mMatrix._41 = m_vPos.x;
            m_mMatrix._42 = m_vPos.y;
            m_mMatrix._43 = m_vPos.z;
            m_mMatrix._31 = vVel.x;
            m_mMatrix._32 = vVel.y;
            m_mMatrix._33 = vVel.z;
            m_mMatrix._21 = vUp.x;
            m_mMatrix._22 = vUp.y;
            m_mMatrix._23 = vUp.z;
            g_pCamera->Init(m_vPos - 10.0f*vVel,vVel, 3.14f/13.0f, 60.0f);
            if(InitCinemaUnit(PATTERN_UNIT_WARP_OUT, TRUE) == TRUE)
            {
                // 워프게이트 이펙트
                CObjectChild* pWarpgate = g_pScene->FindWarpGateByPosition( m_vPos );
                if( pWarpgate )
                {
                    // 2008-06-24 by dgwoo 워프 게이트 추가.
                    if(pWarpgate->m_pObjectInfo->Code == WARP_GATE_OBJECT_NUM_2||pWarpgate->m_pObjectInfo->Code == WARP_GATE_OBJECT_NUM_3)// 2012-06-13 by jhahn 워프 게이트 추가2.
                    {
                        g_pD3dApp->m_pSound->PlayD3DSound(SOUND_WARP_2, pWarpgate->m_vPos, FALSE);
                    }
                    else
                    {
                        g_pD3dApp->m_pSound->PlayD3DSound(SOUND_WARP, pWarpgate->m_vPos, FALSE);
                    }
                    pWarpgate->ChangeBodycondition( BODYCON_WARP_MASK );
                    
                    // 주변의 Enemy들에게 워프게이트 바디컨티션 Change를 전송한다.
                    MSG_FC_OBJECT_CHANGE_BODYCONDITION sMsg;
                    sMsg.AVec3Position = pWarpgate->m_vPos;
                    sMsg.bodyCondition = BODYCON_WARP_MASK;
                    sMsg.nObjectCodeNum = pWarpgate->m_nCode;
                    g_pFieldWinSocket->SendMsg(T_FC_OBJECT_CHANGE_BODYCONDITION, (char*)&sMsg, sizeof(sMsg));
                }

                m_bSkillMoveIsUse = TRUE;
                m_nCurrentPatternNumber = PATTERN_UNIT_WARP_OUT;
                InitCinemaCamera(PATTERN_CAMERA_WARP_OUT);
            }
            else
            {
                DBGOUT("[PATTERN_UNIT_WARP_OUT] : Pattern File Non Exsistent.\n");
            }
        }
        
        if(bOtherMap)
        {
            // 2009. 09. 03 by ckPark 로봇기어 롤링하면서 워프 통과시 이펙트 남아있는 문제
            if( COMPARE_BODYCON_BIT( m_pCharacterInfo->m_nCurrentBodyCondition, BODYCON_ROLLING_MASK ) )
                g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_ROLLING_MASK, FALSE );
            // end 2009. 09. 03 by ckPark 로봇기어 롤링하면서 워프 통과시 이펙트 남아있는 문제
            
            m_nShuttleStopState = 0;
            m_nBooster = BOOSTER_TYPE_MAX;
            ChangeSingleBodyCondition(BODYCON_BOOSTER2_MASK);
            ChangeBodyConditionFromServer(BODYCON_BOOSTER3_MASK);
            m_nBooster = BOOSTER_TYPE_MIN;                            // 2006-09-27 by ispark, 부스터 게이지 달지 않게 하기
        }
        m_fShuttleSpeed = m_fShuttleSpeedBoosterOn;        
    }
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CheckPickingTarget(CUnitData* pTarget)
/// \brief        마우스 포인트에 대한 pTarget의 메시 Picking을 수행한다.
/// \author        jschoi
/// \date        2005-03-24 ~ 2005-03-24
/// \warning    
///
/// \param        
/// \return        TRUE : 충돌한다.(Picked)
///                FALSE : Pick 실패
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::CheckPickingTarget(CUnitData* pTarget)
{
    if(pTarget == NULL)
        return FALSE;

    CSkinnedMesh* pMesh = NULL;
    if(pTarget->m_dwPartType == _ENEMY)
    {// Enemy
        CEnemyData* pEnemy = (CEnemyData*)pTarget;
        int nUnitNum = pTarget->GetUnitNum();
        map<int,CSkinnedMesh*>::iterator itEnemy = pEnemy->m_pRender->m_mapSkinnedMesh.find(nUnitNum);        
        if(itEnemy != pEnemy->m_pRender->m_mapSkinnedMesh.end())
        {
            pMesh = itEnemy->second;
        }
    }
    else if(pTarget->m_dwPartType == _MONSTER)
    {// Monster
        CMonsterData* pMon = (CMonsterData*)pTarget;
        if(pMon->m_pMonMesh)
        {
            pMesh = pMon->m_pMonMesh;
        }
    }

    if(pMesh)
    {
        D3DXMATRIX matProj,matView,matWorld,mat;
        g_pD3dDev->GetTransform(D3DTS_PROJECTION,&matProj);
        g_pD3dDev->GetTransform(D3DTS_VIEW,&matView);
        g_pD3dDev->GetTransform(D3DTS_WORLD,&matWorld);

        POINT MousePt;
        GetCursorPos(&MousePt);
        ScreenToClient(g_pD3dApp->GetHwnd(), &MousePt);
        CheckMouseReverse(&MousePt);

        D3DXVECTOR3 v,vDir,vPos,vUp;
        v.x =  (((2.0f*MousePt.x)/g_pD3dApp->GetBackBufferDesc().Width)-1)/matProj._11;
        v.y = -(((2.0f*MousePt.y)/g_pD3dApp->GetBackBufferDesc().Height)-1)/matProj._22;
        v.z = 1.0f;

        D3DXMatrixInverse(&matView,NULL,&matView);

        vDir.x = v.x*matView._11 + v.y*matView._21 + v.z*matView._31;
        vDir.y = v.x*matView._12 + v.y*matView._22 + v.z*matView._32;
        vDir.z = v.x*matView._13 + v.y*matView._23 + v.z*matView._33;

        D3DXVec3Normalize(&vDir,&vDir);

        vPos.x = matView._41;
        vPos.y = matView._42;
        vPos.z = matView._43;

        vUp.x = 0.0f;
        vUp.y = 1.0f;
        vUp.z = 0.0f;

        D3DXMatrixLookAtLH(&mat,&vPos,&(vPos + vDir),&vUp);
        
        float fRadius = pMesh->m_fRadius;

//        pMesh->Tick(pTarget->m_fCurrentTime);
//        pMesh->SetWorldMatrix(pTarget->m_mMatrix);

        float fDistance = pMesh->CheckCollision(mat,vPos,DEFAULT_COLLISION_DISTANCE,TRUE,FALSE).fDist;
        if(fDistance < DEFAULT_COLLISION_DISTANCE)
        {
            DBGOUT("fDistance : %f  \n",fDistance);
            return TRUE;
        }
    }

    return FALSE;
}

void CShuttleChild::SetEffectPos(float fElapsedTime)
{
    // 좌우 날개 엔진 위치 설정
    D3DXVECTOR3 vSide;
    vSide.x = m_mMatrix._11;
    vSide.y = m_mMatrix._12;
    vSide.z = m_mMatrix._13;

    if(IS_DT(m_myShuttleInfo.UnitKind) && !m_bIsAir)
    {//4.0f


        // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

////    D3DXVec3TransformCoord(&m_vLWBackPos, &m_PrimaryAttack.vSideBackPos, &m_mMatrix);
//         D3DXVec3TransformCoord(&m_vLWBackPos, &m_PrimaryAttack.vSidePos, &m_mMatrix);
//         D3DXVECTOR3 vTempPrimary = m_PrimaryAttack.vSideBackPos - m_PrimaryAttack.vSidePos;
//         float fLen = D3DXVec3Length(&vTempPrimary);
//         D3DXVec3Normalize(&m_vWeaponVel,&m_vWeaponVel);
//         m_vLWPos = m_vLWBackPos + fLen*m_vWeaponVel;
// 
//         //D3DXVECTOR3 vPrimaryRightPos = D3DXVECTOR3(-m_vPrimarySidePos.x, m_vPrimarySidePos.y, m_vPrimarySidePos.z);
//         D3DXVECTOR3 vPrimaryRightBackPos = D3DXVECTOR3(-m_PrimaryAttack.vSideBackPos.x, m_PrimaryAttack.vSideBackPos.y, m_PrimaryAttack.vSideBackPos.z);
//         D3DXVECTOR3 vRWBackPos;
//         D3DXVec3TransformCoord(&vRWBackPos, &vPrimaryRightBackPos, &m_mMatrix);
//         m_vRWPos = vRWBackPos + fLen*m_vWeaponVel;

        // 발사 위치의 vSideBackPos에서 가져오나
        // y축은 아머 위치에서 가져온다(vSidePos)
        D3DXVECTOR3 vPrimaryLeftBackPos = m_PrimaryAttack.vSideBackPos;
        vPrimaryLeftBackPos.y = m_PrimaryAttack.vSidePos.y;
        
        // 왼쪽 발사 위치
        D3DXVec3TransformCoord( &m_vLWBackPos, &vPrimaryLeftBackPos, &m_mMatrix );
        D3DXVECTOR3 vTempPrimary = vPrimaryLeftBackPos - m_PrimaryAttack.vSidePos;
        
        float fLen = D3DXVec3Length( &vTempPrimary );
        D3DXVec3Normalize( &m_vWeaponVel,&m_vWeaponVel );
        m_vLWPos = m_vLWBackPos + fLen * m_vWeaponVel;
        
        // 오른쪽 발사 위치
        D3DXVECTOR3 vPrimaryRightBackPos = vPrimaryLeftBackPos;
        vPrimaryRightBackPos.x = -vPrimaryRightBackPos.x;
        
        D3DXVECTOR3 vRWBackPos;
        D3DXVec3TransformCoord( &vRWBackPos, &vPrimaryRightBackPos, &m_mMatrix) ;
        m_vRWPos = vRWBackPos + fLen * m_vWeaponVel;

        // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
        //m_vRWPos += 3.0f*m_vWeaponVel;
        D3DXVec3TransformCoord(&m_vLWSecondaryPos, &m_SecondaryAttack.vSidePos, &m_mMatrix);
        D3DXVECTOR3 vSecondaryRightPos = D3DXVECTOR3(-m_SecondaryAttack.vSidePos.x, m_SecondaryAttack.vSidePos.y, m_SecondaryAttack.vSidePos.z);
        D3DXVec3TransformCoord(&m_vRWSecondaryPos, &vSecondaryRightPos, &m_mMatrix);

        // 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치
        D3DXVec3TransformCoord(&m_vPetLeftPos, &m_PetAttack.vSidePos, &m_mMatrix);
        D3DXVECTOR3 vPetRightPos = D3DXVECTOR3(-m_PetAttack.vSidePos.x,
                                                m_PetAttack.vSidePos.y, 
                                                m_PetAttack.vSidePos.z);
        D3DXVec3TransformCoord(&m_vPetRightPos, &vPetRightPos, &m_mMatrix);
        // end 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치
    }

    // 스크린상의 좌표를 구한다.
    g_pD3dApp->CalcObjectSourceScreenCoords(m_vPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
        m_nObjScreenX, m_nObjScreenY,m_nObjScreenW);
    // 카메라로부터의 거리
    m_fDistanceCamera = D3DXVec3Length(&(m_vPos - g_pD3dApp->m_pCamera->GetEyePt()));
    // 그림자 위치 설정
    CheckShadowPos();
    m_bCollObjectCheck = 0;                // 초기화
    m_bCollMonsterCheck = 0;
    m_bCollShuttleCheck = 0;
    m_bCollAttackCheck = 0;            // 초기화
    // effect matrix & ticking
    if(m_pCharacterInfo)
    {
        m_pCharacterInfo->SetAllBodyConditionMatrix( m_mMatrix );
        m_pCharacterInfo->Tick(fElapsedTime);
    }

}


///////////////////////////////////////////////////////////////////////////////
/// \fn            RollingSystem()
/// \brief        롤링 시스템(키를 검색후 그에 맞는 애니메이션 하드코딩)
///                회전값을 리턴
///                (단, 0은 이동이 없을 때 쓴다.)
/// \author        ispark
/// \date        2005-07-05 ~ 2005-07-05
/// \warning    
///
/// \param        void
/// \return        float
///////////////////////////////////////////////////////////////////////////////
float CShuttleChild::RollingSystem(float fElapsedTime)
{
    // A키 좌측 롤링
    if(m_bKeyDownA)
    {
        m_fRollTime += fElapsedTime;
        if(m_fRollTime >= ROLLING_USE_TIME)                    // 롤링 지속시간
        {
            m_bKeyDownA = FALSE;

            m_bMouseMoveLock = FALSE;
            m_bRollUsed = FALSE;
            m_fCurrentSideRate = 0.0f;
            if(g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetChapter() == L3_2 && g_pTutorial->GetLesson() >= L3)
            {
                g_pTutorial->m_Lesson3State.bLRolling = TRUE;
            }

            // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_ROLLING_MASK, FALSE );
            // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

            return 0.0f;
        }

    
//        if(80 <= D3DXVec3Length(&(m_vRollStart - m_vPos)))
        {
            m_fRollAngle += 360 * fElapsedTime;

            if(D3DXToRadian(360) <= D3DXToRadian(m_fRollAngle))
            {
                m_bKeyDownA = FALSE;

                m_bMouseMoveLock = FALSE;
                m_bRollUsed = FALSE;
                m_fCurrentSideRate = 0.0f;
                if(g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetChapter() == L3_2 && g_pTutorial->GetLesson() >= L3)
                {
                    g_pTutorial->m_Lesson3State.bLRolling = TRUE;
                }
    
                // 2006-07-14 by ispark, 롤링시 편대 비행 형태 원상복귀
                if(m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER &&
                    m_pClientParty->IsFormationFlight())
                {
                    g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(m_byFormationFlightType);
                }

                // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
                g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_ROLLING_MASK, FALSE );
                // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

                return 0.0f;
            }
            else
            {
                return D3DXToRadian(m_fRollAngle);
            }
        }
//        else
//        {
//            return D3DXToRadian(m_fRollAngle);
//        }
    }
    
    // D키 우측 롤링
    if(m_bKeyDownD)
    {
        m_fRollTime += fElapsedTime;
        if(m_fRollTime >= ROLLING_USE_TIME)                    // 롤링 지속 시간
        {
            m_bKeyDownD = FALSE;

            m_bMouseMoveLock = FALSE;
            m_bRollUsed = FALSE;
            m_fCurrentSideRate = 0.0f;
            if(g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetChapter() == L3_2 && g_pTutorial->GetLesson() >= L3)
            {
                g_pTutorial->m_Lesson3State.bRRolling = TRUE;
            }

            // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_ROLLING_MASK, FALSE );
            // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

            return 0.0f;
        }

    
//        if(80 <= D3DXVec3Length(&(m_vRollStart - m_vPos)))
        {
            m_fRollAngle += 360 * fElapsedTime;

            if(D3DXToRadian(360) <= D3DXToRadian(m_fRollAngle))
            {
                m_bKeyDownD = FALSE;

                m_bMouseMoveLock = FALSE;
                m_bRollUsed = FALSE;
                m_fCurrentSideRate = 0.0f;
                if(g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetChapter() == L3_2 && g_pTutorial->GetLesson() >= L3)
                {
                    g_pTutorial->m_Lesson3State.bRRolling = TRUE;
                }
                
                // 2006-07-14 by ispark, 롤링시 편대 비행 형태 원상복귀
                if(m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER &&
                    m_pClientParty->IsFormationFlight())
                {
                    g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(m_byFormationFlightType);
                }
                
                // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
                g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_ROLLING_MASK, FALSE );
                // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

                return 0.0f;
            }
            else
            {
                return -D3DXToRadian(m_fRollAngle);
            }
        }
//        else
//        {
//            return -D3DXToRadian(m_fRollAngle);
//         }
    }

    return 0.0f;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CheckRollKeyDown()
/// \brief        더블클릭으로 롤링으로 변경
/// \author        ispark
/// \date        2005-07-05 ~ 2005-07-05, 2005-09-26
/// \warning    
///
/// \param        void
/// \return        void
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckRollKeyDown(float fElapsedTime)
{
    // 더블클릭 시간
    m_fADoubleKeyTime += fElapsedTime;
    m_fDDoubleKeyTime += fElapsedTime;
    
    // 2008-11-13 by bhsohn 롤링 변수 오버 플로우 날수 있는 소지 제거
    // 롤링 변수 오버 플로우 날수 있는 소지 제거
    if(m_fADoubleKeyTime > 100000.0f)
    {
        m_fADoubleKeyTime = 0.5f;
    }
    if(m_fDDoubleKeyTime > 100000.0f)
    {
        m_fDDoubleKeyTime = 0.5f;
    }
    // 어느 하나라도 TRUE이면 함수를 쓰지 않는다. 
    if(m_bKeyDownA || m_bKeyDownD)
    {
        return;
    }
    else if(g_pD3dApp->m_bChatMode ||                                        // 채팅 모드
        // 2008-11-13 by bhsohn 조이스틱 작업
            //g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SPACE) ||            // 부스터 사용 
            g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_SPACE) ||            // 부스터 사용 
            g_pD3dApp->m_bFixUnitDirMode ||                                    // 유닛 고정시
            g_pGameMain->m_pSystem->m_bQuitGameMessage ||                    // 메뉴사용시 유닛정지 옵션 체크
            g_pD3dApp->m_bMessageBox ||                                        // 메세지 박스 떴을 때
            m_dwState != _NORMAL  ||                                        // 비행중이 아니라면 
            m_bUnitStop == TRUE ||                                            // 2005-08-08 by ispark 유닛정지시 사용 못함
            m_bPenaltyUnitStop == TRUE)                                        // 2005-08-10 by ispark 페널티시 사용 금지
    {
        return;
    }

    if(m_bKeyDownACheck || m_bKeyDownDCheck)
    {
        // 이미 보낸 메세지 이므로 더 이상 보내지 않는다.
        m_fRollMessageTime += fElapsedTime;

        // 2초 후에는 보낼 수 있다.
        if(2.0f >= m_fRollMessageTime)
            return;
    
        m_fRollMessageTime = 0.0f;
        m_bKeyDownACheck = FALSE;
        m_bKeyDownDCheck = FALSE;
    }



    // 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
    // 홀드가 걸렸을때는 롤링 못함
    if( m_pSkill->IsExistDesParamSKill( DES_SKILL_MON_HOLD ) )
        return;
    // end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현



    // 더블클릭 끝 체크
    // A키 좌측 롤링
    if(m_bKeyDownADouble == TRUE)
    {
        m_bKeyDownACheck = TRUE;
        m_bKeyDownADouble = FALSE;

        // 롤링 패킷
        INIT_MSG_WITH_BUFFER(MSG_FC_MOVE_ROLLING, T_FC_MOVE_ROLLING, pSRolling, SendBuf);
        pSRolling->PositionAVec3    = m_vPos;
        pSRolling->TargetAVec3        = m_vVel * 1000.0f;
        D3DXVECTOR3 vUp;
        vUp.x = m_mMatrix._21;
        vUp.y = m_mMatrix._22;
        vUp.z = m_mMatrix._23;
        pSRolling->UpAVec3            = vUp * 1000.0f;
        pSRolling->byLeftDirectionFlag    = TRUE;
        g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_MOVE_ROLLING));
//        DBGOUT("패킷 보냈엉~~~\n");
        return;
    }

    // D키 좌측 롤링
    if(m_bKeyDownDDouble == TRUE)
    {
        m_bKeyDownDCheck = TRUE;
        m_bKeyDownDDouble = FALSE;

        // 롤링 패킷
        INIT_MSG_WITH_BUFFER(MSG_FC_MOVE_ROLLING, T_FC_MOVE_ROLLING, pSRolling, SendBuf);
        pSRolling->PositionAVec3    = m_vPos;
        pSRolling->TargetAVec3        = m_vVel * 1000.0f;
        D3DXVECTOR3 vUp;
        vUp.x = m_mMatrix._21;
        vUp.y = m_mMatrix._22;
        vUp.z = m_mMatrix._23;
        pSRolling->UpAVec3            = vUp * 1000.0f;
        pSRolling->byLeftDirectionFlag    = FALSE;
        g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_MOVE_ROLLING));
//        DBGOUT("패킷 보냈엉~~~\n");
        return;
    }

}


///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        ispark
/// \date        2005-07-07 ~ 2005-07-07
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckRoll(BYTE byDir)
{
//    DBGOUT("패킷 받았엉~~~\n");
    if(byDir)
    {
        m_bKeyDownA = TRUE;
    }
    else
    {
        m_bKeyDownD = TRUE;
    }

    m_fRollTime = 0.0f;    
    m_bKeyDownACheck = m_bKeyDownDCheck = FALSE;
    m_fRollMessageTime = 0.0f;
    m_fRollAngle = 0.0f;
    m_vRollStart = m_vRollOldPos = m_vPos;
    m_bMouseMoveLock = TRUE;
    m_fRollDownSpeed = m_fShuttleSpeed * (float)(2.0f / 3.0f);
    m_bRollUsed = TRUE;

    if(m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER &&
        m_pClientParty->IsFormationFlight())
    {
        g_pShuttleChild->m_pClientParty->ISendPartyChangeFlightFormation(FLIGHT_FORM_NONE);
    }
    m_byFormationFlightType = g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingType;

    // 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
    // 로봇아머일 경우
    if( IsRobotArmor() )
    {
        // 부스터 애니메이션과 롤링 바디컨션을 다 끝낸뒤, 새로 시작한다
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_BOOSTER1_MASK, FALSE );
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_BOOSTER2_MASK, FALSE );
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_BOOSTER3_MASK, FALSE );
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_BOOSTER4_MASK, FALSE );
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_BOOSTER5_MASK, FALSE );
        g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_ROLLING_MASK, FALSE );
    }
    g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_ROLLING_MASK, TRUE );
    // end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::InitRolling()
/// \brief        롤링 초기화
/// \author        ispark
/// \date        2005-11-15 ~ 2005-11-15
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::InitRolling()
{
    m_bKeyDownA = FALSE;            // A 좌측 롤링
    m_bKeyDownD = FALSE;            // D 우측 롤링
    m_bKeyDownACheck = FALSE;        // A 좌측 사용 허가 체크
    m_bKeyDownDCheck = FALSE;        // D 우측 사용 허가 체크
    m_bKeyDownADouble = FALSE;        // A 더블클릭
    m_bKeyDownDDouble = FALSE;        // D 더블클릭
    m_fADoubleKeyTime = 0.5f;            // A 더블키 타임
    m_fDDoubleKeyTime = 0.5f;            // D 더블키 타임
    m_fRollTime = 0.0f;                // 롤링 타임
    m_fRollAngle = 0.0f;            // 롤링 회전 각도
    m_fRollLength = 0.0f;            // 롤링 거리 체크
    m_fRollDownSpeed = 0.0f;
    m_fRollMessageTime = 0.0f;        // 롤링 메세지
    m_bMissileWarning = FALSE;        // 마사일 경고
    m_bRollUsed = FALSE;            // 2005-07-14 by ispark
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::GetIsUseInterface()
/// \brief        인터페이스 사용 가능한가? 기어 상태에 따라 다름
/// \author        ispark
/// \date        2005-10-26 ~ 2005-10-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::GetIsUseInterface()
{
    // 1. 기어 상태이다
    // 2. B키 기어 정지가 아니다
    // 3. 착륙 상태 아니다
    // 4. A기어 Ground 상태가 아니다
    // 인터페이스 사용 금지
    if(g_pD3dApp->m_bCharacter == FALSE && 
       g_pShuttleChild->m_bUnitStop == FALSE && 
       g_pShuttleChild->m_bLandedMove == FALSE &&
       m_bIsAir == TRUE)
       return FALSE;

    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::CheckCollForObject2()
/// \brief        오브젝트 검사 LANDED 상태 일 때만
/// \author        ispark
/// \date        2005-10-26 ~ 2005-10-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckCollForObject2(float fElapsedTime)
{
    COLLISION_RESULT CollResult;
    D3DXMATRIX matArray[2];
    D3DXVECTOR3 vVel,vFrontPos, vBackPos;
    
    vFrontPos = m_vPos;            // 사람 키높이를 생각하자
    vBackPos = m_vPos;            // 절벽를 계산하기 위해서 내 위치 보다 더 앞쪽 좌표 입력
    vBackPos += m_vVel * -5;

    D3DXMatrixLookAtLH(&matArray[0],&vFrontPos,&(vFrontPos + m_vVel),&m_vUp);        // 앞
    D3DXMatrixLookAtLH(&matArray[1],&vBackPos,&(vBackPos - m_vVel),&m_vUp);        // 뒤
    
    float fFrontMove = m_fShuttleSpeed*fElapsedTime;
    float size[2] = {25 + fFrontMove, 25 + fFrontMove};    // 유닛 크기    앞, 아래
    
    // 오브젝트와 충돌 처리 2004.06.18 jschoi
    
    if(    /*m_bCollObjectCheck && */
        g_pScene &&
        g_pScene->m_byMapType != MAP_TYPE_TUTORIAL &&
        g_pScene->m_pObjectRender)
    {
        // 정면
        CollResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray[0], m_vPos);    
        // 2005-07-29 by ispark
        // 오브젝트안에서 나오지 못하는 버그 예외 처리(나오게 함)
        float dot = D3DXVec3Dot(&m_vVel, &CollResult.vNormalVector);
//        if(0.0f >= dot && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W))
        if(0.0f >= dot)
        {
            if(CollResult.fDist < size[0])
            {
                m_vPos = m_vMyOldPos;
//                m_vPos = m_vOldPos;
//                m_vMyOldPos = m_vOldPos;
                m_fFrameLerp = 1.0f;
            }
        }

        // 뒤
        CollResult = g_pScene->m_pObjectRender->CheckCollMesh(matArray[1], m_vPos);
        dot = D3DXVec3Dot(&(-m_vVel), &CollResult.vNormalVector);
//        if(0.0f >= dot && g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_W))
        if(0.0f >= dot)
        {
            if(CollResult.fDist < size[1])
            {
                m_vPos = m_vMyOldPos;
//                m_vPos = m_vOldPos;
//                m_vMyOldPos = m_vOldPos;
                m_fFrameLerp = 1.0f;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \class        CShuttleChild::SetUnLockOn()
///
/// \brief        
/// \author        ispark
/// \version    
/// \date        2005-10-19 ~ 2005-10-19
/// \warning    
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetUnLockOn()
{
    if(m_pOrderTarget)
    {
        if(    m_pOrderTarget->m_dwPartType == _ENEMY &&                        // 1. 오더타겟이 Enemy였고,
//            !IsInfluenceCharacter(m_myShuttleInfo.InfluenceType, ((CEnemyData*)m_pOrderTarget)->m_infoCharacter.CharacterInfo.InfluenceType) && // 2. 서로 다른 세력이라면
//            ((CEnemyData*)m_pOrderTarget)->GetPkState() == PK_NORMAL &&        // 3. 순수 강제 타겟이었다면.
//            ((CEnemyData*)m_pOrderTarget)->GetbPkAttack() &&                // 4. 순수 강제 타겟이었다면.
            g_pShuttleChild->m_pPkNormalTimer->IsCityWar() == FALSE)        // 3. 도시 점령전중에는 보내지 않는다.
        {    // 서버로 보냄
//            DBGOUT("언락온 보냄\n");
            MSG_FC_MOVE_UNLOCKON sMsg;
            sMsg.AttackIndex = m_myShuttleInfo.ClientIndex;
            sMsg.TargetIndex = ((CEnemyData*)m_pOrderTarget)->m_infoCharacter.CharacterInfo.ClientIndex;    
            g_pFieldWinSocket->SendMsg( T_FC_MOVE_UNLOCKON, (char*)&sMsg, sizeof(sMsg) );                
        }
        m_pOrderTarget = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::IsEnemyPKTarget(int nTagetIndex)
/// \brief        
/// \author        ispark
/// \date        2005-10-19 ~ 2005-10-19
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsEnemyPKTarget(int nTagetIndex)
{
    // 2005-11-03 by ispark
    // Enemy 타겟이 있는지 검사
    CMapEnemyIterator it = g_pScene->m_mapEnemyList.find(nTagetIndex);
    if(it != g_pScene->m_mapEnemyList.end())
    {
//        int index = it->second->m_nTargetIndex;
//        if(it->second->m_nTargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
            return TRUE;
    }

    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::IsEnemyPKAttackTime(int nTagetIndex)
/// \brief        
/// \author        ispark
/// \date        2005-11-03 ~ 2005-11-03
/// \warning    
///
/// \param        
/// \return        BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsEnemyPKAttackTime(int nTagetIndex)
{
    // 2005-11-03 by ispark
    // Enemy 공격 가능 검사
    CMapEnemyIterator it = g_pScene->m_mapEnemyList.find(nTagetIndex);
    if(it != g_pScene->m_mapEnemyList.end())
    {
        // 도시점령전 중이면 공격 가능. 시간과 관계 없음
        if(it->second->m_pPkNormalTimer->IsPkEnableNormalOrderTarget() || m_pPkNormalTimer->IsCityWar())
            return TRUE;
    }

    return FALSE;
}

BOOL CShuttleChild::WarpSkipInOutAirPort()
{
    if(m_bWarpLink == TRUE)
    {
        switch(m_nEventTypeAirPort)
        {
        case AIR_PORT_LANDING_PATTERN_START:
        case AIR_PORT_LANDING_PATTERN_END:
            {
                m_nEventTypeAirPort = AIR_PORT_LANDING_PATTERN_END;
                DeletePattern();
                ChangeUnitState( _LANDING );
                m_pCurrentObjEvent = m_pCurrentObjEventOld;
                m_nEventIndex = m_pCurrentObjEvent->m_nNextEventIndex;

                CObjectChild* pObj = g_pScene->FindObjectByIndex(m_nEventIndex);
                m_nEventIndex = pObj->m_nNextEventIndex;
                m_bWarpLink = FALSE;
                m_bLandingField = TRUE;
                m_bSkipPattern = TRUE;
            }
            return TRUE;
        case AIR_PORT_TAKEOFF_PATTERN_START:
        case AIR_PORT_TAKEOFF_PATTERN_END:
            {
                m_nEventTypeAirPort = AIR_PORT_TAKEOFF_PATTERN_END;
                DeletePattern();
                ChangeUnitState( _NORMAL );
                m_bWarpLink = FALSE;
                m_bKeyBlock = FALSE;
                // 2007-06-12 by bhsohn 미션 인터페이스 수정안
//                if(g_pGameMain->m_pMissionInfo->GetMissionMapIndex())
//                {
//                    INIT_MSG_WITH_BUFFER(MSG_FC_QUEST_MOVE_QUEST_MAP, T_FC_QUEST_MOVE_QUEST_MAP,
//                        pSQuestMove, SendBuf);
//                    pSQuestMove->QuestIndex0 = g_pGameMain->m_pMissionInfo->GetMissionIndex()->QuestIndex;
//                    g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_QUEST_MOVE_QUEST_MAP));
//                }
                
                // 2007.09.19 by bhsohn 전진 기지 출격 처리
                //SendGoMissionMap();
                BOOL bGoMissionMap = SendGoMissionMap();
                if(FALSE == bGoMissionMap)
                {
                    // 미션맵 안 보내면 출격 발동
                    CObjectChild* pObj = g_pScene->FindEventObjectByWarp();
                    if(pObj)
                    {
                        SendFieldSocketRequestEventObjectWarpIn(pObj);
                    }
                }                
                // end 2007.09.19 by bhsohn 전진 기지 출격 처리
            }
            return TRUE;
        }
    }

    return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::InitCharacterToShuttleData()
/// \brief        캐릭터가 셔틀한테 초기화 시키는 함수. 캐릭터만 쓴다.
/// \author        ispark
/// \date        2005-11-10 ~ 2005-11-10
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::InitCharacterToShuttleData()
{
    m_pTarget = NULL;
    m_pOrderTarget = NULL;

    if(m_pPrimaryWeapon)
    {
        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        //m_pPrimaryWeapon->m_vecTargetIndex.clear();
        m_pPrimaryWeapon->m_vecTargetIndexData.clear();
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
    }
    if(m_pSecondaryWeapon)
    {
        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        //m_pSecondaryWeapon->m_vecTargetIndex.clear();
        m_pSecondaryWeapon->m_vecTargetIndexData.clear();
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
    }
    
    SetMissileCount(0);                        // 2005-08-10 by ispark
    SetMissileWarning(FALSE);

    m_nBooster = BOOSTER_TYPE_STOP;
    SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_LANDED_MASK);
    SendFieldSocketChangeBodyCondition(g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_BOOSTER5_MASK|BODYCON_SET_OR_CLEAR_MASK);
    CheckBodyConditionToServer(g_pShuttleChild->m_myShuttleInfo.BodyCondition);
    ChangeSingleBodyCondition(BODYCON_BOOSTER5_MASK);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::InitUnitStop()
/// \brief        유닛정지 상태를 푼다. 
/// \author        ispark
/// \date        2005-11-17 ~ 2005-11-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::InitUnitStop()
{
    m_bUnitStop = FALSE;
    m_bPenaltyUnitStop = FALSE;
    m_bMouseMoveLock = FALSE;
    m_nBooster = BOOSTER_TYPE_MIN;
    m_bMouseLock = FALSE;
    g_pD3dApp->m_bFixUnitDirMode = FALSE;
    if(g_pInterface && g_pInterface->m_pGameMain)
        g_pInterface->m_pGameMain->InitShowWindow();            // 2005-08-05 by ispark
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::UnitStop()
/// \brief        유닛을 정지 시킨다.
/// \author        dgwoo
/// \date        2007-05-11 ~ 2007-05-11
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::UnitStop()
{
    //g_pShuttleChild->m_bUnitStop = !g_pShuttleChild->m_bUnitStop;
    g_pShuttleChild->m_bUnitStop = TRUE;
    g_pShuttleChild->m_bMouseLock = TRUE;
    g_pShuttleChild->m_bMouseMoveLock = TRUE;
#ifdef _DISABLE_BSTOP_AUTOHORIZONT
    g_pD3dApp->m_bFixUnitDirMode = FALSE;
#else
    g_pD3dApp->m_bFixUnitDirMode = TRUE;
#endif
    // 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달    
    SendFieldSocketChangeBodyCondition( m_myShuttleInfo.ClientIndex, BODYCON_STOP_MASK | BODYCON_SET_OR_CLEAR_MASK);
    //end 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달
    // 2008-11-06 by bhsohn 마우스 가두기 모드 보완
    g_pD3dApp->RefreshMouseMode();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::SetFormationMove()
/// \brief        편대비행시 방향 전환. 타겟을 맞추기 위한 수정. 단 편대장은 제외
/// \author        ispark
/// \date        2005-11-21 ~ 2005-11-21
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetFormationMove(float fElapsedTime)
{
    int    nx = g_pD3dApp->GetBackBufferDesc().Width/2;
    int    ny = g_pD3dApp->GetBackBufferDesc().Height/2;

    D3DXMATRIX  matTemp;
    
    // 편대장은 자유자재로 조정이 가능하므로 편대장이 아닌경우만 한다.
    if(m_pClientParty->GetPartyInfo().bPartyType != _PARTYMASTER)
    {
        if((m_ptDir.x - m_ptDirOld.x) > 0)
        {
            m_ptDirOld.x += FORMATION_MOVE_RATE;
            if(m_ptDir.x < m_ptDirOld.x)
                m_ptDirOld.x = m_ptDir.x;
        }
        else if((m_ptDir.x - m_ptDirOld.x) < 0)
        {
            m_ptDirOld.x += -FORMATION_MOVE_RATE;
            if(m_ptDir.x > m_ptDirOld.x)
                m_ptDirOld.x = m_ptDir.x;
        }
        
        if((m_ptDir.y - m_ptDirOld.y) > 0)
        {
            m_ptDirOld.y += FORMATION_MOVE_RATE;
            if(m_ptDir.y < m_ptDirOld.y)
                m_ptDirOld.y = m_ptDir.y;
        }
        else if((m_ptDir.y - m_ptDirOld.y) < 0)
        {
            m_ptDirOld.y += -FORMATION_MOVE_RATE;
            if(m_ptDir.y > m_ptDirOld.y)
                m_ptDirOld.y = m_ptDir.y;
        }

        // 상하
        float fAngle = (float)(FORMATION_UPDOWN_RANGE * m_ptDir.y) / ny;
        if(fAngle > FORMATION_UPDOWN_RANGE)
            fAngle = FORMATION_UPDOWN_RANGE;
        else if(fAngle < -FORMATION_UPDOWN_RANGE)
            fAngle = -FORMATION_UPDOWN_RANGE;

        m_fCurrentAngle = (float)(FORMATION_UPDOWN_RANGE * m_ptDirOld.y) / ny;
        if(m_fCurrentAngle > FORMATION_UPDOWN_RANGE)
            m_fCurrentAngle = FORMATION_UPDOWN_RANGE;
        else if(m_fCurrentAngle < -FORMATION_UPDOWN_RANGE)
            m_fCurrentAngle = -FORMATION_UPDOWN_RANGE;

        D3DXMatrixRotationAxis(&matTemp,&m_vSideVel, m_fMouseRate*m_fCurrentAngle);
        D3DXVec3TransformCoord(&m_vVel,&m_vVel, &matTemp );

        // 좌우
        m_fSideRate = (float)(FORMATION_SIDE_RANGE * m_ptDir.x) / nx;
        if(m_fSideRate > FORMATION_SIDE_RANGE)
            m_fSideRate = FORMATION_SIDE_RANGE;
        else if(m_fSideRate < -FORMATION_SIDE_RANGE)
            m_fSideRate = -FORMATION_SIDE_RANGE;

        m_fCurrentSideRate = (float)(FORMATION_SIDE_RANGE * m_ptDirOld.x) / nx;
        if(m_fCurrentSideRate > FORMATION_SIDE_RANGE)
            m_fCurrentSideRate = FORMATION_SIDE_RANGE;
        else if(m_fCurrentSideRate < -FORMATION_SIDE_RANGE)
            m_fCurrentSideRate = -FORMATION_SIDE_RANGE;

        D3DXMatrixRotationAxis(&matTemp, &m_vUp, m_fMouseRate*m_fCurrentSideRate);
        D3DXVec3TransformCoord(&m_vVel, &m_vVel, &matTemp);
    }
}

// 2006-01-20 by ispark, 삭제
//BOOL CShuttleChild::IsInfluenceWarType(int nEnemyIndex)
//{
//    if( !COMPARE_INFLUENCE(nEnemyIndex, m_myShuttleInfo.InfluenceType) &&
//        !COMPARE_INFLUENCE(nEnemyIndex,        INFLUENCE_TYPE_NORMAL) && 
//        !COMPARE_INFLUENCE(m_myShuttleInfo.InfluenceType,    INFLUENCE_TYPE_NORMAL))
//    {
//        return TRUE;
//    }
//
//    return FALSE;    
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::CheckWearItem()
/// \brief        장착 아이템 체크 : 분당 체크
/// \author        ispark
/// \date        2005-12-29 ~ 2005-12-29
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckWearItem(float fElapsedTime)
{
    char buf[256];
    m_fCheckWearItemTime += fElapsedTime;
    if(60.0f <= m_fCheckWearItemTime)
    {
        m_fCheckWearItemTime = 0.0f;

        ITEM_GENERAL* pCheckRearWearItem = m_pStoreData->FindItemInInventoryByWindowPos(POS_REAR);
        if(pCheckRearWearItem == NULL)
        {
            wsprintf(buf,STRMSG_C_051203_0001);    // "엔진이 장착되어 있지 않습니다."
            g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);            
        }
        
        ITEM_GENERAL* pCheckCenterWearItem = m_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
        if(pCheckCenterWearItem == NULL)
        {
            wsprintf(buf,STRMSG_C_051229_0100);    // "아머가 장착되어 있지 않습니다."
            g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);            
        }
    
        ITEM_GENERAL* pCheckRadarWearItem = m_pStoreData->FindItemInInventoryByWindowPos(POS_PROW);
        if(pCheckRadarWearItem == NULL)
        {
            wsprintf(buf,STRMSG_C_051229_0101);    // "레이더가 장착되어 있지 않습니다."
            g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);            
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::CheckDamageBodyCondition()
/// \brief        기존 Damage 처리를 에너지 상태를 받으면 그때 한번 처리를 변환이 있을때 처리로 변경
/// \author        ispark
/// \date        2005-12-30 ~ 2005-12-30
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckDamageBodyCondition()
{
    int nDamageState = 0;
    // 2007-05-31 by bhsohn 부활시 기체 불타는 현상 처리
//    if(m_myShuttleInfo.CurrentHP / m_myShuttleInfo.HP < CHARACTER_DAMAGE_STATE_05)
//    {
//        nDamageState = 3;
//    }
//    else if(m_myShuttleInfo.CurrentHP / m_myShuttleInfo.HP < CHARACTER_DAMAGE_STATE_10)
//    {
//        nDamageState = 2;
//    }
//    else if(m_myShuttleInfo.CurrentHP / m_myShuttleInfo.HP < CHARACTER_DAMAGE_STATE_15)
//    {
//        nDamageState = 1;
//    }
//    else
//    {
//        nDamageState = 0;
//    }
    float fCurrentHP = m_fNextHP;

    if(fCurrentHP / m_myShuttleInfo.HP < CHARACTER_DAMAGE_STATE_05)
    {
        nDamageState = 3;
    }
    else if(fCurrentHP / m_myShuttleInfo.HP < CHARACTER_DAMAGE_STATE_10)
    {
        nDamageState = 2;
    }
    else if(fCurrentHP / m_myShuttleInfo.HP < CHARACTER_DAMAGE_STATE_15)
    {
        nDamageState = 1;
    }
    else
    {
        nDamageState = 0;
    }
    // end 2007-05-31 by bhsohn 부활시 기체 불타는 현상 처리

    if(m_nCheckDamage != nDamageState)
    {
        m_nCheckDamage = nDamageState;

        if(m_nCheckDamage == 0)
        {
            // 데미지 비활성화
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE1_MASK, FALSE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE2_MASK, FALSE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE3_MASK, FALSE );
            g_pGameMain->m_pUnitState->OffState(INF_UW_DANGER_MASK);
        }
        else if(m_nCheckDamage == 1)
        {
            // 데미지 1번 활성화
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE1_MASK, TRUE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE2_MASK, FALSE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE3_MASK, FALSE );
            g_pGameMain->m_pUnitState->OffState(INF_UW_DANGER_MASK);
        }
        else if(m_nCheckDamage == 2)
        {
            // 데미지 1,2번 활성화
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE1_MASK, TRUE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE2_MASK, TRUE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE3_MASK, FALSE );
            g_pGameMain->m_pUnitState->InsertState(INF_UW_DANGER_MASK);
            if(g_pD3dApp->m_bCharacter == FALSE)
                g_pD3dApp->m_pSound->PlayD3DSound(SOUND_WARNING, D3DXVECTOR3(0,0,0), FALSE);
        }
        else if(m_nCheckDamage == 3)
        {
            // 데미지 1,2,3번 모두 활성화
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE1_MASK, TRUE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE2_MASK, TRUE );
            g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_DAMAGE3_MASK, TRUE );
            g_pGameMain->m_pUnitState->InsertState(INF_UW_DANGER_MASK);
            if(g_pD3dApp->m_bCharacter == FALSE)
                g_pD3dApp->m_pSound->PlayD3DSound(SOUND_WARNING_HP, D3DXVECTOR3(0,0,0), FALSE);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CEnemyData::RemoveEnemyTarget()
/// \brief        적 타겟 삭제
/// \author        ispark
/// \date        2006-01-13 ~ 2006-01-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::RemoveEnemyTarget()
{
    CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
    while(it != g_pScene->m_mapEnemyList.end())
    {
        it->second->m_nTargetIndex = 0;
        it++;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::InitMissileWarning()
/// \brief        미사일 경고 초기화
/// \author        ispark
/// \date        2006-02-13 ~ 2006-02-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::InitMissileWarning()
{
    m_bMissileWarning = FALSE;
    m_nMissileCount = 0;

    if(g_pD3dApp->m_pSound)
    {
        g_pD3dApp->m_pSound->StopD3DSound(SOUND_MISSILE_WARNNING);
    }
}

void CShuttleChild::SetInvenMesh(int nIndex, CSkinnedMesh* pMesh)
{ 

    if(FALSE == util::in_range(0, nIndex, 3))
    {
        return;
    }
    // 2007-11-09 by dgwoo 무기 메쉬를 배열에서 벡터로 변경.
    int nSize = m_vectInvenWeaponMesh.size();
    int i=0;
    for(i=0; i < nSize; i++)
    {
        if(m_vectInvenWeaponMesh[i].nMatIndex == nIndex
            && m_vectInvenWeaponMesh[i].pInvenWeaponMesh == pMesh)
        {
            return;
        }
    }

//    DBGOUT("SetInvenMesh index = %d pMesh = %x\n",nIndex,pMesh);
    INVEN_WEAPON_MESH tmMesh;
    tmMesh.nMatIndex        = nIndex;
    tmMesh.pInvenWeaponMesh    = pMesh;
    m_vectInvenWeaponMesh.push_back(tmMesh);
}

void CShuttleChild::InitInvenMesh()
{
    // 2007-11-09 by dgwoo 무기 메쉬를 배열에서 벡터로 변경.
    m_vectInvenWeaponMesh.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::SendDeadMsg()
/// \brief        기어 죽을 때 기본 메세지
/// \author        ispark
/// \date        2006-04-10 ~ 2006-04-10
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SendDeadMsg()
{
    // 2009. 12. 04 by jskim 유닛 사망시 인터페이스를 없애면 귀환 메시지 박스도 없어지는 버그 수정
    vector<CINFMessageBox*>::iterator it = g_pGameMain->m_pInfWindow->m_vecMessageBox.begin();
    while(it != g_pGameMain->m_pInfWindow->m_vecMessageBox.end())
    {
        if ( (*it)->m_nMsgType == _Q_DEAD_RETURN
            || (*it)->m_nMsgType == _Q_FUELALLIN_DEAD_RETURN
            || (*it)->m_nMsgType == _Q_INFLUENCEMAP_DEAD_RETURN
            || (*it)->m_nMsgType == _Q_REVIVE_WAIT_5SECOND )
            return;
        else
            it++;
    }
    //end 2009. 12. 04 by jskim 유닛 사망시 인터페이스를 없애면 귀환 메시지 박스도 없어지는 버그 수정

    // 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
    // 인피 사망시 5초 부활시스템
    if( g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_REVIVE_WAIT_5SECOND )
        || g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_ARENA_RESTART ) )
        return;
    // end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(m_myShuttleInfo.MapChannelIndex.MapIndex);
    if(m_dwState == _FUELALLIN)
    {
        g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_CARD_0003, _Q_FUELALLIN_DEAD_RETURN);//"격납고로 이동합니다."        
    }
    // 2006-08-08 by ispark, 이벤트 맵
    else if(IS_MAP_INFLUENCE_EVENT_AREA(pMapInfo->MapInfluenceType))
    {
        g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060102_0100, _Q_INFLUENCEMAP_DEAD_RETURN); //"도시로 귀환하겠습니다."        
    }


    // 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

//     // 2007-05-07 by dgwoo 아레나 맵일경우 처리.
//     else if(IS_MAP_INFLUENCE_ARENA(pMapInfo->MapInfluenceType))
//     {
//         if(!g_pInterface->m_pGameArena->IsArenaGameState(ARENA_GAME_STATE_RESULT))
//             g_pGameMain->m_pInfWindow->AddTimeMsgBox(STRMSG_C_070507_0100,_Q_ARENA_RESTART,30,STRMSG_C_070507_0101);// "부활하시겠습니까?"
//     }
    else if( IS_MAP_INFLUENCE_ARENA( pMapInfo->MapInfluenceType )
             || IS_MAP_INFLUENCE_INFINITY( pMapInfo->MapInfluenceType ) )
    {
        // 인피 사망시 5초 부활시스템
        if( !g_pInterface->m_pGameArena->IsArenaGameState( ARENA_GAME_STATE_RESULT ) )
            g_pGameMain->m_pInfWindow->AddTimeMsgBox( " ", _Q_REVIVE_WAIT_5SECOND, 5, STRMSG_C_091103_0313 );// "\\r%d초\\r 후에 부활이 가능합니다."
    }

    // end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


    // 2007-09-28 by dgwoo 모선전 진행중이면 텔레포트 여부를 묻는다.
    // 2009. 10. 14 by jskim 프리스카 제거
    //else if(g_pD3dApp->IsWarMotherShip() && IS_OTHER_INFLUENCE_MAP(m_myShuttleInfo.InfluenceType, pMapInfo->MapIndex, pMapInfo->MapInfluenceType))
    else if(g_pD3dApp->IsWarMotherShip() && CAtumSJ::IsOtherInfluenceMap(m_myShuttleInfo.InfluenceType, pMapInfo->MapIndex, pMapInfo->MapInfluenceType))
    //end 2009. 10. 14 by jskim 프리스카 제거
    {
        g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_070928_0100, _Q_DEAD_RETURN);//"도시맵으로 귀환하시겠습니까? (승인:도시맵, 취소:텔레포트)"
    }
    else if(!EnableRevivalMap(m_myShuttleInfo.InfluenceType, m_myShuttleInfo.MapChannelIndex.MapIndex))
    {
        // 2006-01-02 by ispark, 다른 세력맵이므로 자기 세력 도시맵으로 귀환(현재 맵에 부활 안함)
        g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060102_0100, _Q_INFLUENCEMAP_DEAD_RETURN); //"도시로 귀환하겠습니다."
    }
    // 2006-08-08 by ispark, 다른 세력 일반맵(이전맵으로 부활 가능
    // 2009. 10. 14 by jskim 프리스카 제거
    //else if(g_pD3dApp->IsWarMotherShip() && IS_OTHER_INFLUENCE_MAP((m_myShuttleInfo.InfluenceType, pMapInfo->MapInfluenceType))
    //else if(CAtumSJ::IsOtherInfluenceMap(m_myShuttleInfo.InfluenceType, pMapInfo->MapIndex, pMapInfo->MapInfluenceType))
    // 2009. 11. 11 by jskim 프리스카 제거 수정
    else if(g_pD3dApp->IsWarMotherShip() && CAtumSJ::IsOtherInfluenceMap(m_myShuttleInfo.InfluenceType, pMapInfo->MapIndex, pMapInfo->MapInfluenceType))
    //end 2009. 11. 11 by jskim 프리스카 제거 수정
    //end 2009. 10. 14 by jskim 프리스카 제거
    {
        g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060808_0000, _Q_DEAD_RETURN);//"도시맵으로 귀환하시겠습니까? (승인:도시맵, 취소:이전맵)"
    }
    else
    {
        g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_051205_0005, _Q_DEAD_RETURN);//"도시맵으로 귀환하시겠습니까? (확인:도시맵, 취소:현재맵)"
        g_pD3dApp->m_bDeadByP2PPK = FALSE;
    }
    // 2008-06-17 by bhsohn 편대 관련 처리
    // 죽었을 보낸다.
    if(m_pClientParty && m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER 
        && m_pClientParty->IsFormationFlight())
    {        
        m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
    }

}

void CShuttleChild::SetBackView(BOOL bUpDown)
{
    // 2006-05-03 by ispark, 백뷰
    // 튜토리얼 모드오 ㅏ시즈모드시에는 백뷰 사용 금지
    if(g_pTutorial->IsTutorialMode() == FALSE && m_bAttackMode != _SIEGE)
    {
        // DOWN
        if(bUpDown == TRUE)
        {
            m_bTurnCamera = TRUE;
            m_bMouseLock  = TRUE;                    
            m_fOldUpRate = 0.0f;                            
        }
        // UP
        else if(bUpDown == FALSE)
        {
            m_bTurnCamera = FALSE;
            m_bMouseLock  = FALSE;
            m_fOldUpRate = 0.0f;
            // 2007-03-26 by dgwoo 옵저버 모드시는 MouseLock 을 건다.
            if(IsObserverMode())
            {
                m_bMouseLock = TRUE;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CShuttleChild::SetSelectCharacterItem()
/// \brief        선택창에 갈때 현재 내 캐릭터 정보 입력
/// \author        ispark
/// \date        2006-05-23 ~ 2006-05-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetSelectCharacterItem()
{
    // 2007-11-22 by bhsohn 아레나 통합서버
    if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
    {
        // 아레나 에서 게임 종료시, 전에 있던 정보 가져옴.        
        CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
        // 게임정보를 다시 갱신 
        g_pD3dApp->SetMyShuttleInfo(pMainInfo);        

    }
    // end 2007-11-22 by bhsohn 아레나 통합서버    

    FC_CONNECT_LOGIN_INFO info;
    memset(&info, 0x00, sizeof(FC_CONNECT_LOGIN_INFO));
    strcpy(info.CharacterName, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName);
    info.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
    info.Color = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Color;
    info.PilotFace = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.PilotFace;
    info.Race = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race;
    info.UnitKind = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind;
    // 2009. 11. 3 by jskim 캐쉬(인벤/창고 확장) 아이템 추가 구현
    info.RacingPoint = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.RacingPoint;
    //end 2009. 11. 3 by jskim 캐쉬(인벤/창고 확장) 아이템 추가 구현
//    info.CharacterRenderInfo.RI_ProwOut = GetCreateUnitWeapon11(pMsg->Character.UnitKind);
//    info.CharacterRenderInfo.RI_Rear = GetCreateUnitEngine(pMsg->Character.UnitKind);
    CHARACTER_RENDER_INFO CharacterRenderInfo;
    memset(&CharacterRenderInfo, 0x00, sizeof(CHARACTER_RENDER_INFO));
    CItemInfo* pInfo;
    pInfo = g_pShuttleChild->m_pStoreData->FindItemInInventoryByWindowPos(POS_PROWOUT);
    CharacterRenderInfo.RI_ProwOut = 0;


    // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

    CharacterRenderInfo.RI_ProwOut_ShapeItemNum     = 0;
    CharacterRenderInfo.RI_ProwOut_EffectItemNum = 0;

    // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


    if(pInfo)
    {
        CharacterRenderInfo.RI_ProwOut = pInfo->ItemNum;

        // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

        CharacterRenderInfo.RI_ProwOut_ShapeItemNum     = pInfo->GetShapeItemNum();
        CharacterRenderInfo.RI_ProwOut_EffectItemNum = pInfo->GetEffectItemNum();

        // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
    }

    pInfo = g_pShuttleChild->m_pStoreData->FindItemInInventoryByWindowPos(POS_WINGOUT);
    CharacterRenderInfo.RI_WingOut = 0;

    // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

    CharacterRenderInfo.RI_WingOut_ShapeItemNum        = 0;
    CharacterRenderInfo.RI_WingOut_EffectItemNum    = 0;

    // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


    if(pInfo)
    {
        CharacterRenderInfo.RI_WingOut = pInfo->ItemNum;

        // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

        CharacterRenderInfo.RI_WingOut_ShapeItemNum        = pInfo->GetShapeItemNum();
        CharacterRenderInfo.RI_WingOut_EffectItemNum    = pInfo->GetEffectItemNum();

        // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
    }

    pInfo = g_pShuttleChild->m_pStoreData->FindItemInInventoryByWindowPos(POS_CENTER);
    CharacterRenderInfo.RI_Center = 0;
    
    // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

    //CharacterRenderInfo.RI_ArmorColorCode = 0;
    CharacterRenderInfo.RI_Center_ShapeItemNum    = 0;

    // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

    if(pInfo)
    {
        CharacterRenderInfo.RI_Center = pInfo->ItemNum;

        // 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

        //CharacterRenderInfo.RI_ArmorColorCode = pInfo->ColorCode;
#ifdef _REWORKED_COLORSHOP
        if (pInfo->ColorCode)
        {
            CharacterRenderInfo.RI_Center_ShapeItemNum = pInfo->ColorCode;
        }
        else
        {
#endif
            CharacterRenderInfo.RI_Center_ShapeItemNum = pInfo->GetShapeItemNum();
#ifdef _REWORKED_COLORSHOP
        }
#endif

        // end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
    }

    pInfo = g_pShuttleChild->m_pStoreData->FindItemInInventoryByWindowPos(POS_REAR);
    CharacterRenderInfo.RI_Rear = 0;
    if(pInfo)
    {
        CharacterRenderInfo.RI_Rear = pInfo->ItemNum;
    }

    // 2006-08-14 by dgwoo 레이다 정보를 저장.
    pInfo = g_pShuttleChild->m_pStoreData->FindItemInInventoryByWindowPos(POS_PROWIN);
    CharacterRenderInfo.RI_ProwIn = 0;
    if(pInfo)
    {
        CharacterRenderInfo.RI_ProwIn = pInfo->ItemNum;
    }
    //--------------------------------------------------------------------------//

    pInfo = g_pShuttleChild->m_pStoreData->FindItemInInventoryByWindowPos ( POS_PET );
    CharacterRenderInfo.RI_Pet = 0;
    if ( pInfo )
    {
        tPET_CURRENTINFO *psPetCurInfo = GetPetManager()->GetPtr_PetCurrentData ( pInfo->UniqueNumber );
        tPET_LEVEL_DATA *psPetLvData = NULL;
        
        if ( psPetCurInfo )
            psPetLvData = g_pDatabase->GetPtr_PetLevelData ( psPetCurInfo->PetIndex , psPetCurInfo->PetLevel );
        
        CharacterRenderInfo.RI_Pet = pInfo->ItemNum;

        if ( psPetLvData )
            CharacterRenderInfo.RI_Pet_ShapeItemNum = pInfo->GetItemInfo()->SourceIndex;//psPetLvData->FieldSourceIndex; //2011-10-06 by jhahn 파트너 성장형 시스템
    }

    info.CharacterRenderInfo = CharacterRenderInfo;

    g_pSelect->SetUnitRenderInfo(g_pSelect->GetCurrentSelectIndex(), info);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::InitWarpShuttle(MAP_CHANNEL_INDEX &channelIndex = NULL)
/// \brief        워프시 초기화
/// \author        ispark
/// \date        2006-05-29 ~ 2006-05-29
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::InitWarpShuttle(MAP_CHANNEL_INDEX channelIndex)
{
    // 2006-12-14 by dgwoo    도시맵에선 토글 스킬을 해제시킨다.
    if(IS_BAZAAR_MAP_INDEX(channelIndex.MapIndex) 
        || IS_CITY_MAP_INDEX(channelIndex.MapIndex))
    {
        m_pSkill->ReleaseAllUsingToggleSkill();
    }
    // 2006-12-22 by dgwoo 워프 통과시 그라운드스킬을 해제한다.
    if(m_pSkill)
        m_pSkill->ReleaseGroundSkill();
    DeleteItemPoint();// Effect로 생성한 아이템포인터를 지운다
    m_bSkillMoveIsUse = FALSE;
    // 2007-01-08 by bhsohn 인비지블후 워프 통과 시, 공격이 되는 현상 처리
    //m_bAttack = TRUE;
    if(m_pSkill)
    {
        if(TRUE == m_pSkill->IsAttackPossibility())
        {
            m_bAttack = TRUE;
        }
    }
    else
    {
        m_bAttack = TRUE;
    }
    // end 2007-01-08 by bhsohn 인비지블후 워프 통과 시, 공격이 되는 현상 처리
    InitMissileWarning();
    SetBackView(FALSE);        // 2006-05-29 by ispark, 백뷰 초기화
    InitRolling();
}

void CShuttleChild::InitWarpShuttle()
{
    // 2006-12-22 by dgwoo 워프 통과시 그라운드스킬을 해제한다.
    if(m_pSkill)
        m_pSkill->ReleaseGroundSkill();
    
    DeleteItemPoint();// Effect로 생성한 아이템포인터를 지운다
    m_bSkillMoveIsUse = FALSE;
    // 2007-01-08 by bhsohn 인비지블후 워프 통과 시, 공격이 되는 현상 처리
    //m_bAttack = TRUE;
    if(m_pSkill )
    {
        if(TRUE == m_pSkill->IsAttackPossibility())
        {
            m_bAttack = TRUE;
        }        
    }
    else
    {
        m_bAttack = TRUE;
    }
    // end 2007-01-08 by bhsohn 인비지블후 워프 통과 시, 공격이 되는 현상 처리
    InitMissileWarning();
    SetBackView(FALSE);        // 2006-05-29 by ispark, 백뷰 초기화
    InitRolling();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            int CShuttleChild::CheckUnitState()
/// \brief        현재 기어 상태들을 여러가지로 통해서 판단
/// \author        ispark
/// \date        2006-09-19 ~ 2006-09-19
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int CShuttleChild::CheckUnitState()
{
    if(m_dwState == _FALLING ||
        m_dwState == _FALLEN ||
        m_dwState == _EXPLODING ||
        m_dwState == _EXPLODED ||
        m_dwState == _FUELALLIN)
    {
        return BREAKDOWN;
    }

    if(m_dwState == _NORMAL ||
        m_dwState == _WARP ||
        m_dwState == _LANDING ||
        m_dwState == _LANDED ||
        m_dwState == _TAKINGOFF)
    {
        return FLIGHT;
    }

    return DONOTKNOW;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::CheckFormationMoveDelete()
/// \brief        편대 비행 중이면 해제
/// \author        ispark
/// \date        2006-11-02 ~ 2006-11-02
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckFormationMoveDelete()
{
    if(    m_pClientParty &&
        m_pClientParty->IsFormationFlightMove())
    {
        SendPacket(T_IC_PARTY_CANCEL_FLIGHT_POSITION);
        m_pClientParty->FormationFlightClear();
        g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PARTY_0001, COLOR_SYSTEM);

        // 해제시 방향값 초기화 
        m_fCurrentSideRate = 0.0f;
        m_fCurrentAngle = 0.0f;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            CHARACTER* GetShuttleInfo();
/// \brief        셔틀의 정보를 얻어온다.
/// \author        dgwoo
/// \date        2007-01-26 ~ 2007-01-26
/// \warning    
///
/// \param        
/// \return        CHARACTER*
///////////////////////////////////////////////////////////////////////////////
const CHARACTER* CShuttleChild::GetShuttleInfo()
{
    return &m_myShuttleInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CShuttleChild::IsMyShuttleRolling()
/// \brief        셔틀의 정보를 얻어온다.
/// \author        // 2007-03-23 by bhsohn 롤링하면서 공중 시즈 안되는 문제 해결
/// \date        2007-03-23 ~ 2007-03-23
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsMyShuttleRolling()
{
    return m_bRollUsed;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CShuttleChild::IsObserverMode()
/// \brief        옵저버 모드 
/// \author        // 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
/// \date        2007-03-27 ~ 2007-03-27
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsObserverMode()
{
    if(m_stObserve.ClientIndex)
        return TRUE;
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CShuttleChild::IsOperAndObser()
/// \brief        옵저버 모드 이거나 관전 모드중인가?
/// \author        dgwoo
/// \date        2007-06-18 ~ 2007-06-18
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsOperAndObser()
{
    if(m_bObserve || m_bOperation)
        return TRUE;
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL ChangeObserveUser(int nKey);
/// \brief        옵저버 유저를 변경할때.
/// \author        dgwoo
/// \date        2007-03-29 ~ 2007-03-29
/// \warning    
///
/// \param        단축키의 번호.
/// \return        유저가 존재하는가 존재하지 않은가.
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::ChangeObserveUser(int nKey)
{
    // 중복으로 서버로 인덱스값을 보내는것을 막기 위한 소스
    ClientIndex_t tempIndex = m_stObserve.ClientIndex;
    if(tempIndex == m_ObserveQuick[nKey])
        return FALSE;

    // 기존의 옵저버 모드로 붙어있을시 해제를 먼저 한다.
    if(IsObserverMode())
    {
        ObserveEnd();
    }

    // 단축키 번호에 저장되어있는 옵저버로 붙을것을 요청.
    
    if(m_ObserveQuick[nKey])
    {
        MSG_FC_CHARACTER_OBSERVER_TARGET_CHARACTERINDEX sMsg;
        sMsg.TargetClientIndex = m_ObserveQuick[nKey];
        g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_OBSERVER_START, (char*)&sMsg, sizeof(sMsg) );
        return TRUE;
    }
    DbgOut("Observe Quick[%d].ClientIndex = %d\n",nKey,m_ObserveQuick[nKey]);
    return FALSE;
}

void CShuttleChild::TickOperation(float fElapsedTime)
{
    if(g_pD3dApp->m_bChatMode == FALSE)
    {
        // 키값 처리.
        // F키를 눌렀을경우 유저 상태창은 사라진다.
        if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_F))
        {
            g_pInterface->m_pGameArena->m_bShowOperUserInfo = !g_pInterface->m_pGameArena->m_bShowOperUserInfo;
        }
        // 옵저버 모드를 취소할 경우.
        if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_ESCAPE))
        {
            ObserveCancelUpdateInfo();
            ObserveEnd();
        }

    }
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            void TickObserve(float fElapsedTime)
/// \brief        옵저버모드시 이함수를 거친다.
/// \author        dgwoo
/// \date        2007-03-29 ~ 2007-03-29
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::TickObserve(float fElapsedTime)
{
    if(g_pD3dApp->m_bChatMode == FALSE)
    {
        // 키값 처리.
        for(int i = 0 ; i < OBSERVE_MAX_QUICK ; i++)
        {
            // 2008-11-13 by bhsohn 조이스틱 작업
            //if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_1+i) )
            if( g_pD3dApp->GetAsyncKeyState_DIK_DIJ(DIK_1+i) )
            {
                if(i == 9)
                {
                    ChangeObserveUser(0);
                }
                else{
                    // 옵저버 모드에서 현재 가려고하는 유저의 단축키를 눌렀을경우.
                    ChangeObserveUser(i+1);
                }
                
            }
        }
        // 옵저버 모드를 취소할 경우.
        if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_ESCAPE))
        {
            ObserveCancelUpdateInfo();
            ObserveEnd();
        }
    }
}
void CShuttleChild::ObserveCancelUpdateInfo()
{
    // 2007-04-04 by dgwoo ESC키를 누를 경우에만 셔틀의 정보를 다시 입력해준다.
    g_pShuttleChild->m_myShuttleInfo.DP = g_pShuttleChild->m_stObserve.ClientInfo.DP;
    g_pShuttleChild->m_myShuttleInfo.HP = g_pShuttleChild->m_stObserve.ClientInfo.HP;
    g_pShuttleChild->m_myShuttleInfo.EP = g_pShuttleChild->m_stObserve.ClientInfo.EP;
    g_pShuttleChild->m_myShuttleInfo.SP = g_pShuttleChild->m_stObserve.ClientInfo.SP;
    g_pShuttleChild->m_myShuttleInfo.CurrentDP = g_pShuttleChild->m_stObserve.ClientInfo.CurrentDP;
    g_pShuttleChild->m_myShuttleInfo.CurrentHP = g_pShuttleChild->m_stObserve.ClientInfo.CurrentHP;
    g_pShuttleChild->m_myShuttleInfo.CurrentEP = g_pShuttleChild->m_stObserve.ClientInfo.CurrentEP;
    g_pShuttleChild->m_myShuttleInfo.CurrentSP = g_pShuttleChild->m_stObserve.ClientInfo.CurrentSP;
    g_pGameMain->m_pUnitInfoBar->UpdateAllGage();
    //--------------------------------------------------------------------------//
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::ObserveEnd()
/// \brief        옵저버 종료시.
/// \author        dgwoo
/// \date        2007-04-03 ~ 2007-04-03
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::ObserveEnd()
{
    if(0 == m_stObserve.ClientIndex)
    {
        return;
    }

    MSG_FC_CHARACTER_OBSERVER_END sMsg;
    sMsg.TargetClientIndex = m_stObserve.ClientIndex;
    g_pFieldWinSocket->SendMsg( T_FC_CHARACTER_OBSERVER_END, (char*)&sMsg, sizeof(sMsg) );

    m_bMouseLock = FALSE;
    m_stObserve.ClientIndex = NULL;
    // 옵저버 모드가 꺼지면서 부스터 이펙트 역시 삭제한다.
    CAppEffectData* pEffect =  g_pScene->FindEffect(RC_EFF_BOOSTER);
    if(pEffect)
    {
        pEffect->ChangeBodyCondition(BODYCON_LANDED_MASK); // BODYCON_BOOSTER_OFF
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CShuttleChild::IsbAGearFollowEnemy()
/// \brief        
/// \author        // 2007-04-17 by bhsohn 가까운 거리에서의 시즈모드시 데이지 안들어가는 현상 처리
/// \date        2007-04-17 ~ 2007-04-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsAGearFollowEnemy()
{
    return m_bAGearFollowEnemy;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        스캔 아이템 사용 및 취
/// \author        // 2007-04-19 by bhsohn 서치아이 아이템 추가
/// \date        2007-04-19 ~ 2007-04-19
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::UseScanItem(BOOL i_bScan, float i_fCheckEnableTime, float i_fScanRange)
{    
    m_pSkill->SetScanSkill(SCAN_ITEM, i_bScan);
    if(FALSE == i_bScan)
    {
        return;
    }
    m_pSkill->SetScanRange(SCAN_ITEM, i_fScanRange);    

    ClientIndex_t nMyClientIndex = m_myShuttleInfo.ClientIndex;
    CUnitData *pUnit = g_pScene->FindUnitDataByClientIndex(nMyClientIndex);
    if(NULL == pUnit)
    {
        return;
    }    
    D3DXVECTOR3    vVel, vUp, vPos;
    vVel = D3DXVECTOR3(0,0,1);
    vUp = D3DXVECTOR3(0,1,0);
    vPos = pUnit->m_vPos;
    // 스캔 이펙트 생성
    CreateScanItemEffect(pUnit, vPos, vVel, vUp, i_fCheckEnableTime, RC_EFF_SCAN_ITEM);    

    // 스캔 위치 지정 
    m_pSkill->SetScanPosition(SCAN_ITEM, vPos);
    
    INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_SHOW_MAP_EFFECT, T_FC_CHARACTER_SHOW_MAP_EFFECT, pSMode, SendBuf);
    pSMode->avec3Position    = vPos;
    pSMode->avec3Target        = vVel*1000.0f;
    pSMode->avec3Up            = vUp*1000.0f;
    pSMode->nLifetime        = (int)i_fCheckEnableTime;
    pSMode->EffectIndex        = RC_EFF_SCAN_ITEM;
    
    g_pD3dApp->m_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_SHOW_MAP_EFFECT));    
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        스캔 아이템 사용 및 취
/// \author        // 2007-04-19 by bhsohn 서치아이 아이템 추가
/// \date        2007-04-19 ~ 2007-04-19
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::UseScanItem(BOOL i_bScan, float i_fCheckEnableTime, float i_fScanRange,INT EffectChange)
{    
// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#ifdef C_SEARCHEYE_SHUTTLE_POSITION_MSPARK
    if(RC_ITEM_SEARCHEYE_NUM == EffectChange)
    {
        m_pSkill->SetScanSkill(SCAN_ITEM, i_bScan);
        if(FALSE == i_bScan)
        {
            return;
        }
        m_pSkill->SetScanRange(SCAN_ITEM, i_fScanRange);

        g_pD3dApp->SetPlaySearchEyeEffect(this, RC_EFF_SEARCHEYE_SHUTTLE_POSITION, 0);
    }    
    else if(RC_ITEM_SEARCHEYE2_NUM == EffectChange)
    {
        m_pSkill->SetScanSkill(SCAN_ITEM2, i_bScan);
        if(FALSE == i_bScan)
        {
            return;
        }
        m_pSkill->SetScanRange(SCAN_ITEM2, i_fScanRange);

        g_pD3dApp->SetPlaySearchEyeEffect(this, RC_EFF_SEARCHEYE_SHUTTLE_POSITION2, 0);
    }    
#else
    m_pSkill->SetScanSkill(SCAN_ITEM, i_bScan);
    if(FALSE == i_bScan)
    {
        return;
    }
    m_pSkill->SetScanRange(SCAN_ITEM, i_fScanRange);    
    
    ClientIndex_t nMyClientIndex = m_myShuttleInfo.ClientIndex;
    CUnitData *pUnit = g_pScene->FindUnitDataByClientIndex(nMyClientIndex);
    if(NULL == pUnit)
    {
        return;
    }    
    D3DXVECTOR3    vVel, vUp, vPos;
    vVel = D3DXVECTOR3(0,0,1);
    vUp = D3DXVECTOR3(0,1,0);
    vPos = pUnit->m_vPos;
    
    UINT    EffectNum = NULL;
    if(7037070 == EffectChange)
    {
        EffectNum = RC_EFF_SCAN_ITEM2;
    }
    else
    {
        EffectNum = RC_EFF_SCAN_ITEM;
    }
    
    // 스캔 이펙트 생성   
    CreateScanItemEffect(pUnit, vPos, vVel, vUp, i_fCheckEnableTime, EffectNum);    
    
    // 스캔 위치 지정 
    m_pSkill->SetScanPosition(SCAN_ITEM, vPos);
    
    INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_SHOW_MAP_EFFECT, T_FC_CHARACTER_SHOW_MAP_EFFECT, pSMode, SendBuf);
    pSMode->avec3Position    = vPos;
    pSMode->avec3Target        = vVel*1000.0f;
    pSMode->avec3Up            = vUp*1000.0f;
    pSMode->nLifetime        = (int)i_fCheckEnableTime;
    pSMode->EffectIndex        = EffectNum;
    
    g_pD3dApp->m_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_SHOW_MAP_EFFECT));    
#endif
// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2007-04-19 by bhsohn 서치아이 아이템 추가
/// \date        2007-04-19 ~ 2007-04-19
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CreateScanItemEffect(CUnitData *pUnit, 
                                        D3DXVECTOR3 vPos, D3DXVECTOR3 vVel, D3DXVECTOR3 vUp, 
                                        float i_fCheckEnableTime,
                                        UINT i_uEffectId)
{        
    // 자신이 쓴 스캔일 경우는 눈을 생성하고 아니라면 Enemy->UseSkillFromServer에서 생성한다.    
    g_pScene->AddFieldItemScanObject(pUnit, i_fCheckEnableTime, i_uEffectId, _LAY, vVel, vUp, &vPos);    
}

ClientIndex_t CShuttleChild::GetTargetClientIndext()
{
    ClientIndex_t clientIdx = 0;
    if(NULL == m_pOrderTarget)
    {
        return clientIdx;
    }
    
    DWORD dwPartType = m_pOrderTarget->m_dwPartType;
    ClientIndex_t AttackIndex = 0; 
    switch(dwPartType)
    {
    case _ENEMY:
        {
            clientIdx = ((CEnemyData*)m_pOrderTarget)->m_infoCharacter.CharacterInfo.ClientIndex;
        }
        break;
    case _MONSTER:
        {
            clientIdx = ((CMonsterData*)m_pOrderTarget)->m_info.MonsterIndex;
        }
        break;
    }    
    return clientIdx;
}
ClientIndex_t CShuttleChild::GetTargetToClientIdx(CAtumData* pTarget)
{
    ClientIndex_t clientIdx = 0;
    if(NULL == pTarget)
    {
        return clientIdx;
    }
    DWORD dwPartType = pTarget->m_dwPartType;
    switch(dwPartType)
    {
    case _ENEMY:
        {
            clientIdx = ((CEnemyData*)pTarget)->m_infoCharacter.CharacterInfo.ClientIndex;
        }
        break;
    case _MONSTER:
        {
            clientIdx = ((CMonsterData*)pTarget)->m_info.MonsterIndex;
        }
        break;
    }    
    return clientIdx;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        각 기어별 이륙 시간 체크
/// \author        // 2007-05-15 by bhsohn A기어 보완
/// \date        2007-05-15 ~ 2007-05-15
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float CShuttleChild::GetRasingCheckTime()
{
    if(IS_DT(m_myShuttleInfo.UnitKind))
    {
        // A기어냐?
        return AGEAR_RASING_TIME;
    }
    return 1.0f;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        풀스탯 정보 업데이트 
/// \author        // 2007-05-15 by bhsohn 기어 스탯 관련 처리
/// \date        2007-05-15 ~ 2007-05-15
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetMyShuttleFullStatInfo(GEAR_STAT i_MyShuttleFullStatInfo)
{
    m_myShuttleFullStatInfo = i_MyShuttleFullStatInfo;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        풀스탯 정보 알려줌. 
/// \author        // 2007-05-15 by bhsohn 기어 스탯 관련 처리
/// \date        2007-05-15 ~ 2007-05-15
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
GEAR_STAT CShuttleChild::GetMyShuttleFullStatInfo()
{
    return m_myShuttleFullStatInfo;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        최대 값을 못 넘게 변경 
/// \author        // 2007-05-15 by bhsohn 기어 스탯 관련 처리
/// \date        2007-05-15 ~ 2007-05-15
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::ReformMyShuttleStatInfo(GEAR_STAT* pMyGearStat)
{
    // 2010. 02. 19 by ckPark 100레벨미만일시 300스탯초과량이 적용되는 버그 수정
    
//     // 공격파트
//     if(pMyGearStat->AttackPart > COUNT_MAX_STAT_POINT)
//     {
//         pMyGearStat->AttackPart = COUNT_MAX_STAT_POINT;
//     }
// 
//     // 방어파트
//     if(pMyGearStat->DefensePart > COUNT_MAX_STAT_POINT)
//     {
//         pMyGearStat->DefensePart = COUNT_MAX_STAT_POINT;
//     }
// 
//     // 연료파트
//     if(pMyGearStat->FuelPart > COUNT_MAX_STAT_POINT)
//     {
//         pMyGearStat->FuelPart = COUNT_MAX_STAT_POINT;
//     }
// 
//     // 감응파트
//     if(pMyGearStat->SoulPart > COUNT_MAX_STAT_POINT)
//     {
//         pMyGearStat->SoulPart = COUNT_MAX_STAT_POINT;
//     }
// 
//     // 쉴드파트
//     if(pMyGearStat->ShieldPart > COUNT_MAX_STAT_POINT)
//     {
//         pMyGearStat->ShieldPart = COUNT_MAX_STAT_POINT;
//     }
// 
//     // 회피파트, 0 ~ 255의 확률
//     if(pMyGearStat->DodgePart > COUNT_MAX_STAT_POINT)
//     {
//         pMyGearStat->DodgePart = COUNT_MAX_STAT_POINT;
//     }

    for( int i=0; i<6; ++i )
    {
        Stat_t* pStat;

        switch( i )
        {
        case 0:
            pStat = &(pMyGearStat->AttackPart);
            break;
        case 1:
            pStat = &(pMyGearStat->DefensePart);
            break;
        case 2:
            pStat = &(pMyGearStat->FuelPart);
            break;
        case 3:
            pStat = &(pMyGearStat->SoulPart);
            break;
        case 4:
            pStat = &(pMyGearStat->ShieldPart);
            break;
        case 5:
            pStat = &(pMyGearStat->DodgePart);
            break;
        }

        if( m_myShuttleInfo.Level <= CHARACTER_LEVEL_100
            && (*pStat) > CHARACTER_LEVEL_100_MAX_STAT_POINT )
        {
            (*pStat) = CHARACTER_LEVEL_100_MAX_STAT_POINT;
        }
        else if( m_myShuttleInfo.Level > CHARACTER_LEVEL_100
                 && (*pStat) > CHARACTER_LEVEL_110_MAX_STAT_POINT )
        {
            (*pStat) = CHARACTER_LEVEL_110_MAX_STAT_POINT;
        }
    }

    // end 2010. 02. 19 by ckPark 100레벨미만일시 300스탯초과량이 적용되는 버그 수정
}

// 2007-05-28 by bhsohn 아이템에 대한 체크섬 추가

// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
// BOOL CShuttleChild::GetCheckSum(UINT *o_puiCheckSum, int *o_pnFileSize, char* pFilePath)
// {
//     return m_pGameData->GetCheckSum(o_puiCheckSum, o_pnFileSize, pFilePath);
// }

BOOL CShuttleChild::GetCheckSum(BYTE o_byObjCheckSum[32], int *o_pnFileSize, char* pFilePath)
{
    return m_pGameData->GetCheckSum(o_byObjCheckSum, o_pnFileSize, pFilePath);
}
// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CMonsterData::()
/// \brief        오브젝트를 타켓 잡을지 말지 결정
/// \author        // 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리    
/// \date        2007-05-17 ~ 2007-05-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsCheckObjectColl(CAtumData *pTarget)
{
    BOOL bTargetEnemyInfo = FALSE;
    if(NULL == pTarget)
    {
        return bTargetEnemyInfo;
    }
    switch(pTarget->m_dwPartType)
    {
    case _ENEMY:
    case _MONSTER:
        {
            CUnitData* pEnemyUnit = (CUnitData*)pTarget;            
            bTargetEnemyInfo = pEnemyUnit->IsCheckObjectColl();    // 충돌체크
        }
        break;        
    default:
        {
            return bTargetEnemyInfo;
        }
    }    
    
    return bTargetEnemyInfo;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        현재 있는 맵에 대한 정보
/// \author        // 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
/// \date        2007-06-04 ~ 2007-06-04
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
const CHARACTER& CShuttleChild::GetMyShuttleInfo() const
{
    return m_myShuttleInfo;
}

CHARACTER& CShuttleChild::GetMyShuttleInfo()
{
    return m_myShuttleInfo;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date        2007-06-12 ~ 2007-06-12
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
PARTYINFO CShuttleChild::GetPartyInfo()
{
    return m_pClientParty->GetPartyInfo();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        미션맵으로 보내기
/// \author        // 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date        2007-06-12 ~ 2007-06-12
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::SendGoMissionMap()
{    
    int nMapQuestIndex = g_pGameMain->GetMissionMapQuestIdx();    
    if(nMapQuestIndex)
    {
        INIT_MSG_WITH_BUFFER(MSG_FC_QUEST_MOVE_QUEST_MAP, T_FC_QUEST_MOVE_QUEST_MAP,
            pSQuestMove, SendBuf);
        pSQuestMove->QuestIndex0 = nMapQuestIndex;
        g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_QUEST_MOVE_QUEST_MAP));
        return TRUE;
    }
    return FALSE;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        // 유닛 정보 초기화 
/// \author        // 2007-07-04 by bhsohn 랜딩 모션중 포탈 탔을시 버그 수정
/// \date        2007-07-04 ~ 2007-07-04
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::InitUnitState()
{
    m_bOrderMove = FALSE;
    g_pD3dApp->m_vecCharacterOtherInfo.clear();
//    g_pD3dApp->m_vecCharacterOtherInfoRequest.clear();
    g_pD3dApp->m_vecCharacterOtherRenderInfo.clear();
//    g_pD3dApp->m_vecCharacterOtherRenderInfoRequest.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CShuttleChild::IsWeaponeTarget()
/// \brief        1형 2형 무기 발사시 타격을 줄수있는 상태인가?
/// \author        dgwoo
/// \date        2007-07-25 ~ 2007-07-25
/// \warning    
///
/// \param        2형무기인가?
/// \return        공격할경우 타겟이 맞으면 True 허공에 맞으면 False
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsWeaponeTarget(BOOL i_bSecondW)
{
    if(i_bSecondW)
    {
        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        //if(m_pSecondaryWeapon->m_vecTargetIndex.empty() == FALSE)
        if(m_pSecondaryWeapon->m_vecTargetIndexData.empty() == FALSE)
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        {
            return TRUE;
        }
    }
    else
    {
        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        //if(m_pPrimaryWeapon->m_vecTargetIndex.empty() == FALSE)
        if(m_pPrimaryWeapon->m_vecTargetIndexData.empty() == FALSE)
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        {
            return TRUE;
        }
    }
    return FALSE;
}
void CShuttleChild::InitWeaponeTarget()
{
    if(m_pSecondaryWeapon)
    {
        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        m_pSecondaryWeapon->m_vecTargetIndexData.clear();
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
    }
        if(m_pPrimaryWeapon)
    {
        // 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
        m_pPrimaryWeapon->m_vecTargetIndexData.clear();
        // end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
    }
}
void CShuttleChild::RenderInvenBack()
{
    D3DXMATRIX pTemp, pMatrix, pMatBack, pMatOldProj, pMatOldView, pMatPresView,pMatBackProj;
    D3DXMatrixIdentity(&pMatrix);
    D3DXMatrixIdentity(&pMatBack);
    D3DXMatrixIdentity(&pTemp);
    D3DXMatrixIdentity(&pMatOldView);
    D3DXMatrixIdentity(&pMatPresView);
    D3DXMatrixIdentity(&pMatBackProj);
    

    RECT WindowRect;
    GetClientRect(g_pD3dApp->GetHwnd(), &WindowRect);
    D3DXMatrixOrthoLH(&pMatBackProj, 
                     (float)WindowRect.right, 
                     (float)WindowRect.bottom, 
                     0.0f, 100.0f);

    g_pD3dDev->GetTransform( D3DTS_VIEW,        &pMatOldView);
    g_pD3dDev->GetTransform( D3DTS_PROJECTION, &pMatOldProj );    
    g_pD3dDev->SetTransform( D3DTS_VIEW,        &pMatPresView);
    g_pD3dDev->SetTransform( D3DTS_PROJECTION, &pMatBackProj);    
    
    // 백그라운드 렌더링
    // Restore the modified render states
    g_pD3dDev->SetRenderState( D3DRS_CLIPPLANEENABLE, 0x00 );
    g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    
    // Finally, render the mirror itself (as an alpha-blended quad)
    g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
    g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );    
    g_pD3dDev->SetRenderState( D3DRS_ZENABLE, FALSE );
    
    int nScreenWidth = -(int)g_pD3dApp->GetBackBufferDesc().Width / 2;
    int nScreenHeight = ((float)g_pD3dApp->GetBackBufferDesc().Height / 2) + 0.5f;
    int nWindowPosY = g_pGameMain->m_nLeftWindowY;

    D3DXMatrixTranslation(&pTemp, 
                            nScreenWidth, 
                            nScreenHeight - nWindowPosY, 
                            0.0f);

    pMatBack = pMatBack * pTemp;

    // 백그라운드        
    g_pD3dDev->SetTransform( D3DTS_WORLD, &pMatBack );
    g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE );
    g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

    g_pD3dDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
    g_pD3dDev->SetTransform( D3DTS_VIEW, &pMatOldView);    
    g_pD3dDev->SetTransform( D3DTS_PROJECTION, &pMatOldProj);    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CShuttleChild::IsRevivalUseItem()
/// \brief        부활 아이템을 사용할 조건이 되는가?
/// \author        dgwoo
/// \date        2007-10-08 ~ 2007-10-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsRevivalUseItem()
{
    MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);    
    if(IS_MAP_INFLUENCE_EVENT_AREA(pMapInfo->MapInfluenceType) ||   // 2008-04-08 by dgwoo && -> || 변경.
    IS_MAP_INFLUENCE_ARENA(pMapInfo->MapInfluenceType))
    {
        return FALSE;
    }

    // 일반 세력은 맵과 상관없이 사용가능하다.
    if(IS_NORMAL_INFLUENCE_TYPE(m_myShuttleInfo.InfluenceType))
    {
        return TRUE;
    }

    // 2007-10-08 by dgwoo 안전지역은 모두 TRUE
    if(MAP_INFLUENCE_NEUTRALITY == pMapInfo->MapInfluenceType                    // 중립지역이거나
        || IS_MAP_INFLUENCE_CONFLICT_AREA(pMapInfo->MapInfluenceType)            // 분쟁지역이거나
        || m_myShuttleInfo.MapChannelIndex.ChannelIndex > 0)                    // 안전 채널이면 사용가능.
    {
        return TRUE;
    }

    // 거점 관련.
    if(g_pInterface->m_pINFOpMain->IsRevivalUseItemWarMonster(m_myShuttleInfo.MapChannelIndex.MapIndex))
    {// 현재 내가 있는 맵이 거점전과 관련이 있는 맵이면.
        return FALSE;
    }

    // 전진기지전 관련.
    if(IS_MAP_INFLUENCE_OUTPOST(pMapInfo->MapInfluenceType))
    {// 전진기지 맵에선 부활 아이템 사용금지.
        return FALSE;
    }

    // 모선전 관련.
    // 2008-04-04 by bhsohn 모선전 관리 클래스 추가
    if(g_pGameMain->GetSummonMotherShipCnt())
    {
        if(IS_MAP_INFLUENCE_VCN(pMapInfo->MapInfluenceType))
        {
            return g_pGameMain->GetMotherShipInfl(MOTHERSHIP_INFLUENCE_ANI);
        }
        else if(IS_MAP_INFLUENCE_ANI(pMapInfo->MapInfluenceType))
        {
            return g_pGameMain->GetMotherShipInfl(MOTHERSHIP_INFLUENCE_VCN);
        }

    }
    return TRUE;
    
}

CEffectInfo* CShuttleChild::GetEffectWeapon(int nInvenIdx)
{
    char *pObjName = m_vectInvenWeaponMesh[nInvenIdx].pInvenWeaponMesh->GetMeshObjFileName();
    CEffectInfo* pEffectInfo = g_pD3dApp->m_pEffectRender->GetObjEffectInfo(pObjName);
    if(NULL == pEffectInfo)
    {
        return NULL;
    }                
    if(NULL == pEffectInfo->m_pEffect)
    {
        return NULL;
    }
    
    return pEffectInfo;
    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2007-12-05 by bhsohn 벽에 뚫고 들어가는 현상 처리
/// \date        2007-12-05 ~ 2007-12-05
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::CheckObjLandingColl(D3DXVECTOR3 vNormalVector )
{    
    D3DXVECTOR3 vObjNormalVector = vNormalVector;
    D3DXVECTOR3 vObjSide = vObjNormalVector;
    
    D3DXVec3Normalize(&vObjNormalVector,&vObjNormalVector);
    
    vObjSide.y = 0;
    D3DXVec3Normalize(&vObjSide,&vObjSide);
    
    float fAngleRadian = GetRadianVectorBetweenVector(vObjNormalVector, vObjSide);    
    float fDo = 90.0f - (fAngleRadian*57.29577951f);
    DBGOUT("fDo [%.2f] \n", fDo);    
    // 10도 넘으면 안된다. 
    if(fDo > MAX_LADING_DO)
    {
        return FALSE;
    }
    return TRUE;    
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2007-12-05 by bhsohn 벽에 뚫고 들어가는 현상 처리
/// \date        2007-12-05 ~ 2007-12-05
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::CancelLandingMode()
{
    if(g_pTutorial->IsTutorialMode() == FALSE)
    {
        //if(m_fNumpad0keyTime <= 0.0f)
        {
            // 착륙전 시점이 1인칭이였으면 다시 1인칭으로 변환 
            if(m_bLandingOldCamtypeFPS)
            {
                g_pD3dApp->m_pCamera->ChangeCamType();
                m_bLandingOldCamtypeFPS = FALSE;
            }
            
            SendPacket(T_FC_MOVE_TAKEOFF);
            DBGOUT("T_FC_MOVE_TAKEOFF\n");
            g_pCamera->Init(m_vPos - 10.0f*m_vVel,m_vVel, 3.14f/13.0f, 60.0f);
            m_fNumpad0keyTime = 2.0f;
            ChangeUnitState(_TAKINGOFF);
            // 2007-05-15 by bhsohn A기어 보완
            //m_fRasingCheckTime = 1.0f;
            m_fRasingCheckTime = GetRasingCheckTime();
        }
        return TRUE;
    }
    return FALSE;
    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2008-01-16 by bhsohn A기어 워프 실패시 위장,그라운드 엑셀 취소 시킴
/// \date        2008-01-16 ~ 2008-01-16
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetShuttleAirMode(BOOL bIsAir)
{
    m_bIsAir = bIsAir;
    if(bIsAir && IS_DT(m_myShuttleInfo.UnitKind) && (m_pSkill))
    {
        // 기체가 떠있는 상태에서 A기어 그라운드 기술을 끔.
        m_pSkill->ReleaseGroundSkill();            
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2007-11-22 by bhsohn 아레나 통합서버
/// \date        2007-11-22 ~ 2007-11-22
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
UID32_t    CShuttleChild::GetCharacterUniqueNumber()
{
    if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
    {
        CHARACTER*    pArenaCharacter = g_pD3dApp->GetArenaCharacterInfo();
        return pArenaCharacter->CharacterUniqueNumber;
    }
    return m_myShuttleInfo.CharacterUniqueNumber;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2008-03-17 by bhsohn 윈도우 이동시 Tick안들어오는 문제 처리
/// \date        2008-03-17 ~ 2008-03-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsUnitStop()
{
    return m_bUnitStop;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2008-03-17 by bhsohn 윈도우 이동시 Tick안들어오는 문제 처리
/// \date        2008-03-17 ~ 2008-03-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsAir()
{
    return m_bIsAir;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        공개설정 옵션 변경
/// \author        // 2008-06-20 by bhsohn EP3 옵션관련 처리
/// \date        2008-06-25 ~ 2008-06-25
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetSecretInfoOption(INT i_nSecretInfoOption)
{
    // 2008-12-16 by dgwoo 미션마스터.

    if((m_myShuttleInfo.SecretInfoOption & USER_INFO_OPTION_MISSIONMASTER) != (i_nSecretInfoOption & USER_INFO_OPTION_MISSIONMASTER))
    {
        if(i_nSecretInfoOption & USER_INFO_OPTION_MISSIONMASTER)
        {// 미션마스터로 등록.
            g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081219_0100,COLOR_SYSTEM);
        }else
        {// 미션마스터에서 해제.
            g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081219_0101,COLOR_SYSTEM);
        }
        m_myShuttleInfo.SecretInfoOption = i_nSecretInfoOption;    
        g_pGameMain->m_pMissionMain->UpdateMissionMasterButton();
    }else
    {
        char buf[256];
        wsprintf(buf, STRMSG_C_080609_0200); //"\\y옵션이 변경되었습니다."
        g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);    
    }
    m_myShuttleInfo.SecretInfoOption = i_nSecretInfoOption;    
    
}
void CShuttleChild::SetAmorHeight(INT i_nAmorNum)
{
    m_fAmorHeight = GetAmorGearHeight(i_nAmorNum);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date        2008-09-08 ~ 2008-09-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
D3DXMATRIX CShuttleChild::GetMatWeaponPosition(int nIdx)
{
    if(nIdx < 0 || nIdx >= 4)
    {
        D3DXMATRIX MatInven;
        D3DXMatrixIdentity(&MatInven);
        return MatInven;
    }

    return m_pMatWeaponPosition[nIdx];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date        2008-09-08 ~ 2008-09-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int CShuttleChild::GetInvenWeaponMeshSize()
{
    return m_vectInvenWeaponMesh.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date        2008-09-08 ~ 2008-09-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
INVEN_WEAPON_MESH* CShuttleChild::GetInvenWeaponMesh(int nIdx)
{
    if(nIdx < 0 || nIdx>=m_vectInvenWeaponMesh.size())
    {
        return NULL;
    }
    return &m_vectInvenWeaponMesh[nIdx];
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date        2008-09-08 ~ 2008-09-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float CShuttleChild::GetRotationX()
{
    return m_fRotationX;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        
/// \author        // 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date        2008-09-08 ~ 2008-09-08
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float CShuttleChild::GetRotationZ()
{
    return m_fRotationZ;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        // 부스터 시간
/// \author        // 2008-09-26 by bhsohn 신규 인첸트 처리
/// \date        2008-09-26 ~ 2008-09-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float CShuttleChild::GetEnchantDesParam(CItemInfo* pItemInfo, int nDestParam)
{
    float fBoosterTimeUp = 0.0f;
    
    //ITEM        *pRefITEM = pItemInfo->GetItemInfo();
    RARE_ITEM_INFO *pRefPrefixRareInfo = pItemInfo->GetPrefixRareInfo();
    RARE_ITEM_INFO *pRefSuffixRareInfo = pItemInfo->GetSuffixRareInfo();
    CParamFactor *    pRefEnchant = pItemInfo->GetEnchantParamFactor();
    if(!pRefEnchant)
    {
        return fBoosterTimeUp;
    }
    fBoosterTimeUp = GetParamFactor_DesParam((*pRefEnchant), nDestParam);

    return fBoosterTimeUp;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        // 엔진 인첸 정보 갱신
/// \author        // 2008-09-26 by bhsohn 신규 인첸트 처리
/// \date        2008-09-26 ~ 2008-09-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::UpdateEngineInfo()
{
    // 엔진 아이템 가져오기
    CItemInfo* pEngineItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR ); 
    if(NULL == pEngineItem)
    {
        return;
    }
    ITEM* pITEM = pEngineItem->GetRealItemInfo();
    
    // 부스터 추가 시간 
    {
        m_fBURN = (float)pITEM->Time/1000.0f;        
        float fTimeUp = GetEnchantDesParam(pEngineItem, DES_ENGINE_BOOSTER_TIME_UP);
        m_fBURN += fTimeUp;        
        // FLOAT형 재정렬 하여 사용
        m_fBURN = FloatRangeDecimal(m_fBURN, 2);
    }

    // 최소 속도
    {
        m_fShuttleSpeedMin = pITEM->AbilityMin;
        float fTime = GetEnchantDesParam(pEngineItem, DES_ENGINE_MIN_SPEED_UP);
        m_fShuttleSpeedMin += fTime;

    }
    // 최대 속도
    {
        m_fShuttleSpeedMax = pITEM->AbilityMax;
        float fTime = GetEnchantDesParam(pEngineItem, DES_ENGINE_MAX_SPEED_UP);
        m_fShuttleSpeedMax += fTime;        
    }

    // 지상 속도
    {
        m_fShuttleGroundSpeedMax = pITEM->Charging;            // 2005-11-26 by ispark
        float fTime = GetEnchantDesParam(pEngineItem, DES_ENGINE_GROUND_SPEED_UP);
        m_fShuttleGroundSpeedMax += fTime;        
    }

    // 부스터 속도
    {
        m_fShuttleSpeedBoosterOn = (float)pITEM->Range;
        float fTime = GetEnchantDesParam(pEngineItem, DES_ENGINE_BOOSTER_SPEED_UP);
        m_fShuttleSpeedBoosterOn += fTime;        
    }    
    
    // 하이퍼 무빙(선회각)
    {    
        CParamFactor*    pRefEnchant = pEngineItem->GetEnchantParamFactor();        
        if(pRefEnchant)
        {
            m_paramFactor.pfm_ENGINE_ANGLE_UP = pRefEnchant->pfm_ENGINE_ANGLE_UP;
            m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = pRefEnchant->pfm_ENGINE_BOOSTERANGLE_UP;
        }
        else
        {
            // 초기화            
            m_paramFactor.pfm_ENGINE_ANGLE_UP = m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = 0.0f;
        }
        
        m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pITEM, &m_paramFactor);
    }

    // 엔진 UI갱신
    if(g_pGameMain && g_pGameMain->m_pUnitInfoBar)
    {
        g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
        g_pGameMain->m_pUnitInfoBar->SetSpeed(m_fShuttleSpeed, m_fShuttleSpeedMin, m_fShuttleSpeedBoosterOn);
    }

    SetShuttleSpeed(); // 스피드 갱신
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        // 엔진 인첸 정보 갱신
/// \author        // 2008-09-26 by bhsohn 신규 인첸트 처리
/// \date        2008-09-26 ~ 2008-09-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetRadarRange(CItemInfo* i_pRadarItemInfo)
{
    CParamFactor*    pRefEnchant = i_pRadarItemInfo->GetEnchantParamFactor();
    
    // 2011. 2. 25 by jskim 보급 상점 착륙시 레이더 초기화 되는 버그 수정
     if(pRefEnchant)
     {
         m_paramFactor.pfm_ATTACK_RANGE_01 = pRefEnchant->pfm_ATTACK_RANGE_01;
         m_paramFactor.pfm_ATTACK_RANGE_02 = pRefEnchant->pfm_ATTACK_RANGE_02;
     }
     else
     {
         // 초기화
         m_paramFactor.pfm_ATTACK_RANGE_01 = m_paramFactor.pfm_ATTACK_RANGE_02 = 0.0f;
     }
    if(pRefEnchant)
    {
        m_paramFactor.pfm_ATTACK_RANGE_01 = pRefEnchant->pfm_ATTACK_RANGE_01;
        m_paramFactor.pfm_ATTACK_RANGE_02 = pRefEnchant->pfm_ATTACK_RANGE_02;
     }
    // end 2011. 2. 25 by jskim 보급 상점 착륙시 레이더 초기화 되는 버그 수정
    // 2008-11-13 by bhsohn EP3-2 에서 레이더 사정거리 증가 펫 적용 안되는 문제 해결
    // 2009-05-14 by bhsohn 레이더 변경시, 펫이 아닌 아이템에서 사거리 적용 안되는 문제 해결    
//    CItemInfo* pPetItem = m_pStoreData->FindItemInInventoryByWindowPos(POS_PET);
//    if(pPetItem)
//     {
//         // 2009-04-21 by bhsohn 아이템 DesParam추가    
//         //     if(m_wearItemParamFactor[POS_PET].DestParameter1 == DES_ATTACK_RANGE_01 
//         //             || m_wearItemParamFactor[POS_PET].DestParameter1 == DES_ATTACK_RANGE_02 )
//         //         {
//         //             SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[POS_PET].DestParameter1, 
//         //                 m_wearItemParamFactor[POS_PET].ParameterValue1 );
//         //         }
//         //         if(m_wearItemParamFactor[POS_PET].DestParameter2 == DES_ATTACK_RANGE_01 
//         //             || m_wearItemParamFactor[POS_PET].DestParameter2 == DES_ATTACK_RANGE_02 )
//         //         {
//         //             SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[POS_PET].DestParameter2, 
//         //                 m_wearItemParamFactor[POS_PET].ParameterValue2 );
//         //         }
//         // 
//         //         if(m_wearItemParamFactor[POS_PET].DestParameter3 == DES_ATTACK_RANGE_01 
//         //             || m_wearItemParamFactor[POS_PET].DestParameter3 == DES_ATTACK_RANGE_02 )
//         //         {
//         //             SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[POS_PET].DestParameter3, 
//         //                 m_wearItemParamFactor[POS_PET].ParameterValue3 );
//         //         }
//         // 
//         //         if(m_wearItemParamFactor[POS_PET].DestParameter4 == DES_ATTACK_RANGE_01 
//         //             || m_wearItemParamFactor[POS_PET].DestParameter4 == DES_ATTACK_RANGE_02 )
//         //         {
//         //             SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[POS_PET].DestParameter4, 
//         //                 m_wearItemParamFactor[POS_PET].ParameterValue4 );
//         //         }
//         int nArrParamCnt = 0;
//         for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
//         {
//             if(m_wearItemParamFactor[POS_PET].ArrDestParameter[nArrParamCnt] == DES_ATTACK_RANGE_01 
//                 || m_wearItemParamFactor[POS_PET].ArrDestParameter[nArrParamCnt] == DES_ATTACK_RANGE_02 )
//             {
//                 SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[POS_PET].ArrDestParameter[nArrParamCnt], 
//                     m_wearItemParamFactor[POS_PET].ArrParameterValue[nArrParamCnt] );
//             }
//             
//         }                
//         // end 2009-04-21 by bhsohn 아이템 DesParam추가    
//     }        


    // 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수값 변경.
    //for(int nEqId=0; nEqId<WEAR_ITEM_NUMBER; nEqId++)
    for( int nEqId=0; nEqId < MAX_EQUIP_POS; nEqId++ )
    {
        CItemInfo* pEqItem = m_pStoreData->FindItemInInventoryByWindowPos(nEqId);
        if(pEqItem)
        {            
            int nArrParamCnt = 0;
            for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
            {
                if(m_wearItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt] == DES_ATTACK_RANGE_01 
                    || m_wearItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt] == DES_ATTACK_RANGE_02 )
                {
                    SetParamFactorDesParam(m_paramFactor, m_wearItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt], 
                        m_wearItemParamFactor[nEqId].ArrParameterValue[nArrParamCnt] );
                }
                
            }                            
        }
    }
    // end 2009-05-14 by bhsohn 레이더 변경시, 펫이 아닌 아이템에서 사거리 적용 안되는 문제 해결


    // 2011. 2. 25 by jskim 보급 상점 착륙시 레이더 초기화 되는 버그 수정
    // 접두, 접미 능력치 중 레이더 거리 데스파람 적용
    for( int nEqId = 0; nEqId < MAX_EQUIP_POS; ++nEqId )
    {
        CItemInfo* pEqItem = m_pStoreData->FindItemInInventoryByWindowPos( nEqId );
        if( pEqItem )
        {            
            for( int nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; ++nArrParamCnt )
            {
                if( DES_ATTACK_RANGE_01 == m_wearPreRareItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt]                    
                ||    DES_ATTACK_RANGE_02 == m_wearPreRareItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt] )                    
                {
                    SetParamFactorDesParam(m_paramFactor,
                        m_wearPreRareItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt],
                        m_wearPreRareItemParamFactor[nEqId].ArrParameterValue[nArrParamCnt] );
                }
                
                if( DES_ATTACK_RANGE_01 == m_wearSufRareItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt]
                ||    DES_ATTACK_RANGE_02 == m_wearSufRareItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt] )
                {
                    SetParamFactorDesParam(m_paramFactor,
                        m_wearSufRareItemParamFactor[nEqId].ArrDestParameter[nArrParamCnt],
                        m_wearSufRareItemParamFactor[nEqId].ArrParameterValue[nArrParamCnt] );
                }
            }
        }
    }
    //end 2011. 2. 25 by jskim 보급 상점 착륙시 레이더 초기화 되는 버그 수정


    // 2009-02-25 by bhsohn 버프형 사거리 레이더 안되는 버그 수정
    if(g_pGameMain && g_pGameMain->m_pInfSkill)
    {
        g_pGameMain->m_pInfSkill->RefreshSkillRader();
    }
    // end 2009-02-25 by bhsohn 버프형 사거리 레이더 안되는 버그 수정
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        // 엔진 인첸 정보 갱신
/// \author        // 2008-09-26 by bhsohn 신규 인첸트 처리
/// \date        2008-09-26 ~ 2008-09-26
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
CParamFactor* CShuttleChild::GetShuttleParamFactor()
{
    return &m_paramFactor;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        조이스틱
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetCursorJostickPos()
{    
    CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
    if(!pJoyStick)
    {
        // 조이스틱 장치 없음
        return;
    }
    
    //A기어가 땅에 있을때
//    if(m_bIsAir == FALSE && IS_DT(m_myShuttleInfo.UnitKind))
//    {
//    }
//    else 
//    {
//        SetAirCursorJostickPos();
//    }
    // 2009-02-25 by bhsohn 조이스틱 이용, 메뉴창 띠우면 A기어는 마우스로 조절
    if(IS_DT(m_myShuttleInfo.UnitKind))
    {
        if(g_pGameMain)
        {
            GUI_BUILDINGNPC* pCurrentBuildingNpc = NULL;
            if(g_pInterface->m_pCityBase)
            {
                //현재 얼린 상점이있냐?
                pCurrentBuildingNpc = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
            }

            if((g_pGameMain->m_pCommunity && g_pGameMain->m_pCommunity->IsShowCommunityWindow())
                || (g_pGameMain->m_pCharacterInfo && g_pGameMain->m_pCharacterInfo->IsShowWnd())
                || (g_pGameMain->m_pInven && g_pGameMain->m_pInven->IsShowInven())
                || (g_pGameMain->m_pSystem && g_pGameMain->m_pSystem->IsOptionShow())
                || (pCurrentBuildingNpc))
            {
                // 창 띠우고 있으면 조이스틱안됨
                return;
            }
        }
    }
    // end 2009-02-25 by bhsohn 조이스틱 이용, 메뉴창 띠우면 A기어는 마우스로 조절

    SetAirCursorJostickPos();
    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        조이스틱
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetAirCursorJostickPos()
{
    if(!IsPossibleJoystickMove())
    {
        return;
    }
    
    FLOG( "CShuttleChild::SetCursorInit()" );
    POINT pt,pt2;
    GetCursorPos(&pt);
    pt2 = pt;
    
    ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
    
    int nMaxX = g_pD3dApp->GetBackBufferDesc().Width/2;
    int nMaxY = g_pD3dApp->GetBackBufferDesc().Height/2;
    
    int nX = pt2.x - pt.x;
    int nY = pt2.y - pt.y;
    {
        // 가운데 영역
        nX += nMaxX;
        nY += nMaxY;
    }
    
    int nJoyStick = 0;
    int nCurselValue = 0;
    
    nJoyStick = SetCursorJostickLeftRight(JOSTICK_OPTION_MOUSE_LEFT, nMaxX);    // 마우스 왼쪽 체크
    nX += nJoyStick;    
    
    nJoyStick = SetCursorJostickLeftRight(JOSTICK_OPTION_MOUSE_RIGHT, nMaxX);    // 마우스 오른쪽 체크
    nX += nJoyStick;        
    
    nJoyStick = SetCursorJostickUpDown(JOSTICK_OPTION_MOUSE_UP, nMaxY);            // 마우스 위로 체크
    nY += nJoyStick;    
    
    nJoyStick = SetCursorJostickUpDown(JOSTICK_OPTION_MOUSE_DOWN, nMaxY);            // 마우스 아래로 체크
    nY += nJoyStick;    
    
    
    SetCursorPos(nX,nY);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        왼쪽, 오른쪽 체크
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int  CShuttleChild::SetCursorJostickLeftRight(int nLeftRightIndex, int nMaxX)
{
    CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
    if(!pJoyStick)
    {
        // 조이스틱 장치 없음
        return 0;
    }
    else if(g_pGameMain && g_pGameMain->IsShowOpJoystick())
    {
        // 조이스틱 옵션창이 떠있다.
        return 0;
    }

    int nRtnJoyStatus = 0;
    int nJoyStick = 0;
    
    int nKeyIdx, nKeyValue;
    float fTmpVaule = 0.0f;
    nKeyIdx = nKeyValue = 0;

    // 마우스 제어는 한군데 밖에 못넣는다.    
    g_pInterface->GetJoystickInfo(nLeftRightIndex, 0, &nKeyIdx, &nKeyValue);
    if(-1 == nKeyIdx)
    {
        return nRtnJoyStatus;
    }
        
    // 휠방식인냐 키 방식이냐를 체크
    if(pJoyStick->IsJoystickWheel(nKeyIdx))
    {
        // 휠스크롤 방식이다.
        nJoyStick = pJoyStick->GetJoySitckInfo(nKeyIdx);
        if((nKeyValue >= 0 && nJoyStick >= 0)
            || (nKeyValue <= 0 && nJoyStick <= 0))
        {
            fTmpVaule = ((float)nJoyStick)/JOYSTIC_STEP_MAX_VALUE;    // 비율계산                    
            if(fTmpVaule > 1.0f)
            {
                fTmpVaule = 1.0f;
            }
            else if(fTmpVaule < -1.0f)
            {
                fTmpVaule = -1.0f;
            }        
            fTmpVaule = GetJoystckMoveValue(fTmpVaule);            
        }                
        
        fTmpVaule *= ((float)nMaxX);        
        nRtnJoyStatus = (int)fTmpVaule;
    }
    else
    {
        // 키 클릭 방식 이다.
        nJoyStick = pJoyStick->GetJoySitckInfo(nKeyIdx);
        if(nJoyStick > 0)
        {                
            if(JOSTICK_OPTION_MOUSE_LEFT == nLeftRightIndex)
            {
                nRtnJoyStatus = -1*nMaxX;
            }
            else
            {
                nRtnJoyStatus = nMaxX;
            }                
        }
    }
    
    return nRtnJoyStatus;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        왼쪽, 오른쪽 체크
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
int  CShuttleChild::SetCursorJostickUpDown(int nUpDownIndex, int nMaxY)
{
    CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
    if(!pJoyStick)
    {
        // 조이스틱 장치 없음
        return 0;
    }
    else if(g_pGameMain && g_pGameMain->IsShowOpJoystick())
    {
        // 조이스틱 옵션창이 떠있다.
        return 0;
    }
    int nRtnJoyStatus = 0;
    int nJoyStick = 0;
    
    int nKeyIdx, nKeyValue;
    float fTmpVaule = 0.0f;
    nKeyIdx = nKeyValue = 0;

    // 마우스 제어는 한군데 밖에 못넣는다.    
    g_pInterface->GetJoystickInfo(nUpDownIndex, 0, &nKeyIdx, &nKeyValue);
    if(-1 == nKeyIdx)
    {
        return nRtnJoyStatus;
    }
        
    // 휠방식인냐 키 방식이냐를 체크
    if(pJoyStick->IsJoystickWheel(nKeyIdx))
    {
        // 휠스크롤 방식이다.
        nJoyStick = pJoyStick->GetJoySitckInfo(nKeyIdx);
        if((nKeyValue >= 0 && nJoyStick >= 0)
            || (nKeyValue <= 0 && nJoyStick <= 0))
        {
            fTmpVaule = ((float)nJoyStick)/JOYSTIC_STEP_MAX_VALUE;    // 비율계산                    
            if(fTmpVaule > 1.0f)
            {
                fTmpVaule = 1.0f;
            }            
            else if(fTmpVaule < -1.0f)
            {
                fTmpVaule = -1.0f;
            }            
            fTmpVaule = GetJoystckMoveValue(fTmpVaule);
        }                
        
        fTmpVaule *= ((float)nMaxY);        
        nRtnJoyStatus = (int)fTmpVaule;
    }
    else
    {
        // 키 클릭 방식 이다.
        nJoyStick = pJoyStick->GetJoySitckInfo(nKeyIdx);
        if(nJoyStick > 0)
        {                
            if(JOSTICK_OPTION_MOUSE_UP == nUpDownIndex)
            {
                nRtnJoyStatus = -1*nMaxY;
            }
            else
            {
                nRtnJoyStatus = nMaxY;
            }                
        }
    }
    
    return nRtnJoyStatus;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        롤링이 가능한 상태다
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsPossibleRolling()
{
    if((g_pTutorial->IsTutorialMode() == FALSE 
        || (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() >= L3)) 
        &&m_bIsAir == TRUE 
        && m_dwState == _NORMAL)
    {        
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        롤링이 가능한 상태다
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::StartRolling(int nKeyIndex)
{
    if(!IsPossibleRolling())
    {
        return;
    }
    switch(nKeyIndex)
    {
    case DIK_A:
        {
            // 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
            ActiveKeyDownADouble();
//             if(m_fADoubleKeyTime < DOUBLEKEY_TIME 
//                 && m_bKeyDownACheck == FALSE 
//                 // 부스터 사용시에는 롤링을 저장하지 않는다.
//                 && m_nBooster != BOOSTER_TYPE_MAX)
//             {
//                 DbgOut("CShuttleChild::WndProc() : m_bKeyDownADouble = TRUE\n");
//                 m_bKeyDownADouble = TRUE;
//                 m_bKeyDownDDouble = FALSE;
//             }
//             else
//             {
//                 m_fADoubleKeyTime = 0.0f;
//             }
            // end 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
        }
        break;
    case DIK_D:
        {
            // 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
            ActiveKeyDownDDouble();
//             if(m_fDDoubleKeyTime < DOUBLEKEY_TIME 
//                 && m_bKeyDownDCheck == FALSE
//                 // 부스터 사용시에는 롤링을 저장하지 않는다.
//                 && m_nBooster != BOOSTER_TYPE_MAX)
//             {
//                 DbgOut("CShuttleChild::WndProc() : m_bKeyDownDDouble = TRUE\n");
//                 m_bKeyDownDDouble = TRUE;
//                 m_bKeyDownADouble = FALSE;
//             }
//             else
//             {
//                 m_fDDoubleKeyTime = 0.0f;
//             }
            // end 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
        }
        break;
    }

}
///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        롤링을 체크한다.
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::CheckJoystickKeyDown()
{
    CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
    if(!pJoyStick)
    {
        // 조이스틱 장치 없음
        return;
    }
    else if(!IsPossibleJoystickMove())
    {
        // 비행중이 아니다.
        return;
    }

    int nJoyStickRolling = 0;    
    BOOL bArrowKey = FALSE;

    if( g_pInterface->GetJostickStats(DIK_A) )
    {
        bArrowKey = TRUE;
        nJoyStickRolling = DIK_A;
        if(g_pInterface->IsChangeJoySitckInfo(DIK_A) 
            && (0 == m_nJoyStickRolling))
        {            
            StartRolling(DIK_A);    // 롤링 시작 체크
        }
    }
    else if(g_pInterface->GetJostickStats(DIK_D))
    {
        bArrowKey = TRUE;
        nJoyStickRolling = DIK_D;
        if(g_pInterface->IsChangeJoySitckInfo(DIK_D)
            && (0 == m_nJoyStickRolling))
        {            
            StartRolling(DIK_D);    // 롤링 시작 체크
        }
    }

    if(g_pInterface->GetJostickStats(DIK_LSHIFT) 
        && g_pInterface->IsChangeJoySitckInfo(DIK_LSHIFT))
    {
        // Target UnLock
        SetJoystickUnLockOn();
    }

    int nSlotIdx = 0;
    for(nSlotIdx = 0;nSlotIdx < MAX_JOSTICK_OPTION_BEHAVIOR;nSlotIdx++)
    {
        {
            // 1형무기
            BOOL bWeaponChangeKey1 = FALSE;
            int nKeyIdx, nKeyValue;
            nKeyIdx = nKeyValue = 0;
            
            g_pInterface->GetJoystickInfo(JOSTICK_OPTION_WEAPON_1, nSlotIdx, &nKeyIdx, &nKeyValue);
            
            if(nKeyIdx != -1)
            {
                bWeaponChangeKey1 = pJoyStick->IsChangeJoySitckInfo(nKeyIdx) ;
            }
            
            if(bWeaponChangeKey1)
            {
                int nJoyStickStas = pJoyStick->GetJoySitckInfo(nKeyIdx);
                
                // 기본무기
                if(nJoyStickStas)
                {
                    if(    m_pSkill && m_pSkill->GetSkillTargetState() == TRUE )
                    {
                        //    내가 지금 타겟 스킬을 사용하기 위해 타겟을 찾는 중이라면..
                        //    총알이 발사되지 않고, 대신에 스킬 클래스에 있는 타겟을 찾는 함수를 호출한다.
                        m_pSkill->FindTargetForSkill();
                    }
                    // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음                    
                    //else if(!m_bLButtonState  && (IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster)))
                    else if(!m_bLButtonState  && (IS_WEAPON_ENABLESTATE(m_dwState, m_nBooster))&& IsPossibleAttackStat())
                    {
                        //기본 무기 발사
                        m_bLButtonState = TRUE;
                    }
                }            
                else if(m_bLButtonState && !nJoyStickStas)
                {
                    // 기본무기 발사 취소
                    m_bLButtonState = FALSE;
                    m_PrimaryAttack.AttackData.TargetInfo.SetNullTarget();
                    m_bTargetChange = TRUE;
                    
                }
            }
        }
        {
            // 2형무기
            BOOL bWeaponChangeKey2 = FALSE;
            int nKeyIdx, nKeyValue;
            nKeyIdx = nKeyValue = 0;
            
            g_pInterface->GetJoystickInfo(JOSTICK_OPTION_WEAPON_2, nSlotIdx, &nKeyIdx, &nKeyValue);
            
            if(nKeyIdx != -1)
            {
                bWeaponChangeKey2 = pJoyStick->IsChangeJoySitckInfo(nKeyIdx) ;
            }
            
            if(bWeaponChangeKey2)
            {
                int nJoyStickStas = pJoyStick->GetJoySitckInfo(nKeyIdx);
                // 고급무기
                // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음                                    
                //if(!m_bRButtonState && nJoyStickStas)
                if(!m_bRButtonState && nJoyStickStas && IsPossibleAttackStat())
                {            
                    m_bRButtonState = TRUE;
                }
                else if(m_bRButtonState && !nJoyStickStas)
                {            
                    m_bRButtonState = FALSE;
                    m_SecondaryAttack.AttackData.TargetInfo.SetNullTarget();
                    
                }         
            }    
        }
        {
            // 煥?    
            BOOL bKeyDown = FALSE;    
            BOOL bChangeKey = FALSE;
            int nKeyIdx, nKeyValue;
            int nJoyStickStas =0;
            nKeyIdx = nKeyValue = 0;
            
            g_pInterface->GetJoystickInfo(JOSTICK_OPTION_BACKVIEW, nSlotIdx, &nKeyIdx, &nKeyValue);
            
            if(nKeyIdx != -1)
            {
                bChangeKey = pJoyStick->IsChangeJoySitckInfo(nKeyIdx) ;
            }
            
            if(bChangeKey)
            {
                nJoyStickStas = pJoyStick->GetJoySitckInfo(nKeyIdx);
                if( nJoyStickStas&& !m_bTurnCamera)
                {
                    SetBackView(TRUE);
                }
                else if(m_bTurnCamera)
                {
                    SetBackView(FALSE);
                }
            }            
        }
    }
    
    

    {
        // 유닛정지
        if(g_pInterface->OnJoystickKeyDown(JOSTICK_OPTION_UNITSTOP))
        {
            OnKeyDownUnitStop();
        }
    }

    {
        // 출동
        if(g_pInterface->OnJoystickKeyDown(JOSTICK_OPTION_TAKEOFF))
        {            
        }
    }

    
    {
        // 편대 대형 참여
        if(g_pInterface->OnJoystickKeyDown(JOSTICK_OPTION_JOIN_FORMATION))
        {
            OnKeyDownJoinFormation();
        }
    }
    {
        // 스킬 슬롯 변경 
        if(g_pInterface->OnJoystickKeyDown(JOSTICK_OPTION_SLOT_CHANGE))
        {
            if(g_pGameMain && g_pGameMain->m_pQuickSlot)
            {
                g_pGameMain->m_pQuickSlot->AddCurrentTab();
            }
            
        }
    }
    // 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가
    {
        // 승인 버튼
        if(g_pInterface->OnJoystickKeyDown(JOSTICK_OPTION_OK))
        {
            // if i==0 then ok, if i==1 then cancel
            g_pGameMain->m_pInfWindow->OnButtonClick(0);
        }        
        // 취소 버튼
        else if(g_pInterface->OnJoystickKeyDown(JOSTICK_OPTION_CANCEL))
        {
            g_pGameMain->m_pInfWindow->OnButtonClick(1);
        }
    }
    // end 2009-01-16 by bhsohn 조이스틱 추가 승인/취소 추가

    m_nJoyStickRolling = nJoyStickRolling;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        롤링을 체크한다.
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::SetJoystickUnLockOn()
{
    SetUnLockOn();
    
    if( IsPkEnableMap(m_myShuttleInfo.MapChannelIndex.MapIndex) )
    {
        SetPKMode( FALSE );
    }    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        감도를 가져온다.
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
float CShuttleChild::GetJoystckMoveValue(float i_fValue)
{
    float fDstValue = i_fValue;
    CJoyStickInput *pJoyStick = g_pD3dApp->GetJoystickControl();
    
    if(!pJoyStick && !g_pD3dApp->IsUseJoyStick())
    {
        // 조이스틱 장치 없음
        return fDstValue;
    }    

    fDstValue = pJoyStick->GetSensity(g_pJoysticOp->nJoysticSense, i_fValue);

    return fDstValue;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        유닛 스톱
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::OnKeyDownUnitStop()
{    
    // 유닛정지 시스템    
    // 기어의 상태가 죽은 상황일때는 B키는 막는다.
    if(CheckUnitState() == BREAKDOWN)
    {
        return;
    }
    else if(m_bIsAir == FALSE && IS_DT(m_myShuttleInfo.UnitKind)            //A기어가 땅에 있을때
        || g_pD3dApp->m_dwGameState == _SHOP                        //상점에 있을때
        || m_bAttackMode == _SIEGE)                                    //시즈 모드일때 
    {
        return;
    }
    // 2005-10-06 by ispark
    // 튜토리얼일때는 정지 사용 안함
    // 편대 비행일때 편대장 이외는 정시 사용 금지
    // 기어가 LANDED일때 정지 사용 안함
    // 패턴 사용중에는 정지 사용 안함
    if((g_pTutorial->IsTutorialMode() == FALSE || 
        (g_pTutorial->GetLesson() == L2 && g_pTutorial->GetChapter() == L2_2 && g_pTutorial->GetChapterState() == TUTORIAL_STATE_MOTION)) &&
        m_pClientParty->IsFormationFlightMove() == FALSE &&
        m_bLandedMove == FALSE &&
        m_bSkillMoveIsUse == FALSE)
    {
        if(m_bUnitStop == FALSE)    
        {            
            // 유닛 정지
            g_pShuttleChild->UnitStop();
        }
        else if(m_bSkillGoMove)    // 출발 할 때
        {
            // 2008-07-03 by bhsohn EP3 버그 리포트 수정
            if(g_pGameMain->m_pCommunity->IsShowCommunityWindow())
            {
                // 출발시 강제로 창닫음
                g_pGameMain->m_pCommunity->ShowCommunityWindow(FALSE);
            }
            if(g_pGameMain->m_pSystem && g_pGameMain->m_pSystem->IsOptionShow())
            {
                g_pGameMain->PopupSystemOptinWnd();
            }
            // end 2008-07-03 by bhsohn EP3 버그 리포트 수정
            
            m_bUnitStop = FALSE;
            m_bPenaltyUnitStop = TRUE;
            m_bMouseMoveLock = FALSE;
            m_nBooster = BOOSTER_TYPE_MIN;
            g_pInterface->m_pGameMain->InitShowWindow();
            // 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달
            g_pShuttleChild->SendFieldSocketChangeBodyCondition( m_myShuttleInfo.ClientIndex, BODYCON_SET_OR_CLEAR_MASK );
            //end 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달
        }                        
    }    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        유닛 스톱
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::OnKeyDownJoinFormation()
{    
    // 2004-11-26 by jschoi - _GAME 상태가 아니거나 워프중에는 대형에 참여할 수 없다.
    if( g_pD3dApp->m_dwGameState != _GAME ||
        m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT ||
        m_nCurrentPatternNumber == PATTERN_UNIT_WARP_IN 
        || IS_CITY_MAP_INDEX(g_pD3dApp->GetMyShuttleMapChannelIndex().MapIndex)) // 2007-06-26 by bhsohn 마을에서 편대 비행 되는 버그 수정
    {
        DBGOUT("Current Game State (%d) , and PatternType is (%d), So Can't join FormationFlight.\n",g_pD3dApp->m_dwGameState,m_nCurrentPatternNumber);
        return;
    }
    
    if( m_pClientParty->GetPartyInfo().bFormationFlyingType && 
        !m_pClientParty->GetPartyInfo().bFormationFlyingPositionType && 
        m_pClientParty->GetPartyInfo().bPartyType == _PARTYMEMBER &&
        m_dwState == _NORMAL &&
        m_bAttackMode != _SIEGE &&
        (FALSE == IS_DT(m_myShuttleInfo.UnitKind) || m_bIsAir))
    {
        vector<PARTYENEMYINFO*>::iterator itParty = m_pClientParty->m_vecPartyEnemyInfo.begin();
        while(itParty != m_pClientParty->m_vecPartyEnemyInfo.end())
        {
            if(    (*itParty)->m_pEnemyData &&
                (*itParty)->m_bPartyType == _PARTYMASTER)
            {
                                
                if( D3DXVec3Length(&((*itParty)->m_pEnemyData->m_vPos - m_vPos)) < 300.0f &&
                    (*itParty)->m_pEnemyData->m_dwState == _NORMAL)
                {
                    m_pClientParty->ISendPartyGetFlightPosition();
                                    
                    // 2005-10-24 by ispark
                    // 편대비행 신청일 때 기어 정지 푼다.
                    m_bUnitStop = FALSE;
                    m_nBooster = BOOSTER_TYPE_MIN;
                    m_bPenaltyUnitStop = FALSE;
                    m_bMouseLock = FALSE;
                    m_bMouseMoveLock = FALSE;
                    g_pD3dApp->m_bFixUnitDirMode = FALSE;
                    g_pInterface->m_pGameMain->InitShowWindow();
                    // 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달    
                    SendFieldSocketChangeBodyCondition( m_myShuttleInfo.ClientIndex, BODYCON_SET_OR_CLEAR_MASK );
                    //end 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달    
                    m_ptDirOld.x = 0;
                    m_ptDirOld.y = 0;
                }
                else if((*itParty)->m_pEnemyData->m_dwState != _NORMAL)
                {
                    char buf[256];
                    wsprintf(buf,STRMSG_C_PARTY_0042);//"편대장이 편대비행을 할 수 없는 상태입니다."
                    g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
                }
                else
                {
                    char buf[256];
                    wsprintf(buf,STRMSG_C_PARTY_0043);//"편대장으로부터 거리가 너무 멀리 떨어져 있습니다."
                    g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
                }
                return;
            }
            //                    itEnemy++;
            itParty++;
        }
    }
    else if(!m_pClientParty->GetPartyInfo().bFormationFlyingType)
    {//
        char buf[256];
        wsprintf(buf,STRMSG_C_PARTY_0044);//"편대 비행 대형 선택이 잘못되었습니다."
        g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
    }
    else if(m_pClientParty->GetPartyInfo().bFormationFlyingPositionType)
    {
        char buf[256];
        wsprintf(buf,STRMSG_C_PARTY_0045);//"편대 비행 포지션이 잘못되었습니다."
        g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
    }
    else if(m_pClientParty->GetPartyInfo().bPartyType != _PARTYMEMBER)
    {
        char buf[256];
        wsprintf(buf,STRMSG_C_PARTY_0046);//"현재 편대원이 아닙니다. 확인해 주시기 바랍니다."
        g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
    }
    else
    {
        char buf[256];
        wsprintf(buf,STRMSG_C_PARTY_0047);//"편대 비행을 할 수 없는 상태입니다."
        g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        조이스틱으로 움직일수 있는 상태냐?
/// \author        // 2008-11-13 by bhsohn 조이스틱 작업
/// \date        2008-11-13 ~ 2008-11-13
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsPossibleJoystickMove()
{
    if((TRUE == g_pD3dApp->m_pKeyBoard->GetKeyBoardLostDevice())
        ||(FALSE == g_pD3dApp->m_pKeyBoard->GetKeyBoardFocus())) // 포커스 없을시, 강제 조이스틱 사용안함
    {
        return FALSE;
    }
    else if(g_pGameMain && g_pGameMain->IsShowOpJoystick())
    {
        // 조이스틱 옵션창이 떠있다.
        return FALSE;
    }
    else if(g_pD3dApp->IsPossibleWindowMove())
    {
        if(m_bIsAir == FALSE && IS_DT(m_myShuttleInfo.UnitKind))
        {    
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;    
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        무한으로 총알 나가는 현상 처리
/// \author        // 2008-12-04 by bhsohn 무한으로 총알 나가는 현상 처리
/// \date        2008-12-04 ~ 2008-12-04
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsWeapongShotting()
{
    if(TRUE == g_pD3dApp->IsMyShuttleCharacter())
    {
        return FALSE;
    }
    else if((TRUE == m_bRButtonState)
        ||(TRUE == m_bLButtonState))
    {
        return TRUE;
    }
    return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        카메라 시선 변경후, 쉽게 사냥 방법 막음
/// \author        // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음
/// \date        2009-02-19 ~ 2009-02-19
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CShuttleChild::TickCheckGearAttackStat()
{
    //float GetOriDistance() {return m_fOriDistance;}// 디폴트 카메라의 셔틀로부터 거리
    if(IsPossibleAttackStat())
    {
        return;        
    }
    if(m_bLButtonState)
    {
        // 기본무기 발사 취소
        m_bLButtonState = FALSE;
        m_PrimaryAttack.AttackData.TargetInfo.SetNullTarget();
        m_bTargetChange = TRUE;
        
    }
    if(m_bRButtonState)
    {            
        m_bRButtonState = FALSE;
        m_SecondaryAttack.AttackData.TargetInfo.SetNullTarget();
        
    }    

}

///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        현재 공격 가능한 상태냐?
/// \author        // 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음
/// \date        2009-02-19 ~ 2009-02-19
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CShuttleChild::IsPossibleAttackStat()
{
    if(NULL == g_pCamera)
    {
        return TRUE;
    }        
    // // 디폴트 카메라의 셔틀로부터 거리
    if(g_pCamera->GetCamDistance() <= (g_pCamera->GetOriDistance()+ATTACK_POSSIBLE_DISTANCE)) // 약간의 여백을 두자 여백의 묘미;;
    {
        return TRUE;        
    }
    return FALSE;
}

// 2010. 04. 15 by jskim 부스터 최소 속도, 최고 속도 2배 되는 버그 수정
// 2010. 03. 25 by jskim Speed, Angle 추가 계산
void CShuttleChild::RecalculateSpeedAndAngle()
{
//     // 엔진 아이템 가져오기
//     CItemInfo* pEngineItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR ); 
//     
//     if(!pEngineItem)
//     {
//         m_fShuttleSpeed = 0.0f;
//         m_fShuttleSpeedMax = 10.0f;
//         m_fShuttleSpeedBoosterOn = 10.0f;
//         m_fShuttleSpeedMin = 0.0f;
//         m_bySpeedPenalty = 0;                            // 2005-08-16 by ispark
//         m_fBURN = 1;
//         m_fCurrentBURN = 1;
//         m_fMouseRate = 0.5f;
//         return;
//     }
// 
//     ITEM* pITEM = pEngineItem->GetRealItemInfo();
// 
//     m_fBURN                        = pITEM->Time/1000.0f    + GetEnchantDesParam(pEngineItem, DES_ENGINE_BOOSTER_TIME_UP);
//     m_fShuttleSpeedMin            = pITEM->AbilityMin        + GetEnchantDesParam(pEngineItem, DES_ENGINE_MIN_SPEED_UP);
//     m_fShuttleSpeedMax            = pITEM->AbilityMax        + GetEnchantDesParam(pEngineItem, DES_ENGINE_MAX_SPEED_UP);
//     m_fShuttleGroundSpeedMax    = pITEM->Charging        + GetEnchantDesParam(pEngineItem, DES_ENGINE_GROUND_SPEED_UP);
//     m_fShuttleSpeedBoosterOn    = (float)pITEM->Range    + GetEnchantDesParam(pEngineItem, DES_ENGINE_BOOSTER_SPEED_UP);
// 
//     if(pEngineItem->GetEnchantParamFactor())
//     {
//         m_paramFactor.pfm_ENGINE_ANGLE_UP            = GetParamFactor_DesParam( *pEngineItem->GetEnchantParamFactor(), DES_ENGINE_ANGLE_UP);
//         m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP    = GetParamFactor_DesParam( *pEngineItem->GetEnchantParamFactor(), DES_ENGINE_BOOSTERANGLE_UP);
//     }
//     else
//     {
//         // 초기화            
//         m_paramFactor.pfm_ENGINE_ANGLE_UP = m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP = 0.0f;
//     }
//     
//     // 2010. 04. 14 by jskim 아이템 사용시 부스터 기능 추가
//     // 사용류 아이템 기능 가져오기
//     vector<ItemFontInfo*> ItemInfo = g_pGameMain->m_pInfSkill->GetItemFontInfo();
//     vector<ItemFontInfo*>::iterator it = ItemInfo.begin();
//     while(it != ItemInfo.end())
//     {
//         // 부스터 추가 시간 
//         if((*it)->pItemInfo)
//         {
//             m_fBURN += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_BOOSTER_TIME_UP);
//             // FLOAT형 재정렬 하여 사용
//             m_fBURN = FloatRangeDecimal(m_fBURN, 2);
//                 
//         // 최소 속도
//             m_fShuttleSpeedMin += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_MIN_SPEED_UP);
// 
//         // 최대 속도
//             m_fShuttleSpeedMax += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_MAX_SPEED_UP);
//         
//         // 지상 속도
//             m_fShuttleGroundSpeedMax += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_GROUND_SPEED_UP);
//     
//         // 부스터 속도
//             m_fShuttleSpeedBoosterOn += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_BOOSTER_SPEED_UP);
//         
//         // 하이퍼 무빙(선회각)
//             m_paramFactor.pfm_ENGINE_ANGLE_UP += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_ANGLE_UP);
//             m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_BOOSTERANGLE_UP);
//         }
//         it++;
//     }
//     //end 2010. 04. 14 by jskim 아이템 사용시 부스터 기능 추가
//     
//     for(int i = POS_PROW; i <= POS_PET; i++)
//     {
//         CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( i ); 
//     
//         if(!pAllItem || i == POS_REAR)
//             continue;
//         
//         // 부스터 추가 시간 
//         {
//             float fTime = GetEnchantDesParam(pAllItem, DES_ENGINE_BOOSTER_TIME_UP);
//             float fTempValue = pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_BOOSTER_TIME_UP);
//             m_fBURN += fTime;
//             m_fBURN += fTempValue;
//             // FLOAT형 재정렬 하여 사용
//             m_fBURN = FloatRangeDecimal(m_fBURN, 2);
//         }
//         
//         // 최소 속도
//         {
//             float fTime = GetEnchantDesParam(pAllItem, DES_ENGINE_MIN_SPEED_UP);
//             float fTempValue = pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_MIN_SPEED_UP);
//             m_fShuttleSpeedMin += fTime;
//             m_fShuttleSpeedMin += fTempValue;        
//         }
// 
//         // 최대 속도
//         {
//             float fTime = GetEnchantDesParam(pAllItem, DES_ENGINE_MAX_SPEED_UP);
//             float fTempValue = pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_MAX_SPEED_UP);
//             m_fShuttleSpeedMax += fTime;
//             m_fShuttleSpeedMax += fTempValue;
//         }
//         
//         // 지상 속도
//         {
//             float fTime = GetEnchantDesParam(pAllItem, DES_ENGINE_GROUND_SPEED_UP);
//             float fTempValue = pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_GROUND_SPEED_UP);
//             m_fShuttleGroundSpeedMax += fTime;
//             m_fShuttleGroundSpeedMax += fTempValue;
//         }
//         
//         // 부스터 속도
//         {
//             float fTime = GetEnchantDesParam(pAllItem, DES_ENGINE_BOOSTER_SPEED_UP);
//             float fTempValue = pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_BOOSTER_SPEED_UP);
//             m_fShuttleSpeedBoosterOn += fTime;
//             m_fShuttleSpeedBoosterOn += fTempValue;        
//         }    
//         
//         // 하이퍼 무빙(선회각)
//         {
//             if(pAllItem->GetEnchantParamFactor())
//             {
//                 float fTime = GetParamFactor_DesParam( *pAllItem->GetEnchantParamFactor(), DES_ENGINE_ANGLE_UP);
//                 float fTempValue = pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_ANGLE_UP);
//                 
//                 m_paramFactor.pfm_ENGINE_ANGLE_UP += fTime;
//                 m_paramFactor.pfm_ENGINE_ANGLE_UP += fTempValue;        
//     
//                 fTime = GetParamFactor_DesParam( *pAllItem->GetEnchantParamFactor(), DES_ENGINE_BOOSTERANGLE_UP);
//                 fTempValue = pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_BOOSTERANGLE_UP);
//                 
//                 m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP += fTime;
//                 m_paramFactor.pfm_ENGINE_BOOSTERANGLE_UP += fTempValue;
//             }            
//         }
//     }
//     m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pITEM, &m_paramFactor);
//     
//     // 엔진 UI갱신    
//     if(g_pGameMain && g_pGameMain->m_pUnitInfoBar)
//     {
//         g_pGameMain->m_pUnitInfoBar->SetBURN(m_fCurrentBURN, m_fBURN);
//         g_pGameMain->m_pUnitInfoBar->SetSpeed(m_fShuttleSpeed, m_fShuttleSpeedMin, m_fShuttleSpeedBoosterOn);
//     }
    
    // 장착된 아이템에서 부스터 시가 가져오기
    CItemInfo* pEngineItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR );
    
    if(!pEngineItem)
    {
        m_fBURN = 1;
        return;
    }
    m_fBURN    = pEngineItem->GetRealItemInfo()->Time/1000.0f + GetEnchantDesParam(pEngineItem, DES_ENGINE_BOOSTER_TIME_UP);
    m_fBURN = FloatRangeDecimal(m_fBURN, 2);

    for(int i = POS_PROW; i <= POS_PET; i++)
    {
        CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( i ); 
        
        if(!pAllItem || i == POS_REAR )
            continue;

        // 2010. 04. 21 by jskim 신규 럭키 머신 구현
        //// 부스터 추가 시간 
        //m_fBURN += GetEnchantDesParam(pAllItem, DES_ENGINE_BOOSTER_TIME_UP) + pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_BOOSTER_TIME_UP);
        float tempBURN = 0.0f;
        if(pAllItem && i == POS_CENTER)
        {
            CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( i );
            
            for(int j=0; j <SIZE_DES_PARAM_PER_RARE_ITEM_INFO; j++)
            {
                if(pAllItem->GetPrefixRareInfo())
                {
                    if(pAllItem->GetPrefixRareInfo()->DesParameter[j] == DES_ENGINE_BOOSTER_TIME_UP)
                    {
                        tempBURN += pAllItem->GetPrefixRareInfo()->ParameterValue[j];
                    }
                }
                if(pAllItem->GetSuffixRareInfo())
                {
                    if(pAllItem->GetSuffixRareInfo()->DesParameter[j] == DES_ENGINE_BOOSTER_TIME_UP)
                    {
                        tempBURN += pAllItem->GetSuffixRareInfo()->ParameterValue[j];
                    }
                }
            }
        }    
        // 부스터 추가 시간 
        m_fBURN += GetEnchantDesParam(pAllItem, DES_ENGINE_BOOSTER_TIME_UP) + pAllItem->GetRealItemInfo()->GetParameterValue(DES_ENGINE_BOOSTER_TIME_UP) + tempBURN;
        //end 2010. 04. 21 by jskim 신규 럭키 머신 구현
        m_fBURN = FloatRangeDecimal(m_fBURN, 2);
    }

    // 사용류 아이템 기능 가져오기
    vector<ItemFontInfo*> ItemInfo = g_pGameMain->m_pInfSkill->GetItemFontInfo();
    vector<ItemFontInfo*>::iterator it = ItemInfo.begin();
    while(it != ItemInfo.end())
    {
        // 부스터 추가 시간 
        if((*it)->pItemInfo)
         {
             m_fBURN += (*it)->pItemInfo->GetParameterValue(DES_ENGINE_BOOSTER_TIME_UP);
             // FLOAT형 재정렬 하여 사용
             m_fBURN = FloatRangeDecimal(m_fBURN, 2);
         }
        it++;
     }
}
//end 2010. 03. 25 by jskim Speed, Angle 추가 계산
//end 2010. 04. 15 by jskim 부스터 최소 속도, 최고 속도 2배 되는 버그 수정

// 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
void    CShuttleChild::SetDeadMessageEnable( BOOL bDeadMessageEnable )
{
    m_bDeadMessageEnable = bDeadMessageEnable;
}
// end 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가

// 2010-06-08 by dgwoo, 펫시스템 추가. 
void CShuttleChild::SetPartner(char* i_szPartner,BOOL i_bCharacter)
{
    int nPartner = atoi(i_szPartner);
    DeleteWearItem(WEAR_ITEM_KIND_PET);
    CreateWearItem( WEAR_ITEM_KIND_PET, nPartner, i_bCharacter);
}
// 2010-06-08 by dgwoo, 펫시스템 추가. 

// 2010-06-15 by shcho&hslee 펫 시스템 - 펫 소유 정보 받기.
BOOL CShuttleChild:: PutPetInfo(MSG_FC_ITEM_PET_BASEDATA* pMsg)
{

    if ( NULL == pMsg )
        return FALSE;

    tPET_CURRENTINFO *pNewPetCurInfo = new tPET_CURRENTINFO;

    if ( NULL == pNewPetCurInfo )
        return FALSE;



    //2011-10-06 by jhahn 파트너 성장형 시스템    
    pNewPetCurInfo->CreatedPetUID = pMsg->CreatedPetUID;
//    pNewPetCurInfo->EnableEditPetName = pMsg->EnableEditPetName;
//    pNewPetCurInfo->EnableLevelUp = pMsg->EnableLevelUp;
    //end 2011-10-06 by jhahn 파트너 성장형 시스템
    pNewPetCurInfo->PetExp = pMsg->PetExp;
    pNewPetCurInfo->PetIndex = pMsg->PetIndex;
    pNewPetCurInfo->PetLevel = pMsg->PetLevel;

//2011-10-06 by jhahn 파트너 성장형 시스템
    pNewPetCurInfo->PetExpRatio = pMsg->PetExpRatio;
    pNewPetCurInfo->PetEnableSocketCount = pMsg->PetEnableSocketCount;    

    pNewPetCurInfo->PetKitHP.ItemNum = pMsg->PetKitHP.ItemNum;
    pNewPetCurInfo->PetKitHP.TriggerValue = pMsg->PetKitHP.TriggerValue;

    pNewPetCurInfo->PetKitShield.ItemNum = pMsg->PetKitShield.ItemNum;
    pNewPetCurInfo->PetKitShield.TriggerValue = pMsg->PetKitShield.TriggerValue;

    pNewPetCurInfo->PetKitSP.ItemNum = pMsg->PetKitSP.ItemNum;
    pNewPetCurInfo->PetKitSP.TriggerValue = pMsg->PetKitSP.TriggerValue;

            

                        
    for (int i = 0 ; i < SIZE_MAX_PETSOCKET;i++)
    {
        pNewPetCurInfo->PetSocketItemUID[i] = pMsg->PetSocketItemUID[i];
        pNewPetCurInfo->PetAutoSkill.ItemNum[i] = pMsg->PetAutoSkill.ItemNum[i];            
    }

    strcpy ( pNewPetCurInfo->PetName , pMsg->PetName );    
//end 2011-10-06 by jhahn 파트너 성장형 시스템

    GetPetManager()->InsertPetCurrentData( pNewPetCurInfo );

    return TRUE;

}
// End 2010-06-15 by shcho&hslee 펫 시스템 - 펫 소유 정보 받기.

// 2011-05-31 by jhahn    인피3차시네마 변경
void CShuttleChild::Infinity3Cinema(short bSuccess)
{


    
    if ( TRUE == g_pInterface->m_pInfinity->GetEnableCinemaSkip() )
        return;
    
    // 2011-06-30 by jhahn  인피3차 시네마 스킵금지
    m_InfiState        = FALSE;
    //end 2011-06-30 by jhahn  인피3차 시네마 스킵금지
    

    
    CObjectChild * pCurrentObjEvent;

    if (bSuccess == 1)
    {
        // 2011-06-30 by jhahn  인피3차 시네마 임시위치
        g_pShuttleChild->m_vPos.x = INFINITY_THIRD_X_POS;
        g_pShuttleChild->m_vPos.z = INFINITY_THIRD_Z_POS;
        g_pShuttleChild->m_vPos.y = INFINITY_THIRD_Y_POS;
        //end 2011-06-30 by jhahn  인피3차 시네마 임시위치
        pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPositionIndex(EVENT_TYPE_INFI_CINEMA_THIRD, EVENT_TYPE_PARAM_INFITHIRD_CINEMA_SUCCESS);
    }    
    else if (bSuccess == 2)
    {
        // 2011-06-30 by jhahn  인피3차 시네마 임시위치
        g_pShuttleChild->m_vPos.x = INFINITY_THIRD_X_POS;
        g_pShuttleChild->m_vPos.z = INFINITY_THIRD_Z_POS;
        g_pShuttleChild->m_vPos.y = INFINITY_THIRD_Y_POS;
        //end 2011-06-30 by jhahn  인피3차 시네마 임시위치
        pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPositionIndex(EVENT_TYPE_INFI_CINEMA_THIRD, EVENT_TYPE_PARAM_INFITHIRD_CINEMA_FAIL);
    }    
    if(pCurrentObjEvent)
    {
        D3DXVECTOR3 vVel,vUp, vPos;
        vVel = m_vPos - pCurrentObjEvent->m_vPos;
        D3DXVec3Normalize(&vPos, &(pCurrentObjEvent->m_vPos - g_pShuttleChild->m_vPos));
        vUp.x = m_mMatrix._21;
        vUp.y = m_mMatrix._22;
        vUp.z = m_mMatrix._23;
        if(vVel == vUp)
        {// 매트릭스가 깨지지 않도록
            m_mMatrix._21 += 0.001f;
            m_mMatrix._22 -= 0.001f;
            m_mMatrix._23 += 0.002f;
        }
        m_mMatrix._31 = vVel.x;
        m_mMatrix._32 = vVel.y;
        m_mMatrix._33 = vVel.z;
        int succestype;
        if(bSuccess == 1)
        {
            succestype = PATTERN_CAMERA_SUCCESS_INFINITYTHIRD;
        }
        else if (bSuccess  == 2)
        {
            succestype = PATTERN_CAMERA_FAIL_INFINITYTHIRD;
        }
        
        if(InitCinemaUnit(succestype, FALSE, pCurrentObjEvent->m_vPos, vPos) == TRUE)
        {
        // 2011-06-30 by jhahn 인피3차    시점 1인칭 변경
            g_pD3dApp->m_pCamera->SetCamType( CAMERA_TYPE_FPS);
        //end  2011-06-30 by jhahn 인피3차    시점 1인칭 변경
            m_bSkillMoveIsUse = TRUE;
            g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
        if(g_pInterface->m_bShowInterface)
        {                        
                // 2011-06-30 by jhahn 인피3차 시네마 때 공격금지
                m_bLButtonState = m_bRButtonState = FALSE;
                //end 2011-06-30 by jhahn 인피3차 시네마 때 공격금지
            
                SetShowCinema ( TRUE );
            g_pInterface->SetScreenShotMode(TRUE);
                m_bKeyBlock = TRUE;
            }
        }
        else
        {
            DBGOUT("[succestype] : Pattern File Non Exsistent.\n");
            
            g_pInterface->m_pInfinity->SendPacket_SkipEndingCinema();
        }
    }    
}
// End 2011-05-31 by jhahn    인피3차시네마 변경

// 2012-09-20 by jhahn 인비지블에서 파트너 아이템 사용가능
void CShuttleChild::PetUseItme()
{

    CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 

    if( pAllItem == NULL )
    {
        return;
    }

    tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );
    if( tempCurrentInfo == NULL )
    {
        return;
    }

    tPET_LEVEL_DATA* tempLevelData = g_pDatabase->GetPtr_PetLevelData( tempCurrentInfo->PetIndex,tempCurrentInfo->PetLevel );
    if( tempLevelData == NULL )
    {
        return;
    }

    // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
    DWORD dwGameCurrentTime = g_pD3dApp->GetGameCurrentTime();
    BOOL bUsePacketSend = FALSE;    
    if(max(1, dwGameCurrentTime-m_dwPetUseItemTime_Spell) > MAX_PETUSETITEM_CAP)  // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
    {
        //2011-10-06 by jhahn 파트너 성장형 시스템
        for (int i = 0 ; i < SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT ; i++)
        {
            if(tempCurrentInfo->PetAutoSkill.ItemNum[i] && g_pShuttleChild->GetPetManager()->GetSocketOnOff())
            {
                CSkillInfo* tempskill =  g_pShuttleChild->m_pSkill->FindItemSkill(tempCurrentInfo->PetAutoSkill.ItemNum[i]);
                
                if (tempskill &&(g_pShuttleChild->m_myShuttleInfo.CurrentSP >= tempskill->ItemInfo->ReqSP ) &&  (tempskill->GetCheckReattackTime() <= 0.0f ))
                {
                    g_pShuttleChild->m_pSkill->UseSkill(tempskill );
                    // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
                    bUsePacketSend = TRUE;
                    // END 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정    
                }
            }        
        }
    }

    // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
    if(bUsePacketSend)
    {
        m_dwPetUseItemTime_Spell = g_pD3dApp->GetGameCurrentTime();
    }
    // END 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정

    // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
    CItemInfo* pItemInfo = NULL;
    if(max(1, dwGameCurrentTime-m_dwPetUseItemTime_Hp) > MAX_PETUSETITEM_CAP)  
    {
        CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByItemNum( tempCurrentInfo->PetKitHP.ItemNum );
        if( pItemInfo != NULL )
        {
            float temp = g_pShuttleChild->m_myShuttleInfo.HP;
            float temp2 = temp / 100;
            if ( g_pShuttleChild->m_myShuttleInfo.CurrentHP < (temp2 * tempCurrentInfo->PetKitHP.TriggerValue ))
            {
                HpCharge = TRUE;
            }
            
            if ( HpCharge )
            {
                if (g_pShuttleChild->m_myShuttleInfo.CurrentHP >= g_pShuttleChild->m_myShuttleInfo.HP)
                {
                    HpCharge = FALSE;
                }
                else
                {
                    if (tempLevelData->KitLevelHP >= pItemInfo->ItemInfo->ArrParameterValue[1] && g_pShuttleChild->GetPetManager()->GetSocketOnOff())
                    {
                        g_pGameMain->m_pInven->SendUseItem( (ITEM_BASE*)pItemInfo );

                        m_dwPetUseItemTime_Hp = dwGameCurrentTime; // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
                    }
                    else
                        pItemInfo = NULL;
                    
                    
                }
                
            }
        }
    }
    // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
    if(max(1, dwGameCurrentTime-m_dwPetUseItemTime_Dp) > MAX_PETUSETITEM_CAP)  
    {        
        pItemInfo = g_pStoreData->FindItemInInventoryByItemNum( tempCurrentInfo->PetKitShield.ItemNum );
        if( pItemInfo != NULL  )
        {
            float temp = g_pShuttleChild->m_myShuttleInfo.DP;
            float temp2 = temp / 100;
            if ( g_pShuttleChild->m_myShuttleInfo.CurrentDP < (temp2 * tempCurrentInfo->PetKitShield.TriggerValue ))
            {
                ShieldCharge = TRUE;
            }
            
            if ( ShieldCharge )
            {
                if (g_pShuttleChild->m_myShuttleInfo.CurrentDP >= g_pShuttleChild->m_myShuttleInfo.DP)
                {
                    ShieldCharge = FALSE;
                }
                else
                {
                    if (tempLevelData->KitLevelShield >= pItemInfo->ItemInfo->ArrParameterValue[1] && g_pShuttleChild->GetPetManager()->GetSocketOnOff())
                    {
                        g_pGameMain->m_pInven->SendUseItem( (ITEM_BASE*)pItemInfo );

                        m_dwPetUseItemTime_Dp = dwGameCurrentTime; // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
                    }
                    
                }
                
            }
        }
    }
    // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
    if(max(1, dwGameCurrentTime-m_dwPetUseItemTime_Sp) > MAX_PETUSETITEM_CAP)  
    {
        pItemInfo = g_pStoreData->FindItemInInventoryByItemNum( tempCurrentInfo->PetKitSP.ItemNum );
        if( pItemInfo != NULL  )
        {
            float temp = g_pShuttleChild->m_myShuttleInfo.SP;
            float temp2 = temp / 100;
//            if ( g_pShuttleChild->m_myShuttleInfo.CurrentSP < (temp2 * tempCurrentInfo->PetKitSP.TriggerValue ))
            // m_myShuttleInfo.CurrentSP : 랜더링되고 있는 SP값
            // g_pShuttleChild->m_fNextSP : 실제 서버 에서 알려주는 SP값 (중요. 이 값으로 현재 내 기체의 SP값을 체크해야한다.!!)
            if ( g_pShuttleChild->m_fNextSP < (temp2 * tempCurrentInfo->PetKitSP.TriggerValue ))
            {
//                DBGOUT(" SpCharge = TRUE; #1 \n");
                SpCharge = TRUE;
            }
            
            if ( SpCharge  )
            {
                if (g_pShuttleChild->m_fNextSP >= g_pShuttleChild->m_myShuttleInfo.SP)
                {                    
                    SpCharge = FALSE;
                }
                else
                {
                    if (tempLevelData->KitLevelSP  >= pItemInfo->ItemInfo->ArrParameterValue[1] && g_pShuttleChild->GetPetManager()->GetSocketOnOff())
                    {
                        g_pGameMain->m_pInven->SendUseItem( (ITEM_BASE*)pItemInfo );                        

                        m_dwPetUseItemTime_Sp = dwGameCurrentTime; // 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
                    }
                    
                }        
            }
        }        
    }    
    //end 2011-10-06 by jhahn 파트너 성장형 시스템

}
//end 2012-09-20 by jhahn 인비지블에서 파트너 아이템 사용가능

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
INT CShuttleChild::GetInSameMapPartyMemberCount()
{                     
    if(m_pClientParty)
        return m_pClientParty->GetInSameMapPartyMemberCount();
    return 0;
}
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
void CShuttleChild::ActiveKeyDownADouble()
{
    if((g_pTutorial->IsTutorialMode() == FALSE || 
        (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() >= L3)) &&
        m_bIsAir == TRUE &&
        m_dwState == _NORMAL)
    {
        if(m_fADoubleKeyTime < DOUBLEKEY_TIME 
            && m_bKeyDownACheck == FALSE 
            && m_nBooster != BOOSTER_TYPE_MAX // 부스터 사용시에는 롤링을 저장하지 않는다.
            && !m_bRollUsed) // 롤링중일때 선입력 받는것을 막는다
        {
            DbgOut("CShuttleChild::WndProc() : m_bKeyDownADouble = TRUE\n");
            m_bKeyDownADouble = TRUE;
            m_bKeyDownDDouble = FALSE;
        }
        else
        {
            m_fADoubleKeyTime = 0.0f;
        }
    }
}
void CShuttleChild::ActiveKeyDownDDouble()
{
    if((g_pTutorial->IsTutorialMode() == FALSE || 
        (g_pTutorial->IsTutorialMode() == TRUE && g_pTutorial->GetLesson() >= L3)) &&
        m_bIsAir == TRUE &&
        m_dwState == _NORMAL)
    {
        if(m_fDDoubleKeyTime < DOUBLEKEY_TIME 
            && m_bKeyDownDCheck == FALSE
            && m_nBooster != BOOSTER_TYPE_MAX // 부스터 사용시에는 롤링을 저장하지 않는다.
            && !m_bRollUsed) // 롤링중일때 선입력 받는것을 막는다
        {
            DbgOut("CShuttleChild::WndProc() : m_bKeyDownDDouble = TRUE\n");
            m_bKeyDownDDouble = TRUE;
            m_bKeyDownADouble = FALSE;
        }
        else
        {
            m_fDDoubleKeyTime = 0.0f;
        }
    }
}
// end 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정


///////////////////////////////////////////////////////////////////////////////
/// \fn            
/// \brief        A기어 배리어 이펙트 워프후 사라지는 오류 처리
/// \author        // 2012-12-17 by bhsohn A기어 배리어 이펙트 워프후 사라지는 오류 처리
/// \date        2012-12-17 ~ 2012-12-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
//void CShuttleChild::UpdateSkillEffectInfo()
// 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
void CShuttleChild::UpdateSkillEffectInfo(int nSkillNum)
// end 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
{
    vector<CSkillInfo*>::iterator itvecSkillInfo = m_pSkill->m_vecUsingSkill.begin();
    while(itvecSkillInfo != m_pSkill->m_vecUsingSkill.end())
    {
        CSkillInfo* pUsingSkillInfo = (*itvecSkillInfo);
        if(!pUsingSkillInfo)
        {
            itvecSkillInfo++;
            continue;
        }
        else if(!pUsingSkillInfo->ItemInfo)
        {
            itvecSkillInfo++;
            continue;
        }
        else if(SKILLTYPE_TIMELIMIT != pUsingSkillInfo->ItemInfo->SkillType)
        {
            // 타임 버프가 아니다
            itvecSkillInfo++;
            continue;
        }
        else if(SKILL_STATE_USING != pUsingSkillInfo->GetSkillState())
        {
            // 사용중이 아니다.
            itvecSkillInfo++;
            continue;
        }

        INT   ItemNum = pUsingSkillInfo->ItemNum;        // 아이템 번호
        //if(SKILL_BASE_NUM(ItemNum) != AGEAR_SKILL_BASENUM_BARIAL)        
        // 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
        if(SKILL_BASE_NUM(ItemNum) != nSkillNum)        
        // end 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
        {
            itvecSkillInfo++;
            continue;
        }
        if(m_pSkillEffect->FindSkillEffect(ItemNum))
        {
            DBGOUT("FindSkillEffect ItemNum[%d] \n", ItemNum);
        }
        else
        {
            DBGOUT("~FindSkillEffect ItemNum[%d] \n", ItemNum);
            ITEM* pItem = g_pDatabase->GetServerItemInfo(ItemNum);
            if(pItem)
            {
                // 2013-03-13 by bhsohn 스킬 번호 구조 수정
//                CAppEffectData* pEff = new CAppEffectData(g_pShuttleChild,SKILL_BASE_NUM(pItem->SourceIndex),D3DXVECTOR3(0,0,0));
                CAppEffectData* pEff = new CAppEffectData(g_pShuttleChild,SKILL_BASE_SOURCEINDEX(pItem->SourceIndex),D3DXVECTOR3(0,0,0));

                if(pEff->m_bUsing == TRUE )
                {
                    pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
                }
                else
                {
                    delete pEff;
                }
            }
        }
        

        itvecSkillInfo++;
    }    

}
// 2013-06-25 by ssjung 데미지 이펙트가 안나오는 현상 수정
BodyCond_t CShuttleChild::ShuttleChangeBodyCondition()
{
    BodyCond_t hyBody = BODYCON_SET_OR_CLEAR_MASK;                    

    if(m_pCharacterInfo)
    {
        if(COMPARE_BODYCON_BIT(m_pCharacterInfo->m_nCurrentBodyCondition , BODYCON_DAMAGE1_MASK ))
        {
            hyBody |= BODYCON_DAMAGE1_MASK;                        
        }                        
        if(COMPARE_BODYCON_BIT(m_pCharacterInfo->m_nCurrentBodyCondition , BODYCON_DAMAGE2_MASK ))
        {
            hyBody |= BODYCON_DAMAGE2_MASK;                        
        }
        if(COMPARE_BODYCON_BIT(m_pCharacterInfo->m_nCurrentBodyCondition , BODYCON_DAMAGE3_MASK ))
        {
            hyBody |= BODYCON_DAMAGE3_MASK ;                        
        }
    }
    return hyBody;
}
// end 2013-06-25 by ssjung 데미지 이펙트가 안나오는 현상 수정