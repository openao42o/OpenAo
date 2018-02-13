// // 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 클래스	
// DissolutionItemmanager.cpp: implementation of the DissolutionItemmanager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DissolutionItemmanager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDissolutionItemManager::CDissolutionItemManager()
{
	Initialize();
}

CDissolutionItemManager::~CDissolutionItemManager()
{
	Release();
}

void CDissolutionItemManager::Initialize ( void )
{
	m_mapDissolutionItemData.clear();
}

void CDissolutionItemManager::Release ( void )
{
	m_mapDissolutionItemData.clear();
}

BOOL CDissolutionItemManager::LoadDBDissolutionItemData ( CODBCStatement *i_odbcstmt )
{
	if ( NULL == i_odbcstmt )
	{
		return FALSE;
	}

	Release();
	
	CAtumDBHelper::DBLoadDissolutionItemDataInfo( i_odbcstmt , &m_mapDissolutionItemData, FALSE);
	
	return TRUE;
}

// 용해 아이템 체크 함수
tDissolutionItemInfo CDissolutionItemManager::CheckDissolutionItem(INT i_itemnum)
{	
	m_mapDissolutionItemData.lock();  // 락 잡아주자....
	
	tDissolutionItemInfo tmpDisItem;
	util::zero(&tmpDisItem,sizeof(tDissolutionItemInfo));
	mtmapDissolutionItemInfo::iterator iter = m_mapDissolutionItemData.find(i_itemnum);

	if(iter !=m_mapDissolutionItemData.end()) 
	{ // 데이터가 있을경우
			tmpDisItem.SourceItemnum = iter->first;
			
			for(int icnt = 0 ; icnt < SIZE_MAX_DISSOLUTION_ITEMS ; icnt++)
			{
				tmpDisItem.ResultItemNum[icnt]			=	iter->second.ResultItemNum[icnt];
				tmpDisItem.ResultItemMinCount[icnt]		=	iter->second.ResultItemMinCount[icnt]; 
				tmpDisItem.ResultItemMaxCount[icnt]		=	iter->second.ResultItemMaxCount[icnt]; 
				tmpDisItem.ResultItemProbabillity[icnt]	=	iter->second.ResultItemProbabillity[icnt]; 
			}
			m_mapDissolutionItemData.unlock(); // 언락
			return tmpDisItem;
			
	}
	
	m_mapDissolutionItemData.unlock(); // 언락
	return tmpDisItem;
}

// 아이템 용해 함수 리턴값: TRUE OR FALSE
BOOL CDissolutionItemManager::CreateDissolutionItem(INT i_itemnum, tDissolutionItemInfo i_DissolutionData, DissolutionItems* o_Dissolved_ItemData)
{
	INT DissolutionItemNum = 0;
	INT SuccessPersent = NULL;

	if(!i_itemnum) // 아이템 값이 없으면
	{
		return FALSE;
	}

	for(int icnt = 0 ; icnt < SIZE_MAX_DISSOLUTION_ITEMS ; ++icnt)
	{
		SuccessPersent = RANDI(0,99); // 0~99사이 값을 구한다.
		
		if(SuccessPersent <= i_DissolutionData.ResultItemProbabillity[icnt]) // 100사이의 값이 확률값보다 작거나 같을 경우에만 적용시킨다.
		{
			o_Dissolved_ItemData->Itemnum[icnt] = i_DissolutionData.ResultItemNum[icnt]; // 용해된 아이템 정보(3개)
			// 용해된 3가지 아이템의 카운트
			o_Dissolved_ItemData->Itemcount[icnt] = RANDI(i_DissolutionData.ResultItemMinCount[icnt],i_DissolutionData.ResultItemMaxCount[icnt]);
		}
		else 
		{
			o_Dissolved_ItemData->Itemnum[icnt]  = 0;
			o_Dissolved_ItemData->Itemcount[icnt] = 0;
		}  
		SuccessPersent = NULL; // 100값 초기화
	}
	//..

	return TRUE;
}

