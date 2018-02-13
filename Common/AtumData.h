// AtumData.h: interface for the CAtumData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMDATA_H__263209E9_D038_4DAB_81BC_C6634AC9921E__INCLUDED_)
#define AFX_ATUMDATA_H__263209E9_D038_4DAB_81BC_C6634AC9921E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"
#include "CharacterInfo.h"
#include "AppEffectData.h"

struct ATTACK_DATA
{
	MSG_FC_BATTLE_ATTACK_OK	AttackData;
	BOOL					bShootingWeapon;		// weapon을 쏘고 있는 상태이면 TRUE
	float					fCheckAutoMaticTimer;	// Automatic timer
	BOOL					bZigZagWeapon;			// 지그재그 형태에서 지그재그로 나가게 하기 위한 변수
	FLOAT					fCheckAttackTime;		// 재 발사하기위한 체크 시간(더미발사)
	BYTE					nAttackCount;			// 발사되어야할 총알수(ShotNum*MultiNum)
	BYTE					nMultiNumCount;			// MultiNum
	D3DXVECTOR3				vSidePos;				// 무기 좌우사이드 좌표
	D3DXVECTOR3				vSideBackPos;			// 무기 좌우사이드 백 좌표(무기 나가는 방향 잡기 위함, A기어는 WeaponVel과 실제 방향이 다른 경우가 생김)
	float					fExplosionRange;		// 2007-06-12 by dgwoo 폭발 반경.
	float					fWarheadSpeed;			// 2007-06-15 by dgwoo 고급무기 탄두 속도
};

// 2011. 03. 08 by jskim 인피3차 구현
struct MULTI_TARGET_DATA
{
	INT			PointIndex;							// 추가된 몬스터 타겟의 번호
	D3DXMATRIX	TargetMatrix;						// 실제 매트릭스
};									 
// end 2011. 03. 08 by jskim 인피3차 구현

class CAtumData : public CAtumNode  
{
public:
	CAtumData();
	virtual ~CAtumData();

	D3DXVECTOR3			m_vPos;					// 렌더링 상의 좌표
	D3DXVECTOR3			m_vNextPos;				// 몬스터 및 적캐릭의 실제 좌표
	D3DXVECTOR3			m_vUp;
	D3DXVECTOR3			m_vVel;
    D3DXVECTOR3			m_vSideVel;				// 옆방향 벡터
    D3DXVECTOR3			m_vLWPos;				// 왼쪽 윙의 위치
    D3DXVECTOR3			m_vLWBackPos;			// 왼쪽 윙의 뒷좌표(A기어 1형무기)
    D3DXVECTOR3			m_vRWPos;				// 오른쪽 윙의 위치
    D3DXVECTOR3			m_vLWSecondaryPos;		// 왼쪽 2형 윙의 위치
    D3DXVECTOR3			m_vRWSecondaryPos;		// 오른쪽 2형 윙의 위치

	BOOL				m_bShadowIsRender;		// 그림자를 렌더링 할것인가
	D3DXVECTOR3			m_vShadowPos1;			// 그림자 위치 (앞부분 왼쪽)
	D3DXVECTOR3			m_vShadowPos2;			// 그림자 위치 (앞부분 오른쪽)
	D3DXVECTOR3			m_vShadowPos3;			// 그림자 위치 (뒷부분 오른쪽)
	D3DXVECTOR3			m_vShadowPos4;			// 그림자 위치 (뒷부분 왼쪽)
	D3DXVECTOR3			m_vMoveVel;
	INT					m_nObjScreenX;			// 스크린상 X 좌표
	INT					m_nObjScreenY;			// 스크린상 Y 좌표
	INT					m_nObjScreenW;			// 스크린상 W 좌표
	FLOAT				m_fDistanceCamera;		// 카메라로부터의 거리
	FLOAT				m_fObjectSize;
	CAtumNode	*		m_pMoveChat;
	CAtumNode	*		m_pIDChat;
	DWORD				m_dwState;				// -1:initializing, 0:normal, 1:exploding, 2:close
	FLOAT				m_fCurrentTime;
	D3DXMATRIX			m_mMatrix;
//	D3DXMATRIX			m_mMatrixEffect;		
	BOOL				m_bCheckBlend;
	BOOL				m_bIsUnderWater;
	BOOL				m_bAniColl;				// 충돌 애니메이션 설정
	FLOAT				m_fAniCollTime;
	BOOL				m_bShielding;			// 쉴드 발동 중인가
	INT					m_bDegree;				// 디테일 정도
	BOOL				m_bItemDegree;			// 디테일 정도

	BYTE				m_bProgressiveRate;
	// 2003.4.23 by dhkwon, 캐릭터 이펙트 추가 내용
	CCharacterInfo*		m_pCharacterInfo;
	// 2010. 03. 18 by jskim 몬스터변신 카드
	//void LoadCharacterEffect(char* strFileName);
	void LoadCharacterEffect(char* strFileName, int nMonsterTransformer = 0, float nMonsterTransScale = 0.0f);
	//end 2010. 03. 18 by jskim 몬스터변신 카드
	
	// 2010-06-15 by shcho&hslee 펫시스템 - 1형, 2형 포지션 매트릭스 함수 분리
	//void SetWeaponPosition();
	void SetPrimaryWeaponPosition();
	void SetSecondaryWeaponPosition();
	// 2010-06-15 by shcho&hslee 펫시스템 - 1형, 2형 포지션 매트릭스 함수 분리

	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	void SetPetPosition();
	// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성

	// 2011-03-21 by jhAhn 마크시스템 마크 위치 생성
	void SetMarkPosition();
	//end 2011-03-21 by jhAhn 마크시스템 마크 위치 생성

	// 2011-07-20 by jhahn	인피3차 시네마플레이중 타캐릭터 안보이기
	void SetHideData();
	void SetShowData();
	//end 2011-07-20 by jhahn	인피3차 시네마플레이중 타캐릭터 안보이기

//	CAppEffectData *	m_pWeapon1_1;			// 1-1형 장착된 아이템(이펙트)
	CAppEffectData *	m_pWeapon1_2;			// 1-2형 장착된 아이템(이펙트)
//	CAppEffectData *	m_pWeapon2_1;			// 2-1형 장착된 아이템(이펙트)
	CAppEffectData *	m_pWeapon2_2;			// 2-2형 장착된 아이템(이펙트)
	CAppEffectData *	m_pEngine;				// 엔진에 장착된 아이템(이펙트)
	CAppEffectData *	m_pRadar;				// 레이다에 장착된 아이템(이펙트)
//	CAppEffectData *	m_pArmor;				// 방어류에 장착된 아이템(이펙트)
	CAppEffectData *	m_pContainer;			// 컨테이너(연료통등_)에 장착된 아이템(이펙트) // 2006-06-28 by ispark, 무제한 악세사리
	CAppEffectData *	m_pAccessories;				// 시간 제한 아이템(ITEMKIND_ACCESSORY_TIMELIMIT) // 2006-08-18 by ispark, 시간 제한 악세사리
	CAppEffectData *	m_pWingIn;				// 2006-08-18 by ispark, 특수

// 2006-01-12 by ispark, 무기 모듈
	CAppEffectData *	m_pWeapon1_1_1;			// 1-1형 장착된 아이템(이펙트)
	CAppEffectData *	m_pWeapon1_1_2;			// 1-2형 장착된 아이템(이펙트)
	CAppEffectData *	m_pWeapon2_1_1;			// 2-1형 장착된 아이템(이펙트)
	CAppEffectData *	m_pWeapon2_1_2;			// 2-2형 장착된 아이템(이펙트)


	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	CAppEffectData*		m_pRobotAGearWeapon_1;	// 로봇 A기어용 장착 이펙트
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

	CAppEffectData*		m_pPartner;				// 2010-06-08 by dgwoo, 펫시스템 추가. 
	CAppEffectData*		m_pPartner1;			// 2010-06-08 by dgwoo, 펫시스템 추가. 
	CAppEffectData*		m_pDummyPartner;		// 2010-06-08 by dgwoo, 펫시스템 추가.(더미) 
	CAppEffectData*		m_pDummyPartner1;		// 2010-06-08 by dgwoo, 펫시스템 추가.(더미1) 

	ATTACK_DATA			m_PrimaryAttack;
	ATTACK_DATA			m_SecondaryAttack;

	ATTACK_DATA			m_PetAttack;			// 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격

	// 2003..12.09 
	D3DXMATRIX			m_mWeaponMatrix;					// A-Gear 용 무기 매트릭스
	D3DXVECTOR3			m_vWeaponPos;						// A-Gear 용 무기 위치(x->m_vSideVel,y->m_vUp,z->m_vVel)
	D3DXVECTOR3			m_vWeaponVel;						// A-Gear 용 무기 앞방향
	D3DXVECTOR3			m_vWeaponUp;						// A-Gear 용 무기 위방향

	D3DXMATRIX			m_pMatWeaponPosition[4];// 2006-01-03 by ispark, 무기 포지션 메트릭스
	
	D3DXMATRIX			m_pMatPetPosition[2]; // 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	D3DXMATRIX			m_pMatMarkPosition; // 2011-03-21 by jhAhn 마크시스템 마크 위치 생성

	D3DXMATRIX			m_vPetLeftMatrix;					// 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태 left 펫 Matrix
	D3DXVECTOR3			m_vPetLeftPos;						// 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태 left 펫 위치
	D3DXVECTOR3			m_vPetLeftVel;						// 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태 left 펫 방향
	D3DXVECTOR3			m_vPetLeftUp;						// 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태 left 펫 업 백터

	D3DXMATRIX			m_vPetRightMatrix;					// 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태 Right 펫 Matrix
	D3DXVECTOR3			m_vPetRightPos;						// 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태 Right 펫 위치
	D3DXVECTOR3			m_vPetRightVel;						// 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태 Right 펫 방향
	D3DXVECTOR3			m_vPetRightUp;						// 2010-06-15 by shcho&hslee 펫시스템 - A-Gear 시즈상태 Right 펫 업 백터
	// 1형 무기의 연사력을 높여주기 위한 다음 데이타, 지울 변수들
//	MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY			m_PrimaryAttackData1;		// 발사와 관련된 데이타1
//	MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY	m_PrimaryAttackData2;		// 발사와 관련된 데이타2

	// 앞으로 지울 변수들
//	BOOL										m_bIsItemAttack;			// 아이템에 발사했는가
//	CAtumData *									m_pPrimaryAttackTarget;		// 타겟의 포인터
//	FLOAT										m_fPrimaryAttackTime;		// 1형을 재 발사하기위한 체크 시간(더미발사)
};

#endif // !defined(AFX_ATUMDATA_H__263209E9_D038_4DAB_81BC_C6634AC9921E__INCLUDED_)
