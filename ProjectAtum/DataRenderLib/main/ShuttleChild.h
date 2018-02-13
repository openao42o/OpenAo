#pragma once


#include "UnitData.h"
#include "TickTimeProcess.h"

#define WEAR_ITEM_NUMBER				9
#define A_GEAR_GUN_LIMIT_ANGLE_UP		25
#define A_GEAR_GUN_LIMIT_ANGLE_DOWN		110
#define OBSERVE_MAX_QUICK				10

// 2011-06-30 by jhahn  인피3차 시네마 임시위치
#define INFINITY_THIRD_X_POS		5000
#define INFINITY_THIRD_Z_POS		2500
#define INFINITY_THIRD_Y_POS		1200

struct WEAR_ITEM_PARAM_FACTOR
{
	DestParam_t	ArrDestParameter[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];
	float		ArrParameterValue[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];
};

struct USED_ITEM
{
	LONGLONG	UniqueNumber;
	float		fOverHeatTime;
	float		fOverHeatCheckTime;
	BOOL		bIsOverHeat;
};

enum ITEMTYPE
{
	ITEM_PRIMARY_WEAPON,
	ITEM_SECONDARY_WEAPON,
	ITEM_CONTROL_SKILL
};

// 연출시 사용
enum EVENT_TYPE
{
	EVENT_WARP_IN,
	EVENT_WARP_OUT,
	EVENT_CITY_OUTDOOR,
	EVENT_CITY_OUT_MOVE,
	EVENT_GAME_START,
	EVENT_GAME_END
};

class CClientParty;
class CSkill;
class CCinema;
class CStoreData;
//class CQuestData;
class CShuttleRender;
class CWeaponItemInfo;
class CUnitRender;
class CPkNormalTimer;
class CSkinnedMesh;
class CPetChild;		// 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리

// 2006-04-04 by ispark
struct INVEN_WEAPON_MESH
{
	CSkinnedMesh*	pInvenWeaponMesh;
	int				nMatIndex;
};
typedef vector<INVEN_WEAPON_MESH>		vectINVEN_WEAPON_MESH;

// 2007-03-20 by dgwoo 임시 옵저버 모드.
struct stOBSERVE
{
	ClientIndex_t	ClientIndex;
	// 2007-03-19 by dgwoo
	CEnemyData		*pEnemyData;			// 옵저버 모드시 따라가야할 Enemy의 주소.
	D3DXVECTOR3		vObserveUp;
	D3DXVECTOR3		vObserveSide;
	D3DXVECTOR3		vObserveLook;
	D3DXVECTOR3		vObservePos;
	D3DXVECTOR3		vObserveNextPos;
	MSG_FC_CHARACTER_OBSERVER_INFO		ClientInfo;
	stOBSERVE()
	{
		ClientIndex = NULL;
		pEnemyData  = NULL;
	}
};
typedef stOBSERVE stOPERATION;

class CShuttleChild : public CUnitData
{
public:
	CShuttleChild();
	virtual ~CShuttleChild();
	virtual void Tick(float fElapsedTime);
	virtual void Render();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	void InvalidateChat();
	void RestoreChat();
	void SetPos( const D3DXVECTOR3& vPos );
	void HandleInput(float fElapsedTime);
	FLOAT CheckMap(D3DXVECTOR3 vPos);
	BOOL CheckMapIsColl(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel);
	FLOAT CheckMapIsColl(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel,int cont);
	void CheckIndexList(MSG_FC_CHARACTER_GET_OTHER_INFO_OK* pMsg);
	void CheckIndexList(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK* pMsg);
	void CheckMove(MSG_FC_MOVE_OK* pMsg);
	void CheckShadowPos();
	BOOL CheckIsWater(D3DXVECTOR3 vPos);
	void ResortingItem();
	void UseSkillMove(float fElapsedTime);
	void SetMoveType(D3DXVECTOR3 vPos,float fHeight);
	void ChangeEngine(CItemInfo* pItemInfo, int nCurrentBURN);
	void UpdateEngine(CItemInfo* pItemInfo);
	// 2010. 03. 25 by jskim Speed, Angle 추가 계산
	void RecalculateSpeedAndAngle();
	//end 2010. 03. 25 by jskim Speed, Angle 추가 계산
	void InitEffect();// 2003.4.23 by dhkwon, 이펙트 초기화 함수
	void CheckGroundEffect();
	void InitShuttleData(BOOL bState = FALSE);	// 워프시등과 같이 데이타 초기화 (bState로 초기 착륙상태 설정)
	void SetShuttleData(BYTE type);				//(0~3->방어력셋팅)0:전체셋팅 1:아이템 2:마인드 컨트롤 3:기체  (4~->중량셋팅)4:총가능중량 5:아이템총중량
	void CheckEventType();						// 이벤트 타입 체크
	void CheckEventTypeObject();				// 이벤트 타입 체크(오브젝트)
	BOOL ISSetAutoPos();						// 파티로인한 자동 포지션이 조절될 것인가?
	void WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void CheckMapRegion();
	void SiegeTarget();
	void SetPVPClientIndex(ClientIndex_t PVPClientIndex) {m_nTargetIndex = PVPClientIndex;}
	ClientIndex_t GetPVPClientIndex() {return m_nTargetIndex;}

	void SetWeaponData(BYTE bType,BOOL bReset);//,BOOL bWear);// bType:1~2형 bAct:새로 장착 bReset: 정보 갱신 bWear:장착 or 비장착
	void SetShuttleInputType(BYTE bType){m_bMoveInputType = bType;}
	void SetShuttleFirstPosition(BOOL bMode = FALSE);
	void SetWeaponCountPrimary(INT nClientIndex, INT nCount);
	void SetWeaponCountSecondary(INT nClientIndex,INT nCount = -1);
	void Observe_PosSeting();
	void Move_Observe();

	void InitItemPoint();


	void SetShuttleFlight();												// Numpad0, Backspace 입력받음
	void SetShuttleFlightInit(float fElapsedTime,bool bType = true);							// 셔틀의 평형마추기
//	void Move_B_Gear(float fElapsedTime);	
//	void Move_M_Gear(float fElapsedTime);
//	void Move_A_Gear(float fElapsedTime);
//	void Move_I_Gear(float fElapsedTime);
	void Move_Air(float fElapsedTime);		//Move_B_Gear(float fElapsedTime)
	void Move_Ground(float fElapsedTime);	//Move_A_Gear(float fElapsedTime)
	void Move_AirSiege(float fElapsedTime);		//Move_A_GearSiege(float fElapsedTime)

	void CheckBoosterState(float fElapsedTime,bool bSet,int nOldBoosterType);
//	void CheckAirBreakState(float fElapsedTime,bool bSet);
//	void CheckMoveRate(float fElapsedTime,int nSideMove, int nFBMove);
	void CheckMoveRate(float fElapsedTime,int nSideMove);
	void CheckCollForMap();
	void CheckCollForObject(float fElapsedTime);
	BOOL CheckCollForObjectFrontByDistance(float fElapsedTime, D3DXVECTOR3 vNewPos, D3DXVECTOR3 vOldPos);
	void SendPacket(int nType,UINT64 n = 0);
	void CheckAniTime();
	void CheckBoosterAni();
	void CheckMouseDir();
	void CheckTarget();
	void OldCheckTarget();
	void Old2CheckTarget();
	void NewCheckTarget();
//	void CheckObjectRange(D3DXVECTOR3  vOldPos, float fElapsedTime);//, BOOL	bCollType);

	void CheckClientQuestList();
	void CheckMonsterMove(CMonsterData * pMon);
	BOOL MoveOrder(BYTE bType);
	BOOL SetOrderMoveTargetPos(BOOL bObjectLanding=TRUE);

	void SetCursorInit();

	// 2005-03-31 by jschoi
	void SetEffectPos(float fElapsedTime);
	void SetMatrix_Move_Air(float fElapsedTime);	// void SetEffectPos_B_Gear(float fElapsedTime)
	void SetMatrix_Move_Ground(float fElapsedTime);	// void SetEffectPos_A_Gear(float fElapsedTime)

	// remodeling, 2004-03-21 by dhkwon
	void SendFieldSocketChangeBodyCondition(ClientIndex_t nClientIndex, BodyCond_t hyBody);
	void SendFieldSocketRequestEventObjectWarpIn(CObjectChild * pObj);
	void SendMoveWeaponVel();
	void PrimaryBulletReloaded(MSG_FC_BATTLE_PRI_BULLET_RELOADED* pMsg);
	void SecondaryBulletReloaded(MSG_FC_BATTLE_SEC_BULLET_RELOADED* pMsg);

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//virtual void CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM=NULL);
	virtual void CheckAttack( ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM = NULL, ITEM* pEffectItem = NULL );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	virtual USHORT GetShiftWeaponBodyconditionByUnitKind();
	virtual void CreateSecondaryShieldDamage(D3DXVECTOR3 vCollPos );

	void ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition);

	void SetUnitState();

	// 중점좌표 구하기 2004-06-07 ydkim
	void SetTargetVectorforLandedMove(void);
	void SetUnitMovingAnimation(float fElapsedTime);
	BOOL ScanEnemyFrontBAckAndSpace(void);

//  스킬에 패턴 적용 2004.06.17 jschoi 
	BOOL InitCinemaSkill(int nSkillItemNumber);
	BOOL InitCinemaCamera(int nCameraType);
	void DeletePattern();

//	장착 아이템 해제 관련 함수 2004-07-27 jschoi
	void UsedItemTick(float fElapsedTime);		 
	void PutUsedItem(CWeaponItemInfo* pWeaponItemInfo);
	USED_ITEM* GetUsedItem(LONGLONG UniqueNumber);
	void DeleteUsedItem();

	BOOL InitCinemaUnit(int nCinemaType, BOOL bUpUse);			// 2005-08-12 by ispark
	// 2010. 05. 27 by jskim 시네마 적용 카메라 구현
	BOOL InitCinemaUnit(int nCinemaType, BOOL bUpUse, D3DXVECTOR3 vPosition, D3DXVECTOR3 vDirection);			// 2005-08-12 by ispark
	BOOL InitCinemaCamera(int nCameraType, D3DXVECTOR3 vPosition, D3DXVECTOR3 vDirection);
	void InfinityCinema(BOOL bSuccess);
	//end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
	void ChangeUnitStateDead();

	BOOL CheckCollMapObject();


	void TryLandingState();			// 착륙 시도
	void SetPKMode(BOOL bSet);

	// Tick function
	void UnitInfoBarSmoothRepair(float fElapsedTime);
	void TurnOnWall(float fElapsedTime);
	void TickCity(float fElapsedTime);
	void MoveLandingStation(float fElapsedTime);
	void TickBurn(float fElapsedTime);
	void TickWeapon(float fElapsedTime);
	BOOL TickChangeClientArea();

	DWORD GetLastAttackTick();
	const CHARACTER* GetShuttleInfo();
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	void SetWearRareItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo);
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	//void SetWearItemToCharacterParamFactor(int nPos, ITEM* pITEM);		// 장착/해제시에만 사용,ITEM의 ParamFactor를 CHARACTER에 적용한다.
	//void ChangeWearItemToCharacterParamFactor(int nPos, ITEM* pITEM);		// 장착/해제시에만 사용,ITEM의 ParamFactor를 CHARACTER에 적용한다.
	void SetWearItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo);		// 장착/해제시에만 사용,ITEM의 ParamFactor를 CHARACTER에 적용한다.
	void ChangeWearItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo);		// 장착/해제시에만 사용,ITEM의 ParamFactor를 CHARACTER에 적용한다.
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
	void ReleaseWearItemToCharacterParamFactor(int nPos);	// 장착/해제시에만 사용,ITEM의 ParamFactor를 CHARACTER에서 뺀다.
	void FieldSocketCharacterSendParamfactorInRange(int nPos);
	void SetParamFactorDesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue);
	void ReleaseParamFactorDesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue);
	void SetShuttleSpeed();
	void SetGroundTargetPos();
	void SetAirGroundTargetPos();											// 2006-12-01 by ispark, 공중 폭격
	D3DXVECTOR3 GetGroundTargetPos() { return m_vGroundTargetPos; }
	void PatternWarpIn();
	void PatternWarpOut(BOOL bOtherMap);	
	BOOL IsWarpGateZone() { return m_bWarpGateZone; }
	BOOL CheckPickingTarget(CUnitData* pTarget);

	// 롤링
	void CheckRollKeyDown(float fElapsedTime);									// 키값 처리	// 2005-07-05 by ispark
	float RollingSystem(float fElapsedTime);								// 롤링 시스템	// 2005-07-05 by ispark
	void CheckRoll(BYTE byDir);
	void InitRolling();

	// 2005-07-11 by ispark
	// 미사일 경고
	void SetMissileWarning(BOOL bMissileWarning) { 	m_bMissileWarning = bMissileWarning;}
	void SetSkillMissileWarning(BOOL bSkillMissileWarning) { m_bSkillMissileWarning = bSkillMissileWarning; }
	BOOL GetSkillMissileWarning()							{return m_bSkillMissileWarning;} 
//	BOOL GetMissileWarning() { return m_bMissileWarning; }
	BOOL IsMissileWarning() { return (m_bMissileWarning && !m_bSkillMissileWarning);}
	void SetMissileCount(int count) { m_nMissileCount = count; }
	int	 GetMissileCount() { return m_nMissileCount; }
	void InitMissileWarning();
	
	void RenderMirror(POINT *pMirrorPos=NULL);
	void UnitEffectRender(D3DXMATRIX pMatUnit);

	// 2005-10-12 by ispark
	// PK 타겟 (Enemy와 관련)
	BOOL IsEnemyPKTarget(int nTagetIndex);
	BOOL IsEnemyPKAttackTime(int nTagetIndex);
//	BOOL IsInfluenceWarType(int nEnemyIndex);

	void CheckBodyConditionToServer(BodyCond_t hyBodyCondition);

	BOOL GetIsUseInterface();
	void CheckCollForObject2(float fElapsedTime);
	BOOL WarpSkipInOutAirPort();
	void SetRenderInven(BOOL bFlag){m_bRenderInven = bFlag;}
	void InitCharacterToShuttleData();
	void MirrorTurnLeft(){m_fRotationX = m_fRotationX+0.05f;}
	void MirrorTurnRight(){m_fRotationX = m_fRotationX-0.05f;}
	void MirrorTurnUp(){m_fRotationZ = m_fRotationZ+0.05f;}
	void MirrorTurnDown(){m_fRotationZ = m_fRotationZ-0.05f;}
	void MirrorTurnOrig(){m_fRotationX = SHUTTLE_ROTATION_DEFAULT_X; m_fRotationZ = SHUTTLE_ROTATION_DEFAULT_Z;}
	BOOL GetRenderInvenFlag(){ return m_bRenderInven;}

	void CheckDamageBodyCondition();

	void SetAmorColor(int nAmorColor) { m_nArmorColorIndex = nAmorColor; }
	int	 GetAmorColor() { return m_nArmorColorIndex; }
	void SetInvenAmorColor(int nInvenAmorColor) { m_nInvenArmorColorIndex = nInvenAmorColor; }
	int	 GetInvenAmorColor() { return m_nInvenArmorColorIndex; }
	
	void SetInvenMesh(int nIndex, CSkinnedMesh* pMesh);
	void InitInvenMesh();

	void SendDeadMsg();
	BOOL IsRevivalUseItem();
	void SetSelectCharacterItem();
	void InitWarpShuttle();
	void InitWarpShuttle(MAP_CHANNEL_INDEX channelIndex);

	void CheckBazaarInfo(MSG_FC_BAZAAR_INFO_OK* pMsg);

	int	 CheckUnitState();									// 2006-09-19 by ispark, 유닛 상태 체크
	void CheckFormationMoveDelete();						// 2006-11-02 by ispark, 편대 비행 중이면 해제

	void SetUnLockOn();

	// 2007-03-23 by bhsohn 롤링하면서 공중 시즈 안되는 문제 해결
	BOOL IsMyShuttleRolling();

	// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
	BOOL IsObserverMode();
	BOOL ChangeObserveUser(int nKey);
	void TickObserve(float fElapsedTime);
	void ObserveUpdateGage(MSG_FC_CHARACTER_OBSERVER_INFO *pMsg);
	void ObserveEnd();
	void ObserveCancelUpdateInfo();

	void TickOperation(float fElapsedTime);

	// 2007-06-15 by dgwoo 오퍼레이션 모드인가?
	BOOL IsOperation()						{return m_bOperation;}
	void SetOperation(BOOL bOperation)		{m_bOperation = bOperation;}

	BOOL IsOperAndObser();
		

	// 2007-04-17 by bhsohn 가까운 거리에서의 시즈모드시 데이지 안들어가는 현상 처리
	BOOL IsAGearFollowEnemy();

	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	void	UseScanItem(BOOL i_bScan, float i_fCheckEnableTime, float i_fScanRange);
	void	CreateScanItemEffect(CUnitData *pUnit, 
										D3DXVECTOR3 vPos, D3DXVECTOR3 vVel, D3DXVECTOR3 vUp, 
										float i_fCheckEnableTime,
										UINT i_uEffectId);

	void	UseScanItem(BOOL i_bScan, float i_fCheckEnableTime, float i_fScanRange,INT EffectChange);

	ClientIndex_t GetTargetClientIndext();
	ClientIndex_t GetTargetToClientIdx(CAtumData* pTarget);

	void	UnitStop();				// 유닛을 정지 시킬경우.
	void	InitUnitStop();			// 유닛을 움지이게 할경우.

	// 2007-05-15 by bhsohn A기어 보완
	float GetRasingCheckTime();

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	void SetMyShuttleFullStatInfo(GEAR_STAT i_MyShuttleFullStatInfo);
	GEAR_STAT GetMyShuttleFullStatInfo();
	void ReformMyShuttleStatInfo(GEAR_STAT* pMyGearStat);

	// 2007-05-28 by bhsohn 아이템에 대한 체크섬 추가
	
	// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
	//BOOL GetCheckSum(UINT *o_puiCheckSum, int *o_pnFileSize, char* pFilePath);
	BOOL GetCheckSum(BYTE o_byObjCheckSum[32], int *o_pnFileSize, char* pFilePath);
	// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	const CHARACTER& GetMyShuttleInfo() const;
	CHARACTER& GetMyShuttleInfo();

	// 2007-07-04 by bhsohn 랜딩 모션중 포탈 탔을시 버그 수정
	// 유닛 정보 초기화 
	void InitUnitState();

	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	PARTYINFO GetPartyInfo();
	BOOL SendGoMissionMap();

	// 2007-07-25 by dgwoo 미사일이나 총알 발사시 타격을 줄수있는가?
	BOOL	IsWeaponeTarget(BOOL i_bSecondW = TRUE);
	void	InitWeaponeTarget();

	// 2007-08-10 by dgwoo 미러 찍기위한 배경(.
	void RenderInvenBack();

	// 2007-11-08 by bhsohn 인벤 이펙트 관련 처리
	CEffectInfo* GetEffectWeapon(int nInvenIdx);

	// 2007-12-05 by bhsohn 벽에 뚫고 들어가는 현상 처리
	BOOL CheckObjLandingColl(D3DXVECTOR3 vNormalVector );
	BOOL CancelLandingMode();

	// 2008-01-16 by bhsohn A기어 워프 실패시 위장,그라운드 엑셀 취소 시킴
	void SetShuttleAirMode(BOOL bIsAir);

	// 2008-03-17 by bhsohn 윈도우 이동시 Tick안들어오는 문제 처리
	BOOL IsUnitStop();
	BOOL IsAir();

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	void SetSecretInfoOption(INT i_nSecretInfoOption);
	void SetAmorHeight(INT i_nAmorNum);

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	D3DXMATRIX GetMatWeaponPosition(int nIdx);	
	int GetInvenWeaponMeshSize();
	INVEN_WEAPON_MESH* GetInvenWeaponMesh(int nIdx);
	float GetRotationX();
	float GetRotationZ();

	// 2008-09-26 by bhsohn 신규 인첸트 처리
	void UpdateEngineInfo();
	float GetEnchantDesParam(CItemInfo* pItemInfo, int nDestParam);
	void SetRadarRange(CItemInfo* i_pRadarItemInfo);
	CParamFactor* GetShuttleParamFactor();

	// 2008-11-13 by bhsohn 조이스틱 작업
	BOOL IsPossibleRolling();
	void StartRolling(int nKeyIndex);
	void CheckJoystickKeyDown();
	void SetJoystickUnLockOn();
	void OnKeyDownUnitStop();		// 유닛 스톱
	void OnKeyDownJoinFormation();	// 편대 비행 참여
	void SetAirCursorJostickPos();
	BOOL IsPossibleJoystickMove();	// 조이스틱 움직이는지 여부

	// 2008-12-04 by bhsohn 무한으로 총알 나가는 현상 처리
	BOOL	IsWeapongShotting();

	// 2009-03-31 by bhsohn 1인칭 시점 에어 시즈시, 버그 수정
	void SetBackView(BOOL bUpDown);
	// end 2009-03-31 by bhsohn 1인칭 시점 에어 시즈시, 버그 수정

	// 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
	void	SetDeadMessageEnable( BOOL bDeadMessageEnable );
	// end 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가

	// 2010-06-08 by dgwoo, 펫시스템 추가. 
	void	SetPartner(char* i_szPartner,BOOL i_bCharacter = FALSE);
	// 2010-06-08 by dgwoo, 펫시스템 추가. 

	BOOL	PutPetInfo(MSG_FC_ITEM_PET_BASEDATA* pMsg);	// 2010-06-15 by shcho&hslee 펫 시스템 - 펫 소유 정보 받기.

	// 2011-05-31 by jhahn	인피3차시네마 변경
	void Infinity3Cinema(short bSuccess);
	// end 2011-05-31 by jhahn	인피3차시네마 변경
// 2012-09-20 by jhahn 인비지블에서 파트너 아이템 사용가능
	void	PetUseItme();

	//2011-10-06 by jhahn 파트너 성장형 시스템
	BOOL HpCharge;
	BOOL ShieldCharge;
	BOOL SpCharge;							  
	//end 2011-10-06 by jhahn 파트너 성장형 시스템		  
//end 2012-09-20 by jhahn 인비지블에서 파트너 아이템 사용가능

public :	// Inline Process.

	// 시네마 연출 재생 중인지 활성화값 리턴.	2010. 08. 25. by hsLee.
	inline BOOL IsShowCinema ( void ) { return m_bShowCinema; }

	// 2011-06-30 by jhahn  인피3차 시네마 스킵금지
	inline BOOL ReturnInfistate(void) { return m_InfiState;} 
	//end 2011-06-30 by jhahn  인피3차 시네마 스킵금지

	// 시네마 연출 재생 활성화값 대입.	2010. 08. 25. by hsLee.
	inline void SetShowCinema ( BOOL bEnable ) { m_bShowCinema = bEnable; }
	// 2010-10-14 by jskim 롤링과 에어시즈 같이 사용했을시 타겟 안잡히는 버그 수정
	void SetKeyDownADouble(BOOL ADouble) { m_bKeyDownADouble = ADouble; }
	BOOL GetKeyDownADouble() { return m_bKeyDownADouble; }	
	void SetKeyDownDDouble(BOOL DDouble) { m_bKeyDownDDouble = DDouble; }
	BOOL GetKeyDownDDouble() { return m_bKeyDownDDouble; }

	// 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
	void ActiveKeyDownADouble();
	void ActiveKeyDownDDouble();
	// end 2012-12-04 by jhjang 부스터 사용 직전 롤링 선입력 버그 수정
	// end 2010-10-14 by jskim 롤링과 에어시즈 같이 사용했을시 타겟 안잡히는 버그 수정

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	INT GetInSameMapPartyMemberCount();
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	// 2007-11-22 by bhsohn 아레나 통합서버
	UID32_t	GetCharacterUniqueNumber();
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 2012-12-17 by bhsohn A기어 배리어 이펙트 워프후 사라지는 오류 처리
	//void UpdateSkillEffectInfo();
	// 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
	void UpdateSkillEffectInfo(int nSkillNum);
	// end 2013-02-14 by mspark, M기어 무적 스킬 워프후에 상대방에게 보이지 않는 문제 수정
	// END 2012-12-17 by bhsohn A기어 배리어 이펙트 워프후 사라지는 오류 처리

	// 2013-03-06 by bhsohn 복귀 유저 시스템
	virtual void SetRtnGameUser(BYTE byUsingReturnItem) {m_myShuttleInfo.bUsingReturnItem = byUsingReturnItem;}
	virtual BYTE GetRtnGameUser() {return m_myShuttleInfo.bUsingReturnItem;}
	// END 2013-03-06 by bhsohn 복귀 유저 시스템

	BodyCond_t ShuttleChangeBodyCondition();			// 2013-06-25 by ssjung 데미지 이펙트가 안나오는 현상 수정

protected : 

	void CheckWearItem(float fElapsedTime);
	void SetFormationMove(float fElapsedTime);
	
	void CheckOnObject();
	void SetWearItemParamFactor(int nPos, ITEM* pITEM);
	HRESULT SetResourceFile(char* szFileName);
	DataHeader * FindResource(char* szRcName);
	void RemoveEnemyTarget();
	// 2009-03-31 by bhsohn 1인칭 시점 에어 시즈시, 버그 수정
	//void SetBackView(BOOL bUpDown);
	// end 2009-03-31 by bhsohn 1인칭 시점 에어 시즈시, 버그 수정

	// 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
	void UpdateEnemyState(CEnemyData *pEnemyData, MSG_FC_MOVE_OK* pMsg);
	void SetEmenyState(CUnitData* pUnit, INT ItemNum);
	void ClearEmenyState(CUnitData* pUnit, INT ItemNum);

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	BOOL IsCheckObjectColl(CAtumData *pTarget);
	//BOOL GetResolutionPos(DWORD i_dwKind,float &fPosX,float &fPosY,float &fPosZ,float &fScaling);// 2008-08-22 by bhsohn EP3 인벤토리 처리


	// 2008-11-13 by bhsohn 조이스틱 작업
	void SetCursorJostickPos();
	int  SetCursorJostickLeftRight(int nLeftRightIndex, int nMaxX);
	int  SetCursorJostickUpDown(int nUpDownIndex, int nMaxY);
	float GetJoystckMoveValue(float i_fValue);

	// 2008-12-16 by bhsohn A기어 공중시지 기능 업그레이드
	void GetSiegeUpVelVector(D3DXVECTOR3	*o_vWeaponVel,	D3DXVECTOR3* o_vWeaponUp);
	void GetAirSiegeUpVelVector(D3DXVECTOR3	*o_vWeaponVel,	D3DXVECTOR3* o_vWeaponUp);

	// 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음
	void TickCheckGearAttackStat();
	BOOL IsPossibleAttackStat();
	// end 2009-02-19 by bhsohn 카메라 시선 변경후, 쉽게 사냥 방법 막음

	// 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
	void UpdateSafeUpVector(D3DXVECTOR3	vOldUpTmp);
	void CheckUpNVelVector(D3DXVECTOR3	vAirVel, D3DXVECTOR3	vOldUpVec);
	// END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리

public:
	CGameData	*		m_pGameData;
	CStoreData			*m_pStoreData;
	CUnitRender*		m_pRender;							// Shuttle의 Rendering 처리
	// 2007-03-20 by dgwoo 임시 옵저버 모드.
	stOBSERVE			m_stObserve;						// 옵저버할 유저의 정보.and 관전.
	BOOL				m_bObserve;							// 옵저버 모드인가?
	ClientIndex_t		m_ObserveQuick[OBSERVE_MAX_QUICK];	// 옵저버 단축키 1~0 번까지 이며 각 인덱스 번호를 가지고있는다.

	D3DXVECTOR3			m_vTargetPosition;
	D3DXVECTOR3			m_vGroundTargetPos;					// 지상 폭격시 타겟 포지션
    D3DXVECTOR3			m_vBasisUp;							// Basis Up Vector
	FLOAT				m_fSideRate;						// 좌우 회전 속도 조절 및 기울임
	FLOAT				m_fSideCheckRate;					// 좌우 회전 속도 및 기울임 조절변수(착륙시 수평을 마추기 위해)
	FLOAT				m_fHeightRate;						// 고도 조절 변수
	FLOAT				m_fLRVelRate;						// 좌우 가속도에 영향을 줄 변수
	FLOAT				m_fOldUpRate;						// 이전 업벡터를 기록하여 부드럽게 기울인다(좌우 이동 기울임과 회전 기울임 조절)
	
	FLOAT				m_fShuttleSpeed;					// 셔틀속도 변수 (엔진에 좌우된다)		
	FLOAT				m_fShuttleSpeedMax;					// 셔틀속도 최대 (엔진에 좌우된다)		
	FLOAT				m_fShuttleSpeedMin;					// 셔틀속도 최소 (엔진에 좌우된다)		
	FLOAT				m_fShuttleSpeedBoosterOn;			// 셔틀속도 부스터 사용 (엔진에 좌우된다)
	FLOAT				m_fShuttleGroundSpeed;				// 셔틀속도 땅 속력 (엔진에 좌우된다)
	FLOAT				m_fShuttleGroundSpeedMax;			// 셔틀속도 땅 속력 최대 (엔진에 좌우된다) // 2005-11-26 by ispark
	BYTE				m_bySpeedPenalty;					// 엔진선회가속력	// 2005-08-16 by ispark

	FLOAT				m_fRealShuttleSpeedMax;				// 실제 셔틀 최대 속력
	FLOAT				m_fRealShuttleSpeedMin;				// 실제 셔틀 최소 속력
	FLOAT				m_fRealShuttleSpeedBoosterOn;		// 실제 셔틀 부스터 속력

	FLOAT				m_fBoosterCheckTime;				// 부스터 사용 체크 타임
	FLOAT				m_fChangeBodyForBoosterCheckTime;	// 부스터 관련 전송 체크 타임
	INT					m_nShuttleBoosterState;				// 부스터를 키기 위한 더블클릭체크 상태
	INT					m_nShuttleStopState;				// 공중부양을 위한 더블클릭체크 상태

	INT					m_nFBDir;							// 앞뒤 방향
	INT					m_nLRDir;							// 좌우 방향
	INT					m_nSideRotateDir;					// 좌우 방향
	
	CHARACTER			m_myShuttleInfo;					// Shuttle 의 정보
	FLOAT				m_fDefenseTotal;					// Shuttle 의 Total 방어력
	FLOAT				m_fDefenseForItem;					// Shuttle 의 Item 방어력
	FLOAT				m_fDefenseForSkill;					// Shuttle 의 Skill 방어력
	FLOAT				m_fDefenseForShuttle;				// Shuttle 의 기체 방어력
	FLOAT				m_fTransportTotal;					// Shuttle 의 Total 중량
	FLOAT				m_fTransportCurrent;				// Shuttle 의 Current 중량

	BYTE				m_bCollObjectCheck;					// 충돌 체크해야 하는가?(오브젝트, 몬스터, 타 캐릭터)
	BYTE				m_bCollMonsterCheck;				// 충돌 체크해야 하는가?(오브젝트, 몬스터, 타 캐릭터)
	BYTE				m_bCollShuttleCheck;				// 충돌 체크해야 하는가?(오브젝트, 몬스터, 타 캐릭터)
	BYTE				m_bCollAttackCheck;					// Attack 시 충돌 체크해야 하는가?(오브젝트, 몬스터, 타 캐릭터)

	FLOAT				m_fExpPer;							// 경험치 퍼센티지 
	/////////////// 상대 정보 요청을 위한 플래그와 인덱스 변수 ////////
	BOOL				m_bGetOtherInfo;
	FLOAT				m_fGetOtherInfoCheckTime;
	INT					m_nGetOtherIndex;
	///////////////// 캐릭간 충돌 처리 위한 변수 //////////////////////
	BOOL				m_bColl;							// 몬스터 및 타 캐릭터와의 충돌
	D3DXVECTOR3			m_vCollPos;							// 충돌 포지션
	FLOAT				m_fCheckCollTime;
	INT					m_nBooster;							// Booster State
	INT					m_nHyperBooster;					// 2006-10-11 by ispark, 하이퍼 부스터
	BOOL				m_bOnObject;						// 셔틀 아래,위에 오브젝트가 있는지
	FLOAT				m_fAxisRate;						// 셔틀의 기울임 정도 
	INT					m_nUDState;							// 고저 조절 상태 
	///////////////// 자동 고도 셋팅  /////////////////////
	D3DXVECTOR3			m_vOldPos;

	///////////////// Party  /////////////////////
	CClientParty	*	m_pClientParty;						// 파티 관련

	
	BYTE				m_bCheckMove;						// 이동 타입 결정	
															// 0:못가는 지역, 1:2차감속, 2:1차감속, 3:가는지역, 4:일반비행지역
															// 0~3은 충돌시에 체크되고 4는 충돌이 발생하지 않음을 뜻한다
	BOOL				m_bIsWater;							// 현재 비행중인곳이 물인가

	BOOL				m_bCollMap;							// 맵과의 충돌 판단 변수
	FLOAT				m_fCollMapCheckTime;				// 맵과의 충돌시 튕겨져 올라가는 시간을 측정할 변수

	D3DXVECTOR3			m_vNormal;							// 현재 맵의 노말벡터

	// Primary, Second, Skill 의 한번에 발사되는 양(Amount), 한번에 발사되고 다음 턴까지의 딜레이(RATime), 딜레이가 적용되면서 발사되는 최대 횟수(Cont)

	////////////////////////////////////////////////////////////////////////////////////
	//							1형 2형 Weapon 관련 변수들							  //
	////////////////////////////////////////////////////////////////////////////////////
	BYTE				m_bAttackMode;						// 공격 모드-0:일반 1:폭격 2:시즈
	BOOL				m_bAttack;							// 공격이 가능한가. TRUE : 공격가능 FALSE : 공격불가능.

	//////////////////////////  Skill Test Key Input (매크로) //////////////////////////
	FLOAT				m_fSkillMoveTime;					// 이동 마인드 컨트롤시 타임 체크
	BOOL				m_bSkillMoveIsUse;					// 이동에 관련된 마인드 컨트롤 사용 유무
	BOOL				m_bIsCameraPattern;					// 카메라 패턴을 사용해 이동중인가? 2004.06.28 jschoi
	BOOL				m_bSkillMoveType;					// 마인드 컨트롤발동시 Move Type 테스트(도는모양 정의)
	D3DXVECTOR3			m_vSkillMoveVel;					// 회전 마인드 컨트롤 발동시 방향이 셋팅되어질 변수(이전 방향을 기억함)

	BOOL				m_bTurnCamera;
	CAtumData	*		m_pTarget;							// 현재 공격하고 있는 타겟 포인터
	CAtumData	*		m_pOrderTarget;						// 타겟 지시시 표시로 가르키게 되는 포인터
	UINT				m_nCurrentPatternNumber;			// 현재 사용중인 패턴 번호
	BYTE				m_bMoveGroundType;					// 지형에 셋팅된 이동 타입
	////////////////////////////////////////////////////////////////////////////////////
	BYTE				m_bEffectGround;					// 0:공중  1:땅위낮게 2:물위낮게

	///////////////// DT 형을 위한 공중에 떠 있는 상태인지 /////////////////////
	BOOL				m_bIsAir;
	FLOAT				m_fDTEventCheckTime;				// DT 형 이벤트 체크 타임
	////////////////////////////////////////////////////////////////////////////////////
	FLOAT				m_fNumpad0keyTime;
	FLOAT				m_fCancelSkillCheckTime;
	BYTE				m_bMoveInputType;					// 이동 키조작 방법	0:Auto 1:Normal
	FLOAT				m_fMouseLRMoveRate;					// 마우스 좌, 우 회전 비율
	FLOAT				m_fMouseUDMoveRate;					// 마우스 상, 하 회전 비율

	char				m_strChatPToP[MAX_CHAT_PTOP_CONT][SIZE_MAX_CHARACTER_NAME];
	BYTE				m_bCurPToPPos;
	BYTE				m_bOldPToPPos;

	BOOL				m_bRButtonState;					// 마우스 우측 버튼 상태
	BOOL				m_bLButtonState;					// 마우스 좌측 버튼 상태
	BOOL				m_bMButtonState;					// 마우스 휠버튼 상태
	BOOL				m_bUpButtonState;					// 키보드 UP 버튼 상태
	BOOL				m_bDownButtonState;					// 키보드 DOWN 버튼 상태
	BOOL				m_bLeftButtonState;					// 키보드 LEFT 버튼 상태
	BOOL				m_bRightButtonState;				// 키보드 RIGHT 버튼 상태
	BOOL				m_bSkillGoMove;						// 움직일수 있는 상태인가? T:움직임, F:못움직임.

	POINT				m_pOldMousePoint;

	BOOL				m_bUseBooster;						// 부스터 사용후 MaxSpeed로 돌아올때까지 부스터 사용금지
	BOOL				m_bReChargeBURN;
	FLOAT				m_fBURN;
	FLOAT				m_fCurrentBURN;

	POINT				m_ptOldPoint;

	BOOL				m_bFirstStart;						// 최초 시작시 착륙상태일때의 셋팅을 위한 변수(한번 Tick을 수행하지 않기 위함)

	BOOL				m_bAutoLockon;						// 자동 강제 락온 
	BOOL				m_bAutoHeightControl;				// 강제 락온 된 몬스터에 자동 고도 조절
//	BOOL				m_bEventReady;						// 이벤트기다리는중,  삭제해도 될듯.
	FLOAT				m_fWeaponMaxDistance;				// 1 ~ 2 형 무기중 사정거리를 체크 - 가장 긴 거리의 값
	INT					m_nMonsterCount;					// 초보 수련모드에서의 몬스터 수를 표시하기 위한 변수
//	FLOAT				m_fEventCheckTime;					// 이벤트 체크 타임

	FLOAT				m_fNextHP;							// 서서히 HP가 회복하는것을 보여주기 위한 변수(다음 HP)
	FLOAT				m_fNextSP;							// 서서히 SP가 회복하는것을 보여주기 위한 변수(다음 SP)
	FLOAT				m_fNextEP;							// 서서히 EP가 회복하는것을 보여주기 위한 변수(다음 EP)
	FLOAT				m_fNextDP;							// 서서히 EP가 회복하는것을 보여주기 위한 변수(다음 EP)
	FLOAT				m_fNextHPRate;						// 서서히 HP가 회복하는것을 보여주기 위한 변수(회복 속도)
	FLOAT				m_fNextSPRate;						// 서서히 SP가 회복하는것을 보여주기 위한 변수(회복 속도)
	FLOAT				m_fNextEPRate;						// 서서히 EP가 회복하는것을 보여주기 위한 변수(회복 속도)
	FLOAT				m_fNextDPRate;						// 서서히 EP가 회복하는것을 보여주기 위한 변수(회복 속도)

	BOOL				m_bIsSetFlighting;					// 비행 평형을 마추고 있는 중인가?
	FLOAT				m_fCollSendDamageCheckTime;			// 지형충돌시 데미지 전송을 위한 타임 조절

	D3DXVECTOR3			m_vMouseDir;						// 마우스 방향 벡터
	D3DXVECTOR3			m_vMousePos;						// 2010-06-15 by shcho&hslee 펫시스템 - 마우스 위치
	FLOAT				m_fMouseRate;						// 마우스 감도 조절변수(회전속도 영향)
	POINT				m_pt;								// 마우스 좌표
	POINT				m_ptDir;							// 마우스 방향 좌표
	POINT				m_ptDirOld;							// 마우스 방향 좌표 올드
	POINT				m_ptMove;							// 마우스 이동 좌표
	
	vector<USED_ITEM*>				m_vecUsedItem;			// 사용하다가 내린 아니템 (1형,2형,스킬)
	
	D3DXVECTOR3			m_vCollCenter;						// 충돌이 발생한 오브젝트의 중심좌표
	BYTE				m_bBeginnerQuestDegree;				// 초보 퀘스트 진행 단계 변수

	BOOL				m_bMouseLock;						// 마우스 락 모드 전환

	BOOL				m_bOrderMove;						// 카메라 컨트롤등과 같은 강제 이동 모드시 입력 적용 하지 않음
	BYTE				m_bOrderMoveType;					// 강제 이동 타입
	D3DXVECTOR3			m_vTargetPos;						// 타겟으로 이동하기 위한 좌표(자동이동에 쓰인다)
	D3DXVECTOR3			m_vTargetVel;						// 타겟으로 이동하여 바라볼 방향(자동이동에 쓰인다)
	FLOAT				m_fLandingCheckTime;				// 랜딩 강제 이동시 적용할 착륙 속도
	FLOAT				m_fLandingDist;						// 랜딩 강제 이동시 적용할 착륙장까지의 거리
	FLOAT				m_fRasingCheckTime;					// Take_Off 시 일정시간 상승을 위한 변수

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수값 변경.
	WEAR_ITEM_PARAM_FACTOR	m_wearItemParamFactor[MAX_EQUIP_POS];
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	WEAR_ITEM_PARAM_FACTOR	m_wearPreRareItemParamFactor[MAX_EQUIP_POS];
	WEAR_ITEM_PARAM_FACTOR	m_wearSufRareItemParamFactor[MAX_EQUIP_POS];
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

	/*
	WEAR_ITEM_PARAM_FACTOR	m_wearItemParamFactor[WEAR_ITEM_NUMBER];
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	WEAR_ITEM_PARAM_FACTOR	m_wearPreRareItemParamFactor[WEAR_ITEM_NUMBER];
	WEAR_ITEM_PARAM_FACTOR	m_wearSufRareItemParamFactor[WEAR_ITEM_NUMBER];
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
	*/
	// End 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수값 변경.

	CSkill	*			m_pSkill;

	// 2004-06-03, ydkim, 착륙장 타일단위 이동 관련  
    BOOL                m_bAniKeyBlock;						// 애니메이션중 키블록 
	BOOL                m_bAgearPortflag;					// A기어 관련 이벤트지역 상태확인 
	BOOL				m_bLandingOldCamtypeFPS;			// 랜딩전 캠타입이 1인칭이였나 

	DWORD				m_dwLastAttackTick;
	
	float               m_fAniFlag;							// 애니메이션 상태확인 		
	float				m_fKeyboardInputTimer;				// 키보드 입력 타이머
	float               m_fMoveFrameTimer;                  // 초당 움직이는 프레임수
	float               m_fFrameLerp;                       // 프레임 러프값 
	float				m_fAnimationMoveTimer;				// 애니메에션 중 프레임 타임 
	float				m_fSinMove;							// 착륙시 부드럽게

//	POINT				m_pAutoTargetOldMousePos;			// 시즈형모드에서 사용될 자동조준 모드시 마우스포인트 이동 포지션 


	D3DXVECTOR3         m_vMyOldPos;						// 방향키를 누르기전 나의 위치 
	D3DXVECTOR3			m_vAniFrame;						// 프레임 목표값 
	BOOL				m_bReflexionKeyBlock;				// 벽을 만났을때 키블럭 

	CCinema		*		m_pCinemaUnitPattern;				// 유닛(스킬,연출) 패턴
	CCinema		*		m_pCinemaCamera;					// 카메라 패턴
	
	EVENT_TYPE			m_nEventType;

	float				m_fWarpOutDistance;					// 워프 이벤트 오브젝트 사이 거리(워프 아웃시)
	D3DXVECTOR3			m_vWarpOutPosition;					// 워프 아웃시 최초 위치 

	CObjectChild *		m_pCurrentObjEvent;					// m_nEventType == EVENT_WARP_IN 시에 사용
	CObjectChild *		m_pCurrentObjEventOld;
	CWeaponItemInfo*	m_pPrimaryWeapon;
	CWeaponItemInfo*	m_pSecondaryWeapon;	

	MSG_FC_GUILD_GET_WAR_INFO_OK	*	m_pGuildWarInfo;		// 여단전시에 이 정보가 세팅된다.
	USHORT				m_nOldMapIndex;
	
	// 클라이언트 Tick Time 계산
	CTickTimeProcess    m_timeProcess;
	
//#ifdef _DEBUG
//	int					m_nDebugWarpMove;
//	int					m_nDebugSendPacketMove;
//	float				m_fDebugWarpCheckTime;
//	float				m_fDebugSendCheckTime;
//#endif
//	float				m_fMissileFireTime;

	// 2005-02-15 by jschoi  -  PK Delay Timer
	CPkNormalTimer*		m_pPkNormalTimer;	
	BOOL				m_bCtrlKey;		// CtrlKey 상태 변수 TRUE : Ctrl Down 상태, FALSE : Ctrl Up 상태

	BYTE				m_byCityWarTeamType;	// 도시점령전 상태 변수
	float				m_fTempGravity;			// 중력 값 
	float				m_fRollTime;			// 롤링 시스템 시작 시간			// 2005-07-11 by ispark
	BOOL				m_bRollUsed;			// 2005-07-14 by ispark 롤링 사용중인가
	
	// // 2005-07-21 by ydkim 
	int					m_nEventTypeAirPort;	// 착륙장 착륙 패턴타입
	int					m_nEventIndex;
	BOOL				m_bKeyBlock;
	BOOL				m_bWarpLink;			// 착륙장 워프 연결 패턴
	BOOL				m_bUnitStop;
	BOOL				m_bPenaltyUnitStop;		// 2005-08-09 by ispark 유닛정지 페널티 - 출발시 적용
	BOOL				m_bMouseMoveLock;		// 마우스 이동 막음 // 2005-08-09 by ispark

	CItemInfo*			m_pRadarItemInfo;		// 2005-08-16 by ispark 레이더 아이템 정보

	// 2005-10-10 by ispark - PK 타겟
	vector<PK_TARGET *>	m_vecPKTarget;
	int					m_bPKState;				// TRUE : 락온, FALSE : 언락온

	BOOL				m_bLandedMove;			// Landed후 키로 기어를 움직일 수 있는 상태
	BOOL				m_bSkipPattern;			// 패턴 스킵
	
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
//	BOOL				m_bInvenRender;
//	D3DXMATRIX			m_pMatInven;
//	D3DXMATRIX			m_pMatInvenWeaponSetPosition[4];
//	D3DXMATRIX			m_pMatInvenWeaponOrgPosition[4];// 2006-01-03 by ispark, 무기 포지션 메트릭스

	float				m_fAdvanceTime;			// 메모리 핵 체크 Advance

	// 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화
	DWORD				m_dwCheckTimeSendMoveVel;
	// end 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화

private:
	BOOL				m_bOperation;						// 관전 모드인가?

	BOOL				m_bWarpGateZone;
	BOOL				m_bTargetChange;
	BOOL				m_bRenderInven;

	float				m_fCurrentAngle;		// Y축 선회 각도   // 2005.6.13 by dhkwon - 선회 각도 가속력
	float				m_fCurrentSideRate;		// X축 선회 각도

	// 2005-07-05 by ispark
	// 롤링 시스템 키 처리
	BOOL				m_bKeyDownA;			// A 좌측 롤링
	BOOL				m_bKeyDownD;			// D 우측 롤링
	BOOL				m_bKeyDownACheck;		// A 좌측 롤링 사용허가 체크
	BOOL				m_bKeyDownDCheck;		// D 우측 롤링 사용허가 체크
	// 2005-09-26 by ispark
	// 롤링 변형(A, D클릭시 좌우 이동 부활, 더블클릭시 롤링)
	BOOL				m_bKeyDownADouble;		// A 더블클릭
	BOOL				m_bKeyDownDDouble;		// D 더블클릭
	float				m_fADoubleKeyTime;
	float				m_fDDoubleKeyTime;

	float				m_fRollMessageTime;		// 롤링 메세지 보내는 시간 체크
	float				m_fRollAngle;			// 롤링 회전 각도
	float				m_fRollLength;			// 롤링 좌우 이동 거리 체크
	float				m_fRollDownSpeed;		// 롤링 다운 스피드
	D3DXVECTOR3			m_vRollStart;			// 롤링 시작한 좌표
	D3DXVECTOR3			m_vRollOldPos;			// 롤링 좌표 이동시 전 좌표

	BOOL				m_bMissileWarning;		// 적 2형무기 공격중
	BOOL				m_bSkillMissileWarning;	// 스킬로 인한 미사일 경고 유무
	int					m_nMissileCount;		// 적 2형무기 갯수

	BOOL				m_bLandingField;		// 착륙장인가 (A기어가 착륙중에는 아무 처리 안하기 위해서)
	
	// 인벤토리
//	LPDIRECT3DVERTEXBUFFER9 m_pMirrorVB;
	LPDIRECT3DTEXTURE9      m_pTexturesBackInven;
	
	float				m_fRotationX;
	float				m_fRotationY;
	float				m_fRotationZ;	
	float				m_fRotationA;

	BOOL				m_bSkillAirMove;		// 스킬 에어 무브 패턴이다.

	float				m_fCheckWearItemTime;	// 체크 장착 아이템
	int					m_nCheckDamage;			// 체크 데미지

	float				m_fCheckDead;			// 죽었는지 체크

	int					m_nArmorColorIndex;		// 2006-02-17 by ispark, 칼라 인덱스
	int					m_nInvenArmorColorIndex;// 2006-02-17 by ispark,인벤 칼라 인덱스

	//INVEN_WEAPON_MESH	m_stInvenWeaponMesh[6];	// 2006-04-04 by ispark
	vectINVEN_WEAPON_MESH	m_vectInvenWeaponMesh;	// 2006-04-04 by ispark

	BYTE				m_byFormationFlightType;

	// 2007-04-17 by bhsohn 가까운 거리에서의 시즈모드시 데이지 안들어가는 현상 처리
	BOOL				m_bAGearFollowEnemy;		// A기어 포대 현재 적을 따라가고있냐?

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	GEAR_STAT			m_myShuttleFullStatInfo;	// 전체 스탯정보 

	float				m_fUnitX;
	float				m_fUnitY;
	float				m_fUnitZ;
	float				m_fScaling;

	BOOL				m_bNowMousePosWindowArea;
	BOOL				m_bOldMousePosWindowArea;

	FLOAT				m_fAmorHeight;				// 현재 아머의 높이.

	// 2008-11-13 by bhsohn 조이스틱 작업
	BOOL				m_bSetCursorPos;
	int					m_nJoyStickRolling;	

	// 2009-03-30 by bhsohn 차징샷 이펙트 버그 수정
	float				m_fDelCapChagingSkill;

	// 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가
	BOOL				m_bDeadMessageEnable;

	DWORD				m_dwDeadTime;
	// end 2010. 03. 25 by ckPark 사망시 5초이상 추락하면 부활메세지 추가

	// 2010. 05. 27 by jskim 시네마 적용 카메라 구현
	//BOOL				m_ShowInterface;
	BOOL				m_bShowCinema;
	//end 2010. 05. 27 by jskim 시네마 적용 카메라 구현


	//2011-06-30 by jhahn  인피3차 시네마 스킵금지
	BOOL					m_InfiState;
	//end 2011-06-30 by jhahn  인피3차 시네마 스킵금지

	// 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
	DWORD					m_dwPetUseItemTime_Spell;
	DWORD					m_dwPetUseItemTime_Hp;
	DWORD					m_dwPetUseItemTime_Dp;
	DWORD					m_dwPetUseItemTime_Sp;
	// END 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정

	// 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리
	float					m_fReverserIgnoreTime;
	// END 2013-06-27 by bhsohn M기어[리버스엔진] 충돌체크 안되는 부분 처리

	BOOL					m_bLendSoundPlay;        // 2013-08-07 A기어가 지상에서 드라이빙 할 때 나는 소리가 안꺼지는 현상 수정
};