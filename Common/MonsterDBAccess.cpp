#include "stdafx.h"
#include "MonsterDBAccess.h"
#include "AtumDBManager.h"
#include "Monster.h"

CMonsterDBAccess::CMonsterDBAccess()
{
	RETCODE ret;

	henv = SQL_NULL_HENV;
	hdbc = SQL_NULL_HDBC;
	hstmt = SQL_NULL_HSTMT;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		CAtumDBHelper::ProcessLogMessagesStatic(SQL_HANDLE_ENV, hstmt, "GetMONSTER() Failed!\n", TRUE);
		return;
	}

	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//	ret = SQLConnect(hdbc, g_pGlobal->GetODBCDSN(), SQL_NTS, g_pGlobal->GetODBCUID(), SQL_NTS, g_pGlobal->GetODBCPASSWORD(), SQL_NTS);
	char szConnectionString[512];
	char szOutConnectionString[512];
	SQLSMALLINT pcb;
#if defined(DB_SERVER_MYSQL)
	sprintf(szConnectionString, "DRIVER={MySQL ODBC 5.1 Driver};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s"
		, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetDBServerDatabaseName());
#else
	sprintf(szConnectionString, "DRIVER={SQL Server};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s"
	        , g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetDBServerDatabaseName());
#endif
	ret = SQLDriverConnect(hdbc, g_pGlobal->GetMainWndHandle(), (SQLCHAR*)szConnectionString, strlen(szConnectionString), (SQLCHAR*)szOutConnectionString, 512, &pcb, SQL_DRIVER_NOPROMPT);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CMonsterDBAccess::CMonsterDBAccess_) !! %s,%d %s %s %s\r\n"
		                                , g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());
		CAtumDBHelper::ProcessLogMessagesStatic(SQL_HANDLE_DBC, hstmt, "GetMONSTER() Failed!\n", TRUE);
		return;
	}

	// prints DB Info
	char DatabaseName[40];
	char DSN[40];
	SQLSMALLINT StrLen;
	SQLGetInfo(hdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
	SQLGetInfo(hdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		CAtumDBHelper::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt, "GetMONSTER() Failed!\n", TRUE);
		return;
	}
}

CMonsterDBAccess::~CMonsterDBAccess()
{
	// cleanup odbc resources
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
#ifndef _DEBUG
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
#endif
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

bool CMonsterDBAccess::MonsterMoveTypeCheck(const MONSTER_INFO* pMon)
{
	switch (pMon->MonsterForm)
	{
	case FORM_FLYING_RIGHT:
	case FORM_SWIMMINGFLYING_RIGHT:
		switch (pMon->MovePattern)
		{
			//case MOVPAT_STOP:						// 전폭형 몬스터는 정지 이동 타입이 안됨
		case MOVPAT_STRAIGHT:
		case MOVPAT_RIGHTANDLEFT:
		case MOVPAT_8RIGHTANDLEFT:
		case MOVPAT_PATROL: // check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_PARTY:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
		case MOVPAT_UPANDDOWN:
		case MOVPAT_OBLIQUE:
			//case MOVPAT_OBLIQUEANDCURVE:			// 전폭형은 구현하지 않음
		case MOVPAT_TELEPORT: // check cmkwon 아직 구현이 안된것, 향후 구현 해야함
		case MOVPAT_FOLLOWPATH: // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - FORM_FLYING_RIGHT / FORM_FLYING_COPTER 만 지원
			return TRUE;
		}
		break;
	case FORM_FLYING_COPTER:
	case FORM_SWIMMINGFLYING_COPTER: //
		switch (pMon->MovePattern)
		{
		case MOVPAT_STOP:
		case MOVPAT_STRAIGHT:
		case MOVPAT_RIGHTANDLEFT:
		case MOVPAT_8RIGHTANDLEFT:
		case MOVPAT_PATROL: // check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_PARTY:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
		case MOVPAT_UPANDDOWN:
		case MOVPAT_OBLIQUE:
		case MOVPAT_OBLIQUEANDCURVE:
		case MOVPAT_TELEPORT: // check cmkwon 아직 구현이 안된것, 향후 구현 해야함
		case MOVPAT_FOLLOWPATH: // 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - FORM_FLYING_RIGHT / FORM_FLYING_COPTER 만 지원
			return TRUE;
		}
		break;
	case FORM_GROUND_MOVE:
		switch (pMon->MovePattern)
		{
		case MOVPAT_STOP:
		case MOVPAT_STRAIGHT:
		case MOVPAT_RIGHTANDLEFT:
		case MOVPAT_8RIGHTANDLEFT:
			//case MOVPAT_PATROL:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_PARTY:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_UPANDDOWN:
			//case MOVPAT_OBLIQUE:
			//case MOVPAT_OBLIQUEANDCURVE:
			//case MOVPAT_TELEPORT:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_FOLLOWPATH:				// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - FORM_FLYING_RIGHT / FORM_FLYING_COPTER 만 지원
			return TRUE;
		}
		break;
	case FORM_FLYINGandGROUND_RIGHT:
		switch (pMon->MovePattern)
		{
			//case MOVPAT_STOP:						// 전폭형 몬스터는 정지 이동 타입이 안됨
		case MOVPAT_STRAIGHT:
		case MOVPAT_RIGHTANDLEFT:
		case MOVPAT_8RIGHTANDLEFT:
			//case MOVPAT_PATROL:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_PARTY:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_UPANDDOWN:
			//case MOVPAT_OBLIQUE:
			//case MOVPAT_OBLIQUEANDCURVE:
			//case MOVPAT_TELEPORT:
			//case MOVPAT_FOLLOWPATH:				// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - FORM_FLYING_RIGHT / FORM_FLYING_COPTER 만 지원
			return TRUE;
		}
		break;
	case FORM_FLYINGandGROUND_COPTER:
		switch (pMon->MovePattern)
		{
		case MOVPAT_STOP:
		case MOVPAT_STRAIGHT:
		case MOVPAT_RIGHTANDLEFT:
		case MOVPAT_8RIGHTANDLEFT:
			//case MOVPAT_PATROL:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_PARTY:					// check cmkwon 아직 구현이 안된것, 향후 구현 해야함
			//case MOVPAT_UPANDDOWN:
			//case MOVPAT_OBLIQUE:
			//case MOVPAT_OBLIQUEANDCURVE:
			//case MOVPAT_TELEPORT:
			//case MOVPAT_FOLLOWPATH:				// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현 - FORM_FLYING_RIGHT / FORM_FLYING_COPTER 만 지원
			return TRUE;
		}
		break;
	case FORM_OBJECT_STOP:
	case FORM_OBJECT_PLANE_ROTATE:
	case FORM_OBJECT_CANNON:
		if (pMon->MovePattern == MOVPAT_STOP) return TRUE;
		break;
	}
	return FALSE;
}

bool CMonsterDBAccess::CheckMPOption(MONSTER_INFO* pMonInfo)
{
	if (COMPARE_MPOPTION_BIT(pMonInfo->MPOption, MPOPTION_BIT_MOVE_PARTY)
		&& COMPARE_MPOPTION_BIT(pMonInfo->MPOption, MPOPTION_BIT_AUTO_DESTROY))
	{
		return FALSE;
	}

	if (COMPARE_MPOPTION_BIT(pMonInfo->MPOption, MPOPTION_BIT_MOVE_PARTY)
		&& 0 == pMonInfo->MPOptionParam2)
	{
		return FALSE;
	}

	if (COMPARE_MPOPTION_BIT(pMonInfo->MPOption, MPOPTION_BIT_AUTO_DESTROY)
		&& 0 == pMonInfo->MPOptionParam1)
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-26 by cmkwon, 인피2차 수정 - 두개의 설정이 둘다 될수는 없다
	if (COMPARE_MPOPTION_BIT(pMonInfo->MPOption, MPOPTION_BIT_KEY_MONSTER)
		&& COMPARE_MPOPTION_BIT(pMonInfo->MPOption, MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR))
	{
		return FALSE;
	}


	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-14 by cmkwon, 인피2차 몬스터 랜텀 타겟 변경 처리 - 유효값(1초 ~ 15초)
	if (pMonInfo->ChangeTarget)
	{
		pMonInfo->ChangeTargetTime = max(1000, pMonInfo->ChangeTargetTime);
		pMonInfo->ChangeTargetTime = min(TICK_MONSTER_TARGET_UNCHANGE_TERM, pMonInfo->ChangeTargetTime);
	}
	return TRUE;
}

BOOL CMonsterDBAccess::GetAllMonsters(ez_map<int, MONSTER_INFO>& mapMonster, ez_map<INT, ITEM*>* pMapItemInfo, MonsterLoadType loadType)
{

	SQLINTEGER cb[CB_COUNT_MONSTER];

	auto ret = SQLExecDirect(hstmt, PROCEDURE_110317_0001, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		CAtumDBHelper::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt, "CMonsterDBAccess::GetAllMonsterMultiTarget() Failed!\n", TRUE);

		return FALSE;
	}


	MONSTER_MULTI_TARGET retMonsterTarget;

	CAtumDBManager::BindColMonsterMultiTarget(hstmt, retMonsterTarget, cb);

	util::zero(retMonsterTarget);


	vectorMonsterMultiTarget vectMonsterMultiTargetinfo;
	vectMonsterMultiTargetinfo.clear();

	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		vectMonsterMultiTargetinfo.push_back(retMonsterTarget);

		util::zero(&retMonsterTarget, sizeof(retMonsterTarget));
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);


	mapMonster.clear();
	

	if (loadType == MONSTER_LOAD_TYPE_SIZE_FOR_SERVER)
	{
		ret = SQLExecDirect(hstmt, PROCEDURE_080827_0063, SQL_NTS);
	}
	else if (loadType == MONSTER_LOAD_TYPE_SIZE_FOR_CLIENT)
	{
		ret = SQLExecDirect(hstmt, PROCEDURE_080827_0062, SQL_NTS);
	}
	else
	{
		assert(0);
	}

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		CAtumDBHelper::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt, "CMonsterDBAccess::GetAllMonsters() Failed!\n", TRUE);
		return FALSE;
	}

	MONSTER_INFO retMonsterInfo;

	CAtumDBManager::BindColMONSTER(hstmt, retMonsterInfo, cb);

	util::zero(retMonsterInfo);

	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			CAtumDBHelper::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt, "CMonsterDBAccess::GetAllMonsters() Failed!\n", TRUE);
			return FALSE;
		}

		auto nItemCount = 0;
		
		for (auto i = 0; i < ARRAY_SIZE_MONSTER_ITEM - 1; i++)
		{
			auto nTmItemNum = INT(retMonsterInfo.ItemInfo[i].pItemInfo);
			retMonsterInfo.ItemInfo[i].pItemInfo = NULL; // ItemNum을 가져오고 NULL로 할당한다.
			if (nTmItemNum)
			{
				if (i < ARRAY_SIZE_MONSTER_ITEM - ARRAY_SIZE_MONSTER_SKILL_ITEM - 1)
				{
					retMonsterInfo.ItemInfo[nItemCount].pItemInfo = pMapItemInfo->findEZ(nTmItemNum);
					if (retMonsterInfo.ItemInfo[nItemCount].pItemInfo)
					{
						nItemCount++;
					}
					else
					{
						char szSystemLog[1024];
						sprintf(szSystemLog, "[Error] CMonsterDBAccess::GetAllMonsters_1 Monster Item error, MonsterName[%20s: %6d] ItemNum[%d]\r\n"
						        , retMonsterInfo.MonsterName, retMonsterInfo.MonsterUnitKind, nTmItemNum);
						g_pGlobal->WriteSystemLog(szSystemLog);
						DBGOUT(szSystemLog);
					}
				}
				else
				{
					retMonsterInfo.ItemInfo[i].pItemInfo = pMapItemInfo->findEZ(nTmItemNum);
					if (!retMonsterInfo.ItemInfo[i].pItemInfo)
					{
						char szSystemLog[1024];
						sprintf(szSystemLog, "[Error] CMonsterDBAccess::GetAllMonsters_1 Monster Item error, MonsterName[%20s: %6d] ItemNum[%d]\r\n"
						        , retMonsterInfo.MonsterName, retMonsterInfo.MonsterUnitKind, nTmItemNum);
						g_pGlobal->WriteSystemLog(szSystemLog);
						DBGOUT(szSystemLog);
					}
				}
			}
		}


		for (auto i = 0; i < ARRAY_SIZE_MONSTER_ITEM; i++) retMonsterInfo.ItemInfo[i].byBodyConArrayIndex = i;
		
	
		if (nItemCount > 1)
		{
			auto lamda = [](MONSTER_ITEM op1, MONSTER_ITEM op2) { return op1.pItemInfo->Charging > op2.pItemInfo->Charging; };

			sort(retMonsterInfo.ItemInfo, retMonsterInfo.ItemInfo + nItemCount, lamda);
		}

		for (auto i = 0; i < ARRAY_SIZE_MONSTER_ITEM; i++) retMonsterInfo.ItemInfo[i].byArrayIndex = i;


		MONSTER_ITEM* pLastItem = NULL;
		for (auto i = 0; i < ARRAY_SIZE_MONSTER_ITEM - 1; i++)
		{
			MONSTER_ITEM* pMonsterItem = &(retMonsterInfo.ItemInfo[i]);
			if (pMonsterItem->pItemInfo)
			{
				if (pMonsterItem->pItemInfo->Charging > 0)
				{
					pMonsterItem->dwUsingPercent = ((NULL == pLastItem) ? 0 : pLastItem->dwUsingPercent) + pMonsterItem->pItemInfo->Charging;
					pLastItem = pMonsterItem;
				}
			}
		}

		if (pLastItem && 100 != pLastItem->dwUsingPercent)
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CMonsterDBAccess::GetAllMonsters_2 Monster Item percent error !!, MonsterName[%20s: %6d] TotalUsingPercent[%d] \r\n"
			                                , retMonsterInfo.MonsterName, retMonsterInfo.MonsterUnitKind, pLastItem->dwUsingPercent);
		}

		// 5. 컨트롤 스킬 아이템 할당
		auto nTmItemNum = INT(retMonsterInfo.ItemInfo[ARRAY_INDEX_MONSTER_SKILL_ITEM].pItemInfo);
		if (nTmItemNum)
		{
			retMonsterInfo.ItemInfo[ARRAY_INDEX_MONSTER_SKILL_ITEM].pItemInfo = pMapItemInfo->findEZ(nTmItemNum);
			if (NULL == retMonsterInfo.ItemInfo[ARRAY_INDEX_MONSTER_SKILL_ITEM].pItemInfo)
			{
				char szSystemLog[1024];
				sprintf(szSystemLog, "[Error] CMonsterDBAccess::GetAllMonsters_1 Monster Item error, MonsterName[%20s: %6d] ItemNum[%d]\r\n"
				        , retMonsterInfo.MonsterName, retMonsterInfo.MonsterUnitKind, nTmItemNum);
				g_pGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
			}
		}

		// 6. 몬스터 체크
		if (MonsterMoveTypeCheck(&retMonsterInfo) == FALSE)
		{
			char szSystemLog[1024];
			sprintf(szSystemLog, "[Error] CMonsterDBAccess::GetAllMonsters_3 MoveTypeCheck error, MonsterName[%20s: %6d] MonsterForm[0x%2X] MovePattern[0x%X]\r\n"
			        , retMonsterInfo.MonsterName, retMonsterInfo.MonsterUnitKind, retMonsterInfo.MonsterForm, retMonsterInfo.MovePattern);
			g_pGlobal->WriteSystemLog(szSystemLog);
			DBGOUT(szSystemLog);
		}

		// 7. MPOption 체크
		if (FALSE == CheckMPOption(&retMonsterInfo))
		{
			char szSystemLog[1024];
			sprintf(szSystemLog, "[Error] CMonsterDBAccess::GetAllMonsters_4 MPOption error, Monster[%20s: %6d] MPOption[0x%llX] MPParam1[%d] MPParam2[%d]\r\n"
			        , retMonsterInfo.MonsterName, retMonsterInfo.MonsterUnitKind, retMonsterInfo.MPOption, retMonsterInfo.MPOptionParam1, retMonsterInfo.MPOptionParam2);
			g_pGlobal->WriteSystemLog(szSystemLog);
			DBGOUT(szSystemLog);
		}

		// check_cmkwon, 다른 곳에 추가해야 함, 20040327
		//		retMonsterInfo.HP = retMonsterInfo.CurrentHP;						// HP를 할당한다.
		retMonsterInfo.Size = retMonsterInfo.Size * retMonsterInfo.ScaleValue; // 서버에서 사용하는 사이즈를 계산하여 설정한다.
		retMonsterInfo.QuickSpeed = retMonsterInfo.QuickSpeed / (1000 / MONSTER_UPDATE_MOVE_TERM_TICK); // DB에는 1초동안의 스피드가 있지만 실제 사용은 200ms동안의 속도를 사용하므로 다시 설정
		retMonsterInfo.TurnAngle = retMonsterInfo.TurnAngle / (1000 / MONSTER_UPDATE_MOVE_TERM_TICK);

		// start 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
		
		for (auto itMT = vectMonsterMultiTargetinfo.begin(); itMT != vectMonsterMultiTargetinfo.end(); ++itMT)
		{
			if (itMT->MonsterIndex == retMonsterInfo.MonsterUnitKind)
			{
				if (itMT->PointIndex < 1 || itMT->PointIndex > ARRAY_SIZE_MONSTER_TARGET)
				{
					server::log(true, "[Error] CMonsterDBAccess::GetAllMonsters_5 MonsterMultiTarget Value Error !!!!!!, MonsterName[%20s: %6d] PointIndex[%2d] Position[%6.2f][%6.2f][%6.2f]\r\n",
						retMonsterInfo.MonsterName, retMonsterInfo.MonsterUnitKind, itMT->PointIndex, itMT->TargetPosition.x, itMT->TargetPosition.y, itMT->TargetPosition.z);

					continue;
				}

				retMonsterInfo.MultiTargetInfo[itMT->PointIndex - 1].Position = itMT->TargetPosition;
			}
		}
		// end 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

		// mapMonster.insert(map<int, MONSTER_INFO>::value_type(retMonsterInfo.MonsterUnitKind, retMonsterInfo));

		// DevX replaced code above with a more sensible solution

		mapMonster[retMonsterInfo.MonsterUnitKind] = retMonsterInfo;

		// initialize to 0x00
		util::zero(retMonsterInfo);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	return TRUE;
}

