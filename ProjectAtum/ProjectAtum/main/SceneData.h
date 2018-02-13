// SceneData.h: interface for the CSceneData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEDATA_H__5325A197_B598_4A01_9E34_946C65760A16__INCLUDED_)
#define AFX_SCENEDATA_H__5325A197_B598_4A01_9E34_946C65760A16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

///////////////////////////////////////////////////////////////////////////////
/// \class		CSceneData
///
/// \brief		렌더링할 배경화면 데이타, 맵데이타, 몬스터, object, enemy,날씨,시간 등
/// \author		dhkwon
/// \version	
/// \date		2004-03-19 ~ 2004-03-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CCinema;
class CFrustum;
class CBackground;
class CObjRender;
class CObjectChild;
class CMonRender;
class CSunRender;
class CSunData;
class CRainRender;
class CRainData;
class CETCRender;
class CWeapon;
class CItemData;
class CGameData;
class CUnitData;
class CAtumData;
class CItemData;
class CAppEffectData;
class CWater;
struct ALPHA_CHARACTERINFO;

class CSceneData  : CAtumNode
{
public:
	CSceneData();
	virtual ~CSceneData();

	void SetFogLevel(DWORD dwFogColor, float fFogStart, float fFogEnd ); 

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render();
	virtual void Tick();

	void	RenderCity();
// character, monster functions
	CUnitData * FindUnitDataByClientIndex( ClientIndex_t nIndex );
	int		FindClientIndexByUnitData( CUnitData* pUnit );
// Field Item functions
	void	SendItemGetItemAll();
	void	SendItemDeleteItemAdmin( int x, int y );
	void	DeleteFieldItemOfUnitData( CUnitData* pUnitData );
	void	DeleteFieldItemOfFieldIndex( UINT nFieldIndex );
	void	ExplodingFieldItemOfFieldIndex( UINT nFieldIndex );
	void	AddFieldItemItemSHowItem( MSG_FC_ITEM_SHOW_ITEM *pMsg );
	void	AddFieldItemBattleDropDummyOk( CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg );
	void	AddFieldItemBattleDropFixerOk(CAtumNode *pTarget,CAtumNode *pAttack,MSG_FC_BATTLE_DROP_FIXER_OK* pMsg);
	void	DeleteFieldItemBattleDropFixerOk(UINT nItemFieldIndex);		// 2004-09-13 jschoi
	CItemData * FindFieldItemByFieldIndex( UINT nFieldIndex );
	CItemData * FindFieldItemBy2DDistance( D3DXVECTOR2 vPos, float fDist );
	CItemData * FindFieldItemByPartTypeAndParent( DWORD dwPartType, CAtumNode* pParent );
	CItemData * FindFieldItemByParent( CAtumNode* pParent );
	CObjectChild * FindWarpGateByPosition( D3DXVECTOR3 vPos , float fDistance = RANGE_OF_VISION );
	int		GetDummyCountOfUnit(CAtumData* pUnit);
// monster weapon functions
	void	CreateWeaponMonsterMissile(MSG_FC_MISSILE_MOVE_OK* pMsg);
//	void	CreateWeaponMonsterSecondary(MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg);
// scene init, delete, render functions
	BOOL	InitBackground();
	VOID	InitRes();
	VOID	RestoreRes();
	VOID	InvalidateRes();
	VOID	DeleteRes();
	HRESULT	LoadTex();
	VOID	DeleteTexTileDevice();
	VOID	SetShuttleLandState(CAtumData * pNode);				// 셔틀의 게임스타트시 맵의 타입이 착륙가능지역이면 착륙상태로 시작하게 된다.
																// 서버에서 게임엔드시 저장해주어야한다. 착륙 상태로...
	VOID	SetupLights();
	VOID	CheckWeather();
	BOOL	ApplyFogDistanceAsHeight( float fOriginStart , float fOriginEnd );
	VOID	CheckDay();
	VOID	SetDay();
	D3DXVECTOR3	SetLightDirection();
	VOID	CheckObjectRenderList(BOOL bCheckRange = TRUE);
	VOID	SetObjectRenderList();
	VOID	DeleteObjectList(int nIndex);
	VOID	ChangeResource();
	VOID	InsertToBlockData(CAtumNode * pNode);
	VOID	ChangeToBlockData(CAtumNode * pNode);
	VOID	DeleteToBlockData(CAtumNode * pNode);

	// 2007-02-09 by dgwoo 스캔 관련 	
	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	//void	AddFieldItemScanObject(CAtumNode * pUnitData, float fCheckTime);
	void	AddFieldItemScanObject(CAtumNode * pUnitData, float fCheckTime, UINT uObjId, DWORD dwPartType, 
										D3DXVECTOR3	vVel, D3DXVECTOR3 vUp, D3DXVECTOR3 *i_pPos=NULL);
	void	TickScanObeject();

	//--------------------------------------------------------------------------//
	
// 순간 effect
	CAppEffectData* FindEffect(int nType);
	CAppEffectData* FindEffect(int nType, CUnitData *pParent);
	void DeleteEffect(CAtumNode* pParent);
// mapEnemyList, mapMonsterList
//	void SetExceptAllUnitTarget(); // 타겟리스트에서 제외시킴
	void SetPKSettingGuildWar( int nPeerGuildUniqueNumber, BOOL bPK );
// 시네마 데이타 펑션
	CCinema * LoadCinemaData(char* szFileName, int nFileName = -1);
// 주변 캐릭터 아이디 가져오기
	CEnemyData * GetEnemyCharaterID(D3DXVECTOR2 vPos1);
// 2007-09-05 by dgwoo 주변 몬스터의 정보 가져오기.
	CMonsterData * GetMonsterInfo2D(D3DXVECTOR2 vPos1);
	CEnemyData * GetPickEnemy(D3DXVECTOR2 vPos);
	void ChangeGammaOption( int nGamma );
	CObjectChild * FindEventObjectByIndex(D3DXVECTOR3 vPos, UINT nEventObjectIndex); // vPos 와 가장 가까운 nEventObjectIndex 오브젝트를 찾는다.
	CObjectChild * FindRandomObjectByEventType(BYTE bObjectType, BOOL bRand = FALSE);		// 2006-05-19 by ispark
	CObjectChild * FindEventObjectByTypeAndPosition(BYTE bObjectType, D3DXVECTOR3 vPos, float fDist=10.0f);
	CObjectChild * FindMapObjectByCodeAndPosition(int nCode, D3DXVECTOR3 vPos);
	CObjectChild * FindObjectByIndex(UINT nEventObjectIndex);					// 인덱스로 오브젝트를 찾는다
	CObjectChild * FindEventObjectByWarp();										// 워프 오브젝트를 찾는다
	// 2010. 05. 27 by jskim 시네마 적용 카메라 구현
	CObjectChild * FindEventObjectByTypeAndPositionIndex(BYTE bObjectType, short sEventIndex);
	//end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
	
// 2005-05-16 by jschoi - 도시점령전 팀 셋
	void	ChangeAllEnemyCityWarTeamType(BYTE byCityWarTeamType);// 모든 Enemy의 도시점령전 팀을 셋팅한다.

// 2005-07-28 by ispark
	void SetMaxAtitudeHeight(int i_nMaxHeight);
	void ChangeEnemyCharacterMode(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK* pMsg);		// 적 모드 바꾸기

// 2007-04-23 by dgwoo 아레나
	void ChangeObjectBodyCondition(INT nCode, BodyCond_t body);							// 바디컨디션 변경.

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	// 점 사이에 오브젝트가 있는지 체크
	BOOL IsObjectCheckPosTOPos(D3DXVECTOR3	vMyShuttlePos, D3DXVECTOR3	vEmenyPos, D3DXVECTOR3	vEnemyUp);
	// 점 사이에 지형이 있는지 체크
	BOOL IsTileCheckPosTOPos(D3DXVECTOR3	vMyShuttlePos, D3DXVECTOR3	vEmenyPos);

	// 2007-06-13 by bhsohn 메모리 버그 디버깅
	void DeleteRenderEnemy(ClientIndex_t nIndex );

	// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	void ReLoadEnemyRenderList();

	// 2008-07-14 by bhsohn EP3 관련 처리
	BOOL GetEmemyCharacterUniqueNumber(char* pName, UID32_t* o_CharacterUniqueNumber);
	void InvectoryFullMessage();

	ClientIndex_t GetEmemyCharacterClientIndex(char* pName);	// 2012-07-19 by isshin 아템미리보기


	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	CObjectChild*	FindMapObjectByIndex( int nIndex );
	CObjectChild*	FindMapObjectEvendByIndex( int nIndex );		// 2011-11-01 by jhahn EP4 트리거 시스템 
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	void	ChangeSkyBox( char* szSkyBoxName );
	void	ChangeEventObjectBodyCondition( short nEventParam, BodyCond_t body );
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

    // 2010. 10. 05 by jskim 맵로딩 구조 변경
	void	StepBackground();
	void	StepBackground_Step1();
	void	StepBackground_Step2();
	void	StepBackground_Step3();
	void	StepBackground_Step4();
	void	StepBackground_Step5();
    // end 2010. 10. 05 by jskim 맵로딩 구조 변경

	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	void	ShowObjectChild_ByType(BOOL bShow, BYTE bEventType, INT nBuildingKind); // 오브젝트 Event 타입으로 Show
	void	ShowObjectChild_ByIndex(BOOL bShow, INT nCodeNum);
	void	UpdateInflBuffNPCObject();
	// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

protected:
	void LoadCinemaFile();									// "cinema.tex"
	void DeleteCinemaFile();								// 전체 지우기


// 오브젝트와 충돌을 위한 영역 구하기 위한 함수
	enum MoveType
	{
		NOMOVE,
		TOPLEFT,
		TOPRIGHT,
		BOTTOMLEFT,
		BOTTOMRIGHT
	};

	int		CheckMove();
	void	CalcCollisionRange(int nMoveType);
	void	RenderWater();

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	float CheckCollRenderListRangeObject(D3DXMATRIX mat, D3DXVECTOR3 vPos, float fMovingDistance);

public:
	CFrustum		*			m_pFrustum;
	CAtumNode		*			m_pItemData;					// 아이템 데이타(필드내의)
	vector<CItemData*>			m_vecScanData;
	CWeapon			*			m_pWeaponData;				// 무기 데이타, 이거 다른 구조로 바꿔야 함
	CBackground		*			m_pGround;					// 맵
	CObjRender		*			m_pObjectRender;			// 맵상의 오브젝트(지물들)
	CMonRender		*			m_pMonsterRender;			// 몬스터 렌더러
	CETCRender		*			m_pETCRender;				// 기타 각종 렌더러
	CWater			*			m_pWater;					// 물렌더링
	CMapMonsterList				m_mapMonsterList;				// 몬스터 리스트
//	CMapMonsterList				m_mapMonsterRenderList;			// 몬스터 렌더링 리스트
	CVecMonsterList				m_vecMonsterRenderList;			// 몬스터 렌더링 리스트
	CVecMonsterList				m_vecMonsterShadowRenderList;	// 몬스터 그림자 렌더링 리스트	
	CMapEnemyList				m_mapEnemyList;					// 타캐릭터 리스트(인덱스가 0인 경우 60000번대로 임시로 넣는다)
//	CMapEnemyList				m_mapEnemyRenderList;			// 타캐릭터 렌더링 리스트
	CVecEnemyList				m_vecEnemyRenderList;			// 타캐릭터 렌더링 리스트
	CVecEnemyList				m_vecEnemyShadowRenderList;		// 타캐릭터 그림자 렌더링 리스트
	vector<CUnitData*>			m_vecUnitRenderList;
	///////// 안개 효과 변수들 /////////////
	DWORD						m_dwFogColor;					// 안개색
	FLOAT						m_fOrgFogStartValue;				// 안개 시작지점-거리(실제값)
	FLOAT						m_fOrgFogEndValue;					// 안개 끝지점-거리(질제값)
	FLOAT						m_fFogStartValue;				// 안개 시작지점-거리(현재값)
	FLOAT						m_fFogEndValue;					// 안개 끝지점-거리(현재값)
	FLOAT						m_fBeforeFogStartValue;			// 방금전의 안개 시작지점-거리
	FLOAT						m_fBeforeFogEndValue;			// 반금전의 안개 끝지점-거리
	BOOL						m_bFog;							// 렌더링을 On Off 시키는 변수, 사용안함
	BOOL						m_bFogStay;						// 안개가 완전히 걷히기 전까지 대기 시키는 변수, 사용안함
	D3DLIGHT9					m_light0;						// 전체를 비출 라이트
	D3DLIGHT9					m_light1;						// 헤드라이트
	D3DLIGHT9					m_light2;						// 이펙트 보조 라이트
	D3DLIGHT9					m_light3;						// 셔틀을 비출 라이트
	BOOL						m_bNight;

	CSunRender		*			m_pSunRender;					// 태양 렌더러
	CSunData		*			m_pSunData;						// 태양 데이타

	///////////// Tile Texture Buffer /////////////
	CGameData		*			m_pData;						// 데이타파일 (맵타일파일)
	LPDIRECT3DTEXTURE9			m_pCreateTexture[TEXTILE_NUM];

	BOOL						m_bIsRestore;					// Restore-Res 참조

	FLOAT						m_fSkyRedColor;
	FLOAT						m_fSkyGreenColor;
	FLOAT						m_fSkyBlueColor;
	CRainRender				*	m_pRainRender;				// 날씨 비 렌더러
	CAtumNode				*	m_pRainList;				// 날씨 비 리스트
	BYTE						m_byWeatherType;				// 날씨 타입(0:없음, 1:눈, 2:비)
	DWORD						m_dwStartTime;				// 시간을 계산하기 위한 클라이언트의 시작시간
	INT							m_nBaseTime;				// 서버로 전송받은 기준 시간
	BYTE						m_byMapType;					// 맵 타입(도시, 필드 등..)

	CVecEnemyList	*			m_vecEnemyBlockList;
	CVecMonsterList	*			m_vecMonsterList;
	vectorCObjectChildPtr		m_vectorRangeObjectPtrList;		//맵상의 시야주위의 영역 오브젝트 리스트
	vectorCObjectChildPtr		m_vectorCulledObjectPtrList;	//맵상의 렌더링할 오브젝트 리스트
	vectorCObjectChildPtr		m_vectorCollisionObjectPtrList; //맵상의 충돌검사를 할 오브젝트 리스트

	int							m_nMaxAtitudeHeight;			// 맵상 이동 가능한 최대 높이 // 2005-07-11 by ispark

	int							m_nBlockSizeX;
	int							m_nBlockSizeY;
	BOOL						m_bChangeWeather;			// 날씨 교체시에 실행

	// 2004-10-20 by jschoi
	float						m_fChangeWeatherCheckTime;		// 날씨 변화 진행 시간

	// 2005-01-20 by jschoi
	float						m_fAlphaSky;		// 스카이 박스 낮,밤 혼합비율

//	float						m_fFogDestStartValue;			// 디버깅 용
//	float						m_fFogDestEndValue;			// 디버깅 용

	// 2006-11-16 by ispark, 알파 렌더링
	vector<ALPHA_CHARACTERINFO>	m_vecAlphaEffectRender;
	vector<CUnitData*>			m_vecAlphaUnitRender;
 
    // 2010. 10. 05 by jskim 맵로딩 구조 변경
	PROJECTINFO					m_prProject; 
    // end 2010. 10. 05 by jskim 맵로딩 구조 변경

protected:
	CGameData				*	m_pCinemaData;
	BOOL						m_bWaterShaderRenderFlag;

	float						m_fGetItemAllDelay;
	float						m_fGetItemMessage;				// 채팅창에 인벤이 다 찼다는 경고 시간체크.

// 2008. 12. 11 by ckPark 물렌더링

	LPDIRECT3DTEXTURE9			m_pWaterBumpTexture;			// 물 범프 텍스쳐(게임 시작시 한번만 생성한다)
// end 2008. 12. 11 by ckPark 물렌더링
};

#endif // !defined(AFX_SCENEDATA_H__5325A197_B598_4A01_9E34_946C65760A16__INCLUDED_)
