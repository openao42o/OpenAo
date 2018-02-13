#include "stdafx.h"
#include "AtumDBHelper.h"
#include "sql.h"
#include "sqlext.h"
#define UNICODE
#include "odbcss.h"
#undef UNICODE
#include "atumparam.h"
#include "AtumProtocol.h"

#include "GlobalGameServer.h"

#include "WayPointManager.h" // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

#include "CompileUtils.h"

#ifndef DBGOUT
#define DBGOUT (void(0))
#endif

#ifdef _ATUM_SERVER
CHARACTER& CHARACTER::operator=(const CHARACTER_DB_EX& rhs)
{
	this->CreatedTime = rhs.CreatedTime;
	this->LastStartedTime = rhs.LastStartedTime;
	this->LevelUpTime = rhs.LevelUpTime; // 2006-12-18 by dhjin, 레벨업 시간

	return *this;
}
#endif

int CAtumDBHelper::LoadItemInfo(CODBCStatement* i_pODBCStmt, ez_map<INT, ITEM>* o_pMapItemInfo, ez_map<string, ITEM>* o_pMapItemInfoName /* = NULL */)
{
	// clear map
	o_pMapItemInfo->clear();

	SQLINTEGER cb[CB_COUNT_ITEM];
	ITEM itemInfo { }; //memset(&itemInfo, 0x00, sizeof(ITEM));

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery("SELECT * FROM ti_item WITH (NOLOCK)");

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(QUERY_080702_0056);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0056));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	// Bind Columns
	BindColITEM(i_pODBCStmt->m_hstmt, itemInfo, cb);

	int nObjects = 0;
	
	while (SQLFetch(i_pODBCStmt->m_hstmt) != SQL_NO_DATA)
	{
		o_pMapItemInfo->insertEZ(itemInfo.ItemNum, itemInfo);
		if (o_pMapItemInfoName != NULL)
		{
			char* pItemNumPtr = StringCullingForValidChar(itemInfo.ItemName, SIZE_MAX_ITEM_NAME - 8);
			sprintf(pItemNumPtr, " %d", itemInfo.ItemNum);
			o_pMapItemInfoName->insertEZ(itemInfo.ItemName, itemInfo);
		}

		nObjects++;

		memset(&itemInfo, 0x00, sizeof(ITEM));
	}

	i_pODBCStmt->FreeStatement();

	return nObjects;
}

int CAtumDBHelper::LoadEnchantInfo(CODBCStatement* i_pODBCStmt, ez_map<INT, ENCHANT_INFO>* o_pMapEnchantInfo)
{
	// clear map
	o_pMapEnchantInfo->clear();

	RETCODE ret;
	ENCHANT_INFO retEnchantInfo;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery("SELECT * FROM ti_EnchantInfo WITH (NOLOCK)");

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(QUERY_080702_0057);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0057));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2005-08-02 by cmkwon, 인챈트를 최대 9개에서 40개로 수정함
	SQLINTEGER arrCB[CB_COUNT_ti_EnchantInfo];
	fill_n(arrCB, CB_COUNT_ti_EnchantInfo, SQL_NTS);

	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &retEnchantInfo.EnchantItemNum, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &retEnchantInfo.EnchantItemCount, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &retEnchantInfo.EnchantCost, 0, &arrCB[3]);

	for (auto i = 0; i < carrlen(retEnchantInfo.ProbabilityPerLevel); i++)
		
		SQLBindCol(i_pODBCStmt->m_hstmt, 4 + i, SQL_C_PROB10K, &retEnchantInfo.ProbabilityPerLevel[0 + i], 0, &arrCB[4 + i]);


	int nObjects = 0;
	util::zero(retEnchantInfo);
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		// 2009-10-30 by cmkwon, 체크 추가
		if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumDBHelper::LoadEnchantInfo# error !! ObjectCount(%d) ret(0x%X) \r\n", nObjects, ret);
			return 0;
		}

		o_pMapEnchantInfo->insertEZ(retEnchantInfo.EnchantItemNum, retEnchantInfo);

		nObjects++;

		util::zero(retEnchantInfo);
	}

	i_pODBCStmt->FreeStatement();

	return nObjects;
}

int CAtumDBHelper::LoadItemMixingInfo(CODBCStatement* i_pODBCStmt, vector<ITEM_MIXING_INFO>* o_pVectorItemMixingInfo)
{
	o_pVectorItemMixingInfo->clear();

	SQLINTEGER cb1, cb2, cb3, cb4, cb5; // 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가)
	ITEM_MIXING_INFO retMixingInfo;
	ITEM_MIXING_INFO_TOOL retMixingInfoTool;
	ITEM_MIXING_ELEMENT_TOOL retMixingElementTool;

	vector<ITEM_MIXING_INFO_TOOL> vectorMixingInfoTool;
	vector<ITEM_MIXING_ELEMENT_TOOL> vectorMixingElementTool;

	// ItemMixingInfoTool 읽기

	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0058));
	if (!bRet)
	{
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &retMixingInfoTool.UniqueID, 0, &cb1);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &retMixingInfoTool.TargetItemNum, 0, &cb2);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_PROB10K, &retMixingInfoTool.MixingProbability, 0, &cb3);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &retMixingInfoTool.MixingCost, 0, &cb4);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &retMixingInfoTool.Visible, 0, &cb5); // 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가)

	memset(&retMixingInfoTool, 0x00, sizeof(retMixingInfoTool));
	while (SQLFetch(i_pODBCStmt->m_hstmt) != SQL_NO_DATA)
	{
		vectorMixingInfoTool.push_back(retMixingInfoTool);

		memset(&retMixingInfoTool, 0x00, sizeof(retMixingInfoTool));
	}

	i_pODBCStmt->FreeStatement();


	// ItemMixingElementTool 읽기

	bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_120220_0003));
	if (!bRet)
	{
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &retMixingElementTool.UniqueID, 0, &cb1);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &retMixingElementTool.SourceItem.ItemNum, 0, &cb2);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &retMixingElementTool.SourceItem.Count, 0, &cb3);

	memset(&retMixingElementTool, 0x00, sizeof(retMixingElementTool));
	while (SQLFetch(i_pODBCStmt->m_hstmt) != SQL_NO_DATA)
	{
		vectorMixingElementTool.push_back(retMixingElementTool);

		memset(&retMixingElementTool, 0x00, sizeof(retMixingElementTool));
	}

	i_pODBCStmt->FreeStatement();

	// ItemMixingInfo 조합

	int iMixingInfoToolSize = vectorMixingInfoTool.size();
	int iMixingElementToolSize = vectorMixingElementTool.size();

	int b = 0;
	int nObjects = 0;
	int iElementCount = 0;

	for (int a = 0; a < iMixingInfoToolSize; a++)
	{
		memset(&retMixingInfo, 0x00, sizeof(retMixingInfo));

		retMixingInfo.TargetItemNum = vectorMixingInfoTool[a].TargetItemNum;
		retMixingInfo.MixingProbability = vectorMixingInfoTool[a].MixingProbability;
		retMixingInfo.MixingCost = vectorMixingInfoTool[a].MixingCost;

		iElementCount = 0;

		for (; b < iMixingElementToolSize; b++)
		{
			if (vectorMixingInfoTool[a].UniqueID == vectorMixingElementTool[b].UniqueID)
			{
				retMixingInfo.SourceItem[iElementCount].ItemNum = vectorMixingElementTool[b].SourceItem.ItemNum;
				retMixingInfo.SourceItem[iElementCount].Count = vectorMixingElementTool[b].SourceItem.Count;

				iElementCount++;
			}

			if (vectorMixingElementTool[b].UniqueID > vectorMixingInfoTool[a].UniqueID)
			{
				break;
			}
		}

		retMixingInfo.NumOfSourceItems = 0;
		for (int i = 0; i < COUNT_ITEM_MIXING_SOURCE; i++)
		{
			if (retMixingInfo.SourceItem[i].ItemNum != 0)
			{
				retMixingInfo.NumOfSourceItems++;
			}
		}

		retMixingInfo.Visible = vectorMixingInfoTool[a].Visible; // 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가)

		if (0 != retMixingInfo.NumOfSourceItems)
		{
			o_pVectorItemMixingInfo->push_back(retMixingInfo);

			nObjects++;
		}
	}

	// end 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo

	return nObjects;
}

int CAtumDBHelper::LoadRareItemInfo(CODBCStatement* i_pODBCStmt, ez_map<INT, RARE_ITEM_INFO>* o_pMapRareItemInfo, ez_map<string, RARE_ITEM_INFO>* o_pMapRareItemInfoName, ez_map<BYTE, vector<RARE_ITEM_INFO*>>* o_pMapLevel2RareItemInfo)
{
	// clear map
	o_pMapRareItemInfo->clear();

	RETCODE ret;
	SQLINTEGER cb[CB_COUNT_RARE_ITEM_INFO ];
	fill_n(cb, CB_COUNT_RARE_ITEM_INFO, SQL_NTS);

	RARE_ITEM_INFO rareItemInfo;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery("SELECT * FROM ti_RareItemInfo WITH (NOLOCK)");

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(QUERY_080702_0059);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0059));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	// Bind Columns
	CAtumDBHelper::BindColRareItemInfo(i_pODBCStmt->m_hstmt, rareItemInfo, cb);

	int nObjects = 0;
	memset(&rareItemInfo, 0x00, sizeof(RARE_ITEM_INFO));
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_pMapRareItemInfo->insertEZ(rareItemInfo.CodeNum, rareItemInfo);
		if (o_pMapRareItemInfoName != NULL)
		{
			o_pMapRareItemInfoName->insertEZ(rareItemInfo.Name, rareItemInfo);
		}

		nObjects++;
		memset(&rareItemInfo, 0x00, sizeof(RARE_ITEM_INFO));
	}

	i_pODBCStmt->FreeStatement();

	// load level to rareiteminfo
	int i;
	if (o_pMapLevel2RareItemInfo != NULL)
	{
		for (i = 1; i <= CHARACTER_MAX_LEVEL; i++)
		{
			vector<RARE_ITEM_INFO*> tmpVector;
			o_pMapLevel2RareItemInfo->insertEZ(i, tmpVector);
		}

		ez_map<INT, RARE_ITEM_INFO>::iterator itrRareItemInfo = o_pMapRareItemInfo->begin();
		while (itrRareItemInfo != o_pMapRareItemInfo->end())
		{
			RARE_ITEM_INFO* pRareItemInfo = &(itrRareItemInfo->second);

			for (int j = pRareItemInfo->ReqMinLevel; j <= pRareItemInfo->ReqMaxLevel; j++)
			{
				vector<RARE_ITEM_INFO*>* pTmpVector = o_pMapLevel2RareItemInfo->findEZ_ptr(j);
				if (pTmpVector)
				{// 2006-09-08 by cmkwon, 체크 필요한 사항임

					// 2007-12-07 by cmkwon, 레어 시스템 수정 - 리스트를 랜덤하게 추가 해야 한다.					
					//pTmpVector->push_back(pRareItemInfo);

					// 2007-12-07 by cmkwon, 레어 시스템 수정 - 아래와 같이 처리 함					
					if (1 >= pTmpVector->size())
					{
						pTmpVector->push_back(pRareItemInfo);
					}
					else
					{
						int nIdx = RANDI(0, pTmpVector->size()-1);
						vector<RARE_ITEM_INFO*>::iterator itr = pTmpVector->begin();
						for (int i = 0; itr != pTmpVector->end(); itr++ , i++)
						{
							if (i == nIdx)
							{
								pTmpVector->insert(itr, pRareItemInfo);
								break;
							}
						}
					}
				}
			}

			itrRareItemInfo++;
		}
	}

	return nObjects;
}

int CAtumDBHelper::LoadMapObjectInfo(CODBCStatement* i_pODBCStmt, ez_map<int, MAPOBJECTINFO>* o_mapMapObjectInfo)
{
	// clear vector
	o_mapMapObjectInfo->clear();

	RETCODE ret;
	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6;
	MAPOBJECTINFO retMapObjectInfo;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery("SELECT Code, Alpha, Collision, CollisionForServer,\
	//												RenderIndex, ObjectRenderType FROM ti_MapObject WITH (NOLOCK)");

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(QUERY_080702_0060);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0060));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &retMapObjectInfo.Code, 0, &cb1);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &retMapObjectInfo.Alpha, 0, &cb2);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_TINYINT, &retMapObjectInfo.Collision, 0, &cb3);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &retMapObjectInfo.CollisionForServer, 0, &cb4);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_USHORT, &retMapObjectInfo.RenderIndex, 0, &cb5);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_TINYINT, &retMapObjectInfo.ObjectRenderType, 0, &cb6);

	int nObjects = 0;
	memset(&retMapObjectInfo, 0x00, sizeof(MAPOBJECTINFO));
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_mapMapObjectInfo->insertEZ(retMapObjectInfo.Code, retMapObjectInfo);

		nObjects++;
		memset(&retMapObjectInfo, 0x00, sizeof(MAPOBJECTINFO));
	}

	i_pODBCStmt->FreeStatement();

	return nObjects;
}

int CAtumDBHelper::LoadAllBUILDINGNPC(CODBCStatement* i_pODBCStmt, vector<BUILDINGNPC>* o_pVectorBuildingNPC)
{
	// clear vector
	o_pVectorBuildingNPC->clear();

	RETCODE ret;
	SDWORD arrCB[10];
	SQLINTEGER pcbNTS = SQL_NTS;
	BUILDINGNPC retBuildingNPC;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery("SELECT * FROM ti_BuildingNPC WITH (NOLOCK)");

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(QUERY_080702_0061);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0061));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &retBuildingNPC.BuildingIndex, 0, &arrCB[1]);
	arrCB[2] = SQL_NTS;
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, retBuildingNPC.BuildingName, SIZE_MAX_BUILDING_NAME, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_UTINYINT, &retBuildingNPC.BuildingKind, 0, &arrCB[3]);
	arrCB[4] = SQL_NTS;
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_USHORT, &retBuildingNPC.MapIndex, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &retBuildingNPC.NPCIndex, 0, &arrCB[5]);
	arrCB[6] = SQL_NTS;
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, retBuildingNPC.NPCName, SIZE_MAX_NPC_NAME, &arrCB[6]);
	arrCB[7] = SQL_NTS;
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_CHAR, retBuildingNPC.GreetingTalk, SIZE_MAX_BUILDING_NPC_TALK_STRING, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_ULONG, &retBuildingNPC.OwnerCityMapIndex, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_ULONG, &retBuildingNPC.OwnerOrderNumber, 0, &arrCB[9]);

	int nObjects = 0;
	memset(&retBuildingNPC, 0x00, sizeof(BUILDINGNPC));
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_pVectorBuildingNPC->push_back(retBuildingNPC);

		nObjects++;
		memset(&retBuildingNPC, 0x00, sizeof(BUILDINGNPC));
	}

	i_pODBCStmt->FreeStatement();

	return nObjects;
}

int CAtumDBHelper::Load_MEX_MONSTER_INFO(CODBCStatement* i_pODBCStmt, vector<MEX_MONSTER_INFO>* o_pVectorMexMonsterInfo)
{
	// clear vector
	o_pVectorMexMonsterInfo->clear();

	RETCODE ret;
	SQLINTEGER cb[CB_COUNT_MONSTER ];
	MONSTER_INFO retMonsterInfo;

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(MONSTER_SQL_QUERY_FOR_CLIENT);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0062));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	// Bind Columns
	CAtumDBHelper::BindColMONSTER(i_pODBCStmt->m_hstmt, retMonsterInfo, cb);

	int nObjects = 0;
	memset(&retMonsterInfo, 0x00, sizeof(MONSTER_INFO));
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		MEX_MONSTER_INFO tmpMexMonsterInfo;
		tmpMexMonsterInfo = retMonsterInfo;
		o_pVectorMexMonsterInfo->push_back(tmpMexMonsterInfo);

		nObjects++;
		memset(&retMonsterInfo, 0x00, sizeof(MONSTER_INFO));
	}

	i_pODBCStmt->FreeStatement();

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			MEX_MONSTER_INFO * CAtumDBHelper::FindMEX_MONSTER_INFO(vector<MEX_MONSTER_INFO> *i_pVectorMonsterInfo, int i_nMonsterNum)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CAtumDBHelper::FindMEX_MONSTER_INFO
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MEX_MONSTER_INFO* CAtumDBHelper::FindMEX_MONSTER_INFO(vector<MEX_MONSTER_INFO>* i_pVectorMonsterInfo, int i_nMonsterNum)
{
	vector<MEX_MONSTER_INFO>::iterator itr(i_pVectorMonsterInfo->begin());
	for (; itr != i_pVectorMonsterInfo->end(); itr++)
	{
		MEX_MONSTER_INFO* pMexMonInfo = &*itr;
		if (pMexMonInfo->MonsterUnitKind == i_nMonsterNum)
		{
			return pMexMonInfo;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadHPActionByDB(CODBCStatement *i_pODBCStmt, ez_map<INT, vectHPAction> * o_pMapHPAction, CLocalization *i_pLocalization)
/// \brief		인피니티 - HPAction DB 로딩
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::LoadHPActionByDB(CODBCStatement* i_pODBCStmt, ez_map<INT, vectHPAction>* o_pMapHPAction)
{
	o_pMapHPAction->clear();

	RETCODE ret;
	SQLINTEGER cb[15];
	fill_n(cb, 15, SQL_NTS);

	HPACTION SDBHPAction;
	util::zero(&SDBHPAction, sizeof(HPACTION));
	vectHPAction DBHPActionList;
	DBHPActionList.clear();

	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0500));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &SDBHPAction.HPActionUID, 0, &cb[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &SDBHPAction.HPActionNum, 0, &cb[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_UTINYINT, &SDBHPAction.HPMaxValueRate, 0, &cb[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &SDBHPAction.HPMinValueRate, 0, &cb[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &SDBHPAction.UseItemArrayIdx, 0, &cb[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &SDBHPAction.NextUseItemArrayIdx, 0, &cb[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_UTINYINT, &SDBHPAction.HitRate, 0, &cb[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_ULONG, &SDBHPAction.UseCount, 0, &cb[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_UTINYINT, &SDBHPAction.HPTalkCondition, 0, &cb[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_UTINYINT, &SDBHPAction.HPTalkImportance, 0, &cb[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_ULONG, &SDBHPAction.PreHPCameraTremble, 0, &cb[11]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_CHAR, SDBHPAction.PreHPTalk, SIZE_MAX_HPTALK_DESCRIPTION, &cb[12]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_ULONG, &SDBHPAction.HPCameraTremble, 0, &cb[13]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 14, SQL_C_CHAR, SDBHPAction.HPTalk, SIZE_MAX_HPTALK_DESCRIPTION, &cb[14]);

	int nObjects = 0;
	int CheckHPActionNum = 0;
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		if (0 == CheckHPActionNum)
		{
			CheckHPActionNum = SDBHPAction.HPActionNum;
			DBHPActionList.push_back(SDBHPAction);
			nObjects++;
			util::zero(&SDBHPAction, sizeof(HPACTION));
			continue;
		}

		if (CheckHPActionNum != SDBHPAction.HPActionNum)
		{
			o_pMapHPAction->insert(pair<INT, vectHPAction>(CheckHPActionNum, DBHPActionList));
			DBHPActionList.clear();
			CheckHPActionNum = SDBHPAction.HPActionNum;
			DBHPActionList.push_back(SDBHPAction);
			nObjects++;
			util::zero(&SDBHPAction, sizeof(HPACTION));
			continue;
		}

		DBHPActionList.push_back(SDBHPAction);
		nObjects++;
		util::zero(&SDBHPAction, sizeof(HPACTION));
	}

	o_pMapHPAction->insert(pair<INT, vectHPAction>(CheckHPActionNum, DBHPActionList));
	i_pODBCStmt->FreeStatement();

	return nObjects;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumDBHelper::DBLoadInfinityModeInfo(CODBCStatement *i_pODBCStmt, vectorInfinityModeInfo * o_pvectInfinityModeIndo, vectMapIndexList *o_pArenaMapIndexList)	
/// \brief		인피니티 - 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBHelper::DBLoadInfinityModeInfo(CODBCStatement* i_pODBCStmt, vectorInfinityModeInfo* o_pvectInfinityModeInfo, vectMapIndexList* o_pArenaMapIndexList)
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_Load_InfinityMode
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
		SELECT InfinityModeUID, ModeTypeNum, MapIdx, InfinityMonsterIdx, CinemaNum, RevisionNum
				, ResetCycle, EntranceCount, LimitTime, MinLv, MaxLv, MinAdmissionCount, MaxAdmissionCount, TimePenaltyValue
				FROM atum2_db_account.dbo.ti_InfinityMode WITH (NOLOCK);
	GO
	************************************************************************/
	RETCODE ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_090909_0502, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	INFINITY_MODEINFO tmInfinityModeInfo;
	util::zero(&tmInfinityModeInfo, sizeof(INFINITY_MODEINFO));

	SQLINTEGER arrCB[16];
	fill_n(arrCB, 16, SQL_NTS);
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmInfinityModeInfo.InfinityModeUID, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_UTINYINT, &tmInfinityModeInfo.ModeTypeNum, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmInfinityModeInfo.MapIdx, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tmInfinityModeInfo.InfinityMonsterIdx, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &tmInfinityModeInfo.CinemaNum, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &tmInfinityModeInfo.RevisionNum, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_UTINYINT, &tmInfinityModeInfo.ResetCycle, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_UTINYINT, &tmInfinityModeInfo.EntranceCount, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_ULONG, &tmInfinityModeInfo.LimitTime, 0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_UTINYINT, &tmInfinityModeInfo.MinLv, 0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_UTINYINT, &tmInfinityModeInfo.MaxLv, 0, &arrCB[11]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_UTINYINT, &tmInfinityModeInfo.MinAdmissionCount, 0, &arrCB[12]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_UTINYINT, &tmInfinityModeInfo.MaxAdmissionCount, 0, &arrCB[13]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 14, SQL_C_LONG, &tmInfinityModeInfo.TimePenaltyValue, 0, &arrCB[14]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	SQLBindCol(i_pODBCStmt->m_hstmt, 15, SQL_C_LONG, &tmInfinityModeInfo.HPPenaltyValue, 0, &arrCB[15]); // 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)

	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		vectMapIndexList::iterator itr = find(o_pArenaMapIndexList->begin(), o_pArenaMapIndexList->end(), tmInfinityModeInfo.MapIdx);
		if (itr == o_pArenaMapIndexList->end())
		{
			o_pArenaMapIndexList->push_back(tmInfinityModeInfo.MapIdx);
		}
		o_pvectInfinityModeInfo->push_back(tmInfinityModeInfo);
		util::zero(&tmInfinityModeInfo, sizeof(INFINITY_MODEINFO));
	}
	i_pODBCStmt->FreeStatement();

	return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_LoadSelfServiceInfor8ServerGroupInfo(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* o_pServiInfo, SWRK_SERVER_GROUP* io_pServGroup)
//{
//	util::zero(o_pServiInfo, sizeof(*o_pServiInfo));
//	util::zero(io_pServGroup->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME);
//
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 월드랭킹을 위한 Service 정보 가져오기
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &io_pServGroup->ServerGroupID, 0, NULL);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0300);
//	if (FALSE == bRet)
//	{
//		// 2009-02-19 by cmkwon, 월드랭킹 관련 로컬 설정을 하지 안으면 모든 랭킹 정보 처리하지 않는다.
//		i_pODBCStmt->ProcessLogMessagesForStmt(PROCEDURE_090219_0300);
//		i_pODBCStmt->FreeStatement();
//		return FALSE;
//	}
//
//	SQLINTEGER cb[4];
//	fill_n(cb, 4, SQL_NTS);
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &o_pServiInfo->ServiceUID, 0, &cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_CHAR, o_pServiInfo->ServiceName, SIZE_MAX_SERVICE_NAME, &cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_CHAR, io_pServGroup->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME, &cb[3]);
//	RETCODE ret = SQLFetch(hstmt);
//	i_pODBCStmt->FreeStatement();
//	if (0 == o_pServiInfo->ServiceUID
//		|| 0 >= strlen(o_pServiInfo->ServiceName)
//		|| 0 >= strlen(io_pServGroup->ServerGroupNameforRK))
//	{
//		return FALSE;
//	}
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_LevelArrangeRanking(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, SWRK_SERVER_GROUP* i_pServGroup)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 현재 서버군의 랭킹 리스트를 삭제한다.
//	SQLINTEGER cb[4];
//	fill_n(cb, 4, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServGroup->ServerGroupID, 0, &cb[2]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0301);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 2. 현재 서버군의 랭킹 리스트를 다시 정렬한다. 
//	cb[1] = cb[2] = cb[3] = SQL_NTS;
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServGroup->ServerGroupID, 0, &cb[2]);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, i_pServGroup->ServerGroupNameforRK, 0, &cb[3]);
//	bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0302);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_LevelLoadRanking(CODBCStatement* i_pODBCStmt, INT i_ServiUID, BYTE i_byRankingScope, mtvectSWRK_LEVEL_FOR_DB* o_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 
//	SQLINTEGER cb[12];
//	fill_n(cb, 12, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_ServiUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byRankingScope, 0, &cb[2]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0303);
//	if (FALSE == bRet)
//	{
//		i_pODBCStmt->FreeStatement();
//		return FALSE;
//	}
//
//	SWRK_LEVEL_FOR_DB tmRanker;
//	SQL_TIMESTAMP_STRUCT sqlTime;
//
//	cb[1] = cb[2] = cb[3] = cb[4] = cb[5] = cb[6] = cb[7] = cb[8] = cb[9] = cb[10] = cb[11] = SQL_NTS;
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmRanker.ServiceUID, 0, &cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmRanker.RankingScope, 0, &cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmRanker.ServerGroupID, 0, &cb[3]);
//	SQLBindCol(hstmt, 4, SQL_C_CHAR, tmRanker.ServerGroupNameforRK, SIZE_MAX_SERVER_NAME, &cb[4]);
//	SQLBindCol(hstmt, 5, SQL_C_ULONG, &tmRanker.CharacterUID, 0, &cb[5]);
//	SQLBindCol(hstmt, 6, SQL_C_CHAR, tmRanker.NameforRK, SIZE_MAX_NAME_FOR_RANKING, &cb[6]);
//	SQLBindCol(hstmt, 7, SQL_C_LONG, &tmRanker.UnitKind, 0, &cb[7]);
//	SQLBindCol(hstmt, 8, SQL_C_UTINYINT, &tmRanker.InfluenceType, 0, &cb[8]);
//	SQLBindCol(hstmt, 9, SQL_C_UTINYINT, &tmRanker.Level, 0, &cb[9]);
//	SQLBindCol(hstmt, 10, SQL_C_DOUBLE, &tmRanker.Experience, 0, &cb[10]); // 2009-09-22 by cmkwon, 월드 랭킹(레벨랭킹) 버그 수정 - 
//	SQLBindCol(hstmt, 11, SQL_C_TIMESTAMP, &sqlTime, 0, &cb[11]);
//
//	while (TRUE)
//	{
//		util::zero(&tmRanker, sizeof(tmRanker));
//		util::zero(&sqlTime, sizeof(sqlTime));
//
//		SQLRETURN ret = SQLFetch(hstmt);
//		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//		{
//			break;
//		}
//		tmRanker.LevelUpTime = sqlTime;
//		o_pvectRankerList->push_back(tmRanker);
//	}
//	i_pODBCStmt->FreeStatement();
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_LevelInsertRanking(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_LEVEL_FOR_DB* i_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 랭킹 삭제
//	SQLINTEGER cb[12];
//	fill_n(cb, 12, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0331);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	mtvectSWRK_LEVEL_FOR_DB::iterator itr(i_pvectRankerList->begin());
//	for (; itr != i_pvectRankerList->end(); itr++)
//	{
//		SWRK_LEVEL_FOR_DB* pRanker = &*itr;
//
//		char szLevelUpTime[SIZE_MAX_SQL_DATETIME_STRING];
//		util::zero(szLevelUpTime, SIZE_MAX_SQL_DATETIME_STRING);
//		pRanker->LevelUpTime.GetSQLDateTimeString(szLevelUpTime, SIZE_MAX_SQL_DATETIME_STRING);
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-19 by cmkwon, 2. 현재 서비스의 랭킹 리스트를 추가한다.
//		cb[1] = cb[2] = cb[3] = cb[4] = cb[5] = cb[6] = cb[7] = cb[8] = cb[9] = cb[10] = cb[11] = SQL_NTS;
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServiceUID, 0, &cb[1]);
//		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->RankingScope, 0, &cb[2]);
//		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServerGroupID, 0, &cb[3]);
//		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRanker->ServerGroupNameforRK, 0, &cb[4]);
//		SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRanker->CharacterUID, 0, &cb[5]);
//		SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRanker->NameforRK, 0, &cb[6]);
//		SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->UnitKind, 0, &cb[7]);
//		SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->InfluenceType, 0, &cb[8]);
//		SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->Level, 0, &cb[9]);
//		SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pRanker->Experience, 0, &cb[10]); // 2009-09-22 by cmkwon, 월드 랭킹(레벨랭킹) 버그 수정 - 
//		SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, &szLevelUpTime, 0, &cb[11]);
//		bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0334);
//		i_pODBCStmt->FreeStatement();
//		if (FALSE == bRet)
//		{
//			return FALSE;
//		}
//	}
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_LevelUpdateRankingToWRankingServer(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_LEVEL_FOR_DB* i_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 랭킹 삭제
//	SQLINTEGER cb[12];
//	fill_n(cb, 12, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0322);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	mtvectSWRK_LEVEL_FOR_DB::iterator itr(i_pvectRankerList->begin());
//	for (; itr != i_pvectRankerList->end(); itr++)
//	{
//		SWRK_LEVEL_FOR_DB* pRanker = &*itr;
//
//		char szLevelUpTime[SIZE_MAX_SQL_DATETIME_STRING];
//		util::zero(szLevelUpTime, SIZE_MAX_SQL_DATETIME_STRING);
//		pRanker->LevelUpTime.GetSQLDateTimeString(szLevelUpTime, SIZE_MAX_SQL_DATETIME_STRING);
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-19 by cmkwon, 2. 현재 서비스의 랭킹 리스트를 추가한다.
//		fill_n(cb, 12, SQL_NTS);
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServiceUID, 0, &cb[1]);
//		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->RankingScope, 0, &cb[2]);
//		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServerGroupID, 0, &cb[3]);
//		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRanker->ServerGroupNameforRK, 0, &cb[4]);
//		SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRanker->CharacterUID, 0, &cb[5]);
//		SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRanker->NameforRK, 0, &cb[6]);
//		SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->UnitKind, 0, &cb[7]);
//		SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->InfluenceType, 0, &cb[8]);
//		SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->Level, 0, &cb[9]);
//		SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pRanker->Experience, 0, &cb[10]); // 2009-09-22 by cmkwon, 월드 랭킹(레벨랭킹) 버그 수정 - 
//		SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, &szLevelUpTime, 0, &cb[11]);
//		bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0325);
//		i_pODBCStmt->FreeStatement();
//		if (FALSE == bRet)
//		{
//			return FALSE;
//		}
//	}
//
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_LevelLoadRankingToWorldRankingServer(CODBCStatement* i_pODBCStmt, INT i_ExcludeServiUID, mtvectSWRK_LEVEL_FOR_DB* o_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0328);
//	if (FALSE == bRet)
//	{
//		i_pODBCStmt->FreeStatement();
//		return FALSE;
//	}
//
//	SWRK_LEVEL_FOR_DB tmRanker;
//	SQL_TIMESTAMP_STRUCT sqlTime;
//
//	SQLINTEGER cb[12];
//	fill_n(cb, 12, SQL_NTS);
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmRanker.ServiceUID, 0, &cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmRanker.RankingScope, 0, &cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmRanker.ServerGroupID, 0, &cb[3]);
//	SQLBindCol(hstmt, 4, SQL_C_CHAR, tmRanker.ServerGroupNameforRK, SIZE_MAX_SERVER_NAME, &cb[4]);
//	SQLBindCol(hstmt, 5, SQL_C_ULONG, &tmRanker.CharacterUID, 0, &cb[5]);
//	SQLBindCol(hstmt, 6, SQL_C_CHAR, tmRanker.NameforRK, SIZE_MAX_NAME_FOR_RANKING, &cb[6]);
//	SQLBindCol(hstmt, 7, SQL_C_LONG, &tmRanker.UnitKind, 0, &cb[7]);
//	SQLBindCol(hstmt, 8, SQL_C_UTINYINT, &tmRanker.InfluenceType, 0, &cb[8]);
//	SQLBindCol(hstmt, 9, SQL_C_UTINYINT, &tmRanker.Level, 0, &cb[9]);
//	SQLBindCol(hstmt, 10, SQL_C_DOUBLE, &tmRanker.Experience, 0, &cb[10]);
//	SQLBindCol(hstmt, 11, SQL_C_TIMESTAMP, &sqlTime, 0, &cb[11]);
//
//	while (TRUE)
//	{
//		util::zero(&tmRanker, sizeof(tmRanker));
//		util::zero(&sqlTime, sizeof(sqlTime));
//
//		SQLRETURN ret = SQLFetch(hstmt);
//		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//		{
//			break;
//		}
//		tmRanker.LevelUpTime = sqlTime;
//
//		if (i_ExcludeServiUID != tmRanker.ServiceUID)
//		{
//			o_pvectRankerList->push_back(tmRanker);
//		}
//	}
//	i_pODBCStmt->FreeStatement();
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_FameArrangeRanking(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, SWRK_SERVER_GROUP* i_pServGroup)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 현재 서버군의 랭킹 리스트를 삭제한다.
//	SQLINTEGER cb[4];
//	fill_n(cb, 4, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServGroup->ServerGroupID, 0, &cb[2]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0304);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 2. 현재 서버군의 랭킹 리스트를 다시 정렬한다.
//	fill_n(cb, 4, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServGroup->ServerGroupID, 0, &cb[2]);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, i_pServGroup->ServerGroupNameforRK, 0, &cb[3]);
//	bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0305);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_FameLoadRanking(CODBCStatement* i_pODBCStmt, INT i_ServiUID, BYTE i_byRankingScope, mtvectSWRK_FAME_FOR_DB* o_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 
//	SQLINTEGER cb[11];
//	fill_n(cb, 11, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_ServiUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byRankingScope, 0, &cb[2]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0306);
//	if (FALSE == bRet)
//	{
//		i_pODBCStmt->FreeStatement();
//		return FALSE;
//	}
//
//	SWRK_FAME_FOR_DB tmRanker;
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmRanker.ServiceUID, 0, &cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmRanker.RankingScope, 0, &cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmRanker.ServerGroupID, 0, &cb[3]);
//	SQLBindCol(hstmt, 4, SQL_C_CHAR, tmRanker.ServerGroupNameforRK, SIZE_MAX_SERVER_NAME, &cb[4]);
//	SQLBindCol(hstmt, 5, SQL_C_ULONG, &tmRanker.CharacterUID, 0, &cb[5]);
//	SQLBindCol(hstmt, 6, SQL_C_CHAR, tmRanker.NameforRK, SIZE_MAX_NAME_FOR_RANKING, &cb[6]);
//	SQLBindCol(hstmt, 7, SQL_C_LONG, &tmRanker.UnitKind, 0, &cb[7]);
//	SQLBindCol(hstmt, 8, SQL_C_UTINYINT, &tmRanker.InfluenceType, 0, &cb[8]);
//	SQLBindCol(hstmt, 9, SQL_C_UTINYINT, &tmRanker.Level, 0, &cb[9]);
//	SQLBindCol(hstmt, 10, SQL_C_LONG, &tmRanker.Fame, 0, &cb[10]);
//	while (TRUE)
//	{
//		util::zero(&tmRanker, sizeof(tmRanker));
//
//		SQLRETURN ret = SQLFetch(hstmt);
//		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//		{
//			break;
//		}
//		o_pvectRankerList->push_back(tmRanker);
//	}
//	i_pODBCStmt->FreeStatement();
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_FameInsertRanking(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_FAME_FOR_DB* i_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 랭킹 삭제
//	SQLINTEGER cb[11];
//	fill_n(cb, 11, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0332);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	mtvectSWRK_FAME_FOR_DB::iterator itr(i_pvectRankerList->begin());
//	for (; itr != i_pvectRankerList->end(); itr++)
//	{
//		SWRK_FAME_FOR_DB* pRanker = &*itr;
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-19 by cmkwon, 2. 현재 서비스의 랭킹 리스트를 추가한다.
//		fill_n(cb, 11, SQL_NTS);
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServiceUID, 0, &cb[1]);
//		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->RankingScope, 0, &cb[2]);
//		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServerGroupID, 0, &cb[3]);
//		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRanker->ServerGroupNameforRK, 0, &cb[4]);
//		SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRanker->CharacterUID, 0, &cb[5]);
//		SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRanker->NameforRK, 0, &cb[6]);
//		SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->UnitKind, 0, &cb[7]);
//		SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->InfluenceType, 0, &cb[8]);
//		SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->Level, 0, &cb[9]);
//		SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->Fame, 0, &cb[10]);
//		bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0335);
//		i_pODBCStmt->FreeStatement();
//		if (FALSE == bRet)
//		{
//			return FALSE;
//		}
//	}
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_FameUpdateRankingToWRankingServer(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_FAME_FOR_DB* i_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 랭킹 삭제
//	SQLINTEGER cb[11];
//	fill_n(cb, 11, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0323);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	mtvectSWRK_FAME_FOR_DB::iterator itr(i_pvectRankerList->begin());
//	for (; itr != i_pvectRankerList->end(); itr++)
//	{
//		SWRK_FAME_FOR_DB* pRanker = &*itr;
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-19 by cmkwon, 2. 현재 서비스의 랭킹 리스트를 추가한다.
//		fill_n(cb, 11, SQL_NTS);
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServiceUID, 0, &cb[1]);
//		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->RankingScope, 0, &cb[2]);
//		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServerGroupID, 0, &cb[3]);
//		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRanker->ServerGroupNameforRK, 0, &cb[4]);
//		SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRanker->CharacterUID, 0, &cb[5]);
//		SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRanker->NameforRK, 0, &cb[6]);
//		SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->UnitKind, 0, &cb[7]);
//		SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->InfluenceType, 0, &cb[8]);
//		SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->Level, 0, &cb[9]);
//		SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->Fame, 0, &cb[10]);
//		bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0326);
//		i_pODBCStmt->FreeStatement();
//		if (FALSE == bRet)
//		{
//			return FALSE;
//		}
//	}
//
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_FameLoadRankingToWorldRankingServer(CODBCStatement* i_pODBCStmt, INT i_ExcludeServiUID, mtvectSWRK_FAME_FOR_DB* o_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0329);
//	if (FALSE == bRet)
//	{
//		i_pODBCStmt->FreeStatement();
//		return FALSE;
//	}
//
//	SWRK_FAME_FOR_DB tmRanker;
//
//	SQLINTEGER cb[11];
//	fill_n(cb, 11, SQL_NTS);
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmRanker.ServiceUID, 0, &cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmRanker.RankingScope, 0, &cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmRanker.ServerGroupID, 0, &cb[3]);
//	SQLBindCol(hstmt, 4, SQL_C_CHAR, tmRanker.ServerGroupNameforRK, SIZE_MAX_SERVER_NAME, &cb[4]);
//	SQLBindCol(hstmt, 5, SQL_C_ULONG, &tmRanker.CharacterUID, 0, &cb[5]);
//	SQLBindCol(hstmt, 6, SQL_C_CHAR, tmRanker.NameforRK, SIZE_MAX_NAME_FOR_RANKING, &cb[6]);
//	SQLBindCol(hstmt, 7, SQL_C_LONG, &tmRanker.UnitKind, 0, &cb[7]);
//	SQLBindCol(hstmt, 8, SQL_C_UTINYINT, &tmRanker.InfluenceType, 0, &cb[8]);
//	SQLBindCol(hstmt, 9, SQL_C_UTINYINT, &tmRanker.Level, 0, &cb[9]);
//	SQLBindCol(hstmt, 10, SQL_C_LONG, &tmRanker.Fame, 0, &cb[10]); // 2012-03-09 by hskim, 랭킹서버 Fame 관련 버그 수정
//
//	while (TRUE)
//	{
//		util::zero(&tmRanker, sizeof(tmRanker));
//
//		SQLRETURN ret = SQLFetch(hstmt);
//		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//		{
//			break;
//		}
//
//		if (i_ExcludeServiUID != tmRanker.ServiceUID)
//		{
//			o_pvectRankerList->push_back(tmRanker);
//		}
//	}
//	i_pODBCStmt->FreeStatement();
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_PVPArrangeRanking(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, SWRK_SERVER_GROUP* i_pServGroup)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 현재 서버군의 랭킹 리스트를 삭제한다.
//	SQLINTEGER cb[4];
//	fill_n(cb, 4, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServGroup->ServerGroupID, 0, &cb[2]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0307);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2009-02-19 by cmkwon, 2. 현재 서버군의 랭킹 리스트를 다시 정렬한다. 
//	fill_n(cb, 4, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServGroup->ServerGroupID, 0, &cb[2]);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, i_pServGroup->ServerGroupNameforRK, 0, &cb[3]);
//	bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0308);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_PVPLoadRanking(CODBCStatement* i_pODBCStmt, INT i_ServiUID, BYTE i_byRankingScope, mtvectSWRK_PVP_FOR_DB* o_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 
//	SQLINTEGER cb[13];
//	fill_n(cb, 13, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_ServiUID, 0, &cb[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byRankingScope, 0, &cb[2]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0309);
//	if (FALSE == bRet)
//	{
//		i_pODBCStmt->FreeStatement();
//		return FALSE;
//	}
//
//	SWRK_PVP_FOR_DB tmRanker;
//	fill_n(cb, 13, SQL_NTS);
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmRanker.ServiceUID, 0, &cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmRanker.RankingScope, 0, &cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmRanker.ServerGroupID, 0, &cb[3]);
//	SQLBindCol(hstmt, 4, SQL_C_CHAR, tmRanker.ServerGroupNameforRK, SIZE_MAX_SERVER_NAME, &cb[4]);
//	SQLBindCol(hstmt, 5, SQL_C_ULONG, &tmRanker.CharacterUID, 0, &cb[5]);
//	SQLBindCol(hstmt, 6, SQL_C_CHAR, tmRanker.NameforRK, SIZE_MAX_NAME_FOR_RANKING, &cb[6]);
//	SQLBindCol(hstmt, 7, SQL_C_LONG, &tmRanker.UnitKind, 0, &cb[7]);
//	SQLBindCol(hstmt, 8, SQL_C_UTINYINT, &tmRanker.InfluenceType, 0, &cb[8]);
//	SQLBindCol(hstmt, 9, SQL_C_UTINYINT, &tmRanker.Level, 0, &cb[9]);
//	SQLBindCol(hstmt, 10, SQL_C_LONG, &tmRanker.WinPoint, 0, &cb[10]);
//	SQLBindCol(hstmt, 11, SQL_C_LONG, &tmRanker.LossPoint, 0, &cb[11]);
//	SQLBindCol(hstmt, 12, SQL_C_LONG, &tmRanker.Score, 0, &cb[12]);
//
//	while (TRUE)
//	{
//		util::zero(&tmRanker, sizeof(tmRanker));
//
//		SQLRETURN ret = SQLFetch(hstmt);
//		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//		{
//			break;
//		}
//
//		o_pvectRankerList->push_back(tmRanker);
//	}
//	i_pODBCStmt->FreeStatement();
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_PVPInsertRanking(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_PVP_FOR_DB* i_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 랭킹 삭제
//	SQLINTEGER cb[13];
//	fill_n(cb, 13, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0333);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	mtvectSWRK_PVP_FOR_DB::iterator itr(i_pvectRankerList->begin());
//	for (; itr != i_pvectRankerList->end(); itr++)
//	{
//		SWRK_PVP_FOR_DB* pRanker = &*itr;
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-19 by cmkwon, 2. 현재 서비스의 랭킹 리스트를 추가한다.
//		fill_n(cb, 13, SQL_NTS);
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServiceUID, 0, &cb[1]);
//		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->RankingScope, 0, &cb[2]);
//		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServerGroupID, 0, &cb[3]);
//		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRanker->ServerGroupNameforRK, 0, &cb[4]);
//		SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRanker->CharacterUID, 0, &cb[5]);
//		SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRanker->NameforRK, 0, &cb[6]);
//		SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->UnitKind, 0, &cb[7]);
//		SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->InfluenceType, 0, &cb[8]);
//		SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->Level, 0, &cb[9]);
//		SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->WinPoint, 0, &cb[10]);
//		SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->LossPoint, 0, &cb[11]);
//		SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->Score, 0, &cb[12]);
//		bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0336);
//		i_pODBCStmt->FreeStatement();
//		if (FALSE == bRet)
//		{
//			return FALSE;
//		}
//	}
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_PVPUpdateRankingToWRankingServer(CODBCStatement* i_pODBCStmt, SWRK_SERVICE_INFO* i_pServiInfo, BYTE i_byRankingScope, mtvectSWRK_PVP_FOR_DB* i_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 1. 기존 랭킹 삭제
//	SQLINTEGER cb[13];
//	fill_n(cb, 13, SQL_NTS);
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiInfo->ServiceUID, 0, &cb[1]);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0324);
//	i_pODBCStmt->FreeStatement();
//	if (FALSE == bRet)
//	{
//		return FALSE;
//	}
//
//	mtvectSWRK_PVP_FOR_DB::iterator itr(i_pvectRankerList->begin());
//	for (; itr != i_pvectRankerList->end(); itr++)
//	{
//		SWRK_PVP_FOR_DB* pRanker = &*itr;
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2009-02-19 by cmkwon, 2. 현재 서비스의 랭킹 리스트를 추가한다.
//		fill_n(cb, 13, SQL_NTS);
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServiceUID, 0, &cb[1]);
//		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->RankingScope, 0, &cb[2]);
//		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->ServerGroupID, 0, &cb[3]);
//		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRanker->ServerGroupNameforRK, 0, &cb[4]);
//		SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRanker->CharacterUID, 0, &cb[5]);
//		SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRanker->NameforRK, 0, &cb[6]);
//		SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->UnitKind, 0, &cb[7]);
//		SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->InfluenceType, 0, &cb[8]);
//		SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRanker->Level, 0, &cb[9]);
//		SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->WinPoint, 0, &cb[10]);
//		SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->LossPoint, 0, &cb[11]);
//		SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRanker->Score, 0, &cb[12]);
//		bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0327);
//		i_pODBCStmt->FreeStatement();
//		if (FALSE == bRet)
//		{
//			return FALSE;
//		}
//	}
//
//
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
///// \author		cmkwon
///// \date		2009-02-24 ~ 2009-02-24
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumDBHelper::WRK_PVPLoadRankingToWorldRankingServer(CODBCStatement* i_pODBCStmt, INT i_ExcludeServiUID, mtvectSWRK_PVP_FOR_DB* o_pvectRankerList)
//{
//	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
//
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2009-02-19 by cmkwon, 
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090219_0330);
//	if (FALSE == bRet)
//	{
//		i_pODBCStmt->FreeStatement();
//		return FALSE;
//	}
//
//	SWRK_PVP_FOR_DB tmRanker;
//
//	SQLINTEGER cb[13];
//	fill_n(cb, 13, SQL_NTS);
//	SQLBindCol(hstmt, 1, SQL_C_LONG, &tmRanker.ServiceUID, 0, &cb[1]);
//	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmRanker.RankingScope, 0, &cb[2]);
//	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmRanker.ServerGroupID, 0, &cb[3]);
//	SQLBindCol(hstmt, 4, SQL_C_CHAR, tmRanker.ServerGroupNameforRK, SIZE_MAX_SERVER_NAME, &cb[4]);
//	SQLBindCol(hstmt, 5, SQL_C_ULONG, &tmRanker.CharacterUID, 0, &cb[5]);
//	SQLBindCol(hstmt, 6, SQL_C_CHAR, tmRanker.NameforRK, SIZE_MAX_NAME_FOR_RANKING, &cb[6]);
//	SQLBindCol(hstmt, 7, SQL_C_LONG, &tmRanker.UnitKind, 0, &cb[7]);
//	SQLBindCol(hstmt, 8, SQL_C_UTINYINT, &tmRanker.InfluenceType, 0, &cb[8]);
//	SQLBindCol(hstmt, 9, SQL_C_UTINYINT, &tmRanker.Level, 0, &cb[9]);
//	SQLBindCol(hstmt, 10, SQL_C_LONG, &tmRanker.WinPoint, 0, &cb[10]);
//	SQLBindCol(hstmt, 11, SQL_C_LONG, &tmRanker.LossPoint, 0, &cb[11]);
//	SQLBindCol(hstmt, 12, SQL_C_LONG, &tmRanker.Score, 0, &cb[12]);
//
//	while (TRUE)
//	{
//		util::zero(&tmRanker, sizeof(tmRanker));
//
//		SQLRETURN ret = SQLFetch(hstmt);
//		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
//		{
//			break;
//		}
//
//		if (i_ExcludeServiUID != tmRanker.ServiceUID)
//		{
//			o_pvectRankerList->push_back(tmRanker);
//		}
//	}
//	i_pODBCStmt->FreeStatement();
//	return TRUE;
//}


int CAtumDBHelper::LoadMapInfo(CODBCStatement* i_pODBCStmt, vectorMAP_INFO* o_mapMapInfo)
{
	// clear vector
	o_mapMapInfo->clear();

	RETCODE ret;

	// 2008-06-20 by cmkwon, EP3 백맵 시스템 수정(맵당 백맵을 2개 설정 할 수 있다) - DB에서 가져오기 추가
	SQLINTEGER arrCB[54];
	fill_n(arrCB, 54, SQL_NTS);

	MAP_INFO retMapInfo;

	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0064));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_USHORT, &retMapInfo.MapIndex, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_USHORT, &retMapInfo.RenderMapIndex, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_USHORT, &retMapInfo.BeforeMapIndex, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, retMapInfo.MapName, SIZE_MAX_MAP_NAME, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_SHORT, &retMapInfo.MapInfluenceType, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &retMapInfo.CityMapIndex, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &retMapInfo.CityWarQuestIndex, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &retMapInfo.CityWarQuestNPCIndex, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &retMapInfo.CityWarQuestMonsterUID, 0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_SHORT, &retMapInfo.MaxUserCount, 0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_SHORT, &retMapInfo.MaxGroundHeight, 0, &arrCB[11]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_SHORT, &retMapInfo.MinGroundHeight, 0, &arrCB[12]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_SHORT, &retMapInfo.WaterHeight, 0, &arrCB[13]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 14, SQL_C_SHORT, &retMapInfo.UserVisibleDistance, 0, &arrCB[14]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 15, SQL_C_SHORT, &retMapInfo.MonsterVisibleDistance, 0, &arrCB[15]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 16, SQL_C_LONG, &retMapInfo.QuestIndexForWarp, 0, &arrCB[16]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 17, SQL_C_SHORT, &retMapInfo.FrontPositionDistance, 0, &arrCB[17]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 18, SQL_C_UTINYINT, &retMapInfo.AutoRecoveryFlag, 0, &arrCB[18]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 19, SQL_C_USHORT, &retMapInfo.DefaultMapIndex, 0, &arrCB[19]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 20, SQL_C_SHORT, &retMapInfo.CityWarpTargetPosition.x, 0, &arrCB[20]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 21, SQL_C_SHORT, &retMapInfo.CityWarpTargetPosition.y, 0, &arrCB[21]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 22, SQL_C_SHORT, &retMapInfo.CityWarpTargetPosition.z, 0, &arrCB[22]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 23, SQL_C_SHORT, &retMapInfo.ClientMaxAltitudeHeight, 0, &arrCB[23]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 24, SQL_C_LONG, &retMapInfo.DayFogColor, 0, &arrCB[24]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 25, SQL_C_LONG, &retMapInfo.NightFogColor, 0, &arrCB[25]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 26, SQL_C_SHORT, &retMapInfo.DayFogStartDistance, 0, &arrCB[26]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 27, SQL_C_SHORT, &retMapInfo.DayFogEndDistance, 0, &arrCB[27]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 28, SQL_C_SHORT, &retMapInfo.NightFogStartDistance, 0, &arrCB[28]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 29, SQL_C_SHORT, &retMapInfo.NightFogEndDistance, 0, &arrCB[29]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 30, SQL_C_SHORT, &retMapInfo.DaySunDirection.x, 0, &arrCB[30]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 31, SQL_C_SHORT, &retMapInfo.DaySunDirection.y, 0, &arrCB[31]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 32, SQL_C_SHORT, &retMapInfo.DaySunDirection.z, 0, &arrCB[32]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 33, SQL_C_SHORT, &retMapInfo.NightSunDirection.x, 0, &arrCB[33]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 34, SQL_C_SHORT, &retMapInfo.NightSunDirection.y, 0, &arrCB[34]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 35, SQL_C_SHORT, &retMapInfo.NightSunDirection.z, 0, &arrCB[35]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 36, SQL_C_UTINYINT, &retMapInfo.WaterType, 0, &arrCB[36]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 37, SQL_C_UTINYINT, &retMapInfo.PKZoneFlag, 0, &arrCB[37]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 38, SQL_C_UTINYINT, &retMapInfo.TileRenderingFlag, 0, &arrCB[38]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 39, SQL_C_UTINYINT, &retMapInfo.SkyRenderingFlag, 0, &arrCB[39]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 40, SQL_C_UTINYINT, &retMapInfo.SunRenderingFlag, 0, &arrCB[40]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 41, SQL_C_UTINYINT, &retMapInfo.FogRenderingFlag, 0, &arrCB[41]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 42, SQL_C_LONG, &retMapInfo.VCNWarpObjectIndex, 0, &arrCB[42]); // 2006-12-08 by cmkwon
	SQLBindCol(i_pODBCStmt->m_hstmt, 43, SQL_C_LONG, &retMapInfo.ANIWarpObjectIndex, 0, &arrCB[43]); // 2006-12-08 by cmkwon
	SQLBindCol(i_pODBCStmt->m_hstmt, 44, SQL_C_USHORT, &retMapInfo.Dat, 0, &arrCB[44]); // 2007-03-15 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 45, SQL_C_USHORT, &retMapInfo.Map, 0, &arrCB[45]); // 2007-03-15 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 46, SQL_C_USHORT, &retMapInfo.Tex, 0, &arrCB[46]); // 2007-03-15 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 47, SQL_C_USHORT, &retMapInfo.Cloud, 0, &arrCB[47]); // 2007-03-15 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 48, SQL_C_USHORT, &retMapInfo.Sky, 0, &arrCB[48]); // 2007-03-15 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 49, SQL_C_USHORT, &retMapInfo.Nsky, 0, &arrCB[49]); // 2007-03-15 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 50, SQL_C_USHORT, &retMapInfo.Bgm, 0, &arrCB[50]); // 2007-03-15 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 51, SQL_C_LONG, &retMapInfo.TeleportWarpObjectIndex, 0, &arrCB[51]); // 2007-09-05 by dhjin
	SQLBindCol(i_pODBCStmt->m_hstmt, 52, SQL_C_USHORT, &retMapInfo.BeforeMapIndex2, 0, &arrCB[52]); // 2008-06-20 by cmkwon, EP3 백맵 시스템 수정(맵당 백맵을 2개 설정 할 수 있다) - DB에서 가져오기 코드 추가
	SQLBindCol(i_pODBCStmt->m_hstmt, 53, SQL_C_CHAR, retMapInfo.MapDescription, SIZE_MAX_MAP_DESCRIPTION, &arrCB[53]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	int nObjects = 0;
	memset(&retMapInfo, 0x00, sizeof(MAP_INFO));
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_mapMapInfo->push_back(retMapInfo);

		nObjects++;
		memset(&retMapInfo, 0x00, sizeof(MAP_INFO));
	}

	i_pODBCStmt->FreeStatement();

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadMysteryItemDropInfo(mmapINT2MYSTERY_ITEM_DROP *o_pmmapMysteryItemDropList, CODBCStatement *i_pODBCStmt, ez_map<INT, ITEM*> *i_pemapItemInfoList, int i_nMGServerID)
/// \brief		서버에서 가져오기
/// \author		cmkwon
/// \date		2007-05-11 ~ 2007-05-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::LoadMysteryItemDropInfo(mtmapINT2MYSTERY_ITEM_DROP* o_pmmapMysteryItemDropList, CODBCStatement* i_pODBCStmt, ez_map<INT, ITEM*>* i_pemapItemInfoList)
{
	o_pmmapMysteryItemDropList->clear(); // clear

	RETCODE ret;
	//////////////////////////////////////////////////////////////////////////
	// 2008-11-04 by dhjin, 럭키머신 - 미스테리 아이템 번호가 없는 불필요한 td_MysteryItemDropCount 테이블에 카운트를 지운다.
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_ValidCheck_td_MysteryItemDropCount
	-- DESC      : 2008-11-25 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_ValidCheck_td_MysteryItemDropCount
	AS
		DELETE FROM atum2_db_account.dbo.td_MysteryItemDropCount WHERE MysteryItemDropNum NOT IN 
		(SELECT MysteryItemDropNum FROM atum2_db_account.dbo.ti_MysteryItemDrop WITH(NOLOCK))
	GO
	**************************************************************************/
	SQLRETURN nSQLRet = SQLExecDirect(i_pODBCStmt->GetSTMTHandle(), PROCEDURE_081125_0276, SQL_NTS);
	if (nSQLRet != SQL_SUCCESS && nSQLRet != SQL_SUCCESS_WITH_INFO && nSQLRet != SQL_NO_DATA)
	{
		// error
		i_pODBCStmt->FreeStatement();

		// 2009-11-16 by cmkwon
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumDBHelper::LoadMysteryItemDropInfo# 10000 nSQLRet(%d) \r\n", nSQLRet);
		return 0;
	}
	i_pODBCStmt->FreeStatement();

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_load_ti_MysteryItemDrop
	-- DESC      : 2007-02-22 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_load_ti_MysteryItemDrop
		@i_ServerGroupID	INT
	AS
		SELECT D.MysteryItemDropNum,D.ReqUnitKind,D.ReqMinLevel,D.ReqMaxLevel,D.DropItemNum
				,D.MinCount,D.MaxCount,D.Probability,D.PrefixProbability,D.SuffixProbability
				,D.Period,D.CountPerPeriod,C.DropCount,D.starttime		-- // 2008-11-04 by dhjin, 럭키머신
		FROM atum2_db_account.dbo.ti_MysteryItemDrop AS D WITH(NOLOCK) LEFT JOIN 
				atum2_db_account.dbo.td_MysteryItemDropCount AS C WITH(NOLOCK)
				ON D.MysteryItemDropNum = C.MysteryItemDropNum AND C.ServerGroupID = @i_ServerGroupID AND D.DropItemNum = C.DropItemNum 
	GO
	**************************************************************************/
	SQLINTEGER arrCBLoad[2] = {SQL_NTS}; // 2008-11-04 by dhjin, 럭키머신
	nSQLRet = SQLExecDirect(i_pODBCStmt->GetSTMTHandle(), PROCEDURE_080822_0027, SQL_NTS); // 2008-11-04 by dhjin, 럭키머신
	if (nSQLRet != SQL_SUCCESS && nSQLRet != SQL_SUCCESS_WITH_INFO && nSQLRet != SQL_NO_DATA)
	{
		// error
		i_pODBCStmt->FreeStatement();

		// 2009-11-16 by cmkwon
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumDBHelper::LoadMysteryItemDropInfo# 11000 nSQLRet(%d) \r\n", nSQLRet);
		return 0;
	}

	SQLINTEGER arrCB[15];
	fill_n(arrCB, 15, SQL_NTS);
	MYSTERY_ITEM_DROP retMysteryDrop;
	SQL_TIMESTAMP_STRUCT StartDTime; // 2008-11-04 by dhjin, 럭키머신
	util::zero(&StartDTime, sizeof(SQL_TIMESTAMP_STRUCT)); // 2008-11-04 by dhjin, 럭키머신

	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &retMysteryDrop.MysteryItemDropNum, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &retMysteryDrop.ReqUnitKind, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_UTINYINT, &retMysteryDrop.ReqMinLevel, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &retMysteryDrop.ReqMaxLevel, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &retMysteryDrop.DropItemNum, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &retMysteryDrop.MinCount, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &retMysteryDrop.MaxCount, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &retMysteryDrop.Probability, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &retMysteryDrop.PrefixProbability, 0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &retMysteryDrop.SuffixProbability, 0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_SHORT, &retMysteryDrop.Period, 0, &arrCB[11]); // 2008-11-04 by dhjin, 럭키머신	
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_LONG, &retMysteryDrop.CountPerPeriod, 0, &arrCB[12]); // 2008-11-04 by dhjin, 럭키머신
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_LONG, &retMysteryDrop.DropCount, 0, &arrCB[13]); // 2008-11-04 by dhjin, 럭키머신
	SQLBindCol(i_pODBCStmt->m_hstmt, 14, SQL_C_TIMESTAMP, &StartDTime, 0, &arrCB[14]); // 2008-11-04 by dhjin, 럭키머신

	int nCnts = 0;
	util::zero(&retMysteryDrop, sizeof(retMysteryDrop));
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		ITEM* pItemInfo = i_pemapItemInfoList->findEZ(retMysteryDrop.DropItemNum);
		if (NULL == pItemInfo
			|| retMysteryDrop.ReqMinLevel > retMysteryDrop.ReqMaxLevel
			|| retMysteryDrop.MinCount > retMysteryDrop.MaxCount)
		{
			char szSystemLog[1024];
			sprintf(szSystemLog, "[ERROR] CAtumDBHelper::LoadMysteryItemDropInfo_ error !!, pItemInfo(%p) MysteryItemDropNum(%8d) ReqLevel(%3d~%3d) DropItemNum(%8d) DropCount(%4d~%4d)\r\n"
			        , pItemInfo, retMysteryDrop.MysteryItemDropNum, retMysteryDrop.ReqMinLevel, retMysteryDrop.ReqMaxLevel
			        , retMysteryDrop.DropItemNum, retMysteryDrop.MinCount, retMysteryDrop.MaxCount);
			g_pGlobal->WriteSystemLog(szSystemLog); // 2009-04-15 by cmkwon, 시스템 로그 수정 - 
			DbgOut(szSystemLog);
			continue;
		}
		retMysteryDrop.Starttime = StartDTime; // 2008-11-04 by dhjin, 럭키머신
		o_pmmapMysteryItemDropList->insert(pair<INT, MYSTERY_ITEM_DROP>(retMysteryDrop.MysteryItemDropNum, retMysteryDrop));

		nCnts++;
		util::zero(&retMysteryDrop, sizeof(retMysteryDrop));
		util::zero(&StartDTime, sizeof(SQL_TIMESTAMP_STRUCT)); // 2008-11-04 by dhjin, 럭키머신
	}

	i_pODBCStmt->FreeStatement();

	return TRUE; // 2009-11-16 by cmkwon, TRUE로 수정함.
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadMysteryItemDropInfoByOmi(mmapINT2MYSTERY_ITEM_DROP *o_pmmapMysteryItemDropList, CODBCStatement *i_pODBCStmt, ez_map<INT, ITEM> *i_pemapItemInfoList)
/// \brief		
/// \author		dhjin
/// \date		2008-11-04 ~ 2008-11-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::LoadMysteryItemDropInfoByOmi(mmapINT2MYSTERY_ITEM_DROP* o_pmmapMysteryItemDropList, CODBCStatement* i_pODBCStmt, ez_map<INT, ITEM>* i_pemapItemInfoList)
{
	o_pmmapMysteryItemDropList->clear(); // clear

	RETCODE ret;
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_load_ti_MysteryItemDropByOmi
	-- DESC      : 2007-02-22 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_load_ti_MysteryItemDropByOmi
	AS
		SELECT MysteryItemDropNum,ReqUnitKind,ReqMinLevel,ReqMaxLevel,DropItemNum
				,MinCount,MaxCount,Probability,PrefixProbability,SuffixProbability,Period	-- // 2008-12-30 by dhjin, 럭키머신 - 클라이언트 이펙트 처리 위해 수정
		FROM atum2_db_account.dbo.ti_MysteryItemDrop WITH(NOLOCK)
	GO
	**************************************************************************/

	SQLRETURN nSQLRet = SQLExecDirect(i_pODBCStmt->GetSTMTHandle(), PROCEDURE_081126_0277, SQL_NTS);
	if (nSQLRet != SQL_SUCCESS && nSQLRet != SQL_SUCCESS_WITH_INFO && nSQLRet != SQL_NO_DATA)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLINTEGER arrCB[12];
	fill_n(arrCB, 12, SQL_NTS);
	MYSTERY_ITEM_DROP retMysteryDrop;
	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &retMysteryDrop.MysteryItemDropNum, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &retMysteryDrop.ReqUnitKind, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_UTINYINT, &retMysteryDrop.ReqMinLevel, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &retMysteryDrop.ReqMaxLevel, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &retMysteryDrop.DropItemNum, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &retMysteryDrop.MinCount, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &retMysteryDrop.MaxCount, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &retMysteryDrop.Probability, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &retMysteryDrop.PrefixProbability, 0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &retMysteryDrop.SuffixProbability, 0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_SHORT, &retMysteryDrop.Period, 0, &arrCB[11]); // 2008-12-30 by dhjin, 럭키머신 - 클라이언트 이펙트 처리 위해 수정

	int nCnts = 0;
	util::zero(&retMysteryDrop, sizeof(retMysteryDrop));
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		ITEM* pItemInfo = &(i_pemapItemInfoList->findEZ(retMysteryDrop.DropItemNum));
		if (NULL == pItemInfo
			|| retMysteryDrop.ReqMinLevel > retMysteryDrop.ReqMaxLevel
			|| retMysteryDrop.MinCount > retMysteryDrop.MaxCount)
		{
			char szSystemLog[1024];
			sprintf(szSystemLog, "CAtumDBHelper::LoadMysteryItemDropInfoByOmi_ error !!, MysteryItemDropNum(%8d) ReqLevel(%3d~%3d) DropItemNum(%8d) DropCount(%4d~%4d)\r\n"
			        , retMysteryDrop.MysteryItemDropNum, retMysteryDrop.ReqMinLevel, retMysteryDrop.ReqMaxLevel
			        , retMysteryDrop.DropItemNum, retMysteryDrop.MinCount, retMysteryDrop.MaxCount);
			DbgOut(szSystemLog);
			continue;
		}
		o_pmmapMysteryItemDropList->insert(pair<INT, MYSTERY_ITEM_DROP>(retMysteryDrop.MysteryItemDropNum, retMysteryDrop));

		nCnts++;
		util::zero(&retMysteryDrop, sizeof(retMysteryDrop));
	}

	i_pODBCStmt->FreeStatement();

	return nCnts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadLuckyMachineOmi(CODBCStatement *i_pODBCStmt, vector<LUCKY_MACHINE_OMI> *o_pvectLuckyMachine, CLocalization *i_pLocalization)
/// \brief		// 2009-03-03 by dhjin, 럭키머신 수정안
/// \author		dhjin
/// \date		2009-03-03 ~ 2009-03-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::LoadLuckyMachineOmi(CODBCStatement* i_pODBCStmt, vector<LUCKY_MACHINE_OMI>* o_pvectLuckyMachine)
{
	o_pvectLuckyMachine->clear(); // clear

	RETCODE ret;
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_load_ti_LuckyMachineByOmi
	-- DESC      : 2009-03-03 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_load_ti_LuckyMachineByOmi
	AS
		SELECT MachineOrder, BuildingInx, MachineKind, MachineNum, CoinItemNum, SlotNum, SourceIndex, Description
		FROM atum2_db_account.dbo.ti_LuckyMachine WITH(NOLOCK)
	GO
	**************************************************************************/
	SQLRETURN nSQLRet = SQLExecDirect(i_pODBCStmt->GetSTMTHandle(), PROCEDURE_090303_0341, SQL_NTS);
	if (nSQLRet != SQL_SUCCESS && nSQLRet != SQL_SUCCESS_WITH_INFO && nSQLRet != SQL_NO_DATA)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLINTEGER arrCB[9];
	fill_n(arrCB, 9, SQL_NTS);
	LUCKY_MACHINE_OMI retLuckyMachine;
	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &retLuckyMachine.order, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &retLuckyMachine.BuildingInx, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_UTINYINT, &retLuckyMachine.MachineKind, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &retLuckyMachine.MachineNum, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &retLuckyMachine.CoinItemNum, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_UTINYINT, &retLuckyMachine.SlotNum, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &retLuckyMachine.SourceIndex, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, retLuckyMachine.szDescription, SIZE_MAX_LUCKYMACHINE_DESCRIPTION, &arrCB[8]);

	util::zero(&retLuckyMachine, sizeof(LUCKY_MACHINE_OMI));
	int nCnts = 0;
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_pvectLuckyMachine->push_back(retLuckyMachine);
		nCnts++;
		util::zero(&retLuckyMachine, sizeof(LUCKY_MACHINE_OMI));
	}

	i_pODBCStmt->FreeStatement();

	return nCnts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadInvokingWearItemDestParamNum(CODBCStatement *i_pODBCStmt, InvokingWearItemDestParamList *o_pInvokingWearItemDestParamNumList)
/// \brief		인피니티 - 발동류장착아이템
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::LoadInvokingWearItemDestParamNum(CODBCStatement* i_pODBCStmt, InvokingWearItemDestParamList* o_pInvokingWearItemDestParamNumList)
{
	o_pInvokingWearItemDestParamNumList->clear(); // clear
	RETCODE ret;
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_InvokingWearItemDestParamNum
	-- DESC				: -- // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 
	-- DESC				: 발동류 장착 아이템 착용 DestParam 값 가져오기
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_InvokingWearItemDestParamNum
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
		SELECT InvokingDestParamID, InvokingDestParam, InvokingDestParamValue, InvokingEffectIdx 
		FROM atum2_db_account.dbo.ti_InvokingWearItemDPNum WITH (NOLOCK);
	GO
	**********************************************************************/
	SQLRETURN nSQLRet = SQLExecDirect(i_pODBCStmt->GetSTMTHandle(), PROCEDURE_100210_0552, SQL_NTS);
	if (SQL_SUCCESS != nSQLRet && SQL_SUCCESS_WITH_INFO != nSQLRet && SQL_NO_DATA != nSQLRet)
	{
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLINTEGER cb[5];
	fill_n(cb, 5, SQL_NTS);
	INVOKING_WEAR_ITEM_DESTPARAM tmInvokingWearItemDestParam;
	util::zero(&tmInvokingWearItemDestParam, sizeof(INVOKING_WEAR_ITEM_DESTPARAM));

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmInvokingWearItemDestParam.InvokingDestParamID, 0, &cb[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_USHORT, &tmInvokingWearItemDestParam.InvokingDestParam, 0, &cb[2]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tmInvokingWearItemDestParam.InvokingDestParamValue, 0, &cb[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tmInvokingWearItemDestParam.InvokingEffectIdx, 0, &cb[4]);

	int nCnts = 0;
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_pInvokingWearItemDestParamNumList->push_back(tmInvokingWearItemDestParam);
		util::zero(&tmInvokingWearItemDestParam, sizeof(INVOKING_WEAR_ITEM_DESTPARAM));
		nCnts++;
	}

	i_pODBCStmt->FreeStatement();

	return nCnts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadInvokingWearItemDestParamNumByUse(CODBCStatement *i_pODBCStmt, InvokingWearItemDestParamList *o_pInvokingWearItemDestParamNumByUseList)
/// \brief		인피니티 - 발동류장착아이템
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::LoadInvokingWearItemDestParamNumByUse(CODBCStatement* i_pODBCStmt, InvokingWearItemDestParamList* o_pInvokingWearItemDestParamNumByUseList)
{
	o_pInvokingWearItemDestParamNumByUseList->clear(); // clear
	RETCODE ret;
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_InvokingWearItemDestParamNum
	-- DESC				: -- // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 
	-- DESC				: 발동류 장착 아이템 사용 DestParam 값 가져오기
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_InvokingWearItemDestParamNumByUse
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
		SELECT InvokingDestParamIDByUse, InvokingDestParamByUse, InvokingDestParamValueByUse, InvokingEffectIdxbyUse 
		FROM atum2_db_account.dbo.ti_InvokingWearItemDestParamNumByUse WITH (NOLOCK);
	GO
	**********************************************************************/
	SQLRETURN nSQLRet = SQLExecDirect(i_pODBCStmt->GetSTMTHandle(), PROCEDURE_100210_0555, SQL_NTS);
	if (SQL_SUCCESS != nSQLRet && SQL_SUCCESS_WITH_INFO != nSQLRet && SQL_NO_DATA != nSQLRet)
	{
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLINTEGER cb[5];
	fill_n(cb, 5, SQL_NTS);
	INVOKING_WEAR_ITEM_DESTPARAM tmInvokingWearItemDestParamByUse;
	util::zero(&tmInvokingWearItemDestParamByUse, sizeof(INVOKING_WEAR_ITEM_DESTPARAM));

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmInvokingWearItemDestParamByUse.InvokingDestParamID, 0, &cb[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_USHORT, &tmInvokingWearItemDestParamByUse.InvokingDestParam, 0, &cb[2]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tmInvokingWearItemDestParamByUse.InvokingDestParamValue, 0, &cb[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &tmInvokingWearItemDestParamByUse.InvokingEffectIdx, 0, &cb[4]);

	int nCnts = 0;
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_pInvokingWearItemDestParamNumByUseList->push_back(tmInvokingWearItemDestParamByUse);
		util::zero(&tmInvokingWearItemDestParamByUse, sizeof(INVOKING_WEAR_ITEM_DESTPARAM));
		nCnts++;
	}

	i_pODBCStmt->FreeStatement();

	return nCnts;
}

int CAtumDBHelper::LoadBurningMap(CODBCStatement* i_pODBCStmt, BurningMapInfoList* o_pBurningMapInfo)
{ // 2010-08-05 by dhjin, 버닝맵 - 
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_Load_BurningMap
	AS
		SELECT BuringMapUID, MapIndex, ReqUnitKind, ReqMinLv, ReqMaxLv
		FROM dbo.ti_BurningMap WITH(NOLOCK)
	GO
	**************************************************************************/
	RETCODE ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_100805_0566, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLINTEGER cb[6];
	fill_n(cb, 6, SQL_NTS);
	BURNING_MAP_INFO tmBurningMapInfo;
	util::zero(&tmBurningMapInfo, sizeof(BURNING_MAP_INFO));

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmBurningMapInfo.BurningMapUID, 0, &cb[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_USHORT, &tmBurningMapInfo.MapIndex, 0, &cb[2]); // 2010-09-07 by shcho, 버닝맵 - 자료형 수정
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_USHORT, &tmBurningMapInfo.ReqUnitKind, 0, &cb[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &tmBurningMapInfo.ReqMinLv, 0, &cb[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &tmBurningMapInfo.ReqMaxLv, 0, &cb[5]);

	o_pBurningMapInfo->clear();
	int nCnts = 0;
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_pBurningMapInfo->push_back(tmBurningMapInfo);
		util::zero(&tmBurningMapInfo, sizeof(BURNING_MAP_INFO));
		nCnts++;
	}
	i_pODBCStmt->FreeStatement();

	return nCnts;
}

////////////////////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-06-15 by shcho&hslee 펫시스템 - 펫 기본 정보 DB에서 읽어오기.
/// \author		shcho & hslee
/// \date		2010-06-15 ~ 
/// \warning	
///
/// \param		Uniquenumber int, PetIndex int, Petkind int, DefaultPetName varchar(40), EnableName int, EnableLevel int	
/// \return		
////////////////////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadPetBaseDataInfo(CODBCStatement* i_pODBCStmt, vectorPetDataInfo* o_PetDataInfo)
{
	if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0569))
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadPetBaseDataInfo call PROCEDURE_100615_0569 \r\n");
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	tPET_BASE_ALL_DATA tmPetData;
	util::zero(&tmPetData.BaseData, sizeof(tPET_BASEDATA)); // 2015-05-15 Future, memsetting the vector in tPET_BASE_ALL_DATA to 0 causes memory leaks

	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
	//
	// 기존 
	//
	// 	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	// 
	// 	SQLBindCol(i_pODBCStmt->m_hstmt,	1,	SQL_C_LONG,		&tmPetData.BaseData.UniqueNumber,		0, &arrCB[0]);
	// 	SQLBindCol(i_pODBCStmt->m_hstmt,	2,	SQL_C_LONG,		&tmPetData.BaseData.PetIndex,			0, &arrCB[1]);
	// 	SQLBindCol(i_pODBCStmt->m_hstmt,	3,	SQL_C_LONG,		&tmPetData.BaseData.PetKind,			0, &arrCB[2]);
	// 	SQLBindCol(i_pODBCStmt->m_hstmt,	4,	SQL_C_CHAR,		&tmPetData.BaseData.szPetName,			SIZE_MAX_PET_NAME, &arrCB[3]);
	// 	SQLBindCol(i_pODBCStmt->m_hstmt,	5,	SQL_C_LONG,		&tmPetData.BaseData.EnableName,			0, &arrCB[4]);
	// 	SQLBindCol(i_pODBCStmt->m_hstmt,	6,	SQL_C_LONG,		&tmPetData.BaseData.EnableLevel,		0, &arrCB[5]);

	/////////
	// 수정
	int iCBCount = 0;
	SQLINTEGER arrCB[8];
	fill_n(arrCB, 8, SQL_NTS);

	SQLBindCol(i_pODBCStmt->m_hstmt, ++iCBCount, SQL_C_LONG, &tmPetData.BaseData.PetIndex, 0, &arrCB[iCBCount]);
	SQLBindCol(i_pODBCStmt->m_hstmt, ++iCBCount, SQL_C_CHAR, &tmPetData.BaseData.PetName, SIZE_MAX_PET_NAME, &arrCB[iCBCount]);
	SQLBindCol(i_pODBCStmt->m_hstmt, ++iCBCount, SQL_C_LONG, &tmPetData.BaseData.PetKind, 0, &arrCB[iCBCount]);
	SQLBindCol(i_pODBCStmt->m_hstmt, ++iCBCount, SQL_C_BIT, &tmPetData.BaseData.EnableName, 0, &arrCB[iCBCount]);
	SQLBindCol(i_pODBCStmt->m_hstmt, ++iCBCount, SQL_C_BIT, &tmPetData.BaseData.EnableLevel, 0, &arrCB[iCBCount]);

	SQLBindCol(i_pODBCStmt->m_hstmt, ++iCBCount, SQL_C_LONG, &tmPetData.BaseData.MaxLevel, 0, &arrCB[iCBCount]);
	SQLBindCol(i_pODBCStmt->m_hstmt, ++iCBCount, SQL_C_LONG, &tmPetData.BaseData.BaseSocketCount, 0, &arrCB[iCBCount]);
	// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
	///////////////////////////////////////////////////////////////////////////////////////

	int rtn = 0;
	RETCODE ret;
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_PetDataInfo->push_back(tmPetData);
		util::zero(&tmPetData.BaseData, sizeof(tPET_BASEDATA)); // 2015-05-15 Future, memsetting the vector in tPET_BASE_ALL_DATA to 0 causes memory leaks

		rtn++;
	}
	i_pODBCStmt->FreeStatement();

	return rtn;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-06-15 by shcho&hslee 펫시스템 - 펫 레벨업 관련 정보 DB에서 읽어오기.
/// \author		shcho & hslee
/// \date		2010-06-15 ~ 
/// \warning	
///
/// \param	UniqueNumber int,PetLevel int,PetEXP float,Stamina float,SourceIndex int,
///			PetSKillslot0 int,PetSKillslot1 int,PetSKillslot2 int,PetSKillslot3 int,PetSKillslot4 int,PetSKillslot5 int	
/// \return		
/////////////////////////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadPetLevelDataInfo(CODBCStatement* i_pODBCStmt, vectorPetDataInfo* o_PetDataInfo)
{
	int rtn = 0;
	RETCODE ret;

	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
	//
	// 기존 
	// 	SQLINTEGER	arrCB[12] = 
	// 	{
	// 		SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, 
	// 		SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, 
	// 		SQL_NTS, SQL_NTS
	// 	};
	// 수정
	SQLINTEGER arrCB[9];
	fill_n(arrCB, 9, SQL_NTS);
	// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
	///////////////////////////////////////////////////////////////////////////////////////

	tPET_LEVEL_DATA tmPetLevelData;
	util::zero(&tmPetLevelData, sizeof(tmPetLevelData));

	short shColumnNum = 0; // hskim, 파트너 시스템 2차 - 자료 구조 결정

	vectorPetDataInfo::iterator iter = o_PetDataInfo->begin();

	for (; iter != o_PetDataInfo->end(); ++iter)
	{
		///////////////////////////////////////////////////////////////////////////////////////
		// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		//
		// 기존 
		// 
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &iter->BaseData.UniqueNumber, 0, NULL);
		// 		
		// 		if(FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0570))
		// 		{
		// 			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadPetLevelDataInfo call PROCEDURE_100615_0570 \r\n");
		// 			i_pODBCStmt->FreeStatement();
		// 			return 0;
		// 		}
		//
		//		shColumnNum = 1;
		//
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.Level,					0, &arrCB[0] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_DOUBLE,	&tmPetLevelData.NeedExp,				0, &arrCB[1] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_DOUBLE,	&tmPetLevelData.Stamina,				0, &arrCB[2] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.CitySourceIndex,		0, &arrCB[3] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.FieldSourceIndex,		0, &arrCB[4] ); shColumnNum++;
		// 				// 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리 총알 인덱스 필드 추가
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.PetAttackIndex,			0, &arrCB[5] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.PetSkillItemIndex[0],	0, &arrCB[6] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.PetSkillItemIndex[1],	0, &arrCB[7] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.PetSkillItemIndex[2],	0, &arrCB[8] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.PetSkillItemIndex[3],	0, &arrCB[9] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.PetSkillItemIndex[4],	0, &arrCB[10] ); shColumnNum++;
		// 		SQLBindCol( i_pODBCStmt->m_hstmt,	shColumnNum,	SQL_C_LONG,		&tmPetLevelData.PetSkillItemIndex[5],	0, &arrCB[11] ); shColumnNum++;

		/////////
		// 수정

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &iter->BaseData.PetIndex, 0, NULL);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0570))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadPetLevelDataInfo call PROCEDURE_100615_0570 \r\n");
			i_pODBCStmt->FreeStatement();
			return 0;
		}

		shColumnNum = 0;

		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_LONG, &tmPetLevelData.PetIndex, 0, &arrCB[shColumnNum]);
		shColumnNum++;
		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_LONG, &tmPetLevelData.Level, 0, &arrCB[shColumnNum]);
		shColumnNum++;
		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_DOUBLE, &tmPetLevelData.NeedExp, 0, &arrCB[shColumnNum]);
		shColumnNum++;
		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_LONG, &tmPetLevelData.UseWeaponIndex, 0, &arrCB[shColumnNum]);
		shColumnNum++;
		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_UTINYINT, &tmPetLevelData.SlotCountSkill, 0, &arrCB[shColumnNum]);
		shColumnNum++;

		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_UTINYINT, &tmPetLevelData.SlotCountSocket, 0, &arrCB[shColumnNum]);
		shColumnNum++;
		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_UTINYINT, &tmPetLevelData.KitLevelHP, 0, &arrCB[shColumnNum]);
		shColumnNum++;
		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_UTINYINT, &tmPetLevelData.KitLevelShield, 0, &arrCB[shColumnNum]);
		shColumnNum++;
		SQLBindCol(i_pODBCStmt->m_hstmt, (shColumnNum + 1), SQL_C_UTINYINT, &tmPetLevelData.KitLevelSP, 0, &arrCB[shColumnNum]);
		shColumnNum++;
		// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		///////////////////////////////////////////////////////////////////////////////////////

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			tmPetLevelData.PetIndex = iter->BaseData.PetIndex; // 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
			iter->vectPetLevelDataInfo.push_back(tmPetLevelData);
			util::zero(&tmPetLevelData, sizeof(tPET_LEVEL_DATA));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	i_pODBCStmt->FreeStatement();

	return rtn;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업. - 펫 오퍼레이터 정보.
/// \author		shcho & jskim
/// \date		2010-08-17 ~ 
/// \warning	
///
/// \param		
/// \return		
/////////////////////////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadOperatorDataInfo(CODBCStatement* i_pODBCStmt, vectorOperatorInfo* o_OperatorInfo)
{
	int rtn = 0;
	RETCODE ret;

	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);

	OperatorInfo tmOpData;
	util::zero(&tmOpData, sizeof(tmOpData));

	if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0577))
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadOperatorDataInfo call PROCEDURE_100615_0577 \r\n");
		i_pODBCStmt->FreeStatement();
		return 0;
	}
	/* int ActionType, int Itemnum, int FunctionIndex, double FunctionValue, int SourceIndex, char ActionDesc[128]; */
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmOpData.ActionType, 0, &arrCB[0]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmOpData.Itemnum, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmOpData.FunctionIndex, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_DOUBLE, &tmOpData.FunctionValue, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &tmOpData.SourceIndex, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, &tmOpData.ActionDesc, 128, &arrCB[5]);

	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		o_OperatorInfo->push_back(tmOpData);
		util::zero(&tmOpData, sizeof(tmOpData));
		rtn++;
	}

	i_pODBCStmt->FreeStatement();

	return rtn;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
/// \author		shcho & jskim
/// \date		2010-08-17 ~ 
/// \warning	
///
/// \param		
/// \return		
/////////////////////////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveOperatorData(CODBCStatement* i_pODBCStmt, vectorOperatorInfo* i_OperatorData, HWND hWnd)
{
	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0579, SQL_NTS); //삭제(delete) 프로시저를 추가해야 한다.
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_OperatorData->size() - 1));

	//Insert 
	int nObjects = 0;

	OperatorInfo temp_Info;
	vectorOperatorInfo::iterator itr(i_OperatorData->begin());
	SQLRETURN RET;

	for (; itr != i_OperatorData->end(); itr++)
	{
		temp_Info = *itr;

		//SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[0], 0, &cb[7]);

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.ActionType, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.Itemnum, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.FunctionIndex, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &temp_Info.FunctionValue, 0, &arrCB[3]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.SourceIndex, 0, &arrCB[4]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PET_OPERATOR_DESCRIPTION, 0, &temp_Info.ActionDesc, 0, &arrCB[5]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0578); //삽입(insert)
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : FunctionIndex(%d) !!", temp_Info.FunctionIndex);
			MessageBox(NULL, err, "ti_OperatorAction", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveDissolutionItemData(CODBCStatement *i_pODBCStmt, vectorDissolutionItemInfo* i_DissolutionItemInfo)
/// \brief		// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.		
/// \author		shcho&jskim
/// \date		2010-08-31 ~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveDissolutionItemData(CODBCStatement* i_pODBCStmt, vectorDissolutionItemInfo* i_DissolutionItemInfo, HWND hWnd)
{
	SQLINTEGER arrCB[13];
	fill_n(arrCB, 13, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100831_0002, SQL_NTS); //삭제(delete) 프로시저를 추가해야 한다.
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)(*i_DissolutionItemInfo).size() - 1));

	//Insert 
	int nObjects = 0;

	tDissolutionItemInfo* temp_Info;
	vectorDissolutionItemInfo::iterator itr(i_DissolutionItemInfo->begin());
	SQLRETURN RET;

	for (; itr != i_DissolutionItemInfo->end(); itr++)
	{
		temp_Info = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->SourceItemnum, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemNum[0], 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemMinCount[0], 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemMaxCount[0], 0, &arrCB[3]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemProbabillity[0], 0, &arrCB[4]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemNum[1], 0, &arrCB[5]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemMinCount[1], 0, &arrCB[6]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemMaxCount[1], 0, &arrCB[7]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemProbabillity[1], 0, &arrCB[8]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemNum[2], 0, &arrCB[9]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemMinCount[2], 0, &arrCB[10]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemMaxCount[2], 0, &arrCB[11]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->ResultItemProbabillity[2], 0, &arrCB[12]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100831_0003); //삽입(insert)
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : SourceItemnum(%d) !!", temp_Info->SourceItemnum);
			MessageBox(NULL, err, "ti_DissolutionItem", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadDissolutionItemDataInfo(CODBCStatement *i_pODBCStmt, vectorDissolutionItemInfo* o_DissolutionItemInfo)
/// \brief		// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
/// \author		shcho&jskim
/// \date		2010-08-31 ~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadDissolutionItemDataInfo(CODBCStatement* i_pODBCStmt, void* o_DissolutionItemInfo, BOOL ToolFlag /* = TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	tDissolutionItemInfo tmDisItemData;
	memset(&tmDisItemData, 0, sizeof(tmDisItemData));

	SQLINTEGER arrCB[13];
	fill_n(arrCB, 13, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		vectorDissolutionItemInfo* ptmvectDisIteminfo = (vectorDissolutionItemInfo*)(o_DissolutionItemInfo);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_100831_0001))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadDissolutionItemDataInfo call PROCEDURE_100831_0001 \r\n");
			i_pODBCStmt->FreeStatement();
			return 0;
		}
		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmDisItemData.SourceItemnum, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmDisItemData.ResultItemNum[0], 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmDisItemData.ResultItemMinCount[0], 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmDisItemData.ResultItemMaxCount[0], 0, &arrCB[3]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &tmDisItemData.ResultItemProbabillity[0], 0, &arrCB[4]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmDisItemData.ResultItemNum[1], 0, &arrCB[5]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmDisItemData.ResultItemMinCount[1], 0, &arrCB[6]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &tmDisItemData.ResultItemMaxCount[1], 0, &arrCB[7]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &tmDisItemData.ResultItemProbabillity[1], 0, &arrCB[8]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &tmDisItemData.ResultItemNum[2], 0, &arrCB[9]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_LONG, &tmDisItemData.ResultItemMinCount[2], 0, &arrCB[10]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_LONG, &tmDisItemData.ResultItemMaxCount[2], 0, &arrCB[11]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_LONG, &tmDisItemData.ResultItemProbabillity[2], 0, &arrCB[12]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			ptmvectDisIteminfo->push_back(tmDisItemData);
			util::zero(&tmDisItemData, sizeof(tmDisItemData));
			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	else if (ToolFlag == FALSE) // 이것은 필드서버에서 로드 하기위해 사용
	{
		mtmapDissolutionItemInfo* ptmmapDisItemData = (mtmapDissolutionItemInfo*)(o_DissolutionItemInfo);


		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_100831_0001))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadDissolutionItemDataInfo call PROCEDURE_100831_0001 \r\n");
			i_pODBCStmt->FreeStatement();
			return 0;
		}
		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmDisItemData.SourceItemnum, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmDisItemData.ResultItemNum[0], 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmDisItemData.ResultItemMinCount[0], 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmDisItemData.ResultItemMaxCount[0], 0, &arrCB[3]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &tmDisItemData.ResultItemProbabillity[0], 0, &arrCB[4]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmDisItemData.ResultItemNum[1], 0, &arrCB[5]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmDisItemData.ResultItemMinCount[1], 0, &arrCB[6]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &tmDisItemData.ResultItemMaxCount[1], 0, &arrCB[7]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &tmDisItemData.ResultItemProbabillity[1], 0, &arrCB[8]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &tmDisItemData.ResultItemNum[2], 0, &arrCB[9]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_LONG, &tmDisItemData.ResultItemMinCount[2], 0, &arrCB[10]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_LONG, &tmDisItemData.ResultItemMaxCount[2], 0, &arrCB[11]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_LONG, &tmDisItemData.ResultItemProbabillity[2], 0, &arrCB[12]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			// ptmmapDisItemData->insert(std::make_pair(tmDisItemData.SourceItemnum,tmDisItemData));
			ptmmapDisItemData->insert(pair<INT, tDissolutionItemInfo>(tmDisItemData.SourceItemnum, tmDisItemData));
			util::zero(&tmDisItemData, sizeof(tmDisItemData));
			rtn++;
		}
		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

// start 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveMonsterMultiTargetData(CODBCStatement *i_pODBCStmt, vectorMonsterMultiTarget* i_MonsterMultiTarget, HWND hWnd)
/// \brief		// 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가		
/// \author		hskim
/// \date		2011-03-17 ~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveMonsterMultiTargetData(CODBCStatement* i_pODBCStmt, vectorMonsterMultiTarget* i_MonsterMultiTarget, HWND hWnd)
{
	SQLINTEGER arrCB[5];
	fill_n(arrCB, 5, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_110317_0002, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)(*i_MonsterMultiTarget).size() - 1));

	//Insert 
	int nObjects = 0;

	MONSTER_MULTI_TARGET* temp_Info;
	vectorMonsterMultiTarget::iterator itr(i_MonsterMultiTarget->begin());
	SQLRETURN RET;

	for (; itr != i_MonsterMultiTarget->end(); itr++)
	{
		temp_Info = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->MonsterIndex, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info->PointIndex, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &temp_Info->TargetPosition.x, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &temp_Info->TargetPosition.y, 0, &arrCB[3]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &temp_Info->TargetPosition.z, 0, &arrCB[4]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_110317_0003);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MonsterIndex(%d) PointIndex(%d)!!", temp_Info->MonsterIndex, temp_Info->PointIndex);
			MessageBox(NULL, err, "ti_MonsterMultiTarget", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadMonsterMultiTargetData(CODBCStatement *i_pODBCStmt, void* o_MonsterMultiTarget, BOOL ToolFlag /* = TRUE*/)
/// \brief		// 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
/// \author		hskim
/// \date		2011-03-17 ~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadMonsterMultiTargetData(CODBCStatement* i_pODBCStmt, void* o_MonsterMultiTarget, BOOL ToolFlag /* = TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	MONSTER_MULTI_TARGET tmMonsterMultiTargetData;
	util::zero(&tmMonsterMultiTargetData, sizeof(tmMonsterMultiTargetData));

	SQLINTEGER arrCB[13];
	fill_n(arrCB, 13, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		vectorMonsterMultiTarget* ptmvectMonsterMultiTargetinfo = (vectorMonsterMultiTarget*)(o_MonsterMultiTarget);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_110317_0001))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadMonsterMultiTargetData call PROCEDURE_110317_0001 \r\n");
			i_pODBCStmt->FreeStatement();
			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmMonsterMultiTargetData.MonsterIndex, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmMonsterMultiTargetData.PointIndex, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &tmMonsterMultiTargetData.TargetPosition.x, 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_FLOAT, &tmMonsterMultiTargetData.TargetPosition.y, 0, &arrCB[3]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_FLOAT, &tmMonsterMultiTargetData.TargetPosition.z, 0, &arrCB[4]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			ptmvectMonsterMultiTargetinfo->push_back(tmMonsterMultiTargetData);
			util::zero(&tmMonsterMultiTargetData, sizeof(tmMonsterMultiTargetData));
			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		// 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
		// CMonsterDBAccess::GetAllMonster() 함수에서 로드처리

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

// end 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveWayPointData(CODBCStatement *i_pODBCStmt, vectorWayPoint *i_pWayPoint, HWND hWnd)
/// \brief		// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현	
/// \author		hskim
/// \date		2011-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveWayPointData(CODBCStatement* i_pODBCStmt, vectorWayPoint* i_pWayPoint, HWND hWnd)
{
	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_110529_0005, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pWayPoint->size() - 1));

	//Insert
	int nObjects = 0;
	SWAY_POINT* pInfo;
	vectorWayPoint::iterator itr = i_pWayPoint->begin();
	SQLRETURN RET;

	for (; itr != i_pWayPoint->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInfo->dwPatternIndex, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInfo->dwSequenceNum, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pInfo->byPostionAttribute, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pInfo->vPoint.x, 0, &arrCB[3]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pInfo->vPoint.y, 0, &arrCB[4]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pInfo->vPoint.z, 0, &arrCB[5]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_110529_0006);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : WayPoint PatternIndex(%d) SequenceNum(%d) PostionAttribute(%d) X(%f) Y(%f) Z(%f)!!",
			        pInfo->dwPatternIndex, pInfo->dwSequenceNum, pInfo->byPostionAttribute, pInfo->vPoint.x, pInfo->vPoint.y, pInfo->vPoint.z);
			MessageBox(NULL, err, "ti_WayPoint", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadWayPointDataInfo(CODBCStatement *i_pODBCStmt, void* o_MonsterMultiTarget, BOOL ToolFlag /* = TRUE*/)
/// \brief		// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
/// \author		hskim
/// \date		2011-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadWayPointDataInfo(CODBCStatement* i_pODBCStmt, void* o_pWayPointManager, BOOL ToolFlag /* = TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	SWAY_POINT tmWayPoint;
	util::zero(&tmWayPoint, sizeof(tmWayPoint));

	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		CWayPointManager* pWayPointManager = (CWayPointManager *)(o_pWayPointManager);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_110529_0004))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadWayPointDataInfo call PROCEDURE_110529_0004 \r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmWayPoint.dwPatternIndex, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmWayPoint.dwSequenceNum, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_TINYINT, &tmWayPoint.byPostionAttribute, 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_FLOAT, &tmWayPoint.vPoint.x, 0, &arrCB[3]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_FLOAT, &tmWayPoint.vPoint.y, 0, &arrCB[4]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_FLOAT, &tmWayPoint.vPoint.z, 0, &arrCB[5]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			D3DXVECTOR3 vecPosition(tmWayPoint.vPoint.x, tmWayPoint.vPoint.y, tmWayPoint.vPoint.z);
			SWAY_POINT WayPoint(tmWayPoint.dwPatternIndex, tmWayPoint.dwSequenceNum, tmWayPoint.byPostionAttribute, tmWayPoint.vPoint);
			pWayPointManager->AddWayPoint(&WayPoint);

			util::zero(&tmWayPoint, sizeof(tmWayPoint));
			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveWayPointMoveData(CODBCStatement *i_pODBCStmt, vectorWayPointMove *i_pWayPointMove, HWND hWnd)
/// \brief		// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현	
/// \author		hskim
/// \date		2011-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveWayPointMoveData(CODBCStatement* i_pODBCStmt, vectorWayPointMove* i_pWayPointMove, HWND hWnd)
{
	SQLINTEGER arrCB[2];
	fill_n(arrCB, 2, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_110529_0002, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pWayPointMove->size() - 1));

	//Insert 
	int nObjects = 0;
	SWAY_POINT_MOVE* pInfo = NULL;
	vectorWayPointMove::iterator itr = i_pWayPointMove->begin();
	SQLRETURN RET;

	for (; itr != i_pWayPointMove->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInfo->dwIndex, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pInfo->byCompletionAction, 0, &arrCB[1]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_110529_0003);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : WayPointMove Index(%d) CompletionAction(%d)!!", pInfo->dwIndex, pInfo->byCompletionAction);
			MessageBox(NULL, err, "ti_WayPointMove", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadWayPointMoveDataInfo(CODBCStatement *i_pODBCStmt, void* o_pWayPointManager, BOOL ToolFlag /* = TRUE*/)
/// \brief		// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
/// \author		hskim
/// \date		2011-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadWayPointMoveDataInfo(CODBCStatement* i_pODBCStmt, void* o_pWayPointManager, BOOL ToolFlag /* = TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	SWAY_POINT_MOVE tmWayPointMove;
	util::zero(&tmWayPointMove, sizeof(tmWayPointMove));

	SQLINTEGER arrCB[2];
	fill_n(arrCB, 2, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		CWayPointManager* pWayPointManager = (CWayPointManager *)(o_pWayPointManager);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_110529_0001))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadWayPointMoveDataInfo call PROCEDURE_110529_0001 \r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmWayPointMove.dwIndex, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &tmWayPointMove.byCompletionAction, 0, &arrCB[1]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			CWayPointMove PatternData(tmWayPointMove.dwIndex, tmWayPointMove.byCompletionAction);
			pWayPointManager->AddPattern(&PatternData);

			util::zero(&tmWayPointMove, sizeof(tmWayPointMove));
			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

// start 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerMapBuff(CODBCStatement *i_pODBCStmt, vectorTriggerMapBuff *i_pTriggerMapBuff, HWND hWnd)
/// \brief		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
/// \author		hskim
/// \date		2011-10-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerMapBuff(CODBCStatement* i_pODBCStmt, vectorTriggerMapBuff* i_pTriggerMapBuff, HWND hWnd)
{
	SQLINTEGER arrCB[2];
	fill_n(arrCB, 2, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111010_0002, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerMapBuff->size() - 1));

	//Insert
	int nObjects = 0;
	STRIGGER_MAP_BUFF* pInfo;
	vectorTriggerMapBuff::iterator itr = i_pTriggerMapBuff->begin();
	SQLRETURN RET;

	for (; itr != i_pTriggerMapBuff->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInfo->MapIndex, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInfo->ItemNum, 0, &arrCB[1]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111010_0003);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MapBuff MapIndex(%d) ItemNum(%d)!!",
			        pInfo->MapIndex, pInfo->ItemNum);
			MessageBox(NULL, err, "ti_MapBuff", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerMapBuffInfo(CODBCStatement *i_pODBCStmt, void* i_pTriggerMapBuff, BOOL ToolFlag /* = TRUE*/)
/// \brief		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
/// \author		hskim
/// \date		2011-10-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerMapBuffInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerMapBuff, BOOL ToolFlag /* = TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_MAP_BUFF tmMapBuff;
	util::zero(&tmMapBuff, sizeof(tmMapBuff));

	SQLINTEGER arrCB[2];
	fill_n(arrCB, 2, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerMapBuff* ptmvectTriggerMapBuff = (vectorTriggerMapBuff *)(i_pTriggerMapBuff);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_111010_0001))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadTriggerMapBuffInfo call PROCEDURE_111010_0001\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmMapBuff.MapIndex, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmMapBuff.ItemNum, 0, &arrCB[1]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmvectTriggerMapBuff != NULL)
			{
				ptmvectTriggerMapBuff->push_back(tmMapBuff);
			}

			util::zero(&tmMapBuff, sizeof(tmMapBuff));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

// end 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerMap(CODBCStatement *i_pODBCStmt, vectorTriggerMap *i_pTriggerMap, HWND hWnd)
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerMap(CODBCStatement* i_pODBCStmt, vectorTriggerMap* i_pTriggerMap, HWND hWnd)
{
	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0002, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerMap->size() - 1));

	//Insert
	int nObjects = 0;
	STRIGGER_MAP* pInfo;
	vectorTriggerMap::iterator itr = i_pTriggerMap->begin();
	SQLRETURN RET;

	int a = i_pTriggerMap->size();

	for (; itr != i_pTriggerMap->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->MapTriggerID, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->MapIndex, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pInfo->MapChannel, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->FunctionID, 0, &arrCB[3]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0003);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MapTrigger MapTriggerID(%d) MapIndex(%d) MapChannel(%d) FunctionID(%d)!!",
			        pInfo->MapTriggerID, pInfo->MapIndex, pInfo->MapChannel, pInfo->FunctionID);
			MessageBox(NULL, err, "ti_MapTrigger", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerMapInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerMap, BOOL ToolFlag /*= TRUE*/)
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerMapInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerMap, BOOL ToolFlag /*= TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_MAP tmTriggerMap;
	util::zero(&tmTriggerMap, sizeof(tmTriggerMap));

	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerMap* ptmTriggerMap = (vectorTriggerMap *)(i_pTriggerMap);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0001))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadTriggerMapInfo call PROCEDURE_111028_0001\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_SLONG, &tmTriggerMap.MapTriggerID, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_SLONG, &tmTriggerMap.MapIndex, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_USHORT, &tmTriggerMap.MapChannel, 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_SLONG, &tmTriggerMap.FunctionID, 0, &arrCB[3]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerMap != NULL)
			{
				ptmTriggerMap->push_back(tmTriggerMap);
			}

			util::zero(&tmTriggerMap, sizeof(tmTriggerMap));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerFunctionCrystal(CODBCStatement *i_pODBCStmt, vectorTriggerFunctionCrystal *i_pTriggerFunctionCrystal, HWND hWnd)
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerFunctionCrystal(CODBCStatement* i_pODBCStmt, vectorTriggerFunctionCrystal* i_pTriggerFunctionCrystal, HWND hWnd)
{
	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0005, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerFunctionCrystal->size() - 1));

	//Insert
	int nObjects = 0;
	STRIGGER_FUNCTION_CRYSTAL* pInfo;
	vectorTriggerFunctionCrystal::iterator itr = i_pTriggerFunctionCrystal->begin();
	SQLRETURN RET;

	for (; itr != i_pTriggerFunctionCrystal->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->FunctionID, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->CrystalGroupID, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->PeriodTime, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pInfo->RandomSequence, 0, &arrCB[3]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0006);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TriggerFunctionCrystal FunctionID(%d) CrystalGroupID(%d) PeriodTime(%d) RandomSequence(%d)!!",
			        pInfo->FunctionID, pInfo->CrystalGroupID, pInfo->PeriodTime, pInfo->RandomSequence);
			MessageBox(NULL, err, "ti_TriggerFunctionCrystal", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerFunctionCrystalInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerFunctionCrystal, BOOL ToolFlag /*= TRUE*/)
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerFunctionCrystalInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerFunctionCrystal, BOOL ToolFlag /*= TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_FUNCTION_CRYSTAL tmTriggerFunctionCrystal;
	util::zero(&tmTriggerFunctionCrystal, sizeof(tmTriggerFunctionCrystal));

	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerFunctionCrystal* ptmTriggerFunctionCrystal = (vectorTriggerFunctionCrystal *)(i_pTriggerFunctionCrystal);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0004))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadTriggerFunctionCrystalInfo call PROCEDURE_111028_0004\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_SLONG, &tmTriggerFunctionCrystal.FunctionID, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_SLONG, &tmTriggerFunctionCrystal.CrystalGroupID, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_SLONG, &tmTriggerFunctionCrystal.PeriodTime, 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_UTINYINT, &tmTriggerFunctionCrystal.RandomSequence, 0, &arrCB[3]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerFunctionCrystal != NULL)
			{
				ptmTriggerFunctionCrystal->push_back(tmTriggerFunctionCrystal);
			}

			util::zero(&tmTriggerFunctionCrystal, sizeof(tmTriggerFunctionCrystal));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerCrystalGroup(CODBCStatement *i_pODBCStmt, vectorTriggerCrystalGroup *i_pTriggerCrystalGroup, HWND hWnd)
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerCrystalGroup(CODBCStatement* i_pODBCStmt, vectorTriggerCrystalGroup* i_pTriggerCrystalGroup, HWND hWnd)
{
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0008, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerCrystalGroup->size() - 1));

	//Insert
	int nObjects = 0;
	STRIGGER_CRYSTAL_GROUP* pInfo;
	vectorTriggerCrystalGroup::iterator itr = i_pTriggerCrystalGroup->begin();
	SQLRETURN RET;

	for (; itr != i_pTriggerCrystalGroup->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->CrystalGroupID, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->DestroyGroupID, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->EventID, 0, &arrCB[2]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0009);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TriggerCrystalGroup CrystalGroupID(%d) DestroyGroupID(%d) EventID(%d)!!",
			        pInfo->CrystalGroupID, pInfo->DestroyGroupID, pInfo->EventID);
			MessageBox(NULL, err, "ti_TriggerCrystalGroup", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerCrystalGroupInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerCrystalGroup, BOOL ToolFlag /*= TRUE*/)
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerCrystalGroupInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerCrystalGroup, BOOL ToolFlag /*= TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_CRYSTAL_GROUP tmTriggerCrystalGroup;
	util::zero(&tmTriggerCrystalGroup, sizeof(tmTriggerCrystalGroup));

	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerCrystalGroup* ptmTriggerCrystalGroup = (vectorTriggerCrystalGroup *)(i_pTriggerCrystalGroup);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0007))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadTriggerCrystalGroupInfo call PROCEDURE_111028_0007\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmTriggerCrystalGroup.CrystalGroupID, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmTriggerCrystalGroup.DestroyGroupID, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmTriggerCrystalGroup.EventID, 0, &arrCB[2]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerCrystalGroup != NULL)
			{
				ptmTriggerCrystalGroup->push_back(tmTriggerCrystalGroup);
			}

			util::zero(&tmTriggerCrystalGroup, sizeof(tmTriggerCrystalGroup));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerCrystalDestroyGroup(CODBCStatement *i_pODBCStmt, vectorTriggerCrystalDestroyGroup *i_pTriggerCrystalDestroyGroup, HWND hWnd)
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerCrystalDestroyGroup(CODBCStatement* i_pODBCStmt, vectorTriggerCrystalDestroyGroup* i_pTriggerCrystalDestroyGroup, HWND hWnd)
{
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0011, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerCrystalDestroyGroup->size() - 1));

	//Insert
	int nObjects = 0;
	STRIGGER_CRYSTAL_DESTROY_GROUP* pInfo;
	vectorTriggerCrystalDestroyGroup::iterator itr = i_pTriggerCrystalDestroyGroup->begin();
	SQLRETURN RET;

	for (; itr != i_pTriggerCrystalDestroyGroup->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->DestroyGroupID, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pInfo->SequenceNumber, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->TargetMonster, 0, &arrCB[2]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0012);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TriggerCrystalDestroyGroup DestroyGroupID(%d) SequenceNumber(%d) TargetMonster(%d)!!",
			        pInfo->DestroyGroupID, pInfo->SequenceNumber, pInfo->TargetMonster);
			MessageBox(NULL, err, "ti_TriggerCrystalDestroyGroup", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerCrystalDestroyGroupInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerCrystalDestroyGroup, BOOL ToolFlag /*= TRUE*/)
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerCrystalDestroyGroupInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerCrystalDestroyGroup, BOOL ToolFlag /*= TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_CRYSTAL_DESTROY_GROUP tmTriggerCrystalDestroyGroup;
	util::zero(&tmTriggerCrystalDestroyGroup, sizeof(tmTriggerCrystalDestroyGroup));

	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerCrystalDestroyGroup* ptmTriggerCrystalDestroyGroup = (vectorTriggerCrystalDestroyGroup *)(i_pTriggerCrystalDestroyGroup);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_111028_0010))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadTriggerCrystalDestroyGroupInfo call PROCEDURE_111028_0010\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_SLONG, &tmTriggerCrystalDestroyGroup.DestroyGroupID, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_USHORT, &tmTriggerCrystalDestroyGroup.SequenceNumber, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_SLONG, &tmTriggerCrystalDestroyGroup.TargetMonster, 0, &arrCB[2]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerCrystalDestroyGroup != NULL)
			{
				ptmTriggerCrystalDestroyGroup->push_back(tmTriggerCrystalDestroyGroup);
			}

			util::zero(&tmTriggerCrystalDestroyGroup, sizeof(tmTriggerCrystalDestroyGroup));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerFunctionNGCInflWar(CODBCStatement *i_pODBCStmt, vectorTriggerFunctionNGCInflWar *i_pTriggerFunctionNGCInflWar, HWND hWnd)
/// \brief		// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
/// \author		hskim
/// \date		2011-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerFunctionNGCInflWar(CODBCStatement* i_pODBCStmt, vectorTriggerFunctionNGCInflWar* i_pTriggerFunctionNGCInflWar, HWND hWnd)
{
	SQLINTEGER arrCB[8];
	fill_n(arrCB, 8, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111107_0002, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerFunctionNGCInflWar->size() - 1));

	//Insert
	int nObjects = 0;
	STRIGGER_FUNCTION_NGC_INFLWAR* pInfo;
	vectorTriggerFunctionNGCInflWar::iterator itr = i_pTriggerFunctionNGCInflWar->begin();
	SQLRETURN RET;

	int a = i_pTriggerFunctionNGCInflWar->size();

	for (; itr != i_pTriggerFunctionNGCInflWar->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->FunctionID, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->NGCInflWarGroupID, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->PeriodTime, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->BossMonster, 0, &arrCB[3]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->WinInfluenceWP, 0, &arrCB[4]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->LossInfluenceWP, 0, &arrCB[5]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->FirstDamageGuildWP, 0, &arrCB[6]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->FixedTimeType, 0, &arrCB[7]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111107_0003);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TriggerFunctionNGCInflWar FunctionID(%d) NGCInflWarGroupID(%d) PeriodTime(%d) BossMonster(%d) WinInfluenceWP(%d) LossInfluenceWP(%d) FirstDamageGuildWP(%d) FixedTimeType(%d)!!",
			        pInfo->FunctionID, pInfo->NGCInflWarGroupID, pInfo->PeriodTime, pInfo->BossMonster, pInfo->WinInfluenceWP, pInfo->LossInfluenceWP, pInfo->FirstDamageGuildWP, pInfo->FixedTimeType);
			MessageBox(NULL, err, "ti_TriggerFunctionNGCInflWar", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerFunctionNGCInflWarInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerFunctionNGCInflWar, BOOL ToolFlag /*= TRUE*/)
/// \brief		// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
/// \author		hskim
/// \date		2011-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerFunctionNGCInflWarInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerFunctionNGCInflWar, BOOL ToolFlag /*= TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_FUNCTION_NGC_INFLWAR tmTriggerFunctionNGCInflWar;
	util::zero(&tmTriggerFunctionNGCInflWar, sizeof(tmTriggerFunctionNGCInflWar));

	SQLINTEGER arrCB[8];
	fill_n(arrCB, 8, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerFunctionNGCInflWar* ptmTriggerFunctionNGCInflWar = (vectorTriggerFunctionNGCInflWar *)(i_pTriggerFunctionNGCInflWar);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_111107_0001))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadTriggerFunctionNGCInflWarInfo call PROCEDURE_111107_0001\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_SLONG, &tmTriggerFunctionNGCInflWar.FunctionID, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_SLONG, &tmTriggerFunctionNGCInflWar.NGCInflWarGroupID, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_SLONG, &tmTriggerFunctionNGCInflWar.PeriodTime, 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_SLONG, &tmTriggerFunctionNGCInflWar.BossMonster, 0, &arrCB[3]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_SLONG, &tmTriggerFunctionNGCInflWar.WinInfluenceWP, 0, &arrCB[4]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_SLONG, &tmTriggerFunctionNGCInflWar.LossInfluenceWP, 0, &arrCB[5]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_SLONG, &tmTriggerFunctionNGCInflWar.FirstDamageGuildWP, 0, &arrCB[6]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_SLONG, &tmTriggerFunctionNGCInflWar.FixedTimeType, 0, &arrCB[7]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerFunctionNGCInflWar != NULL)
			{
				// 2013-07-08 by jhseol, 트리거 시스템 확장 - 선행조건 사용 여부
#ifdef S_TRIGGER_SYSTEM_EX // - 선행조건 사용 여부

				if (	tmTriggerFunctionNGCInflWar.FunctionID == STRIGEER_NGCINFLWAR_WW2_EVENT_TRIGGER_FUNID_BCU
					||	tmTriggerFunctionNGCInflWar.FunctionID == STRIGEER_NGCINFLWAR_WW2_EVENT_TRIGGER_FUNID_ANI )
				{
					tmTriggerFunctionNGCInflWar.DestroyCrystalcount = 2;
				}
#endif
				// end 2013-07-08 by jhseol, 트리거 시스템 확장 - 선행조건 사용 여부
				ptmTriggerFunctionNGCInflWar->push_back(tmTriggerFunctionNGCInflWar);
			}

			util::zero(&tmTriggerFunctionNGCInflWar, sizeof(tmTriggerFunctionNGCInflWar));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerNGCInflWarMonsterGroup(CODBCStatement *i_pODBCStmt, vectorTriggerNGCInflWarMonsterGroup *i_pTriggerNGCInflWarMonsterGroup, HWND hWnd)
/// \brief		// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
/// \author		hskim
/// \date		2011-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerNGCInflWarMonsterGroup(CODBCStatement* i_pODBCStmt, vectorTriggerNGCInflWarMonsterGroup* i_pTriggerNGCInflWarMonsterGroup, HWND hWnd)
{
	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111107_0005, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerNGCInflWarMonsterGroup->size() - 1));

	//Insert
	int nObjects = 0;
	STRIGGER_NGC_INFLWAR_MONSTER_GROUP* pInfo;
	vectorTriggerNGCInflWarMonsterGroup::iterator itr = i_pTriggerNGCInflWarMonsterGroup->begin();
	SQLRETURN RET;

	int a = i_pTriggerNGCInflWarMonsterGroup->size();

	for (; itr != i_pTriggerNGCInflWarMonsterGroup->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->NGCInflWarGroupID, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->Monster, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->SummonCount, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->SummonRandomPos, 0, &arrCB[3]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_111107_0006);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TriggerNGCInflWarMonsterGroup NGCInflWarGroupID(%d) BossMonster(%d) SummonCount(%d) SummonRandomPos(%d)!!",
			        pInfo->NGCInflWarGroupID, pInfo->Monster, pInfo->SummonCount, pInfo->SummonRandomPos);
			MessageBox(NULL, err, "ti_TriggerNGCInflWarMonsterGroup", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadSystemEventInfo(CODBCStatement *i_pODBCStmt , ATUM_DATE_TIME *i_pEventDateTime, BOOL ToolFlag)
/// \brief		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
/// \author		hskim
/// \date		2011-12-21
/// \warning	
///
/// \param	
/// \return		
/////////////////////////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadSystemEventInfo(CODBCStatement* i_pODBCStmt, ATUM_DATE_TIME* i_pEventDateTime, BOOL ToolFlag)
{
	int rtn = 0;
	RETCODE ret;

	eSYSTEM_EVENT ParamID = SYSTEM_EVENT_OPENING_MOVIE; // 0 <= 동영상 기준 시간

	SQLINTEGER arrCB = SQL_NTS;

	SQL_TIMESTAMP_STRUCT Datetime;
	util::zero(&Datetime, sizeof(SQL_TIMESTAMP_STRUCT));

	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &ParamID, 0, NULL);

	if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_111221_0001))
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadSystemEventInfo call PROCEDURE_111221_0001 \r\n");
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_TIMESTAMP, &Datetime, 0, &arrCB);

	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		*i_pEventDateTime = Datetime;

		rtn++;
	}

	i_pODBCStmt->FreeStatement();

	return rtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerNGCInflWarMonsterGroupInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerNGCInflWarMonsterGroup, BOOL ToolFlag /*= TRUE*/)
/// \brief		// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
/// \author		hskim
/// \date		2011-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerNGCInflWarMonsterGroupInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerNGCInflWarMonsterGroup, BOOL ToolFlag /*= TRUE*/)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_NGC_INFLWAR_MONSTER_GROUP tmTriggerNGCInflWarMonsterGroup;
	util::zero(&tmTriggerNGCInflWarMonsterGroup, sizeof(tmTriggerNGCInflWarMonsterGroup));

	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerNGCInflWarMonsterGroup* ptmTriggerNGCInflWarMonsterGroup = (vectorTriggerNGCInflWarMonsterGroup *)(i_pTriggerNGCInflWarMonsterGroup);

		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_111107_0004))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumFieldDBManager::DBLoadTriggerNGCInflWarMonsterGroupInfo call ExecuteQuery(PROCEDURE_111107_0007\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_SLONG, &tmTriggerNGCInflWarMonsterGroup.NGCInflWarGroupID, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_SLONG, &tmTriggerNGCInflWarMonsterGroup.Monster, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_SLONG, &tmTriggerNGCInflWarMonsterGroup.SummonCount, 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_SLONG, &tmTriggerNGCInflWarMonsterGroup.SummonRandomPos, 0, &arrCB[3]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerNGCInflWarMonsterGroup != NULL)
			{
				ptmTriggerNGCInflWarMonsterGroup->push_back(tmTriggerNGCInflWarMonsterGroup);
			}

			util::zero(&tmTriggerNGCInflWarMonsterGroup, sizeof(tmTriggerNGCInflWarMonsterGroup));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템

// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerFunctionNGCOutPost(CODBCStatement *i_pODBCStmt, vectorTriggerFunctionNGCOutPost *i_pTriggerFunctionNGCOutPost, HWND hWnd)
/// \brief		전진기지 트리거 정보 DB Insert 함수
/// \author		jhseol
/// \date		2013-01-21
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerFunctionNGCOutPost(CODBCStatement* i_pODBCStmt, vectorTriggerFunctionNGCOutPost* i_pTriggerFunctionNGCOutPost, HWND hWnd)
{
	//Delete 
	/*******************************************************************************************
	-------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteTriggerFunctionNGCOutPost
	-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteTriggerFunctionNGCOutPost
	--WITH EXECUTE AS 'proexe'
	AS
	BEGIN
	DELETE FROM atum2_db_account.dbo.ti_TriggerFunctionNGCOutPost
	END
	GO
	*******************************************************************************************/
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0005, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	//Insert
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerFunctionNGCOutPost->size() - 1));
	SQLINTEGER arrCB[8];
	fill_n(arrCB, 8, SQL_NTS);

	int nObjects = 0;
	STRIGGER_FUNCTION_NGC_OUTPOST* pInfo;
	vectorTriggerFunctionNGCOutPost::iterator itr = i_pTriggerFunctionNGCOutPost->begin();
	SQLRETURN RET;

	int a = i_pTriggerFunctionNGCOutPost->size();

	for (; itr != i_pTriggerFunctionNGCOutPost->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->FunctionID, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->SummonMonsterGroupID, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &pInfo->StandardOutPostMap, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->BossMonster, 0, &arrCB[3]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->WinInfluenceWP, 0, &arrCB[4]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->LossInfluenceWP, 0, &arrCB[5]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &pInfo->NextOutPostMap, 0, &arrCB[6]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &pInfo->DestroyCrystalcount, 0, &arrCB[7]);


		/*******************************************************************************************
		-------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_InsertTriggerFunctionNGCOutPost
		-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_InsertTriggerFunctionNGCOutPost
		@i_FunctionID			INT,
		@i_NGCInflWarGroupID	INT,
		@i_StandardOutPostMap	INT,
		@i_BossMonster			INT,
		@i_WinInfluenceWP		INT,
		@i_LossInfluenceWP		INT,
		@i_NextOutPostMap		INT,
		@i_DestroyCrystalcount	INT
		--WITH EXECUTE AS 'proexe'
		AS
		BEGIN
		INSERT INTO atum2_db_account.dbo.ti_TriggerFunctionNGCOutPost
		(FunctionID, NGCInflWarGroupID, StandardOutPostMap, BossMonster, WinInfluenceWP, LossInfluenceWP, NextOutPostMap, DestroyCrystalcount)
		VALUES(@i_FunctionID, @i_NGCInflWarGroupID, @i_StandardOutPostMap, @i_BossMonster, @i_WinInfluenceWP, @i_LossInfluenceWP, @i_NextOutPostMap, @i_DestroyCrystalcount)
		END
		GO
		*******************************************************************************************/
		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0001);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TriggerFunctionNGCOutPost FunctionID(%d) SummonMonsterGroupID(%d) StandardOutPostMap(%d) BossMonster(%d) WinInfluenceWP(%d) LossInfluenceWP(%d) NextOutPostMap(%d) DestroyCrystalcount(%d)!!",
			        pInfo->FunctionID, pInfo->SummonMonsterGroupID, pInfo->StandardOutPostMap, pInfo->BossMonster, pInfo->WinInfluenceWP, pInfo->LossInfluenceWP, pInfo->NextOutPostMap, pInfo->DestroyCrystalcount);
			MessageBox(NULL, err, "ti_TriggerFunctionNGCOutPost", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerOutPostBossKill(CODBCStatement *i_pODBCStmt, vectorTriggerOutPostBossKillInfo *i_pTriggerOutPostBossKillInfo, HWND hWnd)
/// \brief		전진기지 보스몬스터 공략 정보 DB Insert 함수
/// \author		jhseol
/// \date		2013-01-21
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerOutPostBossKill(CODBCStatement* i_pODBCStmt, vectorTriggerOutPostBossKillInfo* i_pTriggerOutPostBossKillInfo, HWND hWnd)
{
	//Delete 
	/*******************************************************************************************
	-------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteTriggerOutPosKill
	-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteTriggerOutPosKill
	--WITH EXECUTE AS 'proexe'
	AS
	BEGIN
	DELETE FROM atum2_db_account.dbo.ti_TriggerOutPostBossKill
	END
	GO
	*******************************************************************************************/
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0006, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}

	//Insert
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerOutPostBossKillInfo->size() - 1));
	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);

	int nObjects = 0;
	STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO* pInfo;
	vectorTriggerOutPostBossKillInfo::iterator itr = i_pTriggerOutPostBossKillInfo->begin();
	SQLRETURN RET;

	int a = i_pTriggerOutPostBossKillInfo->size();

	for (; itr != i_pTriggerOutPostBossKillInfo->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->BossMonster, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->CrystalNum, 0, &arrCB[1]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->RegenMinTimeMinutes, 0, &arrCB[2]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->RegenMaxTimeMinutes, 0, &arrCB[3]);

		/*******************************************************************************************
		-------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_InsertTriggerOutPosKill
		-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_InsertTriggerOutPosKill
		@i_BossMonster			INT,
		@i_CrystalNum			INT,
		@i_RegenMinTimeMinutes	INT,
		@i_RegenMaxTimeMinutes	INT
		--WITH EXECUTE AS 'proexe'
		AS
		BEGIN
		INSERT INTO atum2_db_account.dbo.ti_TriggerOutPostBossKill
		(BossMonster, CrystalNum, RegenMinTimeMinutes, RegenMaxTimeMinutes)
		VALUES(@i_BossMonster, @i_CrystalNum, @i_RegenMinTimeMinutes, @i_RegenMaxTimeMinutes)
		END
		GO
		*******************************************************************************************/
		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0002);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TriggerOutPosKill BossMonster(%d) CrystalNum(%d) RegenMinTimeMinutes(%d) RegenMaxTimeMinutes(%d)!!",
			        pInfo->BossMonster, pInfo->CrystalNum, pInfo->RegenMinTimeMinutes, pInfo->RegenMaxTimeMinutes);
			MessageBox(NULL, err, "ti_TriggerOutPostBossKill", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerFunctionNGCOutPostInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerFunctionNGCOutPost, BOOL ToolFlag = TRUE)
/// \brief		전진기지 트리거 정보 가져오는 함수
/// \author		jhseol
/// \date		2013-01-21
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerFunctionNGCOutPostInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerFunctionNGCOutPost, BOOL ToolFlag)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_FUNCTION_NGC_OUTPOST tmTriggerFunctionNGCOutPost;
	util::zero(&tmTriggerFunctionNGCOutPost, sizeof(tmTriggerFunctionNGCOutPost));

	SQLINTEGER arrCB[8];
	fill_n(arrCB, 8, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerFunctionNGCOutPost* ptmTriggerFunctionNGCOutPost = (vectorTriggerFunctionNGCOutPost *)(i_pTriggerFunctionNGCOutPost);

		/*******************************************************************************************
		-------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_LoadTriggerFunctionNGCOutPost
		-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_LoadTriggerFunctionNGCOutPost
		--WITH EXECUTE AS 'proexe'
		AS
		BEGIN
		SELECT FunctionID, NGCInflWarGroupID, StandardOutPostMap, BossMonster, WinInfluenceWP, LossInfluenceWP, NextOutPostMap, DestroyCrystalcount
		FROM atum2_db_account.dbo.ti_TriggerFunctionNGCOutPost
		END
		GO
		*******************************************************************************************/
		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0003))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumDBHelper::DBLoadTriggerFunctionNGCOutPostInfo call PROCEDURE_130121_0003\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_SLONG, &tmTriggerFunctionNGCOutPost.FunctionID, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_SLONG, &tmTriggerFunctionNGCOutPost.SummonMonsterGroupID, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_USHORT, &tmTriggerFunctionNGCOutPost.StandardOutPostMap, 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_SLONG, &tmTriggerFunctionNGCOutPost.BossMonster, 0, &arrCB[3]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_SLONG, &tmTriggerFunctionNGCOutPost.WinInfluenceWP, 0, &arrCB[4]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_SLONG, &tmTriggerFunctionNGCOutPost.LossInfluenceWP, 0, &arrCB[5]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_USHORT, &tmTriggerFunctionNGCOutPost.NextOutPostMap, 0, &arrCB[6]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_USHORT, &tmTriggerFunctionNGCOutPost.DestroyCrystalcount, 0, &arrCB[7]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerFunctionNGCOutPost != NULL)
			{
				ptmTriggerFunctionNGCOutPost->push_back(tmTriggerFunctionNGCOutPost);
			}

			util::zero(&tmTriggerFunctionNGCOutPost, sizeof(tmTriggerFunctionNGCOutPost));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}
	return rtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerNGCPostBossKillInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerFunctionOutPostBossKillInfo, BOOL ToolFlag = TRUE)
/// \brief		전진기지 보스몬스터 공략 정보 가져오는 함수
/// \author		jhseol
/// \date		2013-01-21
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerOutPostBossKillInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerOutPostBossKillInfo, BOOL ToolFlag)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO tmTriggerOutPostBossKill;
	util::zero(&tmTriggerOutPostBossKill, sizeof(tmTriggerOutPostBossKill));

	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerOutPostBossKillInfo* ptmTriggerOutPostKill = (vectorTriggerOutPostBossKillInfo *)(i_pTriggerOutPostBossKillInfo);

		/*******************************************************************************************
		-------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_LoadTriggerOutPosKill
		-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_LoadTriggerOutPosKill
		--WITH EXECUTE AS 'proexe'
		AS
		BEGIN
		SELECT BossMonster, CrystalNum, RegenMinTimeMinutes, RegenMaxTimeMinutes
		FROM atum2_db_account.dbo.ti_TriggerOutPostBossKill
		END
		GO
		*******************************************************************************************/
		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0004))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumDBHelper::DBLoadTriggerOutPostBossKillInfo call PROCEDURE_130121_0004\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_SLONG, &tmTriggerOutPostBossKill.BossMonster, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_SLONG, &tmTriggerOutPostBossKill.CrystalNum, 0, &arrCB[1]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_SLONG, &tmTriggerOutPostBossKill.RegenMinTimeMinutes, 0, &arrCB[2]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_SLONG, &tmTriggerOutPostBossKill.RegenMaxTimeMinutes, 0, &arrCB[3]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerOutPostKill != NULL)
			{
				ptmTriggerOutPostKill->push_back(tmTriggerOutPostBossKill);
			}

			util::zero(&tmTriggerOutPostBossKill, sizeof(tmTriggerOutPostBossKill));

			rtn++;
		}

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTriggerCrystalBuff(CODBCStatement *i_pODBCStmt, vectorTriggerFunctionNGCOutPost *i_pTriggerFunctionNGCOutPost, HWND hWnd)
/// \brief		크리스탈 버프 DB Insert 함수
/// \author		jhseol
/// \date		2013-01-21
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveTriggerCrystalBuff(CODBCStatement* i_pODBCStmt, vectorTriggerCrystalBuff* i_pTriggerCrystalBuff, HWND hWnd)
{
	//Delete 
	/*******************************************************************************************
	-------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteTriggerCrystalBuff
	-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteTriggerCrystalBuff
	--WITH EXECUTE AS 'proexe'
	AS
	BEGIN
	DELETE FROM atum2_db_account.dbo.ti_TriggerCrystalBuff
	END
	GO
	*******************************************************************************************/
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0009, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	//Insert
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, ((LPARAM)i_pTriggerCrystalBuff->size() - 1));
	SQLINTEGER arrCB[7];
	fill_n(arrCB, 7, SQL_NTS);
	int nObjects = 0;
	STRIGGER_CRYSTAL_BUFF* pInfo;
	vectorTriggerCrystalBuff::iterator itr = i_pTriggerCrystalBuff->begin();
	SQLRETURN RET;

	int a = i_pTriggerCrystalBuff->size();

	for (; itr != i_pTriggerCrystalBuff->end(); itr++)
	{
		pInfo = &*itr;

		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->FunctionID, 0, &arrCB[0]);
		RET = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pInfo->SkillItemNum, 0, &arrCB[1]);

		/*******************************************************************************************
		-------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_InsertTriggerCrystalBuff
		-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_InsertTriggerCrystalBuff
		@i_FunctionID			INT,
		@i_SkillItemNum			INT
		--WITH EXECUTE AS 'proexe'
		AS
		BEGIN
		INSERT INTO atum2_db_account.dbo.ti_TriggerCrystalBuff
		(FunctionID, SkillItemNum)
		VALUES(@i_FunctionID, @i_SkillItemNum)
		END
		GO
		*******************************************************************************************/
		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0007);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TriggerFunctionNGCOutPost FunctionID(%d) SkillItemNum(%d)!!", pInfo->FunctionID, pInfo->SkillItemNum);
			MessageBox(NULL, err, "ti_TriggerFunctionNGCOutPost", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}

	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::DBLoadTriggerCrystalBuffInfo(CODBCStatement *i_pODBCStmt, void *i_pTriggerCrystalBuffInfo, BOOL ToolFlag)
/// \brief		트리거 맵의 크리스탈 버프정보 가져오는 함수
/// \author		jhseol
/// \date		2013-01-21
/// \warning	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::DBLoadTriggerCrystalBuffInfo(CODBCStatement* i_pODBCStmt, void* i_pTriggerCrystalBuffInfo, BOOL ToolFlag)
{
	int rtn = 0;
	RETCODE ret;
	STRIGGER_CRYSTAL_BUFF tmTriggerCrystalBuff;
	util::zero(&tmTriggerCrystalBuff, sizeof(tmTriggerCrystalBuff));

	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);

	if (ToolFlag == TRUE) // 툴에서 사용할 경우
	{
		// omi.tex 에 데이터 안 넣음

		i_pODBCStmt->FreeStatement();
	}
	else if (ToolFlag == FALSE)
	{
		vectorTriggerCrystalBuff* ptmTriggerCrystalBuff = (vectorTriggerCrystalBuff *)(i_pTriggerCrystalBuffInfo);

		/*******************************************************************************************
		-------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_LoadTriggerCrystalBuff
		-- DESC				: // 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_LoadTriggerCrystalBuff
		--WITH EXECUTE AS 'proexe'
		AS
		BEGIN
		SELECT FunctionID, SkillItemNum
		FROM atum2_db_account.dbo.ti_TriggerCrystalBuff
		END
		GO
		*******************************************************************************************/
		if (FALSE == i_pODBCStmt->ExecuteQuery(PROCEDURE_130121_0008))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB ERROR] CAtumDBHelper::DBLoadTriggerCrystalBuffInfo call PROCEDURE_130121_0007\r\n");

			i_pODBCStmt->FreeStatement();

			return 0;
		}

		SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_SLONG, &tmTriggerCrystalBuff.FunctionID, 0, &arrCB[0]);
		SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_SLONG, &tmTriggerCrystalBuff.SkillItemNum, 0, &arrCB[1]);

		while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
		{
			if (ptmTriggerCrystalBuff != NULL)
			{
				ptmTriggerCrystalBuff->push_back(tmTriggerCrystalBuff);
			}

			util::zero(&tmTriggerCrystalBuff, sizeof(ptmTriggerCrystalBuff));

			rtn++;
		}


		/*
				//int asdsad[10] = {7840030,7840040,7840030,7840040,7840030,7840040,7840030,7840040,7840030,7840040};
				//int asdsad[10] = {7840010,7840020,7840030,7840040,7840050,7840060,7840070,7840080,7840090,7840100};
				for ( int i = 0 ; i < 10 ; i++ )
				{
					tmTriggerCrystalBuff.FunctionID = 5000;
					tmTriggerCrystalBuff.nSkillItemNum = asdsad[i];
					
					if( ptmTriggerCrystalBuff != NULL )
					{
						ptmTriggerCrystalBuff->push_back(tmTriggerCrystalBuff);
					}
					
					util::zero(&tmTriggerCrystalBuff, sizeof(tmTriggerCrystalBuff));
					
					rtn++;
				}
		*/

		i_pODBCStmt->FreeStatement();
	}

	return rtn;
}

// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadPCBangInfo(CODBCStatement *i_pODBCStmt, PCBANG_INFO *o_pPCbangInfo, char o_ErrorString)
/// \brief		
/// \author		dhjin
/// \date		2007-01-22 ~ 2007-01-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//int CAtumDBHelper::LoadPCBangInfo(CODBCStatement* i_pODBCStmt, vectorPCBANG_INFO* o_pPCbangInfo, char* o_ErrorString)
//{
//	o_pPCbangInfo->clear();
//	PCBANG_INFO tempPCBangInfo;
//	util::zero(&tempPCBangInfo,sizeof(PCBANG_INFO));
//
//	RETCODE ret;
//	SQLINTEGER arrCB[14]; // 2007-06-25 by dhjin, PC방 등급 관련 추가
//	fill_n(arrCB, 14, SQL_NTS);
//
//	// Bind Columns
//	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempPCBangInfo.PCBangUID, 0, &arrCB[1]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, tempPCBangInfo.ST_IP, SIZE_MAX_IPADDRESS, &arrCB[2]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, tempPCBangInfo.ED_IP, 4, &arrCB[3]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, tempPCBangInfo.User_Id, SIZE_MAX_ACCOUNT_NAME, &arrCB[4]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_CHAR, tempPCBangInfo.Branch_Name, 50, &arrCB[5]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, tempPCBangInfo.Branch_RegNO, 12, &arrCB[6]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_CHAR, tempPCBangInfo.Branch_Tel, 14, &arrCB[7]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, tempPCBangInfo.ZipCode, 15, &arrCB[8]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_CHAR, tempPCBangInfo.Addr_Sido, 10, &arrCB[9]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_CHAR, tempPCBangInfo.Addr_SiGuGun, 20, &arrCB[10]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_CHAR, tempPCBangInfo.Addr_Dong, 100, &arrCB[11]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_CHAR, tempPCBangInfo.Addr_Detail, 50, &arrCB[12]);
//	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_UTINYINT, &tempPCBangInfo.User_Level, 0, &arrCB[13]);
//
//	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("Select TempIp.UID, TempIp.ST_IP, TempIp.ED_IP, TempBr.User_Id,	\
//	// 												TempBr.Branch_Name, TempBr.Branch_RegNO, TempBr.Branch_Tel, \
//	// 												TempBr.ZipCode, TempBr.Addr_Sido, TempBr.Addr_SiGugun, TempBr.Addr_Dong, \
//	// 												TempBr.Addr_Detail, TempBr.User_Level \
//	// 											From 	(Select MAX(BSeq) As UID, ST_IP, MAX(ED_IP)As ED_IP \
//	// 												From pc_mem_branch_ip \
//	// 												where ST_IP <> ' ' AND ST_IP NOT LIKE '%..%' \
//	// 												Group by ST_IP \
//	// 												) As TempIp  \
//	// 												Inner Join \
//	// 												(select TempBranch.BSeq, TempBranch.User_Id,	\
//	// 													TempBranch.Branch_Name, 	TempBranch.Branch_RegNO,	 TempBranch.Branch_Tel, \
//	// 													TempBranch.ZipCode,		 TempBranch.Addr_Sido, 		TempBranch.Addr_SiGugun,	 TempBranch.Addr_Dong, \
//	// 													TempBranch.Addr_Detail, 		TempMember.User_Level  \
//	// 												from \
//	// 													(select * from pc_mem_branch) As TempBranch Inner join	\
//	// 													(select User_Id, User_Level  from pc_member Where  pc_member.User_Level >= 3) As TempMember	\
//	// 												On TempBranch.User_Id = TempMember.User_Id) As TempBr \
//	// 											On TempIp.UID = TempBr.BSeq");
//
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(QUERY_080702_0065);
//	// 2008-09-12 by cmkwon, 한국 Yedang_Kor PCBang 리로드 버그 수정 - 쿼리로 해야 함.
//	//	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0065));
//	if (!bRet)
//	{
//		// error
//		i_pODBCStmt->FreeStatement();
//		return 0;
//	}
//
//	int nObjects = 0;
//
//	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
//	{
//		char* pIPStart0 = tempPCBangInfo.ST_IP;
//		char* pIPEndD = tempPCBangInfo.ED_IP;
//
//		char* pIPClassA = strtok(pIPStart0, ".");
//		if (pIPClassA == NULL)
//		{
//			sprintf(o_ErrorString, "[Error] UID = %d IPClassA RANGE error \r\n", tempPCBangInfo.PCBangUID);
//			i_pODBCStmt->FreeStatement();
//			return -1;
//		}
//		char* pIPClassB = strtok(NULL, ".");
//		if (pIPClassB == NULL)
//		{
//			sprintf(o_ErrorString, "[Error] UID = %d IPClassB RANGE error \r\n", tempPCBangInfo.PCBangUID);
//			i_pODBCStmt->FreeStatement();
//			return -1;
//		}
//		char* pIPClassC = strtok(NULL, ".");
//		if (pIPClassC == NULL)
//		{
//			sprintf(o_ErrorString, "[Error] UID = %d IPClassC RANGE error \r\n", tempPCBangInfo.PCBangUID);
//			i_pODBCStmt->FreeStatement();
//			return -1;
//		}
//		char* pIPClassD = strtok(NULL, "\0");
//		if (pIPClassD == NULL)
//		{
//			sprintf(o_ErrorString, "[Error] UID = %d IPClassD RANGE error \r\n", tempPCBangInfo.PCBangUID);
//			i_pODBCStmt->FreeStatement();
//			return -1;
//		}
//
//		int iIPStartA = atoi(pIPClassA);
//		int iIPStartB = atoi(pIPClassB);
//		int iIPStartC = atoi(pIPClassC);
//		int iIPStartD = atoi(pIPClassD);
//		int iIPEndD = atoi(pIPEndD);
//
//		if (FALSE == util::in_range(1, iIPStartA, 255) ||
//			FALSE == util::in_range(0, iIPStartB, 255) ||
//			FALSE == util::in_range(0, iIPStartC, 255) ||
//			FALSE == util::in_range(1, iIPStartD, 255) ||
//			FALSE == util::in_range(1, iIPEndD, 255))
//		{// 2007-01-22 by dhjin, IP 범위 체크
//			sprintf(o_ErrorString, "[Error] UID = %d UpdatePCBangIPList RANGE error \r\n", tempPCBangInfo.PCBangUID);
//			i_pODBCStmt->FreeStatement();
//			return -1;
//		}
//
//		int IPClassDGap = (iIPEndD - iIPStartD);
//
//		if (0 > IPClassDGap)
//		{// 2007-01-22 by dhjin, 시작IP가 끝IP보다 크면 오류
//			sprintf(o_ErrorString, "[Error] UID = %d UpdatePCBangIPList (StartIP - EndIp) error \r\n", tempPCBangInfo.PCBangUID);
//			i_pODBCStmt->FreeStatement();
//			return -1;
//		}
//
//		sprintf(tempPCBangInfo.ST_IP, "%d.%d.%d.%d", iIPStartA, iIPStartB, iIPStartC, iIPStartD);
//
//		o_pPCbangInfo->push_back(tempPCBangInfo);
//
//		nObjects++;
//		util::zero(&tempPCBangInfo,sizeof(PCBANG_INFO));
//	}
//	i_pODBCStmt->FreeStatement();
//
//	// 2008-12-05 by cmkwon, PC방 리스트 강제 넣는 부분에 한글을 영문으로 수정하기 - 아래와 같이 수정 함.
//	// 	// 2007-01-24 by dhjin, 마상, 예당 PC방 강제 넣기
//	// 	tempPCBangInfo.SetPCBANG_INFO(100000000, "121.134.114.1", "255", "admin", "마상&예당PC방", "0-0-0", "000-0000-0000"
//	// 		, "000-000", "서울", "서초구", "서초동", "남승빌딩", 10);
//	// 	o_pPCbangInfo->push_back(tempPCBangInfo);
//	// 	tempPCBangInfo.SetPCBANG_INFO(100000001, "211.44.231.120", "125", "admin", "영업팀", "0-0-0", "000-0000-0000"
//	// 		, "000-000", "서울", "서초구", "서초동", "제우빌딩", 10);
//	// 	o_pPCbangInfo->push_back(tempPCBangInfo);
//	// 
//	// 	// 2007-05-22 by dhjin, 예당 안창석실장님 추가
//	// 	tempPCBangInfo.SetPCBANG_INFO(100000002, "218.55.117.215", "255", "admin", "예당콜센터&운영팀", "0-0-0", "000-0000-0000"
//	// 		, "000-000", "서울", "서초구", "서초동", "제우빌딩", 10);
//	// 	o_pPCbangInfo->push_back(tempPCBangInfo);
//	// 
//	// 	tempPCBangInfo.SetPCBANG_INFO(100000003, "10.10.90.1", "255", "admin", "마상&예당PC방", "0-0-0", "000-0000-0000"
//	// 		, "000-000", "서울", "서초구", "서초동", "제우빌딩", 10);
//	// 	o_pPCbangInfo->push_back(tempPCBangInfo);
//
//	// 2008-12-05 by cmkwon, PC방 리스트 강제 넣는 부분에 한글을 영문으로 수정하기 - 아래와 같이 수정 함.
//	// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(121.134.114.)
//	tempPCBangInfo.SetPCBANG_INFO(100000000, "115.144.35.", "255", "admin", "Masang&Yedang", "0-0-0", "000-0000-0000", "000-000", "Seoul", "Seocho-gu", "Seocho-dong", "Namseung", 10);
//	o_pPCbangInfo->push_back(tempPCBangInfo);
//	tempPCBangInfo.SetPCBANG_INFO(100000001, "211.44.231.120", "125", "admin", "Yedang", "0-0-0", "000-0000-0000", "000-000", "Seoul", "Seocho-gu", "Seocho-dong", "JaeWoo", 10);
//	o_pPCbangInfo->push_back(tempPCBangInfo);
//
//	// 2007-05-22 by dhjin, 예당 안창석실장님 추가
//	tempPCBangInfo.SetPCBANG_INFO(100000002, "218.55.117.215", "255", "admin", "Yedang", "0-0-0", "000-0000-0000", "000-000", "Seoul", "Seocho-gu", "Seocho-dong", "JaeWoo", 10);
//	o_pPCbangInfo->push_back(tempPCBangInfo);
//
//	tempPCBangInfo.SetPCBANG_INFO(100000003, "10.10.90.1", "255", "admin", "Masang&Yedang", "0-0-0", "000-0000-0000", "000-000", "Seoul", "Seocho-gu", "Seocho-dong", "JaeWoo", 10);
//	o_pPCbangInfo->push_back(tempPCBangInfo);
//
//
//	return nObjects;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::InsertPCBangInfo(CODBCStatement *i_pODBCStmt, vectorPCBANG_INFO *o_pPCbangInfo)
/// \brief		
/// \author		dhjin
/// \date		2007-01-22 ~ 2007-01-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//int CAtumDBHelper::InsertPCBangInfo(CODBCStatement* i_pODBCStmt, vectorPCBANG_INFO* o_pPCbangInfo)
//{
//	SQLINTEGER arrCB[14]; // 2007-06-25 by dhjin, PC방 등급 관련 추가
//	fill_n(arrCB, 14, SQL_NTS);
//
//	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	//	BOOL bRet = i_pODBCStmt->ExecuteQuery("DELETE dbo.td_PCBang");
//
//	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(QUERY_080702_0066);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0066));
//	if (!bRet)
//	{
//		// error
//		i_pODBCStmt->FreeStatement();
//		return -1; // 2007-09-27 by cmkwon, 에러 코드 구분을 위해 수정
//	}
//
//	// 2008-09-01 by cmkwon, SQLPrepare 대체 처리 - 
//	// // 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	// //	if(SQLPrepare(i_pODBCStmt->m_hstmt,(SQLCHAR*)"INSERT INTO dbo.td_PCBang VALUES(?,?,?,?,?, ?,?,?,?,?, ?,?,?)", SQL_NTS) != SQL_SUCCESS)
//	// 	if(SQLPrepare(i_pODBCStmt->m_hstmt,(SQLCHAR*)QUERY_080702_0067, SQL_NTS) != SQL_SUCCESS)
//	// 	{
//	// 		i_pODBCStmt->FreeStatement();
//	// 		return -2;	// 2007-09-27 by cmkwon, 에러 코드 구분을 위해 수정
//	// 	}
//	// 	
//	// 	PCBANG_INFO			tempPCBangInfo;
//	// 	
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tempPCBangInfo.PCBangUID, 0,	&arrCB[1]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, SIZE_MAX_IPADDRESS, 0, tempPCBangInfo.ST_IP, 0,		&arrCB[2]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 4, 0, tempPCBangInfo.ED_IP, 0,						&arrCB[3]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, SIZE_MAX_ACCOUNT_NAME, 0, tempPCBangInfo.User_Id, 0,	&arrCB[4]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 50, 0, tempPCBangInfo.Branch_Name, 0,					&arrCB[5]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 12, 0, tempPCBangInfo.Branch_RegNO, 0,				&arrCB[6]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 14, 0, tempPCBangInfo.Branch_Tel,	0,					&arrCB[7]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 15, 0, tempPCBangInfo.ZipCode,		0,					&arrCB[8]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 10, 0, tempPCBangInfo.Addr_Sido,	0,					&arrCB[9]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,20, 0, tempPCBangInfo.Addr_SiGuGun, 0,				&arrCB[10]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,100,0, tempPCBangInfo.Addr_Dong,	0,				&arrCB[11]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 50,0, tempPCBangInfo.Addr_Detail,  0,				&arrCB[12]);
//	// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tempPCBangInfo.User_Level,  0,		&arrCB[13]);
//	// 
//	// 	int nObjects = 0;
//	// 	vectorPCBANG_INFO::iterator itr(o_pPCbangInfo->begin());
//	// 	while(itr != o_pPCbangInfo->end())
//	// 	{
//	// 		tempPCBangInfo	= *itr;
//	// 		if(SQLExecute(i_pODBCStmt->m_hstmt) != SQL_SUCCESS)
//	// 		{
//	// 			i_pODBCStmt->FreeStatement();
//	// 			return -3;	// 2007-09-27 by cmkwon, 에러 코드 구분을 위해 수정
//	// 		}
//	// 		
//	// 		itr++;
//	// 		nObjects++;
//	// 	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2008-09-01 by cmkwon, SQLPrepare 대체 처리 - 하나씩 추가	
//	int nObjects = 0;
//	vectorPCBANG_INFO::iterator itr(o_pPCbangInfo->begin());
//	for (; itr != o_pPCbangInfo->end(); itr++)
//	{
//		PCBANG_INFO tempPCBangInfo;
//		tempPCBangInfo = *itr;
//
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tempPCBangInfo.PCBangUID, 0, &arrCB[1]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_IPADDRESS, 0, tempPCBangInfo.ST_IP, 0, &arrCB[2]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 4, 0, tempPCBangInfo.ED_IP, 0, &arrCB[3]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 15, 0, tempPCBangInfo.User_Id, 0, &arrCB[4]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, tempPCBangInfo.Branch_Name, 0, &arrCB[5]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 12, 0, tempPCBangInfo.Branch_RegNO, 0, &arrCB[6]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 14, 0, tempPCBangInfo.Branch_Tel, 0, &arrCB[7]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 15, 0, tempPCBangInfo.ZipCode, 0, &arrCB[8]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, tempPCBangInfo.Addr_Sido, 0, &arrCB[9]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, tempPCBangInfo.Addr_SiGuGun, 0, &arrCB[10]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, tempPCBangInfo.Addr_Dong, 0, &arrCB[11]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, tempPCBangInfo.Addr_Detail, 0, &arrCB[12]);
//		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tempPCBangInfo.User_Level, 0, &arrCB[13]);
//		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0067));
//		i_pODBCStmt->FreeStatement();
//		if (!bRet)
//		{
//			// error
//			return -3; // 2007-09-27 by cmkwon, 에러 코드 구분을 위해 수정
//		}
//		nObjects++;
//	}
//
//	return nObjects;
//}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumDBHelper::LoadInfluenceLeader(UID32_t *o_pVCNLeader, UID32_t *o_pANILeader, CODBCStatement *i_pOdbcStmt, int i_nMGServerID)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBHelper::LoadInfluenceLeader(UID32_t* o_pVCNLeader, UID32_t* o_pANILeader, CODBCStatement* i_pOdbcStmt)
{
	BYTE nInflTy;
	UID32_t charUID;
	RETCODE ret;
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//	char szQuery[1024];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	// 	wsprintf(szQuery, "SELECT InfluenceType, InflLeaderCharacterUID \
	// 		FROM td_InfluenceWarData WITH (NOLOCK) WHERE MGameServerID = %d", i_nMGServerID);

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	wsprintf(szQuery, QUERY_080702_0068, i_nMGServerID);
	//	BOOL bRet = i_pOdbcStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = i_pOdbcStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	BOOL bRet = i_pOdbcStmt->ExecuteQuery((char*)(PROCEDURE_080827_0068));
	if (!bRet)
	{// error
		i_pOdbcStmt->FreeStatement();
		return FALSE;
	}

	// Bind Columns
	SQLBindCol(i_pOdbcStmt->m_hstmt, 1, SQL_C_UTINYINT, &nInflTy, 0, &arrCB[1]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 2, SQL_C_ULONG, &charUID, 0, &arrCB[2]);
	nInflTy = 0;
	charUID = 0;
	while ((ret = SQLFetch(i_pOdbcStmt->m_hstmt)) != SQL_NO_DATA)
	{
		switch (nInflTy)
		{
		case INFLUENCE_TYPE_VCN:
			*o_pVCNLeader = charUID;
			break;
		case INFLUENCE_TYPE_ANI:
			*o_pANILeader = charUID;
		}
		nInflTy = 0;
		charUID = 0;
	}
	i_pOdbcStmt->FreeStatement();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumDBHelper::LoadInfluenceSub1Leader(UID32_t *o_pVCNSub1Leader, UID32_t *o_pANISub1Leader, CODBCStatement *i_pOdbcStmt, int i_nMGServerID)
/// \brief		
/// \author		dhjin
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBHelper::LoadInfluenceSub1Leader(UID32_t* o_pVCNSub1Leader, UID32_t* o_pANISub1Leader, CODBCStatement* i_pOdbcStmt)
{
	BYTE nInflTy;
	UID32_t charUID;
	RETCODE ret;
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//	char szQuery[1024];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	// 	wsprintf(szQuery, "SELECT InfluenceType, InflSub1LeaderCharacterUID \
	// 		FROM td_InfluenceWarData WITH (NOLOCK) WHERE MGameServerID = %d", i_nMGServerID);

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	wsprintf(szQuery, QUERY_080702_0069, i_nMGServerID);
	//	BOOL bRet = i_pOdbcStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = i_pOdbcStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	BOOL bRet = i_pOdbcStmt->ExecuteQuery((char*)(PROCEDURE_080827_0069));
	if (!bRet)
	{// error
		i_pOdbcStmt->FreeStatement();
		return FALSE;
	}

	// Bind Columns
	SQLBindCol(i_pOdbcStmt->m_hstmt, 1, SQL_C_UTINYINT, &nInflTy, 0, &arrCB[1]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 2, SQL_C_ULONG, &charUID, 0, &arrCB[2]);
	nInflTy = 0;
	charUID = 0;
	while ((ret = SQLFetch(i_pOdbcStmt->m_hstmt)) != SQL_NO_DATA)
	{
		switch (nInflTy)
		{
		case INFLUENCE_TYPE_VCN:
			*o_pVCNSub1Leader = charUID;
			break;
		case INFLUENCE_TYPE_ANI:
			*o_pANISub1Leader = charUID;
		}
		nInflTy = 0;
		charUID = 0;
	}
	i_pOdbcStmt->FreeStatement();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumDBHelper::LoadInfluenceSub2Leader(UID32_t *o_pVCNSub2Leader, UID32_t *o_pANISub2Leader, CODBCStatement *i_pOdbcStmt, int i_nMGServerID)
/// \brief		
/// \author		dhjin
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBHelper::LoadInfluenceSub2Leader(UID32_t* o_pVCNSub2Leader, UID32_t* o_pANISub2Leader, CODBCStatement* i_pOdbcStmt)
{
	BYTE nInflTy;
	UID32_t charUID;
	RETCODE ret;
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//	char szQuery[1024];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	// 	wsprintf(szQuery, "SELECT InfluenceType, InflSub2LeaderCharacterUID \
	// 		FROM td_InfluenceWarData WITH (NOLOCK) WHERE MGameServerID = %d", i_nMGServerID);

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	wsprintf(szQuery, QUERY_080702_0070, i_nMGServerID);
	//	BOOL bRet = i_pOdbcStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = i_pOdbcStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	BOOL bRet = i_pOdbcStmt->ExecuteQuery((char*)(PROCEDURE_080827_0070));
	if (!bRet)
	{// error
		i_pOdbcStmt->FreeStatement();
		return FALSE;
	}

	// Bind Columns
	SQLBindCol(i_pOdbcStmt->m_hstmt, 1, SQL_C_UTINYINT, &nInflTy, 0, &arrCB[1]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 2, SQL_C_ULONG, &charUID, 0, &arrCB[2]);
	nInflTy = 0;
	charUID = 0;
	while ((ret = SQLFetch(i_pOdbcStmt->m_hstmt)) != SQL_NO_DATA)
	{
		switch (nInflTy)
		{
		case INFLUENCE_TYPE_VCN:
			*o_pVCNSub2Leader = charUID;
			break;
		case INFLUENCE_TYPE_ANI:
			*o_pANISub2Leader = charUID;
		}
		nInflTy = 0;
		charUID = 0;
	}
	i_pOdbcStmt->FreeStatement();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadEachInflueceTypeCount(int *o_pNormalInfl, int *o_pVCNInfl, int *o_pANIInfl, CODBCStatement *i_pODBCStmt)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-07 ~ 2006-02-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::LoadEachInflueceTypeCount(int* o_pNormalInfl, int* o_pVCNInfl, int* o_pANIInfl, CODBCStatement* i_pODBCStmt)
{
	RETCODE ret;
	BYTE byInflType;
	int nInflTypeCount;
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);
	// 2006-05-19 by cmkwon
	// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("SELECT InfluenceType, COUNT(*) FROM td_character	GROUP BY InfluenceType");
	//	char szQuery[1024];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	// 	wsprintf(szQuery, "SELECT InfluenceType, COUNT(*) FROM td_character WITH (NOLOCK) WHERE LastStartedTime > DATEADD(day, %d, GETDATE()) \
	// 					GROUP BY InfluenceType", (-1)*INFLWAR_APPLY_TAX_PERIOD_DAY);

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	wsprintf(szQuery, QUERY_080702_0071, (-1)*INFLWAR_APPLY_TAX_PERIOD_DAY);
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(szQuery);
	int nPeriodDay = (-1) * INFLWAR_APPLY_TAX_PERIOD_DAY;
	SQLHSTMT hstmt = i_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2];
	fill_n(arrCB2, 2, SQL_NTS);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(nPeriodDay), 0, &arrCB2[1]);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0071));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}

	BOOL bWillRet = TRUE;

	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_UTINYINT, &byInflType, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &nInflTypeCount, 0, &arrCB[2]);
	byInflType = INFLUENCE_TYPE_UNKNOWN ;
	nInflTypeCount = 0;
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		switch (byInflType)
		{
		case INFLUENCE_TYPE_NORMAL:
			{
				*o_pNormalInfl = nInflTypeCount;
				bWillRet = TRUE;
			}
			break;
		case INFLUENCE_TYPE_VCN:
			{
				*o_pVCNInfl = nInflTypeCount;
				bWillRet = TRUE;
			}
			break;
		case INFLUENCE_TYPE_ANI:
			{
				*o_pANIInfl = nInflTypeCount;
				bWillRet = TRUE;
			}
			break;
		default:
			{
				bWillRet = FALSE;
			}
		}

		if (FALSE == bWillRet)
		{
			break;
		}

		byInflType = INFLUENCE_TYPE_UNKNOWN ;
		nInflTypeCount = 0;
	}
	i_pODBCStmt->FreeStatement();

	return bWillRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::LoadInfluenceWarInfo(CODBCStatement *i_pODBCStmt, vectDB_INFLUENCE_WAR_INFO *i_pvectDB_INFLUENCE_WAR_INFO)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-14 ~ 2006-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::LoadInfluenceWarInfo(CODBCStatement* i_pODBCStmt, vectDB_INFLUENCE_WAR_INFO* i_pvectDB_INFLUENCE_WAR_INFO)
{
	RETCODE ret;
	SQLINTEGER arrCB[57]; // 2009-03-10 by dhjin, 단계별 모선 시스템
	fill_n(arrCB, 57, SQL_NTS);

	SDB_INFLUENCE_WAR_INFO tmWarInfo;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery("SELECT * FROM ti_InfluenceWar WITH (NOLOCK)");

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(QUERY_080702_0072);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0072));
	if (!bRet)
	{
		// error
		i_pODBCStmt->FreeStatement();
		return 0;
	}
	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_UTINYINT, &tmWarInfo.InfluenceType, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_UTINYINT, &tmWarInfo.WartimeStage, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &tmWarInfo.ReqContributionPoint, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_FLOAT, &tmWarInfo.HPRepairRate, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_FLOAT, &tmWarInfo.DPRepairRate, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_FLOAT, &tmWarInfo.SPRepairRate, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmWarInfo.EventTime, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_FLOAT, &tmWarInfo.EventExperience, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_FLOAT, &tmWarInfo.EventDropSPI, 0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_FLOAT, &tmWarInfo.EventDownExperience, 0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_FLOAT, &tmWarInfo.EventDropItem, 0, &arrCB[11]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_FLOAT, &tmWarInfo.EventDropRareItem, 0, &arrCB[12]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_LONG, &tmWarInfo.SupplyItemNum, 0, &arrCB[13]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 14, SQL_C_LONG, &tmWarInfo.SupplyItemCount, 0, &arrCB[14]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 15, SQL_C_UTINYINT, &tmWarInfo.IsSummonJacoMonster, 0, &arrCB[15]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 16, SQL_C_LONG, &tmWarInfo.BossMonsterUnitKind, 0, &arrCB[16]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 17, SQL_C_LONG, &tmWarInfo.BossMonsterCount, 0, &arrCB[17]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 18, SQL_C_LONG, &tmWarInfo.BossMonsterSummonTerm, 0, &arrCB[18]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 19, SQL_C_SHORT, &tmWarInfo.BossMonsterSummonMapIndex, 0, &arrCB[19]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 20, SQL_C_SHORT, &tmWarInfo.BossMonsterSummonPositionX, 0, &arrCB[20]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 21, SQL_C_SHORT, &tmWarInfo.BossMonsterSummonPositionY, 0, &arrCB[21]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 22, SQL_C_SHORT, &tmWarInfo.BossMonsterSummonPositionZ, 0, &arrCB[22]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 23, SQL_C_LONG, &tmWarInfo.SummonMonsterUnitKind[0], 0, &arrCB[23]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 24, SQL_C_LONG, &tmWarInfo.SummonCount[0], 0, &arrCB[24]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 25, SQL_C_SHORT, &tmWarInfo.SummonMapIndex[0], 0, &arrCB[25]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 26, SQL_C_SHORT, &tmWarInfo.SummonPositionX[0], 0, &arrCB[26]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 27, SQL_C_SHORT, &tmWarInfo.SummonPositionY[0], 0, &arrCB[27]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 28, SQL_C_SHORT, &tmWarInfo.SummonPositionZ[0], 0, &arrCB[28]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 29, SQL_C_LONG, &tmWarInfo.SummonMonsterUnitKind[1], 0, &arrCB[29]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 30, SQL_C_LONG, &tmWarInfo.SummonCount[1], 0, &arrCB[30]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 31, SQL_C_SHORT, &tmWarInfo.SummonMapIndex[1], 0, &arrCB[31]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 32, SQL_C_SHORT, &tmWarInfo.SummonPositionX[1], 0, &arrCB[32]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 33, SQL_C_SHORT, &tmWarInfo.SummonPositionY[1], 0, &arrCB[33]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 34, SQL_C_SHORT, &tmWarInfo.SummonPositionZ[1], 0, &arrCB[34]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 35, SQL_C_LONG, &tmWarInfo.SummonMonsterUnitKind[2], 0, &arrCB[35]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 36, SQL_C_LONG, &tmWarInfo.SummonCount[2], 0, &arrCB[36]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 37, SQL_C_SHORT, &tmWarInfo.SummonMapIndex[2], 0, &arrCB[37]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 38, SQL_C_SHORT, &tmWarInfo.SummonPositionX[2], 0, &arrCB[38]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 39, SQL_C_SHORT, &tmWarInfo.SummonPositionY[2], 0, &arrCB[39]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 40, SQL_C_SHORT, &tmWarInfo.SummonPositionZ[2], 0, &arrCB[40]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 41, SQL_C_LONG, &tmWarInfo.SummonMonsterUnitKind[3], 0, &arrCB[41]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 42, SQL_C_LONG, &tmWarInfo.SummonCount[3], 0, &arrCB[42]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 43, SQL_C_SHORT, &tmWarInfo.SummonMapIndex[3], 0, &arrCB[43]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 44, SQL_C_SHORT, &tmWarInfo.SummonPositionX[3], 0, &arrCB[44]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 45, SQL_C_SHORT, &tmWarInfo.SummonPositionY[3], 0, &arrCB[45]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 46, SQL_C_SHORT, &tmWarInfo.SummonPositionZ[3], 0, &arrCB[46]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 47, SQL_C_LONG, &tmWarInfo.SummonMonsterUnitKind[4], 0, &arrCB[47]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 48, SQL_C_LONG, &tmWarInfo.SummonCount[4], 0, &arrCB[48]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 49, SQL_C_SHORT, &tmWarInfo.SummonMapIndex[4], 0, &arrCB[49]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 50, SQL_C_SHORT, &tmWarInfo.SummonPositionX[4], 0, &arrCB[50]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 51, SQL_C_SHORT, &tmWarInfo.SummonPositionY[4], 0, &arrCB[51]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 52, SQL_C_SHORT, &tmWarInfo.SummonPositionZ[4], 0, &arrCB[52]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 53, SQL_C_LONG, &tmWarInfo.SupplyItemNum2, 0, &arrCB[53]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 54, SQL_C_LONG, &tmWarInfo.SupplyItemCount2, 0, &arrCB[54]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 55, SQL_C_LONG, &tmWarInfo.WinBossMonsterUnitKind, 0, &arrCB[55]); // 2009-03-10 by dhjin, 단계별 모선 시스템
	SQLBindCol(i_pODBCStmt->m_hstmt, 56, SQL_C_LONG, &tmWarInfo.LossBossMonsterUnitKind, 0, &arrCB[56]); // 2009-03-10 by dhjin, 단계별 모선 시스템

	util::zero(&tmWarInfo, sizeof(tmWarInfo));
	while ((ret = SQLFetch(i_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		i_pvectDB_INFLUENCE_WAR_INFO->push_back(tmWarInfo);
		util::zero(&tmWarInfo, sizeof(tmWarInfo));
	}
	i_pODBCStmt->FreeStatement();

	return i_pvectDB_INFLUENCE_WAR_INFO->size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumDBHelper::LoadEventMonster(mtvectSEVENT_MONSTER *o_pEventMonsterList, CODBCStatement *i_pOdbcStmt, int i_nServerGroupID/*=0*/)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CAtumDBHelper::LoadEventMonster() 추가
/// \author		cmkwon
/// \date		2008-04-16 ~ 2008-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBHelper::LoadEventMonster(mtvectSEVENT_MONSTER* o_pEventMonsterList, CODBCStatement* i_pOdbcStmt)
{
	RETCODE ret;
	SQLINTEGER arrCB[13];
	fill_n(arrCB, 13, SQL_NTS);

	SEVENT_MONSTER tmEventMonster;
	SQL_TIMESTAMP_STRUCT StartDTime;
	SQL_TIMESTAMP_STRUCT EndDTime;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//	BOOL bRet = i_pOdbcStmt->ExecuteQuery("SELECT EventMonsterUID, ServerGroupID, StartDateTime, EndDateTime, SummonerMapIndex, \
	//		SummonerReqMinLevel, SummonerReqMaxLevel, SummonerExceptMonster, SummonMonsterNum, SummonMonsterCount, SummonDelayTime, SummonProbability FROM ti_EventMonster WITH (NOLOCK)");

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	BOOL bRet = i_pOdbcStmt->ExecuteQuery(QUERY_080702_0073);
	BOOL bRet = i_pOdbcStmt->ExecuteQuery((char*)(PROCEDURE_080827_0073));
	if (!bRet)
	{
		i_pOdbcStmt->FreeStatement();
		return FALSE; // error
	}

	// Bind Columns
	SQLBindCol(i_pOdbcStmt->m_hstmt, 1, SQL_C_LONG, &tmEventMonster.EventMonsterUID, 0, &arrCB[1]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 2, SQL_C_LONG, &tmEventMonster.ServerGroupID, 0, &arrCB[2]); // 2008-04-28 by cmkwon, 버그 수정
	SQLBindCol(i_pOdbcStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &StartDTime, 0, &arrCB[3]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 4, SQL_C_TIMESTAMP, &EndDTime, 0, &arrCB[4]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 5, SQL_C_USHORT, &tmEventMonster.SummonerMapIndex, 0, &arrCB[5]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 6, SQL_C_UTINYINT, &tmEventMonster.SummonerReqMinLevel, 0, &arrCB[6]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 7, SQL_C_UTINYINT, &tmEventMonster.SummonerReqMaxLevel, 0, &arrCB[7]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 8, SQL_C_ULONG, &tmEventMonster.SummonerExceptMonster, 0, &arrCB[8]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 9, SQL_C_LONG, &tmEventMonster.SummonMonsterNum, 0, &arrCB[9]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 10, SQL_C_LONG, &tmEventMonster.SummonMonsterCount, 0, &arrCB[10]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 11, SQL_C_SHORT, &tmEventMonster.SummonDelayTime, 0, &arrCB[11]);
	SQLBindCol(i_pOdbcStmt->m_hstmt, 12, SQL_C_PROB10K, &tmEventMonster.SummonProbability, 0, &arrCB[12]);

	util::zero(&tmEventMonster, sizeof(SEVENT_MONSTER));
	util::zero(&StartDTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&EndDTime, sizeof(SQL_TIMESTAMP_STRUCT));
	ATUM_DATE_TIME atCur { true };
	while ((ret = SQLFetch(i_pOdbcStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmEventMonster.StartDateTime = StartDTime;
		tmEventMonster.EndDateTime = EndDTime;

		if (tmEventMonster.EndDateTime > atCur) o_pEventMonsterList->push_back(tmEventMonster);

		util::zero(&tmEventMonster, sizeof(SEVENT_MONSTER));
		util::zero(&StartDTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&EndDTime, sizeof(SQL_TIMESTAMP_STRUCT));
	}
	i_pOdbcStmt->FreeStatement();

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumDBHelper::LoadArenaMapList(vectMapIndexList *o_pArenaMapIndexList, CODBCStatement *i_pODBCStmt)
/// \brief		// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
/// \author		cmkwon
/// \date		2008-09-10 ~ 2008-09-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBHelper::LoadArenaMapList(vectMapIndexList* o_pArenaMapIndexList, CODBCStatement* i_pODBCStmt)
{
	o_pArenaMapIndexList->clear();

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_LoadArenaMapInfo
	-- DESC			 : ArenaMapInfo 얻어오기
	-- 2007-04-25 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadArenaMapInfo
	AS
		SELECT ArenaMapIndex, ArenaMode, ReqMemberNum, PlayLimitedTime, WINCondition, 
				LvDSupplyItemNum1, LvDSupplyItemCount1, LvDSupplyItemNum2, LvDSupplyItemCount2, LvDSupplyItemNum3, LvDSupplyItemCount3
		FROM atum2_db_account.dbo.ti_ArenaMapInfo WITH (NOLOCK)
	GO
	************************************************************************/
	RETCODE ret = SQLExecDirect(i_pODBCStmt->m_hstmt, PROCEDURE_080822_0033, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
	SQLINTEGER arrCB[CB_COUNT_ti_ArenaMapinfo];
	fill_n(arrCB, CB_COUNT_ti_ArenaMapinfo, SQL_NTS);
	// end 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)

	SDBARENA_MAPINFO tmDBArenaMapInfo;
	util::zero(&tmDBArenaMapInfo, sizeof(SDBARENA_MAPINFO));

	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmDBArenaMapInfo.ArenaMapIndex, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_TINYINT, &tmDBArenaMapInfo.ArenaMode, 0, &arrCB[2]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 3, SQL_C_TINYINT, &tmDBArenaMapInfo.ReqMemberNum, 0, &arrCB[3]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &tmDBArenaMapInfo.PlayLimitedTime, 0, &arrCB[4]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &tmDBArenaMapInfo.WINCondition, 0, &arrCB[5]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum1, 0, &arrCB[6]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount1, 0, &arrCB[7]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum2, 0, &arrCB[8]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount2, 0, &arrCB[9]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum3, 0, &arrCB[10]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 11, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount3, 0, &arrCB[11]);
	// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
#ifdef _ARENA_ADDITIONAL_SUPPLY_ITEMS
	SQLBindCol(i_pODBCStmt->m_hstmt, 12, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum4, 0, &arrCB[12]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 13, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount4, 0, &arrCB[13]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 14, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemNum5, 0, &arrCB[14]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 15, SQL_C_LONG, &tmDBArenaMapInfo.LvDSupplyItemCount5, 0, &arrCB[15]);
#endif
	// end 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)

	while (TRUE)
	{
		util::zero(&tmDBArenaMapInfo, sizeof(SDBARENA_MAPINFO));
		ret = SQLFetch(i_pODBCStmt->m_hstmt);
		if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}

		vectMapIndexList::iterator itr = find(o_pArenaMapIndexList->begin(), o_pArenaMapIndexList->end(), tmDBArenaMapInfo.ArenaMapIndex);
		if (itr == o_pArenaMapIndexList->end())
		{
			o_pArenaMapIndexList->push_back(tmDBArenaMapInfo.ArenaMapIndex);
		}
	}
	i_pODBCStmt->FreeStatement();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveItem(CODBCStatement *i_pODBCStmt, vector<ITEM> *o_pItem)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveItem(CODBCStatement *i_pODBCStmt, vector<ITEM> *o_pItem)
int CAtumDBHelper::SaveItem(CODBCStatement* i_pODBCStmt, vector<ITEM>* o_pItem, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[CB_COUNT_ti_Item];
	std::fill_n(arrCB, CB_COUNT_ti_Item, SQL_NTS);

	//Delete ti_Item
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0373, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pItem->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인

	//Insert ti_Item
	int nObjects = 0;
	vector<ITEM>::iterator itr(o_pItem->begin());
	for (; itr != o_pItem->end(); itr++)
	{
		ITEM tmItem;
		tmItem = *itr;
		int i = 0;

		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItem.ItemNum, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.Kind, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ITEM_NAME, 0, tmItem.ItemName, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.AbilityMin, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.AbilityMax, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ReqRace, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmItem.ReqGearStat.AttackPart, 0, &arrCB[i]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmItem.ReqGearStat.DefensePart, 0, &arrCB[i]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmItem.ReqGearStat.FuelPart, 0, &arrCB[i]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmItem.ReqGearStat.SoulPart, 0, &arrCB[i]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmItem.ReqGearStat.ShieldPart, 0, &arrCB[i]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmItem.ReqGearStat.DodgePart, 0, &arrCB[i]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmItem.ReqUnitKind, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.ReqMinLevel, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.ReqMaxLevel, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.ReqItemKind, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.Weight, 0, &arrCB[i]);

		// 2010-07-19 by dhjin, 확률 수식 변경	// 2009-12-17 by cmkwon, 데미지 계산식 관련한 필드(방어력,회피,피어스,확률)에 255이상 값을 설정 가능하게 수정 - 공격확률
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.HitRate, 0, &arrCB[i]);

		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.Defense, 0, &arrCB[i]);

		// 2010-07-19 by dhjin, 확률 수식 변경 // 2009-12-17 by cmkwon, 데미지 계산식 관련한 필드(방어력,회피,피어스,확률)에 255이상 값을 설정 가능하게 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.FractionResistance, 0, &arrCB[i]);

		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.NaturalFaction, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.SpeedPenalty, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.Range, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.Position, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.Scarcity, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.Endurance, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.AbrasionRate, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.Charging, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.Luck, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.MinTradeQuantity, 0, &arrCB[i]);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmItem.SellingPrice, 0, &arrCB[i]);
#else // SHOP_PRICES_PER_BUILDING_NPC

		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmItem.Price, 0,						&arrCB[i]);		// 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmItem.CashPrice, 0,					&arrCB[i]);		// 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
#endif // SHOP_PRICES_PER_BUILDING_NPC

		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[0], 0, &arrCB[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[0], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[1], 0, &arrCB[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[1], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[2], 0, &arrCB[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[2], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[3], 0, &arrCB[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[3], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[4], 0, &arrCB[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[4], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[5], 0, &arrCB[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[5], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[6], 0, &arrCB[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[6], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[7], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[7], 0, &arrCB[i]);
#ifdef _Parameter_ADDITIONAL
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[8], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[8], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ArrDestParameter[9], 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.ArrParameterValue[9], 0, &arrCB[i]);
#endif 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmItem.ReAttacktime, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItem.Time, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.RepeatTime, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.Material, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ReqMaterial, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.RangeAngle, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.UpgradeNum, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItem.LinkItem, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.MultiTarget, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ExplosionRange, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.ReactionRange, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.ShotNum, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.MultiNum, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmItem.AttackTime, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.ReqSP, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItem.SummonMonster, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItem.NextSkill, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.SkillLevel, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.SkillHitRate, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.SkillType, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.SkillTargetType, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.Caliber, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.OrbitType, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &tmItem.ItemAttribute, 0, &arrCB[i]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItem.BoosterAngle, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItem.CameraPattern, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItem.SourceIndex, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ITEM_DESCRIPTION, 0, tmItem.Description, 0, &arrCB[i]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItem.EnchantCheckDestParam, 0, &arrCB[i]); // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmItem.InvokingDestParamID, 0, &arrCB[i]); // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
		SQLBindParameter(i_pODBCStmt->m_hstmt, ++i, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmItem.InvokingDestParamIDByUse, 0, &arrCB[i]); // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템


		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0350));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			//error
			char err[1024];
			sprintf(err, "ERROR line : ItemNum(%d)   ItemName(%s)!!", tmItem.ItemNum, tmItem.ItemName);
			MessageBox(NULL, err, "ti_Item", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveItemInfo(CODBCStatement *i_pODBCStmt, vectItem_Info *o_pItemInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveItemInfo(CODBCStatement *i_pODBCStmt, vectItem_Info *o_pItemInfo)
int CAtumDBHelper::SaveItemInfo(CODBCStatement* i_pODBCStmt, vectItem_Info* o_pItemInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[9];
	fill_n(arrCB, 9, SQL_NTS);

	//Delete ti_MonsterItem
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0395, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pItemInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_MonsterItem
	int nObjects = 0;
	vectItem_Info::iterator itr(o_pItemInfo->begin());
	for (; itr != o_pItemInfo->end(); itr++)
	{
		ITEM_INFO tmItemInfo;
		tmItemInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItemInfo.IsDisable, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItemInfo.GameUID, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItemInfo.ItemNum, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ITEM_NAME, 0, tmItemInfo.ItemName, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItemInfo.TabIndex, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_TABINDEX, 0, tmItemInfo.IconFileName, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmItemInfo.ItemUnitPrice, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ITEMDESCRIPTION, 0, tmItemInfo.ItemDescription, 0, &arrCB[8]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0372));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : ItemNum(%d)   ItemName(%s)!!", (int)tmItemInfo.ItemNum, tmItemInfo.ItemName);
			MessageBox(NULL, err, "ti_MonsterItem", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveMonsterItem(CODBCStatement *i_pODBCStmt, vector<MONSTER2ITEM> *o_pMonsterItemInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveMonsterItem(CODBCStatement *i_pODBCStmt, vector<MONSTER2ITEM> *o_pMonsterItemInfo)
int CAtumDBHelper::SaveMonsterItem(CODBCStatement* i_pODBCStmt, vector<MONSTER2ITEM>* o_pMonsterItemInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[9];
	fill_n(arrCB, 9, SQL_NTS);

	//Delete ti_MonsterItem
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0374, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pMonsterItemInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인

	//Insert ti_MonsterItem
	int nObjects = 0;
	vector<MONSTER2ITEM>::iterator itr(o_pMonsterItemInfo->begin());
	for (; itr != o_pMonsterItemInfo->end(); itr++)
	{
		MONSTER2ITEM tmMonster2Item;
		tmMonster2Item = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster2Item.MonsterUniqueNumber, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster2Item.ItemNumber, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster2Item.MinCount, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster2Item.MaxCount, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster2Item.Probability, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster2Item.PrefixProbability, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster2Item.SuffixProbability, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster2Item.DropType, 0, &arrCB[8]); // 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0351));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MonsterUniqueNumber(%d)   ItemNum(%d)!!", tmMonster2Item.MonsterUniqueNumber, tmMonster2Item.ItemNumber);
			MessageBox(NULL, err, "ti_MonsterItem", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveRareItemInfo(CODBCStatement *i_pODBCStmt, vector<RARE_ITEM_INFO> *o_pRareItemInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveRareItemInfo(CODBCStatement *i_pODBCStmt, vector<RARE_ITEM_INFO> *o_pRareItemInfo)
int CAtumDBHelper::SaveRareItemInfo(CODBCStatement* i_pODBCStmt, vector<RARE_ITEM_INFO>* o_pRareItemInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[32];
	fill_n(arrCB, 32, SQL_NTS);

	//Delete ti_RareItemInfo
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0375, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pRareItemInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_RareItemInfo
	int nObjects = 0;
	vector<RARE_ITEM_INFO>::iterator itr(o_pRareItemInfo->begin());
	for (; itr != o_pRareItemInfo->end(); itr++)
	{
		RARE_ITEM_INFO tmRareItemInfo;
		tmRareItemInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmRareItemInfo.CodeNum, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_RARE_FIX_NAME, 0, tmRareItemInfo.Name, 0, &arrCB[2]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmRareItemInfo.ReqUseType, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmRareItemInfo.ReqMinLevel, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmRareItemInfo.ReqMaxLevel, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.ReqItemKind, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmRareItemInfo.ReqGearStat.AttackPart, 0, &arrCB[7]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmRareItemInfo.ReqGearStat.DefensePart, 0, &arrCB[8]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmRareItemInfo.ReqGearStat.DodgePart, 0, &arrCB[9]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmRareItemInfo.ReqGearStat.FuelPart, 0, &arrCB[10]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmRareItemInfo.ReqGearStat.ShieldPart, 0, &arrCB[11]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmRareItemInfo.ReqGearStat.SoulPart, 0, &arrCB[12]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[0], 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[0], 0, &arrCB[14]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[1], 0, &arrCB[15]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[1], 0, &arrCB[16]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[2], 0, &arrCB[17]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[2], 0, &arrCB[18]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[3], 0, &arrCB[19]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[3], 0, &arrCB[20]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[4], 0, &arrCB[21]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 22, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[4], 0, &arrCB[22]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 23, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[5], 0, &arrCB[23]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 24, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[5], 0, &arrCB[24]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 25, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[6], 0, &arrCB[25]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 26, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[6], 0, &arrCB[26]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 27, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[7], 0, &arrCB[27]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 28, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[7], 0, &arrCB[28]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 29, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRareItemInfo.DesParameter[8], 0, &arrCB[29]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 30, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmRareItemInfo.ParameterValue[8], 0, &arrCB[30]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 31, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmRareItemInfo.Probability, 0, &arrCB[31]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0352));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : CodeNum(%d)   Name(%s)!!", tmRareItemInfo.CodeNum, tmRareItemInfo.Name);
			MessageBox(NULL, err, "ti_RareItemInfo", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveShop(CODBCStatement *i_pODBCStmt, vectSHOP_INFO *o_pShopInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveShop(CODBCStatement *i_pODBCStmt, vectSHOP_INFO *o_pShopInfo)
int CAtumDBHelper::SaveShop(CODBCStatement* i_pODBCStmt, vectSHOP_INFO* o_pShopInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	//Delete ti_Shop
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0376, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pShopInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_Shop
	int nObjects = 0;
	vectSHOP_INFO::iterator itr(o_pShopInfo->begin());
	for (; itr != o_pShopInfo->end(); itr++)
	{
		SHOP_INFO tmShopInfo;
		tmShopInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmShopInfo.ShopOrder, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmShopInfo.UniqueNumber, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmShopInfo.ItemNum, 0, &arrCB[3]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0353));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : ShopOrder(%d)   UniqueNumber(%d)   ItemNum(%d)!!", tmShopInfo.ShopOrder, tmShopInfo.UniqueNumber, tmShopInfo.ItemNum);
			MessageBox(NULL, err, "ti_Shop", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveCityTargetWarpMap(CODBCStatement *i_pODBCStmt, vectCITY_TARGET_WARPMAP *o_pTargetWarp)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveCityTargetWarpMap(CODBCStatement *i_pODBCStmt, vectCITY_TARGET_WARPMAP *o_pTargetWarp)
int CAtumDBHelper::SaveCityTargetWarpMap(CODBCStatement* i_pODBCStmt, vectCITY_TARGET_WARPMAP* o_pTargetWarp, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[7];
	fill_n(arrCB, 7, SQL_NTS);

	//Delete ti_CityTargetWarpMap
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0377, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pTargetWarp->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_CityTargetWarpMap
	int nObjects = 0;
	vectCITY_TARGET_WARPMAP::iterator itr(o_pTargetWarp->begin());
	for (; itr != o_pTargetWarp->end(); itr++)
	{
		CITY_TARGET_WARPMAP tmTargetWarp;
		tmTargetWarp = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTargetWarp.MapIndex, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTargetWarp.BuildingIndex, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTargetWarp.WarpTargetMapIndex, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTargetWarp.WarpTargetIndex, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_WARP_TARGET_NAME, 0, &tmTargetWarp.WarpTargetName, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTargetWarp.Fee, 0, &arrCB[6]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0354));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MapIndex(%d)   WarpTargetName(%s)!!", tmTargetWarp.MapIndex, tmTargetWarp.WarpTargetName);
			MessageBox(NULL, err, "ti_CityTargetWarpMap", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveItemMixingInfo(CODBCStatement *i_pODBCStmt, vector<ITEM_MIXING_INFO_TOOL> *o_pMapItemMixingInfo, HWND hWnd)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
///				// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo				
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveItemMixingInfo(CODBCStatement *i_pODBCStmt, vector<ITEM_MIXING_INFO> *o_pMapItemMixingInfo)
int CAtumDBHelper::SaveItemMixingInfo(CODBCStatement* i_pODBCStmt, vector<ITEM_MIXING_INFO_TOOL>* o_pMapItemMixingInfo, HWND hWnd) // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[6]; // 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가) - // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	fill_n(arrCB, 6, SQL_NTS);

	//Delete ti_ItemMixingInfo
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0378, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pMapItemMixingInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_ItemMixingInfo
	int nObjects = 0;
	vector<ITEM_MIXING_INFO_TOOL>::iterator itr(o_pMapItemMixingInfo->begin()); // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	for (; itr != o_pMapItemMixingInfo->end(); itr++)
	{
		ITEM_MIXING_INFO_TOOL tmItemMixingInfo; // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
		tmItemMixingInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMixingInfo.UniqueID, 0, &arrCB[1]); // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMixingInfo.TargetItemNum, 0, &arrCB[2]); // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMixingInfo.MixingProbability, 0, &arrCB[3]); // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMixingInfo.MixingCost, 0, &arrCB[4]); // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmItemMixingInfo.Visible, 0, &arrCB[5]); // 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가)

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0355));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : UniqueID(%d) TargetItemNum(%d)   MixingCost(%d)   MixingProbability(%d)	Visible(%d) !!", tmItemMixingInfo.UniqueID, tmItemMixingInfo.TargetItemNum, tmItemMixingInfo.MixingProbability, tmItemMixingInfo.MixingCost, tmItemMixingInfo.Visible); // 2013-05-06 by hskim, 아이템 미리 보기 (속성 값 추가) - // 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
			MessageBox(NULL, err, "ti_ItemMixingInfo", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

// start 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveItemMixingElement(CODBCStatement *i_pODBCStmt, vector<ITEM_MIXING_ELEMENT_TOOL> *o_pMapItemMixingElement, HWND hWnd)
/// \brief		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
/// \author		hskim
/// \date		
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveItemMixingElement(CODBCStatement* i_pODBCStmt, vector<ITEM_MIXING_ELEMENT_TOOL>* o_pMapItemMixingElement, HWND hWnd)
{
	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	//Delete ti_ItemMixingInfo
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_120220_0002, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}

	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pMapItemMixingElement->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_ItemMixingInfo
	int nObjects = 0;
	vector<ITEM_MIXING_ELEMENT_TOOL>::iterator itr(o_pMapItemMixingElement->begin());
	for (; itr != o_pMapItemMixingElement->end(); itr++)
	{
		ITEM_MIXING_ELEMENT_TOOL tmItemMixingInfo;
		tmItemMixingInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMixingInfo.UniqueID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMixingInfo.SourceItem.ItemNum, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMixingInfo.SourceItem.Count, 0, &arrCB[3]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_120220_0001));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : UniqueID(%d)   ItemNum(%d)   Count(%d)!!", tmItemMixingInfo.UniqueID, tmItemMixingInfo.SourceItem.ItemNum, tmItemMixingInfo.SourceItem.Count);
			MessageBox(NULL, err, "ti_ItemMixingElement", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

// end 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveEnchantInfo(CODBCStatement *i_pODBCStmt, vector<ENCHANT_INFO> *o_pEnchantInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveEnchantInfo(CODBCStatement *i_pODBCStmt, vector<ENCHANT_INFO> *o_pEnchantInfo)
int CAtumDBHelper::SaveEnchantInfo(CODBCStatement* i_pODBCStmt, vector<ENCHANT_INFO>* o_pEnchantInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[44];
	fill_n(arrCB, 44, SQL_NTS);

	//Delete ti_EnchantInfo
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0379, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pEnchantInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_EnchantInfo
	int nObjects = 0;
	vector<ENCHANT_INFO>::iterator itr(o_pEnchantInfo->begin());
	for (; itr != o_pEnchantInfo->end(); itr++)
	{
		ENCHANT_INFO tmEnchantInfo;
		tmEnchantInfo = *itr;

		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-30 by cmkwon, SCMonitor 엑셀파일 밀어넣기 툴 버그 수정 - 자료형 버그 수정 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmEnchantInfo.EnchantItemNum, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmEnchantInfo.EnchantItemCount, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmEnchantInfo.EnchantCost, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[0], 0, &arrCB[4]); // 2009-09-23 by cmkwon, 태국 서버군 통합 관련 서버군 정리 - 여기부터 아래 주석 까지 수정됨
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[1], 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[2], 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[3], 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[4], 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[5], 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[6], 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[7], 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[8], 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[9], 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[10], 0, &arrCB[14]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[11], 0, &arrCB[15]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[12], 0, &arrCB[16]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[13], 0, &arrCB[17]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[14], 0, &arrCB[18]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[15], 0, &arrCB[19]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[16], 0, &arrCB[20]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[17], 0, &arrCB[21]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 22, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[18], 0, &arrCB[22]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 23, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[19], 0, &arrCB[23]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 24, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[20], 0, &arrCB[24]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 25, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[21], 0, &arrCB[25]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 26, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[22], 0, &arrCB[26]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 27, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[23], 0, &arrCB[27]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 28, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[24], 0, &arrCB[28]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 29, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[25], 0, &arrCB[29]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 30, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[26], 0, &arrCB[30]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 31, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[27], 0, &arrCB[31]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 32, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[28], 0, &arrCB[32]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 33, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[29], 0, &arrCB[33]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 34, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[30], 0, &arrCB[34]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 35, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[31], 0, &arrCB[35]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 36, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[32], 0, &arrCB[36]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 37, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[33], 0, &arrCB[37]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 38, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[34], 0, &arrCB[38]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 39, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[35], 0, &arrCB[39]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 40, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[36], 0, &arrCB[40]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 41, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[37], 0, &arrCB[41]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 42, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[38], 0, &arrCB[42]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 43, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_INTEGER, 0, 0, &tmEnchantInfo.ProbabilityPerLevel[39], 0, &arrCB[43]); // 2009-09-23 by cmkwon, 태국 서버군 통합 관련 서버군 정리 - 위의 주석부터 여기까지 수정됨
		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0356));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : EnchantItemNum(%d)   EnchantItemCount(%d)   EnchantCost(%d)!!", tmEnchantInfo.EnchantItemNum, tmEnchantInfo.EnchantItemCount, tmEnchantInfo.EnchantCost);
			MessageBox(NULL, err, "ti_EnchantInfo", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveStrategyPointSummonMapIndex(CODBCStatement *i_pODBCStmt, vectSTRATEGYPOINT_SUMMON_MAPINDEX *o_pStrategyPoint)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveStrategyPointSummonMapIndex(CODBCStatement *i_pODBCStmt, vectSTRATEGYPOINT_SUMMON_MAPINDEX *o_pStrategyPoint)
int CAtumDBHelper::SaveStrategyPointSummonMapIndex(CODBCStatement* i_pODBCStmt, vectSTRATEGYPOINT_SUMMON_MAPINDEX* o_pStrategyPoint, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[4];
	fill_n(arrCB, 4, SQL_NTS);

	//Delete ti_StrategyPointSummonMapIndex
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0380, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pStrategyPoint->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_StrategyPointSummonMapIndex
	int nObjects = 0;
	vectSTRATEGYPOINT_SUMMON_MAPINDEX::iterator itr(o_pStrategyPoint->begin());
	for (; itr != o_pStrategyPoint->end(); itr++)
	{
		STRATEGYPOINT_SUMMON_MAPINDEX tmStrategyPoint;
		tmStrategyPoint = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_MAP_NAME, 0, tmStrategyPoint.MapName, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmStrategyPoint.MapIndex, 0, &arrCB[2]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmStrategyPoint.StratrgyPiontNum, 0, &arrCB[3]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0357));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MapName(%s)   MapIndex(%d)!!", tmStrategyPoint.MapName, tmStrategyPoint.MapIndex);
			MessageBox(NULL, err, "ti_StrategyPointSummonMapIndex", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveArenaInfo(CODBCStatement *i_pODBCStmt, vectorSDBARENA_INFO *o_pArenaInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveArenaInfo(CODBCStatement *i_pODBCStmt, vectorSDBARENA_INFO *o_pArenaInfo)
int CAtumDBHelper::SaveArenaInfo(CODBCStatement* i_pODBCStmt, vectorSDBARENA_INFO* o_pArenaInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[7];
	fill_n(arrCB, 7, SQL_NTS);

	//Delete ti_ArenaInfo
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0381, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pArenaInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_ArenaInfo
	int nObjects = 0;
	vectorSDBARENA_INFO::iterator itr(o_pArenaInfo->begin());
	for (; itr != o_pArenaInfo->end(); itr++)
	{
		SDBARENA_INFO tmArenaInfo;
		tmArenaInfo = *itr;
		//////////////////////////////////////////////////////////////////////////////
		// 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
		/************************************************************************
		--------------------------------------------------------------
		-- ti_ArenaInfo 데이터 생성 관련
		--------------------------------------------------------------
		-- !!!!
		-- Name:	atum_import_ti_arenainfo
		-- Desc: // 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 -
		--		 --> 2012-05-23 by jhseol, 아레나 WP 기본지급량 필드 추가
		-- ====
		ALTER PROCEDURE [dbo].[atum_import_ti_arenainfo]
		@ArenaMode			TINYINT,
		@ReqMemberNum			TINYINT,
		@PayInfluencePointWIN 		INT,
		@PayInfluencePointLOSE		INT,
		@PayWarPointWIN			INT,
		@PayWarPointLOSE		INT,
		@DefaultPayWarPointWIN		INT,
		@DefaultPayWarPointLOSE		INT
		AS
		INSERT INTO atum2_db_account.dbo.ti_arenainfo(ArenaMode, ReqMemberNum, PayInfluencePointWIN, PayInfluencePointLOSE, PayWarPointWIN, PayWarPointLOSE, DefaultPayWarPointWIN, DefaultPayWarPointLOSE)
		VALUES (@ArenaMode, @ReqMemberNum, @PayInfluencePointWIN, @PayInfluencePointLOSE, @PayWarPointWIN, @PayWarPointLOSE, @DefaultPayWarPointWIN, @DefaultPayWarPointLOSE)
		GO
		************************************************************************/
		// end 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &tmArenaInfo.ArenaMode, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &tmArenaInfo.ReqMemberNum, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaInfo.PayInfluencePointWIN, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaInfo.PayInfluencePointLOSE, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaInfo.PayWarPointWIN, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaInfo.PayWarPointLOSE, 0, &arrCB[6]);
		//////////////////////////////////////////////////////////////////////////////
		// 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaInfo.DefaultPayWarPointWIN, 0, NULL);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaInfo.DefaultPayWarPointLOSE, 0, NULL);
		// end 2012-05-23 by jhseol, 아레나 추가개발 - 최소 보상 워포 필드 추가
		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0358));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : ArenaMode(%d)   ReqMemberNum(%d)!!", tmArenaInfo.ArenaMode, tmArenaInfo.ReqMemberNum);
			MessageBox(NULL, err, "ti_ArenaInfo", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveArenaMapinfo(CODBCStatement *i_pODBCStmt, vectorSDBARENA_MAPINFO *o_pArenaMapInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveArenaMapinfo(CODBCStatement *i_pODBCStmt, vectorSDBARENA_MAPINFO *o_pArenaMapInfo)
int CAtumDBHelper::SaveArenaMapinfo(CODBCStatement* i_pODBCStmt, vectorSDBARENA_MAPINFO* o_pArenaMapInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
	SQLINTEGER arrCB[CB_COUNT_ti_ArenaMapinfo];
	fill_n(arrCB, CB_COUNT_ti_ArenaMapinfo, SQL_NTS);
	// end 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)

	//Delete ti_ArenaMapinfo
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0382, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pArenaMapInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_ArenaMapinfo
	int nObjects = 0;
	vectorSDBARENA_MAPINFO::iterator itr(o_pArenaMapInfo->begin());
	for (; itr != o_pArenaMapInfo->end(); itr++)
	{
		SDBARENA_MAPINFO tmArenaMapInfo;
		tmArenaMapInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmArenaMapInfo.ArenaMapIndex, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &tmArenaMapInfo.ArenaMode, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &tmArenaMapInfo.ReqMemberNum, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &tmArenaMapInfo.PlayLimitedTime, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &tmArenaMapInfo.WINCondition, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemNum1, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemCount1, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemNum2, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemCount2, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemNum3, 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemCount3, 0, &arrCB[11]);
		// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)
#ifdef _ARENA_ADDITIONAL_SUPPLY_ITEMS
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemNum4, 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemCount4, 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemNum5, 0, &arrCB[14]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmArenaMapInfo.LvDSupplyItemCount5, 0, &arrCB[15]);
#endif
		// 2012-09-14 by jhseol, 아레나 추가개발 part3 - 아레나 지급 아이템 추가 (아레나 전용 탄약 재충전 카드)

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0359));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : ArenaMapIndex(%d)   ArenaMode(%d)!!", tmArenaMapInfo.ArenaMapIndex, tmArenaMapInfo.ArenaMode);
			MessageBox(NULL, err, "ti_ArenaMapinfo", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveMysteryItemDrop(CODBCStatement *i_pODBCStmt, vectMYSTERY_ITEM_DROP *o_pMysteryItemDrop)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveMysteryItemDrop(CODBCStatement *i_pODBCStmt, vectMYSTERY_ITEM_DROP *o_pMysteryItemDrop)
int CAtumDBHelper::SaveMysteryItemDrop(CODBCStatement* i_pODBCStmt, vectMYSTERY_ITEM_DROP* o_pMysteryItemDrop, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[14];
	fill_n(arrCB, 14, SQL_NTS);

	//Delete ti_MysteryItemDrop
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0383, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pMysteryItemDrop->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_MysteryItemDrop
	int nObjects = 0;
	vectMYSTERY_ITEM_DROP::iterator itr(o_pMysteryItemDrop->begin());
	for (; itr != o_pMysteryItemDrop->end(); itr++)
	{
		MYSTERY_ITEM_DROP tmMysteryItemDrop;
		tmMysteryItemDrop = *itr;

		char szStarttime[SIZE_MAX_SQL_DATETIME_STRING];
		tmMysteryItemDrop.Starttime.GetSQLDateTimeString(szStarttime, SIZE_MAX_SQL_DATETIME_STRING);

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.MysteryItemDropNum, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.ReqUnitKind, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMysteryItemDrop.ReqMinLevel, 0, &arrCB[3]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMysteryItemDrop.ReqMaxLevel, 0, &arrCB[4]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.DropItemNum, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.MinCount, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.MaxCount, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.Probability, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.PrefixProbability, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.SuffixProbability, 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmMysteryItemDrop.Period, 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMysteryItemDrop.CountPerPeriod, 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szStarttime, 0, &arrCB[13]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0360));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MysteryItemDropNum(%d)   DropItemNum(%d)!!", tmMysteryItemDrop.MysteryItemDropNum, tmMysteryItemDrop.DropItemNum);
			MessageBox(NULL, err, "ti_MysteryItemDrop", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveInfluenceWar(CODBCStatement *i_pODBCStmt, vectDB_INFLUENCE_WAR_INFO *o_pInfluenceWar)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveInfluenceWar(CODBCStatement *i_pODBCStmt, vectDB_INFLUENCE_WAR_INFO *o_pInfluenceWar)
int CAtumDBHelper::SaveInfluenceWar(CODBCStatement* i_pODBCStmt, vectDB_INFLUENCE_WAR_INFO* o_pInfluenceWar, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[57];
	fill_n(arrCB, 57, SQL_NTS);

	//Delete ti_InfluenceWar
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0384, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pInfluenceWar->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_InfluenceWar
	int nObjects = 0;
	vectDB_INFLUENCE_WAR_INFO::iterator itr(o_pInfluenceWar->begin());
	for (; itr != o_pInfluenceWar->end(); itr++)
	{
		SDB_INFLUENCE_WAR_INFO tmWarInfo;
		tmWarInfo = *itr;

		// Bind Columns
		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmWarInfo.InfluenceType, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmWarInfo.WartimeStage, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.ReqContributionPoint, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmWarInfo.HPRepairRate, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmWarInfo.DPRepairRate, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmWarInfo.SPRepairRate, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.EventTime, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmWarInfo.EventExperience, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmWarInfo.EventDropSPI, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmWarInfo.EventDownExperience, 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmWarInfo.EventDropItem, 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmWarInfo.EventDropRareItem, 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SupplyItemNum, 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SupplyItemCount, 0, &arrCB[14]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmWarInfo.IsSummonJacoMonster, 0, &arrCB[15]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.BossMonsterUnitKind, 0, &arrCB[16]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.BossMonsterCount, 0, &arrCB[17]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.BossMonsterSummonTerm, 0, &arrCB[18]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.BossMonsterSummonMapIndex, 0, &arrCB[19]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.BossMonsterSummonPositionX, 0, &arrCB[20]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.BossMonsterSummonPositionY, 0, &arrCB[21]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 22, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.BossMonsterSummonPositionZ, 0, &arrCB[22]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 23, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonMonsterUnitKind[0], 0, &arrCB[23]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 24, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonCount[0], 0, &arrCB[24]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 25, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonMapIndex[0], 0, &arrCB[25]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 26, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionX[0], 0, &arrCB[26]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 27, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionY[0], 0, &arrCB[27]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 28, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionZ[0], 0, &arrCB[28]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 29, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonMonsterUnitKind[1], 0, &arrCB[29]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 30, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonCount[1], 0, &arrCB[30]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 31, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonMapIndex[1], 0, &arrCB[31]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 32, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionX[1], 0, &arrCB[32]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 33, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionY[1], 0, &arrCB[33]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 34, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionZ[1], 0, &arrCB[34]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 35, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonMonsterUnitKind[2], 0, &arrCB[35]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 36, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonCount[2], 0, &arrCB[36]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 37, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonMapIndex[2], 0, &arrCB[37]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 38, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionX[2], 0, &arrCB[38]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 39, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionY[2], 0, &arrCB[39]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 40, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionZ[2], 0, &arrCB[40]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 41, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonMonsterUnitKind[3], 0, &arrCB[41]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 42, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonCount[3], 0, &arrCB[42]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 43, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonMapIndex[3], 0, &arrCB[43]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 44, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionX[3], 0, &arrCB[44]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 45, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionY[3], 0, &arrCB[45]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 46, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionZ[3], 0, &arrCB[46]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 47, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonMonsterUnitKind[4], 0, &arrCB[47]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 48, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SummonCount[4], 0, &arrCB[48]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 49, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonMapIndex[4], 0, &arrCB[49]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 50, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionX[4], 0, &arrCB[50]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 51, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionY[4], 0, &arrCB[51]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 52, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmWarInfo.SummonPositionZ[4], 0, &arrCB[52]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 53, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SupplyItemNum2, 0, &arrCB[53]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 54, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.SupplyItemCount2, 0, &arrCB[54]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 55, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.WinBossMonsterUnitKind, 0, &arrCB[55]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 56, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmWarInfo.LossBossMonsterUnitKind, 0, &arrCB[56]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0361));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : InfluenceType(%d)   WartimeStage(%d)!!", tmWarInfo.InfluenceType, tmWarInfo.WartimeStage);
			MessageBox(NULL, err, "ti_InfluenceWar", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveOutPostNextWarTime(CODBCStatement *i_pODBCStmt, vectSOutPostInfo *o_pOutPostNextWarTime)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveOutPostNextWarTime(CODBCStatement *i_pODBCStmt, vectSOutPostInfo *o_pOutPostNextWarTime)
int CAtumDBHelper::SaveOutPostNextWarTime(CODBCStatement* i_pODBCStmt, vectSOutPostInfo* o_pOutPostNextWarTime, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[2];
	fill_n(arrCB, 2, SQL_NTS);

	//Delete ti_OutPostNextWarTime
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0385, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pOutPostNextWarTime->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_OutPostNextWarTime
	int nObjects = 0;
	vectSOutPostInfo::iterator itr(o_pOutPostNextWarTime->begin());
	for (; itr != o_pOutPostNextWarTime->end(); itr++)
	{
		SOUTPOST_INFO tmOutPost;
		tmOutPost = *itr;
		char szNextWarTime[SIZE_MAX_SQL_DATETIME_STRING];

		tmOutPost.OutPostNextWarTime.GetSQLDateTimeString(szNextWarTime, SIZE_MAX_SQL_DATETIME_STRING);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szNextWarTime, 0, &arrCB[1]);
		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0362));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : OutPostNextWarTime(%s)!!", szNextWarTime);
			MessageBox(NULL, err, "ti_OutPostNextWarTime", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveActionByLevel(CODBCStatement *i_pODBCStmt, vectSACTION_BY_LEVEL_DB *o_pActionByLevel)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveActionByLevel(CODBCStatement *i_pODBCStmt, vectSACTION_BY_LEVEL_DB *o_pActionByLevel)
int CAtumDBHelper::SaveActionByLevel(CODBCStatement* i_pODBCStmt, vectSACTION_BY_LEVEL_DB* o_pActionByLevel, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//Delete ti_ActionByLevel
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0386, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pActionByLevel->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_ActionByLevel
	int nObjects = 0;
	vectSACTION_BY_LEVEL_DB::iterator itr(o_pActionByLevel->begin());
	for (; itr != o_pActionByLevel->end(); itr++)
	{
		SACTION_BY_LEVEL_DB tmActionByLevel;
		tmActionByLevel = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmActionByLevel.Level, 0, &arrCB[1]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmActionByLevel.PollPoint, 0, &arrCB[2]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0363));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : Level(%d)   PollPoint(%d)!!", tmActionByLevel.Level, tmActionByLevel.PollPoint);
			MessageBox(NULL, err, "ti_ActionByLevel", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveMapInfo(CODBCStatement *i_pODBCStmt, vectorMAP_INFO *o_pMapInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveMapInfo(CODBCStatement *i_pODBCStmt, vectorMAP_INFO *o_pMapInfo)
int CAtumDBHelper::SaveMapInfo(CODBCStatement* i_pODBCStmt, vectorMAP_INFO* o_pMapInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[54]; // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	fill_n(arrCB, 54, SQL_NTS);

	//Delete ti_MapInfo
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0387, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pMapInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_MapInfo
	int nObjects = 0;
	vectorMAP_INFO::iterator itr(o_pMapInfo->begin());
	for (; itr != o_pMapInfo->end(); itr++)
	{
		MAP_INFO tmMapInfo;
		tmMapInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.MapIndex, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.RenderMapIndex, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.BeforeMapIndex, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_MAP_NAME, 0, tmMapInfo.MapName, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.MapInfluenceType, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapInfo.CityMapIndex, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapInfo.CityWarQuestIndex, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapInfo.CityWarQuestNPCIndex, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapInfo.CityWarQuestMonsterUID, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.MaxUserCount, 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.MaxGroundHeight, 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.MinGroundHeight, 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.WaterHeight, 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.UserVisibleDistance, 0, &arrCB[14]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.MonsterVisibleDistance, 0, &arrCB[15]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapInfo.QuestIndexForWarp, 0, &arrCB[16]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.FrontPositionDistance, 0, &arrCB[17]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapInfo.AutoRecoveryFlag, 0, &arrCB[18]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.DefaultMapIndex, 0, &arrCB[19]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.CityWarpTargetPosition.x, 0, &arrCB[20]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.CityWarpTargetPosition.y, 0, &arrCB[21]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 22, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.CityWarpTargetPosition.z, 0, &arrCB[22]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 23, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.ClientMaxAltitudeHeight, 0, &arrCB[23]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 24, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmMapInfo.DayFogColor, 0, &arrCB[24]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 25, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmMapInfo.NightFogColor, 0, &arrCB[25]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 26, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.DayFogStartDistance, 0, &arrCB[26]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 27, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.DayFogEndDistance, 0, &arrCB[27]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 28, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.NightFogStartDistance, 0, &arrCB[28]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 29, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.NightFogEndDistance, 0, &arrCB[29]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 30, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.DaySunDirection.x, 0, &arrCB[30]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 31, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.DaySunDirection.y, 0, &arrCB[31]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 32, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.DaySunDirection.z, 0, &arrCB[32]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 33, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.NightSunDirection.x, 0, &arrCB[33]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 34, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.NightSunDirection.y, 0, &arrCB[34]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 35, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMapInfo.NightSunDirection.z, 0, &arrCB[35]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 36, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapInfo.WaterType, 0, &arrCB[36]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 37, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapInfo.PKZoneFlag, 0, &arrCB[37]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 38, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapInfo.TileRenderingFlag, 0, &arrCB[38]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 39, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapInfo.SkyRenderingFlag, 0, &arrCB[39]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 40, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapInfo.SunRenderingFlag, 0, &arrCB[40]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 41, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapInfo.FogRenderingFlag, 0, &arrCB[41]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 42, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapInfo.VCNWarpObjectIndex, 0, &arrCB[42]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 43, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapInfo.ANIWarpObjectIndex, 0, &arrCB[43]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 44, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.Dat, 0, &arrCB[44]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 45, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.Map, 0, &arrCB[45]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 46, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.Tex, 0, &arrCB[46]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 47, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.Cloud, 0, &arrCB[47]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 48, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.Sky, 0, &arrCB[48]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 49, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.Nsky, 0, &arrCB[49]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 50, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.Bgm, 0, &arrCB[50]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 51, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapInfo.TeleportWarpObjectIndex, 0, &arrCB[51]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 52, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapInfo.BeforeMapIndex2, 0, &arrCB[52]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 53, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_MAP_DESCRIPTION, 0, tmMapInfo.MapDescription, 0, &arrCB[53]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0364));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MapIndex(%d)   MapName(%s)!!", tmMapInfo.MapIndex, tmMapInfo.MapName);
			MessageBox(NULL, err, "ti_MapInfo", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveBuildingNPC(CODBCStatement *i_pODBCStmt, vector<BUILDINGNPC> *o_pBuildingNPC)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveBuildingNPC(CODBCStatement *i_pODBCStmt, vector<BUILDINGNPC> *o_pBuildingNPC)
int CAtumDBHelper::SaveBuildingNPC(CODBCStatement* i_pODBCStmt, vector<BUILDINGNPC>* o_pBuildingNPC, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[10];
	fill_n(arrCB, 10, SQL_NTS);

	//Delete ti_BuildingNPC
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0388, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pBuildingNPC->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_BuildingNPC
	int nObjects = 0;
	vector<BUILDINGNPC>::iterator itr(o_pBuildingNPC->begin());
	for (; itr != o_pBuildingNPC->end(); itr++)
	{
		BUILDINGNPC tmBuildingNPC;
		tmBuildingNPC = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmBuildingNPC.BuildingIndex, 0, &arrCB[1]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BUILDING_NAME, 0, tmBuildingNPC.BuildingName, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmBuildingNPC.BuildingKind, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmBuildingNPC.MapIndex, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmBuildingNPC.NPCIndex, 0, &arrCB[5]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NPC_NAME, 0, tmBuildingNPC.NPCName, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BUILDING_NPC_TALK_STRING, 0, tmBuildingNPC.GreetingTalk, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmBuildingNPC.OwnerCityMapIndex, 0, &arrCB[8]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmBuildingNPC.OwnerOrderNumber, 0, &arrCB[9]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0365));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : BuildingIndex(%d)   BuildingName(%s)!!", tmBuildingNPC.BuildingIndex, tmBuildingNPC.BuildingName);
			MessageBox(NULL, err, "ti_BuildingNPC", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveMonster(CODBCStatement *i_pODBCStmt, vectMONSTER_DATA *o_pMonster)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveMonster(CODBCStatement *i_pODBCStmt, vectMONSTER_DATA *o_pMonster)
int CAtumDBHelper::SaveMonster(CODBCStatement* i_pODBCStmt, vectMONSTER_DATA* o_pMonster, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[59]; // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - WayPointPattern 필드 추가
	fill_n(arrCB, 59, SQL_NTS);

	//Delete ti_Monster
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0389, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pMonster->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_Monster
	int nObjects = 0;
	vectMONSTER_DATA::iterator itr(o_pMonster->begin());
	for (; itr != o_pMonster->end(); itr++)
	{
		MONSTER_DATA tmMonster;
		tmMonster = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterUnitKind, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_MONSTER_NAME, 0, tmMonster.MonsterName, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.Level, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &tmMonster.Experience, 0, &arrCB[4]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterHP, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmMonster.Race, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.Speed, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[0], 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[1], 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[2], 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[3], 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[4], 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[5], 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[6], 0, &arrCB[14]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[7], 0, &arrCB[15]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[8], 0, &arrCB[16]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[9], 0, &arrCB[17]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[10], 0, &arrCB[18]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[11], 0, &arrCB[19]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[12], 0, &arrCB[20]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[13], 0, &arrCB[21]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 22, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterItem[14], 0, &arrCB[22]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 23, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.ControlSkil, 0, &arrCB[23]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 24, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.SizeForServer, 0, &arrCB[24]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 25, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.SizeForClient, 0, &arrCB[25]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 26, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.Faction, 0, &arrCB[26]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 27, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMonster.MonsterForm, 0, &arrCB[27]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 28, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.AttackPattern, 0, &arrCB[28]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 29, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMonster.MovePattern, 0, &arrCB[29]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 30, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.Belligerence, 0, &arrCB[30]);

		// 2010-07-19 by dhjin, 확률 수식 변경 // 2009-12-17 by cmkwon, 데미지 계산식 관련한 필드(방어력,회피,피어스,확률)에 255이상 값을 설정 가능하게 수정 - 몬스터 방어, 방어확률
		SQLBindParameter(i_pODBCStmt->m_hstmt, 31, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmMonster.Defense, 0, &arrCB[31]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 32, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmMonster.DefenseProbability, 0, &arrCB[32]);

		SQLBindParameter(i_pODBCStmt->m_hstmt, 33, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.AlphaBlending, 0, &arrCB[33]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 34, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.AttackObject, 0, &arrCB[34]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 35, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMonster.AttackRange, 0, &arrCB[35]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 36, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmMonster.HPRecoveryTime, 0, &arrCB[36]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 37, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMonster.HPRecoveryValue, 0, &arrCB[37]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 38, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmMonster.RenderIndex, 0, &arrCB[38]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 39, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmMonster.ScaleValue, 0, &arrCB[39]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 40, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.TextureIndex, 0, &arrCB[40]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 41, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmMonster.SourceIndex, 0, &arrCB[41]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 42, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.QuickTurnAngle, 0, &arrCB[42]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 43, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.QuickSpeed, 0, &arrCB[43]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 44, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &tmMonster.QuickTurnTerm, 0, &arrCB[44]); // 2009-11-09 by cmkwon, 자료형 관련으로 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 45, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &tmMonster.MPOption, 0, &arrCB[45]); // 2010-01-11 by cmkwon, 몬스터 MPOption 64bit로 변경 - 기존(SQL_C_UTINYINT, SQL_TINYINT)
		SQLBindParameter(i_pODBCStmt->m_hstmt, 46, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.MPOptionParam1, 0, &arrCB[46]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 47, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.MPOptionParam2, 0, &arrCB[47]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 48, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmMonster.Range, 0, &arrCB[48]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 49, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmMonster.TurnAngle, 0, &arrCB[49]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 50, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.MonsterHabit, 0, &arrCB[50]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 51, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.ClickEvent, 0, &arrCB[51]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 52, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmMonster.HPActionIdx, 0, &arrCB[52]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 53, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterTarget, 0, &arrCB[53]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 54, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMonster.ChangeTarget, 0, &arrCB[54]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 55, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmMonster.MonsterTarget2, 0, &arrCB[55]); // 2010-03-31 by dhjin, 인피니티(기지방어) -
		SQLBindParameter(i_pODBCStmt->m_hstmt, 56, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_FILE_NAME, 0, tmMonster.PortraitFileName, 0, &arrCB[56]); // 2010-03-31 by dhjin, 인피니티(기지방어) -
		SQLBindParameter(i_pODBCStmt->m_hstmt, 57, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.ChangeTargetTime, 0, &arrCB[57]); // 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - ChangeTargetTime 필드 추가
		SQLBindParameter(i_pODBCStmt->m_hstmt, 58, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMonster.WayPointPattern, 0, &arrCB[58]); // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - WayPointPattern 필드 추가

		/*[Stored Query Definition]************************************************
		--------------------------------------------------------------
		-- ti_Monster 데이터 생성 관련
		--------------------------------------------------------------
		-- !!!!
		-- Name:
		-- Desc: // 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 -
		--			// 2010-01-11 by cmkwon, 몬스터 MPOption 64bit로 변경 - 
		--			// 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - ChangeTargetTime 필드 추가
		--			// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - WayPointPattern 필드 추가
		-- ====
		CALL dbo.atum_import_ti_monster
		**************************************************************************/
		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0366));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : MonsterUnitKind(%d)   MonsterName(%s)!!", tmMonster.MonsterUnitKind, tmMonster.MonsterName);
			MessageBox(NULL, err, "ti_Monster", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveTutorialInfo(CODBCStatement *i_pODBCStmt, vectSTutorialInfo *o_pTutorialInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveTutorialInfo(CODBCStatement *i_pODBCStmt, vectSTutorialInfo *o_pTutorialInfo)
int CAtumDBHelper::SaveTutorialInfo(CODBCStatement* i_pODBCStmt, vectSTutorialInfo* o_pTutorialInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[7];
	fill_n(arrCB, 7, SQL_NTS);

	//Delete ti_TutorialInfo
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0390, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pTutorialInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_TutorialInfo
	int nObjects = 0;
	vectSTutorialInfo::iterator itr(o_pTutorialInfo->begin());
	for (; itr != o_pTutorialInfo->end(); itr++)
	{
		STUTORIAL_INFO tmTutorial;
		tmTutorial = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTutorial.TutorialUID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTutorial.TutorialPayItem1, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTutorial.TutorialPayItemCount1, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTutorial.TutorialPayItem2, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTutorial.TutorialPayItemCount2, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTutorial.TutorialPayItem3, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmTutorial.TutorialPayItemCount3, 0, &arrCB[7]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0367));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TutorialUID(%d)!!", tmTutorial.TutorialUID);
			MessageBox(NULL, err, "ti_TutorialInfo", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveMapObject(CODBCStatement *i_pODBCStmt, vectMAPOBJECT *o_pMapItemInfo)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveMapObject(CODBCStatement *i_pODBCStmt, vectMAPOBJECT *o_pMapItemInfo)
int CAtumDBHelper::SaveMapObject(CODBCStatement* i_pODBCStmt, vectMAPOBJECT* o_pMapItemInfo, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[11];
	fill_n(arrCB, 11, SQL_NTS);

	//Delete ti_MapObject
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0391, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pMapItemInfo->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_MapObject
	int nObjects = 0;
	vectMAPOBJECT::iterator itr(o_pMapItemInfo->begin());
	for (; itr != o_pMapItemInfo->end(); itr++)
	{
		MAPOBJECT tmMapObjectInfo;
		tmMapObjectInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_OBJECT_NAME, 0, tmMapObjectInfo.ObjectName, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmMapObjectInfo.Code, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapObjectInfo.Alpha, 0, &arrCB[3]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapObjectInfo.RadiusForServer, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &tmMapObjectInfo.RadiusForClient, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapObjectInfo.Collision, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapObjectInfo.CollisionForServer, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmMapObjectInfo.RenderIndex, 0, &arrCB[8]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmMapObjectInfo.ObjectRenderType, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_OBJECT_DESCRIPTION, 0, tmMapObjectInfo.ObjectDescription, 0, &arrCB[10]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0368));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : Code(%d)!!", tmMapObjectInfo.Code);
			MessageBox(NULL, err, "ti_MapObject", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAtumDBHelper::SaveLuckyMachine(CODBCStatement *i_pODBCStmt, vectLUCKY_MACHINE *o_pLuckyMachine)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveLuckyMachine(CODBCStatement *i_pODBCStmt, vectLUCKY_MACHINE *o_pLuckyMachine)
int CAtumDBHelper::SaveLuckyMachine(CODBCStatement* i_pODBCStmt, vectLUCKY_MACHINE* o_pLuckyMachine, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[11];
	fill_n(arrCB, 11, SQL_NTS);

	//Delete ti_LuckyMachine
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0392, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pLuckyMachine->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_LuckyMachine
	int nObjects = 0;
	vectLUCKY_MACHINE::iterator itr(o_pLuckyMachine->begin());
	for (; itr != o_pLuckyMachine->end(); itr++)
	{
		LUCKY_MACHINE tmLuckyMachine;
		tmLuckyMachine = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmLuckyMachine.order, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmLuckyMachine.BuildingInx, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmLuckyMachine.MachineKind, 0, &arrCB[3]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmLuckyMachine.MachineNum, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmLuckyMachine.CoinItemNum, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmLuckyMachine.SlotNum, 0, &arrCB[6]); // 2009-09-23 by cmkwon, Excel to DB 관련 자료형 버그 수정 - 
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmLuckyMachine.SlotProbability, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmLuckyMachine.MysteryItemDropNum, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmLuckyMachine.SourceIndex, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_LUCKYMACHINE_DESCRIPTION, 0, tmLuckyMachine.szDescription, 0, &arrCB[10]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0369));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : order(%d)   BuildingInx(%d)!!", tmLuckyMachine.order, tmLuckyMachine.BuildingInx);
			MessageBox(NULL, err, "ti_LuckyMachine", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveQuestMatching(CODBCStatement *i_pODBCStmt, vectSQUEST_MATCHING *o_pQuestMatching)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveQuestMatching(CODBCStatement *i_pODBCStmt, vectSQUEST_MATCHING *o_pQuestMatching)
int CAtumDBHelper::SaveQuestMatching(CODBCStatement* i_pODBCStmt, vectSQUEST_MATCHING* o_pQuestMatching, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//Delete ti_QuestMatching
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0393, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pQuestMatching->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_QuestMatching
	int nObjects = 0;
	vectSQUEST_MATCHING::iterator itr(o_pQuestMatching->begin());
	for (; itr != o_pQuestMatching->end(); itr++)
	{
		SQUEST_MATCHING tmQuestMatching;
		tmQuestMatching = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmQuestMatching.QuestIndexForBCU, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmQuestMatching.QuestIndexForANI, 0, &arrCB[2]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0370));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : QuestIndexForBCU(%d)   QuestIndexForANI(%d)!!", tmQuestMatching.QuestIndexForBCU, tmQuestMatching.QuestIndexForANI);
			MessageBox(NULL, err, "ti_QuestMatching", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveItemMatching(CODBCStatement *i_pODBCStmt, vectSITEM_MATCHING *o_pItemMatching)
/// \brief		// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 - 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveItemMatching(CODBCStatement *i_pODBCStmt, vectSITEM_MATCHING *o_pItemMatching)
int CAtumDBHelper::SaveItemMatching(CODBCStatement* i_pODBCStmt, vectSITEM_MATCHING* o_pItemMatching, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//Delete ti_ItemMatching
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090709_0394, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pItemMatching->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_ItemMatching
	int nObjects = 0;
	vectSITEM_MATCHING::iterator itr(o_pItemMatching->begin());
	for (; itr != o_pItemMatching->end(); itr++)
	{
		SITEM_MATCHING tmItemMatching;
		tmItemMatching = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMatching.ItemNumForBCU, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmItemMatching.ItemNumForANI, 0, &arrCB[2]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090709_0371));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : ItemNumForBCU(%d)   ItemNumForANI(%d)!!", tmItemMatching.ItemNumForBCU, tmItemMatching.ItemNumForANI);
			MessageBox(NULL, err, "ti_ItemMatching", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
/// \author		jwlee
/// \date		2009-07-09 ~ 2009-07-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 10. 27 by jskim 진행률 확인
//int CAtumDBHelper::SaveInfluenceRate(CODBCStatement *i_pODBCStmt, vectSTI_INFLUENCERATE *o_pInfluenceRate)
int CAtumDBHelper::SaveInfluenceRate(CODBCStatement* i_pODBCStmt, vectSTI_INFLUENCERATE* o_pInfluenceRate, HWND hWnd)
//end 2009. 10. 27 by jskim 진행률 확인
{
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//Delete ti_InfluenceRate
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090922_0405, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		// error
		return -1;
	}
	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(o_pInfluenceRate->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인
	//Insert ti_InfluenceRate
	int nObjects = 0;
	vectSTI_INFLUENCERATE::iterator itr(o_pInfluenceRate->begin());
	for (; itr != o_pInfluenceRate->end(); itr++)
	{
		STI_INFLUENCERATE tmInfluenceRate;
		tmInfluenceRate = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfluenceRate.StartLevel, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfluenceRate.EndLevel, 0, &arrCB[2]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090922_0404));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : StartLevel(%d)   EndLevel(%d)!!", tmInfluenceRate.StartLevel, tmInfluenceRate.EndLevel);
			MessageBox(NULL, err, "ti_InfluenceRate", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
int CAtumDBHelper::SaveHPAction(CODBCStatement* i_pODBCStmt, vectHPAction* i_pHPActionList, HWND hWnd)
{
	SQLINTEGER arrCB[15];
	fill_n(arrCB, 15, SQL_NTS);

	//Delete ti_HPAction
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0514, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pHPActionList->size() - 1));

	//Insert ti_HPAction
	int nObjects = 0;
	vectHPAction::iterator itr(i_pHPActionList->begin());
	for (; itr != i_pHPActionList->end(); itr++)
	{
		HPACTION tmHPAction;
		tmHPAction = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmHPAction.HPActionUID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmHPAction.HPActionNum, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmHPAction.HPMaxValueRate, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmHPAction.HPMinValueRate, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmHPAction.UseItemArrayIdx, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmHPAction.NextUseItemArrayIdx, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmHPAction.HitRate, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmHPAction.UseCount, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmHPAction.HPTalkCondition, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmHPAction.HPTalkImportance, 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmHPAction.PreHPCameraTremble, 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_HPTALK_DESCRIPTION, 0, &tmHPAction.PreHPTalk, 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmHPAction.HPCameraTremble, 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_HPTALK_DESCRIPTION, 0, &tmHPAction.HPTalk, 0, &arrCB[14]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0515));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : HPActionUID(%d)  !!", tmHPAction.HPActionUID);
			MessageBox(NULL, err, "ti_HPAction", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

int CAtumDBHelper::SaveCinema(CODBCStatement* i_pODBCStmt, vectorCinemaInfo* i_pCinemaInfoList, HWND hWnd)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_import_ti_Cinema
	-- DESC				: -- // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
	--					  -- // 2010-03-29 by cmkwon, 인피2차 시네마에 효과음 파일 설정 추가 - SoundFileName 필드 추가
						  -- // 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제 기능 추가
	--------------------------------------------------------------------------------
	CALL dbo.atum_import_ti_Cinema
	************************************************************************/
	SQLINTEGER arrCB[28]; // 2010-03-29 by cmkwon, 인피2차 시네마에 효과음 파일 설정 추가 - // 2010-03-31 by dhjin, 인피니티(기지방어) - // 2011-03-09 by hskim, 시네마 퀘스트 인덱스 설정 추가 - // 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가
	fill_n(arrCB, 28, SQL_NTS);

	//Delete ti_Cinema
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0516, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pCinemaInfoList->size() - 1));

	//Insert ti_Cinema
	int nObjects = 0;
	vectorCinemaInfo::iterator itr(i_pCinemaInfoList->begin());
	for (; itr != i_pCinemaInfoList->end(); itr++)
	{
		CINEMAINFO tmCinemaInfo;
		tmCinemaInfo = *itr;

		// 2010-03-31 by dhjin, 인피니티(기지방어) -
		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.CinemaNum, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.CinemaOrder, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.StartTime, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.EffectIdx, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmCinemaInfo.EffectPosition_X, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmCinemaInfo.EffectPosition_Y, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &tmCinemaInfo.EffectPosition_Z, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.EffectPlayTime, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.EffectPlayCount, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmCinemaInfo.ObjectIdx, 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.ChangeObjectIdx, 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.ChangeTime, 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.CameraTremble, 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmCinemaInfo.KeyMonster, 0, &arrCB[14]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.MonsterIdx, 0, &arrCB[15]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.SummonCount, 0, &arrCB[16]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.SummonRandomPos, 0, &arrCB[17]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.ObjectBodyConditionIdx, 0, &arrCB[18]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &tmCinemaInfo.BodyCondition, 0, &arrCB[19]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_FILE_NAME, 0, tmCinemaInfo.TipFileName, 0, &arrCB[20]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_FILE_NAME, 0, tmCinemaInfo.SkyFileName, 0, &arrCB[21]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 22, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CINEMATALK_DESCRIPTION, 0, tmCinemaInfo.CinemaTalk, 0, &arrCB[22]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 23, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_FILE_NAME, 0, tmCinemaInfo.SoundFileName, 0, &arrCB[23]); // 2010-03-29 by cmkwon, 인피2차 시네마에 효과음 파일 설정 추가 - 
		// start 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가
		SQLBindParameter(i_pODBCStmt->m_hstmt, 24, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.QuestIndex, 0, &arrCB[24]); // 2011-03-09 by hskim, 시네마 퀘스트 인덱스 설정 추가 -
		SQLBindParameter(i_pODBCStmt->m_hstmt, 25, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.MonsterIdxType, 0, &arrCB[25]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 26, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.CustomIdx, 0, &arrCB[26]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 27, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmCinemaInfo.UpdateMonsterIdx, 0, &arrCB[27]);
		// end 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가
		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0517));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : CinemaNum(%d)  EffectStartTime(%d) !!", tmCinemaInfo.CinemaNum, tmCinemaInfo.StartTime);
			MessageBox(NULL, err, "ti_Cinema", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

int CAtumDBHelper::SaveRevision(CODBCStatement* i_pODBCStmt, vectorRevisionInfo* i_pRevisionInfoList, HWND hWnd)
{
	SQLINTEGER arrCB[8];
	fill_n(arrCB, 8, SQL_NTS);

	//Delete ti_Revision
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0520, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pRevisionInfoList->size() - 1));

	//Insert ti_Revision
	int nObjects = 0;
	vectorRevisionInfo::iterator itr(i_pRevisionInfoList->begin());
	for (; itr != i_pRevisionInfoList->end(); itr++)
	{
		REVISIONINFO tmRevisionInfo;
		tmRevisionInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmRevisionInfo.RevisionNum, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmRevisionInfo.UnitKind, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmRevisionInfo.RevisionLevel, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmRevisionInfo.RevisionHP, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &tmRevisionInfo.RevisionDP, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmRevisionInfo.RevisionStandardWeapon, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmRevisionInfo.RevisionAdvancedWeapon, 0, &arrCB[7]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0521));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : RevisionNum(%d)  UnitKind(%d) !!", tmRevisionInfo.RevisionNum, tmRevisionInfo.UnitKind);
			MessageBox(NULL, err, "ti_Revision", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

int CAtumDBHelper::SaveInfinityMonster(CODBCStatement* i_pODBCStmt, vectorInfinityMonsterInfo* i_pInfinityMonsterInfoList, HWND hWnd)
{
	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);

	//Delete ti_InfinityMonster
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0518, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pInfinityMonsterInfoList->size() - 1));

	//Insert ti_InfinityMonster
	int nObjects = 0;
	vectorInfinityMonsterInfo::iterator itr(i_pInfinityMonsterInfoList->begin());
	for (; itr != i_pInfinityMonsterInfoList->end(); itr++)
	{
		INFINITY_MONSTERINFO tmInfinityMonsterInfo;
		tmInfinityMonsterInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityMonsterInfo.InfinityMonsterUID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityMonsterInfo.InfinityMonsterIdx, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityMonsterInfo.MonsterIdx, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityMonsterInfo.TargetType, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityMonsterInfo.TargetCount, 0, &arrCB[5]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0519));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : InfinityMonsterUID(%d) !!", tmInfinityMonsterInfo.InfinityMonsterUID);
			MessageBox(NULL, err, "ti_InfinityMonster", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

int CAtumDBHelper::SaveInfinityMode(CODBCStatement* i_pODBCStmt, vectorInfinityModeInfo* i_pInfinityModeInfoList, HWND hWnd)
{
	SQLINTEGER arrCB[16]; // 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해) // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	fill_n(arrCB, 16, SQL_NTS);

	//Delete ti_InfinityMode
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0522, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pInfinityModeInfoList->size() - 1));

	//Insert ti_InfinityMode
	int nObjects = 0;
	vectorInfinityModeInfo::iterator itr(i_pInfinityModeInfoList->begin());
	for (; itr != i_pInfinityModeInfoList->end(); itr++)
	{
		INFINITY_MODEINFO tmInfinityModeInfo;
		tmInfinityModeInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityModeInfo.InfinityModeUID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityModeInfo.ModeTypeNum, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityModeInfo.MapIdx, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityModeInfo.InfinityMonsterIdx, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityModeInfo.CinemaNum, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityModeInfo.RevisionNum, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityModeInfo.ResetCycle, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityModeInfo.EntranceCount, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityModeInfo.LimitTime, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityModeInfo.MinLv, 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityModeInfo.MaxLv, 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityModeInfo.MinAdmissionCount, 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmInfinityModeInfo.MaxAdmissionCount, 0, &arrCB[13]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmInfinityModeInfo.TimePenaltyValue, 0, &arrCB[14]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
		SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmInfinityModeInfo.HPPenaltyValue, 0, &arrCB[15]); // 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0523));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : InfinityModeUID(%d) !!", tmInfinityModeInfo.InfinityModeUID);
			MessageBox(NULL, err, "ti_InfinityMode", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

int CAtumDBHelper::SaveTender(CODBCStatement* i_pODBCStmt, vectorTenderInfo* i_pTenderInfoList, HWND hWnd)
{
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	//Delete ti_InfinityMonster
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0525, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pTenderInfoList->size() - 1));

	//Insert ti_InfinityMonster
	int nObjects = 0;
	vectorTenderInfo::iterator itr(i_pTenderInfoList->begin());
	for (; itr != i_pTenderInfoList->end(); itr++)
	{
		TENDER_INFO tmTenderInfo;
		tmTenderInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmTenderInfo.TenderUID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmTenderInfo.TenderItemNum, 0, &arrCB[2]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0526));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : TenderUID(%d) !!", tmTenderInfo.TenderUID);
			MessageBox(NULL, err, "ti_Tender", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
int CAtumDBHelper::SaveInfinityShop(CODBCStatement* i_pODBCStmt, vectorInfinityShopInfo* i_pInfinityShopInfoList, HWND hWnd)
{
	SQLINTEGER arrCB[14];
	fill_n(arrCB, 14, SQL_NTS);

	//Delete ti_InfinityShop
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_090909_0528, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pInfinityShopInfoList->size() - 1));

	//Insert ti_InfinityShop
	int nObjects = 0;
	vectorInfinityShopInfo::iterator itr(i_pInfinityShopInfoList->begin());
	for (; itr != i_pInfinityShopInfoList->end(); itr++)
	{
		INFINITY_SHOP_INFO tmInfinityShopInfo;
		tmInfinityShopInfo = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.InfinityShopUID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.BuyItemNum, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.BuyItemCount, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemNum1, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemCount1, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemNum2, 0, &arrCB[6]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemCount2, 0, &arrCB[7]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemNum3, 0, &arrCB[8]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemCount3, 0, &arrCB[9]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemNum4, 0, &arrCB[10]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemCount4, 0, &arrCB[11]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemNum5, 0, &arrCB[12]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmInfinityShopInfo.TradeItemCount5, 0, &arrCB[13]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0529));
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : InfinityShopUID(%d) !!", tmInfinityShopInfo.InfinityShopUID);
			MessageBox(NULL, err, "ti_InfinityShop", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-01-18 ~ 2010-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBHelper::LoadItemParamOverlap(CODBCStatement* i_pODBCStmt, vectSITEM_PARAM_OVERLAP* o_ItemParamOverList)
{
	o_ItemParamOverList->clear();

	SITEM_PARAM_OVERLAP tmParamOverlap;
	util::zero(&tmParamOverlap,sizeof(tmParamOverlap));

	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	// Bind Columns
	SQLBindCol(i_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &tmParamOverlap.ItemNum, 0, &arrCB[1]);
	SQLBindCol(i_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &tmParamOverlap.OverlapIndex, 0, &arrCB[2]);
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100118_0540);
	if (!bRet)
	{
		// error
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumDBHelper::LoadItemParamOverlap# error !! \r\n");
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	while (TRUE)
	{
		RETCODE ret = SQLFetch(i_pODBCStmt->m_hstmt);
		if (ret == SQL_NO_DATA)
		{
			break;
		}
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			// error
			g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumDBHelper::LoadItemParamOverlap# error !! ret(%d) \r\n", ret);
			break;
		}

		o_ItemParamOverList->push_back(tmParamOverlap);
		util::zero(&tmParamOverlap,sizeof(tmParamOverlap));
	}
	i_pODBCStmt->FreeStatement();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-01-18 ~ 2010-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveItemParamOverlap(CODBCStatement* i_pODBCStmt, vectSITEM_PARAM_OVERLAP* i_pItemParamOverlapList, HWND hWnd)
{
	SQLINTEGER arrCB[3];
	fill_n(arrCB, 3, SQL_NTS);

	// Delete All ti_ItemParamOverlap
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100118_0542, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{// error

		return -1;
	}

	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pItemParamOverlapList->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인

	//Insert ti_Item
	int nObjects = 0;
	vectSITEM_PARAM_OVERLAP::iterator itr(i_pItemParamOverlapList->begin());
	for (; itr != i_pItemParamOverlapList->end(); itr++)
	{
		SITEM_PARAM_OVERLAP* pOverlap = &*itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pOverlap->ItemNum, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pOverlap->OverlapIndex, 0, &arrCB[2]);
		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100118_0541);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			//error
			char err[1024];
			sprintf(err, "ERROR ti_ItemParamOverlap line : ItemNum(%d) OverlapIndex(%d)!!", pOverlap->ItemNum, pOverlap->OverlapIndex);
			MessageBox(NULL, err, "ti_ItemParamOverlap", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveInvokingWearItemDPNum(CODBCStatement *i_pODBCStmt, InvokingWearItemDestParamList *i_pInvokingItemDPList, HWND hWnd)
/// \brief		인피니티 - 발동류장착아이템
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveInvokingWearItemDPNum(CODBCStatement* i_pODBCStmt, InvokingWearItemDestParamList* i_pInvokingItemDPList, HWND hWnd)
{
	SQLINTEGER arrCB[5];
	fill_n(arrCB, 5, SQL_NTS);

	// Delete All ti_InvokingWearItemDPNum
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100210_0550, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{// error

		return -1;
	}

	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pInvokingItemDPList->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인

	//Insert ti_InvokingWearItemDPNum
	int nObjects = 0;
	InvokingWearItemDestParamList::iterator itr(i_pInvokingItemDPList->begin());
	for (; itr != i_pInvokingItemDPList->end(); itr++)
	{
		INVOKING_WEAR_ITEM_DESTPARAM* pInvokingItemDP = &*itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInvokingItemDP->InvokingDestParamID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pInvokingItemDP->InvokingDestParam, 0, &arrCB[2]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pInvokingItemDP->InvokingDestParamValue, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInvokingItemDP->InvokingEffectIdx, 0, &arrCB[4]);
		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100210_0551);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			//error
			char err[1024];
			sprintf(err, "ERROR ti_InvokingWearItemDPNum line : InvokingDestParamID(%d) InvokingDestParam(%d)!!", pInvokingItemDP->InvokingDestParamID, pInvokingItemDP->InvokingDestParam);
			MessageBox(NULL, err, "ti_InvokingWearItemDPNum", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveInvokingWearItemDPNumByUse(CODBCStatement *i_pODBCStmt, InvokingWearItemDestParamList *i_pInvokingItemDPList, HWND hWnd)
/// \brief		인피니티 - 발동류장착아이템
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveInvokingWearItemDPNumByUse(CODBCStatement* i_pODBCStmt, InvokingWearItemDestParamList* i_pInvokingItemDPList, HWND hWnd)
{
	SQLINTEGER arrCB[5];
	fill_n(arrCB, 5, SQL_NTS);

	// Delete All ti_InvokingWearItemDPNumByUse
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100210_0553, SQL_NTS);
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{// error

		return -1;
	}

	// 2009. 10. 27 by jskim 진행률 확인
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pInvokingItemDPList->size() - 1));
	//end 2009. 10. 27 by jskim 진행률 확인

	//Insert ti_InvokingWearItemDPNum
	int nObjects = 0;
	InvokingWearItemDestParamList::iterator itr(i_pInvokingItemDPList->begin());
	for (; itr != i_pInvokingItemDPList->end(); itr++)
	{
		INVOKING_WEAR_ITEM_DESTPARAM* pInvokingItemDP = &*itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInvokingItemDP->InvokingDestParamID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pInvokingItemDP->InvokingDestParam, 0, &arrCB[2]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pInvokingItemDP->InvokingDestParamValue, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pInvokingItemDP->InvokingEffectIdx, 0, &arrCB[4]);
		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100210_0554);
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			//error
			char err[1024];
			sprintf(err, "ERROR ti_InvokingWearItemDPNumByUse line : InvokingDestParamID(%d) InvokingDestParam(%d)!!", pInvokingItemDP->InvokingDestParamID, pInvokingItemDP->InvokingDestParam);
			MessageBox(NULL, err, "ti_InvokingWearItemDPNumByUse", MB_OK);
			return -3;
		}
		nObjects++;
		// 2009. 10. 27 by jskim 진행률 확인
		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
		//end 2009. 10. 27 by jskim 진행률 확인
	}
	return nObjects;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveLevelAdjustmentList(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyMonsterListInfo *InvokingItemDPByUseList, HWND hWnd);
/// \brief		// 2010-05-24 by shcho 인피니티 난이도 조정 - 인피니티 난이도 조정에 관한 툴 업데이트
/// \author		shcho
/// \date		2010-05-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveLevelAdjustmentList(CODBCStatement* i_pODBCStmt, vectorInfinity_DifficultyInfo_Monster* i_Infinity_DifficultyInfo, HWND hWnd)
{
	SQLINTEGER arrCB[7];
	fill_n(arrCB, 7, SQL_NTS);
	//Delete ti_LevelAdjustment 구현
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100524_0563, SQL_NTS); //삭제(delete) 프로시저를 추가해야 한다.
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_Infinity_DifficultyInfo->size() - 1));

	//Insert ti_LevelAdjustment 구현
	int nObjects = 0;

	INFINITY_DIFFICULTY_MONSTER_SETTING_INFO temp_Info;
	vectorInfinity_DifficultyInfo_Monster::iterator itr(i_Infinity_DifficultyInfo->begin());

	for (; itr != i_Infinity_DifficultyInfo->end(); itr++)
	{
		temp_Info = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iIncreaseStep, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iMonsterAttackItem, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iMonsterDefense, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iMonsterEvasion, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iMonsterHP, 0, &arrCB[5]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iMonsterPopulation, 0, &arrCB[6]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100524_0564); //삽입(insert) 프로시저 구현 해야함
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : IncreaseStep(%d) !!", temp_Info.iIncreaseStep);
			MessageBox(NULL, err, "ti_LevelAdjustment", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDBHelper::SaveLevelAdjustmentRewardList(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Bonus  *i_Infinity_DifficultyInfo, HWND hWnd)
/// \brief		// 2010-05-24 by shcho 인피니티 난이도 조정 - 인피니티 난이도 조정에 관한 툴 업데이트
/// \author		shcho
/// \date		2010-05-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SaveLevelAdjustmentRewardList(CODBCStatement* i_pODBCStmt, vectorInfinity_DifficultyInfo_Bonus* i_Infinity_DifficultyInfo, HWND hWnd)
{
	SQLINTEGER arrCB[5];
	fill_n(arrCB, 5, SQL_NTS);
	//Delete ti_LevelAdjustment 구현
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100524_0565, SQL_NTS); //삭제(delete) 프로시저를 추가해야 한다.
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_Infinity_DifficultyInfo->size() - 1));

	//Insert ti_LevelAdjustment 구현
	int nObjects = 0;

	INFINITY_DIFFICULTY_BONUS_INFO temp_Info;
	vectorInfinity_DifficultyInfo_Bonus::iterator itr(i_Infinity_DifficultyInfo->begin());

	for (; itr != i_Infinity_DifficultyInfo->end(); itr++)
	{
		temp_Info = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iIncreaseStep, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iExp, 0, &arrCB[2]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iDrop_Item_Persent, 0, &arrCB[3]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.iItem_EA_persent, 0, &arrCB[4]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100524_0566); //삽입(insert) 프로시저 구현 해야함
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : IncreaseStep(%d) !!", temp_Info.iIncreaseStep);
			MessageBox(NULL, err, "ti_LevelAdjustment", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

// 2010-08-05 by dhjin, 버닝맵 - 
int CAtumDBHelper::SaveBurningMapList(CODBCStatement* i_pODBCStmt, BurningMapInfoList* i_pBurningMapList, HWND hWnd)
{
	SQLINTEGER arrCB[6];
	fill_n(arrCB, 6, SQL_NTS);
	//Delete ti_BurningMap 구현
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100805_0567, SQL_NTS); //삭제(delete) 프로시저를 추가해야 한다.
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_pBurningMapList->size() - 1));

	//Insert ti_BurningMap 구현
	int nObjects = 0;

	BURNING_MAP_INFO temp_Info;
	BurningMapInfoList::iterator itr(i_pBurningMapList->begin());

	for (; itr != i_pBurningMapList->end(); itr++)
	{
		temp_Info = *itr;

		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.BurningMapUID, 0, &arrCB[1]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &temp_Info.MapIndex, 0, &arrCB[2]); //2010-09-07 by shcho, 버닝 맵 - 자료형 수정
		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &temp_Info.ReqUnitKind, 0, &arrCB[3]); //2010-09-07 by shcho, 버닝 맵 - 자료형 수정
		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &temp_Info.ReqMinLv, 0, &arrCB[4]);
		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &temp_Info.ReqMaxLv, 0, &arrCB[5]);

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100805_0568); //삽입(insert) 프로시저 구현 해야함
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : IncreaseStep(%d) !!", temp_Info.BurningMapUID);
			MessageBox(NULL, err, "ti_BurningMap", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
/// \author		shcho&hslee
/// \date		2010-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

int CAtumDBHelper::SavePetBaseData(CODBCStatement* i_pODBCStmt, vectorPetBaseData* i_BaseData, HWND hWnd)
{
	SQLINTEGER arrCB[7];
	fill_n(arrCB, 7, SQL_NTS);
	//Delete
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0575, SQL_NTS); //삭제(delete) 프로시저를 추가해야 한다.
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_BaseData->size() - 1));

	//Insert ti_LevelAdjustment 구현
	int nObjects = 0;

	tPET_BASEDATA temp_Info;
	vectorPetBaseData::iterator itr(i_BaseData->begin());

	for (; itr != i_BaseData->end(); itr++)
	{
		temp_Info = *itr;

		int ret = 0;

		///////////////////////////////////////////////////////////////////////////////////////
		// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		//
		// 기존 
		//
		// 		ret = SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.UniqueNumber,					0, &arrCB[0]);
		// 		ret = SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.PetIndex,						0, &arrCB[1]);
		// 		ret = SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.PetKind,							0, &arrCB[2]);
		// 		ret = SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PET_NAME, 0, &temp_Info.szPetName,		0, &arrCB[3]);
		// 		ret = SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.EnableName,						0, &arrCB[4]);
		// 		ret = SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.EnableLevel,						0, &arrCB[5]);

		/////////
		// 수정

		int iCBCount = 0;

		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.PetIndex, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PET_NAME, 0, &temp_Info.PetName, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.PetKind, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_BIT, SQL_SMALLINT, 0, 0, &temp_Info.EnableName, 0, &arrCB[iCBCount]);
		iCBCount++;

		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_BIT, SQL_SMALLINT, 0, 0, &temp_Info.EnableLevel, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG, SQL_SMALLINT, 0, 0, &temp_Info.MaxLevel, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG, SQL_TINYINT, 0, 0, &temp_Info.BaseSocketCount, 0, &arrCB[iCBCount]);
		iCBCount++;
		// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		///////////////////////////////////////////////////////////////////////////////////////	

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0568); //삽입(insert) 프로시저
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : PetIndex(%d) !!", temp_Info.PetIndex); // start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
			MessageBox(NULL, err, "ti_Pet", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
/// \author		shcho&hslee
/// \date		2010-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDBHelper::SavePetLevelData(CODBCStatement* i_pODBCStmt, vectorPetLevelData* i_PetLevelData, HWND hWnd)
{
	SQLINTEGER arrCB[9];
	fill_n(arrCB, 9, SQL_NTS);
	//Delete 
	BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0576, SQL_NTS); //삭제(delete) 프로시저를 추가해야 한다.
	i_pODBCStmt->FreeStatement();
	if (!bRet)
	{
		return -1;
	}
	SendMessage(hWnd, WM_PROGRESSBAR_RANGE, 0, (LPARAM)(i_PetLevelData->size() - 1));

	//Insert 
	int nObjects = 0;

	tPET_LEVEL_DATA temp_Info;
	vectorPetLevelData::iterator itr(i_PetLevelData->begin());

	for (; itr != i_PetLevelData->end(); itr++)
	{
		temp_Info = *itr;

		///////////////////////////////////////////////////////////////////////////////////////
		// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		//
		// 기존 
		//
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,  0, 0, &temp_Info.UniqueNumber,			0, &arrCB[0]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,  0, 0, &temp_Info.Level,				0, &arrCB[1]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE,	SQL_DOUBLE,  0, 0, &temp_Info.NeedExp,				0, &arrCB[2]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_DOUBLE,	SQL_DOUBLE,  0, 0, &temp_Info.Stamina,				0, &arrCB[3]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,  0, 0, &temp_Info.CitySourceIndex,		0, &arrCB[4]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,  0, 0, &temp_Info.FieldSourceIndex,		0, &arrCB[5]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,  0, 0, &temp_Info.PetSkillItemIndex[0],	0, &arrCB[6]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,  0, 0, &temp_Info.PetSkillItemIndex[1],	0, &arrCB[7]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER,	 0, 0, &temp_Info.PetSkillItemIndex[2],	0, &arrCB[8]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER, 0, 0, &temp_Info.PetSkillItemIndex[3],	0, &arrCB[9]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER, 0, 0, &temp_Info.PetSkillItemIndex[4],	0, &arrCB[10]);
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER, 0, 0, &temp_Info.PetSkillItemIndex[5],	0, &arrCB[11]);
		// 		// 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리 총알 인덱스 필드 추가
		// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG,		SQL_INTEGER, 0, 0, &temp_Info.PetAttackIndex,		0, &arrCB[12]);

		/////////
		// 수정
		int iCBCount = 0;

		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.PetIndex, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.Level, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &temp_Info.NeedExp, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &temp_Info.UseWeaponIndex, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_SMALLINT, 0, 0, &temp_Info.SlotCountSkill, 0, &arrCB[iCBCount]);
		iCBCount++;

		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_SMALLINT, 0, 0, &temp_Info.SlotCountSocket, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_SMALLINT, 0, 0, &temp_Info.KitLevelHP, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_SMALLINT, 0, 0, &temp_Info.KitLevelShield, 0, &arrCB[iCBCount]);
		iCBCount++;
		SQLBindParameter(i_pODBCStmt->m_hstmt, (iCBCount + 1), SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_SMALLINT, 0, 0, &temp_Info.KitLevelSP, 0, &arrCB[iCBCount]);
		iCBCount++;

		// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		///////////////////////////////////////////////////////////////////////////////////////

		BOOL bRet = i_pODBCStmt->ExecuteQuery(PROCEDURE_100615_0574); //삽입(insert)
		i_pODBCStmt->FreeStatement();
		if (!bRet)
		{
			// error
			char err[1024];
			sprintf(err, "ERROR line : PetIndex(%d) !!", temp_Info.PetIndex); // 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정	
			MessageBox(NULL, err, "ti_PetLevel", MB_OK);
			return -3;
		}
		nObjects++;

		SendMessage(hWnd, WM_PROGRESSBAR_POS, 0, nObjects);
		ProcessLoopMessage(hWnd);
	}
	return nObjects;
}


void CAtumDBHelper::BindColITEM(SQLHSTMT& hstmt, ITEM& item, SQLINTEGER* cb)
{
	int i = 0;

	SQLBindCol(hstmt, ++i, SQL_C_LONG, &item.ItemNum, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.Kind, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_CHAR, item.ItemName, SIZE_MAX_ITEM_NAME, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.AbilityMin, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.AbilityMax, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ReqRace, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SHORT, &item.ReqGearStat.AttackPart, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SHORT, &item.ReqGearStat.DefensePart, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SHORT, &item.ReqGearStat.FuelPart, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SHORT, &item.ReqGearStat.SoulPart, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SHORT, &item.ReqGearStat.ShieldPart, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SHORT, &item.ReqGearStat.DodgePart, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ReqUnitKind, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.ReqMinLevel, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.ReqMaxLevel, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.ReqItemKind, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.Weight, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.HitRate, 0, &cb[i]); // 2010-07-19 by dhjin, 확률 수식 변경		
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.Defense, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.FractionResistance, 0, &cb[i]); // 2010-07-19 by dhjin, 확률 수식 변경
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.NaturalFaction, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.SpeedPenalty, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.Range, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.Position, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.Scarcity, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.Endurance, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_PROB256, &item.AbrasionRate, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.Charging, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.Luck, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.MinTradeQuantity, 0, &cb[i]);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	SQLBindCol(hstmt, ++i, SQL_C_ULONG, &item.SellingPrice, 0, &cb[i]);
#else // SHOP_PRICES_PER_BUILDING_NPC

	SQLBindCol(hstmt, ++i, SQL_C_ULONG, &item.Price, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_ULONG, &item.CashPrice, 0, &cb[i]);
#endif // SHOP_PRICES_PER_BUILDING_NPC

	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[0], 0, &cb[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[0], 0, &cb[i]); // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[1], 0, &cb[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[1], 0, &cb[i]); // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[2], 0, &cb[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[2], 0, &cb[i]); // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[3], 0, &cb[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[3], 0, &cb[i]); // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[4], 0, &cb[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[4], 0, &cb[i]); // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[5], 0, &cb[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[5], 0, &cb[i]); // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[6], 0, &cb[i]); // 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원) - // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[6], 0, &cb[i]); // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 필드명을 배열로 변경 
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[7], 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[7], 0, &cb[i]);
#ifdef _Parameter_ADDITIONAL
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[8], 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[8], 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ArrDestParameter[9], 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.ArrParameterValue[9], 0, &cb[i]);
#endif 
	SQLBindCol(hstmt, ++i, SQL_C_ULONG, &item.ReAttacktime, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_LONG, &item.Time, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.RepeatTime, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.Material, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ReqMaterial, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.RangeAngle, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.UpgradeNum, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SLONG, &item.LinkItem, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.MultiTarget, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ExplosionRange, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.ReactionRange, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.ShotNum, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.MultiNum, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_USHORT, &item.AttackTime, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.ReqSP, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SLONG, &item.SummonMonster, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_SLONG, &item.NextSkill, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.SkillLevel, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_PROB256, &item.SkillHitRate, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.SkillType, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.SkillTargetType, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.Caliber, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.OrbitType, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UBIGINT, &item.ItemAttribute, 0, &cb[i]); // 2010-01-11 by cmkwon, 버그 수정 - 기존(SQL_C_ULONG)
	SQLBindCol(hstmt, ++i, SQL_C_FLOAT, &item.BoosterAngle, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_LONG, &item.CameraPattern, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_LONG, &item.SourceIndex, 0, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_CHAR, item.Description, SIZE_MAX_ITEM_DESCRIPTION, &cb[i]);
	SQLBindCol(hstmt, ++i, SQL_C_UTINYINT, &item.EnchantCheckDestParam, 0, &cb[i]); // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	SQLBindCol(hstmt, ++i, SQL_C_ULONG, &item.InvokingDestParamID, 0, &cb[i]); // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	SQLBindCol(hstmt, ++i, SQL_C_ULONG, &item.InvokingDestParamIDByUse, 0, &cb[i]); // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
}

void CAtumDBHelper::BindColCharacter(SQLHSTMT& hstmt, CHARACTER& character, CHARACTER_DB_EX& characterDBEX, SQLINTEGER* cb)
{
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &character.CharacterUniqueNumber, 0, &cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, character.CharacterName, SIZE_MAX_CHARACTER_NAME, &cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, character.AccountName, SIZE_MAX_ACCOUNT_NAME, &cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_ULONG, &character.AccountUniqueNumber, 0, &cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_UTINYINT, &character.Color, 0, &cb[5]);
	SQLBindCol(hstmt, 6, SQL_C_USHORT, &character.Race, 0, &cb[6]);
	SQLBindCol(hstmt, 7, SQL_C_USHORT, &character.UnitKind, 0, &cb[7]);
	SQLBindCol(hstmt, 8, SQL_C_UTINYINT, &character.InfluenceType, 0, &cb[8]);
	SQLBindCol(hstmt, 9, SQL_C_UTINYINT, &character.SelectableInfluenceMask, 0, &cb[9]);
	SQLBindCol(hstmt, 10, SQL_C_UTINYINT, &character.PilotFace, 0, &cb[10]);
	SQLBindCol(hstmt, 11, SQL_C_UTINYINT, &character.CharacterMode, 0, &cb[11]);
	SQLBindCol(hstmt, 12, SQL_C_UTINYINT, &character.AutoStatType, 0, &cb[12]);
	SQLBindCol(hstmt, 13, SQL_C_SHORT, &character.GearStat.AttackPart, 0, &cb[13]);
	SQLBindCol(hstmt, 14, SQL_C_SHORT, &character.GearStat.DefensePart, 0, &cb[14]);
	SQLBindCol(hstmt, 15, SQL_C_SHORT, &character.GearStat.FuelPart, 0, &cb[15]);
	SQLBindCol(hstmt, 16, SQL_C_SHORT, &character.GearStat.SoulPart, 0, &cb[16]);
	SQLBindCol(hstmt, 17, SQL_C_SHORT, &character.GearStat.ShieldPart, 0, &cb[17]);
	SQLBindCol(hstmt, 18, SQL_C_SHORT, &character.GearStat.DodgePart, 0, &cb[18]);
	SQLBindCol(hstmt, 19, SQL_C_CHAR, character.GuildName, SIZE_MAX_GUILD_NAME, &cb[19]);
	SQLBindCol(hstmt, 20, SQL_C_ULONG, &character.GuildUniqueNumber, 0, &cb[20]);
	SQLBindCol(hstmt, 21, SQL_C_UTINYINT, &character.Level, 0, &cb[21]);
	SQLBindCol(hstmt, 22, SQL_C_DOUBLE, &character.Experience, 0, &cb[22]);
	SQLBindCol(hstmt, 23, SQL_C_DOUBLE, &character.DownExperience, 0, &cb[23]); // 2005-03-23 by cmkwon, 추가
	SQLBindCol(hstmt, 24, SQL_C_LONG, &character.DownSPIOnDeath, 0, &cb[24]); // 2006-04-10 by cmkwon, 추가
	SQLBindCol(hstmt, 25, SQL_C_UBIGINT, &character.BodyCondition, 0, &cb[25]);
	SQLBindCol(hstmt, 26, SQL_C_SLONG, &character.Propensity, 0, &cb[26]);
	SQLBindCol(hstmt, 27, SQL_C_UTINYINT, &character.Status, 0, &cb[27]);
	SQLBindCol(hstmt, 28, SQL_C_USHORT, &character.PKWinPoint, 0, &cb[28]);
	SQLBindCol(hstmt, 29, SQL_C_USHORT, &character.PKLossPoint, 0, &cb[29]);
	SQLBindCol(hstmt, 30, SQL_C_USHORT, &character.Material, 0, &cb[30]);
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	SQLBindCol(hstmt, 31, SQL_C_LONG, &character.HP, 0, &cb[31]);
	SQLBindCol(hstmt, 32, SQL_C_LONG, &character.DP, 0, &cb[32]);
#else
	SQLBindCol(hstmt, 31, SQL_C_SHORT, &character.HP, 0, &cb[31]);
	SQLBindCol(hstmt, 32, SQL_C_SHORT, &character.DP, 0, &cb[32]);
#endif
	SQLBindCol(hstmt, 33, SQL_C_SHORT, &character.SP, 0, &cb[33]);
	SQLBindCol(hstmt, 34, SQL_C_SHORT, &character.EP, 0, &cb[34]);
	SQLBindCol(hstmt, 35, SQL_C_CHAR, character.PetName, SIZE_MAX_PET_NAME, &cb[35]);
	SQLBindCol(hstmt, 36, SQL_C_UTINYINT, &character.PetLevel, 0, &cb[36]);
	SQLBindCol(hstmt, 37, SQL_C_DOUBLE, &character.PetExperience, 0, &cb[37]);
	SQLBindCol(hstmt, 38, SQL_C_FLOAT, &character.PositionVector.x, 0, &cb[38]);
	SQLBindCol(hstmt, 39, SQL_C_FLOAT, &character.PositionVector.y, 0, &cb[39]);
	SQLBindCol(hstmt, 40, SQL_C_FLOAT, &character.PositionVector.z, 0, &cb[40]);
	SQLBindCol(hstmt, 41, SQL_C_USHORT, &character.MapChannelIndex.MapIndex, 0, &cb[41]);
	SQLBindCol(hstmt, 42, SQL_C_USHORT, &character.MapChannelIndex.ChannelIndex, 0, &cb[42]);
	SQLBindCol(hstmt, 43, SQL_C_UTINYINT, &character.MaxLevel, 0, &cb[43]);
	SQLBindCol(hstmt, 44, SQL_C_FLOAT, &character.CurrentHP, 0, &cb[44]);
	SQLBindCol(hstmt, 45, SQL_C_FLOAT, &character.CurrentDP, 0, &cb[45]);
	SQLBindCol(hstmt, 46, SQL_C_SHORT, &character.CurrentSP, 0, &cb[46]);
	SQLBindCol(hstmt, 47, SQL_C_FLOAT, &character.CurrentEP, 0, &cb[47]);
	SQLBindCol(hstmt, 48, SQL_C_UTINYINT, &character.BonusStat, 0, &cb[48]);
	// 2005-11-15 by cmkwon, 삭제함
	//	SQLBindCol(hstmt, 47, SQL_C_UTINYINT, &character.BonusSkillPoint, 0,				&cb[47]);
	SQLBindCol(hstmt, 49, SQL_C_UTINYINT, &character.BonusStatPoint, 0, &cb[49]);
	SQLBindCol(hstmt, 50, SQL_C_UBIGINT, &character.LastPartyID, 0, &cb[50]);
	SQLBindCol(hstmt, 51, SQL_C_LONG, &character.RacingPoint, 0, &cb[51]);
	SQLBindCol(hstmt, 52, SQL_C_UBIGINT, &character.TotalPlayTime, 0, &cb[52]);
	SQLBindCol(hstmt, 53, SQL_C_TIMESTAMP, &characterDBEX.CreatedTime, 0, &cb[53]);
	SQLBindCol(hstmt, 54, SQL_C_TIMESTAMP, &characterDBEX.LastStartedTime, 0, &cb[54]);
	SQLBindCol(hstmt, 55, SQL_C_TIMESTAMP, &characterDBEX.LevelUpTime, 0, &cb[55]); // 2006-12-18 by dhjin, 레벨업 시간
	SQLBindCol(hstmt, 56, SQL_C_LONG, &character.WarPoint, 0, &cb[56]); // 2007-04-17 by dhjin
	SQLBindCol(hstmt, 57, SQL_C_LONG, &character.CumulativeWarPoint, 0, &cb[57]); // 2007-04-17 by dhjin
	SQLBindCol(hstmt, 58, SQL_C_LONG, &character.ArenaWin, 0, &cb[58]); // 2007-06-07 by dhjin
	SQLBindCol(hstmt, 59, SQL_C_LONG, &character.ArenaLose, 0, &cb[59]); // 2007-06-07 by dhjin
	SQLBindCol(hstmt, 60, SQL_C_LONG, &character.ArenaDisConnect, 0, &cb[60]); // 2007-06-07 by dhjin
	SQLBindCol(hstmt, 61, SQL_C_UBIGINT, &character.PCBangTotalPlayTime, 0, &cb[61]); // 2007-06-07 by dhjin
	SQLBindCol(hstmt, 62, SQL_C_ULONG, &character.SecretInfoOption, 0, &cb[62]); // 2008-06-23 by dhjin, EP3 유저정보옵션 -
	SQLBindCol(hstmt, 63, SQL_C_CHAR, &character.NickName, SIZE_MAX_CHARACTER_NAME, &cb[63]); // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
}

void CAtumDBHelper::BindColMONSTER(SQLHSTMT& hstmt, MONSTER_INFO& monster, SQLINTEGER* cb)
{
	SQLBindCol(hstmt, 1, SQL_C_LONG, &monster.MonsterUnitKind, 0, &cb[1]);
	cb[2] = SQL_NTS;
	SQLBindCol(hstmt, 2, SQL_C_CHAR, monster.MonsterName, SIZE_MAX_MONSTER_NAME, &cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_UTINYINT, &monster.Level, 0, &cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_SBIGINT, &monster.Experience, 0, &cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_LONG, &monster.MonsterHP, 0, &cb[5]);
	SQLBindCol(hstmt, 6, SQL_C_USHORT, &monster.Race, 0, &cb[6]);
	SQLBindCol(hstmt, 7, SQL_C_UTINYINT, &monster.Speed, 0, &cb[7]);
	SQLBindCol(hstmt, 8, SQL_C_LONG, &monster.ItemInfo[0].pItemInfo, 0, &cb[8]);
	SQLBindCol(hstmt, 9, SQL_C_LONG, &monster.ItemInfo[1].pItemInfo, 0, &cb[9]);
	SQLBindCol(hstmt, 10, SQL_C_LONG, &monster.ItemInfo[2].pItemInfo, 0, &cb[10]);
	SQLBindCol(hstmt, 11, SQL_C_LONG, &monster.ItemInfo[3].pItemInfo, 0, &cb[11]);
	SQLBindCol(hstmt, 12, SQL_C_LONG, &monster.ItemInfo[4].pItemInfo, 0, &cb[12]);
	SQLBindCol(hstmt, 13, SQL_C_LONG, &monster.ItemInfo[5].pItemInfo, 0, &cb[13]);
	SQLBindCol(hstmt, 14, SQL_C_LONG, &monster.ItemInfo[6].pItemInfo, 0, &cb[14]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 15, SQL_C_LONG, &monster.ItemInfo[7].pItemInfo, 0, &cb[15]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 16, SQL_C_LONG, &monster.ItemInfo[8].pItemInfo, 0, &cb[16]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 17, SQL_C_LONG, &monster.ItemInfo[9].pItemInfo, 0, &cb[17]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 18, SQL_C_LONG, &monster.ItemInfo[10].pItemInfo, 0, &cb[18]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 19, SQL_C_LONG, &monster.ItemInfo[11].pItemInfo, 0, &cb[19]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 20, SQL_C_LONG, &monster.ItemInfo[12].pItemInfo, 0, &cb[20]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 21, SQL_C_LONG, &monster.ItemInfo[13].pItemInfo, 0, &cb[21]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 22, SQL_C_LONG, &monster.ItemInfo[14].pItemInfo, 0, &cb[22]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 23, SQL_C_LONG, &monster.ItemInfo[15].pItemInfo, 0, &cb[23]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	SQLBindCol(hstmt, 24, SQL_C_USHORT, &monster.Size, 0, &cb[24]);
	SQLBindCol(hstmt, 25, SQL_C_UTINYINT, &monster.Faction, 0, &cb[25]);
	SQLBindCol(hstmt, 26, SQL_C_SHORT, &monster.MonsterForm, 0, &cb[26]);
	SQLBindCol(hstmt, 27, SQL_C_UTINYINT, &monster.AttackPattern, 0, &cb[27]);
	SQLBindCol(hstmt, 28, SQL_C_SHORT, &monster.MovePattern, 0, &cb[28]);
	SQLBindCol(hstmt, 29, SQL_C_UTINYINT, &monster.Belligerence, 0, &cb[29]);

	// 2010-07-19 by dhjin, 확률 수식 변경,	// 2009-12-17 by cmkwon, 데미지 계산식 관련한 필드(방어력,회피,피어스,확률)에 255이상 값을 설정 가능하게 수정 - 
	SQLBindCol(hstmt, 30, SQL_C_FLOAT, &monster.Defense, 0, &cb[30]);

	SQLBindCol(hstmt, 31, SQL_C_FLOAT, &monster.DefenseProbability, 0, &cb[31]);
	SQLBindCol(hstmt, 32, SQL_C_UTINYINT, &monster.AlphaBlending, 0, &cb[32]);
	SQLBindCol(hstmt, 33, SQL_C_UTINYINT, &monster.AttackObject, 0, &cb[33]);
	SQLBindCol(hstmt, 34, SQL_C_USHORT, &monster.AttackRange, 0, &cb[34]);
	SQLBindCol(hstmt, 35, SQL_C_USHORT, &monster.HPRecoveryTime, 0, &cb[35]);
	SQLBindCol(hstmt, 36, SQL_C_SHORT, &monster.HPRecoveryValue, 0, &cb[36]);
	SQLBindCol(hstmt, 37, SQL_C_USHORT, &monster.RenderIndex, 0, &cb[37]);
	SQLBindCol(hstmt, 38, SQL_C_FLOAT, &monster.ScaleValue, 0, &cb[38]);
	SQLBindCol(hstmt, 39, SQL_C_UTINYINT, &monster.TextureIndex, 0, &cb[39]);
	SQLBindCol(hstmt, 40, SQL_C_ULONG, &monster.SourceIndex, 0, &cb[40]);
	SQLBindCol(hstmt, 41, SQL_C_UTINYINT, &monster.QuickTurnAngle, 0, &cb[41]);
	SQLBindCol(hstmt, 42, SQL_C_UTINYINT, &monster.QuickSpeed, 0, &cb[42]);
	SQLBindCol(hstmt, 43, SQL_C_SHORT, &monster.QuickTurnTerm, 0, &cb[43]); // 2009-11-09 by cmkwon, 자료형 관련으로 수정 - 기존(SQL_C_USHORT)
	SQLBindCol(hstmt, 44, SQL_C_UBIGINT, &monster.MPOption, 0, &cb[44]); // 2010-01-11 by cmkwon, 몬스터 MPOption 64bit로 변경 - 기존(SQL_C_UTINYINT)
	SQLBindCol(hstmt, 45, SQL_C_UTINYINT, &monster.MPOptionParam1, 0, &cb[45]);
	SQLBindCol(hstmt, 46, SQL_C_UTINYINT, &monster.MPOptionParam2, 0, &cb[46]);
	SQLBindCol(hstmt, 47, SQL_C_USHORT, &monster.Range, 0, &cb[47]);
	SQLBindCol(hstmt, 48, SQL_C_FLOAT, &monster.TurnAngle, 0, &cb[48]);
	SQLBindCol(hstmt, 49, SQL_C_UTINYINT, &monster.MonsterHabit, 0, &cb[49]);
	SQLBindCol(hstmt, 50, SQL_C_UTINYINT, &monster.ClickEvent, 0, &cb[50]);
	SQLBindCol(hstmt, 51, SQL_C_ULONG, &monster.HPActionIdx, 0, &cb[51]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	SQLBindCol(hstmt, 52, SQL_C_ULONG, &monster.MonsterTarget, 0, &cb[52]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	SQLBindCol(hstmt, 53, SQL_C_ULONG, &monster.ChangeTarget, 0, &cb[53]); // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	SQLBindCol(hstmt, 54, SQL_C_ULONG, &monster.MonsterTarget2, 0, &cb[54]); // 2010-03-31 by dhjin, 인피니티(기지방어) - 몬스터 강제 공격 대상 번호 2번째 우선 순위	
	SQLBindCol(hstmt, 55, SQL_C_CHAR, monster.PortraitFileName, SIZE_MAX_FILE_NAME, &cb[55]); // 2010-03-31 by dhjin, 인피니티(기지방어) - 몬스터 초상화 파일
	SQLBindCol(hstmt, 56, SQL_C_LONG, &monster.ChangeTargetTime, 0, &cb[56]); // 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - ChangeTargetTime 필드 추가
	SQLBindCol(hstmt, 57, SQL_C_LONG, &monster.WayPointPattern, 0, &cb[57]); // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - WayPointPattern 필드 추가
}

// start 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
void CAtumDBHelper::BindColMonsterMultiTarget(SQLHSTMT& hstmt, MONSTER_MULTI_TARGET& target, SQLINTEGER* cb)
{
	SQLBindCol(hstmt, 1, SQL_C_LONG, &target.MonsterIndex, 0, &cb[0]);
	SQLBindCol(hstmt, 2, SQL_C_LONG, &target.PointIndex, 0, &cb[1]);
	SQLBindCol(hstmt, 3, SQL_C_FLOAT, &target.TargetPosition.x, 0, &cb[2]);
	SQLBindCol(hstmt, 4, SQL_C_FLOAT, &target.TargetPosition.y, 0, &cb[3]);
	SQLBindCol(hstmt, 5, SQL_C_FLOAT, &target.TargetPosition.z, 0, &cb[4]);
}

// end 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

void CAtumDBHelper::BindColRareItemInfo(SQLHSTMT& hstmt, RARE_ITEM_INFO& rareItemInfo, SQLINTEGER* cb)
{
	SQLBindCol(hstmt, 1, SQL_C_LONG, &rareItemInfo.CodeNum, 0, &cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, rareItemInfo.Name, SIZE_MAX_RARE_FIX_NAME, &cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_LONG, &rareItemInfo.ReqUseType, 0, &cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_LONG, &rareItemInfo.ReqMinLevel, 0, &cb[4]);
	SQLBindCol(hstmt, 5, SQL_C_LONG, &rareItemInfo.ReqMaxLevel, 0, &cb[5]);
	SQLBindCol(hstmt, 6, SQL_C_UTINYINT, &rareItemInfo.ReqItemKind, 0, &cb[6]);
	SQLBindCol(hstmt, 7, SQL_C_SHORT, &rareItemInfo.ReqGearStat.AttackPart, 0, &cb[7]);
	SQLBindCol(hstmt, 8, SQL_C_SHORT, &rareItemInfo.ReqGearStat.DefensePart, 0, &cb[8]);
	SQLBindCol(hstmt, 9, SQL_C_SHORT, &rareItemInfo.ReqGearStat.DodgePart, 0, &cb[9]);
	SQLBindCol(hstmt, 10, SQL_C_SHORT, &rareItemInfo.ReqGearStat.FuelPart, 0, &cb[10]);
	SQLBindCol(hstmt, 11, SQL_C_SHORT, &rareItemInfo.ReqGearStat.ShieldPart, 0, &cb[11]);
	SQLBindCol(hstmt, 12, SQL_C_SHORT, &rareItemInfo.ReqGearStat.SoulPart, 0, &cb[12]);
	SQLBindCol(hstmt, 13, SQL_C_UTINYINT, &rareItemInfo.DesParameter[0], 0, &cb[13]);
	SQLBindCol(hstmt, 14, SQL_C_FLOAT, &rareItemInfo.ParameterValue[0], 0, &cb[14]);
	SQLBindCol(hstmt, 15, SQL_C_UTINYINT, &rareItemInfo.DesParameter[1], 0, &cb[15]);
	SQLBindCol(hstmt, 16, SQL_C_FLOAT, &rareItemInfo.ParameterValue[1], 0, &cb[16]);
	SQLBindCol(hstmt, 17, SQL_C_UTINYINT, &rareItemInfo.DesParameter[2], 0, &cb[17]);
	SQLBindCol(hstmt, 18, SQL_C_FLOAT, &rareItemInfo.ParameterValue[2], 0, &cb[18]);
	SQLBindCol(hstmt, 19, SQL_C_UTINYINT, &rareItemInfo.DesParameter[3], 0, &cb[19]);
	SQLBindCol(hstmt, 20, SQL_C_FLOAT, &rareItemInfo.ParameterValue[3], 0, &cb[20]);
	SQLBindCol(hstmt, 21, SQL_C_UTINYINT, &rareItemInfo.DesParameter[4], 0, &cb[21]);
	SQLBindCol(hstmt, 22, SQL_C_FLOAT, &rareItemInfo.ParameterValue[4], 0, &cb[22]);
	SQLBindCol(hstmt, 23, SQL_C_UTINYINT, &rareItemInfo.DesParameter[5], 0, &cb[23]);
	SQLBindCol(hstmt, 24, SQL_C_FLOAT, &rareItemInfo.ParameterValue[5], 0, &cb[24]);
	SQLBindCol(hstmt, 25, SQL_C_UTINYINT, &rareItemInfo.DesParameter[6], 0, &cb[25]);
	SQLBindCol(hstmt, 26, SQL_C_FLOAT, &rareItemInfo.ParameterValue[6], 0, &cb[26]);
	SQLBindCol(hstmt, 27, SQL_C_UTINYINT, &rareItemInfo.DesParameter[7], 0, &cb[27]);
	SQLBindCol(hstmt, 28, SQL_C_FLOAT, &rareItemInfo.ParameterValue[7], 0, &cb[28]);
	SQLBindCol(hstmt, 29, SQL_C_UTINYINT, &rareItemInfo.DesParameter[8], 0, &cb[29]);
	SQLBindCol(hstmt, 30, SQL_C_FLOAT, &rareItemInfo.ParameterValue[8], 0, &cb[30]);
	SQLBindCol(hstmt, 31, SQL_C_PROB100K, &rareItemInfo.Probability, 0, &cb[31]);
}

void CAtumDBHelper::ProcessLogMessages(SQLSMALLINT plm_handle_type,
                                       SQLHANDLE& plm_handle,
                                       const char* logstring, BOOL ConnInd)
{
	ProcessLogMessagesStatic(plm_handle_type, plm_handle, logstring, ConnInd);
}

void CAtumDBHelper::ProcessLogMessagesStatic(SQLSMALLINT plm_handle_type,
                                             SQLHANDLE& plm_handle,
                                             const char* logstring, BOOL ConnInd)
{
	RETCODE plm_retcode = SQL_SUCCESS;
	UCHAR plm_szSqlState[MAXDBMSGBUFLEN] = "";
	UCHAR plm_szErrorMsg[MAXDBMSGBUFLEN] = "";
	SDWORD plm_pfNativeError = 0L;
	SWORD plm_pcbErrorMsg = 0;
	SQLSMALLINT plm_cRecNmbr = 1;
	SDWORD plm_SS_MsgState = 0, plm_SS_Severity = 0;
	SQLINTEGER plm_Rownumber = 0;
	USHORT plm_SS_Line;
	SQLSMALLINT plm_cbSS_Procname, plm_cbSS_Srvname;
	SQLCHAR plm_SS_Procname[MAXNAME ], plm_SS_Srvname[MAXNAME ];

	DBGOUT(logstring);

	while (plm_retcode != SQL_NO_DATA_FOUND)
	{
		plm_retcode = SQLGetDiagRec(plm_handle_type, plm_handle,
		                            plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError,
		                            plm_szErrorMsg, MAXDBMSGBUFLEN - 1, &plm_pcbErrorMsg);

		if (plm_retcode != SQL_NO_DATA_FOUND)
		{
			if (ConnInd)
			{
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
				                              SQL_DIAG_ROW_NUMBER, &plm_Rownumber,
				                              SQL_IS_INTEGER, NULL);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
				                              SQL_DIAG_SS_LINE, &plm_SS_Line,
				                              SQL_IS_INTEGER, NULL);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
				                              SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState,
				                              SQL_IS_INTEGER, NULL);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
				                              SQL_DIAG_SS_SEVERITY, &plm_SS_Severity,
				                              SQL_IS_INTEGER, NULL);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
				                              SQL_DIAG_SS_PROCNAME, &plm_SS_Procname,
				                              sizeof(plm_SS_Procname), &plm_cbSS_Procname);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
				                              SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname,
				                              sizeof(plm_SS_Srvname), &plm_cbSS_Srvname);
			}

			DBGOUT("szSqlState = %s\n", plm_szSqlState);
			DBGOUT("pfNativeError = %d\n", plm_pfNativeError);
			DBGOUT("szErrorMsg = %s\n", plm_szErrorMsg);
			DBGOUT("pcbErrorMsg = %d\n\n", plm_pcbErrorMsg);

			if (ConnInd)
			{
				DBGOUT("ODBCRowNumber = %d\n", plm_Rownumber);
				DBGOUT("SSrvrLine = %d\n", plm_Rownumber);
				DBGOUT("SSrvrMsgState = %d\n", plm_SS_MsgState);
				DBGOUT("SSrvrSeverity = %d\n", plm_SS_Severity);
				DBGOUT("SSrvrProcname = %s\n", plm_SS_Procname);
				DBGOUT("SSrvrSrvname = %s\n\n", plm_SS_Srvname);
			}
		}
		plm_cRecNmbr++; //Increment to next diagnostic record.
	} // End while.
}

// 2009. 10. 27 by jskim 진행률 확인
void CAtumDBHelper::ProcessLoopMessage(HWND hWnd)
{
	MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_NOREMOVE))
	{
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

//end 2009. 10. 27 by jskim 진행률 확인


