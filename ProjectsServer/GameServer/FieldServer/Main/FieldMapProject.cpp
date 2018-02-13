 // FieldMapProject.cpp: implementation of the CFieldMapProject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldMapProject.h"
#include "FieldIOCP.h"
#include "FieldMapProject.h"
#include "MapProject.h"
#include "FieldGlobal.h"
#include "MapTriggerManager.h"		// 2012-12-17 by hskim, NPC ¼­¹ö Àç½ÃÀÛ½Ã - Æ®¸®°Å ½Ã½ºÅÛ ÃÊ±âÈ­ Àû¿ë




///////////////////////////////////////////////////////////////////////////////
/// \fn			SHOP_ITEM *CShopInfo::GetSHOP_ITEMPtr(int i_itemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-10-20 ~ 2006-10-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHOP_ITEM *CShopInfo::GetSHOP_ITEMPtr(int i_itemNum)
{
	int nSize = m_vectShopItemList.size();
	for(int i=0; i < nSize; i++)
	{
		if(i_itemNum == m_vectShopItemList[i].ItemNum)
		{
			return &m_vectShopItemList[i];
		}
	}
	return NULL;
}
	 
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, Ä³½¬ ¾ÆÀÌÅÛ ÇÑÁ¤ÆÇ¸Å ½Ã½ºÅÛ ±¸Çö - 
/// \author		cmkwon
/// \date		2010-02-02 ~ 2010-02-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHOP_ITEM *CShopInfo::GetSHOP_ITEMPtrFrombuffer(int i_itemNum)
{
	if(0 >= this->m_buffer.size)
	{
		return NULL;
	}

	char *pBuffer		= this->m_buffer.GetPtr();
	int nShopItemCnt	= this->m_vectShopItemList.size();
	if(NULL == pBuffer
		|| 0 >= nShopItemCnt)
	{
		return NULL;
	}

	int nReadBytes = 0;
	MessageType_t nMsgType = 0;

	nReadBytes	= SIZE_FIELD_TYPE_HEADER + sizeof(MSG_FC_SHOP_PUT_ITEM_HEADER);
	nMsgType	= *(MessageType_t*)(pBuffer + nReadBytes);
	nReadBytes	+= SIZE_FIELD_TYPE_HEADER;
	while(nMsgType == T_FC_SHOP_PUT_ITEM)
	{
		MSG_FC_SHOP_PUT_ITEM *pPutItem = (MSG_FC_SHOP_PUT_ITEM*)(pBuffer + nReadBytes);
		nReadBytes	+= sizeof(MSG_FC_SHOP_PUT_ITEM);
		for(int i=0; i < pPutItem->BytesToRead/sizeof(SHOP_ITEM); i++)
		{
			SHOP_ITEM *pShopItem = (SHOP_ITEM*)(pBuffer + nReadBytes);
			nReadBytes += sizeof(SHOP_ITEM);
			if(i_itemNum == pShopItem->ItemNum)
			{
				return pShopItem;
			}
		}

		nMsgType	= *(MessageType_t*)(pBuffer + nReadBytes);
		nReadBytes	+= SIZE_FIELD_TYPE_HEADER;		
	}

	return NULL;
}

// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
void CShopInfo::SetAllRemainCountForLimitedEdition(int iCount)
{
	int nSize = m_vectShopItemList.size();
	for(int i=0; i < nSize; i++)
	{
		m_vectShopItemList[i].RemainCountForLimitedEdition = iCount;

		SHOP_ITEM *pShopItem = GetSHOP_ITEMPtrFrombuffer(m_vectShopItemList[i].ItemNum);

		if(NULL == pShopItem)
		{
			continue;
		}

		pShopItem->RemainCountForLimitedEdition	= iCount;
	}
}

void CShopInfo::SetShopOwner(UID32_t OwnerCharacterUniqueNumber)
{ 
	m_OwnerCharacterUniqueNumber = OwnerCharacterUniqueNumber; 
}

UID32_t CShopInfo::GetShopOwner()
{
	return m_OwnerCharacterUniqueNumber;
}
// end 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldMapProject::CFieldMapProject(CMapWorkspace * i_pMapWorkspace, PROJECTINFO * i_pProjectInfo)
: CMapProject(i_pMapWorkspace, i_pProjectInfo)
{
	m_mapShopInfo.clear();
	m_pDeadDefaultMapProject	= NULL;
	util::zero(m_szUserFriendlyMapName, SIZE_MAX_MAP_NAME);
	m_Weather					= WEATHER_SUNNY;	// µµ½Ã¸¦ Á¦¿ÜÇÑ ¸ÊÀº ±âº»ÀÌ ¸¼À½!

	m_mapBuilding2Building.clear();
	m_mapNPC2Building.clear();

	m_bNPCServerStart			= FALSE;
	m_bNPCServerMonsterCreated	= FALSE;		// 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
	m_pRefCityWar2				= NULL;

	m_nNextWarpTargetEventIndex	= 0;			// 2006-08-03 by cmkwon

	// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
	m_bEnableEnterOneTimeLimited = FALSE;
	m_mtVectEnterOneTimeLimited.lock();
	m_mtVectEnterOneTimeLimited.clear();
	m_mtVectEnterOneTimeLimited.unlock();

	m_bEnableEnterGuildLimited = FALSE;
	m_EnterGuildLimited = 0;

	m_bEnableEnterCharacterLimited = FALSE;
	m_EnterCharacterLimited = 0;

	m_bEnableEnterInfluenceLimited = FALSE;
	m_EnterInfluenceLimited = 0;

	m_bEnterMaxiumUserLimited = FALSE;

	m_EntranceCondition.clear();
	// end 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

	m_pFieldMapWorkspace		= (CFieldMapWorkspace*)m_pMapWorkspace;

	m_CashShopInfo = NULL;
}

CFieldMapProject::~CFieldMapProject()
{
	map<UINT, CShopInfo*>::iterator itr_shop(m_mapShopInfo.begin());
	while (itr_shop != m_mapShopInfo.end())
	{
		delete(itr_shop->second);
		itr_shop++;
	}
	m_mapShopInfo.clear();

	ez_map<UINT, BUILDINGNPC*>::iterator itr_buildingnpc(m_mapBuilding2Building.begin());
	while (itr_buildingnpc != m_mapBuilding2Building.end())
	{
		delete(itr_buildingnpc->second);
		itr_buildingnpc++;
	}
	m_mapBuilding2Building.clear();

	m_mapNPC2Building.clear();			// m_mapBuilding2Building°ú °°Àº µ¥ÀÌÅ¸¸¦ °øÀ¯ÇÏ°í ÀÖÀ¸¹Ç·Î delete Ã³¸®ÇÏÁö ¾Ê¾Æ¾ßÇÔ

	// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
	m_bEnableEnterOneTimeLimited = FALSE;
	m_mtVectEnterOneTimeLimited.lock();
	m_mtVectEnterOneTimeLimited.clear();
	m_mtVectEnterOneTimeLimited.unlock();

	m_bEnableEnterGuildLimited = FALSE;
	m_EnterGuildLimited = 0;

	m_bEnableEnterCharacterLimited = FALSE;
	m_EnterCharacterLimited = 0;

	m_bEnableEnterInfluenceLimited = FALSE;
	m_EnterInfluenceLimited = 0;

	m_bEnterMaxiumUserLimited = FALSE;

	m_EntranceCondition.clear();
	// end 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
}

///////////////////////////////////////////////////////////////////////////
// virtual
// ÃÊ±âÈ­ °ü·Ã, 2004-03-26 by cmkwon
BOOL CFieldMapProject::InitMapProject(void)
{
	BOOL bReted = CMapProject::InitMapProject();
	if(bReted == FALSE){					return FALSE;}

	// LoadBUILDINGNPC()ÀÌ LoadShopInfo()º¸´Ù ¸ÕÀú È£ÃâµÇ¾î¾ß ÇÔ!
	if (LoadBUILDINGNPC(m_pFieldMapWorkspace->GetPtrodbcStmtForLoading()) == FALSE)
	{
		return FALSE;
	}

	if (LoadShopInfo(m_pFieldMapWorkspace->GetPtrodbcStmtForLoading()) == FALSE)
	{
		return FALSE;
	}

	if (LoadCityTargetWarpMap(m_pFieldMapWorkspace->GetPtrodbcStmtForLoading()) == FALSE)
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö -
	if(FALSE == LoadWarpableUserList(m_pFieldMapWorkspace->GetPtrodbcStmtForLoading()))
	{
		return FALSE;
	}

	if (BelongsToServer() == FALSE)
	{
		return TRUE;
	}

	// Ã¤³Î »ý¼º
	if (InitializeMapChannels(m_nCreateChannelCounts) == FALSE){	return FALSE;}

	// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
	if( IS_HELL_2ND_MAP(m_nMapIndex) )
	{
		// Æ¯Á¤ ¸Ê¿¡ ÀÔÀå½Ã Æ¯Á¤ ¾ÆÀÌÅÛ Á¶°ÇÀ» °Ë»ç(ÇÏ°í »èÁ¦) ÇÏ´Â ±â´É
		// ¹ü¿ëÀû »ç¿ëÀÌ ÇÊ¿äÇÑ °æ¿ì DB °ªÀ» °¡Áö°í ¼³Á¤ÇÒ ¼ö ÀÖµµ·Ï Ãß°¡ ±¸Çö
		// DB ½ºÅ°¸¶´Â ItemNum, RequireCount, Option ÀÌ·¸°Ô 3°³¸¦ ³Öµµ·Ï ±¸ÇöÇÏ¼¼¿ä
		// ItemNum <= ÇÊ¿ä ¾ÆÀÌÅÛ
		// RequireCount <= ÇÊ¿äÇÑ ¾ÆÀÌÅÛ ¼ýÀÚ
		// Option <= ÀÔÀå½Ã »èÁ¦ ¿©ºÎ (ÇöÀç±îÁö ±¸ÇöµÈ ³»¿ë)
		// µ¥ÀÌÅÍµé¸¸ DB ·Î »©Áö ¾Ê¾ÒÁö ³»ºÎ ÄÚµå´Â ¸ðµÎ ±¸ÇöµÇ¾î ÀÖÀ½
		// ±¸Çö ÈÄ ÀÌ ºÎºÐÀº DB¿¡¼­ ÀÐ¾î¿Â µ¥ÀÌÅÍ·Î ¼³Á¤ÇÏµµ·Ï ¼öÁ¤
		// 2011³â 12¿ù 19ÀÏ... EP4 ÃÖÁ¾ Àü´ÞÀ» ¸îÀÏ ³²°ÜµÎ°í... hskim

		{
			SMAP_ENTRANCE_CONDITION EntranceCondition;
			EntranceCondition.ItemNum = STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_ENTER_CONDITION_ITEM_UID_01;
			EntranceCondition.RequireCount = 1;
			EntranceCondition.Option = MAP_ENTRANCE_CONDITION_OPTION_DELETE;
			m_EntranceCondition.push_back(EntranceCondition);
		}

		{
			SMAP_ENTRANCE_CONDITION EntranceCondition;
			EntranceCondition.ItemNum = STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_ENTER_CONDITION_ITEM_UID_02;
			EntranceCondition.RequireCount = 1;
			EntranceCondition.Option = MAP_ENTRANCE_CONDITION_OPTION_DELETE;
			m_EntranceCondition.push_back(EntranceCondition);
		}
	}

	if( TRUE == IS_MAP_INFLUENCE_HELL(this->GetMapInfluenceType()) )
	{
		SetEnterCharacterLimited(TRUE, 0);		// ÃÊ±â¿¡´Â ¾Æ¹«µµ ÀÔÀåÇÏÁö ¸øÇÏ°Ô ¼³Á¤
	}
	// end 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

	return TRUE;
}

void CFieldMapProject::SetNPCServerStartFlag(BOOL i_bStartFlag)
{
	m_bNPCServerStart = i_bStartFlag;
}

BOOL CFieldMapProject::GetNPCServerStartFlag(void)
{
	if (FALSE == m_bNPCServerStart
		|| NULL == m_pIOCPSocketMapSession
		|| FALSE == m_pIOCPSocketMapSession->IsUsing()
		|| FALSE == m_pIOCPSocketMapSession->GetPeerUDPReady())
	{
		m_bNPCServerStart = FALSE;
	}

	return m_bNPCServerStart;
}

// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
BOOL CFieldMapProject::IsNPCServerMonsterCreated(void)
{
	return m_bNPCServerMonsterCreated;
}
// end 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

BOOL CFieldMapProject::LoadBUILDINGNPC(CODBCStatement *i_pOdbcStmt)
{
	SQLHENV		henv = i_pOdbcStmt->m_henv;
	SQLHDBC		hdbc = i_pOdbcStmt->m_hdbc;
	SQLHSTMT	hstmt = i_pOdbcStmt->m_hstmt;

	RETCODE		ret;
	SDWORD		arrCB[10];
	BUILDINGNPC	retBuildingNPC;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_GetBuildingNPC
		@MapIndex	SMALLINT
	AS
		SELECT *
		FROM ti_BuildingNPC
		WHERE MapIndex = @MapIndex OR MapIndex = 0
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &m_nMapIndex, 0, 0);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0202, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt,"Load BuildingNPC Failed!\n", TRUE);
		return FALSE;
	}

	SQLBindCol(hstmt,  1, SQL_C_ULONG  , &retBuildingNPC.BuildingIndex, 0,						&arrCB[1]);		
	arrCB[2] = SQL_NTS;
	SQLBindCol(hstmt,  2, SQL_C_CHAR   , retBuildingNPC.BuildingName, SIZE_MAX_BUILDING_NAME,	&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_UTINYINT, &retBuildingNPC.BuildingKind, 0,						&arrCB[3]);
	arrCB[3] = SQL_NTS;
	SQLBindCol(hstmt,  4, SQL_C_USHORT , &retBuildingNPC.MapIndex, 0,							&arrCB[4]);
	SQLBindCol(hstmt,  5, SQL_C_ULONG  , &retBuildingNPC.NPCIndex, 0,							&arrCB[5]);
	arrCB[6] = SQL_NTS;
	SQLBindCol(hstmt,  6, SQL_C_CHAR   , retBuildingNPC.NPCName, SIZE_MAX_NPC_NAME,				&arrCB[6]);
	arrCB[7] = SQL_NTS;
	SQLBindCol(hstmt,  7, SQL_C_CHAR   , retBuildingNPC.GreetingTalk, SIZE_MAX_BUILDING_NPC_TALK_STRING,	&arrCB[7]);
	SQLBindCol(hstmt,  8, SQL_C_ULONG  , &retBuildingNPC.OwnerCityMapIndex, 0,					&arrCB[8]);
	SQLBindCol(hstmt,  9, SQL_C_ULONG  , &retBuildingNPC.OwnerOrderNumber, 0,					&arrCB[9]);

	// m_buffGetBuildingListOK¿¡ °ª ÇÒ´ç
	m_buffGetBuildingListOK.Reset();
	INIT_MSG(MSG_FC_CITY_GET_BUILDING_LIST_OK, T_FC_CITY_GET_BUILDING_LIST_OK, pMsgGetBuildingListOK, m_buffGetBuildingListOK.ptr);
	pMsgGetBuildingListOK->NumOfBuildings = 0;
	m_buffGetBuildingListOK.size = MSG_SIZE(MSG_FC_CITY_GET_BUILDING_LIST_OK);
	// initialize to 0x00
	memset(&retBuildingNPC, 0x00, sizeof(BUILDINGNPC));
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		if(0 == retBuildingNPC.MapIndex)
		{// MapIndex == 0ÀÌ¸é À¯·áÈ­ »óÁ¡ÀÌ´Ù, ÇöÀç MapIndex·Î ¼³Á¤ÇÑ´Ù
			retBuildingNPC.MapIndex = m_nMapIndex;
		}

		BOOL ret = FALSE;

		BUILDINGNPC *pTmpBuildingNPC = new BUILDINGNPC;
		*pTmpBuildingNPC = retBuildingNPC;

		ret = m_mapBuilding2Building.insertEZ(retBuildingNPC.BuildingIndex, pTmpBuildingNPC);
		if (!ret)
		{
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return FALSE;
		}

		m_mapNPC2Building.insertEZ(retBuildingNPC.NPCIndex, pTmpBuildingNPC);
		if (!ret)
		{
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return FALSE;
		}

		// m_buffGetBuildingListOK¿¡ °ª ÇÒ´ç
		CITY_BUILDING_INFO_4_EXCHANGE *pTmp
			= (CITY_BUILDING_INFO_4_EXCHANGE*)(m_buffGetBuildingListOK.ptr + m_buffGetBuildingListOK.size);
		pTmp->BuildingIndex = pTmpBuildingNPC->BuildingIndex;
		pTmp->BuildingKind = pTmpBuildingNPC->BuildingKind;
		m_buffGetBuildingListOK.size += sizeof(CITY_BUILDING_INFO_4_EXCHANGE);
		pMsgGetBuildingListOK->NumOfBuildings++;

		// initialize to 0x00
		memset(&retBuildingNPC, 0x00, sizeof(BUILDINGNPC));
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	return TRUE;
}

BOOL CFieldMapProject::LoadCityTargetWarpMap(CODBCStatement *i_pOdbcStmt)
{
	SQLHENV		henv = i_pOdbcStmt->m_henv;
	SQLHDBC		hdbc = i_pOdbcStmt->m_hdbc;
	SQLHSTMT	hstmt = i_pOdbcStmt->m_hstmt;

	RETCODE		ret;
	SQLINTEGER	cb1, cb2, cb3, cb4, cb5, cb6;
	UINT		MapIndex;
	UINT		BuildingIndex;
	UINT		WarpTargetMapIndex;
	UINT		WarpTargetIndex;
	char		WarpTargetName[SIZE_MAX_WARP_TARGET_NAME];
	INT			Fee;

//	CHAR SQLQuery[512];
	// 2008-07-02 by cmkwon, MySQL Áö¿ø ±¸Çö - 
	//sprintf(SQLQuery, "SELECT * FROM ti_CityTargetWarpMap WITH (NOLOCK) WHERE MapIndex = %d", m_nMapIndex);
// 2008-08-27 by cmkwon, ³¯Äõ¸®¸¦ Procedure·Î ¼öÁ¤ - 	
//	sprintf(SQLQuery, QUERY_080702_0248, m_nMapIndex);

//	ret = SQLExecDirect(hstmt, (UCHAR*)SQLQuery, SQL_NTS);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &m_nMapIndex, 0, NULL);
	ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0248), SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt,"Load CityTargetWarpMap Failed!\n", TRUE);
		return FALSE;
	}

	SQLBindCol(hstmt,  1, SQL_C_ULONG  , &MapIndex, 0, &cb1);
	SQLBindCol(hstmt,  2, SQL_C_ULONG  , &BuildingIndex, 0, &cb2);
	SQLBindCol(hstmt,  3, SQL_C_ULONG  , &WarpTargetMapIndex, 0, &cb3);
	SQLBindCol(hstmt,  4, SQL_C_ULONG  , &WarpTargetIndex, 0, &cb4);
	cb5 = SQL_NTS;
	SQLBindCol(hstmt,  5, SQL_C_CHAR, WarpTargetName, SIZE_MAX_WARP_TARGET_NAME, &cb5);
	SQLBindCol(hstmt,  6, SQL_C_LONG   , &Fee, 0, &cb6);

// 2007-01-29 by cmkwon, ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤ÇÔ
//	// initialize to 0x00
//	m_listCityWarpTargetMapInfo.clear();
//	m_buffGetWarpTargetMapListOK.Reset();
//// 2004-11-10 by cmkwon, T_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK·Î ´ëÃ¼ÇÔ
////	INIT_MSG(MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST_OK, T_FC_CITY_GET_WARP_TARGET_MAP_LIST_OK, pMsgGetWarpList, m_buffGetWarpTargetMapListOK.ptr);
//	INIT_MSG(MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK, T_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK, pMsgGetWarpList, m_buffGetWarpTargetMapListOK.ptr);
//	pMsgGetWarpList->NumOfTargetMaps = 0;
//	m_buffGetWarpTargetMapListOK.size = MSG_SIZE(MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST_OK);
//	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
//	{
//		WARP_TARGET_MAP_INFO_4_EXCHANGE *pTmp
//			= (WARP_TARGET_MAP_INFO_4_EXCHANGE*)((char*)m_buffGetWarpTargetMapListOK.ptr + m_buffGetWarpTargetMapListOK.size);
//		pTmp->MapIndex		= WarpTargetMapIndex;
//		pTmp->TargetIndex	= WarpTargetIndex;
//		pTmp->Fee			= Fee;
//		util::strncpy(pTmp->TargetName, WarpTargetName, SIZE_MAX_WARP_TARGET_NAME);
//		const MAP_INFO *pMapInfo = CAtumSJ::GetMapInfo(pTmp->MapIndex);
//		if(pMapInfo)
//		{
//			util::strncpy(pTmp->TargetName, pMapInfo->MapName, SIZE_MAX_WARP_TARGET_NAME);
//		}
//
//		m_buffGetWarpTargetMapListOK.size += sizeof(WARP_TARGET_MAP_INFO_4_EXCHANGE);
//		pMsgGetWarpList->NumOfTargetMaps++;
//
//		m_listCityWarpTargetMapInfo.push_back(*pTmp);
//	}
//
//	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_vectCityWarpInfoList.clear();
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		WARP_TARGET_MAP_INFO_4_EXCHANGE tmWarpInfo;
		util::zero(&tmWarpInfo, sizeof(tmWarpInfo));

		tmWarpInfo.MapIndex		= WarpTargetMapIndex;
		tmWarpInfo.TargetIndex	= WarpTargetIndex;
		tmWarpInfo.Fee			= Fee;
		util::strncpy(tmWarpInfo.TargetName, WarpTargetName, SIZE_MAX_WARP_TARGET_NAME);
		const MAP_INFO *pMapInfo = CAtumSJ::GetMapInfo(tmWarpInfo.MapIndex);
		if(pMapInfo)
		{
			util::strncpy(tmWarpInfo.TargetName, pMapInfo->MapName, SIZE_MAX_WARP_TARGET_NAME);
		}
		m_vectCityWarpInfoList.push_back(tmWarpInfo);
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_buffGetWarpTargetMapListOK.Reset();
	
	if(false == m_vectCityWarpInfoList.empty())
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-29 by cmkwon, Á¤·Ä


		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-29 by cmkwon, Àü¼Û ¹öÆÛ¸¦ ¼³Á¤ÇÑ´Ù
		INIT_MSG(MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK, T_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK, pMsgGetWarpList, m_buffGetWarpTargetMapListOK.ptr);
		pMsgGetWarpList->NumOfTargetMaps = 0;
		m_buffGetWarpTargetMapListOK.size = MSG_SIZE(MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST_OK);
		for(int i=0; i < m_vectCityWarpInfoList.size(); i++)
		{
			if(m_buffGetWarpTargetMapListOK.size + sizeof(WARP_TARGET_MAP_INFO_4_EXCHANGE) > SIZE_MAX_PACKET)
			{
				char szErr[1024];
				sprintf(szErr, "[Error] CFieldMapProject::LoadCityTargetWarpMap count error, MapIndex(%d), WarpCount(%d)\r\n",
					m_nMapIndex, m_vectCityWarpInfoList.size());
				DbgOut(szErr);
				break;
			}
			WARP_TARGET_MAP_INFO_4_EXCHANGE *pTmp
				= (WARP_TARGET_MAP_INFO_4_EXCHANGE*)((char*)m_buffGetWarpTargetMapListOK.ptr + m_buffGetWarpTargetMapListOK.size);
			*pTmp	= m_vectCityWarpInfoList[i];

			m_buffGetWarpTargetMapListOK.size += sizeof(WARP_TARGET_MAP_INFO_4_EXCHANGE);
			pMsgGetWarpList->NumOfTargetMaps++;
		}
	}
	return TRUE;
}

BOOL CFieldMapProject::LoadShopInfo(CODBCStatement *i_pOdbcStmt)
{
	SQLHENV		henv = i_pOdbcStmt->m_henv;
	SQLHDBC		hdbc = i_pOdbcStmt->m_hdbc;
	SQLHSTMT	hstmt = i_pOdbcStmt->m_hstmt;
	RETCODE		ret;
	SQLINTEGER	arrCB[9];	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 8·Î ¼öÁ¤
	fill_n(arrCB, 9, SQL_NTS);
	UINT		BuildingIndex;
	vector<int> vectBuildingIndexList;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-20 by cmkwon, ÇöÀç ¸ÊÀÇ ¸ðµç BuildingIndex¸¦ °¡Á®¿Â´Ù
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_GetAllBuildingIndex
		@MapIndex SMALLINT
	AS
		SELECT BuildingIndex
			FROM ti_BuildingNPC
			WHERE MapIndex = @MapIndex OR MapIndex = 0
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &m_nMapIndex, 0, 0);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0203, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt,"Load ShopInfo Failed!\n", TRUE);
		return FALSE;
	}
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &BuildingIndex, 0, &arrCB[1]);
	BuildingIndex	= 0;
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		if(0 != BuildingIndex)
		{
			map<UINT, CShopInfo*>::iterator itr_shop = m_mapShopInfo.find(BuildingIndex);
			if (itr_shop == m_mapShopInfo.end())
			{
				vectBuildingIndexList.push_back(BuildingIndex);

				CShopInfo *pShopInfo = new CShopInfo;
				m_mapShopInfo.insert(pair<UINT, CShopInfo*>(BuildingIndex, pShopInfo));
			}
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-20 by cmkwon, ¸ðµç »óÁ¡ÀÇ ¾ÆÀÌÅÛ ¸®½ºÆ®¸¦ °¡Á®¿Â´Ù
	for(int i=0; i < vectBuildingIndexList.size(); i++)
	{
		map<UINT, CShopInfo*>::iterator itr_shop = m_mapShopInfo.find(vectBuildingIndexList[i]);
		if (itr_shop != m_mapShopInfo.end())
		{
			SHOP_ITEM	shopItem;
			/*[Stored Query Definition]************************************************
			-- !!!!
			-- Name:
			-- Desc:2006-10-20 by cmkwon, BuildingIndexÀÇ ¾ÆÀÌÅÛ °¡Á®¿À±â
			--		// 2008-01-22 by cmkwon, S_DB: ti_Shop.ShopOrder ¼øÀ¸·Î »óÁ¡ ¾ÆÀÌÅÛ ¸®½ºÆ®¸¦ °¡Á®¿Â´Ù.
			--		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
			--		// 2010-01-26 by cmkwon, Ä³½¬ ¾ÆÀÌÅÛ ÇÑÁ¤ÆÇ¸Å ½Ã½ºÅÛ ±¸Çö - ÇÑÁ¤ÆÇ¸Å ³²Àº °³¼öµµ °á°ú¿¡ Ãß°¡
			-- ====
			CALL dbo.atum_GetShopItems
			**************************************************************************/
			SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &vectBuildingIndexList[i], 0, 0);
			ret = SQLExecDirect(hstmt, PROCEDURE_080822_0204, SQL_NTS);
			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
			{
				CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt,"Load ShopInfo Failed!\n", TRUE);
				return FALSE;
			}
			arrCB[1]=arrCB[2]=arrCB[3]=arrCB[4]=arrCB[5]=arrCB[6]=arrCB[7]=arrCB[8]=SQL_NTS;		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - ÃÊ±âÈ­ Ãß°¡
			BYTE byTapIndex		= 0;			// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
			BYTE byCashShopBit	= 0;			// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
			SQLBindCol(hstmt, 1, SQL_C_ULONG, &shopItem.ItemNum, 0,					&arrCB[1]);
			SQLBindCol(hstmt, 2, SQL_C_CHAR, shopItem.ItemName, SIZE_MAX_ITEM_NAME,	&arrCB[2]);
			SQLBindCol(hstmt, 3, SQL_C_USHORT, &shopItem.MinTradeQuantity, 0,		&arrCB[3]);
			SQLBindCol(hstmt, 4, SQL_C_LONG, &shopItem.Price, 0,					&arrCB[4]);
			SQLBindCol(hstmt, 5, SQL_C_UTINYINT, &shopItem.ItemKind, 0,				&arrCB[5]);
			SQLBindCol(hstmt, 6, SQL_C_UTINYINT, &byTapIndex, 0,					&arrCB[6]);		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - TabIndex °¡Á®¿À±â
			SQLBindCol(hstmt, 7, SQL_C_UTINYINT, &byCashShopBit, 0,					&arrCB[7]);		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - CashShopBit °¡Á®¿À±â
			SQLBindCol(hstmt, 8, SQL_C_LONG, &shopItem.RemainCountForLimitedEdition, 0,	&arrCB[8]);		// 2010-01-26 by cmkwon, Ä³½¬ ¾ÆÀÌÅÛ ÇÑÁ¤ÆÇ¸Å ½Ã½ºÅÛ ±¸Çö - 

			util::zero(&shopItem, sizeof(SHOP_ITEM));
			shopItem.RemainCountForLimitedEdition		= UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION;	// 2010-01-26 by cmkwon, Ä³½¬ ¾ÆÀÌÅÛ ÇÑÁ¤ÆÇ¸Å ½Ã½ºÅÛ ±¸Çö - ÃÊ±â°ª ¼³Á¤ ÇÊ¿ä
			while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
			{
				// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - TabIndex¿Í CashShopBit¸¦ CashShopIndex·Î ÅëÇÕÇÑ´Ù.
				shopItem.CashShopIndex	= GET_TABINDEX_BY_CASHSHOPINDEX(byTapIndex) + GET_CASHSHOP_BIT_BY_CASHSHOPINDEX(byCashShopBit);
				
				itr_shop->second->m_vectShopItemList.push_back(shopItem);
				util::zero(&shopItem, sizeof(SHOP_ITEM));
				byTapIndex				= 0;		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - ÃÊ±âÈ­
				byCashShopBit			= 0;		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - ÃÊ±âÈ­
				shopItem.RemainCountForLimitedEdition		= UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION;	// 2010-01-26 by cmkwon, Ä³½¬ ¾ÆÀÌÅÛ ÇÑÁ¤ÆÇ¸Å ½Ã½ºÅÛ ±¸Çö - ÃÊ±â°ª ¼³Á¤ ÇÊ¿ä
			}
			SQLFreeStmt(hstmt, SQL_CLOSE);
		}		
	}


// 2006-10-20 by cmkwon
//	g_pFieldGlobal->WriteSystemLogEX(FALSE, "[Item Shop Info: %4d]\r\n", m_nMapIndex);
	map<UINT, CShopInfo*>::iterator itr_shop = m_mapShopInfo.begin();
	while (itr_shop != m_mapShopInfo.end())
	{
		char* SendBuf = itr_shop->second->m_buffer.ptr;
		int offset = 0;
		int NumOfItem = 0;

		// FC_SHOP_PUT_ITEM_HEADER
		*(MessageType_t*)SendBuf = T_FC_SHOP_PUT_ITEM_HEADER;
		offset += SIZE_FIELD_TYPE_HEADER;

		MSG_FC_SHOP_PUT_ITEM_HEADER *pPutHeader = (MSG_FC_SHOP_PUT_ITEM_HEADER*)(SendBuf + offset);
		pPutHeader->BuildingIndex = itr_shop->first;
		offset += sizeof(MSG_FC_SHOP_PUT_ITEM_HEADER);

		// FC_SHOP_PUT_ITEM
		*(MessageType_t*)(SendBuf + offset) = T_FC_SHOP_PUT_ITEM;
		offset += SIZE_FIELD_TYPE_HEADER;

		MSG_FC_SHOP_PUT_ITEM *pPutItem = (MSG_FC_SHOP_PUT_ITEM*)(SendBuf + offset);
		offset += sizeof(MSG_FC_SHOP_PUT_ITEM);

		int nBytesUsedForPutItem = 0;

		vectSHOP_ITEM::iterator itr_item = itr_shop->second->m_vectShopItemList.begin();
		while (itr_item != itr_shop->second->m_vectShopItemList.end())
		{
			if (nBytesUsedForPutItem + sizeof(SHOP_ITEM) > SIZE_MAX_PACKET)
			{
				pPutItem->BytesToRead = nBytesUsedForPutItem;

				*(MessageType_t*)(SendBuf + offset) = T_FC_SHOP_PUT_ITEM;
				offset += SIZE_FIELD_TYPE_HEADER;
				pPutItem = (MSG_FC_SHOP_PUT_ITEM*)(SendBuf + offset);
				offset += sizeof(MSG_FC_SHOP_PUT_ITEM);
				nBytesUsedForPutItem = 0;
			}

			*(SHOP_ITEM*)(SendBuf + offset) = *itr_item;
			offset += sizeof(SHOP_ITEM);
			nBytesUsedForPutItem += sizeof(SHOP_ITEM);

			NumOfItem++;
			itr_item++;
		}

		// ³ª¸ÓÁö ¶³°ÅÁö¿¡ ´ëÇØ¼­, ÀÐ¾î¾ß ÇÒ byte ¼ö ÀúÀå
		pPutItem->BytesToRead = nBytesUsedForPutItem;

		// FC_SHOP_PUT_ITEM_DONE
		*(MessageType_t*)(SendBuf + offset) = T_FC_SHOP_PUT_ITEM_DONE;
		offset += SIZE_FIELD_TYPE_HEADER;

		MSG_FC_SHOP_PUT_ITEM_DONE *pPutItemDone = (MSG_FC_SHOP_PUT_ITEM_DONE*)(SendBuf + offset);
		offset += sizeof(MSG_FC_SHOP_PUT_ITEM_DONE);
		pPutItemDone->NumOfItem = NumOfItem;

		itr_shop->second->m_buffer.size = offset;

		assert(offset <= SIZE_MAX_LONG_PACKET);

		itr_shop++;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::LoadWarpableUserList(CODBCStatement *i_pOdbcStmt)
/// \brief		// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - ·Îµù ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::LoadWarpableUserList(CODBCStatement *i_pOdbcStmt)
{
	if(FALSE == IS_CONFERENCEROOM_MAP_INDEX(this->m_nMapIndex))
	{// 2007-08-30 by cmkwon, Ã¼Å©
		return TRUE;
	}

	SQLHENV		henv = i_pOdbcStmt->m_henv;
	SQLHDBC		hdbc = i_pOdbcStmt->m_hdbc;
	SQLHSTMT	hstmt = i_pOdbcStmt->m_hstmt;

	RETCODE		ret;
	SQLINTEGER	arrCB[3]	= {SQL_NTS,SQL_NTS,SQL_NTS};
	INT			nMapIndex	= this->m_nMapIndex;
	S_S1_CHARACTER_INFO		s1CharInfo;
	util::zero(&s1CharInfo, sizeof(s1CharInfo));

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc: // 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - ·Îµù ÇÁ·Î½ÃÀú Ãß°¡
	--====
	CREATE PROCEDURE dbo.atum_loadWarpableUserList
		@i_MapIndex				INT
	AS
		SELECT c.UniqueNumber, c.CharacterName
		FROM td_user_list_warpable_to_map ul WITH(NOLOCK), td_character c WITH(NOLOCK)
		WHERE ul.MapIndex = @i_MapIndex AND ul.CharacterUID = c.UniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nMapIndex, 0, 0);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0205, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt,"atum_loadWarpableUserList_ Failed!!\n", TRUE);
		return FALSE;
	}
	
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &s1CharInfo.CharacterUID1, 0,							&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, s1CharInfo.CharacterName1, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	while(TRUE) 
	{
		util::zero(&s1CharInfo, sizeof(s1CharInfo));
		ret = SQLFetch(hstmt);
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}

		m_mtvectWarpableUserList.pushBackLock(s1CharInfo);
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	
	return TRUE;
}

BOOL CFieldMapProject::SetDeadDefaultMap()
{
	// ¸ðµç ¸ÊÀÇ loadingÀº ³¡³µ´Ù°í °¡Á¤ÇÔ!
	CFieldMapProject *pFieldMapProject = (CFieldMapProject*)m_pMapWorkspace->GetMapProjectByMapIndex(m_DeadDefaultMapIndex);
	if (pFieldMapProject == NULL)
	{
#ifdef _DEBUG
		assert(0);
#endif
		return FALSE;
	}
	else
	{
		m_pDeadDefaultMapProject = pFieldMapProject;
	}

	return TRUE;
}

D3DXVECTOR3 CFieldMapProject::GetRandomWarpPoint(int EventAreaIndex)
{
	D3DXVECTOR3 ret;

	///////////////////////////////////////////////////////////////////////////////
	// Object¿¡¼­ EventIndex·Î ¸ÕÀú °Ë»öÇÑ´Ù
	OBJECTINFOSERVER *pObjIServer = this->GetRandomObjectInfoServerPtrByEventIndex(EventAreaIndex);
	if(pObjIServer)
	{
		ret = pObjIServer->m_vPos;
		return ret;
	}

	///////////////////////////////////////////////////////////////////////////////
	// Object¿¡ EventIndex°¡ ¾øÀ¸¸é ¸ÊÀÇ Å¸ÀÏ EventIndex¸¦ °Ë»öÇÑ´Ù
	map<int, vector<int>*>::iterator itr = m_WarpAreaMap.find(EventAreaIndex);
	if (itr == m_WarpAreaMap.end()){
		//DBGOUT("NO SUCH EVENT AREA INDEX(%s): %d\r\n", m_strProjectName, EventAreaIndex);
		g_pGlobal->WriteSystemLogEX(FALSE, "[ERROR]: CFieldMapProject::GetRandomWarpPoint_, NO SUCH EVENT AREA INDEX(%4d): %d\r\n", m_nMapIndex, EventAreaIndex);
		//assert(0);

		ret = D3DXVECTOR3(m_fSizeMap_X/2, 0.0f, m_fSizeMap_Z/2);
	}
	else
	{
		vector<int>* pVec = itr->second;
		int randIndex = GetTickCount() % (pVec->size());
		ret = GetTileCenterPosition((*pVec)[randIndex]);
	}

	ret.y = GetMapHeightIncludeWater(&ret) + 300.0f;
	return ret;
}

void CFieldMapProject::GetRandomPositionInRadius(D3DXVECTOR3 *In, D3DXVECTOR3 *Out, float Radius)
{
	if ( (int)(Radius) == 0 )
	{
		Out->x = In->x;
		Out->y = In->y;
		Out->z = In->z;
		return;
	}

	float r = RANDF(0, Radius);		// ¼Ò¼öÁ¡ ÀÌÇÏ 2ÀÚ¸®±îÁö °è»ê
	double theta = RANDF(0, PI);	// ¼Ò¼öÁ¡ ÀÌÇÏ 4ÀÚ¸®±îÁö °è»ê
	Out->x = (float)(In->x + r*cos(theta));
	Out->y = In->y;
	Out->z = (float)(In->z + r*sin(theta));

	if (!CheckCharacterPosition(Out, FALSE))
		
		GetRandomPositionInRadius(In, Out, Radius/2);
}


EVENTINFO *CFieldMapProject::GetEventInfoByObjectTypeAndDistance(DWORD i_dwObjType, D3DXVECTOR3 *i_pVec3Position)
{
	vectorObjectInfoServer::iterator it(m_vectorObjectInfo.begin());
	while(it != m_vectorObjectInfo.end())
	{
		if(i_dwObjType == it->m_dwObjType)
		{			
			if(D3DXVec3Length(&(*i_pVec3Position - it->m_vPos)) < 10.0f)
			{
				return &it->m_EventInfo;
			}
		}
		it++;
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		µ¿ÀÏÇÑ EventIndex¸¦ °¡Áø °ÍÀÌ ¿©·¯°³ ÂïÈú¼ö ÀÖÀ¸¸ç °°Àº°ÍÀÌ ÀÖÀ» ¶§´Â ·£´ýÀ¸·Î ¼³Á¤µÈ´Ù
/// \author		cmkwon
/// \date		2004-11-30 ~ 2004-11-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
OBJECTINFOSERVER *CFieldMapProject::GetRandomObjectInfoServerPtrByEventIndex(short i_sEventIndex)
{
	vectorObjectInfoServerPtr tmVectorObjPtr;

	tmVectorObjPtr.reserve(5);

	for (auto& info : m_vectorObjectInfo)
		
		if (i_sEventIndex == info.m_EventInfo.m_EventwParam1) tmVectorObjPtr.push_back(&info);

	if (tmVectorObjPtr.empty()) return nullptr;

	int i = RANDI(0, tmVectorObjPtr.size() - 1);

	return tmVectorObjPtr[i];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			EVENTINFO *CFieldMapProject::GetEventInfoByMinimumDistance(EventType_t i_eventTy, D3DXVECTOR3 *i_pVec3Position)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-12 ~ 2005-12-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
EVENTINFO *CFieldMapProject::GetEventInfoByMinimumDistance(EventType_t i_eventTy, D3DXVECTOR3 *i_pVec3Position)
{
	float fCurDistance	= 0.0f;
	EVENTINFO *pRetEv	= NULL;
	vectorObjectInfoServer::iterator it(m_vectorObjectInfo.begin());
	for(; it != m_vectorObjectInfo.end(); it++)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2005-12-12 by cmkwon, Â÷´Â ÀÌº¥Æ® Å¸ÀÔÀÌ °°°Å³ª EVENT_TYPE_WARP_TARGETÀÌ¸é EVENT_TYPE_WARPµµ µ¿ÀÏÇÏ°Ô Ã£´Â´Ù
		if(i_eventTy == it->m_EventInfo.m_bEventType
			|| (EVENT_TYPE_WARP_TARGET == i_eventTy && EVENT_TYPE_WARP == it->m_EventInfo.m_bEventType))
		{
			if(NULL == pRetEv)
			{
				pRetEv			= &it->m_EventInfo;
				fCurDistance	= D3DXVec3Length(&(*i_pVec3Position - it->m_vPos));
			}
			else if(fCurDistance > D3DXVec3Length(&(*i_pVec3Position - it->m_vPos)))
			{
				pRetEv			= &it->m_EventInfo;
				fCurDistance	= D3DXVec3Length(&(*i_pVec3Position - it->m_vPos));
			}
		}
	}

	return pRetEv;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldMapProject::GetRandomWarpTargetEventIndex(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-02 ~ 2006-08-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldMapProject::GetRandomWarpTargetEventIndex(void)
{
	vectorObjectInfoServerPtr tmVectorObjPtr;

	tmVectorObjPtr.reserve(5);

	for (auto& info : m_vectorObjectInfo)
	{
		if (EVENT_TYPE_WARP_TARGET == info.m_EventInfo.m_bEventType || EVENT_TYPE_WARP == info.m_EventInfo.m_bEventType)
			
			tmVectorObjPtr.push_back(&info);
	}

	if (tmVectorObjPtr.empty()) return ~0;

	auto nIdx = m_nNextWarpTargetEventIndex++;	

	if (!util::in_range(0, nIdx, tmVectorObjPtr.size() - 1))
	{
		nIdx							= 0;
		m_nNextWarpTargetEventIndex		= 0;
	}

	return tmVectorObjPtr[nIdx]->m_EventInfo.m_EventwParam1;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			WARP_TARGET_MAP_INFO_4_EXCHANGE *CFieldMapProject::GetWarpInfoByTargetMapIndex(MapIndex_t i_mapIdx)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-29 ~ 2007-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
WARP_TARGET_MAP_INFO_4_EXCHANGE *CFieldMapProject::GetWarpInfoByTargetMapIndex(MapIndex_t i_mapIdx)
{
	vectWARP_TARGET_MAP_INFO_4_EXCHANGE::iterator itr(m_vectCityWarpInfoList.begin());
	for(; itr != m_vectCityWarpInfoList.end(); itr++)
	{
		if(itr->MapIndex == i_mapIdx)
		{
			return &*itr;
		}
	}

	return NULL;
}

CFieldMapProject* CFieldMapProject::GetDeadDefaultMapProject(void)
{
	return m_pDeadDefaultMapProject;
}

MapIndex_t CFieldMapProject::GetDeadDefaultMapIndex(void)
{
	return m_DeadDefaultMapIndex;
}


D3DXVECTOR3 CFieldMapProject::GetDefaultPoint(void)
{
	return GetRandomWarpPoint(m_DefaltWarpTargetIndex);
};


EVENTINFO* CFieldMapProject::GetEventInfoByWarpIndex(int WarpIndex)
{	
	return GetEventInfoByEventAreaIndex(WarpIndex);
}

BOOL CFieldMapProject::ResetAllChannel(void)
{
	if (m_vectorMapChannelPtr.empty())
	{
		return FALSE;
	}

	CFieldMapChannel * tmpFMapChannel = NULL;
	vectorMapChannelPtr::iterator itr(m_vectorMapChannelPtr.begin());
	while (itr != m_vectorMapChannelPtr.end())
	{
		tmpFMapChannel = (CFieldMapChannel*)(*itr);
		tmpFMapChannel->FieldDeleteAllMonster();

		((CFieldIOCP*)m_pMapWorkspace->m_pIOCPServer)->m_pMapTriggerManager->ResetEventByMapChannel(tmpFMapChannel->GetMapChannelIndex());		// 2012-12-17 by hskim, NPC ¼­¹ö Àç½ÃÀÛ½Ã - Æ®¸®°Å ½Ã½ºÅÛ ÃÊ±âÈ­ Àû¿ë

		itr++;
	}

	// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
	m_bNPCServerMonsterCreated = FALSE;
	// end 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

	return TRUE;
}

BOOL CFieldMapProject::FieldOnMonsterCreateW(MSG_FN_MONSTER_CREATE * i_pMonCreate)
{
	m_bNPCServerMonsterCreated = TRUE;		// 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pMonCreate->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterCreate(i_pMonCreate);
}

BOOL CFieldMapProject::FieldOnMonsterDeleteW(MSG_FN_MONSTER_DELETE * i_pMonDelete)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pMonDelete->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterDelete(i_pMonDelete);
}

// start 2011-05-11 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡ - ÇØ´ç ¸ÊÃ¤³Î Æ¯Á¤ ¸ó½ºÅÍ º¯°æ ±â´É Ãß°¡
BOOL CFieldMapProject::FieldOnMonsterChangeOkW(MSG_FN_MONSTER_CHANGE_OK * i_pMonChangeOk)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pMonChangeOk->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterChangeOk(i_pMonChangeOk);
}
// end 2011-05-11 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡ - ÇØ´ç ¸ÊÃ¤³Î Æ¯Á¤ ¸ó½ºÅÍ º¯°æ ±â´É Ãß°¡

BOOL CFieldMapProject::FieldOnMonsterMoveW(MSG_FN_MONSTER_MOVE * i_pMonMove)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pMonMove->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterMove(i_pMonMove);
}

BOOL CFieldMapProject::FieldOnGetCharacterInfoW(MSG_FN_GET_CHARACTER_INFO * i_pGetCInfo)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pGetCInfo->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnGetCharacterInfo(i_pGetCInfo);
}

BOOL CFieldMapProject::FieldOnMissileMoveW(MSG_FN_MISSILE_MOVE * i_pMissileMove)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pMissileMove->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMissileMove(i_pMissileMove);
}

BOOL CFieldMapProject::FieldOnMonsterHPRecoveryW(MSG_FN_MONSTER_HPRECOVERY * i_pHPRecovery)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pHPRecovery->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterHPRecovery(i_pHPRecovery);
}

BOOL CFieldMapProject::FieldOnMonsterHideW(MSG_FN_MONSTER_HIDE * i_pMonHide)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pMonHide->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterHide(i_pMonHide);
}

BOOL CFieldMapProject::FieldOnMonsterShowW(MSG_FN_MONSTER_SHOW * i_pMonShow)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pMonShow->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterShow(i_pMonShow);
}

BOOL CFieldMapProject::FieldOnBattleAttackPrimaryW(MSG_FN_BATTLE_ATTACK_PRIMARY * i_pAttackPri)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pAttackPri->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnBattleAttackPrimary(i_pAttackPri);
}

BOOL CFieldMapProject::FieldOnBattleAttackSecondaryW(MSG_FN_BATTLE_ATTACK_SECONDARY * i_pAttackSec)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pAttackSec->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnBattleAttackSecondary(i_pAttackSec);
}

BOOL CFieldMapProject::FieldOnBattleAttackFindW(MSG_FN_BATTLE_ATTACK_FIND * i_pAttackFind)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pAttackFind->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnBattleAttackFind(i_pAttackFind);
}

BOOL CFieldMapProject::FieldOnMonsterChangeBodyconditionW(MSG_FN_MONSTER_CHANGE_BODYCONDITION * i_pChangeBody)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pChangeBody->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterChangeBodycondition(i_pChangeBody);
}

BOOL CFieldMapProject::FieldOnMonsterSkillUseSkillW(MSG_FN_MONSTER_SKILL_USE_SKILL * i_pUseSkill)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pUseSkill->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterSkillUseSkill(i_pUseSkill);
}

BOOL CFieldMapProject::FieldOnMonsterSkillEndSkillW(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pEndSkill->ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterSkillEndSkill(i_pEndSkill);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::FieldOnMonsterAutoDestroyedW(MSG_FN_MONSTER_AUTO_DESTROYED * i_pAutoDestroyed)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-17 ~ 2006-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::FieldOnMonsterAutoDestroyedW(MSG_FN_MONSTER_AUTO_DESTROYED * i_pAutoDestroyed)
{
// 2007-11-26 by cmkwon, ¸ó½ºÅÍ ÀÚµ¿»èÁ¦ ¸Þ½ÃÁö TCP·Î Àü¼Û(N->F) - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤
//	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pAutoDestroyed->ChannelIndex);
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_pAutoDestroyed->MapChannIdx.ChannelIndex);
	if(NULL == ptmpFMapChannel){		return FALSE;}

	return ptmpFMapChannel->FieldOnMonsterAutoDestroyed(i_pAutoDestroyed);
}



CFieldMonster * CFieldMapProject::GetFieldMonsterW(ChannelIndex_t i_channIdx, ClientIndex_t i_monsterIdx, int i_nTestIdx)
{
	CFieldMapChannel * ptmpFMapChannel = GetFieldMapChannelByIndex(i_channIdx);
	if(NULL == ptmpFMapChannel){		return NULL;}

	return ptmpFMapChannel->GetFieldMonster(i_monsterIdx, i_nTestIdx);
}


////////////////////////////////////////////////////////////////////////////////
//
// ÇÔ ¼ö ÀÌ ¸§  : CMapProject::CanMove
// ¹ÝÈ¯µÇ´Â Çü  : BOOL
// ÇÔ ¼ö ÀÎ ÀÚ  : D3DXVECTOR3 *pPosVector3
// ÇÔ ¼ö ÀÎ ÀÚ  : BOOL bFlagChangeableHeight
// ÇÔ ¼ö ¼³ ¸í  :
//
BOOL CFieldMapProject::CheckCharacterPosition(D3DXVECTOR3 *pPosVector3, BOOL bFlagChangeableHeight)
{
	float		fHeight;

	if(FALSE == IsValidPosition(pPosVector3))
	{	// XÁÂÇ¥¿Í YÁÂÇ¥°¡ mapÀÇ ¹üÀ§¸¦ ¹þ¾î³ª´ÂÁö¸¦ ¸ÕÀú È®ÀÎÇÑ´Ù

		return FALSE;
	}

	fHeight = GetMapHeightIncludeWater(pPosVector3);			// ÁÂÇ¥ÀÇ ¸ÊÀÇ ³ôÀÌ¸¦ ±¸ÇÑ´Ù.
	if(fHeight > pPosVector3->y)
	{
		return FALSE;
	}

	if(FALSE == bFlagChangeableHeight)
	{
		return TRUE;
	}

	if(pPosVector3->y > m_sMaximumAltitude + (m_sMaximumAltitude - m_sMinimumAltitude)/2)
	{	// ³ôÀÌ°¡ Á¦ÇÑ ³ôÀÌ º¸´Ù Å©´Ù

		pPosVector3->y -= 1.0f;
	}
	else if(pPosVector3->y < m_sMinimumAltitude - (m_sMaximumAltitude - m_sMinimumAltitude)/2)
	{
		pPosVector3->y += 1.0f;
	}

	return TRUE;
}


BOOL CFieldMapProject::ChangeWeather(BitFlag16_t weather, BOOL bOnOff)
{
	if (!BelongsToServer())
	{
		return FALSE;
	}

	if (bOnOff)
	{
		m_Weather = weather;
	}
	else
	{
		m_Weather &= ~weather;
	}


	INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_CHANGE_WEATHER, T_FC_EVENT_CHANGE_WEATHER, pChangeWeather, pChangeWeatherBuf);
	pChangeWeather->MapWeather = m_Weather;
	SendMessageToAllInAllMapChannels(pChangeWeatherBuf, MSG_SIZE(MSG_FC_EVENT_CHANGE_WEATHER));

	return TRUE;
}

BOOL CFieldMapProject::Send2NPCServer(BYTE *pData, int nSize)
{
	if(FALSE == m_bNPCServerStart){				return FALSE;}

	mt_auto_lock mtA(&m_mtlockDelaySendBuffer);
	DWORD dwCurTick = timeGetTime();
	if(m_nWritedSize + nSize > SIZE_MAX_PACKET)
	{
// 2006-07-25 by cmkwon
// 		char szTemp[1024];
// 		sprintf(szTemp, "	==> Send2NPCServer 1 %4d, WritedSize(%d) TimeGap(%d)\r\n", m_nMapIndex, m_nWritedSize, dwCurTick-m_dwLastSendedTick);
// 		g_pFieldGlobal->WriteSystemLog(szTemp);
// 
		Send2Peer(m_pbyDelaySendBuffer, m_nWritedSize);
		m_nWritedSize		= 0;
		m_dwLastSendedTick	= dwCurTick;
	}

	memcpy(&m_pbyDelaySendBuffer[m_nWritedSize], pData, nSize);
	m_nWritedSize += nSize;

	if(dwCurTick > m_dwLastSendedTick + 500)
	{
		SendDelayBuffer2NPCFerver();
	}
	return TRUE;

// 2006-07-25 by cmkwon
//	return Send2Peer(pData, nSize);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::SendDelayBuffer2NPCFerver(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::SendDelayBuffer2NPCFerver(void)
{
	mt_auto_lock mtA(&m_mtlockDelaySendBuffer);
	if(m_nWritedSize <= 0)
	{
		return TRUE;
	}

// 2006-07-25 by cmkwon
// 	char szTemp[1024];
// 	sprintf(szTemp, "	==> SendDelayBuffer2NPCFerver 2 %4d, WritedSize(%d) TimeGap(%d)\r\n", m_nMapIndex, m_nWritedSize, timeGetTime()-m_dwLastSendedTick);
// 	g_pFieldGlobal->WriteSystemLog(szTemp);

	int ntmSize			= m_nWritedSize;
	m_nWritedSize		= 0;
	m_dwLastSendedTick	= timeGetTime();
	return Send2Peer(m_pbyDelaySendBuffer, ntmSize);	
}

// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - SendMessageToAllInAllMapChannels, ÀÎÀÚÃß°¡(i_nStartCityMapIdx)
void CFieldMapProject::SendMessageToAllInAllMapChannels(BYTE *buffer, int size, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/, MapIndex_t i_nStartCityMapIdx/*=VCN_CITY_MAP_INDEX*/)
{
	vectorMapChannelPtr::iterator itr = m_vectorMapChannelPtr.begin();

	while(itr != m_vectorMapChannelPtr.end())
	{
		CFieldMapChannel *pFieldChannel = (CFieldMapChannel*)(*itr);
		pFieldChannel->SendMessageToAllInChannel(buffer, size, i_byInfluenceMask, i_nStartCityMapIdx);

		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMapProject::SendSummonMessageToAllChannels(MSG_FN_ADMIN_SUMMON_MONSTER *i_pSummonMonster)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-17 ~ 2006-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMapProject::SendSummonMessageToAllChannels(MSG_FN_ADMIN_SUMMON_MONSTER *i_pSummonMonster)
{
	INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pSSummon, SendBuf);
	*pSSummon	= *i_pSummonMonster;
	CFieldMapChannel * tmpFMapChannel = NULL;
	vectorMapChannelPtr::iterator itr(m_vectorMapChannelPtr.begin());
	while (itr != m_vectorMapChannelPtr.end())
	{
		tmpFMapChannel = (CFieldMapChannel*)(*itr);
		pSSummon->ChannelIndex = tmpFMapChannel->GetMapChannelIndex().ChannelIndex;
		tmpFMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::InitializeMapChannels(int i_nChannel)
/// \brief		ÃÊ±â¿¡ ¸Ê Ã¤³ÎÀ» »ý¼ºÇÏ´Â ºÎºÐ
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		i_nChannel [in] »ý¼ºÇÒ Ã¤³ÎÀÇ ¼ö
/// \return
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::InitializeMapChannels(int i_nChannel)
{
	for (int i = 0; i < i_nChannel; i++)
	{
		CFieldMapChannel *pNewChannel
			= new CFieldMapChannel((CFieldMapWorkspace*)m_pMapWorkspace, this, m_vectorMapChannelPtr.size());

		pNewChannel->InitMapChannel();

		//
		// check: MapChannel µµÀÔÁß, 20040324, kelovon, ¿©±â ÇÊ¿äÇÏ´Ù¸é ÃÊ±âÈ­ °úÁ¤ ±¸ÇöÇÏ±â
		//
		m_vectorMapChannelPtr.push_back(pNewChannel);

		// MAP_INFLUENCE_ARENA
		if(i > 1
			&& i >= i_nChannel/2
			&& FALSE == IS_MAP_INFLUENCE_EVENT_AREA(pNewChannel->GetMapInfluenceTypeW())	// 2009-12-15 by cmkwon, ÀÌº¥Æ®¸ÊÀº ½ÃÀÛ½Ã ¸ðµç Ã¤³Î ¿ÀÇÂ - 
			&& !IS_MAP_INFLUENCE_ARENA(pNewChannel->GetMapInfluenceTypeW())			// 2007-05-07 by dhjin, ¾Æ·¹³ª ¸ÊÀº ¸ðµç Ã¤³Î »ý¼º
			&& !IS_MAP_INFLUENCE_INFINITY(pNewChannel->GetMapInfluenceTypeW()))		// 2009-09-09 ~ 2010-01-29 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ´ÏÆ¼ ¸Êµµ ´Ù ¿­°Ô ¼öÁ¤
		{// 2006-06-13 by cmkwon, ¼³Á¤µÈ Ã¤³ÎÀÇ ¹ÝÀÌ ÀÚµ¿À¸·Î ¿ÀÇÂµÇµµ·Ï ¼³Á¤

			pNewChannel->SetChannelState(FALSE);
		}

		// start 2011-10-10 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - È­»êÀç / ¸ð·¡ ÆøÇ³
		if( TRUE == ((CFieldMapWorkspace*)m_pMapWorkspace)->IsTriggerMapBuff(m_nMapIndex) )
		{
			pNewChannel->InitTriggerMapBuffManager();
		}
		// end 2011-10-10 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - È­»êÀç / ¸ð·¡ ÆøÇ³
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::IncreaseMapChannel(int i_nChannelToAdd)
/// \brief		»õ·Î¿î CFieldMapChannelÀ» ÁöÁ¤µÈ °³¼ö¸¸Å­ »ý¼ºÇÑ´Ù
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		i_nChannelToAdd [in] »õ·Î »ý¼ºÇÒ Ã¤³ÎÀÇ °³¼ö
/// \return		BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::IncreaseMapChannels(int i_nChannelToAdd)
{
	for (int i = 0; i < i_nChannelToAdd; i++)
	{
		CFieldMapChannel *pNewChannel
			= new CFieldMapChannel((CFieldMapWorkspace*)m_pMapWorkspace, this, m_vectorMapChannelPtr.size());

		pNewChannel->InitMapChannel();

		//
		// check: MapChannel µµÀÔÁß, 20040324, kelovon, ¿©±â ÇÊ¿äÇÏ´Ù¸é ÃÊ±âÈ­ °úÁ¤ ±¸ÇöÇÏ±â
		//

		m_vectorMapChannelPtr.push_back(pNewChannel);
		m_nCreateChannelCounts++;

		// start 2011-10-10 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - È­»êÀç / ¸ð·¡ ÆøÇ³
		if( TRUE == ((CFieldMapWorkspace*)m_pMapWorkspace)->IsTriggerMapBuff(m_nMapIndex) )
		{
			pNewChannel->InitTriggerMapBuffManager();
		}
		// end 2011-10-10 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - È­»êÀç / ¸ð·¡ ÆøÇ³


		// NPC¿¡ INCREASE_MAP_CHANNEL Àü¼Û
		//ASSERT_NOT_IMPLEMENTED_YET();
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::CleanMapChannels()
/// \brief
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param
/// \return
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::CleanMapChannels()
{
	// check: MapChannel µµÀÔÁß, 20040324, kelovon, ±¸ÇöÇÏ±â


	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CFieldMapChannel *GetFieldMapChannelByIndex(int i_nIndex)
/// \brief		ÇØ´ç indexÀÇ FieldMapChannelÀ» ¹ÝÈ¯
/// \author		kelovon
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///
/// \param		i_nIndex [i]
/// \return		CFieldMapChannel*
///////////////////////////////////////////////////////////////////////////////
CFieldMapChannel *CFieldMapProject::GetFieldMapChannelByIndex(int i_nIndex, BOOL i_bOnlyEnabledChannel/*=FALSE*/)
{
	return (CFieldMapChannel*)GetMapChannelByIndex(i_nIndex, i_bOnlyEnabledChannel);
}

CFieldMapChannel *CFieldMapProject::GetRandomFieldMapChannel(BOOL i_bOnlyEnabledChannel)
{
	return (CFieldMapChannel*)CMapProject::GetRandomMapChannel(i_bOnlyEnabledChannel);
}

CFieldMapChannel *CFieldMapProject::GetFirstFieldMapChannel(BOOL i_bOnlyEnabledChannel)
{
	return (CFieldMapChannel*)CMapProject::GetFirstMapChannel(i_bOnlyEnabledChannel);
}

// 2004-12-07 by cmkwon, Ã¤³Î ½Ç½Ã°£ Áõ°¡°¡ ¾ø¾î ÇÊ¿ä¾ø´Â ÇÔ¼ö
//CFieldMapChannel *CFieldMapProject::GetLastFieldMapChannel(BOOL i_bOnlyEnabledChannel)
//{
//	return (CFieldMapChannel*)CMapProject::GetLastMapChannel(i_bOnlyEnabledChannel);
//}

void CFieldMapProject::MakeMsg_AllChannelState(int *o_pChannelCounts, MSG_FN_CONNECT_SET_CHANNEL_STATE *o_pChannelState)
{
	*o_pChannelCounts = m_vectorMapChannelPtr.size();
	for(int i=0; i < m_vectorMapChannelPtr.size(); i++)
	{
		o_pChannelState[i].MapChannelIndex	= m_vectorMapChannelPtr[i]->GetMapChannelIndex();
		o_pChannelState[i].EnableChannel	= m_vectorMapChannelPtr[i]->IsEnabled();
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMapProject::OnDoMinutelyWorkFieldMapProject(void)
/// \brief		1ºÐ¿¡ ÇÑ¹ø¾¿ È£ÃâµÇ´Â ÇÔ¼ö, ÃÖ»óÀ§´Â CFieldTickManagerÀÓ
/// \author		cmkwon
/// \date		2004-12-07 ~ 2004-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMapProject::OnDoMinutelyWorkFieldMapProject(void)
{
	int nEnableChannelCounts		= 0;
	int nEnableChannelUserCounts	= 0;
	for(int i=0; i < m_vectorMapChannelPtr.size(); i++)
	{
		CFieldMapChannel *pFChannel = this->GetFieldMapChannelByIndex(i);
		if(pFChannel)
		{
			if(FALSE == pFChannel->IsEnabled())
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2004-12-07 by cmkwon, ÇâÈÄ ¼öÁ¤ ÇØ¾ßÇÔ
				char *szNotice1 = STRMSG_S_F2NOTIFY_0138;
				pFChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szNotice1);
				char *szNotice2 = STRMSG_S_F2NOTIFY_0139;
				pFChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szNotice2);
			}
			else
			{
				nEnableChannelCounts++;
				nEnableChannelUserCounts += pFChannel->GetNumClients();
			}

			// start 2012-03-05 by hskim, µå¶ø ¾ÆÀÌÅÛ ÀÏÁ¤ ½Ã°£ÈÄ »èÁ¦ - ¾ÆÀÌÅÛÀÌ »ý¼ºµÈ ½Ã°£
			pFChannel->OnDoMinutelyWorkFieldMapChannel();
			// end 2012-03-05 by hskim, µå¶ø ¾ÆÀÌÅÛ ÀÏÁ¤ ½Ã°£ÈÄ »èÁ¦ - ¾ÆÀÌÅÛÀÌ »ý¼ºµÈ ½Ã°£
		}
	}

	if(nEnableChannelCounts < m_vectorMapChannelPtr.size()
		&& nEnableChannelUserCounts*100/(nEnableChannelCounts*this->m_dwMaxUserCounts) > 90)
	{
		for(int i=0; i < m_vectorMapChannelPtr.size(); i++)
		{
			CFieldMapChannel *pFChannel = this->GetFieldMapChannelByIndex(i);
			if(pFChannel 
				&& FALSE == pFChannel->IsEnabled())
			{
				pFChannel->SetChannelState(TRUE);

				INIT_MSG_WITH_BUFFER(MSG_FN_CONNECT_SET_CHANNEL_STATE, T_FN_CONNECT_SET_CHANNEL_STATE, pFNSetState, SendBuf);
				pFNSetState->MapChannelIndex	= pFChannel->GetMapChannelIndex();
				pFNSetState->EnableChannel		= pFChannel->IsEnabled();
				this->Send2NPCServer(SendBuf, MSG_SIZE(MSG_FN_CONNECT_SET_CHANNEL_STATE));
				break;
			}
		}		
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMapProject::OnDoSecondlyWorkFieldMapProject(ATUM_DATE_TIME *pDateTime)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMapProject::OnDoSecondlyWorkFieldMapProject(ATUM_DATE_TIME *pDateTime)
{
	if(0 == pDateTime->Second%2)
	{
		this->SendDelayBuffer2NPCFerver();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::IsMultiMapChannel(BYTE i_byCharInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-06-02 ~ 2006-06-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::IsMultiMapChannel(BYTE i_byCharInflTy)
{
	int nMapChannelCounts = this->GetNumberOfMapChannels(TRUE);
	if(1 >= nMapChannelCounts)
	{
		return FALSE;
	}

// 2007-05-22 by cmkwon, ÀÏ¹Ý¼¼·Âµµ½Ã¸Ê(¾ÆÄ«µ¥¹Ì¸Ê) ±¸ÇöÀ¸·Î ¼öÁ¤
//	if(FALSE == this->IsConflictAreaMap()
//		&& MAP_INFLUENCE_NEUTRALITY != this->GetMapInfluenceType()		// 2007-02-05 by cmkwon, Áß¸³Áö¿ªµµ ¸ÊÃ¤³Î °¡´ÉÇÔ
//		&& FALSE == IS_SAME_CHARACTER_MAP_INFLUENCE(i_byCharInflTy, this->GetMapInfluenceType()))
//	{
//		return FALSE;
//	}
//
//	return TRUE;

	if(this->IsConflictAreaMap()
		|| MAP_INFLUENCE_NEUTRALITY == this->GetMapInfluenceType()
// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - CFieldMapProject::IsMultiMapChannel#, 
//		|| IS_SAME_CHARACTER_8_MAP_INFLUENCE(i_byCharInflTy, this->GetMapInfluenceType())
		|| CAtumSJ::IsSameCharacterInfluence8MapInfluence(i_byCharInflTy, this->GetMapInfluenceType())		// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - 
		|| IS_MAP_INFLUENCE_EVENT_AREA(this->GetMapInfluenceType())		// 2009-07-28 by cmkwon, (ÀÏº»¿äÃ»)ÀÌº¥Æ®¸Êµµ ´ÙÁß Ã¤³Î ¼±ÅÃ °¡´É - 
		|| COMPARE_INFLUENCE(i_byCharInflTy, INFLUENCE_TYPE_NORMAL))
	{// 2007-05-22 by cmkwon, ºÐÀïÁö¿ª, Áß¸³Áö¿ª, ¸Ê°ú °°Àº¼¼·Â, ÀÏ¹Ý¼¼·ÂÀº ´Ù¸¥Ã¤³Î ¼±ÅÃ°¡´É
		return TRUE;
	}

	return FALSE;
}

void CFieldMapProject::SetCityWarFieldMapProject(CCityWar *i_pCityWar)
{
	///////////////////////////////////////////////////////////////////////////////
	// »óÁ¡ÀÌ µµ½ÃÁ¡·ÉÀüÀÇ µµ½Ã¿¡ ¼ÒÀ¯µÇ´Â °æ¿ì ¼³Á¤µÊ
	ez_map<UINT, BUILDINGNPC*>::iterator itr_buildingnpc(m_mapBuilding2Building.begin());
	for(; itr_buildingnpc != m_mapBuilding2Building.end(); itr_buildingnpc++)
	{
		if(itr_buildingnpc->second->OwnerCityMapIndex == i_pCityWar->CityMapIndex)
		{
			m_pRefCityWar2 = i_pCityWar;
			break;
		}
	}

	if(NULL == m_pRefCityWar2)
	{// µµ½ÃÁ¡·ÉÀü°ü·Ã »óÁ¡ÀÌ ¾ø´Ù
		
		if(m_nMapIndex != i_pCityWar->MapIndex)
		{
			return;
		}

		///////////////////////////////////////////////////////////////////////////////
		// µµ½ÃÁ¡·ÉÀüÀÌ ¹ß»ýÇÏ´Â ¸ÊÀÏ¶§
		m_pRefCityWar2 = i_pCityWar;
	}

	for(int i=0; i < m_vectorMapChannelPtr.size(); i++)
	{
		((CFieldMapChannel*)m_vectorMapChannelPtr[i])->SetCityWarFieldMapChannel(i_pCityWar);
	}
}

BOOL CFieldMapProject::IsCityWarStarted(void)
{
	if(NULL == m_pRefCityWar2)
	{
		return FALSE;
	}

	if(FALSE == IS_CITYWAR_MAP_INDEX(this->m_nMapIndex))
	{
		return FALSE;
	}

	if(CITYWAR_STATE_STARTED != m_pRefCityWar2->GetCityWarState())
	{
		return FALSE;
	}

	return TRUE;
}

// 2005-12-28 by cmkwon, »ç¿ëÇÏÁö ¾ÊÀ½
//BOOL CFieldMapProject::IsCheckQuestForWarp(CFieldIOCPSocket *i_pFISock)
//{
//	if(0 == this->m_nQuestIndexForWarp)
//	{
//		return TRUE;
//	}
//
//	if(IS_SAME_CHARACTER_MAP_INFLUENCE(i_pFISock->GetCharacter()->InfluenceType, this->GetMapInfluenceType()))
//	{// 2005-12-28 by cmkwon, °°Àº ¼¼·Â¸¸ Äù½ºÆ® ÀÎµ¦½º¸¦ Ã¼Å©ÇÑ´Ù.
//		if(FALSE == i_pFISock->IsCheckQuestComplited(this->m_nQuestIndexForWarp))
//		{
//			return FALSE;
//		}
//	}
//
//	return TRUE;
//}

BYTE CFieldMapProject::GetCityWarTeamTypeByGuildUID(UID32_t i_guildUID)
{
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_guildUID)
		|| NULL == m_pRefCityWar2
		|| FALSE == IS_CITYWAR_MAP_INDEX(this->m_nMapIndex))
	{
		return CITYWAR_TEAM_TYPE_NORMAL;
	}

	return m_pRefCityWar2->GetCityWarTeamType(i_guildUID);
}


struct Sfind_if_S_S1_CHARACTER_INFO_BY_CharacterUID1
{
	Sfind_if_S_S1_CHARACTER_INFO_BY_CharacterUID1(UID32_t i_charUID): m_CharacterUID1(i_charUID){};
	bool operator()(const S_S1_CHARACTER_INFO s1CharInfo)
	{
		return s1CharInfo.CharacterUID1 == m_CharacterUID1;
	}
	UID32_t		m_CharacterUID1;
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			S_S1_CHARACTER_INFO *CFieldMapProject::FindInWarpableUserList(UID32_t i_charUID)
/// \brief		// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - CFieldMapProject¿¡ ¸â¹ö ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
S_S1_CHARACTER_INFO *CFieldMapProject::FindInWarpableUserList(UID32_t i_charUID)
{
	if(m_mtvectWarpableUserList.empty()){				return NULL;}

	mt_auto_lock mtA(&m_mtvectWarpableUserList);

	mtvectS_S1_CHARACTER_INFO::iterator itr = find_if(m_mtvectWarpableUserList.begin(), m_mtvectWarpableUserList.end(),
														Sfind_if_S_S1_CHARACTER_INFO_BY_CharacterUID1(i_charUID));
	if(itr == m_mtvectWarpableUserList.end())
	{
		return NULL;
	}
	
	return &*itr;
}

struct Sfind_if_S_S1_CHARACTER_INFO_BY_CharacterName1
{
	Sfind_if_S_S1_CHARACTER_INFO_BY_CharacterName1(char *i_charName)
	{
		util::strncpy(m_CharacterName1, i_charName, SIZE_MAX_CHARACTER_NAME);
	}

	bool operator()(const S_S1_CHARACTER_INFO s1CharInfo)
	{
		return 0 == strnicmp(s1CharInfo.CharacterName1, m_CharacterName1, SIZE_MAX_CHARACTER_NAME);
	}
	char		m_CharacterName1[SIZE_MAX_CHARACTER_NAME];
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::FindInWarpableUserListByCharacterName(S_S1_CHARACTER_INFO *o_ps1CharInfo, char *i_szCharName)
/// \brief		// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - CFieldMapProject¿¡ ¸â¹ö ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::FindInWarpableUserListByCharacterName(S_S1_CHARACTER_INFO *o_ps1CharInfo, char *i_szCharName)
{
	if(m_mtvectWarpableUserList.empty()){				return FALSE;}
	
	mt_auto_lock mtA(&m_mtvectWarpableUserList);

	mtvectS_S1_CHARACTER_INFO::iterator itr = find_if(m_mtvectWarpableUserList.begin(), m_mtvectWarpableUserList.end(),
														Sfind_if_S_S1_CHARACTER_INFO_BY_CharacterName1(i_szCharName));
	if(itr == m_mtvectWarpableUserList.end())
	{
		return FALSE;
	}
	
	*o_ps1CharInfo	= *itr;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::InsertWarpableUserList(UID32_t i_charUID, char *i_szCharName)
/// \brief		// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - CFieldMapProject¿¡ ¸â¹ö ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-08-31 ~ 2007-08-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::InsertWarpableUserList(UID32_t i_charUID, char *i_szCharName)
{
	mt_auto_lock mtA(&m_mtvectWarpableUserList);
	if(SIZE_MAX_CONFERENCEROOM_USER <= m_mtvectWarpableUserList.size())
	{// ÃÖ´ë ÀÎ¿ø¼ö Ã¼Å©
		return FALSE;
	}

	if(NULL != FindInWarpableUserList(i_charUID))
	{// ÀÌ¹Ì Á¸Àç ÇÑ´Ù.
		return FALSE;
	}

	S_S1_CHARACTER_INFO s1CharInfo;		util::zero(&s1CharInfo, sizeof(s1CharInfo));
	s1CharInfo.CharacterUID1	= i_charUID;
	util::strncpy(s1CharInfo.CharacterName1, i_szCharName, SIZE_MAX_CHARACTER_NAME);
	m_mtvectWarpableUserList.pushBackLock(s1CharInfo);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::PopInWarpableUserListByCharacterName(S_S1_CHARACTER_INFO *o_ps1CharInfo, char *i_szCharName)
/// \brief		// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - CFieldMapProject¿¡ ¸â¹ö ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::PopInWarpableUserListByCharacterName(S_S1_CHARACTER_INFO *o_ps1CharInfo, char *i_szCharName)
{
	if(m_mtvectWarpableUserList.empty()){				return FALSE;}
	
	mt_auto_lock mtA(&m_mtvectWarpableUserList);

	mtvectS_S1_CHARACTER_INFO::iterator itr = find_if(m_mtvectWarpableUserList.begin(), m_mtvectWarpableUserList.end(),
														Sfind_if_S_S1_CHARACTER_INFO_BY_CharacterName1(i_szCharName));
	if(itr == m_mtvectWarpableUserList.end())
	{
		return FALSE;
	}
	
	*o_ps1CharInfo	= *itr;
	m_mtvectWarpableUserList.erase(itr);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::IsWarpableUser(UID32_t i_charUID, USHORT i_usRace)
/// \brief		// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - CFieldMapProject¿¡ ¸â¹ö ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::IsWarpableUser(UID32_t i_charUID, USHORT i_usRace)
{
	if(FALSE == IS_CONFERENCEROOM_MAP_INDEX(m_nMapIndex))
	{// 2007-08-30 by cmkwon, È¸ÀÇ·ëÀÌ ¾Æ´Ï¸é Ã¼Å©°¡ ÇÊ¿ä ¾ø´Ù
		return TRUE;
	}

	// 2007-10-06 by cmkwon, ºÎÁöµµÀÚ 2¸íÀÇ È£ÄªÀ» ´Ù¸£°Ô ¼³Á¤ -
	//if(COMPARE_RACE(i_usRace, RACE_INFLUENCE_LEADER|RACE_INFLUENCE_SUBLEADER))
	if(COMPARE_RACE(i_usRace, RACE_INFLUENCE_LEADER|RACE_INFLUENCE_SUBLEADER_MASK))
	{// 2007-08-30 by cmkwon, ÁöµµÀÚ/ºÎÁöµµÀÚ´Â ±âº»À¸·Î Çã¿ë
		return TRUE;
	}
	
	if(NULL == this->FindInWarpableUserList(i_charUID))
	{// 2007-08-30 by cmkwon, Çã¿ë ¸®½ºÆ®¿¡ Á¸ÀçÇÏÁö ¾Ê´Â´Ù.
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMapProject::SendWarpableUserList(CFieldIOCPSocket *i_pFISoc)
/// \brief		// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - CFieldMapProject¿¡ ¸â¹ö ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::SendWarpableUserList(CFieldIOCPSocket *i_pFISoc)
{
	if(NULL == i_pFISoc
		|| FALSE == i_pFISoc->IsValidCharacter(FALSE))
	{
		return FALSE;
	}

	mt_auto_lock mtA(&m_mtvectWarpableUserList);

	int nCnts = m_mtvectWarpableUserList.size();
	i_pFISoc->SendString128(STRING_128_USER_NOTICE, STRMSG_070830_0003, nCnts);
	if(0 >= nCnts)
	{
		return TRUE;
	}

	for(int i=0; i < nCnts; i++)
	{
		i_pFISoc->SendString128(STRING_128_USER_NOTICE, STRMSG_070830_0011, m_mtvectWarpableUserList[i].CharacterName1);
	}	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CFieldMapProject::GetConferenceRoomMapIndex(BYTE i_byCharInflTy)
/// \brief		// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - CFieldMapProject¿¡ ¸â¹ö ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-08-30 ~ 2007-08-30
/// \warning	static function
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CFieldMapProject::GetConferenceRoomMapIndex(BYTE i_byCharInflTy)
{
	if(IS_VCN_INFLUENCE_TYPE(i_byCharInflTy))
	{
		return VCN_CONFERENCEROOM_MAP_INDEX;
	}

	if(IS_ANI_INFLUENCE_TYPE(i_byCharInflTy))
	{
		return ANI_CONFERENCEROOM_MAP_INDEX;
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, Ä³½¬ ¾ÆÀÌÅÛ ÇÑÁ¤ÆÇ¸Å ½Ã½ºÅÛ ±¸Çö - 
/// \author		cmkwon
/// \date		2010-02-02 ~ 2010-02-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMapProject::LimitedEUpdate(UINT i_nBuildIdx, vectITEM_W_COUNT *i_pLimitedItemList)
{
	if(CASH_SHOP_BUILDING_INDEX != i_nBuildIdx)
	{
		return FALSE;
	}

	map<UINT, CShopInfo*>::iterator itr = this->m_mapShopInfo.find(i_nBuildIdx);
	if (itr == this->m_mapShopInfo.end())
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-02 by cmkwon, ÇÑÁ¤ÆÇ¸Å ¾ÆÀÌÅÛ ¸®½ºÆ® Á¤º¸ ¾÷µ¥ÀÌÆ®
	CShopInfo *pShopInfo = itr->second;
	vectITEM_W_COUNT::iterator liItr(i_pLimitedItemList->begin());
	for(; liItr != i_pLimitedItemList->end(); liItr++)
	{
		ITEM_W_COUNT *pItmCnt	= &*liItr;
		SHOP_ITEM *pShopItem	= pShopInfo->GetSHOP_ITEMPtr(pItmCnt->ItemNum);
		if(NULL == pShopItem)
		{
			return FALSE;
		}
		pShopItem->RemainCountForLimitedEdition		= pItmCnt->Count;		// 2010-02-02 by cmkwon, º¤ÅÍ¿¡ °³¼ö ¾÷µ¥ÀÌÆ®
		
		pShopItem	= pShopInfo->GetSHOP_ITEMPtrFrombuffer(pItmCnt->ItemNum);
		if(NULL == pShopItem)
		{
			return FALSE;
		}
		pShopItem->RemainCountForLimitedEdition		= pItmCnt->Count;		// 2010-02-02 by cmkwon, Àü¼Û¹öÆÛ¿¡ °³¼ö ¾÷µ¥ÀÌÆ®
	}

	return TRUE;
}	

// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
void CFieldMapProject::SetEnterOneTimeLimited(BOOL bEnable)
{
	m_bEnableEnterOneTimeLimited = bEnable;
}

BOOL CFieldMapProject::IsEnableEnterOneTimeLimited()
{
	return m_bEnableEnterOneTimeLimited;
}

void CFieldMapProject::InsertEnterOneTimeLimited(UID32_t CharacterUID)
{
	if( FALSE == m_bEnableEnterOneTimeLimited )
	{
		return ;
	}
	
	mt_auto_lock mtOC(&m_mtVectEnterOneTimeLimited);
	
	m_mtVectEnterOneTimeLimited.push_back(CharacterUID);
}

BOOL CFieldMapProject::IsEnterOneTimeLimited(UID32_t CharacterUID)
{
	if( FALSE == m_bEnableEnterOneTimeLimited )
	{
		return FALSE;
	}

	mt_auto_lock mtOC(&m_mtVectEnterOneTimeLimited);

	for(int i=0; i<m_mtVectEnterOneTimeLimited.size(); i++)
	{
		if( CharacterUID == m_mtVectEnterOneTimeLimited[i] )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFieldMapProject::IsGameStartNotAllowedUser(UID32_t CharacterUID)
{
	// Á¢¼Ó 1È¸ Á¦ÇÑÀÌ ÀÖÀ» °æ¿ì
	// °ÔÀÓ Á¢¼Ó½Ã Á¡¿¡ ÇöÀç ¸Ê¿¡ ÀÔÀå Á¤º¸°¡ ¾ø´Ù¸é FALSE ¹ÝÈ¯

	if( FALSE == m_bEnableEnterOneTimeLimited )
	{
		return FALSE;
	}

	mt_auto_lock mtOC(&m_mtVectEnterOneTimeLimited);
	
	for(int i=0; i<m_mtVectEnterOneTimeLimited.size(); i++)
	{
		if( CharacterUID == m_mtVectEnterOneTimeLimited[i] )
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CFieldMapProject::ResetEnterOneTimeLimited()
{
	mt_auto_lock mtOC(&m_mtVectEnterOneTimeLimited);
	m_mtVectEnterOneTimeLimited.clear();
}

Err_t CFieldMapProject::IsEnableWarpByMapProject(CFieldIOCPSocket *i_pFISoc, BOOL bGameStart /*= FALSE*/)
{
	if(NULL == i_pFISoc	|| FALSE == i_pFISoc->IsValidCharacter(FALSE))
	{
		return ERR_INVALID_CHARACTER;
	}

	if( FALSE == bGameStart )
	{
	if( TRUE == IsEnterOneTimeLimited(i_pFISoc->m_character.CharacterUniqueNumber) )
	{
		return ERR_REQ_WARP_ONE_TIME_LIMITED;
	}

		if( TRUE == IsEnterCondition(i_pFISoc) )
		{
			return ERR_REQ_WARP_NO_QUEST_ITEM;
		}
	}
	else
	{
		if( TRUE == IsGameStartNotAllowedUser(i_pFISoc->m_character.CharacterUniqueNumber) )
		{
			return ERR_REQ_WARP_GAME_START_NOT_ALLOWED_USER;
		}
	}

	if( TRUE == IsEnterGuildLimited(i_pFISoc->m_character.GuildUniqueNumber) )
	{
		return ERR_REQ_WARP_GUILD_NOT_MATCHED;
	}

	if( TRUE == IsEnterCharacterLimited(i_pFISoc->m_character.CharacterUniqueNumber) )
	{
		return ERR_REQ_WARP_GUILD_NOT_MATCHED;
	}

	if( TRUE == IsEnterInfluenceLimited(i_pFISoc->m_character.InfluenceType) )
	{
		return ERR_REQ_WARP_INFLUENCE_NOT_MATCHED;
	}

	if( TRUE == IsEnterMaxiumUserLimited() )
	{
		return ERR_REQ_WARP_MAXIMUM_USER_LIMITED;
	}

	return ERR_NO_ERROR;
}

void CFieldMapProject::SetEnterGuildLimited(BOOL bEnable, UID32_t GuildUID /*= 0*/)
{
	m_bEnableEnterGuildLimited = bEnable;
	m_EnterGuildLimited = GuildUID;
}

BOOL CFieldMapProject::IsEnterGuildLimited(UID32_t GuildUID)
{
	if( FALSE == m_bEnableEnterGuildLimited )
	{
		return FALSE;
	}

	if( m_EnterGuildLimited == GuildUID )
	{
		return FALSE;
	}

	return TRUE;
}

void CFieldMapProject::SetEnterCharacterLimited(BOOL bEnable, UID32_t GuildUID /*= 0*/)
{
	m_bEnableEnterCharacterLimited = bEnable;
	m_EnterCharacterLimited = GuildUID;
}

BOOL CFieldMapProject::IsEnterCharacterLimited(UID32_t CharacterUID)
{
	if( FALSE == m_bEnableEnterCharacterLimited )
	{
		return FALSE;
	}

	if( m_EnterCharacterLimited == CharacterUID )
	{
		return FALSE;
	}

	return TRUE;
}

void CFieldMapProject::SetEnterInfluenceLimited(BOOL bEnable, BYTE InfluenceType /*= 0*/)
{
	m_bEnableEnterInfluenceLimited = bEnable;
	m_EnterInfluenceLimited = InfluenceType;
}

BOOL CFieldMapProject::IsEnterInfluenceLimited(BYTE InfluenceType)
{
	if( FALSE == m_bEnableEnterInfluenceLimited )
	{
		return FALSE;
	}

	if( m_EnterInfluenceLimited == InfluenceType )
	{
		return FALSE;
	}

	return TRUE;
}

void CFieldMapProject::SetEnterMaxiumUserLimited(BOOL bEnable)
{
	m_bEnterMaxiumUserLimited = bEnable;
}

BOOL CFieldMapProject::IsEnterMaxiumUserLimited()
{
	if( FALSE == m_bEnterMaxiumUserLimited )
	{
		return FALSE;
	}

	if( m_dwMaxUserCounts <= GetCurrentUser() )
	{
		return TRUE;
	}

	return FALSE;
}

INT CFieldMapProject::GetCurrentUser()
{
	INT iCount = 0;

	vectorMapChannelPtr::iterator itr = m_vectorMapChannelPtr.begin();

	while(itr != m_vectorMapChannelPtr.end())
	{
		CFieldMapChannel *pFieldChannel = (CFieldMapChannel*)(*itr);
		if( NULL != pFieldChannel )
		{
			iCount += pFieldChannel->GetNumClients();
		}

		itr++;
	}

	return iCount;
}

BOOL CFieldMapProject::IsEnterCondition(CFieldIOCPSocket *i_pFISoc)
{
	int iMatchCount = 0;

	if( NULL == i_pFISoc || FALSE == i_pFISoc->IsValidCharacter(FALSE) )
	{
		return FALSE;
	}

	if( 0 == m_EntranceCondition.size() )
	{
		return FALSE;
	}

	for(int i=0; i<m_EntranceCondition.size(); i++)
	{
		mt_auto_lock mtAuto(&i_pFISoc->m_ItemManager.m_mapItemGeneral);
		ITEM_GENERAL *pItemG = i_pFISoc->m_ItemManager.GetFirstItemGeneralByItemNum(m_EntranceCondition[i].ItemNum);

		if (NULL != pItemG && NULL != pItemG->ItemInfo)
		{
			if( pItemG->CurrentCount >= m_EntranceCondition[i].RequireCount )
			{
				iMatchCount++;
			}
		}
	}

	if( iMatchCount == m_EntranceCondition.size() )
	{
		return FALSE;
	}

	return TRUE;
}

CShopInfo* CFieldMapProject::GetShopInfoByBuildingNPC(UINT buildingIndex)
{
	map<UINT, CShopInfo*>::iterator itr = m_mapShopInfo.find(buildingIndex);
	if (itr == m_mapShopInfo.end())
		return NULL;
	else
		return itr->second;
}

CShopInfo* CFieldMapProject::GetCashShopInfo()
{
	if (m_CashShopInfo)
	{
		return m_CashShopInfo;
	}
	else
	{
		// Find cash shop info
		m_CashShopInfo = GetShopInfoByBuildingNPC(CASH_SHOP_BUILDING_INDEX);
		return m_CashShopInfo;
	}
}
