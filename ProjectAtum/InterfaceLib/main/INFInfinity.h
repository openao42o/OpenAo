// INFInfinity.h: interface for the INFInfinity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINFINITY_H__764D1815_2DF5_49F4_B411_53F0EB0C0714__INCLUDED_)
#define AFX_INFINFINITY_H__764D1815_2DF5_49F4_B411_53F0EB0C0714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define FADEINTIME	1000
#define NORMALTIME	3000
#define FADEOUTTIME	1000
#define WHOLETIME	( FADEINTIME + NORMALTIME + FADEOUTTIME )

// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
#define TIP_FADEINTIME	1000
#define TIP_NORMALTIME	5000
#define TIP_FADEOUTTIME	1000
#define TIP_WHOLETIME	( TIP_FADEINTIME + TIP_NORMALTIME + TIP_FADEOUTTIME )

// 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축
#define TIP_NORMALTIMEINFI	2000


// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
struct tALARM_DEFENSESTEPINFO
{
	tALARM_DEFENSESTEPINFO()
	{
		memset ( this , 0x00 , sizeof( tALARM_DEFENSESTEPINFO ) );
	}

	void Start ( const INT a_iStep , const D3DXVECTOR2 &a_v2StartPos , const D3DXVECTOR2 &a_v2EndPos )
	{
		
		v2Pos = v2Start= a_v2StartPos;
		v2End = a_v2EndPos;

		bFrame = TRUE;

		fFrame = .0f;
		fAlpha = .0f;

		iAction = 0;

		dwDelayTime = 2000;
		dwElapsedTime = 0;

		iStep = a_iStep;
		
	}

	D3DXVECTOR2	v2Pos , 
				v2Start , 
				v2End;

	BOOL	bFrame;

	float	fFrame;
	float	fAlpha;

	INT		iAction;

	DWORD	dwDelayTime;
	DWORD	dwElapsedTime;

	INT		iStep;

};

typedef std::vector < tALARM_DEFENSESTEPINFO >	tdf_vecAlarmDefenseStep;
// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)


struct KeyMonsterInfo
{
	MEX_MONSTER_INFO*	m_pMonsterInfo;		// 몬스터 정보

	ClientIndex_t		m_nMonsterUID;		// 몬스터 UID

	INT					m_nCurHP;			// 현재 체력

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	INT					m_nMaxHP;			// 최대 체력
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	BOOL				m_bWarningTime;		// 현재 깜빡여야 하는가?

	BOOL				m_bWarning;			// true면 빨간색 깜빡, false면 아무것도 안그림

	DWORD				m_dwWarningStartTime;// 깜빡임이 시작한 시간

	DWORD				m_dwChangeWaringTime;// 빨색색과 보통이 바뀐 시간

	KeyMonsterInfo( MEX_MONSTER_INFO* pMonsterInfo,
					ClientIndex_t nMonsterUID,
					INT nCurHP , INT nMaxHP ) : m_pMonsterInfo( pMonsterInfo )
									,m_nMonsterUID( nMonsterUID )
									,m_nCurHP( nCurHP )
									,m_bWarningTime( FALSE )
									,m_bWarning( FALSE )
									,m_dwWarningStartTime( 0 )
									,m_dwChangeWaringTime( 0 )

								// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
									, m_nMaxHP( nMaxHP )
								// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	{

	}
};
// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
#include "ItemInfo.h"

struct TenderItem
{
	MSG_FC_INFINITY_TENDER_DROPITEM_INFO m_TenderItem;	// 입찰 아이템

	//UINT			m_nTenderEndTime;					// 시간

	ATUM_DATE_TIME	m_sTenderStartTime;					// 입찰 시작 시간.

	UINT			m_nTenderLeftTime;

	ITEM*			m_pItem;

	CINFImageBtn*	m_pTenderYes;						// 입찰

 	CINFImageBtn*	m_pTenderNo;						// 입찰포기

	CItemInfo*		m_pItemInfo;

	CINFImageEx*	m_pTenderItemBigIcon;				// 입찰아이템 빅아이콘

	POINT			m_pt;

	BOOL				m_bRender;

	BOOL				m_bTendered;

	TenderItem( MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pTenderItem,
				//UINT nTenderEndTime,
				ATUM_DATE_TIME sTenderStartTime ,
				ITEM* pItem,
				CINFImageEx* pTenderItemBigIcon) : //m_nTenderEndTime( nTenderEndTime ),
												 m_nTenderLeftTime( 0 ),
												 m_pItem( pItem ),
												 m_pTenderYes( NULL ),
												 m_pTenderNo( NULL ),
												 m_pItemInfo( NULL ),
												 m_pTenderItemBigIcon( pTenderItemBigIcon ),
												 m_bRender( FALSE ),
												 m_bTendered( FALSE )
	{
		memcpy( &m_TenderItem, pTenderItem, sizeof( MSG_FC_INFINITY_TENDER_DROPITEM_INFO ) );

		ITEM_GENERAL stItem;
		memset( &stItem, 0x00, sizeof(ITEM_GENERAL) );
		stItem.ItemInfo			= pItem;
		stItem.ItemNum			= pItem->ItemNum;
		stItem.Kind				= pItem->Kind;
		stItem.PrefixCodeNum	= m_TenderItem.PrefixCodeNum;
		stItem.SuffixCodeNum	= m_TenderItem.SuffixCodeNum;

		m_sTenderStartTime = sTenderStartTime;

		m_pItemInfo	= new CItemInfo( &stItem );

		m_pt.x = m_pt.y = 0;
	}

	~TenderItem()
	{
		DeleteDeviceObject();
	}

	HRESULT	InitDeviceObject( void )
	{
		if( m_pTenderYes == NULL )
		{
			m_pTenderYes = new CINFImageBtn;
			m_pTenderYes->InitDeviceObjects( "if_tdyes3", "if_tdyes1", "if_tdyes0", "if_tdyes2" );
		}

		if( m_pTenderNo == NULL )
		{
			m_pTenderNo = new CINFImageBtn;
			m_pTenderNo->InitDeviceObjects( "if_tdno3", "if_tdno1", "if_tdno0", "if_tdno2" );
		}

		return S_OK;
	}

	HRESULT	RestoreDeviceObject( void )
	{
		if( m_pTenderYes )
			m_pTenderYes->RestoreDeviceObjects();

		if( m_pTenderNo )
			m_pTenderNo->RestoreDeviceObjects();

		return S_OK;
	}

	HRESULT	DeleteDeviceObject( void )
	{
		if( m_pTenderYes )
		{
			m_pTenderYes->DeleteDeviceObjects();
			util::del( m_pTenderYes );
		}

		if( m_pTenderNo )
		{
			m_pTenderNo->DeleteDeviceObjects();
			util::del( m_pTenderNo );
		}

		return S_OK;
	}

	HRESULT	InvalidateDeviceObject( void )
	{
		if( m_pTenderYes )
			m_pTenderYes->InvalidateDeviceObjects();

		if( m_pTenderNo )
			m_pTenderNo->InvalidateDeviceObjects();

		return S_OK;
	}
};
// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

class CINFInfinity  : public CINFBase
{
private:
	CINFImageEx*		m_pTimeNumImage[ 10 ];		// 숫자 이미지
	
	CINFImageEx*		m_pColonImage;				// 콜론

	CINFImageEx*		m_pClearImage;				// Clear

	CINFImageEx*		m_pFailImage;				// Fail

	CINFImageEx*		m_pResultImage;

	ATUM_DATE_TIME	m_InfinityStartTime;		// 인피니티 시작시간

	MSec_t			m_nInfinityLimitTime;		// 제한시간

	MSec_t			m_nInfinityPenaltyTime;		// 변경된 제한시간

	CINFImageBtn*	m_pGiveUpBtn;				// 포기버튼

	CINFImageBtn*	m_pReturnCityBtn;			// 성공시 도시로귀환 버튼

	UINT			m_nFadeStartTime;			// 시간값들

	DWORD			m_dwAlphaColor;

	BOOL			m_bRenderTimeLimit;

	UINT			m_nHour, m_nMin, m_nSec;

	BOOL			m_bRenderResult;

	UINT			m_nInfinityReturnTime;		// 인피니티 복귀 타이머(인피니티 성공시 5분으로 설정)

	BOOL			m_bAlert5Min, m_bAlert3Min, m_bAlert1Min, m_bAlert30Sec;


	CINFImageEx*		m_pTenderBG;				// 입찰 백그라운드

	CINFImageEx*		m_pTenderProgressFullBar;	// 프로그래스 꽉찬거

	CINFImageEx*		m_pTenderProgressHalfBar;	// 프로그래스 반찬거

	CINFImageEx*		m_pTenderTime[ 10 ];		// 입찰 카운트 시간

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*		m_pRenewInfiTenderImage;
#endif


	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	CINFImageBtn*	m_pTenderYes;				// 입찰
//
// 	CINFImageBtn*	m_pTenderNo;				// 입찰포기
//
// 	UINT			m_nTenderEndTime;			// 시간
// 
// 	UINT			m_nTenderLeftTime;
// 
// 	CINFImage*		m_pTenderItemBigIcon;		// 입찰아이템 빅아이콘
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)


	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	CINFImageEx*		m_pTipImg;					// 팁 이미지

	BOOL			m_bTipShow;					// 팁을 보여줄건가?

	DWORD			m_dwTipShowStartTime;		// 팁을 보여주기 시간한 시간

	DWORD			m_dwTipAlphaColor;			// 팁 알파값


	std::vector<KeyMonsterInfo*>	m_vecKeyMonster;		// 키몬스터 리스트

	std::map<INT, CINFImageEx*>		m_mapKeyMonsterImg;		// 키몬스터 이미지 리스트

	CINFImageEx*						m_pHPBG;				// HP배경

	CINFImageEx*						m_pHP;					// HP바

	CINFImageEx*						m_pWarning;				// 빨간색 경고 표시
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
	std::vector<TenderItem*>		m_vecTenderItemList;
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)


	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	D3DXVECTOR2						m_v2AlarmTargetPos;

	tdf_vecAlarmDefenseStep			m_vecAlarmDefenseStep;

	INT								m_iAlarm_DefenseStep;
	INT								m_iAlarm_DefenseStepFrame;

	CINFImageEx*						m_pIMG_Label_Step;

	CINFImageEx*						m_pIMG_Label_BNum[10];
	CINFImageEx*						m_pIMG_Label_SNum[10];
	// end 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

	// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	BOOL							m_bEnableCinemaSkip;		// 결과 시네마 연출 스킵 체크값.


	CD3DHanFont*					m_pFontInfinity;

// 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축
	int								m_iTip_Normaltime;
	int								m_iTip_Fadeintime;
	int								m_iTip_Fadeouttime;
	int								m_iTip_Wholetime;
//end 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축

// 2011-11-15 EP4 지옥맵 시간 처리
	int								m_nOldtime; 
	int								tick;
//end 2011-11-15 EP4 지옥맵 시간 처리

public:
	CINFInfinity();
	virtual ~CINFInfinity();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void	Tick( void );
	
	void	RenderTimeLimit( void );

	// 제한 시간 설정
	void	SetInfinityLimitTime( ATUM_DATE_TIME* pInfinityStartTime, MSec_t nLimitTime );

	// 2011-05-30 by jhahn	인피3차 시간 셋팅
	void	SetInfinityLimitTimeReset(  MSec_t nLimitTime );
	// end 2011-05-30 by jhahn	인피3차 시간 셋팅

	//  2012-04-27 by jhahn	트리거 시스템 시간 셋팅
	void	SetInfinityTriggerLimitTimeReset(  MSec_t nLimitTime );
	// end 2012-04-27 by jhahn	트리거 시스템 시간 셋팅
	// 제한 시간 변경 설정
	void	SetInfinityChangeLimitTime( MSG_FC_INFINITY_CHANGE_LIMITTIME* pMsg );
	// 페널티 시간 설정
	inline void	SetInfinityPenaltyTime( MSec_t nPenaltyTime )
	{
		m_nInfinityPenaltyTime	= nPenaltyTime;
	}

	void		SetHellMap();
	BOOL		HellMapOn;

	void	RenderInfinityResult( void );
	// 인피니티 결과 설정
	void	SetResult( BOOL bSucceeded );

	void	RenderTenderItem();
	// 2010. 05. 27 by jskim 시네마 적용 카메라 구현
	void	RenderResult();
	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
// 	inline	void	SetTenderItemBigIcon( CINFImage* pBigIcon )
// 	{
// 		m_pTenderItemBigIcon	= pBigIcon;
// 	}
// 	
// 	inline	void	SetTenderEndTime( UINT nTenderEndTime )
// 	{
// 		m_nTenderEndTime	= nTenderEndTime;
// 	}
//

	// 2010-06-25 by shcho, 인피니티 관련로그 찍기
// 	void	SendPutInTender( BOOL bGiveUp );
//void	SendPutInTender( UINT nItemFieldIndex, BOOL bGiveUp );
	void	SendPutInTender( UINT nItemFieldIndex, ItemNum_t nItemNum, BOOL bGiveUp );
	// end 2010-06-25 by shcho, 인피니티 관련로그 찍기
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	void	SetTimelimit(){	m_bRenderTimeLimit = FALSE;}	  // 2011-11-15 EP4 지옥맵 시간 처리
	void	SetInfinityEnd( void );

	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	// 팁 이미지 셋팅
	void	SetTipImg( char* szTipFileName );

// 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축
	void SetTipTick(BYTE choice);

	void SerchTipImg( char* szTipFileName );
//end 2011-07-22 by jhahn 인피3차 (연출 처리) 시간단축

	// 팁 렌더링
	void	RenderTipImg( void );

	// 키몬스터 추가
	void	AddKeyMonster( MEX_MONSTER_INFO* pMonsterInfo, ClientIndex_t nMonsterUID, INT nCurHP , INT nMaxHP );

	// 키몬스터 이미지 추가
	void	AddKeyMonsterImg( MEX_MONSTER_INFO* pMonsterInfo );

	// 키몬스터 얻어오기
	KeyMonsterInfo*	GetKeyMonsterInfo( ClientIndex_t nMonsterUID );

	// 키몬스터 체력 설정
	void	SetKeyMonsterHP( ClientIndex_t nMonsterUID, INT nCurHP );

	// 키몬스터 그리기
	void	RenderKeyMonster( void );
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

	// 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)
	void	AddTenderItem( MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pTenderItem );

	void	RemoveTenderItem( UINT nItemFieldIndex );

	TenderItem*	GetTenderItem( UINT nItemFieldIndex );

	void	ReArrangeTenderItemPosition( void );

	void	ClearTenderItemList( void );

	int		WndProcTender( UINT uMsg, WPARAM wParam, LPARAM lParam );
	// end 2010. 04. 13 by ckPark 인피니티 필드 2차(입찰 보상관련 변경)

	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)

	// 거점 방어 단계 알림 연출 설정.
	void	SetAlarm_DefenseStep ( const INT a_iStep );

	// 거점 방어 단계 알리 연출 설정.
	void	SetAlarm_DefenseStep ( char *pStepMsg );

	// 거정 방어 단계 알림 연출 출력.
	void	RenderAlarm_DefenseStep ( void );

	// 거점 방어 단계 증가 처리.
	void	SetCount_Alarm_DefenseStep ( const INT a_iAddCount = 1 );

	inline	void SetValue_Alarm_DefenseStep ( const INT a_iStep ) { m_iAlarm_DefenseStep = a_iStep; }

	inline	INT GetValue_Alarm_DefenseStep ( void ) { return m_iAlarm_DefenseStep; }

	// 2010. 05. 17 by hsLee 인피니티 필드 2차 UI 추가 수정. 단계 표시 버그 수정.

	BOOL	IsEvent_AlarmDefenseStep ( char *pStrBuff );

	// End 2010. 05. 17 by hsLee 인피니티 필드 2차 UI 추가 수정. 단계 표시 버그 수정.

	// End 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)


	// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	inline BOOL GetEnableCinemaSkip ( void ) { return m_bEnableCinemaSkip; }			// 시네마 연출 스킵 활성화값 리턴.
	inline void	SetEnableCinemaSkip ( BOOL Enable ) { m_bEnableCinemaSkip = Enable; }	// 시네마 연출 스킵 활성화값 대입.

	void SkipEndingCinema ( char *pCName , const bool bNormalEnding = false );			// 시네마 연출 스킵 처리.

	void SendPacket_SkipEndingCinema ( const bool a_bEnableNormalEnding = false );		// 시네마 연출 스킵 요청 신호 쏘기.

	void UpdateTenderAllItemInfo_TenderStartTime ( const ATUM_DATE_TIME a_sUpdateTime );		// 입찰 아이템의 입찰 제한 시간 갱신.
	// End 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.


};

#endif // !defined(AFX_INFINFINITY_H__764D1815_2DF5_49F4_B411_53F0EB0C0714__INCLUDED_)