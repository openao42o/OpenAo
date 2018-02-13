// AtumDatabase.cpp: implementation of the CAtumDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "AtumDatabase.h"
#include "AtumApplication.h"
#include "SceneData.h"
#include "FieldWinSocket.h"
#include "INFImage.h"
#include "INFItemInfo.h"

// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "MonsterData.h"
#include "dxutil.h"

#include "CipherRA.h"

#include "StoreData.h" // 2013-05-28 by bhsohn 아머 컬렉션 시스템

#include "INFImageEx.h"	           // 2011. 10. 10 by jskim UI시스템 변경		 
#include "sha256.h"
#define GUILD_POINTER_SIZE		8	// loading시에 struct에서 제외할 사이즈

#define MAX_AUTOSEARCH_COUNT		1000 // 자동 완성 보이는 리스트 수 // 2013-02-28 by bhsohn [인게임 조합식] 자동 완성 처리

// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
#define MAX_SEARCH_ITEM_COUNT		10000		// 최대 조합수
// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2013-03-13 by bhsohn 스킬 번호 구조 수정
#ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
	int SKILL_BASE_NUM(int nItemNum)	{return g_pDatabase->GetSkillBaseNum(nItemNum);}
	int SKILL_LEVEL(int nItemNum)			{return g_pDatabase->GetSkillLevel(nItemNum);}
	int SKILL_KIND(int nItemNum)			{return g_pDatabase->GetSkillKind(nItemNum);}	
#endif
	int SKILL_KIND_INDEX(int nItemNum)			{return g_pDatabase->GetSkillKindIndex(nItemNum);}
	int SKILL_BASE_SOURCEINDEX(int nSourceIndex)	{return g_pDatabase->GetSkillBaseSourceIndex(nSourceIndex);}
// END 2013-03-13 by bhsohn 스킬 번호 구조 수정

// 2013-03-18 by bhsohn 팩토리 조합 추가
// 번호 순으로 정렬
struct ITEM_NUM_SORT : binary_function<ITEM_W_COUNT, ITEM_W_COUNT, bool>
{
	bool operator()(ITEM_W_COUNT Param1, ITEM_W_COUNT Param2)
	{
		return Param1.ItemNum < Param2.ItemNum; 	// 오름 차순 정렬
	}
};
// END 2013-03-18 by bhsohn 팩토리 조합 추가

CAtumDatabase::CAtumDatabase()
{
	FLOG("CAtumDatabase::CAtumDatabase()");
	g_pDatabase = this;

	// 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기
	memset(&m_DefMapInfo, 0, sizeof(MAP_INFO));
	// end 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기

	// 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정
	m_pMapMixingSearchTable = NULL;
	// end 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	m_pMapMapResultMixingInfo = NULL;
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LOOK_MIXING_ITEM;nCnt++)
	{
		m_pMapItemShapeType[nCnt] = NULL;			
	}
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

}

CAtumDatabase::~CAtumDatabase()
{
	FLOG("CAtumDatabase::~CAtumDatabase()");
	g_pDatabase = NULL;

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	ReleaseResultMixingItemSearchTable();
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	ReleaseMixingItemListIndex();
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색	

	// 2005-02-04 by jschoi - 메모리 누수
	CVectorMixingInfoIterator it = m_VecMixingInfo.begin();
	while(it != m_VecMixingInfo.end())
	{
		util::del(*it);
		it++;
	}
	m_VecMixingInfo.clear();
	// 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정
	ReleaseMixingItemSearchTable();
	// end 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정

	CMapGuildMarkIterator itGuildMark = m_mapGuildMark.begin();
	while(itGuildMark != m_mapGuildMark.end())
	{
		util::del(itGuildMark->second->pImage);
		util::del(itGuildMark->second->pData);
		util::del(itGuildMark->second);
		itGuildMark++;
	}
	m_mapGuildMark.clear();

	CMapMapInfoIterator itMapInfo = m_mapMapInfo.begin();
	while(itMapInfo != m_mapMapInfo.end())
	{
		util::del(itMapInfo->second);
		itMapInfo++;
	}
	m_mapMapInfo.clear();

	CVectorMysteryItemDropIterator itMystery = m_VecMysteryItem.begin();
	while(itMystery != m_VecMysteryItem.end())
	{
		util::del(*itMystery);
		itMystery++;
	}
	m_VecMysteryItem.clear();

	// 2009-03-04 by bhsohn 럭키 머신 수정안
	{		
		CVectorLuckyMachineIterator itLucky = m_VecLuckyMachine.begin();
		while(itLucky != m_VecLuckyMachine.end())
		{
			util::del(*itLucky);
			itLucky++;
		}
		m_VecLuckyMachine.clear();

	}
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	CVectorInvokingWearItemDPIt itDP = m_VecInvokingWearItemDPNum.begin();
	while( itDP != m_VecInvokingWearItemDPNum.end() )
	{
		util::del( (*itDP) );
		++itDP;
	}
	m_VecInvokingWearItemDPNum.clear();

	itDP = m_VecInvokingWearItemDPNumByUse.begin();
	while( itDP != m_VecInvokingWearItemDPNumByUse.end() )
	{
		util::del( (*itDP) );
		++itDP;
	}
	m_VecInvokingWearItemDPNumByUse.clear();
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	// 2010-06-15 by shcho&hslee 펫시스템
	DeletePetData();
	// End 2010-06-15 by shcho&hslee 펫시스템

}

BOOL CAtumDatabase::LoadItemData(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);

#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		ITEM * item = new ITEM;
		if (fread(item, sizeof(ITEM), 1, fd) == 0)
		{
			util::del(item);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)item, sizeof(ITEM), pos + i * sizeof(ITEM));
#endif
		if (!m_mapItemInfoTemp[item->ItemNum]) m_mapItemInfoTemp[item->ItemNum] = item;
		else util::del(item);
	}

	return TRUE;
}

BOOL CAtumDatabase::LoadMonsterInfo(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for(int i = 0; i < nCount; i++)
	{
		MEX_MONSTER_INFO * monster = new MEX_MONSTER_INFO;
		if (fread(monster, sizeof(MEX_MONSTER_INFO), 1, fd) == 0)
		{
			util::del(monster);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)monster, sizeof(MEX_MONSTER_INFO), pos + i * sizeof(MEX_MONSTER_INFO));
#endif

		if (!m_mapMonsterInfoTemp[monster->MonsterUnitKind])
			m_mapMonsterInfoTemp[monster->MonsterUnitKind] = monster;
		else
			util::del(monster);
	}
	return TRUE;
}

BOOL CAtumDatabase::LoadMapObjectInfo(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for(int i = 0; i < nCount; i++)
	{
		MAPOBJECTINFO * object = new MAPOBJECTINFO;
		memset(object, 0x00, sizeof(MAPOBJECTINFO) );
		if (fread(object, sizeof(MAPOBJECTINFO), 1, fd) == 0)
		{
			util::del(object);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)object, sizeof(MAPOBJECTINFO), pos + i * sizeof(MAPOBJECTINFO));
#endif

		if (!m_mapObjectInfoTemp[object->Code])
			m_mapObjectInfoTemp[object->Code] = object;
		else
			util::del(object);
	}
	return TRUE;
}

BOOL CAtumDatabase::LoadRareItemData(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		RARE_ITEM_INFO * pRareItem = new RARE_ITEM_INFO;
		if (fread(pRareItem, sizeof(RARE_ITEM_INFO), 1, fd) == 0)
		{
			util::del(pRareItem);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)pRareItem, sizeof(RARE_ITEM_INFO), pos + i * sizeof(RARE_ITEM_INFO));
#endif

		if (!m_mapRareItemInfoTemp[pRareItem->CodeNum])
			m_mapRareItemInfoTemp[pRareItem->CodeNum] = pRareItem;
		else
			util::del(pRareItem);
	}
	return TRUE;
}

BOOL CAtumDatabase::LoadBuildingNPC(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	DBGOUT("---------------NPC INFO-----------------------------\n");
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
//		RARE_ITEM_INFO * pRareItem = new RARE_ITEM_INFO;
//		if( fread(pRareItem,sizeof(RARE_ITEM_INFO),1,fd) == 0)
//			return FALSE;
//		m_mapRareItemInfoTemp[pRareItem->CodeNum] = pRareItem;
		BUILDINGNPC * pInfo = new BUILDINGNPC;
		if (fread(pInfo, sizeof(BUILDINGNPC), 1, fd) == 0)
		{
			util::del(pInfo);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)pInfo, sizeof(BUILDINGNPC), pos + i * sizeof(BUILDINGNPC));
#endif

		if (!m_mapBuildingNPCInfoTemp[pInfo->BuildingIndex])
			m_mapBuildingNPCInfoTemp[pInfo->BuildingIndex] = pInfo;
		else
			util::del(pInfo);
//		DBGOUT("   index : %d, name : %s)\n",pInfo->NPCIndex, pInfo->NPCName);
	}
	DBGOUT("----------------------------------------------------\n");
	return TRUE;
}

BOOL CAtumDatabase::LoadGuildMark(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		GUILD_MARK_VERSION * pGuildMark = new GUILD_MARK_VERSION;
		if (fread(pGuildMark, sizeof(GUILD_MARK_VERSION) - GUILD_POINTER_SIZE, 1, fd) == 0)
		{
			util::del(pGuildMark);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)pGuildMark, sizeof(GUILD_MARK_VERSION), pos + i * sizeof(GUILD_MARK_VERSION));
#endif

		pGuildMark->pData = new char[pGuildMark->nSizeOfImage];
		if (fread(pGuildMark->pData, pGuildMark->nSizeOfImage, 1, fd) == 0)
		{
			util::del(pGuildMark->pData);
			util::del(pGuildMark);
			return FALSE;
		}
		// 2011. 10. 10 by jskim UI시스템 변경		
		DataHeader* pHeader = new DataHeader;
		pHeader->m_pData = pGuildMark->pData;
		pHeader->m_DataSize = pGuildMark->nSizeOfImage;

		pGuildMark->pImage = new CINFImage;
		pGuildMark->pImage->InitDeviceObjects(pHeader->m_pData ,pHeader->m_DataSize);
		pGuildMark->pImage->RestoreDeviceObjects();
	   // end 2011. 10. 10 by jskim UI시스템 변경
		if (!m_mapGuildMark[pGuildMark->nGuildUniqueNumber]) m_mapGuildMark[pGuildMark->nGuildUniqueNumber] = pGuildMark;
		else
		{
			util::del(pGuildMark->pImage);
			util::del(pHeader);
			util::del(pGuildMark->pData);
			util::del(pGuildMark);
		}
	}
	return TRUE;
}

BOOL CAtumDatabase::LoadMapInfo(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		MAP_INFO * pMapInfo = new MAP_INFO;
		
		if (fread(pMapInfo, sizeof(MAP_INFO), 1, fd) == 0)
		{
			util::del(pMapInfo);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)pMapInfo, sizeof(MAP_INFO), pos + i * sizeof(MAP_INFO));
#endif
		if (!m_mapMapInfo[pMapInfo->MapIndex])
			m_mapMapInfo[pMapInfo->MapIndex] = pMapInfo;
		else
			util::del(pMapInfo);
//		DBGOUT("MapInfo[%d]=[%s]\n",pMapInfo->MapIndex, pMapInfo->MapName);
	}
	return TRUE;
}

BOOL CAtumDatabase::LoadMysteryInfo(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		MYSTERY_ITEM_DROP * pMystery = new MYSTERY_ITEM_DROP;
		if(fread(pMystery, sizeof(MYSTERY_ITEM_DROP), 1, fd) == 0)
		{
			util::del(pMystery);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)pMystery, sizeof(MYSTERY_ITEM_DROP), pos + i * sizeof(MYSTERY_ITEM_DROP));
#endif

		m_VecMysteryItem.push_back(pMystery);
		//DBGOUT("pMystery->DropItemNum %3d : DropItemNum(%d) MysteryItemDropNum(%d)(%x)\n",i,pMystery->DropItemNum,pMystery->MysteryItemDropNum,pMystery);
	}
	return TRUE;
}

BOOL CAtumDatabase::LoadMixingInfo(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		ITEM_MIXING_INFO * pMixingInfo = new ITEM_MIXING_INFO;
		if( fread(pMixingInfo,sizeof(ITEM_MIXING_INFO),1,fd) == 0)
		{
			util::del(pMixingInfo);
			return FALSE;
		}
		
#ifdef CIPHER2015
		CipherRA((char*)pMixingInfo, sizeof(ITEM_MIXING_INFO), pos + i * sizeof(ITEM_MIXING_INFO));
#endif

//		m_mapMixingInfo[pMixingInfo->TargetItemNum] = pMixingInfo;
		m_VecMixingInfo.push_back(pMixingInfo);
#ifdef _DEBUG
// 2006-02-10 by ispark, DBGOUT 내용 삭제
//		DBGOUT("MixingInfo[%d]--->",pMixingInfo->TargetItemNum);
//		for(int j=0;j<COUNT_ITEM_MIXING_SOURCE;j++)
//		{
//			if(pMixingInfo->SourceItem[j].ItemNum != 0 )
//			{
//				DBGOUT("[%d:%d],",pMixingInfo->SourceItem[j].ItemNum, pMixingInfo->SourceItem[j].Count);
//			}
//		}
//		DBGOUT("\n");
#endif
	}	
	return TRUE;
}

// 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정
void CAtumDatabase::InitMixingItemSearchTable()
{
	CVectorMixingInfoIterator it = m_VecMixingInfo.begin();
	CMapMixingSearchTableIterator it_map;

	int nItemNum,i;
	ReleaseMixingItemSearchTable();

	m_pMapMixingSearchTable = new CMapMixingSearchTable;
	while(it != m_VecMixingInfo.end())
	{
		ITEM_MIXING_INFO * pMixingInfo = (*it);
		if(!pMixingInfo->Visible)
		{
			it++;
			continue;
		}
		for(i = 0 ; i < COUNT_ITEM_MIXING_SOURCE ; i++)
		{
			nItemNum = pMixingInfo->SourceItem[i].ItemNum;
			if(nItemNum == 0)
			{
				continue;
			}
			it_map = m_pMapMixingSearchTable->find(nItemNum);
			if(it_map == m_pMapMixingSearchTable->end())
			{
				CVectorMixingInfo* pTemp = new CVectorMixingInfo;
				m_pMapMixingSearchTable->insert(pair<INT,CVectorMixingInfo*>(nItemNum,pTemp));
				it_map = m_pMapMixingSearchTable->find(nItemNum);
			}

			it_map->second->push_back((*it));
		}
		it++;
	}

	int nSize = m_pMapMixingSearchTable->size();
}

void CAtumDatabase::ReleaseMixingItemSearchTable()
{
	if(!m_pMapMixingSearchTable)
		return;

	CMapMixingSearchTableIterator it_map = m_pMapMixingSearchTable->begin();

	while(it_map != m_pMapMixingSearchTable->end())
	{
		util::del(it_map->second);
		it_map++;
	}
	m_pMapMixingSearchTable->clear();
	util::del(m_pMapMixingSearchTable);
}

CVectorMixingInfo* CAtumDatabase::GetMixingItemList(int nItemNum)
{
	CMapMixingSearchTableIterator it;
	it = m_pMapMixingSearchTable->find(nItemNum);

	if(it == m_pMapMixingSearchTable->end())
	{
		return NULL;
	}

	CVectorMixingInfo* pVecMixingInfo = (it->second);
	return pVecMixingInfo;

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		검색된 아이템 결과 index를 Vector로 가져오자
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-02-26 ~ 2013-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetMaterialMixingItemList(int nItemCnt, char* pSearchName, vector<structResultMixingInfo> *o_pVecItemList)
{
	o_pVecItemList->clear();	
	if(!m_pMapMixingSearchTable)
	{
		return nItemCnt;
	}	

	vector<string> vecSeachName;
	SearchStringCull(pSearchName, &vecSeachName);
	if(0 == vecSeachName.size())
	{
		return nItemCnt;
	}
	
	CMapMixingSearchTableIterator it_map = m_pMapMixingSearchTable->begin();
	while(it_map != m_pMapMixingSearchTable->end())
	{
		INT nItemNum = it_map->first;
		CVectorMixingInfo* pVectorInfo = it_map->second;

		ITEM* pItem = GetItemInfoLoadItemData(nItemNum);
		if(!pItem)
		{
			it_map++;
			continue;
		}

		BOOL bFindStr = TRUE;
		vector<string>::iterator itStr = vecSeachName.begin();
		while(itStr != vecSeachName.end())
		{
			const char* pStrIt = (*itStr).c_str();
			if(NULL == _tcsstr(pItem->ItemName, pStrIt))		
			{
				bFindStr = FALSE;
			}
			itStr++;
		}
		
		// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색		
//		if(NULL != _tcsstr(pItem->ItemName, pSearchName))
		if(bFindStr)		
		{
			CVectorMixingInfo* pMMixInfo = GetMixingItemList(nItemNum);			
			if(pMMixInfo)
			{
				CVectorMixingInfoIterator it = pMMixInfo->begin();
				while(it != pMMixInfo->end())
				{
					if(nItemCnt >= MAX_SEARCH_ITEM_COUNT)
					{
						return nItemCnt;
					}

					ITEM_MIXING_INFO* pMixInfo = (*it);
					// 2013-05-06 by bhsohn 인게임 조합식 Show처리
					if(!pMixInfo->Visible)
					{
						it++;
						continue;
					}
					// END 2013-05-06 by bhsohn 인게임 조합식 Show처리

					structResultMixingInfo	struMixInfo;
					memset(&struMixInfo, 0x00, sizeof(structResultMixingInfo));
					{
						// 아이템 이름
						strncpy(struMixInfo.itemname, pItem->ItemName, SIZE_MAX_ITEM_NAME);
						memcpy(&struMixInfo.struMixingInfo, pMixInfo, sizeof(ITEM_MIXING_INFO));						
					}

					// 스트링을 찾았다.
					o_pVecItemList->push_back(struMixInfo);
					
					nItemCnt++;
					it++;
				}				
			}						
		}	
		
		it_map++;
	}		
	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	ITEM* pRtnItemInfo = NULL;
	CMapItemInfoIterator itInfoItemTemp = m_mapItemInfoTemp.begin();
	while(itInfoItemTemp != m_mapItemInfoTemp.end() )
	{
		if(nItemCnt >= MAX_SEARCH_ITEM_COUNT)
		{
			break;
		}
		ITEM* pItemMap = itInfoItemTemp->second;
		if(!pItemMap)
		{
			itInfoItemTemp++;
			continue;
		}
		BOOL bFindStr = TRUE;
		vector<string>::iterator itStr = vecSeachName.begin();
		while(itStr != vecSeachName.end())
		{
			const char* pStrIt = (*itStr).c_str();
			if(NULL == _tcsstr(pItemMap->ItemName, pStrIt))		
			{
				bFindStr = FALSE;
			}
			itStr++;
		}

		if(bFindStr)
		{
			nItemCnt = AddMaterialShapeItemList(nItemCnt, pItemMap, o_pVecItemList); // 외형 변경 아이템 추가			
		}
		itInfoItemTemp++;
	}
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	return nItemCnt;
}

int CAtumDatabase::GetMaterialMixingItemListIndex(INT nItemCnt, INT nSearchItemNum, vector<structResultMixingInfo> *o_pVecItemList)
{
	o_pVecItemList->clear();	
	if(!m_pMapMixingSearchTable)
	{
		return nItemCnt;
	}

	CMapMixingSearchTableIterator it_map = m_pMapMixingSearchTable->begin();
	while(it_map != m_pMapMixingSearchTable->end())
	{
		INT nItemNum = it_map->first;
		CVectorMixingInfo* pVectorInfo = it_map->second;
		
		ITEM* pItem = GetItemInfoLoadItemData(nItemNum);
		if(!pItem)
		{
			it_map++;
			continue;
		}
		
		if(nSearchItemNum == pItem->ItemNum)
		{
			CVectorMixingInfo* pMMixInfo = GetMixingItemList(nSearchItemNum);			
			if(pMMixInfo)
			{
				CVectorMixingInfoIterator it = pMMixInfo->begin();
				while(it != pMMixInfo->end())
				{
					if(nItemCnt >= MAX_SEARCH_ITEM_COUNT)
					{
						return nItemCnt;
					}
					
					ITEM_MIXING_INFO* pMixInfo = (*it);
					// 2013-05-06 by bhsohn 인게임 조합식 Show처리
					if(!pMixInfo->Visible)
					{
						it++;
						continue;
					}
					// END 2013-05-06 by bhsohn 인게임 조합식 Show처리
					structResultMixingInfo	struMixInfo;
					memset(&struMixInfo, 0x00, sizeof(structResultMixingInfo));
					{
						// 아이템 이름
						strncpy(struMixInfo.itemname, pItem->ItemName, SIZE_MAX_ITEM_NAME);
						memcpy(&struMixInfo.struMixingInfo, pMixInfo, sizeof(ITEM_MIXING_INFO));						
					}
					
					// 스트링을 찾았다.
					o_pVecItemList->push_back(struMixInfo);
					
					nItemCnt++;
					it++;
				}				
			}			
			
		}	
		
		it_map++;
	}	
	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	ITEM* pItem = GetItemInfoLoadItemData(nSearchItemNum);
	if(pItem)
	{
		nItemCnt = AddMaterialShapeItemList(nItemCnt, pItem, o_pVecItemList); // 외형 변경 아이템 추가			
	}
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	return nItemCnt;
}

// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
int  CAtumDatabase::AddMaterialShapeItemList(int nItemCnt, ITEM* pItem, vector<structResultMixingInfo> *o_pVecItemList)
{
	int nRtnItemCnt = nItemCnt;
	int nShapeIdx = GetFindLookChangeIdx(pItem);
	if(LOOK_MIXING_NONE == nShapeIdx)
	{
		return nRtnItemCnt;
	}
	// 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리
	if(pItem->IsExistDesParam(DES_INGAME_SHOW))
	{
		return nRtnItemCnt;
	}
	// END 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리

	vector<INT> vecShape;
	Find_ItemType_To_FindIdx(nShapeIdx, &vecShape);
	if(0 == vecShape.size())
	{
		return nRtnItemCnt;
	}
	
	vector<INT>::iterator it = vecShape.begin();
	while(it != vecShape.end())
	{
		if(nRtnItemCnt >= MAX_SEARCH_ITEM_COUNT)
		{
			return nRtnItemCnt;
		}
		INT nShapeTo = (*it); // 목적 인덱스
		if(nShapeTo <0 || nShapeTo >= MAX_LOOK_MIXING_ITEM)
		{
			break;
		}
		CMapItemInfoList* pMapItemShapeType =  m_pMapItemShapeType[nShapeTo];			
		CMapItemInfoIterator itItem = pMapItemShapeType->begin();
		while(itItem != pMapItemShapeType->end())
		{
			// 아이템 재료로 검색
			ITEM* pItemMap = itItem->second;

			// 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리
			if(pItemMap->IsExistDesParam(DES_INGAME_SHOW))
			{
				itItem++;
				continue;
			}
			// END 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리

			structResultMixingInfo	struMixInfo;
			memset(&struMixInfo, 0x00, sizeof(structResultMixingInfo));
			{
				// 아이템 이름
				strncpy(struMixInfo.itemname, pItemMap->ItemName, SIZE_MAX_ITEM_NAME);
				
				switch(nShapeTo)
				{
				case LOOK_MIXING_SHAPE_ITEM:				// 외형 카드 - 
				case LOOK_MIXING_SHAPE_EFFECT:				// 이펙트 카드- 	
				case LOOK_MIXING_SHAPE_PRIMARY_WEAPON_A:	// 무기 외형 - 기본 무기 A 기어
				case LOOK_MIXING_SHAPE_PRIMARY_WEAPON_BMI:	// 무기 외형 - 기본 무기 BMI 기어
				case LOOK_MIXING_SHAPE_SECOND_WEAPON_A:		// 무기 외형 - 고급 무기 A 기어
				case LOOK_MIXING_SHAPE_SECOND_WEAPON_BMI:	// 무기 외형 - 고급 무기 BMI 기어
				case LOOK_MIXING_SHAPE_EFFECT_PRIMARY_WEAPON:	// 무기 외형 - 기본 무기 이펙트
				case LOOK_MIXING_SHAPE_EFFECT_SECOND_WEAPON:	// 무기 외형 - 고급 무기 이펙트
				case LOOK_MIXING_SHAPE_DEFENSE_B:			// 아머 외형 - B 기어
				case LOOK_MIXING_SHAPE_DEFENSE_M:			// 아머 외형 - M 기어
				case LOOK_MIXING_SHAPE_DEFENSE_A:			// 아머 외형 - A 기어
				case LOOK_MIXING_SHAPE_DEFENSE_I:			// 아머 외형 - I 기어
				case LOOK_MIXING_SHAPE_DEFENSE_INIT:		// 아머 외형 - 초기화
				case LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT:	// 이펙트 외형 - 고급 무기 초기화 
				case LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT:	// 이펙트 외형 - 기본 무기 초기화 
				case LOOK_MIXING_SHAPE_PET_ITEM:			// 파트너 외형 - 파트너 기간제 외형 변경
					{
						struMixInfo.struMixingInfo.TargetItemNum = pItem->ItemNum;
						struMixInfo.LinkItem = pItemMap->LinkItem;

// 						if(pItem->ShapeItemNum > 0)
// 						{
// 							struMixInfo.struMixingInfo.TargetItemNum = pItem->ShapeItemNum;
// 						}
					}
					break;
				default:
					{
						struMixInfo.struMixingInfo.TargetItemNum = pItemMap->ItemNum;
						struMixInfo.LinkItem = pItem->LinkItem;
// 						if(pItemMap->ShapeItemNum > 0)
// 						{
// 							struMixInfo.struMixingInfo.TargetItemNum = pItemMap->ShapeItemNum;
// 						}
					}
					break;
				}	
				struMixInfo.struMixingInfo.MixingProbability = 10000; // 100%
				struMixInfo.struMixingInfo.MixingCost = 0; // // 재료 조합을 위한 수수료
				struMixInfo.struMixingInfo.SourceItem[COUNT_ITEM_MIXING_SOURCE-1].ItemNum = pItem->ItemNum;
				struMixInfo.struMixingInfo.SourceItem[COUNT_ITEM_MIXING_SOURCE-1].Count = 1;
				struMixInfo.struMixingInfo.SourceItem[COUNT_ITEM_MIXING_SOURCE-2].ItemNum = pItemMap->ItemNum;
				struMixInfo.struMixingInfo.SourceItem[COUNT_ITEM_MIXING_SOURCE-2].Count = 1;
				struMixInfo.struMixingInfo.NumOfSourceItems = 2;				
			}

			nRtnItemCnt++;			
			itItem++;
		}
		it++;
	}
	return nRtnItemCnt;	
}

// end 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// [결과-이름] 검색을 하기 위한 Map생성
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-02-26 ~ 2013-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::InitResultMixingItemSearchTable()
{
	CVectorMixingInfoIterator it = m_VecMixingInfo.begin();
	CMapResultMixingInfoIterator it_map;
	
	ReleaseResultMixingItemSearchTable();
	
	if(NULL == m_pMapMapResultMixingInfo)
	{
	m_pMapMapResultMixingInfo = new CMapResultMixingInfo;
	}
	while(it != m_VecMixingInfo.end())
	{
		ITEM_MIXING_INFO * pMixingInfo = (*it);
		if(!pMixingInfo)
		{
			it++;
			continue;
		}
		if(!pMixingInfo->Visible)
		{
			it++;
			continue;
		}
		ITEM* pItem = GetItemInfoLoadItemData(pMixingInfo->TargetItemNum); // 결과 Item정보
		if(!pItem)
		{
			it++;
			continue;
		}
		it_map = m_pMapMapResultMixingInfo->find(pItem->ItemName);
		// 2013-06-14 by bhsohn 인게임 조합식 수정
// 		if(it_map != m_pMapMapResultMixingInfo->end())
// 		{
// 			// 이미 있는 정보를 새로 넣지 말자.						
// 			it++;
// 			continue;
// 		}
// 
// 		structResultMixingInfo *pResultMixingInfo = new structResultMixingInfo;
// 		memset(pResultMixingInfo , 0x00, sizeof(structResultMixingInfo));
// 
// 		strncpy(pResultMixingInfo->itemname, pItem->ItemName, SIZE_MAX_ITEM_NAME);
// 		memcpy(&pResultMixingInfo->struMixingInfo, pMixingInfo, sizeof(ITEM_MIXING_INFO));
// 
// 		m_pMapMapResultMixingInfo->insert(pair<std::string, structResultMixingInfo*>(pResultMixingInfo->itemname, pResultMixingInfo));
		if(it_map == m_pMapMapResultMixingInfo->end())
		{			
			CVectorRInfo vecRMixInfo;
			m_pMapMapResultMixingInfo->insert(pair<std::string, CVectorRInfo>(pItem->ItemName, vecRMixInfo));
			it_map = m_pMapMapResultMixingInfo->find(pItem->ItemName);
		}
		structResultMixingInfo *pResultMixingInfo = new structResultMixingInfo;
		{			
			memset(pResultMixingInfo , 0x00, sizeof(structResultMixingInfo));
			
			strncpy(pResultMixingInfo->itemname, pItem->ItemName, SIZE_MAX_ITEM_NAME);
			memcpy(&pResultMixingInfo->struMixingInfo, pMixingInfo, sizeof(ITEM_MIXING_INFO));		
		}
		it_map->second.push_back(pResultMixingInfo);
		// END END 2013-06-14 by bhsohn 인게임 조합식 수정

		it++;
	}
	
	int nSize = m_pMapMapResultMixingInfo->size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		결과 테이블 삭제
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-02-26 ~ 2013-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::ReleaseResultMixingItemSearchTable()
{
	if(!m_pMapMapResultMixingInfo)
		return;
	
	CMapResultMixingInfoIterator it_map = m_pMapMapResultMixingInfo->begin();
	
	while(it_map != m_pMapMapResultMixingInfo->end())
	{
		// 2013-06-14 by bhsohn 인게임 조합식 수정
//		util::del(it_map->second);
		CVectorRInfo vecRInfo = it_map->second;
		CVectorRInfoIterator itRInfo  = vecRInfo.begin();
		while(itRInfo  != vecRInfo.end())
		{
			structResultMixingInfo* pRInfo = (*itRInfo);
			util::del(pRInfo);
			itRInfo ++;
		}		
		vecRInfo.clear();
		// END 2013-06-14 by bhsohn 인게임 조합식 수정

		it_map++;
	}
	m_pMapMapResultMixingInfo->clear();
	util::del(m_pMapMapResultMixingInfo);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		검색된 아이템 결과 index를 Vector로 가져오자
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-02-26 ~ 2013-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetResultMixingItemList(int nItemCnt, char* pItemName, vector<structResultMixingInfo> *o_pVecItemList)
{	
	o_pVecItemList->clear();	
	if(!m_pMapMapResultMixingInfo)
	{
		return nItemCnt;
	}	

	vector<string> vecSeachName;
	SearchStringCull(pItemName, &vecSeachName);
	if(0 == vecSeachName.size())
	{
		return nItemCnt;
	}
	
	CMapResultMixingInfoIterator it_map = m_pMapMapResultMixingInfo->begin();	
	while(it_map != m_pMapMapResultMixingInfo->end())
	{
		if(nItemCnt >= MAX_SEARCH_ITEM_COUNT)
		{
			break;
		}
		std::string strKey = it_map->first;

		// 2013-06-14 by bhsohn 인게임 조합식 수정
//		structResultMixingInfo* pInfo = it_map->second;
		CVectorRInfo vecRInfo = it_map->second;
		CVectorRInfoIterator itRInfo  = vecRInfo.begin();
		while(itRInfo  != vecRInfo.end())
		{
			structResultMixingInfo* pInfo = (*itRInfo);
			// 2013-05-06 by bhsohn 인게임 조합식 Show처리
			if(!pInfo->struMixingInfo.Visible)
			{
				it_map++;
				continue;
			}
			// END 2013-05-06 by bhsohn 인게임 조합식 Show처리
			
			BOOL bFindStr = TRUE;
			vector<string>::iterator itStr = vecSeachName.begin();
			while(itStr != vecSeachName.end())
			{
				const char* pStrIt = (*itStr).c_str();
				if(NULL == _tcsstr(strKey.c_str(), pStrIt))		
				{
					bFindStr = FALSE;
				}
				itStr++;
			}
			
			if(bFindStr)
			{
				// 스트링을 찾았다.
				o_pVecItemList->push_back(*pInfo);
				nItemCnt++;		
			}

			itRInfo++;
		}
		
		
		it_map++;
	}	
	
	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색	
	CMapItemInfoIterator itInfoItemTemp = m_mapItemInfoTemp.begin();
	while(itInfoItemTemp != m_mapItemInfoTemp.end() )
	{
		if(nItemCnt >= MAX_SEARCH_ITEM_COUNT)
		{
			break;
		}
		ITEM* pItemMap = itInfoItemTemp->second;
		if(!pItemMap)
		{
			itInfoItemTemp++;
			continue;
		}
		BOOL bFindStr = TRUE;
		vector<string>::iterator itStr = vecSeachName.begin();
		while(itStr != vecSeachName.end())
		{
			const char* pStrIt = (*itStr).c_str();
			if(NULL == _tcsstr(pItemMap->ItemName, pStrIt))		
			{
				bFindStr = FALSE;
			}
			itStr++;
		}		
		if(bFindStr)
		{
			nItemCnt = AddResultShapeItemList(nItemCnt, pItemMap, o_pVecItemList);
		}
		itInfoItemTemp++;
	}
	
	return nItemCnt;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		검색된 아이템 결과 index를 Vector로 가져오자
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-02-26 ~ 2013-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetResultMixingItemListIndex(int nItemCnt, INT nItemNum, vector<structResultMixingInfo> *o_pVecItemList)
{
	o_pVecItemList->clear();	
	if(!m_pMapMapResultMixingInfo)
	{
		return nItemCnt;
	}
	
	CMapResultMixingInfoIterator it_map = m_pMapMapResultMixingInfo->begin();	
	while(it_map != m_pMapMapResultMixingInfo->end())
	{		
		if(nItemCnt >= MAX_SEARCH_ITEM_COUNT)
		{
			break;
		}
		
		// 2013-06-14 by bhsohn 인게임 조합식 수정
//		structResultMixingInfo* pInfo = it_map->second;
		CVectorRInfo vecRInfo = it_map->second;
		CVectorRInfoIterator itRInfo  = vecRInfo.begin();
		while(itRInfo  != vecRInfo.end())
		{
			structResultMixingInfo* pInfo = (*itRInfo);
			// 2013-05-06 by bhsohn 인게임 조합식 Show처리
			if(!pInfo->struMixingInfo.Visible)
			{
				it_map++;
				continue;
			}
			// END 2013-05-06 by bhsohn 인게임 조합식 Show처리
			if(nItemNum == pInfo->struMixingInfo.TargetItemNum)		
			{
				// 스트링을 찾았다.
				o_pVecItemList->push_back(*pInfo);
				nItemCnt++;		
			}			
			itRInfo ++;
		}
		
		it_map++;
	}	
	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	ITEM* pItem = GetItemInfoLoadItemData(nItemNum);
	if(pItem)
	{
		nItemCnt = AddResultShapeItemList(nItemCnt, pItem, o_pVecItemList);
	}
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	return nItemCnt;
}
// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
int CAtumDatabase::AddResultShapeItemList(int nItemCnt, ITEM* pItem, vector<structResultMixingInfo> *o_pVecItemList)
{	
	int nRtnItemCnt = nItemCnt;
	int nShapeIdx = GetFindLookChangeIdx(pItem);
	if(LOOK_MIXING_NONE == nShapeIdx)
	{
		return nRtnItemCnt;
	}

	// 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리
	if(pItem->IsExistDesParam(DES_INGAME_SHOW))
	{
		return nRtnItemCnt;
	}
	// END 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리

	vector<INT> vecShape;
	Find_ItemType_To_FindIdx(nShapeIdx, &vecShape);
	if(0 == vecShape.size())
	{
		return nRtnItemCnt;
	}
	
	vector<INT>::iterator it = vecShape.begin();
	while(it != vecShape.end())
	{
		INT nShapeTo = (*it); // 목적 인덱스
		if(nShapeTo <0 || nShapeTo >= MAX_LOOK_MIXING_ITEM)
		{
			break;
		}
		CMapItemInfoList* pMapItemShapeType =  m_pMapItemShapeType[nShapeTo];			
		CMapItemInfoIterator itItem = pMapItemShapeType->begin();
		while(itItem != pMapItemShapeType->end())
		{
			if(nRtnItemCnt >= MAX_SEARCH_ITEM_COUNT)
			{
				return nRtnItemCnt;
			}
			ITEM* pItemMap = itItem->second;

			// 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리
			if(pItemMap->IsExistDesParam(DES_INGAME_SHOW))
			{
				itItem++;
				continue;
			}
			// END 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리

			// 결과로 검색 정보 
			structResultMixingInfo	struMixInfo;
			memset(&struMixInfo, 0x00, sizeof(structResultMixingInfo));

			{
				// 아이템 이름
				strncpy(struMixInfo.itemname, pItemMap->ItemName, SIZE_MAX_ITEM_NAME);

				switch(nShapeTo)
				{
				case LOOK_MIXING_SHAPE_ITEM:				// 외형 카드 - 
				case LOOK_MIXING_SHAPE_EFFECT:				// 이펙트 카드- 	
				case LOOK_MIXING_SHAPE_PRIMARY_WEAPON_A:	// 무기 외형 - 기본 무기 A 기어
				case LOOK_MIXING_SHAPE_PRIMARY_WEAPON_BMI:	// 무기 외형 - 기본 무기 BMI 기어
				case LOOK_MIXING_SHAPE_SECOND_WEAPON_A:		// 무기 외형 - 고급 무기 A 기어
				case LOOK_MIXING_SHAPE_SECOND_WEAPON_BMI:	// 무기 외형 - 고급 무기 BMI 기어
				case LOOK_MIXING_SHAPE_EFFECT_PRIMARY_WEAPON:	// 무기 외형 - 기본 무기 이펙트
				case LOOK_MIXING_SHAPE_EFFECT_SECOND_WEAPON:	// 무기 외형 - 고급 무기 이펙트
				case LOOK_MIXING_SHAPE_DEFENSE_B:			// 아머 외형 - B 기어
				case LOOK_MIXING_SHAPE_DEFENSE_M:			// 아머 외형 - M 기어
				case LOOK_MIXING_SHAPE_DEFENSE_A:			// 아머 외형 - A 기어
				case LOOK_MIXING_SHAPE_DEFENSE_I:			// 아머 외형 - I 기어
				case LOOK_MIXING_SHAPE_DEFENSE_INIT:		// 아머 외형 - 초기화
				case LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT:	// 이펙트 외형 - 고급 무기 초기화 
				case LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT:	// 이펙트 외형 - 기본 무기 초기화 
				case LOOK_MIXING_SHAPE_PET_ITEM:			// 파트너 외형 - 파트너 기간제 외형 변경
					{
						struMixInfo.struMixingInfo.TargetItemNum = pItem->ItemNum;						
						struMixInfo.LinkItem = pItemMap->LinkItem;
					}
					break;
				default:
					{
						struMixInfo.struMixingInfo.TargetItemNum = pItemMap->ItemNum;
						struMixInfo.LinkItem = pItem->LinkItem;
					}
					break;
				}	
				struMixInfo.struMixingInfo.MixingProbability = 10000; // 100%
				struMixInfo.struMixingInfo.MixingCost = 0; // // 재료 조합을 위한 수수료
				struMixInfo.struMixingInfo.SourceItem[COUNT_ITEM_MIXING_SOURCE-1].ItemNum = pItem->ItemNum;
				struMixInfo.struMixingInfo.SourceItem[COUNT_ITEM_MIXING_SOURCE-1].Count = 1;
				struMixInfo.struMixingInfo.SourceItem[COUNT_ITEM_MIXING_SOURCE-2].ItemNum = pItemMap->ItemNum;
				struMixInfo.struMixingInfo.SourceItem[COUNT_ITEM_MIXING_SOURCE-2].Count = 1;
				struMixInfo.struMixingInfo.NumOfSourceItems = 2;				
			}			
			o_pVecItemList->push_back(struMixInfo);
			nRtnItemCnt++;
			
			itItem++;
			
		}
		it++;
	}

	return nRtnItemCnt;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		결과 검색에 대한 값을 얻어오자
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-02-26 ~ 2013-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//ITEM_MIXING_INFO* CAtumDatabase::GetResultMixingItemSearchTable(char* pItemName)
// {
// 	if(!m_pMapMapResultMixingInfo)
// 	{
// 		return NULL;
// 	}
// 	CMapResultMixingInfoIterator it_map = m_pMapMapResultMixingInfo->find(pItemName);
// 	if(it_map == m_pMapMapResultMixingInfo->end())
// 	{
// 		return NULL;
// 	}
// 	// 2013-06-14 by bhsohn 인게임 조합식 수정
// // 	structResultMixingInfo* pResultMixingInfo = it_map->second;
// // 	return &pResultMixingInfo->struMixingInfo;
// 	return NULL;
// 	// END 2013-06-14 by bhsohn 인게임 조합식 수정
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		결과 테이블 삭제
/// \author		// 2013-02-26 by bhsohn 인게임 조합 검색 처리
/// \date		2013-02-26 ~ 2013-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM* CAtumDatabase::GetItemInfoLoadItemData(INT nItemNum)
{	
	ITEM* pRtnItemInfo = NULL;
	CMapItemInfoIterator itInfoItemTemp = m_mapItemInfoTemp.find(nItemNum);
	if(m_mapItemInfoTemp.end() == itInfoItemTemp)
	{
		return NULL;
	}
	pRtnItemInfo = (itInfoItemTemp->second);
	return pRtnItemInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		자동완성에 해당되는 아이템 이름을 알려준다.
/// \author		// 2013-02-28 by bhsohn [인게임 조합식] 자동 완성 처리
/// \date		2013-02-26 ~ 2013-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::GetSearchItemList(char* pSearchItem, std::map<INT, std::string> *o_pVecSearchList)
{
	o_pVecSearchList->clear();
	if(strlen(pSearchItem) <= 0)
	{
		return;
	}
	
// 	CMapItemInfoIterator itInfoItemTemp = m_mapItemInfoTemp.begin();
// 	while(itInfoItemTemp != m_mapItemInfoTemp.end())
// 	{		
// // 		if(nCnt >= MAX_AUTOSEARCH_COUNT)
// // 		{
// // 			break;
// // 		}
// 		ITEM* pInfo = itInfoItemTemp->second;
// 		if(NULL != _tcsstr(pInfo->ItemName, pSearchItem))		
// 		{
// 			// 스트링을 찾았다.
// 				o_pVecSearchList->push_back(pInfo->ItemName);
// 			nCnt++;
// 		}	
// 		itInfoItemTemp++;
// 	}

	//재료 검색
	int nCnt = 0;	
	CMapMixingSearchTableIterator it_Smap = m_pMapMixingSearchTable->begin();
	while(it_Smap != m_pMapMixingSearchTable->end())
	{
 		if(nCnt >= MAX_AUTOSEARCH_COUNT)
 		{
 			break;
 		}
		INT nItemNum = it_Smap->first;
		CVectorMixingInfo* pVectorInfo = it_Smap->second;
		
		ITEM* pItem = GetItemInfoLoadItemData(nItemNum);
		if(!pItem)
		{
			it_Smap++;
			continue;
		}
		
		if(NULL != _tcsstr(pItem->ItemName, pSearchItem))
		{
			// 스트링을 찾았다.
			std::map<INT, std::string>::iterator it_map = o_pVecSearchList->find(nItemNum);
			
			if(it_map == o_pVecSearchList->end())
			{
				o_pVecSearchList->insert(pair<INT, std::string>(nItemNum, pItem->ItemName));
			}
		}	
		
		it_Smap++;
		nCnt++;
	}	

	// 결과 검색
	CMapResultMixingInfoIterator it_map = m_pMapMapResultMixingInfo->begin();	
	while(it_map != m_pMapMapResultMixingInfo->end())
	{
		if(nCnt >= MAX_AUTOSEARCH_COUNT)
		{
			break;
 		}

		std::string strKey = it_map->first;

		// 2013-06-14 by bhsohn 인게임 조합식 수정
//		structResultMixingInfo* pInfo = it_map->second;
		CVectorRInfo vecRInfo = it_map->second;
		CVectorRInfoIterator itRInfo  = vecRInfo.begin();
		while(itRInfo  != vecRInfo.end())
		{
			structResultMixingInfo* pInfo = (*itRInfo);
			if(NULL != _tcsstr(strKey.c_str(), pSearchItem))		
			{
				// 스트링을 찾았다.
				INT nItemNum = pInfo->struMixingInfo.TargetItemNum;
				std::map<INT, std::string>::iterator it_map = o_pVecSearchList->find(nItemNum);
				
				if(it_map == o_pVecSearchList->end())
				{
					o_pVecSearchList->insert(pair<INT, std::string>(nItemNum, strKey.c_str()));
				}
			}	
			itRInfo ++;
		}
		
		
		
		it_map++;
		nCnt++;
	}	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인게임 조합창 외형 변경 인첸트 시스템 검색
/// \author		// 2013-04-08 by bhsohn 인게임 조합창 추가 구현 
/// \date		2013-04-11 ~ 2013-04-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::GetSearchItemList_LookChangeInfo(char* pSearchItem, std::map<INT, std::string> *o_pVecSearchList)
{
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
/// \date		2013-04-11 ~ 2013-04-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::InitMixingItemListIndex()
{
	ReleaseMixingItemListIndex();
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LOOK_MIXING_ITEM;nCnt++)
	{
		if(NULL == m_pMapItemShapeType[nCnt])
		{
			m_pMapItemShapeType[nCnt] = new CMapItemInfoList;
		}
	}

	CMapItemInfoIterator itInfoItemTemp = m_mapItemInfoTemp.begin();
	while(itInfoItemTemp != m_mapItemInfoTemp.end())
	{		
		ITEM* pInfo = itInfoItemTemp->second;
		int nItemShapeType = GetFindLookChangeIdx(pInfo); // 아이템의 종류를 미리 검색
		if(LOOK_MIXING_NONE != nItemShapeType)
		{
			AddMixingItemListIndex(pInfo, nItemShapeType);
		}		
		itInfoItemTemp++;
 	}
}

void CAtumDatabase::AddMixingItemListIndex(ITEM* pAddItem, int nItemShapeType)
{	
	if(!pAddItem)
	{
		return;
	}
	INT nItemNum = pAddItem->ItemNum;
	
	if(LOOK_MIXING_NONE == nItemShapeType)
	{
		//넣을 필요가 없다
		return;
	}
	if(nItemShapeType < 0 || MAX_LOOK_MIXING_ITEM <= nItemShapeType)
	{
		return;
	}

	// 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리
	if(pAddItem->IsExistDesParam(DES_INGAME_SHOW))
	{
		return;
	}
	// END 2013-07-02 by bhsohn 인게임 조합식 Show추가 처리

	CMapItemInfoList* pMapItemTypeSearchTable = m_pMapItemShapeType[nItemShapeType];
	CMapItemInfoIterator it = pMapItemTypeSearchTable->find(nItemNum);
	if(it == pMapItemTypeSearchTable->end())
	{
		ITEM* pItem = new ITEM;
// 		pItem->ItemNum	= pAddItem->ItemNum;
// 		pItem->Kind		= pAddItem->Kind;
// 		strncpy(pItem->ItemName, pAddItem->ItemName, SIZE_MAX_ITEM_NAME);
// 		pItem->ReqUnitKind =pAddItem->ReqUnitKind;
// 		pItem->SourceIndex =pAddItem->SourceIndex;
		memcpy(pItem, pAddItem, sizeof(ITEM));

		pMapItemTypeSearchTable->insert(pair<INT, ITEM*>(nItemNum, pItem));
	}
	
}

int CAtumDatabase::GetFindLookChangeIdx(ITEM* pItem)
{
	int nItemShapeType = LOOK_MIXING_NONE;
	if(!pItem)
	{
		return nItemShapeType;
	}
	if( IS_PRIMARY_WEAPON(pItem->Kind))		// 무기 외형 - 기본 무기 A 기어		
	{
		if(IS_AGEAR(pItem->ReqUnitKind))
		{
			// 무기 외형 - 기본 무기 A 기어
			nItemShapeType = LOOK_MIXING_PRIMARY_WEAPON_A;		
		}
		else if(IS_BGEAR(pItem->ReqUnitKind) || IS_MGEAR(pItem->ReqUnitKind) || IS_IGEAR(pItem->ReqUnitKind))
		{
			// 무기 외형 - 기본 무기 BMI 기어
			nItemShapeType = LOOK_MIXING_PRIMARY_WEAPON_BMI;
		}
	}		
	else if( IS_SECONDARY_WEAPON(pItem->Kind))		
	{
		if(IS_AGEAR(pItem->ReqUnitKind))
		{
			// 무기 외형 - 고급 무기 A 기어		
			nItemShapeType = LOOK_MIXING_SECOND_WEAPON_A;		
		}
		else if(IS_BGEAR(pItem->ReqUnitKind) || IS_MGEAR(pItem->ReqUnitKind) || IS_IGEAR(pItem->ReqUnitKind))
		{
			// 무기 외형 - 고급 무기 BMI 기어
			nItemShapeType = LOOK_MIXING_SECOND_WEAPON_BMI;		
		}
		else
		{
			// 무기 외형 - 고급 무기 이펙트		
			nItemShapeType = LOOK_MIXING_EFFECT_SECOND_WEAPON;		
		}
	}	
	else if(ITEMKIND_DEFENSE == pItem->Kind)
	{
		if(IS_BGEAR(pItem->ReqUnitKind))
		{
			// 아머 외형 - B 기어
			nItemShapeType = LOOK_MIXING_DEFENSE_B;		
		}
		else if(IS_MGEAR(pItem->ReqUnitKind))
		{
			// 아머 외형 - M 기어
			nItemShapeType = LOOK_MIXING_DEFENSE_M;		
		}
		else if(IS_AGEAR(pItem->ReqUnitKind))
		{
			// 아머 외형 - A 기어
			nItemShapeType = LOOK_MIXING_DEFENSE_A;		
		}
		else if(IS_IGEAR(pItem->ReqUnitKind))
		{
			// 아머 외형 - I 기어
			nItemShapeType = LOOK_MIXING_DEFENSE_I;		
		}		
	}	
	else if( ITEMKIND_PET_ITEM == pItem->Kind) 		
	{
		// 파트너 외형	
		nItemShapeType = LOOK_MIXING_PET_ITEM;
	}	
	else if(pItem->IsExistDesParam(DES_SHAPE_ITEM))
	{
		nItemShapeType = LOOK_MIXING_SHAPE_ITEM;
		INT nParmaVale = (INT)pItem->GetParameterValue(DES_SHAPE_ITEM);
		switch(nParmaVale)
		{		
		case KIND_ITEM_FIX_MIXING_REQ_PRIMARY_WEAPON_A:		// 무기 외형 - 기본 무기 A 기어
			{
				nItemShapeType = LOOK_MIXING_SHAPE_PRIMARY_WEAPON_A;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_PRIMARY_WEAPON_BMI:		// 무기 외형 - 기본 무기 BMI 기어
			{
				nItemShapeType = LOOK_MIXING_SHAPE_PRIMARY_WEAPON_BMI;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAPON_A:		// 무기 외형 - 고급 무기 A 기어
			{
				nItemShapeType = LOOK_MIXING_SHAPE_SECOND_WEAPON_A;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAPON_BMI:		// 무기 외형 - 고급 무기 BMI 기어
			{
				nItemShapeType = LOOK_MIXING_SHAPE_SECOND_WEAPON_BMI;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_PRIMARY_WEAPON:		// 무기 외형 - 기본 무기 이펙트
			{
				nItemShapeType = LOOK_MIXING_SHAPE_EFFECT_PRIMARY_WEAPON;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAPON:			// 무기 외형 - 고급 무기 이펙트
			{
				nItemShapeType = LOOK_MIXING_SHAPE_EFFECT_SECOND_WEAPON;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_DEFENSE_B:				// 아머 외형 - B 기어
			{
				nItemShapeType = LOOK_MIXING_SHAPE_DEFENSE_B;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_DEFENSE_M:				// 아머 외형 - M 기어
			{
				nItemShapeType = LOOK_MIXING_SHAPE_DEFENSE_M;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_DEFENSE_A:				// 아머 외형 - A 기어
			{
				nItemShapeType = LOOK_MIXING_SHAPE_DEFENSE_A;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_DEFENSE_I:				// 아머 외형 - I 기어
			{
				nItemShapeType = LOOK_MIXING_SHAPE_DEFENSE_I;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_DEFENSE:			// 아머 외형 - 초기화
			{
				nItemShapeType = LOOK_MIXING_SHAPE_DEFENSE_INIT;
			}
			break;		
		case KIND_ITEM_FIX_MIXING_REQ_PET_ITEM:				// 파트너 외형 - // 2012-12-03 by hskim, 파트너 기간제 외형 변경
			{
				nItemShapeType = LOOK_MIXING_SHAPE_PET_ITEM;
			}
			break;		
		case KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAP:		// 이펙트 외형 - 고급 무기 초기화 
			{
				nItemShapeType = LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_PRIMARY_WEAP:	// 이펙트 외형 - 기본 무기 초기화 
			{
				nItemShapeType = LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT;
			}
			break;		
		}
	}
	else if(pItem->IsExistDesParam(DES_EFFECT_ITEM))
	{
		nItemShapeType = LOOK_MIXING_SHAPE_EFFECT;
		INT nParmaVale = (INT)pItem->GetParameterValue(DES_EFFECT_ITEM);
		switch(nParmaVale)
		{		
		case KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAP:		// 이펙트 외형 - 고급 무기 초기화 
			{
				nItemShapeType = LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT;
			}
			break;
		case KIND_ITEM_FIX_MIXING_REQ_PRIMARY_WEAP:	// 이펙트 외형 - 기본 무기 초기화 
			{
				nItemShapeType = LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT;
			}
			break;		
		}
	}
	return nItemShapeType;
}

void CAtumDatabase::Find_ItemType_To_FindIdx(int nShapeIdx, vector<INT> *pvecShape)
{
	pvecShape->clear();

	switch(nShapeIdx)
	{
	case LOOK_MIXING_PRIMARY_WEAPON_A:			// 무기 외형 - 기본 무기 A 기어
		{			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_PRIMARY_WEAPON_A); 
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_EFFECT_PRIMARY_WEAPON); 			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT); 						
		}
		break;
	case LOOK_MIXING_PRIMARY_WEAPON_BMI:		// 무기 외형 - 기본 무기 BMI 기어
		{			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_PRIMARY_WEAPON_BMI); 
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_EFFECT_PRIMARY_WEAPON); 			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT); 						
		}
		break;
	case LOOK_MIXING_SECOND_WEAPON_A:			// 무기 외형 - 고급 무기 A 기어
		{			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_SECOND_WEAPON_A); 
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_EFFECT_SECOND_WEAPON); 			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT); 						
		}
		break;
	case LOOK_MIXING_SECOND_WEAPON_BMI:			// 무기 외형 - 고급 무기 BMI 기어
		{			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_SECOND_WEAPON_BMI); 
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_EFFECT_SECOND_WEAPON); 			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT); 						
		}
		break;
	case LOOK_MIXING_DEFENSE_B:					// 아머 외형 - B 기어
		{			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_DEFENSE_B); 
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_DEFENSE_INIT); 			
		}
		break;
	case LOOK_MIXING_DEFENSE_M:					// 아머 외형 - M 기어
		{			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_DEFENSE_M); 
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_DEFENSE_INIT); 			
		}
		break;
	case LOOK_MIXING_DEFENSE_A:					// 아머 외형 - A 기어
		{			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_DEFENSE_A); 
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_DEFENSE_INIT); 			
		}
		break;
	case LOOK_MIXING_DEFENSE_I:					// 아머 외형 - I 기어	
		{			
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_DEFENSE_I); 
// 			pvecShape->push_back(LOOK_MIXING_SHAPE_DEFENSE_INIT); 			
		}
		break;
	case LOOK_MIXING_PET_ITEM:					// 파트너 외형 - 파트너 기간제 외형 변경
		{			
			pvecShape->push_back(LOOK_MIXING_SHAPE_PET_ITEM); 
		}
		break;
	case LOOK_MIXING_EFFECT_PRIMARY_WEAPON:		// 무기 외형 - 기본 무기 이펙트
		{
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_A);			// 무기 외형 - 기본 무기 A 기어
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_BMI);		// 무기 외형 - 기본 무기 BMI 기어						
		}
		break;
	case LOOK_MIXING_EFFECT_SECOND_WEAPON:		// 무기 외형 - 고급 무기 이펙트		
		{
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_A);			// 무기 외형 - 고급 무기 A 기어			
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_BMI);			// 무기 외형 - 고급 무기 BMI 기어			
		}
		break;
	
	case LOOK_MIXING_SHAPE_ITEM:				// 외형 카드 - 
		{
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_A);			// 무기 외형 - 기본 무기 A 기어
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_BMI);		// 무기 외형 - 기본 무기 BMI 기어
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_A);			// 무기 외형 - 고급 무기 A 기어
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_BMI);			// 무기 외형 - 고급 무기 BMI 기어
			pvecShape->push_back(LOOK_MIXING_DEFENSE_B);					// 아머 외형 - B 기어
			pvecShape->push_back(LOOK_MIXING_DEFENSE_M);					// 아머 외형 - M 기어
			pvecShape->push_back(LOOK_MIXING_DEFENSE_A);					// 아머 외형 - A 기어
			pvecShape->push_back(LOOK_MIXING_DEFENSE_I);					// 아머 외형 - I 기어			
			pvecShape->push_back(LOOK_MIXING_PET_ITEM);					// 파트너 외형 - 파트너 기간제 외형 변경		
		}
		break;
	case LOOK_MIXING_SHAPE_EFFECT:				// 이펙트 카드- 
		{
			pvecShape->push_back(LOOK_MIXING_EFFECT_PRIMARY_WEAPON);		// 무기 외형 - 기본 무기 이펙트
			pvecShape->push_back(LOOK_MIXING_EFFECT_SECOND_WEAPON);		// 무기 외형 - 고급 무기 이펙트				
		}
		break;
	case LOOK_MIXING_SHAPE_PRIMARY_WEAPON_A:				// 무기 외형 - 기본 무기 A 기어
		{
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_A);			
		}
		break;
	case LOOK_MIXING_SHAPE_PRIMARY_WEAPON_BMI:				// 무기 외형 - 기본 무기 BMI 기어
		{
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_BMI);		
		}
		break;
	case LOOK_MIXING_SHAPE_SECOND_WEAPON_A:					// 무기 외형 - 고급 무기 A 기어
		{
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_A);			
		}
		break;
	case LOOK_MIXING_SHAPE_SECOND_WEAPON_BMI:				// 무기 외형 - 고급 무기 BMI 기어
		{
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_BMI);			
		}
		break;
	case LOOK_MIXING_SHAPE_EFFECT_PRIMARY_WEAPON:			// 무기 외형 - 기본 무기 이펙트
		{
//			pvecShape->push_back(LOOK_MIXING_EFFECT_PRIMARY_WEAPON);			
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_A);			// 무기 외형 - 기본 무기 A 기어
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_BMI);		// 무기 외형 - 기본 무기 BMI 기어									
		}
		break;
	case LOOK_MIXING_SHAPE_EFFECT_SECOND_WEAPON:			// 무기 외형 - 고급 무기 이펙트
		{
//			pvecShape->push_back(LOOK_MIXING_EFFECT_SECOND_WEAPON);			
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_A);			// 무기 외형 - 고급 무기 A 기어			
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_BMI);		// 무기 외형 - 고급 무기 BMI 기어			
		}
		break;
	case LOOK_MIXING_SHAPE_DEFENSE_B:						// 아머 외형 - B 기어
		{
			pvecShape->push_back(LOOK_MIXING_DEFENSE_B);			
		}
		break;
	case LOOK_MIXING_SHAPE_DEFENSE_M:						// 아머 외형 - M 기어
		{
			pvecShape->push_back(LOOK_MIXING_DEFENSE_M);			
		}
		break;
	case LOOK_MIXING_SHAPE_DEFENSE_A:						// 아머 외형 - A 기어
		{
			pvecShape->push_back(LOOK_MIXING_DEFENSE_A);			
		}
		break;
	case LOOK_MIXING_SHAPE_DEFENSE_I:						// 아머 외형 - I 기어
		{
			pvecShape->push_back(LOOK_MIXING_DEFENSE_I);			
		}
		break;
	case LOOK_MIXING_SHAPE_DEFENSE_INIT:					// 아머 외형 - 초기화
		{
			pvecShape->push_back(LOOK_MIXING_DEFENSE_B);			
			pvecShape->push_back(LOOK_MIXING_DEFENSE_M);			
			pvecShape->push_back(LOOK_MIXING_DEFENSE_A);			
			pvecShape->push_back(LOOK_MIXING_DEFENSE_I);
		}
		break;
	case LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT:				// 이펙트 외형 - 고급 무기 초기화 
		{
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_A);		// 무기 외형 - 고급 무기 A 기어
			pvecShape->push_back(LOOK_MIXING_SECOND_WEAPON_BMI);	// 무기 외형 - 고급 무기 BMI 기어
		}
		break;
	case LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT:				// 이펙트 외형 - 기본 무기 초기화 
		{
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_A);		// 무기 외형 - 기본 무기 A 기어
			pvecShape->push_back(LOOK_MIXING_PRIMARY_WEAPON_BMI);	// 무기 외형 - 기본 무기 BMI 기어
		}
		break;
	case LOOK_MIXING_SHAPE_PET_ITEM:						// 파트너 외형 - 파트너 기간제 외형 변경
		{
			pvecShape->push_back(LOOK_MIXING_PET_ITEM);			
		}
		break;
	}
}

void CAtumDatabase::GetMixingItemListIndex(INT nItemNum, vector<structResultMixingInfo> *o_pVecItemList)
{

}

void CAtumDatabase::ReleaseMixingItemListIndex()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LOOK_MIXING_ITEM;nCnt++)
	{
		if(!m_pMapItemShapeType[nCnt])
		{
			continue;
		}
		
		CMapItemInfoIterator it = m_pMapItemShapeType[nCnt]->begin();
		while(it != m_pMapItemShapeType[nCnt]->end())
		{
			ITEM* pItem = it->second;
			if(!pItem)
			{
				it++;
				continue;
			}
			util::del(it->second);
			it++;
		}
		m_pMapItemShapeType[nCnt]->clear();
		util::del(m_pMapItemShapeType[nCnt]);
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아이템 스트링 CULL
/// \author		// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
/// \date		2013-04-11 ~ 2013-04-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::SearchStringCull(char* pItemName, vector<string> *o_pVecItemList)
{
	int nCnt = 0;
	char* pCurStr = pItemName;
	int nPos = 0;
	int nStrLen = strlen(pItemName)+1;
	char *chTmp = (char*)malloc(nStrLen);
	memset(chTmp, 0x00, nStrLen);

	for(nCnt = 0;nCnt < strlen(pItemName)+1; nCnt++)
	{
		char *pFind = strstr(&pItemName[nPos], " ");		// 다음 문자로 이동
		if(!pFind)
		{
			// 여백을 못찾았다.
			string strTmp(&pItemName[nPos]);
			o_pVecItemList->push_back(strTmp);
			break;
		}
		int nStrPointGap = pFind - &pItemName[nPos];	
		if(nStrPointGap > 0)
		{
			strncpy(chTmp, &pItemName[nPos], nStrPointGap);
			string strTmp(chTmp);
			o_pVecItemList->push_back(strTmp);
			memset(chTmp, 0x00, nStrLen);
//			nPos = nStrPointGap+strlen(chTmp)+1;			
			nPos = nPos + nStrPointGap + 1;
		}
		if(nPos >= nStrLen)
		{
			break;
		}
		
	}
	free(chTmp);
}

BOOL CAtumDatabase::SaveItemData(FILE* fd)
{
	ASSERT_ASSERT(fd);
	return TRUE;
}

BOOL CAtumDatabase::SaveMonsterInfo(FILE* fd)
{
	ASSERT_ASSERT(fd);
	return TRUE;
}

BOOL CAtumDatabase::SaveMapObjectInfo(FILE* fd)
{
	ASSERT_ASSERT(fd);
	return TRUE;
}

BOOL CAtumDatabase::SaveRareItemData(FILE* fd)
{
	ASSERT_ASSERT(fd);
	return TRUE;
}

BOOL CAtumDatabase::SaveBuildingNPC(FILE* fd)
{
	ASSERT_ASSERT(fd);
	return TRUE;
}

BOOL CAtumDatabase::SaveGuildMark(FILE* fd)
{
	ASSERT_ASSERT(fd);
	DATA_HEADER header;
	header.nType = DB_GUILD_MARK;
	header.nDataCount = m_mapGuildMark.size();
	fwrite((char*)&header,sizeof(header),1,fd);
	CMapGuildMarkIterator it = m_mapGuildMark.begin();
	while(it != m_mapGuildMark.end())
	{
		GUILD_MARK_VERSION * pGuildMark = it->second;
		// 2007-08-03 by bhsohn 길드 마크 심사 시스템 구현
		if(pGuildMark->nSizeOfImage > 0)
		{
			fwrite( pGuildMark, sizeof(GUILD_MARK_VERSION) - GUILD_POINTER_SIZE, 1, fd );
			fwrite( pGuildMark->pData, pGuildMark->nSizeOfImage, 1, fd );
		}
		// end 2007-08-03 by bhsohn 길드 마크 심사 시스템 구현
		it++;
	}

	return TRUE;
}

BOOL CAtumDatabase::SaveMapInfo(FILE* fd)
{
	ASSERT_ASSERT(fd);
	DATA_HEADER header;
	header.nType = DB_MAP_INFO;
	header.nDataCount = m_mapMapInfo.size();
	fwrite((char*)&header,sizeof(header),1,fd);
	CMapMapInfoIterator it = m_mapMapInfo.begin();
	while(it != m_mapMapInfo.end())
	{
		MAP_INFO* pMapInfo = it->second;
		fwrite( pMapInfo, sizeof(MAP_INFO), 1, fd );
		it++;
	}
	return TRUE;
}
BOOL CAtumDatabase::SaveMixingInfo(FILE* fd)
{
	ASSERT_ASSERT(fd);
	DATA_HEADER header;
	header.nType = DB_MIXING_INFO;
	header.nDataCount = m_VecMixingInfo.size();
	fwrite((char*)&header,sizeof(header),1,fd);
	CVectorMixingInfoIterator it = m_VecMixingInfo.begin();
	while(it != m_VecMixingInfo.end())
	{
		ITEM_MIXING_INFO* pMixingInfo = *it;
		fwrite( pMixingInfo, sizeof(ITEM_MIXING_INFO), 1, fd );
		it++;
	}
	return TRUE;
}

void CAtumDatabase::InitDeviceObjects()
{
//	int nFileSize = 0;// 2006-06-22 by ispark
	FILE * fd;
	
	// 2006-07-04 by ispark
	// omi.tex 로딩 시스템 변경. 파일에 이상 유무를 판단한다. 이상시 게임 종료. 
	// 그외 게임상 omi 이상시는 그 때 체크해서 게임 종료를 시킨다.
	// 두번 체크(omi.tex, omi_o.tex)
	for (int nFileCount = 0; nFileCount < 2 ; nFileCount++)
	{
		fd = fopen(".\\Res-Tex\\omi.tex","rb");
		if(!fd)
		{
			DBGOUT("Data File Open Error(omi.tex)\n");
			g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
			return;
		}

		// 파일 사이즈 조사
		fseek(fd, 0L, SEEK_END);
		int length = ftell(fd);
		if(length <= 0)
		{
			DBGOUT("------> File Size Error (omi.tex) <------\n");
			DBGOUT("<------= omi_o.tex Loading =------>\n");
			fclose(fd);

			// 마지막 파일 검사라면 종료한다.
			if(nFileCount == 1)
			{
				g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
				return;
			}

			// omi_o.tex로 임시로딩
			// omi.tex를 지운다. (이미 파일 내용이 없기 때문이다. 그러므로 필요가 없어졌다.)
			// omi_o.tex를 omi.tex로 이름을 변경 후 로딩한다.
			remove(".\\Res-Tex\\omi.tex");
			int nResult = rename(".\\Res-Tex\\omi_o.tex",".\\Res-Tex\\omi.tex");
			if( nResult != 0 )
			{
				DBGOUT("Data File ReName Error(omi_o.tex)\n");
				g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
				return;
			}
		}
		else
		{
			// 파일 이상무
			fseek(fd, 0L, SEEK_SET);
			break;
		}
	}

	while (true)
	{
		int nRead = 0;
		DATA_HEADER header;
		
		int pos = ftell(fd);
		
		if (fread(&header, sizeof(header), 1, fd) == 0) break;

#ifdef CIPHER2015
		CipherRA((char*)&header, sizeof(header), pos);
#endif

//		int size;
//		if( fread(&size,sizeof(int),1,fd) == 0)
//			break;
//		header.nType = DB_ITEM;
//		header.nDataCount = size;
//		nFileSize += header.nDataCount;
		switch(header.nType)
		{
			case DB_ITEM:
				{
					if(LoadItemData( fd, header.nDataCount ) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_ITEM)\n");
						return;
					}

	//				DBGOUT("Load DB_ITEM DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
				}
				break;
			case DB_MONSTER_INFO:
				{
					if(LoadMonsterInfo( fd, header.nDataCount ) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_MONSTER_INFO)\n");
						return;
					}
	//				DBGOUT("Load DB_MONSTER_INFO DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
				}
				break;
			case DB_MAPOBJECT:
				{
					if(LoadMapObjectInfo( fd, header.nDataCount ) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_MAPOBJECT)\n");
						return;
					}
	//				DBGOUT("Load DB_MAPOBJECT DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
				}
				break;
			case DB_RARE_ITEM:
				{
					if(LoadRareItemData( fd, header.nDataCount ) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_RARE_ITEM)\n");
						return;
					}
	//				DBGOUT("Load DB_RARE_ITEM DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
				}
				break;
			case DB_BUILDINGNPC:
				{
					if(LoadBuildingNPC( fd, header.nDataCount ) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_BUILDINGNPC)\n");
						return;
					}
	//				DBGOUT("Load DB_BUILDINGNPC DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
				}
				break;
			case DB_GUILD_MARK:
				{
					if(LoadGuildMark( fd, header.nDataCount ) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_GUILD_MARK)\n");
						return;
					}
	//				DBGOUT("Load DB_GUILD_MARK DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
				}
				break;
			case DB_MAP_INFO:
				{
					if(LoadMapInfo( fd, header.nDataCount ) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_MAP_INFO)\n");
						return;
					}
	//				DBGOUT("Load DB_MAP_INFO DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
				}
				break;
			case DB_MIXING_INFO:
				{
					if(LoadMixingInfo( fd, header.nDataCount ) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_MIXING_INFO)\n");
						return;
					}
	//				DBGOUT("Load DB_MIXING_INFO DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
				}
				break;
			case DB_MYSTERY_ITEM_DROP:
				{
					if(LoadMysteryInfo(fd,header.nDataCount) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_MYSTERY_ITEM_DROP)\n");
						return;
					}
				}
				break;
			// 2009-03-04 by bhsohn 럭키 머신 수정안
			case DB_LUCKYMACHINE:
				{
					if(LoadLuckyMachineInfo(fd,header.nDataCount) == FALSE)
					{
						DBGOUT("Data File Read Error(omi.tex : DB_LUCKYMACHINE)\n");
						return;
					}
				}
				break;
			// end 2009-03-04 by bhsohn 럭키 머신 수정안

			// 2010. 02. 11 by ckPark 발동류 장착아이템
			case DB_INVOKINGWEARITEM_DPNUM:
				{
					// 발동류 데스파람
					if( !LoadInvokingWearItemDPNum( fd, header.nDataCount ) )
					{
						DBGOUT("Data File Read Error(omi.tex : DB_INVOKINGWEARITEM_DPNUM)\n");
						return;
					}
				}
				break;

			case DB_INVOKINGWEARITEM_DPNUM_BY_USE:
				{
					// 사용류 데스파람
					if( !LoadInvokingWearItemDPNumByUse( fd, header.nDataCount ) )
					{
						DBGOUT("Data File Read Error(omi.tex : DB_INVOKINGWEARITEM_DPNUM_BY_USE)\n");
						return;
					}
				}
				break;
		// end 2010. 02. 11 by ckPark 발동류 장착아이템

		// 2010-06-15 by shcho&hslee 펫시스템
			case DB_PET_BASEDATA :

				if ( !LoadPetBaseData ( fd , header.nDataCount ) )
				{
					DBGOUT("Data File Read Error(omi.tex : DB_PET_BASEDATA)\n");
					return;
				}
				break;

			case DB_PET_LEVELDATA : 

				if ( !LoadPetLevelData ( fd , header.nDataCount ) )
				{
					DBGOUT("Data File Read Error(omi.tex : DB_PET_LEVELDATA)\n");
					return;
				}
				break;
            // 2011. 03. 08 by jskim 인피3차 구현
			case DB_PET_OPERATOR :
	
				if ( !LoadOperatorData ( fd , header.nDataCount ) )
				{
					DBGOUT("Data File Read Error(omi.tex : DB_PET_OPERATOR)\n");
					return;
				}
				break;
		// End 2010-06-15 by shcho&hslee 펫시스템
			// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
			case DB_DISSOLUTIONITEM:
				{
					if(!LoadDissolutionItemData( fd , header.nDataCount ) )
					{
						DBGOUT("Data File Read Error(omi.tex : DB_DISSOLUTIONITEM)\n");
						return;
					}
				}
				break;							 
            // end 2011. 03. 08 by jskim 인피3차 구현
			// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
			// 2010-08-10 by dgwoo 버닝맵 시스템
			case DB_BURNING_MAP:
				if(!LoadBurningMapData(fd , header.nDataCount))
				{
					DBGOUT("Data File Read Error(omi.tex : DB_BURNING_MAP)\n");
					return;
				}
				break;
			// 2010-08-10 by dgwoo 버닝맵 시스템

			// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
			case DB_MONSTER_MULTI_TARGET:
#ifdef MULTI_TARGET_JSKIM
				if(!LoadMultiTargetIndexData(fd , header.nDataCount))
				{
					DBGOUT("Data File Read Error(omi.tex : DB_MONSTER_MULTI_TARGET)\n");
					return;
				}
#endif
				break;
			//end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
		}

	}
	fclose(fd);

	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	InitMixingItemListIndex();
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

	// 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정
	InitMixingItemSearchTable();		// [재료-아이템번호] 검색을 하기 위한 Map생성
	// end 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정
	
	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	InitResultMixingItemSearchTable();	// [결과-이름] 검색을 하기 위한 Map생성
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리
}

void CAtumDatabase::RestoreDeviceObjects()
{
	CMapGuildMarkIterator itGuildMark = m_mapGuildMark.begin();
	while(itGuildMark != m_mapGuildMark.end())
	{		
		itGuildMark->second->pImage->RestoreDeviceObjects();
		itGuildMark->second->pImage->SetRect( 0,0,24,12);
		itGuildMark++;
	}
}

void CAtumDatabase::InvalidateDeviceObjects()
{
	CMapGuildMarkIterator itGuildMark = m_mapGuildMark.begin();
	while(itGuildMark != m_mapGuildMark.end())
	{
		itGuildMark->second->pImage->InvalidateDeviceObjects();
		itGuildMark++;
	}
}

void CAtumDatabase::DeleteDeviceObjects()
{
	FLOG("CAtumDatabase::DeleteDeviceObjects()");
	char buf[256];
	wsprintf(buf,".\\Res-Tex\\omi_n.tex");
	FILE * fd;
	fd = fopen(buf,"w+b");
//	int nFileSize = 0;// 2006-06-22 by ispark
	// 2006-07-05 by ispark, 강제 종료는 저장을 시키지 않는다.
	if(fd && g_pD3dApp->m_bShutDown)
	{
		fclose(fd);
		remove(".\\Res-Tex\\omi_n.tex");
		fd = NULL;
	}

	if(fd)
	{
		// Item
		DATA_HEADER header;
		CMapItemInfoIterator itInfoItem = m_mapItemInfo.begin();
		CMapItemInfoIterator itInfoItemTemp = m_mapItemInfoTemp.end();
		while(itInfoItem != m_mapItemInfo.end())
		{
			if(itInfoItem->second)
			{
				ITEM * item = itInfoItem->second;
				itInfoItemTemp = m_mapItemInfoTemp.find(item->ItemNum);
				if(itInfoItemTemp != m_mapItemInfoTemp.end())
				{
					util::del(itInfoItemTemp->second);
					m_mapItemInfoTemp.erase(itInfoItemTemp);
				}
			}
			itInfoItem++;
		}
		//int temp = m_mapItemInfo.size() + m_mapItemInfoTemp.size();
		//fwrite(&temp,1,sizeof(int),fd);
		header.nType = DB_ITEM;
		header.nDataCount = m_mapItemInfo.size() + m_mapItemInfoTemp.size();
		fwrite(&header,1,sizeof(header),fd);
//		nFileSize += header.nDataCount;// 2006-06-22 by ispark
//		DBGOUT("Save DB_ITEM DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
		itInfoItem = m_mapItemInfo.begin();
		while(itInfoItem != m_mapItemInfo.end())
		{
			if(itInfoItem->second)
			{
				ITEM * item = itInfoItem->second;
				fwrite(item,1,sizeof(ITEM),fd);
				util::del(itInfoItem->second);
			}
			itInfoItem++;
		}
		m_mapItemInfo.clear();
		itInfoItemTemp = m_mapItemInfoTemp.begin();
		while(itInfoItemTemp != m_mapItemInfoTemp.end())
		{
			if(itInfoItemTemp->second)
			{
				ITEM * item = itInfoItemTemp->second;
				fwrite(item,1,sizeof(ITEM),fd);
				util::del(itInfoItemTemp->second);
			}
			itInfoItemTemp++;
		}
		m_mapItemInfoTemp.clear();
		// Monster
		CMapMonsterInfoIterator itInfoMon = m_mapMonsterInfo.begin();
		CMapMonsterInfoIterator itInfoMonTemp = m_mapMonsterInfoTemp.end();
		while(itInfoMon != m_mapMonsterInfo.end())
		{
			if(itInfoMon->second)
			{
				MEX_MONSTER_INFO * monster = itInfoMon->second;
				itInfoMonTemp = m_mapMonsterInfoTemp.find(monster->MonsterUnitKind);
				if(itInfoMonTemp != m_mapMonsterInfoTemp.end())
				{
					util::del(itInfoMonTemp->second);
					m_mapMonsterInfoTemp.erase(itInfoMonTemp);
				}
			}
			itInfoMon++;
		}
//		temp = m_mapMonsterInfo.size() + m_mapMonsterInfoTemp.size();
//		fwrite(&temp,1,sizeof(int),fd);
		header.nType = DB_MONSTER_INFO;
		header.nDataCount = m_mapMonsterInfo.size() + m_mapMonsterInfoTemp.size();
		fwrite(&header,1,sizeof(header),fd);
//		nFileSize += header.nDataCount;// 2006-06-22 by ispark
//		DBGOUT("Save DB_MONSTER_INFO DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
		itInfoMon = m_mapMonsterInfo.begin();
		while(itInfoMon != m_mapMonsterInfo.end())
		{
			if(itInfoMon->second)
			{
				MEX_MONSTER_INFO * monster = itInfoMon->second;
				fwrite(monster,1,sizeof(MEX_MONSTER_INFO),fd);
				util::del(itInfoMon->second);
			}
			itInfoMon++;
		}
		m_mapMonsterInfo.clear();
		itInfoMonTemp = m_mapMonsterInfoTemp.begin();
		while(itInfoMonTemp != m_mapMonsterInfoTemp.end())
		{
			if(itInfoMonTemp->second)
			{
				MEX_MONSTER_INFO * monster = itInfoMonTemp->second;
				fwrite(monster,1,sizeof(MEX_MONSTER_INFO),fd);
				util::del(itInfoMonTemp->second);
			}
			itInfoMonTemp++;
		}
		m_mapMonsterInfoTemp.clear();
		// Object
		CMapObjectInfoIterator itInfoObj = m_mapObjectInfo.begin();
		CMapObjectInfoIterator itInfoObjTemp = m_mapObjectInfoTemp.end();
		while(itInfoObj != m_mapObjectInfo.end())
		{
			if(itInfoObj->second)
			{
				MAPOBJECTINFO * object = itInfoObj->second;
				itInfoObjTemp = m_mapObjectInfoTemp.find(object->Code);
				if(itInfoObjTemp != m_mapObjectInfoTemp.end())
				{
					util::del(itInfoObjTemp->second);
					m_mapObjectInfoTemp.erase(itInfoObjTemp);
				}
			}
			itInfoObj++;
		}
//		temp = m_mapObjectInfo.size() + m_mapObjectInfoTemp.size();
//		fwrite(&temp,1,sizeof(int),fd);
		header.nType = DB_MAPOBJECT;
		header.nDataCount = m_mapObjectInfo.size() + m_mapObjectInfoTemp.size();
		fwrite(&header,1,sizeof(header),fd);
//		nFileSize += header.nDataCount;// 2006-06-22 by ispark
//		DBGOUT("Save DB_MAPOBJECT DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
		itInfoObj = m_mapObjectInfo.begin();
		while(itInfoObj != m_mapObjectInfo.end())
		{
			if(itInfoObj->second)
			{
				MAPOBJECTINFO * object = itInfoObj->second;
				fwrite(object,1,sizeof(MAPOBJECTINFO),fd);
				util::del(itInfoObj->second);
			}
			itInfoObj++;
		}
		m_mapObjectInfo.clear();
		itInfoObjTemp = m_mapObjectInfoTemp.begin();
		while(itInfoObjTemp != m_mapObjectInfoTemp.end())
		{
			if(itInfoObjTemp->second)
			{
				MAPOBJECTINFO * object = itInfoObjTemp->second;
				fwrite(object,1,sizeof(MAPOBJECTINFO),fd);
				util::del(itInfoObjTemp->second);
			}
			itInfoObjTemp++;
		}
		m_mapObjectInfoTemp.clear();
		/////Rare Item info ///////////////////////////////////////////////////////////////////
		CMapRareItemInfoIterator itRareItem;
		CMapRareItemInfoIterator itRareItemTemp;
		itRareItem = m_mapRareItemInfo.begin();
		while(itRareItem != m_mapRareItemInfo.end())
		{
			if(itRareItem->second)
			{
				RARE_ITEM_INFO * pRareItem = itRareItem->second;
				itRareItemTemp = m_mapRareItemInfoTemp.find(pRareItem->CodeNum);
				if(itRareItemTemp != m_mapRareItemInfoTemp.end())
				{
					util::del(itRareItemTemp->second);
					m_mapRareItemInfoTemp.erase(itRareItemTemp);
				}
			}
			itRareItem++;
		}
//		temp = m_mapRareItemInfo.size() + m_mapRareItemInfoTemp.size();
//		fwrite(&temp,sizeof(int),1,fd);
		header.nType = DB_RARE_ITEM;
		header.nDataCount = m_mapRareItemInfo.size() + m_mapRareItemInfoTemp.size();
		fwrite(&header,1,sizeof(header),fd);
//		nFileSize += header.nDataCount;// 2006-06-22 by ispark
//		DBGOUT("Save DB_RARE_ITEM DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
		itRareItem = m_mapRareItemInfo.begin();
		while(itRareItem != m_mapRareItemInfo.end())
		{
			if(itRareItem->second)
			{
				RARE_ITEM_INFO * pRareItem = itRareItem->second;
				fwrite(pRareItem,1,sizeof(RARE_ITEM_INFO),fd);
				util::del(itRareItem->second);
			}
			itRareItem++;
		}
		m_mapRareItemInfo.clear();
		itRareItemTemp = m_mapRareItemInfoTemp.begin();
		while(itRareItemTemp != m_mapRareItemInfoTemp.end())
		{
			if(itRareItemTemp->second)
			{
				RARE_ITEM_INFO * pRareItem = itRareItemTemp->second;
				fwrite(pRareItem,1,sizeof(RARE_ITEM_INFO),fd);
				util::del(itRareItemTemp->second);
			}
			itRareItemTemp++;
		}
		m_mapRareItemInfoTemp.clear();
		/////BUILDINGNPC info ///////////////////////////////////////////////////////////////////
		CMapBuildingNPCIterator itBuildingNPC;
		CMapBuildingNPCIterator itBuildingNPCTemp;
		itBuildingNPC = m_mapBuildingNPCInfo.begin();
		while(itBuildingNPC != m_mapBuildingNPCInfo.end())
		{
			if(itBuildingNPC->second)
			{
				BUILDINGNPC * pBuildingNPC = itBuildingNPC->second;
				itBuildingNPCTemp = m_mapBuildingNPCInfoTemp.find(pBuildingNPC->BuildingIndex);
				if(itBuildingNPCTemp != m_mapBuildingNPCInfoTemp.end())
				{
					util::del(itBuildingNPCTemp->second);
					m_mapBuildingNPCInfoTemp.erase(itBuildingNPCTemp);
				}
			}
			itBuildingNPC++;
		}
//		temp = m_mapBuildingNPCInfo.size() + m_mapBuildingNPCInfoTemp.size();
//		fwrite(&temp,sizeof(int),1,fd);
		header.nType = DB_BUILDINGNPC;
		header.nDataCount = m_mapBuildingNPCInfo.size() + m_mapBuildingNPCInfoTemp.size();
		fwrite(&header,1,sizeof(header),fd);
//		nFileSize += header.nDataCount;// 2006-06-22 by ispark
//		DBGOUT("Save DB_BUILDINGNPC DataCount --> %d(Total %d)\n", header.nDataCount, nFileSize);
		itBuildingNPC = m_mapBuildingNPCInfo.begin();
		while(itBuildingNPC != m_mapBuildingNPCInfo.end())
		{
			if(itBuildingNPC->second)
			{
				BUILDINGNPC * pBuildingNPC = itBuildingNPC->second;
				fwrite(pBuildingNPC,1,sizeof(BUILDINGNPC),fd);
				util::del(itBuildingNPC->second);
			}
			itBuildingNPC++;
		}
		m_mapBuildingNPCInfo.clear();
		itBuildingNPCTemp = m_mapBuildingNPCInfoTemp.begin();
		while(itBuildingNPCTemp != m_mapBuildingNPCInfoTemp.end())
		{
			if(itBuildingNPCTemp->second)
			{
				BUILDINGNPC * pBuildingNPC = itBuildingNPCTemp->second;
				fwrite(pBuildingNPC,1,sizeof(BUILDINGNPC),fd);
				util::del(itBuildingNPCTemp->second);
			}
			itBuildingNPCTemp++;
		}
		m_mapBuildingNPCInfoTemp.clear();
		/////////////GUILD MARK///////////////////////////////////////////
		SaveGuildMark(fd);
//		nFileSize += m_mapGuildMark.size();// 2006-06-22 by ispark
//		DBGOUT("Save DB_GUILD_MARK DataCount --> %d(Total %d)\n", m_mapGuildMark.size(), nFileSize);

		////////////MAP_INFO//////////////////////////////////////////////
		SaveMapInfo(fd);
//		nFileSize += m_mapMapInfo.size();// 2006-06-22 by ispark
//		DBGOUT("Save DB_MAP_INFO DataCount --> %d(Total %d)\n", m_mapMapInfo.size(), nFileSize);
		
		SaveMixingInfo(fd);
//		nFileSize += m_VecMixingInfo.size();// 2006-06-22 by ispark
//		DBGOUT("Save DB_MIXING_INFO DataCount --> %d(Total %d)\n", m_VecMixingInfo.size(), nFileSize);
		
		fclose(fd);
		// 2008-02-12 by bhsohn omi_n.tex제거
		remove(".\\Res-Tex\\omi_n.tex");
		// end 2008-02-12 by bhsohn omi_n.tex제거
		//
		// 2007-07-11 by bhsohn omi 체크섬 추가
//		remove(".\\Res-Tex\\omi_o.tex");
//		rename(".\\Res-Tex\\omi.tex",".\\Res-Tex\\omi_o.tex");
//		rename(".\\Res-Tex\\omi_n.tex",".\\Res-Tex\\omi.tex");
	}
	else
	{
		// 아래 코드 정리해야 함(Info,InfoTemp 둘다 지우고 있음)////////////////
		CMapItemInfoIterator itInfoItem = m_mapItemInfo.begin();
		while(itInfoItem != m_mapItemInfo.end())
		{
			if(itInfoItem->second)
			{
				util::del(itInfoItem->second);
			}
			itInfoItem++;
		}
		m_mapItemInfo.clear();
		itInfoItem = m_mapItemInfoTemp.begin();
		while(itInfoItem != m_mapItemInfoTemp.end())
		{
			if(itInfoItem->second)
			{
				util::del(itInfoItem->second);
			}
			itInfoItem++;
		}
		m_mapItemInfoTemp.clear();
		CMapMonsterInfoIterator itInfoMon = m_mapMonsterInfo.begin();
		while(itInfoMon != m_mapMonsterInfo.end())
		{
			if(itInfoMon->second)
			{
				util::del(itInfoMon->second);
			}
			itInfoMon++;
		}
		m_mapMonsterInfo.clear();
		itInfoMon = m_mapMonsterInfoTemp.begin();
		while(itInfoMon != m_mapMonsterInfoTemp.end())
		{
			if(itInfoMon->second)
			{
				util::del(itInfoMon->second);
			}
			itInfoMon++;
		}
		m_mapMonsterInfoTemp.clear();
		CMapObjectInfoIterator itInfoObj = m_mapObjectInfo.begin();
		while(itInfoObj != m_mapObjectInfo.end())
		{
			if(itInfoObj->second)
			{
				util::del(itInfoObj->second);
			}
			itInfoObj++;
		}
		m_mapObjectInfo.clear();
		itInfoObj = m_mapObjectInfoTemp.begin();
		while(itInfoObj != m_mapObjectInfoTemp.end())
		{
			if(itInfoObj->second)
			{
				util::del(itInfoObj->second);
			}
			itInfoObj++;
		}
		m_mapObjectInfoTemp.clear();
	}
	CMapGuildMarkIterator itGuildMark = m_mapGuildMark.begin();
	while(itGuildMark != m_mapGuildMark.end())
	{
		itGuildMark->second->pImage->DeleteDeviceObjects();
		util::del(itGuildMark->second->pImage);
		util::del(itGuildMark->second->pData);
		util::del(itGuildMark->second);
		itGuildMark++;
	}
	m_mapGuildMark.clear();
	CMapOtherGuildInfoIterator itInfo = m_mapOtherGuildInfo.begin();
	while(itInfo != m_mapOtherGuildInfo.end())
	{
		util::del(itInfo->second);
		itInfo++;
	}
	m_mapOtherGuildInfo.clear();

	// 2010-06-15 by shcho&hslee 펫시스템
	DeletePetData();
	// End 2010-06-15 by shcho&hslee 펫시스템

}



MEX_MONSTER_INFO * CAtumDatabase::CheckMonsterInfo(int nMonType)
{
	FLOG("CAtumDatabase::CheckMonsterInfo(int nMonType)");
	MEX_MONSTER_INFO * monster = NULL;
	CMapMonsterInfoIterator itMon = m_mapMonsterInfoTemp.end();
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapIsLoadingIterator itInfo = m_mapMonsterGetInfo.find(nMonType);
	if(itInfo == m_mapMonsterGetInfo.end())
	{
		// 2006-06-15 by ispark, 일정시간에 패킷을 보내는 것으로 변경
//		MSG_FC_INFO_GET_MONSTER_INFO sMsg;
//		sMsg.MonsterUnitKind = nMonType;
//		g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_MONSTER_INFO, (char*)&sMsg, sizeof(sMsg));
		SetIntDataByVector(g_pD3dApp->m_vecMonsterInfo, nMonType);

		BOOL bGetInfo = FALSE;
		m_mapMonsterGetInfo[nMonType] = bGetInfo;
		// 임시 정보
		itMon = m_mapMonsterInfoTemp.find(nMonType);
		if(itMon != m_mapMonsterInfoTemp.end())
			monster = itMon->second;
	}
	else
	{
		if(!itInfo->second)
		{
#ifdef _DEBUG
			// 정보 재요청 후 임시 정보를 꺼낸다.
			// 2006-06-15 by ispark, 일정시간에 패킷을 보내는 것으로 변경
//			MSG_FC_INFO_GET_MONSTER_INFO sMsg;
//			sMsg.MonsterUnitKind = nMonType;
//			g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_MONSTER_INFO, (char*)&sMsg, sizeof(sMsg));
			SetIntDataByVector(g_pD3dApp->m_vecMonsterInfo, nMonType);

			BOOL bGetInfo = FALSE;
			m_mapMonsterGetInfo[nMonType] = bGetInfo;
#endif // _DEBUG_endif
			// 임시 정보
			itMon = m_mapMonsterInfoTemp.find(nMonType);
			if(itMon != m_mapMonsterInfoTemp.end())
				monster = itMon->second;
		}
		else
		{// 정보 요청없이 맵에서 정보를 꺼낸다.
			itMon = m_mapMonsterInfo.find(nMonType);
			if(itMon != m_mapMonsterInfo.end())
				monster = itMon->second;
		}
	}
	LeaveCriticalSection(&g_pD3dApp->m_cs);
	if(!monster)
	{
		DBGOUT("Monster Info Is NULL : %d\n", nMonType);
	}
	return monster;
}

MAPOBJECTINFO* CAtumDatabase::CheckObjectInfo(int nObjType)
{
	FLOG("CAtumDatabase::CheckObjectInfo(int nObjType)");
	MAPOBJECTINFO * object = NULL;
	CMapObjectInfoIterator itObj = m_mapObjectInfoTemp.end();
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapIsLoadingIterator itInfo = m_mapObjectGetInfo.find(nObjType);
	if(itInfo == m_mapObjectGetInfo.end())
	{
		// 2006-06-15 by ispark, 일정시간에 패킷을 보내는 것으로 변경
//		g_pD3dApp->SendFieldSocketGetInfo(nObjType);
//		MSG_FC_INFO_GET_MAPOBJECT_INFO sMsg;
//		sMsg.Code = nObjType;
//		g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_MAPOBJECT_INFO, (char*)&sMsg, sizeof(sMsg));
		SetIntDataByVector(g_pD3dApp->m_vecMapObjectInfo, nObjType);

		BOOL bGetInfo = FALSE;
		m_mapObjectGetInfo[nObjType] = bGetInfo;
		// 임시 정보
		itObj = m_mapObjectInfoTemp.find(nObjType);
		if(itObj != m_mapObjectInfoTemp.end())
			object = itObj->second;
	}
	else
	{
		if(!itInfo->second)
		{
#ifdef _DEBUG
			// 정보 재요청 후 임시 정보를 꺼낸다.
//			MSG_FC_INFO_GET_MAPOBJECT_INFO sMsg;
//			sMsg.Code = nObjType;
//			g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_MAPOBJECT_INFO, (char*)&sMsg, sizeof(sMsg));
//			BOOL bGetInfo = FALSE;
//			m_mapObjectGetInfo[nObjType] = bGetInfo;
#endif //_DEBUG_endif
			// 임시 정보
			itObj = m_mapObjectInfoTemp.find(nObjType);
			if(itObj != m_mapObjectInfoTemp.end())
				object = itObj->second;
		}
		else
		{// 정보 요청없이 맵에서 정보를 꺼낸다.
			itObj = m_mapObjectInfo.find(nObjType);
			if(itObj != m_mapObjectInfo.end())
				object = itObj->second;
		}
	}
	LeaveCriticalSection(&g_pD3dApp->m_cs);
	return object;
}

ITEM * CAtumDatabase::GetServerItemInfo(int nItemNum)
{
	FLOG("CAtumDatabase::GetServerItemInfo(int nItemNum)");
	ITEM * item = NULL;
	if(nItemNum)
	{
		CMapItemInfoIterator itItem = m_mapItemInfoTemp.end();
		EnterCriticalSection(&g_pD3dApp->m_cs);
		CMapIsLoadingIterator itInfo = m_mapItemGetInfo.find(nItemNum);
		if(itInfo == m_mapItemGetInfo.end())
		{// 정보 요청 정보가 없을때 - 정보요청 정보를 보관하고 아이템 정보요청을 한다.(임시 정보를 꺼낸다)
			// 임시 정보 
			itItem = m_mapItemInfoTemp.find(nItemNum);
			if(itItem != m_mapItemInfoTemp.end())
			{
				// 간단 아이템 정보 요청
				item = itItem->second;
				// 2006-06-15 by ispark, 일정시간에 패킷을 보내는 것으로 변경
//				MSG_FC_INFO_GET_SIMPLE_ITEM_INFO sMsg;
//				sMsg.ItemNum = nItemNum;
//				g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_SIMPLE_ITEM_INFO, (char*)&sMsg, sizeof(sMsg));
				SetIntDataByVector(g_pD3dApp->m_vecSimpleItemInfo, nItemNum);
			}
			else
			{
				// 아이템 전체 정보 요청
				// 2006-06-15 by ispark, 일정시간에 패킷을 보내는 것으로 변경
//				MSG_FC_INFO_GET_ITEM_INFO sMsg;
//				sMsg.ItemNum = nItemNum;
//				g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_ITEM_INFO, (char*)&sMsg, sizeof(sMsg));
				SetIntDataByVector(g_pD3dApp->m_vecItemInfo, nItemNum);
			}
			BOOL bGetInfo = FALSE;
			m_mapItemGetInfo[nItemNum] = bGetInfo;
		}
		else
		{// 정보 요청 정보가 있을때
			if(!itInfo->second)
			{
				// 임시 정보 
				itItem = m_mapItemInfoTemp.find(nItemNum);
				if(itItem != m_mapItemInfoTemp.end())
					item = itItem->second;
			}
			else
			{// 정보 요청없이 맵에서 정보를 꺼낸다.
				itItem = m_mapItemInfo.find(nItemNum);
				if(itItem != m_mapItemInfo.end())
					item = itItem->second;
			}
		}
		LeaveCriticalSection(&g_pD3dApp->m_cs);
	}
	return item;
}


ITEM * CAtumDatabase::UpdateItemInfo(ITEM &item)
{
	FLOG("CAtumDatabase::UpdateItemInfo(ITEM &item)");
	// 중량의 변화등은 영향을 주게 된다면 새로 셋팅되어야한다.
	ITEM * pInfo = NULL;
	ITEM  pInfoOld;
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapItemInfoIterator itItem = m_mapItemInfo.find(item.ItemNum);
	if(itItem != m_mapItemInfo.end())
	{
		memcpy(&pInfoOld,itItem->second,sizeof(ITEM));
		memcpy(itItem->second, &item, sizeof(ITEM) - SIZE_MAX_ITEM_DESCRIPTION);
		pInfo = itItem->second;
	}
	else
	{
		pInfo = new ITEM;
		memset(pInfo,0x00,sizeof(ITEM));
		memcpy(pInfo,&item,sizeof(ITEM) - SIZE_MAX_ITEM_DESCRIPTION);
		CMapItemInfoIterator itTemp = m_mapItemInfoTemp.find(pInfo->ItemNum);
		int nDescPoint = sizeof(ITEM)-SIZE_MAX_ITEM_DESCRIPTION;
		if(itTemp != m_mapItemInfoTemp.end())
		{
			memcpy( ((char*)pInfo)+nDescPoint, ((char*)itTemp->second)+nDescPoint, SIZE_MAX_ITEM_DESCRIPTION);
		}
		m_mapItemInfo[pInfo->ItemNum] = pInfo;
	}
	m_mapItemGetInfo[item.ItemNum] = TRUE;

	LeaveCriticalSection(&g_pD3dApp->m_cs);
	return pInfo;
}

ITEM * CAtumDatabase::UpdateSimpleItemInfo(MEX_ITEM_INFO &simpleInfo)
{
	CMapItemInfoIterator it = m_mapItemInfoTemp.find(simpleInfo.ItemNum);
	if(it != m_mapItemInfoTemp.end())
	{
		CMapItemInfoIterator it2 = m_mapItemInfo.find(simpleInfo.ItemNum);
		ITEM* pITEM = NULL;
		if(it2 == m_mapItemInfo.end())
		{
			pITEM = new ITEM;
			memcpy( pITEM, it->second, sizeof(ITEM) );
		}
		else
		{
			pITEM = it2->second;
		}
		pITEM->Kind = simpleInfo.Kind;
		pITEM->AbilityMin = simpleInfo.AbilityMin;
		pITEM->AbilityMax = simpleInfo.AbilityMax;
		pITEM->ReqUnitKind = simpleInfo.ReqUnitKind;
		pITEM->ReqMinLevel = simpleInfo.ReqMinLevel;
		pITEM->HitRate = simpleInfo.HitRate;
		pITEM->Defense = simpleInfo.Defense;
		pITEM->SpeedPenalty = simpleInfo.SpeedPenalty;
		pITEM->Range = simpleInfo.Range;
#ifdef SHOP_PRICES_PER_BUILDING_NPC
		pITEM->SellingPrice = simpleInfo.SellingPrice;
#else
		pITEM->Price = simpleInfo.Price;
#endif // SHOP_PRICES_PER_BUILDING_NPC

		
		// 2009-04-21 by bhsohn 아이템 DesParam추가
// 		pITEM->DestParameter1 = simpleInfo.DestParameter1;
// 		pITEM->ParameterValue1 = simpleInfo.ParameterValue1;
// 		pITEM->DestParameter2 = simpleInfo.DestParameter2;
// 		pITEM->ParameterValue2 = simpleInfo.ParameterValue2;
// 		pITEM->DestParameter3 = simpleInfo.DestParameter3;
// 		pITEM->ParameterValue3 = simpleInfo.ParameterValue3;
// 		pITEM->DestParameter4 = simpleInfo.DestParameter4;
// 		pITEM->ParameterValue4 = simpleInfo.ParameterValue4;
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			pITEM->ArrDestParameter[nArrParamCnt] = simpleInfo.ArrDestParameter[nArrParamCnt];
			pITEM->ArrParameterValue[nArrParamCnt] = simpleInfo.ArrParameterValue[nArrParamCnt];

		}		
		// end 2009-04-21 by bhsohn 아이템 DesParam추가
		pITEM->ReAttacktime = simpleInfo.ReAttacktime;
		pITEM->Time = simpleInfo.Time;
		pITEM->RepeatTime = simpleInfo.RepeatTime;						// 2006-12-08 by ispark
		pITEM->RangeAngle = simpleInfo.RangeAngle;
		pITEM->MultiTarget = simpleInfo.MultiTarget;
		pITEM->ExplosionRange = simpleInfo.ExplosionRange;
		pITEM->ReactionRange = simpleInfo.ReactionRange;
		pITEM->ShotNum = simpleInfo.ShotNum;
		pITEM->MultiNum = simpleInfo.MultiNum;
		pITEM->AttackTime = simpleInfo.AttackTime;
		pITEM->ReqSP = simpleInfo.ReqSP;
		pITEM->OrbitType = simpleInfo.OrbitType;
		pITEM->ItemAttribute = simpleInfo.ItemAttribute;
		pITEM->BoosterAngle = simpleInfo.BoosterAngle;
		m_mapItemInfo[pITEM->ItemNum] = pITEM;
		m_mapItemGetInfo[pITEM->ItemNum] = TRUE;
	}
	return NULL;
}

RARE_ITEM_INFO * CAtumDatabase::GetServerRareItemInfo(int nCodeNum)
{
	FLOG("CAtumDatabase::GetServerRareItemInfo(int nItemNum)");
	RARE_ITEM_INFO * pRareItem = NULL;
	if(nCodeNum)
	{
		CMapRareItemInfoIterator itRareItem;
		EnterCriticalSection(&g_pD3dApp->m_cs);
		CMapIsLoadingIterator itGetInfo = m_mapRareItemGetInfo.find(nCodeNum);
		if(itGetInfo == m_mapRareItemGetInfo.end())
		{
			MSG_FC_INFO_GET_RARE_ITEM_INFO sMsg;
			sMsg.CodeNum = nCodeNum;
			g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_RARE_ITEM_INFO, (char*)&sMsg, sizeof(sMsg));
			BOOL bGetInfo = FALSE;
			m_mapRareItemGetInfo[nCodeNum] = bGetInfo;
			itRareItem = m_mapRareItemInfoTemp.find(nCodeNum);
			if(itRareItem != m_mapRareItemInfoTemp.end())
			{
				pRareItem = itRareItem->second;
			}
		}
		else
		{
			if(!itGetInfo->second)
			{
#ifdef _DEBUG
//				MSG_FC_INFO_GET_RARE_ITEM_INFO sMsg;
//				sMsg.CodeNum = nCodeNum;
//				g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_RARE_ITEM_INFO, (char*)&sMsg, sizeof(sMsg));
//				DBGOUT("ERROR : CAtumDatabase::GetServerRareItemInfo(int nCodeNum) : nCodeNum[%d], 레어 아이템 정보 테이블에 있는데 it->second가 NULL이다.\n",nCodeNum);
//				BOOL bGetInfo = FALSE;
//				m_mapItemGetInfo[nCodeNum] = bGetInfo;
#endif // _DEBUG_endif
				itRareItem = m_mapRareItemInfoTemp.find(nCodeNum);
				if(itRareItem != m_mapRareItemInfoTemp.end())
				{
					pRareItem = itRareItem->second;
				}
			}
			else
			{
				itRareItem = m_mapRareItemInfo.find(nCodeNum);
				if(itRareItem != m_mapRareItemInfo.end())
				{
					pRareItem = itRareItem->second;
				}
			}
		}
		LeaveCriticalSection(&g_pD3dApp->m_cs);
	}
	return pRareItem;
}
// 2010. 04. 21 by jskim 신규 럭키 머신 구현
CMapRareItemInfoList CAtumDatabase::GetServerRareItemList()
{
	return	m_mapRareItemInfoTemp;
}
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDatabase::GetItemNameRareString(char* pOutFullName, int nItemNumber, int nPrefix, int nSuffix)
/// \brief		buf에 rare 스트링을 추가해준다.
/// \author		dgwoo
/// \date		2006-08-11 ~ 2006-08-11
/// \warning	
///
/// \param		buf:기본 무기 이름 , prefix:접두사 , suffix:접미사
/// \return		0 : 기본 아템 1 : 접두사 2 : 접미사 3 : 접두사,접미사.
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetItemNameRareString(char* pOutFullName, int nItemNumber, int nPrefix, int nSuffix)
{
	int type = 0;
	char strPre[ITEMINFO_ITEM_FULL_NAME];
	char strSuf[ITEMINFO_ITEM_FULL_NAME];
	memset(strPre,0x00,ITEMINFO_ITEM_FULL_NAME);
	memset(strSuf,0x00,ITEMINFO_ITEM_FULL_NAME);

	ITEM* item = GetServerItemInfo(nItemNumber);

	if(item)
	{
		if(nPrefix > 0)
		{//접두사스트링 추가.
			RARE_ITEM_INFO* pre = GetServerRareItemInfo(nPrefix);
			if(pre)
			{
				sprintf(strPre,"%s ", pre->Name);
				type = 1;
			}
		}
		if(nSuffix > 0)
		{//접미사스트링 추가.
			RARE_ITEM_INFO* suf = GetServerRareItemInfo(nSuffix);
			if(suf)
			{
				sprintf(strSuf," %s", suf->Name);
				if(type)
				{
					type = 3;
				}
				type = 2;
			}
		}
		sprintf(pOutFullName,"%s%s%s", strPre,item->ItemName,strSuf);
	}
	
	return type;
}


RARE_ITEM_INFO * CAtumDatabase::UpdateRareItemInfo(RARE_ITEM_INFO &rareItem)
{
	FLOG("CAtumDatabase::UpdateItemInfo(ITEM &item)");
	// 중량의 변화등은 영향을 주게 된다면 새로 셋팅되어야한다.
	RARE_ITEM_INFO * pRareItem = NULL;
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapRareItemInfoIterator itRareItem = m_mapRareItemInfo.find(rareItem.CodeNum);
	if( itRareItem != m_mapRareItemInfo.end())
	{
		memcpy( itRareItem->second, &rareItem, sizeof(RARE_ITEM_INFO) );
		pRareItem = itRareItem->second;
	}
	else
	{
		pRareItem = new RARE_ITEM_INFO;
		memcpy(pRareItem, &rareItem, sizeof(RARE_ITEM_INFO));
		m_mapRareItemInfo[pRareItem->CodeNum] = pRareItem;
	}
	m_mapRareItemGetInfo[rareItem.CodeNum] = TRUE;
	LeaveCriticalSection(&g_pD3dApp->m_cs);
	return pRareItem;
}


MEX_MONSTER_INFO * CAtumDatabase::UpdateMonsterInfo(MEX_MONSTER_INFO *pMonster)
{
	FLOG("CAtumDatabase::UpdateMonsterInfo(MEX_MONSTER_INFO *pMonster)");
	MEX_MONSTER_INFO * pInfo = NULL;
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapMonsterInfoIterator itMon = m_mapMonsterInfo.find(pMonster->MonsterUnitKind);
	if(itMon != m_mapMonsterInfo.end())
	{
		memcpy(itMon->second, pMonster, sizeof(MEX_MONSTER_INFO));
		pInfo = itMon->second;
	}
	else
	{
		pInfo = new MEX_MONSTER_INFO;
		memcpy(pInfo,pMonster,sizeof(MEX_MONSTER_INFO));
		m_mapMonsterInfo[pInfo->MonsterUnitKind] = pInfo;
	}
	CMapMonsterIterator itMonster = g_pD3dApp->m_pScene->m_mapMonsterList.begin();
	while(itMonster != g_pD3dApp->m_pScene->m_mapMonsterList.end())
	{
		if(itMonster->second->m_info.MonsterUnitKind == pInfo->MonsterUnitKind)
			itMonster->second->Init();
		itMonster++;
	}
	m_mapMonsterGetInfo[pMonster->MonsterUnitKind] = TRUE;
	LeaveCriticalSection(&g_pD3dApp->m_cs);
	return pInfo;
}

MAPOBJECTINFO* CAtumDatabase::UpdateObjectInfo(MAPOBJECTINFO &obj)
{
	FLOG("CAtumDatabase::UpdateObjectInfo(MAPOBJECTINFO &obj)");
	MAPOBJECTINFO * pInfo  = NULL;
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapObjectInfoIterator itObj = m_mapObjectInfo.find(obj.Code);
	if(itObj != m_mapObjectInfo.end())
	{
		memcpy(itObj->second, &obj, sizeof(MAPOBJECTINFO));
		pInfo = itObj->second;
	}
	else
	{
		pInfo = new MAPOBJECTINFO;
		memcpy(pInfo,&obj,sizeof(MAPOBJECTINFO));
		m_mapObjectInfo[pInfo->Code] = pInfo;
	}
	m_mapObjectGetInfo[obj.Code] = TRUE;
	LeaveCriticalSection(&g_pD3dApp->m_cs);
	return pInfo;
}

BUILDINGNPC * CAtumDatabase::GetServerBuildingNPCInfoFromNPCIndex(int nNPCIndex)
{
	CMapBuildingNPCIterator it = m_mapBuildingNPCInfoTemp.begin();
	while(it != m_mapBuildingNPCInfoTemp.end())
	{
		if((it->second)->NPCIndex == nNPCIndex)
		{
			return it->second;
		}
		it++;
	}
	it = m_mapBuildingNPCInfo.begin();
	while(it != m_mapBuildingNPCInfo.end())
	{
		if((it->second)->NPCIndex == nNPCIndex)
		{
			return it->second;
		}
		it++;
	}
	return NULL;
}

void CAtumDatabase::FindBuildingNPCListOccupyShop(int nCityMapIndex, vector<BUILDINGNPC*> *pVecBuildingNPC)
{	
	CMapBuildingNPCIterator it = m_mapBuildingNPCInfoTemp.begin();
	while(it != m_mapBuildingNPCInfoTemp.end())
	{
		if((it->second)->OwnerCityMapIndex == nCityMapIndex)
		{
			BOOL bExist = FALSE;
			for(int i=0; i < pVecBuildingNPC->size(); i++)
			{
				if((*pVecBuildingNPC)[i]->MapIndex == it->second->MapIndex)
				{
					bExist = TRUE;
					break;
				}
			}
			if(FALSE == bExist)
			{
				pVecBuildingNPC->push_back(it->second);
			}
		}
		it++;
	}
	it = m_mapBuildingNPCInfo.begin();
	while(it != m_mapBuildingNPCInfo.end())
	{
		if((it->second)->OwnerCityMapIndex == nCityMapIndex)
		{
			BOOL bExist = FALSE;
			for(int i=0; i < pVecBuildingNPC->size(); i++)
			{
				if((*pVecBuildingNPC)[i]->MapIndex == it->second->MapIndex)
				{
					bExist = TRUE;
					break;
				}
			}
			if(FALSE == bExist)
			{
				pVecBuildingNPC->push_back(it->second);
			}
		}
		it++;
	}
}

BUILDINGNPC * CAtumDatabase::GetServerBuildingNPCInfo(int nBuildingIndex)
{
	BUILDINGNPC * pBuildingNPC = NULL;
	if(nBuildingIndex)
	{
		CMapBuildingNPCIterator itBuildingNPC;
		EnterCriticalSection(&g_pD3dApp->m_cs);
		CMapIsLoadingIterator itGetInfo = m_mapBuildingNPCGetInfo.find(nBuildingIndex);
		if(itGetInfo == m_mapBuildingNPCGetInfo.end())
		{
			DBGOUT("GetServerBuildingNPCInfo() : Rrequest BuildingInfo(%d) To Server\n", nBuildingIndex);
			MSG_FC_INFO_GET_BUILDINGNPC_INFO sMsg;
			sMsg.BuildingIndex = nBuildingIndex;
			g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_BUILDINGNPC_INFO, (char*)&sMsg, sizeof(sMsg));
			BOOL bGetInfo = FALSE;
			m_mapBuildingNPCGetInfo[nBuildingIndex] = bGetInfo;
			itBuildingNPC = m_mapBuildingNPCInfoTemp.find(nBuildingIndex);
			if(itBuildingNPC != m_mapBuildingNPCInfoTemp.end())
			{
				pBuildingNPC = itBuildingNPC->second;
			}
		}
		else
		{
			if(!itGetInfo->second)
			{
#ifdef _DEBUG
//				MSG_FC_INFO_GET_BUILDINGNPC_INFO sMsg;
//				sMsg.BuildingIndex = nBuildingIndex;
//				g_pFieldWinSocket->SendMsg(T_FC_INFO_GET_BUILDINGNPC_INFO, (char*)&sMsg, sizeof(sMsg));
//				DBGOUT("ERROR : CAtumDatabase::GetServerBuildingNPCInfo(int nBuildingIndex) : nBuildingIndex[%d], 빌딩NPC 정보 테이블에 있는데 it->second가 NULL이다.\n",nBuildingIndex);
//				BOOL bGetInfo = FALSE;
//				m_mapBuildingNPCGetInfo[nBuildingIndex] = bGetInfo;
#endif // _DEBUG_endif
				itBuildingNPC = m_mapBuildingNPCInfoTemp.find(nBuildingIndex);
				if(itBuildingNPC != m_mapBuildingNPCInfoTemp.end())
				{
					pBuildingNPC = itBuildingNPC->second;
				}
			}
			else
			{
				itBuildingNPC = m_mapBuildingNPCInfo.find(nBuildingIndex);
				if(itBuildingNPC != m_mapBuildingNPCInfo.end())
				{
					pBuildingNPC = itBuildingNPC->second;
				}
			}
		}
		LeaveCriticalSection(&g_pD3dApp->m_cs);
	}
	return pBuildingNPC;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BUILDINGNPC * CAtumDatabase::GetBuildingNPCInfo(int nBuildingIndex)
{
	BUILDINGNPC * pBuildingNPC = NULL;
	CMapBuildingNPCIterator itBuildingNPC;
	itBuildingNPC = m_mapBuildingNPCInfoTemp.find(nBuildingIndex);
	if(itBuildingNPC != m_mapBuildingNPCInfoTemp.end())
	{
		pBuildingNPC = itBuildingNPC->second;
	}
	return pBuildingNPC;
}


BUILDINGNPC * CAtumDatabase::UpdateBuildingNPCInfo(BUILDINGNPC & buildingInfo)
{
	BUILDINGNPC * pInfo  = NULL;
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapBuildingNPCIterator itBuildingNPC = m_mapBuildingNPCInfo.find(buildingInfo.BuildingIndex);
	if(itBuildingNPC != m_mapBuildingNPCInfo.end())
	{
		memcpy(itBuildingNPC->second, &buildingInfo, sizeof(BUILDINGNPC));
		pInfo = itBuildingNPC->second;
	}
	else
	{
		pInfo = new BUILDINGNPC;
		memcpy(pInfo,&buildingInfo,sizeof(BUILDINGNPC));
		m_mapBuildingNPCInfo[pInfo->BuildingIndex] = pInfo;
	}
	m_mapBuildingNPCGetInfo[buildingInfo.BuildingIndex] = TRUE;
	LeaveCriticalSection(&g_pD3dApp->m_cs);
	return pInfo;

}

void CAtumDatabase::ReloadDatabase()
{
	FLOG("CAtumDatabase::ReloadDatabase()");
	CMapIsLoadingIterator itInfo = m_mapItemGetInfo.begin();
	while(itInfo != m_mapItemGetInfo.end())
	{
		itInfo->second = FALSE;
		itInfo++;
	}
	itInfo = m_mapMonsterGetInfo.begin();
	while(itInfo != m_mapMonsterGetInfo.end())
	{
		itInfo->second = FALSE;
		itInfo++;
	}
	itInfo = m_mapObjectGetInfo.begin();
	while(itInfo != m_mapObjectGetInfo.end())
	{
		itInfo->second = FALSE;
		itInfo++;
	}
}

CINFImage* CAtumDatabase::GetGuildMark(UINT nGuildUniqueNumber)
{
	CMapGuildMarkIterator it = m_mapGuildMark.find( nGuildUniqueNumber );
	if( it != m_mapGuildMark.end())
	{
		ASSERT_ASSERT(it->second->pImage);
		return it->second->pImage;
	}
	else
	{
		CMapIsLoadingIterator it2  = m_mapGuildMarkGetInfo.find(nGuildUniqueNumber);
		if( it2 == m_mapGuildMarkGetInfo.end())
		{
			MSG_IC_GUILD_GET_GUILD_MARK sMsg;
			sMsg.GuildUniqueNumber = nGuildUniqueNumber;
			g_pIMSocket->SendMsg( T_IC_GUILD_GET_GUILD_MARK, (char*)&sMsg, sizeof(sMsg));
			m_mapGuildMarkGetInfo[nGuildUniqueNumber] = TRUE;
		}
	}
	return NULL;
}

char* CAtumDatabase::GetGuildName(UINT nGuildUniqueNumber)
{
	// first we check the map with the guild info
	auto it = m_mapOtherGuildInfo.find(nGuildUniqueNumber);
	
	// if we didnt find the unique number we are looking for
	if (it == m_mapOtherGuildInfo.end())
	{

		// apparently m_mapOtherGuildInfoGetInfo is always gonna be empty
		// it looks like this functionality has not been implemented yet?
		auto it2  = m_mapOtherGuildInfoGetInfo.find(nGuildUniqueNumber);

		// if not, which should be always
		if (it2 == m_mapOtherGuildInfoGetInfo.end())
		{
			// set up a message to send to the IM server
			// asking for the info
			MSG_IC_GUILD_GET_OTHER_GUILD_INFO sMsg;
			sMsg.GuildUniqueNumber = nGuildUniqueNumber;
			g_pIMSocket->SendMsg( T_IC_GUILD_GET_OTHER_GUILD_INFO, (char*)&sMsg, sizeof(sMsg) );
		}

		// otherwise return "Unkown" but this never happens in practice
		return "Unknown";
	}

	// if we found the info in the map, just return the name
	return it->second->GuildName;
}
/*
void CAtumDatabase::UpdateGuildMark(UINT nGuildUniqueNumber, char* pData, int nSize)
{
	CMapGuildMarkIterator it = m_mapGuildMark.find( nGuildUniqueNumber );
	if( it != m_mapGuildMark.end())
	{
		it->second->InvalidateDeviceObjects();
		it->second->DeleteDeviceObjects();
		util::del(it->second);
	}
	CINFImage* pImage = new CINFImage;
	pImage->InitDeviceObjects( pData, nSize );
	pImage->RestoreDeviceObjects();
	pImage->SetRect( 0,0,24,12);
	m_mapGuildMark[nGuildUniqueNumber] = pImage;
}
*/
void CAtumDatabase::UpdateGuildMark( UINT nGuildUniqueNumber, int nGuildVersion,char* pData, int nSize)
{
	GUILD_MARK_VERSION *pGuildMark;
	CMapGuildMarkIterator it = m_mapGuildMark.find( nGuildUniqueNumber );
	if( it != m_mapGuildMark.end())
	{
		pGuildMark = it->second;
		pGuildMark->pImage->InvalidateDeviceObjects();
		pGuildMark->pImage->DeleteDeviceObjects();
		util::del(pGuildMark->pImage);
		util::del(pGuildMark->pData);
		m_mapGuildMark.erase(it);
	}
	else
	{
		pGuildMark = new GUILD_MARK_VERSION;
		memset( pGuildMark, 0x00, sizeof(GUILD_MARK_VERSION) );
	}
	pGuildMark->nGuildUniqueNumber = nGuildUniqueNumber;
	pGuildMark->nSizeOfImage = nSize;
	pGuildMark->nVersion = nGuildVersion;
	// 2007-08-03 by bhsohn 길드 마크 심사 시스템 구현	
	if(0 == nSize)
	{	
		pGuildMark->pData = NULL;
		pGuildMark->pImage = new CINFImage;				// 2011. 10. 10 by jskim UI시스템 변경
	}
	else
	{
		pGuildMark->pData = new char[nSize];
		memcpy( pGuildMark->pData, pData, nSize );
// 2011. 10. 10 by jskim UI시스템 변경
		pGuildMark->pImage = new CINFImage;		

		DataHeader* pHeader = new DataHeader;
		pHeader->m_pData = pGuildMark->pData;
		pHeader->m_DataSize = nSize;

		pGuildMark->pImage->InitDeviceObjects(  pHeader->m_pData ,pHeader->m_DataSize );
// end 2011. 10. 10 by jskim UI시스템 변경
		
		pGuildMark->pImage->RestoreDeviceObjects();
		pGuildMark->pImage->SetRect( 0,0,24,12);
	}
	// end 2007-08-03 by bhsohn 길드 마크 심사 시스템 구현	
	
	m_mapGuildMark[nGuildUniqueNumber] = pGuildMark;

	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CheckGuildMarkVersion(int nGuildUniqueNumber, int nGuildVersion )
/// \brief		저장된 길드마크의 버전이 예전 버전인지 확인하고 서버로 길드마크 요청
/// \author		dhkwon
/// \date		2004-06-01 ~ 2004-06-01
/// \warning	게임시작시 길드마크 요청부분, 타캐릭터 정보 요청 후 수행
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::CheckGuildMarkVersion(int nGuildUniqueNumber, int nGuildVersion )
{
	GUILD_MARK_VERSION *pGuildMark;
	CMapGuildMarkIterator it = m_mapGuildMark.find( nGuildUniqueNumber );
	if( it != m_mapGuildMark.end())
	{
		pGuildMark = it->second;
		if(pGuildMark->nVersion != nGuildVersion)
		{
			MSG_IC_GUILD_GET_GUILD_MARK	sMsg;
			sMsg.GuildUniqueNumber = nGuildUniqueNumber;
			g_pIMSocket->SendMsg( T_IC_GUILD_GET_GUILD_MARK, (char*)&sMsg, sizeof(sMsg));
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			MEX_OTHER_GUILD_INFO* CAtumDatabase::GetOtherGuildInfo( int nGuildUniqueNumber )
/// \brief		다른 길드 정보 요청
/// \author		dhkwon
/// \date		2004-06-01 ~ 2004-06-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MEX_OTHER_GUILD_INFO* CAtumDatabase::GetOtherGuildInfo( int nGuildUniqueNumber )
{
	CMapOtherGuildInfoIterator it = m_mapOtherGuildInfo.find(nGuildUniqueNumber);
	if( it == m_mapOtherGuildInfo.end())
	{
		CMapIsLoadingIterator it2  = m_mapOtherGuildInfoGetInfo.find(nGuildUniqueNumber);
		if( it2 == m_mapOtherGuildInfoGetInfo.end())
		{
			MSG_IC_GUILD_GET_OTHER_GUILD_INFO sMsg;
			sMsg.GuildUniqueNumber = nGuildUniqueNumber;
			g_pIMSocket->SendMsg( T_IC_GUILD_GET_OTHER_GUILD_INFO, (char*)&sMsg, sizeof(sMsg) );
		}
		return NULL;
	}
	return it->second;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumDatabase::UpdateOtherGuildInfo(MEX_OTHER_GUILD_INFO* pMsg)
/// \brief		다른 길드 정보 업데이트
/// \author		dhkwon
/// \date		2004-06-01 ~ 2004-06-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::UpdateOtherGuildInfo(MEX_OTHER_GUILD_INFO* pMsg)
{
	// first we check whether we already have this data
	auto it = m_mapOtherGuildInfo.find(pMsg->GuildUniqueNumber);

	// if yes, then delete it
	if (it != m_mapOtherGuildInfo.end()) util::del(it->second);
	
	// copy the data to another newly created buffer
	auto pOther = new MEX_OTHER_GUILD_INFO { *pMsg };

	// store the pointer to this buffer in the cache map
	m_mapOtherGuildInfo[pMsg->GuildUniqueNumber] = pOther;

	CheckGuildMarkVersion(pMsg->GuildUniqueNumber, pMsg->GuildMarkVersion );
}

MAP_INFO * CAtumDatabase::GetMapInfo(MapIndex_t nMapIndex)
{
	CMapMapInfoIterator it = m_mapMapInfo.find((USHORT)nMapIndex);
	if( it != m_mapMapInfo.end())
	{
		return it->second;
	}


	// 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기

// 	// 2006-07-04 by ispark, 맵 정보가 없을 때
// 	DBGOUT("\n== WARNING == Not MapInfo (%d)\n\n", nMapIndex);
// 	g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
// 
// 	return NULL;

	return &m_DefMapInfo;

	// end 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기
}
void CAtumDatabase::FindMysteryInfo(INT i_nItemNum,vector<MYSTERY_ITEM_DROP*> *pVecItem)
{
	CVectorMysteryItemDropIterator it = m_VecMysteryItem.begin();
	while(it != m_VecMysteryItem.end())
	{
		if((*it)->MysteryItemDropNum == i_nItemNum)
		{
			pVecItem->push_back(*it);
			
		}
		it++;
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		미스테리 아이템 정보를 얻어온다.
/// \author		// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MYSTERY_ITEM_DROP* CAtumDatabase::GetMysteryInfo(INT	i_MysteryItemDropNum, INT i_nItemNum)
{
	CVectorMysteryItemDropIterator it = m_VecMysteryItem.begin();
	while(it != m_VecMysteryItem.end())
	{		
		if(((*it)->MysteryItemDropNum == i_MysteryItemDropNum)
			&&((*it)->DropItemNum == i_nItemNum))
		{
			return (*it);			
			
		}
		it++;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumDatabase::GetServerItemInfo(vectorItemInfoList *pItemList, int nKind)
/// \brief		아이템 종류로 모든 아이템 정보 가져오기
/// \author		ispark
/// \date		2006-07-31 ~ 2006-07-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::GetServerItemInfo(vectorItemInfoList *pItemList, int nKind)
{
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapItemInfoIterator itInfo = m_mapItemInfoTemp.begin();
	while(itInfo != m_mapItemInfoTemp.end())
	{// 정보 요청 정보가 있을때
		if(itInfo->second->Kind == nKind)
		{
			pItemList->push_back(itInfo->second);
		}
		itInfo++;
	}
	LeaveCriticalSection(&g_pD3dApp->m_cs);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-12-17 by bhsohn 조합 가격 표시
/// \date		2007-12-17 ~ 2007-12-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_MIXING_INFO* CAtumDatabase::GetMixerPrice(ITEM_MIXING_INFO* pMixInfo)
{
	CVectorMixingInfoIterator it = m_VecMixingInfo.begin();
	int nPrice = -1;	

	// 2013-03-18 by bhsohn 팩토리 조합 추가
	vector<ITEM_W_COUNT> vecSourceItem;	
	int nCnt = 0;
	for(nCnt = 0;nCnt < COUNT_ITEM_MIXING_SOURCE; nCnt++)
	{
		vecSourceItem.push_back(pMixInfo->SourceItem[nCnt]);		
	}
	sort(vecSourceItem.begin(), vecSourceItem.end(), ITEM_NUM_SORT());
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
	
	while(it != m_VecMixingInfo.end())
	{
		int nEqualCnt = 0;// 같은 갯수
		ITEM_MIXING_INFO* pMixingInfo = *it;

		if(pMixingInfo->NumOfSourceItems != pMixInfo->NumOfSourceItems)
		{
			it++;
			continue;
		}	
		
		int nSrcCnt = 0;
		int nDstCnt = 0;		

		// 2013-03-18 by bhsohn 팩토리 조합 추가
// 		for(nDstCnt = 0; nDstCnt < pMixInfo->NumOfSourceItems; nDstCnt++)		
// 		{			
// 			for(nSrcCnt = 0;nSrcCnt< pMixingInfo->NumOfSourceItems;nSrcCnt++)			
// 			{				
// 				if((pMixingInfo->SourceItem[nSrcCnt].ItemNum == pMixInfo->SourceItem[nDstCnt].ItemNum)
// 					&&(pMixingInfo->SourceItem[nSrcCnt].Count == pMixInfo->SourceItem[nDstCnt].Count))				
// 				{
// 					nEqualCnt++;
// 				}
// 			}
// 		}				
		vector<ITEM_W_COUNT> vecTargetItem;	
		for(nCnt = 0;nCnt < COUNT_ITEM_MIXING_SOURCE; nCnt++)
		{
			vecTargetItem.push_back(pMixingInfo->SourceItem[nCnt]);			
		}	
		sort(vecTargetItem.begin(), vecTargetItem.end(), ITEM_NUM_SORT());
		if((COUNT_ITEM_MIXING_SOURCE == vecSourceItem.size())
			&& (COUNT_ITEM_MIXING_SOURCE == vecTargetItem.size()))
		{
			for(nCnt = 0;nCnt < COUNT_ITEM_MIXING_SOURCE; nCnt++)
			{
				ITEM_W_COUNT stSource = vecSourceItem[nCnt];
				ITEM_W_COUNT stTarget = vecTargetItem[nCnt];
				if((stSource.ItemNum != 0)
					&& (stSource.ItemNum == stTarget.ItemNum)
 					&&(stSource.Count == stTarget.Count))
				{
					nEqualCnt++;
				}
			}
		}

		// END 2013-03-18 by bhsohn 팩토리 조합 추가
		if(nEqualCnt == pMixInfo->NumOfSourceItems)
		{
			return pMixingInfo;			
		}
		it++;
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDatabase::LoadLuckyMachineInfo(FILE* fd, int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		LUCKY_MACHINE_OMI * pMystery = new LUCKY_MACHINE_OMI;
		
		if(fread(pMystery, sizeof(LUCKY_MACHINE_OMI), 1, fd) == 0)
		{
			util::del(pMystery);
			return FALSE;
		}

#ifdef CIPHER2015
		CipherRA((char*)pMystery, sizeof(LUCKY_MACHINE_OMI), pos + i * sizeof(LUCKY_MACHINE_OMI));
#endif

		m_VecLuckyMachine.push_back(pMystery);
		//DBGOUT("pMystery->DropItemNum %3d : DropItemNum(%d) MysteryItemDropNum(%d)(%x)\n",i,pMystery->DropItemNum,pMystery->MysteryItemDropNum,pMystery);
	}
	return TRUE;
}

// 2010. 02. 11 by ckPark 발동류 장착아이템
BOOL	CAtumDatabase::LoadInvokingWearItemDPNum( FILE* fd, int nCount )
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		INVOKING_WEAR_ITEM_DESTPARAM*	pDPNum = new INVOKING_WEAR_ITEM_DESTPARAM;
		if( fread( pDPNum, sizeof( INVOKING_WEAR_ITEM_DESTPARAM ), 1, fd ) == 0)
		{
			util::del( pDPNum );
			return FALSE;
		}

#ifdef CIPHER2015
		CipherRA((char*)pDPNum, sizeof(INVOKING_WEAR_ITEM_DESTPARAM), pos + i * sizeof(INVOKING_WEAR_ITEM_DESTPARAM));
#endif

		m_VecInvokingWearItemDPNum.push_back( pDPNum );
	}
	return TRUE;
}

BOOL	CAtumDatabase::LoadInvokingWearItemDPNumByUse( FILE* fd, int nCount )
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		INVOKING_WEAR_ITEM_DESTPARAM*	pDPNum = new INVOKING_WEAR_ITEM_DESTPARAM;
		
		if (fread(pDPNum, sizeof(INVOKING_WEAR_ITEM_DESTPARAM), 1, fd) == 0)
		{
			util::del(pDPNum);
			return FALSE;
		}

#ifdef CIPHER2015
		CipherRA((char*)pDPNum, sizeof(INVOKING_WEAR_ITEM_DESTPARAM), pos + i * sizeof(INVOKING_WEAR_ITEM_DESTPARAM));
#endif

		m_VecInvokingWearItemDPNumByUse.push_back(pDPNum);
	}
	return TRUE;
}
// end 2010. 02. 11 by ckPark 발동류 장착아이템


// 2010-06-15 by shcho&hslee 펫시스템
/**************************************************************
**
**	펫 기본 정보 파일 읽기.
**
**	Create Info : 2010-06-15 by shcho&hslee
**
***************************************************************/
BOOL CAtumDatabase :: LoadPetBaseData ( FILE* fd , int nCount )
{

	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		tPET_BASE_ALL_DATA *pPetBaseAllData = new tPET_BASE_ALL_DATA;

		if (fread(&pPetBaseAllData->BaseData, sizeof(tPET_BASEDATA), 1, fd) == 0)
		{
			util::del( pPetBaseAllData );
			return FALSE;
		}

#ifdef CIPHER2015
		CipherRA((char*)&pPetBaseAllData->BaseData, sizeof(tPET_BASEDATA), pos + i * sizeof(tPET_BASEDATA));
#endif

		m_VecPetAllData.push_back( pPetBaseAllData );
	}

	return TRUE;
}


/**************************************************************
**
**	펫 레벨업 정보 파일 읽기.
**
**	Create Info : 2010-06-15 by shcho&hslee
**
***************************************************************/
BOOL CAtumDatabase :: LoadPetLevelData ( FILE* fd , int nCount )
{
	ASSERT_ASSERT(fd);

#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	tPET_BASE_ALL_DATA *pPetBaseAllData = NULL;

	tPET_LEVEL_DATA sNewPetLevelData;

	for (int i = 0; i < nCount; i++)
	{
		memset(&sNewPetLevelData , 0x00, sizeof(tPET_LEVEL_DATA));

		if (fread(&sNewPetLevelData, sizeof(tPET_LEVEL_DATA), 1, fd) == 0) return FALSE;

#ifdef CIPHER2015
		CipherRA((char*)&sNewPetLevelData, sizeof(tPET_LEVEL_DATA), pos + i * sizeof(tPET_LEVEL_DATA));
#endif

		pPetBaseAllData = GetPtr_PetAllData(sNewPetLevelData.PetIndex);	 //2011-10-06 by jhahn 파트너 성장형 시스템

		if (NULL == pPetBaseAllData) return FALSE;
		
		pPetBaseAllData->vectPetLevelDataInfo.push_back(sNewPetLevelData);
	}
	
	return TRUE;

}
BOOL CAtumDatabase :: LoadBurningMapData ( FILE* fd , int nCount )
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		BURNING_MAP_INFO * pBurning = new BURNING_MAP_INFO;
		if (fread(pBurning, sizeof(BURNING_MAP_INFO), 1, fd) == 0)
		{
			util::del(pBurning);
			return FALSE;
		}

#ifdef CIPHER2015
		CipherRA((char*)pBurning, sizeof(BURNING_MAP_INFO), pos + i * sizeof(BURNING_MAP_INFO));
#endif

		m_VecBurningMapData.push_back(pBurning);
	}

	return TRUE;
	
}

tPET_BASE_ALL_DATA *CAtumDatabase :: GetPtr_PetAllData ( const INT a_PetUID )
{

	CVectorPetAllDataItr itr = m_VecPetAllData.begin();

	tPET_BASE_ALL_DATA *pPetBaseAllData = NULL;

	for ( ; itr != m_VecPetAllData.end(); ++itr )
	{
		
		pPetBaseAllData = (*itr);
		if ( NULL == pPetBaseAllData )
			continue;

		if ( pPetBaseAllData->BaseData.PetIndex == a_PetUID )			//2011-10-06 by jhahn 파트너 성장형 시스템
			return pPetBaseAllData;
	}

	return NULL;

}
BOOL CAtumDatabase::LoadOperatorData (FILE* fd , int nCount)
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for (int i = 0; i < nCount; i++)
	{
		OperatorInfo *pOperatorInfo = new OperatorInfo;

		if (fread(pOperatorInfo, sizeof(OperatorInfo), 1, fd) == 0)
		{
			util::del(pOperatorInfo);
			return FALSE;
		}

#ifdef CIPHER2015
		CipherRA((char*)pOperatorInfo, sizeof(OperatorInfo), pos + i * sizeof(OperatorInfo));
#endif

		m_vecOptratorAction.push_back(pOperatorInfo);
	}
	
	return TRUE;
}
// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
BOOL CAtumDatabase::LoadMultiTargetIndexData ( FILE* fd , int nCount )
{
	ASSERT_ASSERT(fd);
	
#ifdef CIPHER2015
	int pos = ftell(fd);
#endif

	for ( int i = 0; i < nCount; i++ )
	{
		MONSTER_MULTI_TARGET MonsterMultiTargetInfo;
		memset(&MonsterMultiTargetInfo , 0x00 , sizeof(MONSTER_MULTI_TARGET));
		if (fread(&MonsterMultiTargetInfo , sizeof(MONSTER_MULTI_TARGET) , 1 , fd) == 0) return FALSE;
		
#ifdef CIPHER2015
		CipherRA((char*)&MonsterMultiTargetInfo, sizeof(MONSTER_MULTI_TARGET), pos + i * sizeof(MONSTER_MULTI_TARGET));
#endif

		if (m_mapMultiTagetInfo[MonsterMultiTargetInfo.MonsterIndex].empty())
		{
			vectorMonsterMultiTarget vecMonsterMultiInfo;

			MONSTER_MULTI_TARGET TempMonsterMultiTargetInfo;
			TempMonsterMultiTargetInfo.MonsterIndex = MonsterMultiTargetInfo.MonsterIndex;
			TempMonsterMultiTargetInfo.PointIndex = 0;
			TempMonsterMultiTargetInfo.TargetPosition = D3DXVECTOR3(0, 0, 0);
			vecMonsterMultiInfo.push_back(TempMonsterMultiTargetInfo);

			// 2011. 06. 09 by jskim 멀티타겟 위치 변경
			//vecMonsterMultiInfo.push_back(MonsterMultiTargetInfo);
			MONSTER_MULTI_TARGET TempMonsterInfo;
			TempMonsterInfo = MonsterMultiTargetInfo;
			TempMonsterInfo.TargetPosition.x = MonsterMultiTargetInfo.TargetPosition.z;
			TempMonsterInfo.TargetPosition.z = -MonsterMultiTargetInfo.TargetPosition.x;
			vecMonsterMultiInfo.push_back(TempMonsterInfo);
			// end 2011. 06. 09 by jskim 멀티타겟 위치 변경
			
			m_mapMultiTagetInfo[MonsterMultiTargetInfo.MonsterIndex] = vecMonsterMultiInfo;
		}
		else
		{
			// 2011. 06. 09 by jskim 멀티타겟 위치 변경
			//m_mapMultiTagetInfo.find(MonsterMultiTargetInfo.MonsterIndex)->second.push_back( MonsterMultiTargetInfo );
			MONSTER_MULTI_TARGET TempMonsterInfo;
			TempMonsterInfo = MonsterMultiTargetInfo;
			TempMonsterInfo.TargetPosition.x = MonsterMultiTargetInfo.TargetPosition.z;
			TempMonsterInfo.TargetPosition.z = -MonsterMultiTargetInfo.TargetPosition.x;
			m_mapMultiTagetInfo.find(MonsterMultiTargetInfo.MonsterIndex)->second.push_back( TempMonsterInfo );
			// end 2011. 06. 09 by jskim 멀티타겟 위치 변경
		}
		
	}
	return TRUE;
}

int CAtumDatabase::GetMultiTargetIndexSize(int nMonsterIndex)
{
	return m_mapMultiTagetInfo.find(nMonsterIndex)->second.size();
}

MONSTER_MULTI_TARGET CAtumDatabase::GetMultiTargetPosion(int nMonsterIndex, int nPosion)
{
	vectoritMonsterMultiTarget itr = m_mapMultiTagetInfo.find(nMonsterIndex)->second.begin();
	MONSTER_MULTI_TARGET tempInfo;
	for( ; itr != m_mapMultiTagetInfo.find(nMonsterIndex)->second.end(); ++itr)
	{
		if((*itr).MonsterIndex == nMonsterIndex && (*itr).PointIndex == nPosion)
		{ 
			tempInfo = (*itr);
			return tempInfo;
		}
	}
	return tempInfo;
}

vectorMonsterMultiTarget CAtumDatabase::GetvecMultiTargetInfo(int nMonsterIndex)
{
	map<INT, vectorMonsterMultiTarget>::iterator multi = m_mapMultiTagetInfo.find(nMonsterIndex);
	if (multi != m_mapMultiTagetInfo.end())
	{
		return multi->second;
	}
	vectorMonsterMultiTarget vec;
	return vec;
}
// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템


/****************************************************************************************
**
**	펫 전체 정보 포인터 리턴. ( Pet Index 기준. ) Public.
**
**	Create Info : 2010. 07. 08	by hsLee.
**
*****************************************************************************************/
tPET_BASE_ALL_DATA *CAtumDatabase :: GetPtr_PetAllDataByIndex ( const INT a_PetIndex )
{

	CVectorPetAllDataItr itr = m_VecPetAllData.begin();

	tPET_BASE_ALL_DATA *pPetBaseAllData = NULL;

	for ( ; itr != m_VecPetAllData.end(); ++itr )
	{
		pPetBaseAllData = (*itr);
		if ( NULL == pPetBaseAllData )
			continue;

		if ( pPetBaseAllData->BaseData.PetIndex == a_PetIndex )
			return pPetBaseAllData;
	}

	return NULL;

}


// 
/****************************************************************************************
**
**	펫 레벨 정보 포인터 리턴. Public.
**
**	Create Info : 2010. 07. 09	by hsLee.
**
*****************************************************************************************/
tPET_LEVEL_DATA *CAtumDatabase :: GetPtr_PetLevelData ( const INT a_PetIndex , const INT a_PetLevel )
{
	tPET_BASE_ALL_DATA *psPetBaseAllData = GetPtr_PetAllDataByIndex ( a_PetIndex );

	if ( psPetBaseAllData )
		return psPetBaseAllData->rtn_LevelData ( a_PetLevel );

	return NULL;
}

/************************************************************
**
**	오퍼레이터 정보 가져오기.
**
**	Create Info :	2010-06-15 by shcho&hslee
**
*************************************************************/

OperatorInfo* CAtumDatabase :: GetPtr_OperatorAction( const INT FunctionIndex )
{
	vectorPreOperatorInfo::iterator it = m_vecOptratorAction.begin();
	
	while (it != m_vecOptratorAction.end())
	{
		if((*it)->FunctionIndex == FunctionIndex)
		{
			return (*it);
		}	
		it++;
	}
	return NULL;
}

/************************************************************
**
**	펫 기본 정보 메모리 해제.
**
**	Create Info :	2010-06-15 by shcho&hslee
**
*************************************************************/
void CAtumDatabase :: DeletePetData ( void )
{
	CVectorPetAllDataItr itPetBaseData = m_VecPetAllData.begin();
	
	while ( itPetBaseData != m_VecPetAllData.end() )
	{
		(*itPetBaseData)->vectPetLevelDataInfo.clear();

		util::del( (*itPetBaseData) );

		itPetBaseData++;
	}

	m_VecPetAllData.clear();
}

// End 2010-06-15 by shcho&hslee 펫시스템

// 2010-08-10 by dgwoo 버닝맵 시스템
BURNING_MAP_INFO *CAtumDatabase::GetPtr_BurningMapInfo(const MapIndex_t i_Index)
{
	CVectorBurningDataItr itBurning = m_VecBurningMapData.begin();
	BURNING_MAP_INFO *pBurning = NULL;
	while(itBurning != m_VecBurningMapData.end())
	{
		pBurning = (*itBurning);
		if(pBurning->MapIndex == i_Index)
			return pBurning;
		itBurning++;
	}
	return NULL;
}
// 2010-08-10 by dgwoo 버닝맵 시스템
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetLuckyMachineLen()
{
	return (int)m_VecLuckyMachine.size();

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
LUCKY_MACHINE_OMI * CAtumDatabase::GetLuckyMachineInfo(int nIdx)
{
	if((nIdx >= GetLuckyMachineLen())
		||(nIdx < 0))
	{
		return NULL;
	}

	return m_VecLuckyMachine[nIdx];

}


// 2010. 02. 11 by ckPark 발동류 장착아이템
void	CAtumDatabase::GetInvokingWearItemDPList( CVectorInvokingWearItemDP* pVec, InvokingDestParamID_t nIdx )
{
	for( CVectorInvokingWearItemDPIt it = m_VecInvokingWearItemDPNum.begin();
		 it != m_VecInvokingWearItemDPNum.end();
		 ++it )
	{
		if( (*it)->InvokingDestParamID == nIdx )
			pVec->push_back( (*it) );
	}
}

void	CAtumDatabase::GetInvokingWearItemDPByUseList( CVectorInvokingWearItemDP* pVec, InvokingDestParamID_t nIdx )
{
	for( CVectorInvokingWearItemDPIt it = m_VecInvokingWearItemDPNumByUse.begin();
		 it != m_VecInvokingWearItemDPNumByUse.end();
		 ++it )
	{
		if( (*it)->InvokingDestParamID == nIdx )
			pVec->push_back( (*it) );
	}
}
// end 2010. 02. 11 by ckPark 발동류 장착아이템


// 2009. 06. 09 by ckPark OMI 게임 시작시 체크섬 계산하도록 변경
void	CAtumDatabase::GetOMICheckSum( const char* szFilePath, BYTE omiCheckSum[32], long* pFileSize )
{
	memset(omiCheckSum, 0x00, 32);
	
	UINT uiCheckSum = 0;
	if(strlen(szFilePath) <=0)
	{
		return;
	}
	FILE *fp;
    fp=fopen(szFilePath, "rb");  
	if(NULL == fp)
	{
		return;
	}
	fseek( fp, 0L, SEEK_END );
	long lFileSize = ftell( fp );
	*pFileSize = lFileSize;			// 2007-05-28 by cmkwon
	fseek( fp, 0L, SEEK_SET );	

	BYTE *pFileData = new BYTE [lFileSize];
	memset(pFileData, 0x00, lFileSize);
	fread(pFileData, lFileSize, 1, fp);	

	sha256_encode(pFileData, lFileSize, omiCheckSum);
	
	fclose(fp);
	delete [] pFileData;
}
// end 2009. 06. 09 by ckPark OMI 게임 시작시 체크섬 계산하도록 변경

// 2010. 03. 18 by jskim 몬스터변신 카드
ITEM * CAtumDatabase::GetServerMonsterTransItemInfo(int nMonUintKind)
{
	EnterCriticalSection(&g_pD3dApp->m_cs);
	CMapItemInfoIterator itInfo = m_mapItemInfoTemp.begin();
	while(itInfo != m_mapItemInfoTemp.end())
	{// 정보 요청 정보가 있을때
		ITEM *pItemInfo = itInfo->second;

		if(pItemInfo->IsExistDesParam(DES_TRANSFORM_TO_MONSTER)
			&& nMonUintKind == (int)pItemInfo->GetParameterValue(DES_TRANSFORM_TO_MONSTER))
		{
			LeaveCriticalSection(&g_pD3dApp->m_cs);
			return pItemInfo;
		}
		itInfo++;
	}
	LeaveCriticalSection(&g_pD3dApp->m_cs);

	return NULL;
}
//end 2010. 03. 18 by jskim 몬스터변신 카드


/************************************************************
**
**	용해 아이템 정보 가져오기.
**
**	Create Info :	// 2010-08-31 by shcho&&jskim
**
*************************************************************/


BOOL CAtumDatabase::LoadDissolutionItemData( FILE* fd , int nCount )
{
	ASSERT_ASSERT(fd);

#ifdef CIPHER2015
	unsigned pos = ftell(fd);
#endif

	tDissolutionItemInfo TempDisItem;

	for ( int i = 0; i < nCount; i++ )
	{
		memset( &TempDisItem , 0x00 , sizeof( tDissolutionItemInfo ) );

		if ( fread ( &TempDisItem , sizeof( tDissolutionItemInfo ) , 1 , fd ) == 0 )
		{
			return FALSE;
		}

#ifdef CIPHER2015
		CipherRA((char*)&TempDisItem, sizeof(tDissolutionItemInfo), pos + i * sizeof(tDissolutionItemInfo));
#endif
		
		m_vecDissolutionItemInfo.push_back( TempDisItem );
	}		
	return TRUE;
}

tDissolutionItemInfo CAtumDatabase :: Get_DissolutionitemInfo( const INT itemNum )
{
	vectorDissolutionItemInfo::iterator it = m_vecDissolutionItemInfo.begin();
	tDissolutionItemInfo tempDissolutionItem;
	memset( &tempDissolutionItem , 0x00 , sizeof( tDissolutionItemInfo ) );
	while (it != m_vecDissolutionItemInfo.end())
	{
		if((*it).SourceItemnum == itemNum)
		{
			return tempDissolutionItem;
		}	
		it++;
	}
	return tempDissolutionItem;
}

BOOL CAtumDatabase :: Is_DissolutionitemInfo( const INT itemNum )
{
	vectorDissolutionItemInfo::iterator it = m_vecDissolutionItemInfo.begin();
	tDissolutionItemInfo tempDissolutionItem;
	memset( &tempDissolutionItem , 0x00 , sizeof( tDissolutionItemInfo ) );
	while (it != m_vecDissolutionItemInfo.end())
	{
		if((*it).SourceItemnum == itemNum)
		{
			return TRUE;
		}	
		it++;
	}
	return FALSE;
}

void CAtumDatabase::DeleteGuildInfo()
{
	CMapGuildMarkIterator itGuildMark = m_mapGuildMark.begin();
	while(itGuildMark != m_mapGuildMark.end())
	{
		util::del(itGuildMark->second->pImage);
		util::del(itGuildMark->second->pData);
		util::del(itGuildMark->second);
		itGuildMark++;
	}
	m_mapGuildMark.clear();
	m_mapGuildMarkGetInfo.clear();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		"#define SKILL_BASE_NUM(x)	((int)x/10)*10	 // 스킬의 ItemNum에서 BaseNum을 구함" 제거를 위해
/// \author		// 2013-03-13 by bhsohn 스킬 번호 구조 수정
/// \date		2013-03-13 ~ 2013-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
#ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
int CAtumDatabase::GetSkillBaseNum(int nItemNum)
{

//	#define SKILL_BASE_NUM(x)	((int)x/10)*10	 // 스킬의 ItemNum에서 BaseNum을 구함
	ITEM* pItem = GetItemInfoLoadItemData(nItemNum);
	if(!pItem)
	{
//		return 0;
		int nTmpItemNum = ((int)nItemNum/10)*10;
		return nTmpItemNum;
	}
	if(0 == pItem->NextSkill)
	{
		int nTmpItemNum = ((int)nItemNum/10)*10;
		return nTmpItemNum;
	}
	return pItem->NextSkill;	//
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		"#define SKILL_LEVEL(x)		(x - ((int)x/10)*10)				// 스킬의 ItemNum에서 SkillLevel을 구함" 제거를 위해
/// \author		// 2013-03-13 by bhsohn 스킬 번호 구조 수정
/// \date		2013-03-13 ~ 2013-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetSkillLevel(int nItemNum)
{
//#define SKILL_LEVEL(x)		(x - ((int)x/10)*10)				// 스킬의 ItemNum에서 SkillLevel을 구함
	ITEM* pItem = GetItemInfoLoadItemData(nItemNum);
	if(!pItem)
	{
//		return 0;
		int nTmpItemNum = (nItemNum - ((int)nItemNum/10)*10);
		return nTmpItemNum;
	}
	return pItem->SkillLevel;	//

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		"#define SKILL_KIND(x)		((int)x/1000) - ((int)x/10000)*10	// Skill 종류(공격: 0, 방어: 1, 컨트롤: 2, 속성|보조: 3)" 제거를 위해
/// \author		// 2013-03-13 by bhsohn 스킬 번호 구조 수정
/// \date		2013-03-13 ~ 2013-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetSkillKind(int nItemNum)
{
// #define SKILL_KIND(x)		((int)x/1000) - ((int)x/10000)*10	// Skill 종류(공격: 0, 방어: 1, 컨트롤: 2, 속성|보조: 3)
	ITEM* pItem = GetItemInfoLoadItemData(nItemNum);
	if(!pItem)
	{
//		return 0;
		int nTmpItemNum = ((int)nItemNum/1000) - ((int)nItemNum/10000)*10;
		return nTmpItemNum;
	}
	return pItem->Kind;	//
}
#endif // #ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-03-13 by bhsohn 스킬 번호 구조 수정
/// \date		2013-03-13 ~ 2013-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetSkillKindIndex(int nItemNum)
{
#ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
	// #define SKILL_KIND(x)		((int)x/1000) - ((int)x/10000)*10	// Skill 종류(공격: 0, 방어: 1, 컨트롤: 2, 속성|보조: 3)	
	int  nIndex = GetSkillKind(nItemNum);	//
	nIndex = max(nIndex-SKILL_KIND_ATTACK, 0);
	nIndex = min(nIndex, SKILL_KIND_ATTRIBUTE-SKILL_KIND_ATTACK);
#else
	int nIndex = SKILL_KIND(nItemNum);
#endif
	return nIndex;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		SourceIndex처리
/// \author		// 2013-03-13 by bhsohn 스킬 번호 구조 수정
/// \date		2013-03-13 ~ 2013-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDatabase::GetSkillBaseSourceIndex(int nSourceIndex)
{
#ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
	//	#define SKILL_BASE_NUM(x)	((int)x/10)*10	 // 스킬의 ItemNum에서 BaseNum을 구함
	int nItemNum = ((int)nSourceIndex/10)*10;
	return nItemNum;
#else
	return SKILL_BASE_NUM(nSourceIndex);
#endif
//	return GetSkillBaseNum(nItemNum);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아머 컬렉션 초기화
/// \author		// 2013-05-28 by bhsohn 아머 컬렉션 시스템
/// \date		2013-05-13 ~ 2013-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumDatabase::InitArmorCollectionInfo()
{
	if(!g_pStoreData)
	{
		return;
	}	

	CMapItemInfoIterator itInfoItemTemp = m_mapItemInfoTemp.begin();
	while(itInfoItemTemp != m_mapItemInfoTemp.end())
	{
		ITEM* pInfo = itInfoItemTemp->second;		
		if(pInfo && (ITEMKIND_CARD == pInfo->Kind) && pInfo->IsExistDesParam(DES_COLLECTION_ARMOR_INDEX))
		{	
			g_pStoreData->AddArmorCollectInfo(pInfo);			
		}

		itInfoItemTemp++;
	}

}


#ifdef _REWORKED_COLORSHOP
std::vector<ITEM*> CAtumDatabase::GetColourItemsByReqUnitKind(int ReqUnitKind)
{
	std::vector<ITEM*> vectItemList;
	CMapItemInfoIterator it = m_mapItemInfoTemp.begin();
	while (it != m_mapItemInfoTemp.end())
	{
		if (ITEMKIND_COLOR_ITEM == it->second->Kind && CompareBitFlag(it->second->ReqUnitKind, ReqUnitKind))
		{
			vectItemList.push_back(it->second);
		}

		it++;
	}
	return vectItemList;
}
#endif