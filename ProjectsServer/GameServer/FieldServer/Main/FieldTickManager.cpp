#include "stdafx.h"
#include "FieldTickManager.h"
#include "FieldODBC.h"
#include "FieldGlobal.h"	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¸Å¿ù Ã¹ °áÀç½Ã ¼±¹°Áö±Þ - ·Î±×¸¦ ³²±â±â À§ÇØ ÇØ´õ Æ÷ÇÔ

CFieldTickManager::CFieldTickManager(CFieldIOCP *i_pFieldIOCP, DWORD i_nTickInterval /* = FIELD_DEFAULT_TICK_INTERVAL */)
: CTickManager(i_nTickInterval)
{
	m_pFieldIOCP6 = i_pFieldIOCP;
}

CFieldTickManager::~CFieldTickManager()
{
}

void CFieldTickManager::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
#ifdef _DEBUG
	DBGOUT(STRMSG_S_F2NOTIFY_0141, pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
#endif
	
	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶·Î º¯°æ
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	switch( pTickEvent->TickEventType )
	{
	case SM_TICK_EVENT_SUB_ITEM_EVENT:
		{
			if ( TRUE == g_pFieldGlobal->IsArenaServer() )	// ¾Æ·¹³ª ¼­¹ö¿¡¼± ¼­ºêÅ¸ÀÓ ÀÌº¥Æ® ¾ÆÀÌÅÛÀ» Áö±ÞÇÏÁö ¾Ê´Â´Ù.
			{
				return;
			}
			int nItemEventUID = pTickEvent->nTickEventParam1;
			UID32_t nCharacterUID = pTickEvent->nTickEventParam2;
			int nPlayTimeBySecond = (int)pTickEvent->vpTickEventParam1;
			CFieldIOCPSocket *pSock = m_pFieldIOCP6->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID, TRUE);
			if ( NULL == pSock )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error1] CFieldTickManager::DoTickEvent, CUID(%8d) pSock Is NULL \r\n", nCharacterUID);
				return;
			}

			ATUM_DATE_TIME atCurrentDateTime;
			atCurrentDateTime.SetCurrentDateTime();
			if ( nPlayTimeBySecond > atCurrentDateTime.GetTimeDiffTimeInSeconds(pSock->m_character.LastStartedTime) )
			{
				char timestr[SIZE_MAX_SQL_DATETIME_STRING] = {0,};
				pSock->m_atLoginDate.GetSQLDateTimeString(timestr, SIZE_MAX_SQL_DATETIME_STRING);
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error2] CFieldTickManager::DoTickEvent, CUID(%8d) Re Start User (%s) EventTime(%d)\r\n", nCharacterUID, timestr, nPlayTimeBySecond);
				return;
			}

			SITEM_EVENT_INFO* ItemEventInfo = m_pFieldIOCP6->m_pGiveItemManager->GetItemEventInfoByEventUID(nItemEventUID);
			if ( NULL == ItemEventInfo )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error3] CFieldTickManager::DoTickEvent, CUID(%8d) ItemEevntUID(%4d) ItemEventInfo Is NULL \r\n",nCharacterUID, nItemEventUID);
				return;
			}
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] CFieldTickManager::DoTickEvent, Run TickEvent AUID(%8d) CUID(%8d) ItemEevntUID(%4d) \r\n"
				, pSock->m_character.AccountUniqueNumber, pSock->m_character.CharacterUniqueNumber, nItemEventUID);
			if ( TRUE == m_pFieldIOCP6->ItemEventSubTypeCheck(pSock->m_character.AccountUniqueNumber, pSock->m_character.CharacterUniqueNumber, *ItemEventInfo, ITEM_EVENT_DES_PLAY_TIME) )
			{
				// 2013-04-22 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶ ½Ã½ºÅÛ º¸¾È
				if( TRUE== pSock->IsValidCharacter(FALSE, TRUE) )
				{
					QPARAM_CHECK_EVENTITEM *pQParam = new QPARAM_CHECK_EVENTITEM;
					pQParam->AccountUID		= pSock->m_character.AccountUniqueNumber;
					pQParam->ItemEventUID	= ItemEventInfo->ItemEventUID;
					pQParam->ItemEventType  = ItemEventInfo->ItemEventType;
					pQParam->ItemNum		= ItemEventInfo->ItemNum;
					pQParam->Count			= ItemEventInfo->Count;
					pQParam->PrefixCodeNum	= ItemEventInfo->PrefixCodeNum;
					pQParam->SuffixCodeNum	= ItemEventInfo->SuffixCodeNum;
					pQParam->CharacterUID	= (FALSE == ItemEventInfo->CheckWithCharacterUID) ? 0 : pSock->m_character.CharacterUniqueNumber;	// 2009-11-19 by cmkwon, ¾ÆÀÌÅÛ ÀÌº¥Æ®¿¡ Account/Character Ã¼Å© ±â´É Ãß°¡ - 
					if ( CS_ARENASERVER_PLAYING == pSock->GetClientState() )
					{
						pQParam->InsertStorage = ITEM_IN_STORE;		// ¾Æ·¹³ª¼­¹ö ÀÌ¿ëÁßÀÌ¸é Ã¢°í·Î ¾ÆÀÌÅÛÀ» ³Ö¾îÁØ´Ù.
					}
					m_pFieldIOCP6->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CheckEventItem, pSock, pSock->m_character.AccountUniqueNumber, pQParam);
				}
				// end 2013-04-22 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶ ½Ã½ºÅÛ º¸¾È
			}
		}
		break;
	default:
		{
		}
		break;
	}
#endif
	// end 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶·Î º¯°æ
	return;
}

void CFieldTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CFieldTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldIOCP6->ResetAllP2PPKFamePoint();
#ifdef NEMERIAN_AUTO_ELECTION_AND_RESET
	//byroo
	GetPollDate();
	//char szSystem[1024];
	//sprintf(szSystem, "[NEMERIAN DEBUG] jour = %u mois = %u moi election = %u\r\n", pDateTime->Day, pDateTime->Month, m_ElectionMonth);
	//g_pFieldGlobal->WriteSystemLog(szSystem);
	if (pDateTime->Day >= 1 && pDateTime->Month >= m_ElectionMonth && pDateTime->Year >= m_ElectionYear) {
		//char szSystem[1024];
		//sprintf(szSystem, "[NEMERIAN DEBUG] jour = %u mois = %u moi election = %u\r\n", pDateTime->Day, pDateTime->Month, m_ElectionMonth);
		//g_pFieldGlobal->WriteSystemLog(szSystem);
		//resultat election
		SetElectionResult();
		//faire resultat election sur le launcher en php
		//new election date
		SetNewElectionDate(pDateTime);
		DBInitAllInflWarData();
		DBInitAllMonthlyGuildFame();
		SetPlayerAsLeader();
	}
#endif
}

void CFieldTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldIOCP6->OnDoHourlyWorkIOCP(pDateTime);
}

void CFieldTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldIOCP6->OnDoMinutelyWorkIOCP(pDateTime);
#ifdef MS_TAKE_NO_DAMAGE
	m_pFieldIOCP6->m_InflWarManager.SYS_H = pDateTime->Hour;
	m_pFieldIOCP6->m_InflWarManager.SYS_M = pDateTime->Minute;
#endif
#ifdef _AUTO_COH
	if (pDateTime->Minute == 30)
	{
		if (!m_pFieldIOCP6->m_InflWarManager.IsDoingInfluenceWar())
		{
			int maps[] = {3008};
			char* messages[] = {"\\cLet Clean up Bark City\\c","\\eLet Clean up Bark City\\e","\\\cLet Clean up Bark City\\c","\\yLet Clean up Bark City\\y"};
			SCALL_WARP_EVENT tmCallWarpEvent;
			util::zero(&tmCallWarpEvent, sizeof(SCALL_WARP_EVENT));
			tmCallWarpEvent.byPermissionInfluenceMask = INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI | INFLUENCE_TYPE_GM;
			tmCallWarpEvent.dwCallWarpEventID = timeGetTime();
			tmCallWarpEvent.dwSupportTermTick = 10000;
			tmCallWarpEvent.vWarpPosition = D3DXVECTOR3(5120.0f, 900.0f, 5120.0f);
			
			
			tmCallWarpEvent.CallMapChannelIndex = MAP_CHANNEL_INDEX(maps[RANDI(0, (sizeof(maps) / sizeof(maps[0])) - 1)], 0);
			//tmCallWarpEvent.CallMapChannelIndex = MAP_CHANNEL_INDEX(maps[rand() % 1], 0); WTF? is this??????
			tmCallWarpEvent.CallMapChannelIndex = MAP_CHANNEL_INDEX(maps[0], 0);
			
			
			tmCallWarpEvent.nPermissionUserCnts = 1000;
			tmCallWarpEvent.byPermissionMinLevel = 0;
			tmCallWarpEvent.byPermissionMaxLevel = 0;
			tmCallWarpEvent.usReqUnitKind = UNITKIND_ALL_MASK;

			m_pFieldIOCP6->AddCallWarpEventRequest(&tmCallWarpEvent);

			INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST, T_FC_EVENT_CALL_WARP_EVENT_REQUEST, pSCallWarp, SendBuf);
			pSCallWarp->dwCallWarpEventID0 = tmCallWarpEvent.dwCallWarpEventID;
			pSCallWarp->dwSupportTermTick0 = tmCallWarpEvent.dwSupportTermTick;
			util::strncpy(pSCallWarp->szUserMessage, messages[RANDI(0, (sizeof(messages) / sizeof(messages[0])) - 1)], SIZE_STRING_128);
			m_pFieldIOCP6->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST), tmCallWarpEvent.byPermissionInfluenceMask
				, tmCallWarpEvent.byPermissionMinLevel, tmCallWarpEvent.byPermissionMaxLevel, &tmCallWarpEvent.CallMapChannelIndex, FALSE, TRUE, tmCallWarpEvent.usReqUnitKind);
		}
	}
#endif
}

void CFieldTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldIOCP6->OnDoSecondlyWorkFIOCP(pDateTime);
}

#ifdef NEMERIAN_AUTO_ELECTION_AND_RESET
void CFieldTickManager::GetPollDate() {
	RETCODE	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0013, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}

	SPOLLDATE_DB		tmPOLLDATE_DB;
	util::zero(&tmPOLLDATE_DB, sizeof(SPOLLDATE_DB));

	SQL_TIMESTAMP_STRUCT	tmApplicationStartDate;
	SQL_TIMESTAMP_STRUCT	tmApplicationEndDate;
	SQL_TIMESTAMP_STRUCT	tmVoteStartDate;
	SQL_TIMESTAMP_STRUCT	tmVoteEndDate;
	SQL_TIMESTAMP_STRUCT	tmElection;

	SQLINTEGER	arrCB[6] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 1, SQL_C_TIMESTAMP, &tmApplicationStartDate, 0, &arrCB[1]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 2, SQL_C_TIMESTAMP, &tmApplicationEndDate, 0, &arrCB[2]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 3, SQL_C_TIMESTAMP, &tmVoteStartDate, 0, &arrCB[3]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 4, SQL_C_TIMESTAMP, &tmVoteEndDate, 0, &arrCB[4]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 5, SQL_C_TIMESTAMP, &tmElection, 0, &arrCB[5]);

	if ((ret = SQLFetch(m_pFieldIOCP6->m_pFieldODBC->m_hstmt)) != SQL_NO_DATA)
	{
		tmPOLLDATE_DB.ApplicationStartDate = tmApplicationStartDate;
		tmPOLLDATE_DB.ApplicationEndDate = tmApplicationEndDate;
		tmPOLLDATE_DB.VoteStartDate = tmVoteStartDate;
		tmPOLLDATE_DB.VoteEndDate = tmVoteEndDate;
		tmPOLLDATE_DB.Election = tmElection;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();

	switch (tmPOLLDATE_DB.Election.Month) {
	case 1:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 2;
		break;
	case 2:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 3;
		break;
	case 3:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 4;
		break;
	case 4:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 5;
		break;
	case 5:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 6;
		break;
	case 6:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 7;
		break;
	case 7:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 8;
		break;
	case 8:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 9;
		break;
	case 9:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 10;
		break;
	case 10:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 11;
		break;
	case 11:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year;
		m_ElectionMonth = 12;
		break;
	case 12:
		m_ElectionYear = tmPOLLDATE_DB.Election.Year+1;
		m_ElectionMonth = 1;
		break;
	}
}

void CFieldTickManager::SetNewElectionDate(ATUM_DATE_TIME *pDateTime) {
	//set new date election for the next month
	ATUM_DATE_TIME *ElectionDate = pDateTime;
	SPOLLDATE_DB		tmPOLLDATE_DB;
	util::zero(&tmPOLLDATE_DB, sizeof(SPOLLDATE_DB));

	switch (ElectionDate->Month) {
	case 1:
		ElectionDate->Day = 31;
		break;
	case 2:
		//except for years with 366 days
		if ((ElectionDate->Year % 4) == 0 && (ElectionDate->Year % 100) || (ElectionDate->Year % 400) == 0) {
			ElectionDate->Day = 29;
		}
		else {
			ElectionDate->Day = 28;
		}
		break;
	case 3:
		ElectionDate->Day = 31;
		break;
	case 4:
		ElectionDate->Day = 30;
		break;
	case 5:
		ElectionDate->Day = 31;
		break;
	case 6:
		ElectionDate->Day = 30;
		break;
	case 7:
		ElectionDate->Day = 31;
		break;
	case 8:
		ElectionDate->Day = 31;
		break;
	case 9:
		ElectionDate->Day = 30;
		break;
	case 10:
		ElectionDate->Day = 31;
		break;
	case 11:
		ElectionDate->Day = 30;
		break;
	case 12:
		ElectionDate->Day = 31;
		break;
	}

	tmPOLLDATE_DB.ApplicationStartDate.SetDateTime(ElectionDate->Year, ElectionDate->Month, ElectionDate->Day - 6
		, 0, 0, 1);
	tmPOLLDATE_DB.ApplicationEndDate.SetDateTime(ElectionDate->Year, ElectionDate->Month, ElectionDate->Day - 4
		, 23, 59, 59);
	tmPOLLDATE_DB.VoteStartDate.SetDateTime(ElectionDate->Year, ElectionDate->Month, ElectionDate->Day - 3
		, 0, 0, 1);
	tmPOLLDATE_DB.VoteEndDate.SetDateTime(ElectionDate->Year, ElectionDate->Month, ElectionDate->Day - 1
		, 23, 59, 59);
	tmPOLLDATE_DB.Election.SetDateTime(ElectionDate->Year, ElectionDate->Month, ElectionDate->Day
		, 23, 50, 59);

	char szApplicationStartDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szApplicationEndDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szVoteStartDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szVoteEndDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szElection[SIZE_MAX_SQL_DATETIME_STRING];
	tmPOLLDATE_DB.ApplicationStartDate.GetSQLDateTimeString(szApplicationStartDate, SIZE_MAX_SQL_DATETIME_STRING);
	tmPOLLDATE_DB.ApplicationEndDate.GetSQLDateTimeString(szApplicationEndDate, SIZE_MAX_SQL_DATETIME_STRING);
	tmPOLLDATE_DB.VoteStartDate.GetSQLDateTimeString(szVoteStartDate, SIZE_MAX_SQL_DATETIME_STRING);
	tmPOLLDATE_DB.VoteEndDate.GetSQLDateTimeString(szVoteEndDate, SIZE_MAX_SQL_DATETIME_STRING);
	tmPOLLDATE_DB.Election.GetSQLDateTimeString(szElection, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szApplicationStartDate, 0, NULL);
	SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szApplicationEndDate, 0, NULL);
	SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szVoteStartDate, 0, NULL);
	SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szVoteEndDate, 0, NULL);
	SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szElection, 0, NULL);

	RETCODE	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0014, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}

}

void CFieldTickManager::SetElectionResult() {
	//set result of election leader
	m_TotalVoteCount = 0;
	m_mtvectSLEADER_CANDIDATE.clear();

	RETCODE	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0015, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}

	SQLINTEGER	arrCB[11] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS
		, SQL_NTS };

	SLEADER_CANDIDATE			tmSLEADER_CANDIDATE;
	util::zero(&tmSLEADER_CANDIDATE, sizeof(SLEADER_CANDIDATE));

	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 1, SQL_C_ULONG, &tmSLEADER_CANDIDATE.LeaderCandidateNum, 0, &arrCB[1]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 2, SQL_C_ULONG, &tmSLEADER_CANDIDATE.AccountUID, 0, &arrCB[2]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 3, SQL_C_ULONG, &tmSLEADER_CANDIDATE.CharacterUID, 0, &arrCB[3]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 4, SQL_C_CHAR, tmSLEADER_CANDIDATE.CharacterName, SIZE_MAX_CHARACTER_NAME, &arrCB[4]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 5, SQL_C_ULONG, &tmSLEADER_CANDIDATE.GuildUID, 0, &arrCB[5]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 6, SQL_C_CHAR, tmSLEADER_CANDIDATE.GuildName, SIZE_MAX_GUILD_NAME, &arrCB[6]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 7, SQL_C_TINYINT, &tmSLEADER_CANDIDATE.Influence, 0, &arrCB[7]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 8, SQL_C_TINYINT, &tmSLEADER_CANDIDATE.DeleteCandidate, 0, &arrCB[8]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 9, SQL_C_ULONG, &tmSLEADER_CANDIDATE.PollCount, 0, &arrCB[9]);
	SQLBindCol(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 10, SQL_C_CHAR, tmSLEADER_CANDIDATE.CampaignPromises, SIZE_MAX_CAMPAIGNPROMISES, &arrCB[10]);

	while ((ret = SQLFetch(m_pFieldIOCP6->m_pFieldODBC->m_hstmt)) != SQL_NO_DATA)
	{
		m_TotalVoteCount += tmSLEADER_CANDIDATE.PollCount;
		m_mtvectSLEADER_CANDIDATE.push_back(tmSLEADER_CANDIDATE);
		util::zero(&tmSLEADER_CANDIDATE, sizeof(SSTRATEGYPOINT_NOT_SUMMON_TIME));
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	//get the pollCount to do calcul
	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	float maxPollCount = 0;
	m_LeaderUID = 0;
	m_LeaderInflTy = 0;
	m_LeaderAccountUID = 0;
	for (; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		float fDivideTmp = (float)(((float)itr->PollCount / (float)m_TotalVoteCount) * 100);
		if (maxPollCount < fDivideTmp) {
			maxPollCount = fDivideTmp;
			m_LeaderUID = itr->CharacterUID;
			m_LeaderInflTy = itr->Influence;
			m_LeaderAccountUID = itr->AccountUID;
		}
	}
}

void CFieldTickManager::SetPlayerAsLeader() {
	char szSystem[1024];
	if (m_LeaderUID != 0)
	{
		RETCODE ret;

		SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_LeaderInflTy, 0, NULL);
		SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_LeaderUID, 0, NULL);

		ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_130116_0001, SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			sprintf(szSystem, "DB Execute Error !!(CCharacterInfoDlg::atum_InitCityLeader)");
			g_pFieldGlobal->WriteSystemLog(szSystem);
			m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		}

		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	}

	string szQuery;

	SQLHSTMT hstmt = m_pFieldIOCP6->m_pFieldODBC->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = { SQL_NTS, SQL_NTS, SQL_NTS };
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_LeaderInflTy, 0, &arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_LeaderUID, 0, &arrCB2[2]);
	BOOL bRet = m_pFieldIOCP6->m_pFieldODBC->ExecuteQuery((char*)(PROCEDURE_080827_0004));
	if (!bRet)
	{
		sprintf(szSystem, "DB Execute Error !!(CCharacterInfoDlg::StoreInflLeader_)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);

		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();

	if (m_LeaderUID != 0)
	{
		RETCODE	ret;

		SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_LeaderInflTy, 0, NULL);
		SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_LeaderAccountUID, 0, NULL);
		SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_LeaderUID, 0, NULL);

		ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0001, SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			sprintf(szSystem, "DB Execute Error !!(CCharacterInfoDlg::InsertLeaderItem_)\r\n");
			g_pFieldGlobal->WriteSystemLog(szSystem);
			m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
			return;
		}

		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	}

	if (m_LeaderUID != 0) {
		RETCODE	ret;

		SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_LeaderInflTy, 0, NULL);
		SQLBindParameter(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_LeaderUID, 0, NULL);

		ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0002, SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			sprintf(szSystem, "DB Execute Error !!(CCharacterInfoDlg::atum_UpdateCityLeaderByLeaderSet)\r\n");
			g_pFieldGlobal->WriteSystemLog(szSystem);
			m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
			return;
		}

		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	}
}

void CFieldTickManager::DBInitAllMonthlyGuildFame() {
	//Reset guild fame
	string szQuery;

	BOOL bRet = m_pFieldIOCP6->m_pFieldODBC->ExecuteQuery((char*)(PROCEDURE_080827_0009));
	if (!bRet)
	{
		char szSystem[1024];

		sprintf(szSystem, "DB Execute Error !!(CInflWarListDlg::DBInitAllMonthlyGuildFame_)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
}

void CFieldTickManager::DBInitAllInflWarData() {
	char szSystem[1024];
	//Reset all war data of the month

	BOOL bRet = m_pFieldIOCP6->m_pFieldODBC->ExecuteQuery((char*)(PROCEDURE_080827_0008));
	if (!bRet)
	{
		sprintf(szSystem, "DB Execute Error !!(CInflWarListDlg::DBInitAllInflWarData_)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	sprintf(szSystem, "Success !!(DBInitAllInflWarData_)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	RETCODE	ret;

	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0006, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		sprintf(szSystem, "DB Execute Error !!(CInflWarListDlg::DeleteLeaderItem_)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	//[NEMERIAN TODO]: send update inventory packet to leaders
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	sprintf(szSystem, "Success !!(DeleteLeaderItem_)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0007, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		sprintf(szSystem, "DB Execute Error !!(atum_deleteAllWarpableUserList_)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	sprintf(szSystem, "Success !!(deleteAllWarpableUserList_)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0008, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		sprintf(szSystem, "DB Execute Error !!(atum_InitCityLeaderForInflLeader)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	sprintf(szSystem, "Success !!(InitCityLeaderForInflLeader)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0009, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		sprintf(szSystem, "DB Execute Error !!(atum_InitVoterList)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	sprintf(szSystem, "Success !!(InitVoterList)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0010, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		sprintf(szSystem, "DB Execute Error !!(atum_InitLeaderCandidate)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	sprintf(szSystem, "Success !!(InitLeaderCandidate)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0011, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		sprintf(szSystem, "DB Execute Error !!(atum_log_InitMSWar)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	sprintf(szSystem, "Success !!(log_InitMSWar)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_080822_0012, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		sprintf(szSystem, "DB Execute Error !!(atum_log_InitSPWar)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	sprintf(szSystem, "Success !!(log_InitSPWar)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	ret = SQLExecDirect(m_pFieldIOCP6->m_pFieldODBC->m_hstmt, PROCEDURE_090114_0292, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		sprintf(szSystem, "DB Execute Error !!(atum_ResetDeclarationOfWarByAdminTool)\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystem);
		m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
		return;
	}
	m_pFieldIOCP6->m_pFieldODBC->FreeStatement();
	// 2008-08-06 by dhjin, ¼º°ø¸Þ¼¼Áö Âï±â,
	sprintf(szSystem, "Success !!(ResetDeclarationOfWar)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);

	sprintf(szSystem, "Success !!(Init_InflWarList)\r\n");
	g_pFieldGlobal->WriteSystemLog(szSystem);
}
#endif