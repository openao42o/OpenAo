// INFItemInfo.h: interface for the CINFItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFITEMINFO_H__3EF422A5_2E9F_43E3_B059_879ADEDAF780__INCLUDED_)
#define AFX_INFITEMINFO_H__3EF422A5_2E9F_43E3_B059_879ADEDAF780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

//#define ITEMINFO_TITLE_NUMBER		9
#define ITEMINFO_PARAMETER_NUMBER	24//14	// desc는 뺀다.	// 2008-09-26 by bhsohn 신규 인첸트 처리
#define ITEMINFO_DESC_LINE_NUMBER	6	// desc는 6줄까지 지원
#define ITEMINFO_EXTENDITEM_LINE_NUMBER	6	// Extenditeminfo는 6줄까지 지원				  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
// 2008-04-14 by bhsohn 유럽 아이템 설명 스트링문제 처리
//#define ITEMINFO_DESC_SIZE			40	// 20 x 6
#define ITEMINFO_DESC_SIZE			256	// 20 x 6

#define ITEMINFO_ITEM_FULL_NAME		140	// 140 = desc title len(20) + prefix len + item name len + suffix len
#define ITEMINFO_BIGICON_WIDTH		84
#define ITEMINFO_TOP_GAB			15
#define ITEMINFO_NAME_IMAGE_GAB		80
#define ITEMINFO_BIGICON_GAB		70

#define ITEMINFO_SIDE_TOPSIZE_WIDTH		4
#define ITEMINFO_SIDE_TOPSIZE_HEIGHT	5
#define ITEMINFO_SIDE_MIDSIZE_WIDTH		4
#define ITEMINFO_SIDE_MIDSIZE_HEIGHT	1
#define ITEMINFO_SIDE_BOTSIZE_WIDTH		4
#define ITEMINFO_SIDE_BOTSIZE_HEIGHT	4
#define ITEMINFO_SIDE_WIDTH_ENGLISH		8							// 영문 글자 WIDTH

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define INCHANTNUM_X				15
#else
#define INCHANTNUM_X				9
#endif

enum FUNCTION_VALUE_TYPE
{
	FUNCTION_VALUE_TYPE_NORMAL,
	FUNCTION_VALUE_TYPE_PROB,
	FUNCTION_VALUE_TYPE_PROB100,
	FUNCTION_VALUE_TYPE_PROB255,
	FUNCTION_VALUE_TYPE_PERCENT	// 2008-09-26 by bhsohn 신규 인첸트 처리
};

enum 
{
	FUNCTION_INDEX_NOT_ACTION = 0,
	FUNCTION_INDEX_ITEM_REMAIN_TIME,
	FUNCTION_INDEX_ITEM_DEL_REMAIN_TIME,
	// 2010. 02. 11 by ckPark 발동류 장착아이템
	FUNCTION_INDEX_ITEM_COOL_TIME
	// end 2010. 02. 11 by ckPark 발동류 장착아이템
};

// 2010. 02. 11 by ckPark 발동류 장착아이템
typedef enum 
{
	FUNCTIONTYPE_NORMAL = 1,
	FUNCTIONTYPE_EQUIP,
	FUNCTIONTYPE_USE,
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	FUNCTIONTYPE_LOOKCHANGE,	// 2013-04-19 by ssjung 외형 변경 킷 강화 시스템
	FUNCTIONTYPE_TIMELIMINT_ATTACK		    //2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시
#endif
} FUNCTION_TYPE;
// end 2010. 02. 11 by ckPark 발동류 장착아이템

struct stTickFuntionIndex
{
	int		nFuntionIndex;
	int		nDataLineIndex;
	int		nParam1;
	int		nParam2;
	int		nParam3;

	stTickFuntionIndex()
	{
		nFuntionIndex	= FUNCTION_INDEX_NOT_ACTION;
		nDataLineIndex	= 0;
		nParam1 = 0;
		nParam2 = 0;
		nParam3 = 0;
	}
};

class CD3DHanFont;
class CParamFactor;
class CINFImage;
class DataHeader;
class CINFImageEx;

class CINFItemInfo : public CAtumNode  
{
protected:
//	CParamFactor	m_paramFactor;
	BOOL			m_bEnableItem;		 // 장착 가능한 아이템 : TRUE
	BOOL			m_bShow;
	char			m_strItemInfo[ITEMINFO_PARAMETER_NUMBER][ITEMINFO_ITEM_FULL_NAME];
	char			m_strDesc[ITEMINFO_DESC_LINE_NUMBER][ITEMINFO_DESC_SIZE];
	char			m_strExtendItemInfo[ITEMINFO_DESC_LINE_NUMBER][ITEMINFO_DESC_SIZE];				  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 

	CD3DHanFont	*	m_pFontItemInfo[ITEMINFO_PARAMETER_NUMBER];
	CD3DHanFont	*	m_pFontDescInfo[ITEMINFO_DESC_LINE_NUMBER];
	CD3DHanFont	*	m_pFontExtendItemInfo[ITEMINFO_DESC_LINE_NUMBER];							   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 

	CD3DHanFont	*	m_pFontItemName;
	CD3DHanFont	*	m_pInchantNum;
	char			m_strItemName[ITEMINFO_ITEM_FULL_NAME];

	int				m_nDescIndex;	// 전체 정보 중에 DESC의 인덱스
	int				m_nExtendItemIndex;																					  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	int				m_nMaxLength;
	int				m_nDescLine;
	RARE_ITEM_INFO *m_pRefPrefixRareInfo;
	RARE_ITEM_INFO *m_pRefSuffixRareInfo;
	CParamFactor *	m_pRefEnchant;
	ITEM		*	m_pRefITEM;					// DB에 있는 ITEM의 정보.
	CItemInfo	*	m_pRefItemInfo;				// 현재 자신이 가지고 있어야만 값이 존재함.
	POINT			m_ptItemInfo;
	CINFImageEx *		m_pBigIcon;
	CINFImageEx	*	m_pInfoBoxSide[9];
	DataHeader*		m_pDataHeader;
	CGameData	*	m_pGameData;

	// 2006-03-31 by ispark, 별도로 셋팅하는 변수
	int				m_nOtherItemCount;		// 미션에서 지급되는 아이템 갯수

	// 2006-10-12 by ispark, 계산이 계속 되어야 하는 별도의 Tick
	vector<stTickFuntionIndex>	m_vecTickFuntionIndex;

	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁	
	BOOL			m_bMyEquipItem;
	CD3DHanFont	*	m_pFontMyEquipItem;
	SIZE			m_szTooltip;	
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	std::vector< std::pair<ItemNum_t, InfinityShopItemCnt_t> >	m_vecExchageMtrl;
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
public:
	CINFItemInfo(CAtumNode* pParent);
	virtual ~CINFItemInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render();
	virtual void Tick();

	void ShowItemInfo(BOOL bShow) { m_bShow = bShow; }	
	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	void SetItemInfoNormal( ITEM* pITEM, int x, int y, BOOL bShop=TRUE, int nItemCount = 0, 
					int nLinkItem=0 , BOOL ArmorCollect = FALSE);	// 2006-03-14 by ispark, nItemCount : 미션지급아이템 개수 추가
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//	void SetItemInfoUser( CItemInfo* pItemInfo, int x, int y );		// 2006-03-31 by ispark, nItemTime : 시간제한 아이템 추가
	void SetItemInfoUser( CItemInfo* pItemInfo, int x, int y, int nLinkItem=0 ,BOOL bInven = FALSE);		// 2006-03-31 by ispark, nItemTime : 시간제한 아이템 추가
	// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	void InitItemInfo();
	void SetGameData(CGameData * pData);
	CINFImageEx* FindBigIcon(int nItemNum);

	BOOL			m_bRestored;

	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	BOOL			IsShowItemInfo();
	void			SetMyEquipItem(BOOL bMyEquipItem);
	int				GetMaxLength();
	CItemInfo	*	GetRefItemInfo();
	POINT			GetItemInfoPos();
	void			SetItemInfoPos(POINT i_ptItemInfoPos);
	SIZE			GetItemInfoTooltipSize();
	void			SetItemInfoTooltipSize(SIZE	i_szTooltip);
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	BOOL IsShowFunction(int nType);	// 2013-02-14 by bhsohn 특정 속성 아이템 툴팁 이상하게 찍히는 현상 처리

	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경
	// 2013-03-27 by bhsohn DestParam 자료형 수정
//	void SetParameterInfo( char * str, BYTE bType,float fValue, BOOL bDefEnchant = TRUE );
//	void SetParameterInfo( char * str, DestParam_t bType,float fValue, BOOL bDefEnchant = TRUE);
	void SetParameterInfo( char * str, DestParam_t bType,float fValue, BOOL bDefEnchant = TRUE , BOOL bMonthlyArmor = FALSE); // 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경 protected -> public
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	void SetRareParameterInfo(char* str, float fValue);
	void DefferentFunction(int* index, CItemInfo* pRefItemInfo);
	//int	 GetRareParameterValue(CItemInfo* pRefItemInfo, int num);
	float GetRareParameterValue(CItemInfo* pRefItemInfo, int num);
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	const ITEM	*GetRefITEM()	{return m_pRefITEM;}					// DB에 있는 ITEM의 정보.
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	// 2013-05-07 by bhsohn M기어 편대버프 리뉴얼 작업
	// 패널티 정보 가져오기
	float GetBuffPanaltyInfo(int nParamSize, DestParam_t *i_bType1, float *i_fValue1, USHORT shUnitKind); 
	// END 2013-05-07 by bhsohn M기어 편대버프 리뉴얼 작업

protected:
	//void SetParameter(int* index, BOOL bLinkItem = FALSE);				// 2006-04-25 by ispark
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	//void SetParameter(int* index, BOOL bLinkItem = FALSE, BOOL bArmorItem = FALSE);				// 2008-09-26 by bhsohn 신규 인첸트 처리
	void SetParameter(int* index, BOOL bLinkItem = FALSE, BOOL bArmorItem = FALSE, float RareValue = 0.0f);				// 2008-09-26 by bhsohn 신규 인첸트 처리

	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	void SetItemAllTime(int nParameterIndex, BOOL bCard = FALSE, BOOL bLinkItem = FALSE);	// 2006-03-31 by ispark
	void SetItemRemainTime(int nParameterIndex, BOOL bShop, BOOL bLinkItem = FALSE, BOOL bSetTick = TRUE);// 2006-03-31 by ispark
	void SetItemDelAllTime(int nParameterIndex);						// 2006-10-11 by ispark
	void SetItemDelRemainTime(int nParameterIndex, BOOL bSetTick = TRUE);	// 2006-10-11 by ispark
	void SetWeaponAngle(int nParameterIndex);
	void SetWeaponSpeed(int nParameterIndex);
	void SetReUseTime(int nParameterIndex);					// 2005-12-07 by ispark
	void SetSkillReAttackTime(int nParameterIndex);			// 2005-11-22 by ispark
	void SetEnginTurningAngle(int nParameterIndex);			// 2005-08-17 by ispark
	void SetRadarRange(int nParameterIndex);
	void SetSecondaryRange(int nParameterIndex);
	void SetPrimaryRange(int nParameterIndex);

	BOOL IsEnableItem(ITEM* pITEM);
	void SetMaxLength();
	void SetItemName( int nParameterIndex );
	void SetItemKind( int nParameterIndex );
	void SetUnitKind( int nParameterIndex );
	void SetReqLevel( int nParameterIndex );
	void SetReqStat(int nParameterIndex );
	void SetReqItemKind( int nParameterIndex );
	void SetShopReqItem(int nParameterIndex );
	void SetAttack( int nParameterIndex );
	void SetSecAttack( int nParameterIndex );
	void SetDefense( int nParameterIndex );
	void SetRate( int nParameterIndex );
	void SetRange( int nParameterIndex );
	void SetAttackTime( int nParameterIndex );
	void SetReAttackTime( int nParameterIndex );
	void SetOverHeatTime( int nParameterIndex );
	void SetRangeAngle( int nParameterIndex );
	void SetExplosionRange( int nParameterIndex );
	void SetReactionRange( int nParameterIndex );
	void SetSkillTime( int nParameterIndex);
	void SetWeight( int nParameterIndex );

	void SetItemLimit(int* nParameterIndex ); // 2012-10-11 by jhahn 기간제 아이템 표시

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	void SetShapeInfo( int nParameterIndex );
	void SetEffectInfo( int nParameterIndex );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	// 발동류 데스파람 툴팁
	void SetInvokeDestParam( int* pParameterIndex );
	// 아이템 쿨타임 툴팁
	void SetItemCoolTime( int* pParameterIndex, BOOL bSetTick = TRUE );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	void SetCountableWeight( int nParameterIndex );
	void SetReqSP( int nParameterIndex );
	void SetSkillLevel( int nParameterIndex );
	void SetBullet( int nParameterIndex );
	void SetFUEL( int nParameterIndex );
	void SetCount( int nParameterIndex );
	void SetSpeed( int nParameterIndex );
	void SetBoosterSpeed( int nParameterIndex );
	void SetCharging(int nParameterIndex);
	void SetBoosterTime( int nParameterIndex );
	void SetRotateAngle( int nParameterIndex );
	void SetBoosterRotateAngle( int nParameterIndex );
	void SetDefenseColorInfo(int  nParameterIndex );
	// 2008-09-26 by bhsohn 신규 인첸트 처리
	void SetPrimaryPierce(int nParameterIndex);
	void SetSecondaryPierce(int nParameterIndex);

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	//void SetFunction(int nParameterIndex, BYTE bType1, float fValue1, BYTE bType2, float fValue2);
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
// 	// 기본 인챈트인지 발동류인 인챈트 추가
// 	void SetFunction( int nParameterIndex,
// 					  BYTE bType1,
// 					  float fValue1,
// 					  BYTE bType2,
// 					  float fValue2,
// 					  FUNCTION_TYPE nFunctionType = FUNCTIONTYPE_NORMAL );
// 	// end 2010. 02. 11 by ckPark 발동류 장착아이템
	// 기본 인챈트인지 발동류인 인챈트 추가
	void SetFunction( int nParameterIndex,
					  DestParam_t bType1,    // 2011-08-02 by jhahn 파트너 성장형 시스템 Byte에서  USHORT형 데스파람으로 변경
					  float fValue1,
					  DestParam_t bType2, //BYTE bType2,// 2013-03-27 by bhsohn DestParam 자료형 수정					  
					  float fValue2,
		FUNCTION_TYPE nFunctionType = FUNCTIONTYPE_NORMAL,
		float fRareValue = 0.0f, 
		BOOL bMonthlyArmor = FALSE);		// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
	// end 2010. 02. 11 by ckPark 발동류 장착아이템
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현


	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	void SetUseAreaInfinity( int* p_nParameterIndex );
	void SetExchangeMaterial( BOOL bShop = FALSE );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	void SetDesc( int nParameterIndex );
	void SetShopSellInfo( int nParameterIndex );
	// 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경 - 위치 이동
	// 	// 2010. 02. 11 by ckPark 발동류 장착아이템
	// 	//void SetParameterInfo(char * str, BYTE bType,float fValue);
	// 	// 기본 인챈트인지 발동류인 인챈트 추가
	// 	void SetParameterInfo( char * str, BYTE bType,float fValue, BOOL bDefEnchant = TRUE );
	// 	// end 2010. 02. 11 by ckPark 발동류 장착아이템
	//end 2010. 03. 31 by jskim 사용류 아이템 보여주는 아이콘 팁 변경

	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 타입 툴팁 추가.
	void SetPetType ( int nParameterIndex );

	void SetPetReName ( int nParameterIndex );
	void SetPetEnableLevelUp ( int nParameterIndex, BOOL bShop = FALSE );
	void SetPetExp    ( int nParameterIndex );
	// End 2010-06-15 by shcho&hslee 펫시스템 - 펫 타입 툴팁 추가.

// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
	void SetRadarItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);						// 2005-08-17 by ispark
	void SetPrimaryWeaponInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetSecondaryWeaponInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetSkillItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetDefenseItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetSupportItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetEnergyItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetIngotItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetCardItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetEnchantItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
//	void SetTankItemInfo(BOOL bShop=FALSE);							// 2006-04-24 by ispark, -> SetAccessoryUnLimitItemInfo()로 변경
	void SetBulletItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetComputerItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetArmorColorInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void PrevetionDeleteItem(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetAccessoryUnLimitItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);				// 2006-04-24 by ispark

	void SetPetSoketItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);						//2011-10-06 by jhahn 파트너 성장형 시스템
	void SetAccessoryTimeLimitItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);			// 2006-03-30 by ispark
	void SetInfluenceBuffItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);				// 2006-04-25 by ispark
	void SetInfluenceGameEventItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);			// 2006-04-25 by ispark
	void SetRandomBoxItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);					// 2006-08-11 by dgwoo 랜덤박스.
	void SetExclusiveUser(int nParameterIndex);
	void SetMarkItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);						// 2006-08-21 by ispark
	void SetSkillSupportItem(BOOL bShop = FALSE, BOOL bInven = FALSE);					// 2006-10-02 by ispark
	void SetPetItemInfo ( BOOL bShop = FALSE , BOOL bInven = FALSE);						// 2010-06-15 by shcho&hslee 펫시스템
// end 2013-06-26 by ssjung 인벤토리 추가 툴팁 		
	void RenderInfoWindows(int x, int y, int cx, int cy);		
	DataHeader* FindResource(char* szRcName);

	void SetOtherFuntionTick();

	// 2007-09-07 by bhsohn 아이템 거래여부 표시
	void SetItemAttribute( int nParameterIndex);

	// 2008-04-14 by bhsohn 유럽 아이템 설명 스트링문제 처리
	int GetItemStringLen();
	BOOL IsStringColor(char *i_szStr);

	// 2008-09-26 by bhsohn 신규 인첸트 처리
	float GetEnchantPreSuffixInfo(int nDesParameter);

	// 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정
//	BOOL	SetRemainTime_Imp( CItemInfo* pRefItem, int* pIndex, BOOL bShop );
	BOOL	SetRemainTime_Imp( ITEM* pRefItem, int* pIndex, BOOL bShop ); // 2013-07-05 by ssjung 조합정보 크래쉬 관련 (인자값을 CItemInfo*에서 ITEM*으로 바꿈)
	// end 2009. 10. 28 by ckPark 시간/기간 제한 아이템 툴팁 수정

	BOOL SingofDecimalDecision(float fVal);

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	void SetLookChangeSystem(int* pParameterIndex, int nSourceItemNum);	//2013-05-20 by ssjung 이달의 아머 - 이달의 아머 툴팁 표시
	
// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
	void SetArmorCollectionItemInfo(BOOL bShop = FALSE);			
    void SetTimeLimt(int* nParameterIndex);
	void CoatingCount(int* nParameterIndex);			
	void SetMonthlyArmorValue(int* nParameterIndex, ATUM_DATE_TIME MonthlyEventEndDate, ItemNum_t MonthlyOptionItemNum);
	void SetArmorCollectionParameter(int* nParameterIndex, int nSourceItemNum = 0);
	void SetMonthlyArmorCollectionValue(int* nParameterIndex);
	void SetArmorAbilityValue(int* nParameterIndex, ITEM* pTempItem, int nParamValue, BOOL bCard);
// end 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
#endif
	void SetItemExtendInfo(BOOL bShow = FALSE, BOOL bflag = FALSE);									  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
};

#endif // !defined(AFX_INFITEMINFO_H__3EF422A5_2E9F_43E3_B059_879ADEDAF780__INCLUDED_)
