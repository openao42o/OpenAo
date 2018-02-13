// ItemInfo.cpp: implementation of the CItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemInfo.h"
#include "AtumDatabase.h"

extern CAtumDatabase * g_pDatabase;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemInfo::CItemInfo(ITEM_GENERAL* pItem)
{
	Kind = pItem->Kind;
	UniqueNumber = pItem->UniqueNumber;
	ItemNum = pItem->ItemNum;
	Wear = pItem->Wear;
	AccountUniqueNumber = pItem->AccountUniqueNumber;
	Possess = pItem->Possess;
	CurrentCount = pItem->CurrentCount;
	
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//ScarcityNumber = pItem->ScarcityNumber;
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	ItemWindowIndex = pItem->ItemWindowIndex;
	NumOfEnchants = pItem->NumOfEnchants;
	PrefixCodeNum = pItem->PrefixCodeNum;
	SuffixCodeNum = pItem->SuffixCodeNum;
	CurrentEndurance = pItem->CurrentEndurance;
	ColorCode = pItem->ColorCode;

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	ShapeItemNum	= pItem->ShapeItemNum;
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	UsingStartTime = pItem->UsingStartTime;
	CreatedTime = pItem->CreatedTime;
	ItemInfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
	m_pRefPrefixRareInfo = NULL;
	m_pRefSuffixRareInfo = NULL;
	m_pEnchantParamFactor = NULL;
	m_pAllParamFactor = NULL;
	m_nEnchantNumber = 0;
	m_pItemInfo = NULL;
	m_fItemPassTime = (float)pItem->UsingTimeStamp;
	m_nReUsable = 0;
	m_bUse = TRUE;
	GetPrefixRareInfo();
	GetSuffixRareInfo();
	GetRealItemInfo();

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	CoolingTimeStamp = pItem->CoolingTimeStamp;
	CoolingStartTime = pItem->CoolingStartTime;
	// 기본 재사용 시간설정
	SetCoolElapsedTime( pItem->CoolingTimeStamp * 1000 );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	// 2010-06-15 by shcho&hslee 펫시스템 - 거래 툴팁
	m_nPetLevel		= NULL;
	m_bReName		= FALSE;
	m_dPetExp		= NULL;
	// end 2010-06-15 by shcho&hslee 펫시스템 - 거래 툴팁
	
	m_UniqueNumber = pItem->UniqueNumber;	// 2012-06-14 by isshin 아템미리보기

	FixedTermShape = pItem->FixedTermShape;	 // 2012-10-13 by jhahn 기간제 아이템 추가 

	m_bLimitTime = FALSE;			// 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현
	
	// 2013-05-15 by ssjung 이달의 아머 - 이달의 아머 변경 사항 패킷 관련 
	nMonthlyOptionItemNum = pItem->nMonthlyOptionItemNum;
	atMonthlyEventEndDate = pItem->atMonthlyEventEndDate;
	// end 2013-05-15 by ssjung 이달의 아머 - 이달의 아머 변경 사항 패킷 관련 
}

CItemInfo::~CItemInfo()
{
//	util::del(m_pRefPrefixRareInfo);// AtumDatabase
//	util::del(m_pRefSuffixRareInfo);// AtumDatabase
	util::del(m_pEnchantParamFactor);
	util::del(m_pAllParamFactor);
	util::del(m_pItemInfo);
}

RARE_ITEM_INFO* CItemInfo::GetPrefixRareInfo() 
{ 
	if(PrefixCodeNum != 0 && m_pRefPrefixRareInfo == NULL)
	{
		m_pRefPrefixRareInfo = g_pDatabase->GetServerRareItemInfo(PrefixCodeNum);
		SetRareItemInfo( m_pRefPrefixRareInfo, PrefixCodeNum );
	}
	return m_pRefPrefixRareInfo; 
}

RARE_ITEM_INFO* CItemInfo::GetSuffixRareInfo() 
{ 
	if(SuffixCodeNum != 0 && m_pRefSuffixRareInfo == NULL)
	{
		m_pRefSuffixRareInfo = g_pDatabase->GetServerRareItemInfo(SuffixCodeNum);
		SetRareItemInfo( m_pRefSuffixRareInfo, SuffixCodeNum );
	}
	return m_pRefSuffixRareInfo; 
}
void CItemInfo::ChangeRareInfo(INT nPrefixCodeNum, INT nSuffixCodeNum)
{
	if(PrefixCodeNum != 0)
	{
		ReleaseRareItemInfo(m_pRefPrefixRareInfo, PrefixCodeNum);
	}
	if(SuffixCodeNum != 0)
	{
		ReleaseRareItemInfo(m_pRefSuffixRareInfo, SuffixCodeNum);
	}
	PrefixCodeNum = nPrefixCodeNum;
	m_pRefPrefixRareInfo = NULL;
	GetPrefixRareInfo();
	SuffixCodeNum = nSuffixCodeNum;
	m_pRefSuffixRareInfo = NULL;
	GetSuffixRareInfo();
	GetRealItemInfo();	
	// 2007-10-16 by bhsohn 인첸 성공후 정보 갱신
	ResetRealItemInfo();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CItemInfo::SetRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex)
/// \brief		m_pPrefixRareInfo, m_pSuffixRareInfo의 정보 세팅용
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	m_pAllParamFactor가 없으면 새로 만든다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::SetRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex)
{
	if(pInfo)
	{
		if(m_pAllParamFactor == NULL)
		{
			m_pAllParamFactor = new CParamFactor;
			m_pAllParamFactor->Reset();
		}
		for(int i=0; i<SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if(pInfo->DesParameter[i] != DES_NULL)
			{
				SetParamFactor_DesParam( *m_pAllParamFactor, pInfo->DesParameter[i], 
							  pInfo->ParameterValue[i] );
			}
		}
	}
}

void CItemInfo::ReleaseRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex)
{
	if(pInfo)
	{
		ASSERT_ASSERT(m_pAllParamFactor != NULL);
		for(int i=0; i<SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if(pInfo->DesParameter[i] != DES_NULL)
			{
				ReleaseParamFactor_DesParam( *m_pAllParamFactor, pInfo->DesParameter[i], 
							  pInfo->ParameterValue[i] );
			}
		}
	}
}


void CItemInfo::AddEnchantItem(INT nEnchantItemNum)
{
	ITEM* pEnchantITEM = g_pDatabase->GetServerItemInfo( nEnchantItemNum );
	if( pEnchantITEM )
	{
		// 2009-04-21 by bhsohn 아이템 DesParam추가
// 		if( pEnchantITEM->DestParameter1 != DES_NULL )
// 		{
// 			PutEnchant(pEnchantITEM->DestParameter1, pEnchantITEM->ParameterValue1);
// 		}
// 		if( pEnchantITEM->DestParameter2 != DES_NULL )
// 		{
// 			PutEnchant(pEnchantITEM->DestParameter2, pEnchantITEM->ParameterValue2);
// 		}
// 		if( pEnchantITEM->DestParameter3 != DES_NULL )
// 		{
// 			PutEnchant(pEnchantITEM->DestParameter3, pEnchantITEM->ParameterValue3);
// 		}
// 		if( pEnchantITEM->DestParameter4 != DES_NULL )
// 		{
// 			PutEnchant(pEnchantITEM->DestParameter4, pEnchantITEM->ParameterValue4);
// 		}
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			if( pEnchantITEM->ArrDestParameter[nArrParamCnt] != DES_NULL )
			{
				PutEnchant(pEnchantITEM->ArrDestParameter[nArrParamCnt], 
							pEnchantITEM->ArrParameterValue[nArrParamCnt]);

				// 2010. 02. 11 by ckPark 발동류 장착아이템
				PushDefEnchantParam( ENCHANT_PARAM( pEnchantITEM->ArrDestParameter[nArrParamCnt],
													pEnchantITEM->ArrParameterValue[nArrParamCnt] ) );
				// end 2010. 02. 11 by ckPark 발동류 장착아이템
			}
		}		
		// end 2009-04-21 by bhsohn 아이템 DesParam추가

		// 2010. 02. 11 by ckPark 발동류 장착아이템
		// 발동류 데스파람 인챈트
		if( pEnchantITEM->InvokingDestParamID )
		{
			// 해당 ID의 인챈트 데스파람 정보 가져오기
			CVectorInvokingWearItemDP vec;
			g_pDatabase->GetInvokingWearItemDPList( &vec, pEnchantITEM->InvokingDestParamID );

			// 데스파람 인챈트 정보 추가
			CVectorInvokingWearItemDPIt it = vec.begin();
			while( it != vec.end() )
			{
				PushInvokeEnchantParam( ENCHANT_PARAM( (*it)->InvokingDestParam, (*it)->InvokingDestParamValue ) );
				++it;
			}
		}

		// 사용류 데스파람 인챈트
		if( pEnchantITEM->InvokingDestParamIDByUse )
		{
			// 해당 ID의 데스파람 가져오기
			CVectorInvokingWearItemDP vec;
			g_pDatabase->GetInvokingWearItemDPByUseList( &vec, pEnchantITEM->InvokingDestParamIDByUse );

			// 데스파람 인챈트 정보 추가
			CVectorInvokingWearItemDPIt it = vec.begin();
			while( it != vec.end() )
			{
				PushInvokeEnchantParam( ENCHANT_PARAM( (*it)->InvokingDestParam, (*it)->InvokingDestParamValue ) );
				++it;
			}
		}
		// end 2010. 02. 11 by ckPark 발동류 장착아이템
	}
	m_nEnchantNumber++;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemInfo::PutEnchant(BYTE nDestParameter, float fParamterValue)
/// \brief		인챈트 정보를 모아 m_pEnchantParamFactor에 넣고, m_pAllParamFactor에도 넣는다.
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	m_pEnchantParamFactor, m_pAllParamFactor가 없으면 새로 만든다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::PutEnchant(BYTE nDestParameter, float fParameterValue)
{
//	m_nEnchantNumber++;
	if(m_pEnchantParamFactor == NULL)
	{
		m_pEnchantParamFactor = new CParamFactor;
		m_pEnchantParamFactor->Reset();
	}
	SetParamFactor_DesParam( *m_pEnchantParamFactor, nDestParameter, fParameterValue );
	if(m_pAllParamFactor == NULL)
	{
		m_pAllParamFactor = new CParamFactor;
		m_pAllParamFactor->Reset();
	}
	SetParamFactor_DesParam( *m_pAllParamFactor, nDestParameter, fParameterValue );
	ResetRealItemInfo();

//	최초 시작시 보내는 코드(삭제)
//	if( Wear == WEAR_LOADED &&
//		IS_PRIMARY_WEAPON(Kind) &&
//		(nDestParameter == DES_SHOTNUM_01 || 
//		nDestParameter == DES_REATTACKTIME_01) )
//	{
//		g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
//	}
}

void CItemInfo::DeleteEnchantInfo()
{
	m_nEnchantNumber = 0;
	
	// 2010. 02. 11 by ckPark 발동류 장착아이템
	ClearDefEnchantParam();
	ClearInvokeEnchantParam();
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	if(m_pEnchantParamFactor == NULL)
	{
		return;
	}
	if(m_pAllParamFactor == NULL)
	{
		m_pAllParamFactor = new CParamFactor;
		m_pAllParamFactor->Reset();
	}
	else
	{
		if(m_pEnchantParamFactor)
		{
			*m_pAllParamFactor -= *m_pEnchantParamFactor;
		}
		else
		{
			//ERROR;
//			DBGOUT("Error : Delete Enchant information \n");// 인탠트 삭제정보 오류
		}
	}
	util::del(m_pEnchantParamFactor);
	ResetRealItemInfo();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemInfo::SetEnchantParam( CParamFactor * pEnchantParam, int nEnchantNumber )
/// \brief		EnchantParamFactor를 외부에서 세팅한다.
/// \author		dhkwon
/// \date		2004-07-18 ~ 2004-07-18
/// \warning	아이템 복제할 때 인챈트 정보만 따로 뽑아서 복사한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////


// 2010. 02. 11 by ckPark 발동류 장착아이템
// void CItemInfo::SetEnchantParam( CParamFactor * pEnchantParam, int nEnchantNumber )
// {
// 	if(pEnchantParam)
// 	{
// 		m_nEnchantNumber = nEnchantNumber;
// 		if(m_pEnchantParamFactor == NULL)
// 		{
// 			m_pEnchantParamFactor = new CParamFactor;
// 			m_pEnchantParamFactor->Reset();
// 		}
// 		*m_pEnchantParamFactor = *pEnchantParam;
// 		if(m_pAllParamFactor == NULL)
// 		{
// 			m_pAllParamFactor = new CParamFactor;
// 			m_pAllParamFactor->Reset();
// 		}
// 		ParamFactor_plus_ParamFactor( *m_pAllParamFactor, *m_pEnchantParamFactor );
// 		ResetRealItemInfo();
// 	}
// }
// 인챈트 정보 복사
void CItemInfo::SetEnchantParam( CParamFactor* pEnchantParam,
								 std::vector<ENCHANT_PARAM>* pDefEnchant,
								 std::vector<ENCHANT_PARAM>* pEnchantParamList,
								 int nEnchantNumber )
{
	// 기본 데스파람 인챈트
	if(pEnchantParam)
	{
		if(m_pEnchantParamFactor == NULL)
		{
			m_pEnchantParamFactor = new CParamFactor;
			m_pEnchantParamFactor->Reset();
		}
		*m_pEnchantParamFactor = *pEnchantParam;
		if(m_pAllParamFactor == NULL)
		{
			m_pAllParamFactor = new CParamFactor;
			m_pAllParamFactor->Reset();
		}
		ParamFactor_plus_ParamFactor( *m_pAllParamFactor, *m_pEnchantParamFactor );
		ResetRealItemInfo();

		m_vecDefEnchant = *pDefEnchant;
	}

	// 발동류 데스파람 인챈트
	if( pEnchantParamList )
		m_vecInvokeEnchant = *pEnchantParamList;

	// 총인챈트 갯수
	m_nEnchantNumber = nEnchantNumber;
}
// end 2010. 02. 11 by ckPark 발동류 장착아이템


///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM* CItemInfo::GetItemInfo()
/// \brief		아이템 세부 정보를 리턴한다.
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	서버값과 다른 인챈트,레어정보가 포함된 값이다.
///				인챈트, 레어 정보는 m_pAllParamFactor에 이미 들어가 있다.
///				SetRareItemInfo, PutEnchant
/// \param		
/// \return		ITEM*
///////////////////////////////////////////////////////////////////////////////
ITEM* CItemInfo::GetRealItemInfo()
{
	if(m_pItemInfo == NULL)
	{
		ITEM* pITEM = g_pDatabase->GetServerItemInfo(ItemNum);	// 기본 정보
		if(pITEM)
		{
			if(pITEM != ItemInfo)
			{
				ItemInfo = pITEM;
			}
			m_pItemInfo = new ITEM;
			memcpy( m_pItemInfo, pITEM, sizeof(ITEM));
			if(m_pAllParamFactor)
			{
				SetItem_ParamFactor( *m_pItemInfo, *m_pAllParamFactor );	// ParamFactor 세팅
			}
		}
	}
	
	return m_pItemInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM* CItemInfo::ResetRealItemInfo()
/// \brief		m_pItemInfo를 다시 세팅한다.
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	인챈트 정보가 변경된 경우 실행한다.
///
/// \param		
/// \return		ITEM*
///////////////////////////////////////////////////////////////////////////////
ITEM* CItemInfo::ResetRealItemInfo()
{
	util::del(m_pItemInfo);
	GetRealItemInfo();
	return m_pItemInfo;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM* CItemInfo::GetItemInfo()
/// \brief		아이템 인포(서버 정보)
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM* CItemInfo::GetItemInfo()
{
	if(ItemInfo == NULL)
	{
		ItemInfo = g_pDatabase->GetServerItemInfo(ItemNum);
	}
	return ItemInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int		TickReUsable(float fElapsedTime);
/// \brief		
/// \author		dgwoo
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		남은 시간을 리턴한다.(ms) 사용가능할땐 0을 리턴한다.
///////////////////////////////////////////////////////////////////////////////
int	CItemInfo::TickReUsable(float fElapsedTime)
{
	if(!m_bUse)
	{
		m_nReUsable -= (int)(fElapsedTime * 1000);
		if(m_nReUsable <= 0)
		{
			m_bUse = TRUE;
			m_nReUsable = 0;
		}else
		{
			return m_nReUsable;
		}
	}
	return m_nReUsable;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int		UseItem()
/// \brief		
/// \author		dgwoo
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		남은 시간을 리턴해준다.
///////////////////////////////////////////////////////////////////////////////
int	CItemInfo::UseItem()
{
	m_nReUsable = ItemInfo->ReAttacktime;
	m_bUse = FALSE;

	return m_nReUsable;
}
float CItemInfo::GetDesItemWeight()
{
	if(m_pAllParamFactor == NULL)
	{
		return 0.0f;
	}

	return (m_pAllParamFactor->pfm_WEIGHT_01 + m_pAllParamFactor->pfm_WEIGHT_02);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아이템을 바꾼다.
/// \author		// 2008-08-19 by bhsohn 세력 마크 시스템 추가
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::ChangeItemNum(INT nChangeItemNum)
{
	ItemNum = nChangeItemNum;
	GetPrefixRareInfo();
	GetSuffixRareInfo();
	GetRealItemInfo();	
	ResetRealItemInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아이템의 풀네임을 얻어온다.
/// \author		// 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
/// \date		2009-04-08 ~ 2009-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::GetItemFullName(char* o_pItemName)
{
	o_pItemName[0] = NULL;
	RARE_ITEM_INFO *pRefPrefixRareInfo = GetPrefixRareInfo();
	RARE_ITEM_INFO *pRefSuffixRareInfo = GetSuffixRareInfo();
	if(pRefPrefixRareInfo)
	{
		// 접두사
		wsprintf( o_pItemName, "\\g%s\\g",pRefPrefixRareInfo->Name );
	}
	ITEM* pRealItem = GetRealItemInfo();
	if(pRealItem)
	{
		char chRealName[100];
		ZERO_MEMORY(chRealName);
		if(COMPARE_BIT_FLAG(pRealItem->ItemAttribute, ITEM_ATTR_UNIQUE_ITEM))
		{
			wsprintf( chRealName, "%s",pRealItem->ItemName);
			SortItemName(chRealName);
			// 인첸트 아이템명
			wsprintf( o_pItemName,"%s %s", o_pItemName, chRealName);		
			
		}	
		else if(COMPARE_BIT_FLAG(pRealItem->ItemAttribute, ITEM_ATTR_LEGEND_ITEM))
		{
			wsprintf( chRealName, "%s",pRealItem->ItemName);
			SortItemName(chRealName);
			// 인첸트 아이템명
			wsprintf( o_pItemName,"%s %s", o_pItemName, chRealName);		
		}	
		else if(GetEnchantNumber() > 0)
		{
			wsprintf( chRealName, "\\e%s\\e",pRealItem->ItemName);
			// 인첸트 아이템명
			wsprintf( o_pItemName,"%s %s", o_pItemName, chRealName);		
		}		
		else
		{
			wsprintf( o_pItemName,"%s %s", o_pItemName, pRealItem->ItemName);
		}
	}

	if(pRefSuffixRareInfo)
	{
		char chSuffixName[100];
		ZERO_MEMORY(chSuffixName);
		wsprintf( chSuffixName, "\\g%s\\g",pRefSuffixRareInfo->Name );
		// 접미사
		wsprintf( o_pItemName,"%s %s", o_pItemName, chSuffixName);		
	}
	//색 홀수로 끝나면 문제가 발생하므로 색을 강제로 넣어준다.
	SortItemName(o_pItemName);
}

// 2010. 02. 11 by ckPark 발동류 장착아이템
INT		CItemInfo::GetCoolElapsedTime( void )
{
	return m_nCoolElapsedTime;
}

VOID	CItemInfo::SetCoolElapsedTime( const INT nCoolElapsedTime )
{
	m_nCoolElapsedTime	= nCoolElapsedTime;
}


void	CItemInfo::PushDefEnchantParam( ENCHANT_PARAM ep )
{
	m_vecDefEnchant.push_back( ep );
}

ParamValue_t	CItemInfo::GetDefEnchantParamValue( DestParam_t nDesParam )
{
	// 같은 데스파람의 값을 총 합산
	ParamValue_t fDesValue = 0.0f;
	std::vector<ENCHANT_PARAM>::iterator it = m_vecDefEnchant.begin();
	while( it != m_vecDefEnchant.end() )
	{
		if( (*it).m_nDesParam == nDesParam )
			fDesValue += (*it).m_fDesValue;
		
		++it;
	}
	
	return fDesValue;
}

std::vector<ENCHANT_PARAM>*	CItemInfo::GetDefEnchantParamList( void )
{
	return &m_vecDefEnchant;
}

void	CItemInfo::ClearDefEnchantParam( void )
{
	m_vecDefEnchant.clear();
}

void	CItemInfo::PushInvokeEnchantParam( ENCHANT_PARAM ep )
{
	m_vecInvokeEnchant.push_back( ep );
}

ParamValue_t	CItemInfo::GetInvokeEnchantParamValue( DestParam_t nDesParam )
{
	// 같은 데스파람의 값을 총 합산
	ParamValue_t fDesValue = 0.0f;
	std::vector<ENCHANT_PARAM>::iterator it = m_vecInvokeEnchant.begin();
	while( it != m_vecInvokeEnchant.end() )
	{
		if( (*it).m_nDesParam == nDesParam )
			fDesValue += (*it).m_fDesValue;
		
		++it;
	}
	
	return fDesValue;
}

std::vector<ENCHANT_PARAM>*	CItemInfo::GetInvokeEnchantParamList( void )
{
	return &m_vecInvokeEnchant;
}

void	CItemInfo::ClearInvokeEnchantParam( void )
{
	m_vecInvokeEnchant.clear();
}

void	CItemInfo::CopyItemInfo( CItemInfo* pSrcItem )
{
	// 인챈트 정보 복사
	SetEnchantParam( pSrcItem->GetEnchantParamFactor(),
					 pSrcItem->GetDefEnchantParamList(),
					 pSrcItem->GetInvokeEnchantParamList(),
					 pSrcItem->GetEnchantNumber() );

	// 쿨타임 정보 설정
	SetCoolElapsedTime( pSrcItem->GetCoolElapsedTime() );

	FixedTermShape = pSrcItem->FixedTermShape;		// 2012-10-13 by jhahn 기간제 아이템 추가

}
// end 2010. 02. 11 by ckPark 발동류 장착아이템

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		색 홀수로 끝나면 문제가 발생하므로 색을 강제로 넣어준다.
/// \author		// 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
/// \date		2009-04-08 ~ 2009-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::SortItemName(char* o_pItemName)
{
	int nCnt = 0;
	int nFindColorCnt = 0;
	char chLastColor = 0;
	int nItemLen = strlen(o_pItemName);
	BOOL bFindBackSpace = FALSE;
	for(nCnt = 0 ;nCnt < nItemLen; nCnt++)
	{
		if('\\' == o_pItemName[nCnt])
		{
			bFindBackSpace = TRUE;
		}
		else if(bFindBackSpace && (0 != GetFontColor( o_pItemName[nCnt])))
		{
			bFindBackSpace = FALSE;
			chLastColor = o_pItemName[nCnt];
			nFindColorCnt++;			
		}
		else
		{
			bFindBackSpace = FALSE;
		}

	}
	if(1 == (nFindColorCnt %2) && (0 != chLastColor))
	{
		// 색 갯수가 홀수면 마지막에 강제로 넣어주자
		o_pItemName[nItemLen] = '\\';
		o_pItemName[nItemLen+1] = chLastColor;
		o_pItemName[nItemLen+2] = NULL;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인게임 조합식 추가구현
/// \author		// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
/// \date		2013-04-08 ~ 2013-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::GetItemGeneral(ITEM_GENERAL* pItem)
{
	pItem->Kind = Kind;	 
	pItem->UniqueNumber = UniqueNumber ;
	pItem->ItemNum = ItemNum ;
	pItem->Wear = Wear;
	pItem->AccountUniqueNumber = AccountUniqueNumber;
	pItem->Possess = Possess;
	pItem->CurrentCount = CurrentCount;
	
	pItem->ItemWindowIndex = ItemWindowIndex;
	pItem->NumOfEnchants = NumOfEnchants;
	pItem->PrefixCodeNum = PrefixCodeNum ;
	pItem->SuffixCodeNum = SuffixCodeNum ;
	pItem->CurrentEndurance = CurrentEndurance;
	pItem->ColorCode = ColorCode;
		
	pItem->ShapeItemNum = ShapeItemNum;
	
	
	pItem->UsingStartTime = UsingStartTime;
	pItem->CreatedTime = CreatedTime ;
	pItem->ItemInfo = NULL;
	
	pItem->UsingTimeStamp = m_fItemPassTime;
	
	// 발동류 장착아이템
	pItem->CoolingTimeStamp = CoolingTimeStamp ;
	pItem->CoolingStartTime = CoolingStartTime;
	
	pItem->UniqueNumber = m_UniqueNumber;	// 2012-06-14 by isshin 아템미리보기	
	pItem->FixedTermShape = FixedTermShape;	 // 2012-10-13 by jhahn 기간제 아이템 추가 
}
