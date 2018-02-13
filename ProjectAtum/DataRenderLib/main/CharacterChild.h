// CharacterChild.h: interface for the CCharacterChild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERCHILD_H__3926B3F0_BE13_4B06_8B22_FA8FA691CD25__INCLUDED_)
#define AFX_CHARACTERCHILD_H__3926B3F0_BE13_4B06_8B22_FA8FA691CD25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UnitData.h"

class CSkill;
class CCharacterRender;
class CObjectChild;
class CPkNormalTimer;
class CChatMoveData;

class CCharacterChild : public CAtumData
{
public:
	CCharacterChild();
	virtual ~CCharacterChild();
	virtual void Tick(float fElapsedTime);
	virtual void Render();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	void WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitCharacterData();
	void FineObjectTakeOff();
	BOOL GetbPickMove() { return m_bPickMove; }
	BOOL CheckEnterShopNPC(POINT *pt, float fLength=50.0f, BOOL nFlag = FALSE);
	CObjectChild *GetEventObject(float fDistance);
	CObjectChild *GetPickingObj(){return m_pSelectObject;}
	CUnitData * GetPickingBazaar(){return m_pSelBazaarShop;}
	D3DXVECTOR3 GetVecPickDir() { return m_vPickDir; }
	BOOL GetbBazaarEvent() {return m_bBazaarEventPos;}

	void LoadCharacterEffect(char* strFileName);

	void DeleteChatMoveShop();

	float GetCharacterSpeed(){ return m_fCharacterSpeed; }

	void SetPartner(char* i_szPartner);											// 2010-06-08 by dgwoo, 펫시스템 추가. 
	// 2010-11-29 by jskim, 펫 장착시 이동속도 증가
	void SetPetWearItem( BOOL flag ) { m_bPetWearItem = flag; }
	BOOL GetPetWearItem() { return m_bPetWearItem; }
	void SetPetAddSpeed( BOOL flag ) { m_bSetSpeed = flag; }
	BOOL IsPetAddSpeed() { return m_bSetSpeed; }
	void SetCharacterAddSpeed( float fSpeed ) { m_fCharacterAddSpeed = fSpeed; }
	float GetCharacterAddSpeed() { return m_fCharacterAddSpeed; }
	// end 2010-11-29 by jskim, 펫 장착시 이동속도 증가

	// 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선
	float	m_fAdminModeSpeed;
	// END 2013-06-10 by bhsohn 세력포인트 개선안 - 버프 '케릭터이동속도' 개선

protected:
	void CheckMoveRate(float fElapsedTime);
	void CheckShadowPos();
	void CheckAniTime(float fElapsedTime);
	void CheckState();
	void CheckEventTypeObject(float fElapsedTime);

	void InitEffect();
	void LoadCharacterEffect(CCharacterInfo** pCharacterInfo, char* strFileName);

	void Move_Character(float fElapsedTime);
	void CameraMoveTick();														// 2005-08-16 by ispark
	void SetMatrix_Move_Character(float fElapsedTime);
	void SetMatrix_Picking(D3DXVECTOR3 vPos, D3DXVECTOR3 vNorPos);
	D3DXVECTOR3 SetMoveVelByEventObject(BYTE bObjectType, float fDist);			// 2006-07-20 by ispark

	HRESULT RestoreShadow();
	void SendRequestEventObjectWarpIn(CObjectChild * pObj);						// 2006-07-19 by ispark

	void ChangeWearItemBodyConditionAllProcess(BodyCond_t i_nBodyCondition);
	
public:
	//////////////////////////////////////////////////////////////////////////
	// 캐릭터 
	int						m_nUnitNum;

//	D3DXVECTOR3				m_vPos;					// 렌더링 상의 좌표
//	D3DXVECTOR3				m_vNextPos;				
//	D3DXVECTOR3				m_vUp;
//	D3DXVECTOR3				m_vVel;
//  D3DXVECTOR3				m_vSideVel;				// 옆방향 벡터
// 
//	INT						m_nObjScreenX;			// 스크린상 X 좌표
//	INT						m_nObjScreenY;			// 스크린상 Y 좌표
//	INT						m_nObjScreenW;			// 스크린상 W 좌표
//
//	CCharacterInfo*			m_pCharacterInfo;		// 캐릭터 바디 컨디션
	CCharacterInfo*			m_pPickingInfo;			// Picking 이펙트

	FLOAT					m_fCurrentTime;
	D3DXMATRIX				m_mMatrix;
	LPDIRECT3DVERTEXBUFFER9 m_pVBShadow;			// 그림자 버퍼

	int						m_nStartEventType;		// 시작 이벤트 번호
	int						m_nSelBazaarType;		// 2006-07-29 by ispark, 선택한 개인상점 타입
	ClientIndex_t			m_ShopEnemyClientIndex;	// 2006-07-29 by ispark, 선택한 개인상점 클라이언트 인덱스

	CUnitData *				m_pSelBazaarShop;		// 선택 개인상점 유닛

	//////////////////////////////////////////////////////////////////////////
	// 개인 상점용
	CChatMoveData	*		m_pMoveChatShop;

protected:
	//////////////////////////////////////////////////////////////////////////
	// 캐릭터
	CCharacterRender*		m_pCharacterRender;		// Character의 Rendering 처리

	float					m_fCharacterAddSpeed;
	float					m_fCharacterSpeed;
	DWORD					m_dwState;

	D3DXVECTOR3				m_vShadowPos[4];		// 그림자
	BOOL					m_bCharacterRender;		// 캐릭터 렌더 할까?
	//////////////////////////////////////////////////////////////////////////
	// 마우스, 키
	BOOL					m_bRButtonState;
	BOOL					m_bMouseMove;			// 2005-08-16 by ispark
	BOOL					m_bMouseWheel;			// 2005-08-17 by ispark
	POINT					m_pOldMousePoint;

	//////////////////////////////////////////////////////////////////////////
	// Picking
	D3DXMATRIX				m_mPickMatrix;			// Picking Matrix
	D3DXVECTOR3				m_vPicking;				// 2005-07-15 by ispark Picking한 좌표
	D3DXVECTOR3				m_vPickDir;				// Picking한 방향
	BOOL					m_bPickMove;			// Picking Move 중인가
//	int						m_nSelectObjectIndex;	// 선택 오브젝트
	CObjectChild *			m_pSelectObject;		// 선택 오브젝트
	CObjectChild *			m_pShopObject;
	BOOL					m_bEnterShopState;
	float					m_fOldLength;			// 전 남은 거리
	
	//////////////////////////////////////////////////////////////////////////
	// 바디 컨디션
	BodyCond_t				m_hyBodyCondition;

	//////////////////////////////////////////////////////////////////////////
	// 기타
	BOOL					m_bStartPositionEvent;	// 2006-07-20 by ispark, 내 위치가 이벤트 포지션 위치이냐.(초기 설정에 쓰기 위한거)
	BOOL					m_bBazaarEventPos;		// 2006-07-28 by ispark, 개인 상점을 할 수 있는 위치인가

	float					m_fCharacterWarpErrTime; // 2006-08-07 by ispark, 캐릭터 워프시 갇혀있지 않기 위해서 일정 시간동안 정지후 반응이 없을시 되돌리는 변수
//	float					m_fCharacterMoveRate;	// 2006-11-02 by ispark, 테스트용
	// 2010-11-29 by jskim, 펫 장착시 이동속도 증가
private:
	BOOL					m_bPetWearItem;
	BOOL					m_bSetSpeed;
	// end 2010-11-29 by jskim, 펫 장착시 이동속도 증가

	//2012-06-25 by jhahn, 펫 장착시 이동속도 증가 기능추가
	int						PetoptionSpeed ;
	//end 2012-06-25 by jhahn, 펫 장착시 이동속도 증가 기능추가
};

#endif // !defined(AFX_CHARACTERCHILD_H__3926B3F0_BE13_4B06_8B22_FA8FA691CD25__INCLUDED_)
