// ItemInfo.h: interface for the CItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMINFO_H__1B0BB4FA_5A16_4585_8186_2F1C911EE29C__INCLUDED_)
#define AFX_ITEMINFO_H__1B0BB4FA_5A16_4585_8186_2F1C911EE29C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumParam.h"

// 2010. 02. 11 by ckPark 발동류 장착아이템
struct ENCHANT_PARAM
{
	DestParam_t		m_nDesParam;		// 데스파람
	ParamValue_t	m_fDesValue;		// 데스파람 값

	ENCHANT_PARAM( DestParam_t nDesParam = 0, ParamValue_t fDesValue = 0.0f ) : m_nDesParam( nDesParam ), m_fDesValue( fDesValue )
	{

	}
};
// end 2010. 02. 11 by ckPark 발동류 장착아이템

///////////////////////////////////////////////////////////////////////////////
/// \class		CItemInfo
///
/// \brief		아이템(유저가 가진 유니크넘버 구분)의 정보 테이블
///				CStoreData의 m_mapItemInInventory는 이 정보를 들고 있어야 한다. 
/// \author		dhkwon
/// \version	
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CItemInfo : public ITEM_GENERAL
{
public:
	CItemInfo(ITEM_GENERAL* pItem);
	virtual ~CItemInfo();

	void AddEnchantItem(INT nEnchantItemNum);
	RARE_ITEM_INFO  *GetPrefixRareInfo();// { return m_pRefPrefixRareInfo; }
	RARE_ITEM_INFO  *GetSuffixRareInfo();// { return m_pRefSuffixRareInfo; }
	CParamFactor	*GetEnchantParamFactor() { return m_pEnchantParamFactor; }	//	인챈트 파라미터만 계산한 값
	int				GetEnchantNumber() { return m_nEnchantNumber; }
	CParamFactor	*GetParamFactor() { return m_pAllParamFactor; }	// m_pPrefixRareInfo,m_pSuffixRareInfo,m_pEnchantParamFactor 가 모두 적용된 결과값
	ITEM* GetRealItemInfo();			// 아이템 진짜 정보(인챈트,레어 정보 들어감)
	ITEM* GetItemInfo();				// 아이템 정보(서버,기본 정보)
	ITEM* ResetRealItemInfo();			// 아이템 진짜 정보 다시 세팅(인챈트 때문에)

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	//void SetEnchantParam( CParamFactor * pEnchantParam, int nEnchantNumber );	// 아이템 복제시(인챈트 정보만 뽑음)
	// 발동류 데스파람 인챈트 추가
	void SetEnchantParam( CParamFactor* pEnchantParam,
						  std::vector<ENCHANT_PARAM>* pDefEnchant,
						  std::vector<ENCHANT_PARAM>* pEnchantParamList,
						  int nEnchantNumber );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	void ChangeRareInfo(INT nPrefixCodeNum, INT nSuffixCodeNum);
	void DeleteEnchantInfo();
	void ReleaseRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex);
	float GetItemPassTime() { return m_fItemPassTime; }
	void SetItemPassTime(float fPassTime) { m_fItemPassTime = fPassTime; }
	float GetDesItemWeight();
	
	int		TickReUsable(float fElapsedTime);
	int		UseItem();

	// 2008-08-19 by bhsohn 세력 마크 시스템 추가
	void	ChangeItemNum(INT nChangeItemNum);

	// 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
	void	GetItemFullName(char* o_pItemName);
	// end 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	// 쿨타임
	INT							GetCoolElapsedTime( void );
	VOID						SetCoolElapsedTime( const INT nCoolElapsedTime );


	// 기본 인챈트 정보 추가
	void						PushDefEnchantParam( ENCHANT_PARAM ep );
	// 기본 인챈트 값
	ParamValue_t				GetDefEnchantParamValue( DestParam_t nDesParam );
	// 기본 인챈트 리스트
	std::vector<ENCHANT_PARAM>*	GetDefEnchantParamList( void );
	// 모든 기본 인챈트 정보 날리기
	void						ClearDefEnchantParam( void );


	// 발동류 인챈트 정보 추가
	void						PushInvokeEnchantParam( ENCHANT_PARAM ep );
	// 발동류 인챈트 값
	ParamValue_t				GetInvokeEnchantParamValue( DestParam_t nDesParam );
	// 발동류 인챈트 리스트
	std::vector<ENCHANT_PARAM>*	GetInvokeEnchantParamList( void );
	// 모든 발동류 인챈트 정보 날리기
	void						ClearInvokeEnchantParam( void );


	// 인챈트 및 쿨타임 정보 복사
	void						CopyItemInfo( CItemInfo* pSrcItem );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	// 2010-06-15 by shcho&hslee 펫시스템 - 거래 툴팁
	void SetPetLevel( int level ) { m_nPetLevel = level; }
	int  GetPetLevel( ) { return m_nPetLevel; }

	void SetReName( BOOL rename ) { m_bReName = rename; }
	BOOL GetReName( ) { return m_bReName; }

	void SetPetEnableLevelUp( BOOL EnableLevelUp ) { m_bEnableLevelUp = EnableLevelUp; }
	BOOL GetPetEnableLevelUp() { return m_bEnableLevelUp; } 

	void SetPetExp( Experience_t Exp ) { m_dPetExp = Exp; }
	Experience_t GetPetExp() { return m_dPetExp; }
	// end 2010-06-15 by shcho&hslee 펫시스템 - 거래 툴팁

	UID64_t GetUniqueNumber(){return m_UniqueNumber;}	// 2012-06-14 by isshin 아템미리보기

	// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
	void GetItemGeneral(ITEM_GENERAL* pItemGeneral);
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	BOOL GetLimitTimeFlag(){return m_bLimitTime;}					// 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현
	void SetLimitTimeFlag(BOOL bFlag){m_bLimitTime = bFlag;}		// 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현
#endif
protected:
	void SetRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex);// m_pPrefixRareInfo, m_pSuffixRareInfo의 정보 세팅용
	void PutEnchant(BYTE nDestParameter, float fParameterValue);					// 서버에서 인챈트 정보 전송 후

	// 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
	void SortItemName(char* o_pItemName);
	// end 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
	
protected:
	RARE_ITEM_INFO  *m_pRefPrefixRareInfo;
	RARE_ITEM_INFO  *m_pRefSuffixRareInfo;
	CParamFactor	*m_pEnchantParamFactor;
	int				m_nEnchantNumber;
	CParamFactor	*m_pAllParamFactor;		
	ITEM			*m_pItemInfo;			// m_pAllParamFactor 가 적용된 아이템

	float			m_fItemPassTime;		// 2006-03-31 by ispark, 시간제한 아이템 경과된 시간
	int				m_nReUsable;			// 재 사용시간.
	BOOL			m_bUse;					// 사용할수 있는 상태인가?

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	INT				m_nCoolElapsedTime;		// 쿨타임

	std::vector<ENCHANT_PARAM> m_vecDefEnchant;		// 기본 인챈트 리스트

	std::vector<ENCHANT_PARAM> m_vecInvokeEnchant;	// 발동류 인챈트 리스트
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	// 2010-06-15 by shcho&hslee 펫시스템 - 거래 툴팁
	int				m_nPetLevel;
	BOOL			m_bReName;
	BOOL			m_bEnableLevelUp;
	Experience_t	m_dPetExp;
	// end 2010-06-15 by shcho&hslee 펫시스템 - 거래 툴팁
		
	UID64_t			m_UniqueNumber;			// 2012-06-14 by isshin 아템미리보기
	BOOL			m_bLimitTime;			// 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현

};

#endif // !defined(AFX_ITEMINFO_H__1B0BB4FA_5A16_4585_8186_2F1C911EE29C__INCLUDED_)
