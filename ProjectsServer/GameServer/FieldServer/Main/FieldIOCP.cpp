// FieldIOCP.cpp: implementation of the CFieldIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"
#include "MonsterDBAccess.h"
#include "FieldGlobal.h"
#include "FieldMapProject.h"
#include "QuestLoaderParser.h"
#include "FieldTickManager.h"
#include "RacingDemon.h"
#include "FieldParty.h"
#include "CouponManager.h"
#include "HappyHourEventManager.h"
#include "FieldODBC.h"
#include "AtumError.h"
#include "AllGuildItemManager.h"
#include "GuildItemManager.h"					// 2006-09-25 by cmkwon
#include "GameDataLast.h"						// 2007-05-28 by cmkwon
#include "AtumLogSender.h"						// 2007-06-18 by dhjin
//#include "SecurityManager.h"					// 2011-06-22 by hskim, 사설 서버 방지
#include "MapTriggerManager.h"					// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
#include "StatisticsManager.h"					// 2012-01-16 by hskim, 통계 - 화패
#include <BonusKillSystem.h>					// 2015-06-16 by Future, Bonus Kill System Definitions
#ifdef NEMERIAN_PVP_AWARD_EFFORT
#include "LastHitManager.h"
#endif // NEMERIAN_PVP_AWARD_EFFORT


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldIOCP::CFieldIOCP(int nPort, char *szLocalIP) : 
	CIOCP { CLIENT_INDEX_START_NUM, SIZE_MAX_FIELDSERVER_SESSION, nPort, szLocalIP, ST_FIELD_SERVER, sizeof(CFieldIOCPSocket) },
	m_SARENA_USE_FIELD_SERVER_INFO { }
{
	CFieldDataManager::ms_pFieldIOCP2 = this;
	CFieldIOCPSocket::ms_pFieldIOCP = this;
	CGuildItemManager::ms_pFieldIOCP3 = this;				// 2006-09-25 by cmkwon

	// Server ID를 결정
	m_FieldServerID.SetValue(this->GetLocalIPAddress(), nPort);

	m_pRacingDemon = new CRacingDemon;

	m_pAtumDBManager = new CAtumFieldDBManager(this);

	auto pFieldIOCPSocket = new CFieldIOCPSocket[m_dwArrayClientSize];

	for (int i = 0; i < m_dwArrayClientSize; i++)
	{
		pFieldIOCPSocket[i].InitFieldIOCPSocket();
		pFieldIOCPSocket[i].InitIOCPSocket(i);
	}

	m_pArrayIOCPSocket = pFieldIOCPSocket;

	m_mapItemInfo.clear();
	m_mtvectItemPtrKillMarks.clear();			// 2006-02-09 by cmkwon

	CleanMonster2Item();

	m_pPreWinSocket = NULL;
	m_pIMWinSocket = NULL;
	m_pDefaultMapProject = NULL;
	m_pNPCTCPSocket = NULL;
	m_pFieldTickManager = NULL;
	//m_pSecurityManager = NULL;		// 2011-06-22 by hskim, 사설 서버 방지
	m_pStatisticsManager = NULL;		// 2012-01-16 by hskim, 통계 - 화패

	m_pFieldMapWorkspace = new CFieldMapWorkspace(this, ST_FIELD_SERVER);

	m_uiAccumulatedUserCounts = 0;
	m_uiMaxUserCounts = 0;
	m_uiAccumulatedMonsterCounts = 0;

	m_mtvectorDeletedFieldParties.reserve(50);

	m_gameEventManager.InitEventManager(this);
	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 삭제됨
	//	m_pCouponManager				= NULL;
	m_pHappyHourEventManager = NULL;

	m_pDGAEventManager = NULL;

	m_pFieldODBC = NULL;

	m_nNormalInfluenceCount = 0;		// 2006-02-08 by cmkwon
	m_nVCNInfluenceCount = 0;		// 2006-02-08 by cmkwon
	m_nANIInfluenceCount = 0;		// 2006-02-08 by cmkwon


	m_vectCharacter2ItemList.clear();			// 2006-03-02 by cmkwon

	m_nSignBoardStringIndex = 1;		// 2006-04-17 by cmkwon, 필히 초기값을 1로 설정한다

	m_bInitHackShield = FALSE;	// 2006-06-29 by cmkwon

	m_bPrepareShutDown = FALSE;	// 2006-08-04 by cmkwon, 서버 다운 진행상태

	m_pGiveItemManager = NULL;
	m_pAllGuildItemManager = NULL;		// 2006-09-20 by dhjin

	m_bUseTimeLimiteSysteOfToday = FALSE;	// 2006-11-17 by cmkwon

	m_nJamboreeInitCommandVerifyNumber = 0;	// 2007-04-10 by cmkwon

	m_bUnusableSpeakerItem = FALSE;	// 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가 - 기본값 사용 가능 상태

	m_vectSACTION_BY_LEVEL_DB.clear();	// 2007-10-29 by dhjin

	//////////////////////////////////////////////////////////////////////////
	// 2008-08-25 by dhjin, 태국 PC방 IP정보 로딩
	m_nPCBangMinTick = 0;
	m_nMaxPCBangMinTick = MaxPCBangMinTick;

	m_vectLuckyMachine.clear();		// 2008-11-10 by dhjin, 럭키머신

	m_mtvectCBossWarContributionCalculation.reserve(2);			// 2008-12-22 by dhjin, 전쟁 보상 추가안
	m_mtvectCOutPostWarContributionCalculation.reserve(5);
	m_mtvectCStrategyPointWarContributionCalculation.reserve(5);

	m_bWarningStateForSelectInfluence = FALSE;		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
//	m_TenderInfoList.clear();			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender
	m_InfinityShopInfoList.clear();		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점

	m_InfinityManager.m_pFieldIOCP21 = this;		// 2010-03-26 by cmkwon, 인피2차 수정 - CFieldIOCP 관리구조 추가
	CInfinityBase::ms_pFieldIOCP4 = this;		// 2010-03-26 by cmkwon, 인피2차 수정 - CFieldIOCP 관리구조 추가

	m_BurningMapList.clear();			// 2010-08-05 by dhjin, 버닝맵 -

	m_ShutDownData.clear();				// START 2011-11-03 by shcho, yedang 셧다운제 구현 -

	m_pMapTriggerManager = NULL;		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	memset(m_bIsMapWarpPossible, TRUE, sizeof(m_bIsMapWarpPossible));	// 2012-11-29 by jhseol, OX이벤트 맵 워프제한 - 전맵이 워프가 가능하도록 TRUE로 초기화

	memset(m_bTriggerNextMapWarpPossible, TRUE, sizeof(m_bTriggerNextMapWarpPossible));	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 워프제한

	m_bServerActive = FALSE;				// 2013-03-13 by hskim, 웹 캐시 상점
	m_MaximumItemUID = 0;					// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지

	m_pMonthlyArmorManager = NULL;		// 2013-04-18 by jhseol,bckim 이달의 아머 - m_pMonthlyArmorManager 초기화

	// 2015-11-25 Future, Multiple IP Restriction System
#ifdef S_IP_UNIQUE_CONNECTION
	m_bMultipleIPRestriction = g_pFieldGlobal->GetMultipleIPRestrictionFlag();
#endif // S_IP_UNIQUE_CONNECTION
#ifdef NEMERIAN_PVP_AWARD_EFFORT
	m_pLastHitManager = new LastHitManager;
	m_pLastHitManager->createThread();
#endif
}

CFieldIOCP::~CFieldIOCP()
{
	ListenerClose();
	IOCPClean();

	//memset(m_ArrayClient, 0x00, sizeof(CIOCPSocket*) * COUNT_MAX_SOCKET_SESSION);
	if (NULL != m_pArrayIOCPSocket)
	{
		delete[](CFieldIOCPSocket*)m_pArrayIOCPSocket;
		m_pArrayIOCPSocket = NULL;
	}

	util::del_destroy(m_pMapTriggerManager);		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	util::del(m_pHappyHourEventManager);
	util::del(m_pDGAEventManager);
	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 삭제됨
	//	util::del(m_pCouponManager);
	util::del(m_pRacingDemon);
	util::del(m_pAtumDBManager);
	util::del(m_pFieldMapWorkspace);
	util::del(m_pFieldTickManager);
	//util::del(m_pSecurityManager);		// 2011-06-22 by hskim, 사설 서버 방지
	util::del(m_pStatisticsManager);		// 2012-01-16 by hskim, 통계 - 화패
	util::del(m_pGiveItemManager);
	util::del(m_pAllGuildItemManager);	// 2006-09-20 by dhjin

	util::del(m_pFieldODBC);

	util::del(m_pMonthlyArmorManager);		// 2013-04-18 by jhseol,bckim 이달의 아머 - m_pMonthlyArmorManager 삭제
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::LoadFieldServerDataDebug(void)
/// \brief		개발용, PreServer & IMServer가 실행되지 않아도 테스트 가능함
/// \author		kelovon
/// \date		2004-04-20
/// \warning	개발 및 테스트용으로만 사용됨
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::LoadFieldServerDataDebug(void)
{
	BOOL	bRet = FALSE;

	CODBCStatement odbcStmt;
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//bRet = odbcStmt.Init((LPCSTR)g_pFieldGlobal->GetODBCDSN(), (LPCSTR)g_pFieldGlobal->GetODBCUID(), (LPCSTR)g_pFieldGlobal->GetODBCPASSWORD());
	bRet = odbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (LPCSTR)g_pGlobal->GetODBCUID(), (LPCSTR)g_pGlobal->GetODBCPASSWORD(), g_pFieldGlobal->GetMainWndHandle());
	if (!bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldIOCP::LoadFieldServerDataDebug_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		MessageBox(NULL, "DB connect fail@CFieldIOCP::IOCPInit()", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
	vectMapIndexList		vectArenaMapIndexList;
	bRet = CAtumDBHelper::LoadArenaMapList(&vectArenaMapIndexList, &odbcStmt);
	if (FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Error] Load CAtumDBHelper::LoadArenaMapList_ !!");
		MessageBox(NULL, "[Error] Load CAtumDBHelper::LoadArenaMapList_ !!", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// Load Map Data...
	// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
	//bRet = m_pFieldMapWorkspace->LoadAllProject(CONFIG_ROOT+string("./map.cfg")).c_str());
#ifdef ARENA
	bRet = m_pFieldMapWorkspace->LoadAllProject((CONFIG_ROOT + string("./ArenaMap.cfg")).c_str(), g_pFieldGlobal->IsArenaServer(), &vectArenaMapIndexList);
#else
	bRet = m_pFieldMapWorkspace->LoadAllProject((CONFIG_ROOT + string("./map.cfg")).c_str(), g_pFieldGlobal->IsArenaServer(), &vectArenaMapIndexList);
#endif //ARENA

	if (FALSE == bRet || m_pFieldMapWorkspace->m_vectorPtrMapProject.empty())
	{
		// 2008-04-28 by cmkwon, 시스템 로그 추가
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "LoadAllProject error !!, Ret(%d), MapProjectCount(%d)\r\n", bRet, m_pFieldMapWorkspace->m_vectorPtrMapProject.size());
		MessageBox(NULL, "LoadAllProject fail@CFieldIOCP::IOCPInit()", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	m_pDefaultMapProject = (CFieldMapProject*)m_pFieldMapWorkspace->GetMapProjectByMapIndex(m_DefaultMapIndex);
	if (NULL == m_pDefaultMapProject)
	{
		// error! no map!
		char szSystemLog[256];
		sprintf(szSystemLog, "Error: No Default Map!\r\n\r\n");
		g_pFieldGlobal->WriteSystemLog(szSystemLog);
		assert(0);
		return FALSE;
	}

	// Load ItemInfo...
	LoadItemInfo();

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
	this->ArrangePartyFormationSkill(&m_PartyFormationItemSkillList, &m_mapItemInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-10 by cmkwon, 아이템 체크, 랜덤박스 지급 아이템 추가, 
	ITEM * pErrItemInfo = CheckAllItemInfo();
	if (NULL != pErrItemInfo)
	{
		char szSystemLog[1024];
		sprintf(szSystemLog, "CheckAllItemInfo Error, ItemInfo(%8d:%20s), ItemKind(%d:%s) \r\n"
			, pErrItemInfo->ItemNum, pErrItemInfo->ItemName, pErrItemInfo->Kind, CAtumSJ::GetItemKindName(pErrItemInfo->Kind));
		g_pFieldGlobal->WriteSystemLog(szSystemLog);
		MessageBox(NULL, szSystemLog, "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}


	// Load RareItemInfo...
	bRet = CAtumDBHelper::LoadRareItemInfo(&odbcStmt, &m_mapRareItemInfo, NULL, &m_mapLevel2RareItemInfo);
	if (!bRet)
	{
		DBGOUT("ERROR: LoadRareItemInfo() Failed!!!\r\n");
		return FALSE;
	}
	//
	//	// Load MonsterInfo...(항상 LoadItemInfo 뒤에 처리되어야함)
	//	CMonsterDBAccess MonsterDBAccess();
	//	MonsterDBAccess.GetAllMonsters(m_mapMonsterInfo, &m_mapItemInfo, MONSTER_LOAD_TYPE_SIZE_FOR_CLIENT);
	//
	//	// Load Monster2Item;
	//	LoadMonster2Item();

	if (!LoadQuestInfo())
	{
		MessageBox(NULL, STRMSG_S_F2QUEST_0000, "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	MessageBox(NULL, STRMSG_S_F2CONFIG_0000, "ERROR - SpaceCowboy FieldServer", MB_TOPMOST | MB_ICONSTOP);

	odbcStmt.Clean();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-08 by cmkwon, 서버에서 맵로딩시 AlivePacket 전송 추가 - 
/// \author		cmkwon
/// \date		2010-04-08 ~ 2010-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendAlivePacket2OtherServers(void)
{
	this->Send2PreAndIMOfAlivePacket();		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 
}

BOOL CFieldIOCP::IOCPInit(void)
{
	BOOL	bRet = FALSE;

	/*	// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
		char szServerName[SIZE_MAX_STRING_128] = { 0, };
		sprintf(szServerName, "FieldServer_%s", g_pFieldGlobal->GetServerGroupName());
		if( FALSE == g_pGlobal->CreateDuplicateRun(szServerName) )
		{
			MessageBox(NULL, "ERROR : \nApplication is running already...", szServerName, MB_OK);
			return FALSE;
		}
		// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
	*/
	// 2006-06-16 by cmkwon, 주석처리함
	// 	///////////////////////////////////////////////////////////////////////////////
	// 	// 테스트 서버 설정
	// 	if(g_pFieldGlobal->IsTestServer())
	// 	{
	// 		CGameEventManager::ms_fDefaultExpRate		= 3.0f;
	// 		CGameEventManager::ms_fDefaultSPIRate		= 2.0f;
	// 		CGameEventManager::ms_fDefaultDropItemRate	= 2.0f;
	// 	}

	//	bRet = m_pRacingDemon->InitRacingDemon();
	//	if(FALSE == bRet)
	//	{
	//		MessageBox(NULL, "RacingDemon 초기화 실패", "ERROR - SpaceCowboy FieldServer", NULL);
	//		return FALSE;
	//	}

		///////////////////////////////////////////////////////////////////////////////
		// 2006-06-05 by cmkwon
		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 아래와 같이 수정함.
		//if(FALSE == this->SecurityInit())
	if (FALSE == CFieldIOCPSocket::SecurityServerInit())		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 
	{
		MessageBox(NULL, " @CFieldIOCP::SecurityInit()", "ERROR - SecurityInit()", NULL);
		return FALSE;
	}

	m_pFieldODBC = new CFieldODBC;
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//	if(FALSE == m_pFieldODBC->InitFieldODBC(this, g_pFieldGlobal->GetODBCDSN(), g_pFieldGlobal->GetODBCUID()
	//		, g_pFieldGlobal->GetODBCPASSWORD()))
	if (FALSE == m_pFieldODBC->InitFieldODBC(this, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID()
		, (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle()))
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldIOCP::IOCPInit_1) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		MessageBox(NULL, "DB connect fail@CFieldIOCP::IOCPInit()", "ERROR - FieldODBC", NULL);
		return FALSE;
	}

	CODBCStatement odbcStmt;
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//bRet = odbcStmt.Init((LPCSTR)g_pFieldGlobal->GetODBCDSN(), (LPCSTR)g_pFieldGlobal->GetODBCUID(), (LPCSTR)g_pFieldGlobal->GetODBCPASSWORD());
	bRet = odbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (LPCSTR)g_pGlobal->GetODBCUID(), (LPCSTR)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
	if (!bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldIOCP::IOCPInit_2) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		MessageBox(NULL, "DB connect fail@CFieldIOCP::IOCPInit()", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// Load AtumSJ Data
	if (!CAtumSJ::LoadData(&odbcStmt))
	{
		MessageBox(NULL, "CAtumSJ::LoadData() fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 아래와 같이 Send2PreAndIMOfAlivePacket() 로 대체
	// 	// 2008-02-14 by cmkwon, Pre,IM Server로 Alive 패킷을 전송한다.
	// 	MessageType_t msgTy;
	// 	msgTy		= T_FP_CONNECT_ALIVE;
	// 	this->Send2PreServerW((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	// 	msgTy		= T_FI_CONNECT_ALIVE;
	// 	this->Send2IMServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	this->Send2PreAndIMOfAlivePacket();		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
	vectMapIndexList		vectArenaMapIndexList;
	bRet = CAtumDBHelper::LoadArenaMapList(&vectArenaMapIndexList, &odbcStmt);
	if (FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Error] Load CAtumDBHelper::LoadArenaMapList_ !!");
		MessageBox(NULL, "[Error] Load CAtumDBHelper::LoadArenaMapList_ !!", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	if (FALSE == this->m_InfinityManager.SetDBManager(m_pAtumDBManager)) {
		MessageBox(NULL, "CInfinityManager::SetDBManager fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	if (g_pFieldGlobal->IsArenaServer()) {
		// 아레나 서버만 Tick시작
		this->m_InfinityManager.StartTick();
	}
	vectorCinemaInfo			tmVectCinemaInfo;
	bRet = CAtumFieldDBManager::DBLoadCinemaInfo(&odbcStmt, &tmVectCinemaInfo);	// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
	if (FALSE == bRet) {
		MessageBox(NULL, "CAtumFieldDBManager::DBLoadCinemaInfo fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	this->m_InfinityManager.SetCinemaInfoW(&tmVectCinemaInfo);

	vectorRevisionInfo			tmVectRevisionInfo;
	bRet = CAtumFieldDBManager::DBLoadRevisionInfo(&odbcStmt, &tmVectRevisionInfo);
	if (FALSE == bRet) {
		MessageBox(NULL, "CAtumFieldDBManager::DBLoadRevisionInfo fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	this->m_InfinityManager.SetRevisionInfoW(&tmVectRevisionInfo);

	this->m_InfinityManager.SetInfinityMapManagerW(this->m_pFieldMapWorkspace);

	vectorInfinityModeInfo		tmVectInfinityModeInfo;
	bRet = CAtumDBHelper::DBLoadInfinityModeInfo(&odbcStmt, &tmVectInfinityModeInfo, &vectArenaMapIndexList);
	if (FALSE == bRet) {
		MessageBox(NULL, "CAtumFieldDBManager::DBLoadInfinityModeInfo fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	vectorInfinityMonsterInfo	tmVectInfinityMonsterInfo;
	bRet = CAtumFieldDBManager::DBLoadInfinityMonsterInfo(&odbcStmt, &tmVectInfinityMonsterInfo);
	if (FALSE == bRet) {
		MessageBox(NULL, "CAtumFieldDBManager::DBLoadInfinityMonsterInfo fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	this->m_InfinityManager.SetDBInfinityMonsterInfo(&tmVectInfinityMonsterInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-05-13 by shcho, 인피니티 난이도 조절 -
	vectorInfinity_DifficultyInfo_Bonus tmDifficultyList;
	if (FALSE == CAtumFieldDBManager::DBLoadInfinityDifficultInfo(&odbcStmt, &tmDifficultyList))
	{
		MessageBox(NULL, "call CAtumFieldDBManager::DBLoadInfinityDifficultInfo# fail !!", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	this->m_InfinityManager.Set_Difficulty_BonusInfo_ListData(&tmDifficultyList);

	vectorInfinity_DifficultyInfo_Monster tmDifficultymonsterList;
	if (FALSE == CAtumFieldDBManager::DBLoadInfinityDifficultMonsterInfo(&odbcStmt, &tmDifficultymonsterList))
	{
		MessageBox(NULL, "call CAtumFieldDBManager::DBLoadInfinityDifficultMonsterInfo# fail !!", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	this->m_InfinityManager.Set_Difficulty_MonsterInfo_ListData(&tmDifficultymonsterList);
	///////////////////////////////////////////////////////////////////////////////

	// start 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
#ifdef S_EP4_HSKIM		// ON/OFF 기능 구현
	if (FALSE == CAtumFieldDBManager::DBLoadTriggerMapBuffInfo(&odbcStmt, &m_pFieldMapWorkspace->m_vecTriggerMapBuff, FALSE))
	{
		MessageBox(NULL, "CAtumFieldDBManager::DBLoadTriggerMapBuffInfo fail", "ERROR - Aceonline FieldServer", NULL);

		return FALSE;
	}
#endif
	// end 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	m_pMapTriggerManager = new CMapTriggerManager;

	if (FALSE == m_pMapTriggerManager->Create(this, &odbcStmt))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "MapTriggerManager Create Fail !!\r\n");
		MessageBox(NULL, "[Error] MapTriggerManager Create Error (LoadDB) !!", "ERROR - Aceonline FieldServer", NULL);
		return FALSE;
	}
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	// 2009-09-09 ~ 2010 by dhjin, 인피니티 -

	// Load Map Data...
	// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
	//bRet = m_pFieldMapWorkspace->LoadAllProject(CONFIG_ROOT+string("./map.cfg")).c_str());
#ifdef ARENA
	bRet = m_pFieldMapWorkspace->LoadAllProject((CONFIG_ROOT + string("./ArenaMap.cfg")).c_str(), g_pFieldGlobal->IsArenaServer(), &vectArenaMapIndexList);
#else
	bRet = m_pFieldMapWorkspace->LoadAllProject((CONFIG_ROOT + string("./map.cfg")).c_str(), g_pFieldGlobal->IsArenaServer(), &vectArenaMapIndexList);
#endif //ARENA
	if (FALSE == bRet || m_pFieldMapWorkspace->m_vectorPtrMapProject.empty())
	{
		// 2008-04-28 by cmkwon, 시스템 로그 추가
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "LoadAllProject error !!, Ret(%d), MapProjectCount(%d)\r\n", bRet, m_pFieldMapWorkspace->m_vectorPtrMapProject.size());
		MessageBox(NULL, "LoadAllProject fail@CFieldIOCP::IOCPInit()", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	m_pDefaultMapProject = (CFieldMapProject*)m_pFieldMapWorkspace->GetMapProjectByMapIndex(m_DefaultMapIndex);
	if (NULL == m_pDefaultMapProject)
	{// error! no default map!
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "Error: No Default Map!\r\n\r\n");
		assert(0);
		return FALSE;
	}

	this->m_InfinityManager.SetDBInfinityModeInfo(&tmVectInfinityModeInfo);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 맵 로딩 종료후 설정(맵 관련 처리가 있다)
	this->ResetInfinityByResetCycleByServerStart();

	// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 아래와 같이 Send2PreAndIMOfAlivePacket() 로 대체
	// 	// 2008-02-14 by cmkwon, Pre,IM Server로 Alive 패킷을 전송한다.
	// 	msgTy		= T_FP_CONNECT_ALIVE;
	// 	this->Send2PreServerW((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	// 	msgTy		= T_FI_CONNECT_ALIVE;
	// 	this->Send2IMServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	this->Send2PreAndIMOfAlivePacket();		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 

	if (m_pAtumDBManager->Init() == FALSE)
	{
		MessageBox(NULL, "CAtumFieldDBManager::Init() fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// Initialize Tick Manager
	m_pFieldTickManager = new CFieldTickManager(this);
	if (m_pFieldTickManager->InitThread() == FALSE) { return FALSE; }

	// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 아래와 같이 Send2PreAndIMOfAlivePacket() 로 대체
	// 	// 2008-02-14 by cmkwon, Pre,IM Server로 Alive 패킷을 전송한다.
	// 	msgTy		= T_FP_CONNECT_ALIVE;
	// 	this->Send2PreServerW((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	// 	msgTy		= T_FI_CONNECT_ALIVE;
	// 	this->Send2IMServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	this->Send2PreAndIMOfAlivePacket();		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	InvokingWearItemDestParamList InvokingWearItemDestParamNumList;
	bRet = CAtumDBHelper::LoadInvokingWearItemDestParamNum(&odbcStmt, &InvokingWearItemDestParamNumList);
	if (FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::LoadInvokingWearItemDestParamNum() Failed!!!\r\n");
		return FALSE;
	}
	this->m_InvokingItemInfo.SetDBInvokingWearItemDestParam(&InvokingWearItemDestParamNumList);
	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to LoadInvokingWearItemDestParamNum(%d) \r\n", InvokingWearItemDestParamNumList.size());

	InvokingWearItemDestParamList InvokingWearItemDestParamNumByUseList;
	bRet = CAtumDBHelper::LoadInvokingWearItemDestParamNumByUse(&odbcStmt, &InvokingWearItemDestParamNumByUseList);
	if (FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::LoadInvokingWearItemDestParamNumByUse() Failed!!!\r\n");
		return FALSE;
	}
	this->m_InvokingItemInfo.SetDBInvokingWearItemDestParamByUse(&InvokingWearItemDestParamNumByUseList);
	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to LoadInvokingWearItemDestParamNumByUse(%d) \r\n", InvokingWearItemDestParamNumByUseList.size());

	// Load ItemInfo...
	LoadItemInfo();

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
	this->ArrangePartyFormationSkill(&m_PartyFormationItemSkillList, &m_mapItemInfo);

	// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 아래와 같이 Send2PreAndIMOfAlivePacket() 로 대체
	// 	// 2008-02-14 by cmkwon, Pre,IM Server로 Alive 패킷을 전송한다.
	// 	msgTy		= T_FP_CONNECT_ALIVE;
	// 	this->Send2PreServerW((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	// 	msgTy		= T_FI_CONNECT_ALIVE;
	// 	this->Send2IMServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	this->Send2PreAndIMOfAlivePacket();		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-10 by cmkwon, 아이템 체크, 랜덤박스 지급 아이템 추가, 
	ITEM * pErrItemInfo = CheckAllItemInfo();
	if (NULL != pErrItemInfo)
	{
		char szSystemLog[1024];
		sprintf(szSystemLog, "CheckAllItemInfo Error, ItemInfo(%8d:%20s), ItemKind(%d:%s)\r\n"
			, pErrItemInfo->ItemNum, pErrItemInfo->ItemName, pErrItemInfo->Kind, CAtumSJ::GetItemKindName(pErrItemInfo->Kind));
		g_pFieldGlobal->WriteSystemLog(szSystemLog);
		MessageBox(NULL, szSystemLog, "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
	vectSITEM_PARAM_OVERLAP tmItemOverlapList;
	if (FALSE == CAtumDBHelper::LoadItemParamOverlap(&odbcStmt, &tmItemOverlapList))
	{
		char szSystemLog[1024];
		sprintf(szSystemLog, "[ERROR] LoadItemParamOverlap Error, vectorCnt(%d)", tmItemOverlapList.size());
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "%s \r\n", szSystemLog);
		MessageBox(NULL, szSystemLog, "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	if (FALSE == this->ResetItemParamOverlapList(&tmItemOverlapList))
	{
		char szSystemLog[1024];
		sprintf(szSystemLog, "[ERROR] ResetItemParamOverlapList Error, vectorCnt(%d) mapCnt(%d)", tmItemOverlapList.size(), m_mapItemParamOverlapList.size());
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "%s \r\n", szSystemLog);
		MessageBox(NULL, szSystemLog, "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2006-08-24 by dhjin
	m_pGiveItemManager = new CGiveItemManager;
	m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadItemEvent, NULL, 0, NULL);

	// 2013-04-18 by jhseol,bckim 이달의 아머 - 서버 시작시 m_pMonthlyArmorManager 객체 생성 및 이벤트 로드
#ifdef S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// 서버 시작시 m_pMonthlyArmorManager 객체 생성 및 이벤트 로드
	if (FALSE == g_pFieldGlobal->IsArenaServer())
	{
		m_pMonthlyArmorManager = new CMonthlyArmorManager(this);
		m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadMonthlyArmorEvent, NULL, 0, NULL);
	}
#endif
	// end 2013-04-18 by jhseol,bckim 이달의 아머 - 서버 시작시 m_pMonthlyArmorManager 객체 생성 및 이벤트 로드

	// 2007-01-22 by dhjin, PC방 IP 가져오기
	// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList 변경 - 아래와 같이 수정
	//m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdatePCBangList, NULL, 0, NULL);
	m_pAtumDBManager->MakeQueryWithNewThread(DQT_UpdatePCBangList, NULL, NULL);	// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList 변경 - 

	// 2006-09-20 by dhjin
	m_pAllGuildItemManager = new CAllGuildItemManager;

	// Load RareItemInfo...
	bRet = CAtumDBHelper::LoadRareItemInfo(&odbcStmt, &m_mapRareItemInfo, NULL, &m_mapLevel2RareItemInfo);
	if (!bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::LoadRareItemInfo() Failed!!!\r\n");
		return FALSE;
	}

	// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 아래와 같이 Send2PreAndIMOfAlivePacket() 로 대체
	//	// 2008-02-14 by cmkwon, Pre,IM Server로 Alive 패킷을 전송한다.
	//	msgTy		= T_FP_CONNECT_ALIVE;
	//	this->Send2PreServerW((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	//	msgTy		= T_FI_CONNECT_ALIVE;
	//	this->Send2IMServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	this->Send2PreAndIMOfAlivePacket();		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 

	// Load EnchantInfo...
	bRet = CAtumDBHelper::LoadEnchantInfo(&odbcStmt, &m_mapEnchantInfo);
	if (!bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::LoadEnchantInfo() Failed!!!\r\n");
		return FALSE;
	}

	// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 아래와 같이 Send2PreAndIMOfAlivePacket() 로 대체
	//	// 2008-02-14 by cmkwon, Pre,IM Server로 Alive 패킷을 전송한다.
	//	msgTy		= T_FP_CONNECT_ALIVE;
	//	this->Send2PreServerW((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	//	msgTy		= T_FI_CONNECT_ALIVE;
	//	this->Send2IMServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	this->Send2PreAndIMOfAlivePacket();		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 

	// Load EnchantInfo...
	bRet = CAtumDBHelper::LoadItemMixingInfo(&odbcStmt, &m_vectorItemMixingInfo);
	if (!bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::LoadItemMixingInfo() Failed!!!\r\n");
		return FALSE;
	}

	// 2006-02-07 by cmkwon, Load Each InfluenceType Count
	bRet = CAtumDBHelper::LoadEachInflueceTypeCount(&m_nNormalInfluenceCount, &m_nVCNInfluenceCount, &m_nANIInfluenceCount, &odbcStmt);
	if (FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::LoadEachInflueceTypeCount() Failed!!!\r\n");
		return FALSE;
	}

	// 2009-02-10 by cmkwon, 세력 유저 카운트 관련 수정 - 로딩 후 시스템 로그 추가
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] Normal user(%4d) BCU user(%4d) ANI user(%4d) \r\n", m_nNormalInfluenceCount, m_nVCNInfluenceCount, m_nANIInfluenceCount);


	// 2007-05-11 by cmkwon, 미스테리아이템캡슐 드랍 아이템 리스트 로딩
	bRet = CAtumDBHelper::LoadMysteryItemDropInfo(&m_mtmapMysteryItemDropList, &odbcStmt, &m_mapItemInfo);
	if (FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::LoadMysteryItemDropInfo() Failed!!!\r\n");
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to LoadMysteryItemDropInfo(%d) \r\n", m_mtmapMysteryItemDropList.size());


	ATUM_DATE_TIME CurrentTime;
	CurrentTime.SetCurrentDateTime();
	this->CheckLuckyItemPeriod(&CurrentTime);

	// 2008-11-10 by dhjin, 럭키머신
	bRet = CAtumFieldDBManager::DBLoadLuckyMachine(&odbcStmt, &m_vectLuckyMachine);
	if (FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::DBLoadLuckyMachine() Failed!!!\r\n");
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadLuckyMachine(%d) \r\n", m_vectLuckyMachine.size());

	// 2007-10-29 by dhjin, 레벨에 따른 여러 행동들 값 정보 로딩
	bRet = CAtumFieldDBManager::DBLoadActionByLevel(&odbcStmt, &m_vectSACTION_BY_LEVEL_DB);
	if (FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: CAtumDBHelper::DBLoadActionByLevel() Failed!!!\r\n");
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadActionByLevel(%d) \r\n", m_vectSACTION_BY_LEVEL_DB.size());

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon, 세력전 정보
	m_InflWarManager.InitCInflWarManager(this);

	vectDB_INFLUENCE_WAR_INFO tmvectWarInfo;
	bRet = CAtumDBHelper::LoadInfluenceWarInfo(&odbcStmt, &tmvectWarInfo);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumDBHelper::LoadInfluenceWarInfo_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to LoadInfluenceWarInfo(%d) \r\n", tmvectWarInfo.size());

	for (int i = 0; i < tmvectWarInfo.size(); i++)
	{
		if (0 < tmvectWarInfo[i].BossMonsterSummonTerm)
			tmvectWarInfo[i].BossMonsterSummonTerm = 5;

		m_InflWarManager.InsertInfluenceWarInfo(&tmvectWarInfo[i]);
	}

	BYTE byOwnerInflOfConflictArea = INFLUENCE_TYPE_UNKNOWN;
	bRet = CAtumFieldDBManager::LoadInfluenceWarData(&byOwnerInflOfConflictArea, &odbcStmt, &m_InflWarManager);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumFieldDBManager::LoadInfluenceWarData_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to LoadInfluenceWarData \r\n");

	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, 선전 포고 - 밑과 같이 수정
//	if(INFLUENCE_TYPE_UNKNOWN != byOwnerInflOfConflictArea)
//	{// 2006-07-12 by cmkwon
//		this->ChangeMapInfluenceType2AllConflictAreaW(GET_OTHER_MONSTER_BELL_BY_CHARACTER_INFLTYPE(byOwnerInflOfConflictArea));
//	}
	bRet = CAtumFieldDBManager::LoadDeclarationOfWarInfo(&odbcStmt, &m_InflWarManager);
	if (FALSE == bRet)
	{


		MessageBox(NULL, "Can't set period of DeclarationOfWar !! Check period of DeclarationOfWar with AdminTool.", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	if (this->m_InflWarManager.m_DeclarationOfWar.GetOwnerInflOfConflictArea(INFLUENCE_TYPE_VCN))
	{
		this->ChangeMapInfluenceType2AllConflictAreaW(GET_OTHER_MONSTER_BELL_BY_CHARACTER_INFLTYPE(INFLUENCE_TYPE_VCN), TRUE);
	}
	if (this->m_InflWarManager.m_DeclarationOfWar.GetOwnerInflOfConflictArea(INFLUENCE_TYPE_ANI))
	{
		this->ChangeMapInfluenceType2AllConflictAreaW(GET_OTHER_MONSTER_BELL_BY_CHARACTER_INFLTYPE(INFLUENCE_TYPE_ANI), TRUE);
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to LoadDeclarationOfWarInfo \r\n");

	// 2007-10-29 by dhjin, 선거 일정 로딩
	bRet = CAtumFieldDBManager::DBLoadPollDate(&odbcStmt, &m_InflWarManager);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumFieldDBManager::DBLoadPollDate fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadPollDate \r\n");

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-12 by cmkwon, m_InflWarManager.SetLeaderGuildUID() 함수에서 쿼리를 하므로 그전에 초기화 해야한다.
// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//	if (!m_GuildWarManager.InitGuildWarMananger(this, g_pFieldGlobal->GetODBCDSN(), g_pFieldGlobal->GetODBCUID(), g_pFieldGlobal->GetODBCPASSWORD()))
	if (!m_GuildWarManager.InitGuildWarMananger(this, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle()))
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldIOCP::IOCPInit_3) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		MessageBox(NULL, "m_GuildWarManager initialize fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	m_InflWarManager.GameStartSetLeader_SubLeaderGuildUID();	// 2007-09-07 by dhjin, 지도자, 여단장이 부지도자인 여단 UID설정


	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to InitGuildWarMananger \r\n");

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-23 by dhjin, Arena 초기화
	m_ArenaManager.InitArenaManager(this);
	bRet = CAtumFieldDBManager::DBLoadArenaInfo(&odbcStmt, &m_ArenaManager);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumFieldDBManager::LoadArenaInfo_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadArenaInfo \r\n");

	bRet = CAtumFieldDBManager::DBLoadArenaMapInfo(&odbcStmt, &m_ArenaManager);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumFieldDBManager::LoadArenaMapInfo_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadArenaMapInfo \r\n");

	//////////////////////////////////////////////////////////////////////////
	// 2007-07-06 by dhjin, Tutorial 
	bRet = CAtumFieldDBManager::DBLoadTutorialInfo(&odbcStmt, &m_vectTutorial);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumFieldDBManager::LoadTutorialInfo_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
	if (FALSE == g_pFieldGlobal->IsArenaServer())
	{
		bRet = CAtumFieldDBManager::DBLoadInitializedGuildList(&odbcStmt);
		if (FALSE == bRet)
		{
			MessageBox(NULL, "CAtumFieldDBManager::DBLoadInitializedGuildList_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
			return FALSE;
		}
	}
	// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그. End


	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadTutorialInfo \r\n");

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-16 by dhjin, 전진기지 설정하기
	m_OutPostManager.InitOutPostManager(this);
	bRet = CAtumFieldDBManager::DBLoadOutPostInfo(&odbcStmt, &m_OutPostManager);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumFieldDBManager::LoadOutPostInfo_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadOutPostInfo \r\n");

	bRet = CAtumFieldDBManager::DBLoadOutPostNextWarTimeInfo(&odbcStmt, &m_OutPostManager);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumFieldDBManager::LoadOutPostNextWarTimeInfo_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadOutPostNextWarTimeInfo \r\n");

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-21 by dhjin, CityLeader 설정하기 
	m_CityLeader.InitCityLeader(this);
	bRet = CAtumFieldDBManager::DBLoadCityLeaderInfo(&odbcStmt, &m_CityLeader);
	if (FALSE == bRet)
	{
		MessageBox(NULL, "CAtumFieldDBManager::LoadCityLeaderInfo_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadCityLeaderInfo \r\n");


	///////////////////////////////////////////////////////////////////////////////
	// 2007-05-28 by cmkwon
	this->LoadResObjCheckList();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	SDBSERVER_GROUP tmDBServGroup;
	util::zero(&tmDBServGroup, sizeof(SDBSERVER_GROUP));
	//tmDBServGroup.ServerGroupID = g_pFieldGlobal->GetMGameServerID();
	util::strncpy(tmDBServGroup.ServerGroupName, g_pFieldGlobal->GetFieldServerGroupName());
	bRet = CAtumFieldDBManager::DBLoadDBServerGroup(&odbcStmt, &tmDBServGroup);
	if (FALSE == bRet)
	{
		util::strncpy(tmDBServGroup.ServerGroupName, g_pFieldGlobal->GetServerGroupName(), SIZE_MAX_SERVER_NAME);
		tmDBServGroup.LimitUserCount = SIZE_MAX_INIT_LIMIT_USER_COUNT_PER_SERVER_GROUP;
		tmDBServGroup.LockCreateCharacterForNewAccount = FALSE;
		CAtumFieldDBManager::DBUpdateDBServerGroup(&odbcStmt, &tmDBServGroup);
	}
	g_pFieldGlobal->SetDBServerGroup(&tmDBServGroup);

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to DBLoadDBServerGroup, LimitUserCnt(%d) IsLockCreateNewAccount(%d) \r\n", tmDBServGroup.LimitUserCount, tmDBServGroup.LockCreateCharacterForNewAccount);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 로딩
	m_QuestMatching.LoadQuestMatchingAndItemMatching(&odbcStmt);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	vectorTenderInfo tmTenderItemList;
	if (FALSE == CAtumFieldDBManager::DBLoad_TenderList(&odbcStmt, &tmTenderItemList))
	{
		MessageBox(NULL, "call CAtumFieldDBManager::DBLoad_TenderList# fail !!", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	this->SetTenderItemList(&tmTenderItemList);

	// 2010-06-15 by shcho&hslee 펫시스템
	if (FALSE == m_cPetDataManager.LoadDBData(&odbcStmt, &m_mapItemInfo))	// 2011-09-06 by hskim, 파트너 시스템 2차
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] fail to call m_cPetDataManager.LoadDBData() \r\n");
		return FALSE;
	}

	// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 서버로딩 부분 구현
	if (FALSE == m_cDissolutionItemManager.LoadDBDissolutionItemData(&odbcStmt))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] fail to call LoadDBDissolutionItemData() \r\n");
		return FALSE;
	}

	odbcStmt.Clean();				// clean

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-14 by cmkwon

	// Load MonsterInfo...(항상 LoadItemInfo 뒤에 처리되어야함)
	CMonsterDBAccess MonsterDBAccess;
	MonsterDBAccess.GetAllMonsters(m_mapMonsterInfo, &m_mapItemInfo, MONSTER_LOAD_TYPE_SIZE_FOR_CLIENT);

	// Load Monster2Item;
	LoadMonster2Item();


	// Load QuestInfo...
	if (!LoadQuestInfo())
	{
		MessageBox(NULL, STRMSG_S_F2QUEST_0000, "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to LoadQuestInfo \r\n");

	// Set Exchange Message Level
	GSetexchangeMsgPrintLevel(PRINTLEVEL_NO_MSG);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 유효성 체크
	if (FALSE == m_QuestMatching.CheckQuestMatchingAndItemMatching(this))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] fail to call m_QuestMatching.CheckQuestMatchingAndItemMatching() \r\n");
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to CheckQuestMatchingAndItemMatching \r\n");


	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 사용하지 않는 부분임
	// 	// Load MapOwnership, 반드시 맵 로딩 이후에 호출해야 함
	// // 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	// //	if (!m_MapOwnershipManager.InitMapOwnershipManager(this, g_pFieldGlobal->GetODBCDSN(), g_pFieldGlobal->GetODBCUID(), g_pFieldGlobal->GetODBCPASSWORD())
	// 	if (!m_MapOwnershipManager.InitMapOwnershipManager(this, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle())
	// //		|| !m_MapOwnershipManager.LoadMapOwnership()
	// 	)
	// 	{
	// 		MessageBox(NULL, "m_MapOwnershipManager initialize fail", "ERROR - SpaceCowboy FieldServer", NULL);
	// 		return FALSE;
	// 	}

	// 2007-09-12 by cmkwon, 위쪽으로 이동함
	//	if (!m_GuildWarManager.InitGuildWarMananger(this, g_pFieldGlobal->GetODBCDSN(), g_pFieldGlobal->GetODBCUID(), g_pFieldGlobal->GetODBCPASSWORD()))
	//	{
	//		MessageBox(NULL, "m_GuildWarManager initialize fail", "ERROR - SpaceCowboy FieldServer", NULL);
	//		return FALSE;
	//	}

	if (!m_DropItemManager.InitDropItemManager(this, m_pAtumDBManager))
	{
		MessageBox(NULL, "m_DropItemManager initialize fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to InitDropItemManager \r\n");

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//	if(FALSE == m_cityWarManager.InitCityWarManager(this, g_pFieldGlobal->GetODBCDSN(), g_pFieldGlobal->GetODBCUID()
	//		, g_pFieldGlobal->GetODBCPASSWORD(), CAtumSJ::GetVectPtrALL_MAP_INFO()))
	if (FALSE == m_cityWarManager.InitCityWarManager(this, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle(), CAtumSJ::GetVectPtrALL_MAP_INFO()))
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldIOCP::IOCPInit_4) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		MessageBox(NULL, "m_cityWarManager initialize fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# success to InitCityWarManager \r\n");

	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 삭제됨
	//	m_pCouponManager = new CCouponManager;
	//	if(FALSE == m_pCouponManager->InitCouponManager(this, g_pFieldGlobal->GetODBCDSN(), g_pFieldGlobal->GetODBCUID()
	//		, g_pFieldGlobal->GetODBCPASSWORD()))
	//	{
	//		MessageBox(NULL, "CouponManager initialize fail", "ERROR - SpaceCowboy FieldServer", NULL);
	//		return FALSE;
	//	}

	m_pHappyHourEventManager = new CHappyHourEventManager(this, &m_gameEventManager);
	m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadHappyHourEvent, NULL, 0, NULL);

	m_pDGAEventManager = new CDGAEventManager(this);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
	m_pAtumDBManager->MakeQueryWithNewThread(DQT_LoadInfluenceRate, NULL, NULL);

	// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 아래와 같이 Send2PreAndIMOfAlivePacket() 로 대체
	//	// 2008-02-14 by cmkwon, Pre,IM Server로 Alive 패킷을 전송한다.
	//	msgTy		= T_FP_CONNECT_ALIVE;
	//	this->Send2PreServerW((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	//	msgTy		= T_FI_CONNECT_ALIVE;
	//	this->Send2IMServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	this->Send2PreAndIMOfAlivePacket();		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 

	if (FALSE == g_pFieldGlobal->IsArenaServer()
		&& FALSE == g_pFieldGlobal->GetIsJamboreeServer())
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - CFieldIOCP::IOCPInit()에서 월드랭킹 정보 로딩
		m_pAtumDBManager->MakeQueryWithNewThread(DQT_ReloadWRKServiceList, NULL, NULL);

		// 2010-01-14 by cmkwon, 월드랭킹 리스트 리로드 구조 변경 - DQT_ReloadWRKServiceList 처리 후에 자동으로 호출되게 수정
		//m_pAtumDBManager->MakeQueryWithNewThread(DQT_ReloadWRKLevel, NULL, NULL, NULL, TRUE);
		//m_pAtumDBManager->MakeQueryWithNewThread(DQT_ReloadWRKFame, NULL, NULL, NULL, TRUE);
		//m_pAtumDBManager->MakeQueryWithNewThread(DQT_ReloadWRKPVP, NULL, NULL, NULL, TRUE);
	}

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	// 	////////////////////////////////////////////////////////////////////////////////
	// 	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender 로딩
	// 	m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadTenderInfo, NULL, 0, NULL);

		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
	m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadInfinityShopInfo, NULL, 0, NULL);

	// 2010-08-05 by dhjin, 버닝맵 -
	m_pAtumDBManager->MakeAndEnqueueQuery(QT_LoadBurningMap, NULL, 0, NULL);

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# End \r\n");

	this->LimitedEReset();		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 

	///////////////////////////////////////////////////////////////////////////////
	// start 2011-06-22 by hskim, 사설 서버 방지
	//m_pSecurityManager = new CSecurityManager();

	//if (FALSE == m_pSecurityManager->InitSecurityManager())
	//{
	//	g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] SecurityManager_Init fail !! \r\n");

	//	MessageBox(NULL, "SecurityManager fail !!", "ERROR - FieldServer", NULL);

	//	return FALSE;
	//}
	// end 2011-06-22 by hskim, 사설 서버 방지

	///////////////////////////////////////////////////////////////////////////////
	// start 2012-01-16 by hskim, 통계 - 화패

#ifdef S_STATISTICS_HSKIM
	if (FALSE == g_pFieldGlobal->IsArenaServer())
	{
		m_pStatisticsManager = new CStatisticsManager();

		if (FALSE == m_pStatisticsManager->Create(this))
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] StatisticsManager Create fail !! \r\n");

			MessageBox(NULL, "StatisticsManager fail !!", "ERROR - FieldServer", NULL);

			return FALSE;
		}
	}
#endif
	// end 2012-01-16 by hskim, 통계 - 화패

	LoadItemSecurityMaximunUID();	// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지

	// 2009-07-02 by cmkwon, 서버 실행시 실행 관련 로그 추가 - 
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::IOCPInit# End \r\n");

	m_bServerActive = TRUE;				// 2013-03-13 by hskim, 웹 캐시 상점

	return CIOCP::IOCPInit();
}

void CFieldIOCP::IOCPClean(void)
{
	m_bServerActive = FALSE;				// 2013-03-13 by hskim, 웹 캐시 상점

	CFieldIOCPSocket *pSock = NULL;

	for (int i = 0; i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock != NULL && pSock->IsUsing() && pSock->m_PeerSocketType == ST_CLIENT_TYPE)
		{
			pSock->CharacterSaveCriticalData(TRUE);	// 먼저 DB에 정보 저장하고
			pSock->m_bFlagDBStore = FALSE;			// Close()로 인해 호출되는 GameEndRoutine()을 무효로 하기 위함
			pSock->ResetAllVariables(RVT_ON_CLOSE);	// 모든 변수 리셋하고
			pSock->Close();							// 모든 클라이언트 연결을 종료
		}
	}

	DBGOUT("===============================\r\n");
	DBGOUT("===== User Data Saving Done....\r\n");
	DBGOUT("===============================\r\n");

	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 삭제됨
	//	if(m_pCouponManager){				m_pCouponManager->CleanCouponManager();}
	if (m_pRacingDemon) { m_pRacingDemon->CleanRacingDemon(); }
	if (m_pAtumDBManager) { m_pAtumDBManager->Clean(); }
	if (m_pFieldTickManager) { m_pFieldTickManager->CleanThread(); }
	//if (m_pSecurityManager) { m_pSecurityManager->CleanThread(); } // 2011-06-22 by hskim, 사설 서버 방지
	if (m_pStatisticsManager) { m_pStatisticsManager->CleanThread(); } // 2011-06-22 by hskim, 사설 서버 방지

	///////////////////////////////////////////////////////////////////////////////
	// 
	CIOCP::IOCPClean();


	m_mtvectItemPtrKillMarks.clearLock();		// 2006-02-09 by cmkwon
	ez_map<int, ITEM*>::iterator itr = m_mapItemInfo.begin();
	while (itr != m_mapItemInfo.end())
	{
		delete(itr->second);
		itr++;
	}
	m_mapItemInfo.clear();

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
	for (auto& x : m_PartyFormationItemSkillList) delete x.second;
	
	m_PartyFormationItemSkillList.clear();

	CleanMonster2Item();
	CleanQuestInfo();

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 사용하지 않는 부분임
	//	m_MapOwnershipManager.CleanMapOwnershipManager();

	m_GuildWarManager.CleanGuildWarMananger();
	m_DropItemManager.CleanDropItemManager();
	m_cityWarManager.CleanCityWarManager();
	CleanNpcScripts(m_NpcScriptsMap);

	/////////////////////////////////////////////////////////////////////////////////////
	// Deleted 된 Party 메모리 삭제
	m_mtvectorDeletedFieldParties.lock();
	for (auto x : m_mtvectorDeletedFieldParties) delete x;
	m_mtvectorDeletedFieldParties.clear();
	m_mtvectorDeletedFieldParties.unlock();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-05 by cmkwon
	// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 사용하지 않음
	//this->SecurityClean();
	CFieldIOCPSocket::SecurityServerClean();		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 
}

SThreadInfo *CFieldIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	SThreadInfo* pTInfo = CIOCP::CheckIOCPThread(i_dwThreadIdToExclude);
	if (!pTInfo)
	{
		pTInfo = m_pAtumDBManager->CheckDBThread(i_dwThreadIdToExclude);
	}
	if (pTInfo
		&& !m_MonitorIOCPSocketPtrVector.empty())
	{
		INIT_MSG_WITH_BUFFER(MSG_FM_SERVER_THREAD_STATUS, T_FM_SERVER_THREAD_STATUS, pSend, SendBuf);
		pSend->dwThreadId = pTInfo->dwThreadId;
		util::strncpy(pSend->szThreadComment, pTInfo->GetThreadCheckComment(pTInfo->enThreadCheckType), SIZE_MAX_THREAD_COMMENT);
		pSend->dwTimeGap = timeGetTime() - pTInfo->dwLastUseStartTick;
		SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_FM_SERVER_THREAD_STATUS));

		INIT_MSG(MSG_FA_ADMIN_CHANGE_SERVER_STATE, T_FA_ADMIN_CHANGE_SERVER_STATE, msgChangeSS, SendBuf);
		msgChangeSS->ServerID = SERVER_ID(g_pFieldGlobal->GetPublicIPLocal(), m_nListenerPort);
		msgChangeSS->ServerType = ST_FIELD_SERVER;
		msgChangeSS->ServerState = SERVER_STATE_ABNORMAL;
		SendMessageToAdminTool(SendBuf, MSG_SIZE(MSG_FA_ADMIN_CHANGE_SERVER_STATE));
	}
	return pTInfo;
}

void CFieldIOCP::CalcTotalTrafficInfo(void)
{
	if (m_pFieldMapWorkspace)
	{
		m_pFieldMapWorkspace->CalcAllMapTotalTraffic();
	}

	CIOCP::CalcTotalTrafficInfo();			// 상위 클래스의 멤버함수를 호출해준다.
}

///////////////////////////////////////////////////////////////////////////////
// 자신 주위에 이동 정보를 전송한다.
// - 자신이 몬스터 일 경우 ==> 다른 몬스터에게는 이동 정보를 전송하지 않는다.
void CFieldIOCP::SendInRangeCharacterMoveOK(CFieldIOCPSocket *pFIOCPSock
	, BYTE *pData
	, int Size
	, CFieldMapChannel *pFieldMapChannel
	, DWORD i_dwSendCounts)
{
	if (FALSE == pFIOCPSock->IsUsing()
		|| NULL == pFieldMapChannel)
	{
		return;
	}

	int nSendCounts = 0;
	// 2005-09-23 by cmkwon
	//	vectorClientIndex*	pVectCltIdxSend		= &pFIOCPSock->m_mtvectClientIdxForSend;
	//	vectorClientIndex*	pVectCltIdxExclude	= &pFIOCPSock->m_mtvectClientIdxForExcludeSend;

	vectorClientIndex	vectCltIdxSend;		vectCltIdxSend.reserve(50);
	vectorClientIndex	vectCltIdxExclude;	vectCltIdxExclude.reserve(10);
	vectorClientIndex*	pVectCltIdxSend = &vectCltIdxSend;
	vectorClientIndex*	pVectCltIdxExclude = &vectCltIdxExclude;

	if (IS_VALID_CLIENT_INDEX(pFIOCPSock->m_peerP2PPKClientIndex))
	{
		pVectCltIdxExclude->push_back(pFIOCPSock->m_peerP2PPKClientIndex);

		CFieldIOCPSocket * ptmFISocket = GetFieldIOCPSocket(pFIOCPSock->m_peerP2PPKClientIndex);
		if (ptmFISocket)
		{
			ptmFISocket->SendAddData(pData, Size);
			nSendCounts++;
		}
	}

	// 2005-07-08 by cmkwon
	//	if(0 == i_dwSendCounts%2)
	//	{
	//		return;
	//	}

		///////////////////////////////////////////////////////////////////////////////
		// 2. i_dwSendCounts가 홀수 이면 Move를 i_pVectorCIdx와 주위 캐릭 모두에게 전송한다.	
	D3DXVECTOR3 vec3CurPos = pFIOCPSock->GetCharacter()->PositionVector;

	CFieldParty *pFParty = pFIOCPSock->m_pFieldParty;
	if (pFParty)
	{// 파티원에게 MoveOK를 전송함

// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - CFieldIOCP::SendInRangeCharacterMoveOK(), 아래와 같이 수정 함
// 		pFParty->m_ListPartyMember.lock();
// 		int nSize = pFParty->m_ListPartyMember.size();
// 		for(int i=0; i < nSize; i++)
// 		{
// 			CFieldIOCPSocket *pSocket = pFParty->m_ListPartyMember[i].PM_CharacterSocket;
// 			if (pSocket
// 				&& pSocket != pFIOCPSock
// 				&& pSocket->IsValidCharacter(FALSE)
// 				&& pSocket->m_character.ClientIndex != pFIOCPSock->m_peerP2PPKClientIndex
// 				&& pSocket->m_pCurrentFieldMapChannel == pFIOCPSock->m_pCurrentFieldMapChannel
// 				&& FALSE == COMPARE_BODYCON_BIT(pSocket->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK))
// 			{
// 				pVectCltIdxExclude->push_back(pSocket->m_character.ClientIndex);
// 				pSocket->SendAddData(pData, Size);
// 				nSendCounts++;
// 			}
// 		}
// 		pFParty->m_ListPartyMember.unlock();
		///////////////////////////////////////////////////////////////////////////////		
		// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - CFieldIOCP::SendInRangeCharacterMoveOK(), 아래와 같이 수정 함
		vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - CFieldIOCP::SendInRangeCharacterMoveOK()
		if (pFIOCPSock->GetPartyMemberList(&vectCFISockList))
		{
			vectCFieldIOCPSocket::iterator itr(vectCFISockList.begin());
			for (; itr != vectCFISockList.end(); itr++)
			{
				CFieldIOCPSocket *pFISoc = *itr;
				if (pFISoc
					&& pFISoc != pFIOCPSock
					&& pFISoc->IsValidCharacter(FALSE)
					&& FALSE == COMPARE_BODYCON_BIT(pFISoc->m_character.BodyCondition, BODYCON_EVENT_HANDLE_MASK)
					&& pFISoc->m_character.ClientIndex != pFIOCPSock->m_peerP2PPKClientIndex
					&& pFISoc->m_pCurrentFieldMapChannel == pFIOCPSock->m_pCurrentFieldMapChannel)
				{
					pVectCltIdxExclude->push_back(pFISoc->m_character.ClientIndex);
					pFISoc->SendAddData(pData, Size);
					nSendCounts++;
				}
			}
		}
	}

	if (pFIOCPSock->m_character.GuildUniqueNumber)
	{// 길드원 처리, check 향후 추가해야함

	}

	ClientIndex_t excludeIdx = pFIOCPSock->GetCharacter()->ClientIndex;
	///////////////////////////////////////////////////////////////////////////////
	// 주위의 캐릭 ClientIndex를 구한다
	if (0 == pFieldMapChannel->GetAdjacentCharacterIndexes(&vec3CurPos, pFieldMapChannel->GetUserVisibleDiameterW()
		, pVectCltIdxSend, excludeIdx))
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// PK 상대와 파티원, 길드원에게 MoveOK를 전송한다
	for (int i = 0; i < pVectCltIdxExclude->size(); i++)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 주위 전송 리스트에서 제거한다
		pVectCltIdxSend->erase(remove(pVectCltIdxSend->begin(), pVectCltIdxSend->end(), (*pVectCltIdxExclude)[i])
			, pVectCltIdxSend->end());
	}
	nSendCounts += this->SendInRangeClientIndexVector(pVectCltIdxSend, pData, Size, excludeIdx);

	if (nSendCounts)
	{
		pFieldMapChannel->AddMapSendTraffic(nSendCounts * Size, nSendCounts);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendInArenaCharacterMoveOK(CFieldIOCPSocket *pFIOCPSock, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, DWORD i_dwSendCounts)
/// \brief		아레나 맵에 있을 때 같은 세력 유저에게 모두 위치 정보를 보낸다.
/// \author		dhjin
/// \date		2007-04-26 ~ 2007-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendInArenaCharacterMoveOK(CFieldIOCPSocket *pFIOCPSock, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, DWORD i_dwSendCounts)
{
	if (FALSE == pFIOCPSock->IsValidCharacter(FALSE)
		|| NULL == pFieldMapChannel
		|| FALSE == IS_MAP_INFLUENCE_ARENA(pFieldMapChannel->GetMapInfluenceTypeW()))
	{
		return;
	}

	int nSendCounts = 0;

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-26 by dhjin, 같은 맵에 있는 전체 유저 리스트를 가져온다.
	vectorClientIndex	tmAllClientIndex;
	pFieldMapChannel->GetAllClientIndexList(&tmAllClientIndex);

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-26 by dhjin, 같은 세력 유저에게 정보를 전송
	D3DXVECTOR3		vec3Center = pFIOCPSock->m_character.PositionVector;
	vec3Center.y = 0.0f;		// 2007-05-17 by cmkwon, 수평거리로 계산하기 위해서
	vectorClientIndex::iterator itr = tmAllClientIndex.begin();
	for (; itr != tmAllClientIndex.end(); itr++)
	{
		CFieldIOCPSocket* pSendFISoc = GetFieldIOCPSocket(*itr);
		if (NULL == pSendFISoc
			|| FALSE == pSendFISoc->IsValidCharacter(FALSE)
			|| pFIOCPSock == pSendFISoc)						// 2007-05-10 by cmkwon, 자신은 제외
		{
			continue;
		}

		//////////////////////////////////////////////////////////////////////////
		// 2007-06-18 by dhjin, 관전자이고 ARENA_STATE_FIGHTING_WARING일때만 세력 구분없이 정보를 보낸다.
		if (pSendFISoc->m_ArenaInfo.WatchState
			&& ARENA_STATE_FIGHTING_WARING == pSendFISoc->m_ArenaInfo.State)
		{
			pSendFISoc->SendAddData(pData, Size);
			nSendCounts++;
			continue;
		}

		if (pFIOCPSock->m_character.InfluenceType != pSendFISoc->m_character.InfluenceType)
		{
			D3DXVECTOR3		tmVec3 = pSendFISoc->m_character.PositionVector;
			tmVec3.y = 0.0f;		// 2007-05-17 by cmkwon, 수평거리로 계산하기 위해서
			float fDist = D3DXVec3Length(&(vec3Center - tmVec3));
			if (pFieldMapChannel->GetUserVisibleDiameterW() / 2.0f < fDist)
			{// 2007-05-10 by cmkwon, 다른세력 유저는 수평거리를 체크하여 전송한다
				continue;
			}
		}

		pSendFISoc->SendAddData(pData, Size);
		nSendCounts++;
	}

	if (nSendCounts)
	{
		pFieldMapChannel->AddMapSendTraffic(nSendCounts * Size, nSendCounts);
	}
}

void CFieldIOCP::SendInRangeCharacterOtherInfo(CFieldIOCPSocket *i_pFISock, CFieldMapChannel *pFieldMapChannel)
{
	if (IS_TUTORIAL_MAP_INDEX(pFieldMapChannel->GetMapChannelIndex().MapIndex))
	{
		return;
	}

	// 2005-09-23 by cmkwon
	//	vectorClientIndex	*pVectIdxSend = &i_pFISock->m_mtvectClientIdxForSend;

	vectorClientIndex	vectCltIdxSend;		vectCltIdxSend.reserve(50);
	vectorClientIndex*	pVectIdxSend = &vectCltIdxSend;

	const CHARACTER		*pCharacter = i_pFISock->GetCharacter();
	if (0 == pFieldMapChannel->GetAdjacentCharacterIndexes((D3DXVECTOR3 *)&pCharacter->PositionVector
		, pFieldMapChannel->GetUserVisibleDiameterW(), pVectIdxSend, pCharacter->ClientIndex))
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 자신에게 주위 모든 캐릭의 정보를 전송한다
	i_pFISock->Send2MeCharacterOtherInfo(pVectIdxSend);

	///////////////////////////////////////////////////////////////////////////////
	// 자신의 정보를 주위 모든  캐릭에게 보내주기, stealth 상태이면 보내지 않음
// 2006-11-08 by cmkwon, 아래와 같이 수정함(IsInvisibleCharacter 변수로 처리 함)
//	if (FALSE == COMPARE_BODYCON_BIT(pCharacter->BodyCondition, BODYCON_STEALTH_MASK))
	if (FALSE == i_pFISock->IsInvisibleCharacter())
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_GET_OTHER_INFO_OK, T_FC_CHARACTER_GET_OTHER_INFO_OK, pMsgOtherInfo, SendBuf);
		pMsgOtherInfo->CharacterInfo = *pCharacter;
		memcpy(pMsgOtherInfo->CharacterInfo.szCharacterMent, i_pFISock->m_szCharacterMent, SIZE_STRING_32);
		pMsgOtherInfo->CharacterInfo.CityWarTeamType = i_pFISock->m_byCityWarTeamType;
		pMsgOtherInfo->CharacterRenderInfo = i_pFISock->m_UnitRenderInfo;
		this->SendInRangeClientIndexVector(pVectIdxSend, SendBuf, MSG_SIZE(MSG_FC_CHARACTER_GET_OTHER_INFO_OK));

		INIT_MSG(MSG_FC_BAZAAR_INFO_OK, T_FC_BAZAAR_INFO_OK, pSBazaarInfo, SendBuf);
		if (i_pFISock->MakeMSG_FC_BAZAAR_INFO_OKW(pSBazaarInfo))
		{// 2006-07-26 by cmkwon, 개인 상점 정보 전송
			this->SendInRangeClientIndexVector(pVectIdxSend, SendBuf, MSG_SIZE(MSG_FC_BAZAAR_INFO_OK));
		}

		int nSendByte = 0;
		if (i_pFISock->Make_MSG_FC_BATTLE_DROP_DUMMY_OK(SendBuf, &nSendByte, SIZE_MAX_PACKET))
		{// 2006-12-04 by dhjin, 체프 정보 전송
			this->SendInRangeClientIndexVector(pVectIdxSend, SendBuf, nSendByte);
		}
	}
}


//void CFieldIOCP::SendInRangeBattleAttackResult(ClientIndex_t idx, BYTE* pData, int Size, CFieldMapChannel *pFieldMapChannel)
//{
//	// stealth 상태이면 return함
//	if (idx < MONSTER_CLIENT_INDEX_START_NUM &&
//		COMPARE_BODYCON_BIT(((CFieldIOCPSocket*)m_ArrayClient[idx])->m_character.BodyCondition, BODYCON_STEALTH_MASK))
//	{
//		return;
//	}
//
//	if (pFieldMapChannel->m_pFieldMapProject->m_bTutorialMap)
//	{
//		// 자기 자신에게만 보냄
//		GetFieldIOCPSocket(idx)->SendAddData(pData, Size);
//		return;
//	}
//
//	int nSize;
//	float x, z;
//	vector<ClientIndex_t> ClientIndexVector;
//	vector<ClientIndex_t>* pClientIndexVector = NULL;
//	IOCPWorkerTLSDATA	*pTLSData = GetIOCPWorkerTLSDATA();
//
//	if(NULL == pTLSData)
//	{
//		ClientIndexVector.reserve(100);
//		pClientIndexVector = &ClientIndexVector;
//	}
//	else
//	{
//		pClientIndexVector = &pTLSData->clientIndexVector;
//	}
//
//	if(FALSE == GetUnitPlanePositonByClientIndex(pFieldMapChannel, idx, &x, &z))
//	{
//		return;
//	}
//
//	nSize = pFieldMapChannel->GetAdjacentCharacterIndexes(x, z, SIZE_VISIBLERECT_X, pClientIndexVector);
//
//	int nSendCounts = 0;
//	while (nSendCounts < nSize )
//	{
//		CFieldIOCPSocket *pFISocket = GetFieldIOCPSocket((*pClientIndexVector)[nSendCounts]);
//		if(pFISocket)
//		{
//			pFISocket->SendBattleAttackResult(pData, Size);
//		}
//		nSendCounts++;
//	}
//
//	pFieldMapChannel->AddMapSendTraffic(nSendCounts * Size, nSendCounts);
//}

//void CFieldIOCP::SendInRangeBattleAttackFindResult(ClientIndex_t idx, BYTE* pData, int Size, CFieldMapChannel *pFieldMapChannel)
//{
//	// stealth 상태이면 return함
//	if (idx < MONSTER_CLIENT_INDEX_START_NUM &&
//		COMPARE_BODYCON_BIT(((CFieldIOCPSocket*)m_ArrayClient[idx])->m_character.BodyCondition, BODYCON_STEALTH_MASK))
//	{
//		return;
//	}
//
//	if (pFieldMapChannel->m_pFieldMapProject->m_bTutorialMap)
//	{
//		// 자기 자신에게만 보냄
//		GetFieldIOCPSocket(idx)->SendAddData(pData, Size);
//		return;
//	}
//
//	int nSize;
//	float x, z;
//	vector<ClientIndex_t> ClientIndexVector;
//	vector<ClientIndex_t>* pClientIndexVector = NULL;
//	IOCPWorkerTLSDATA	*pTLSData = GetIOCPWorkerTLSDATA();
//
//	if(NULL == pTLSData)
//	{
//		ClientIndexVector.reserve(100);
//		pClientIndexVector = &ClientIndexVector;
//	}
//	else
//	{
//		pClientIndexVector = &pTLSData->clientIndexVector;
//	}
//
//	if(FALSE == GetUnitPlanePositonByClientIndex(pFieldMapChannel, idx, &x, &z))
//	{
//		return;
//	}
//
//	nSize = pFieldMapChannel->GetAdjacentCharacterIndexes(x, z, SIZE_VISIBLERECT_X, pClientIndexVector);
//
//	int nSendCounts = 0;
//	while ( nSendCounts < nSize )
//	{
//		CFieldIOCPSocket *pFISocket = GetFieldIOCPSocket((*pClientIndexVector)[nSendCounts]);
//		if(pFISocket)
//		{
//			pFISocket->SendBattleAttackFindResult(pData, Size);
//		}
//		nSendCounts++;
//	}
//
//	pFieldMapChannel->AddMapSendTraffic(nSendCounts * Size, nSendCounts);
//}

void CFieldIOCP::SendInRangeChangeBodyCondition(ClientIndex_t idx
	, CFieldMapChannel *pFieldMapChannel
	, int i_nRangeSize)
{
	// stealth 상태이면 return함
	if (IS_VALID_CLIENT_INDEX(idx))
	{
		CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCPSocket(idx);
		if (NULL == pFieldIOCPSocket
			|| FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE)
			// 2006-11-08 by cmkwon, 아래와 같이 수정함(IsInvisibleCharacter 변수로 처리 함)
			//			|| COMPARE_BODYCON_BIT(pFieldIOCPSocket->m_character.BodyCondition, BODYCON_STEALTH_MASK))
			|| pFieldIOCPSocket->IsInvisibleCharacter())
		{
			return;
		}
	}

	int nSize;
	float x, z;
	vector<ClientIndex_t> ClientIndexVector;
	vector<ClientIndex_t>* pClientIndexVector = NULL;
	IOCPWorkerTLSDATA	*pTLSData = GetIOCPWorkerTLSDATA();

	if (NULL == pTLSData)
	{
		ClientIndexVector.reserve(100);
		pClientIndexVector = &ClientIndexVector;
	}
	else
	{
		pClientIndexVector = &pTLSData->clientIndexVector;
	}

	if (FALSE == GetUnitPlanePositonByClientIndex(pFieldMapChannel, idx, &x, &z))
	{
		return;
	}

	// Create Message
	MSG_FC_CHARACTER_CHANGE_BODYCONDITION	*pMsgChangeBodyCon;
	BYTE									SendBuf[SIZE_MAX_PACKET];
	int										nMsgSize;

	if (idx < MONSTER_CLIENT_INDEX_START_NUM)
	{
		CFieldIOCPSocket			*pFieldIOCPSocket;
		pFieldIOCPSocket = GetFieldIOCPSocket(idx);
		if (pFieldIOCPSocket == NULL || pFieldIOCPSocket->GetClientState() != CS_PLAYING) { return; }

		*(MessageType_t*)SendBuf = T_FC_CHARACTER_CHANGE_BODYCONDITION;
		pMsgChangeBodyCon = (MSG_FC_CHARACTER_CHANGE_BODYCONDITION*)(SendBuf + SIZE_FIELD_TYPE_HEADER);

		pMsgChangeBodyCon->ClientIndex = idx;
		pMsgChangeBodyCon->BodyCondition = pFieldIOCPSocket->GetCharacter()->BodyCondition;
		nMsgSize = MSG_SIZE(MSG_FC_CHARACTER_CHANGE_BODYCONDITION);
	}
	else
	{
		CFieldMonster	*pMonster;

		pMonster = pFieldMapChannel->GetFieldMonster(idx, 1);
		if (!pMonster || pMonster->m_enMonsterState == MS_NULL)
		{
			return;
		}

		*(MessageType_t*)SendBuf = T_FC_CHARACTER_CHANGE_BODYCONDITION;
		pMsgChangeBodyCon = (MSG_FC_CHARACTER_CHANGE_BODYCONDITION*)(SendBuf + SIZE_FIELD_TYPE_HEADER);

		pMsgChangeBodyCon->ClientIndex = idx;
		pMsgChangeBodyCon->BodyCondition = pMonster->BodyCondition;
		nMsgSize = MSG_SIZE(MSG_FC_CHARACTER_CHANGE_BODYCONDITION);
	}

	// Send Message
	nSize = pFieldMapChannel->GetAdjacentCharacterIndexes(x, z, i_nRangeSize, pClientIndexVector);

	int nSendCounts = SendInRangeClientIndexVector(pClientIndexVector, SendBuf, nMsgSize);
	//	int nSendCounts = 0; 
	//	while (nSendCounts < nSize) 
	//	{ 
	//		((CFieldIOCPSocket*)m_ArrayClient[(*pClientIndexVector)[nSendCounts]])->SendAddData(SendBuf, nMsgSize); 
	//		nSendCounts++; 
	//	} 

	pFieldMapChannel->AddMapSendTraffic(nSendCounts * nMsgSize, nSendCounts);
}

class for_each_functor_SendInRangeMonsterMoveOK
{
public:
	for_each_functor_SendInRangeMonsterMoveOK(CFieldIOCP * i_ptFIOCP, BYTE *i_pData, int i_nDataSize, int * i_pSendCounts)
		:m_ptmFIOCP(i_ptFIOCP), m_pData(i_pData), m_nDataSize(i_nDataSize), m_pSendCounts(i_pSendCounts)
	{
	};
	void operator()(ClientIndex_t index)
	{
		CFieldIOCPSocket * ptmFIS = m_ptmFIOCP->GetFieldIOCPSocket(index);
		if (ptmFIS
			&& ST_CLIENT_TYPE == ptmFIS->m_PeerSocketType
			&& FALSE == COMPARE_BODYCON_BIT(ptmFIS->GetCharacter()->BodyCondition, BODYCON_EVENT_HANDLE_MASK)
			&& ptmFIS->SendAddData(m_pData, m_nDataSize))
		{
			(*m_pSendCounts)++;
		}
	}

	CFieldIOCP				*m_ptmFIOCP;
	BYTE					*m_pData;
	int						m_nDataSize;
	int						*m_pSendCounts;
};

void CFieldIOCP::SendInRangeMonsterMoveOK(CFieldMonster *i_pFMonster
	, BYTE *pData
	, int Size
	, CFieldMapChannel *pFieldMapChannel
	, USHORT i_usSendRange)
{
	vectorClientIndex *pVectCltIdxSend = &i_pFMonster->m_mtVectCltIdxForMonsterMoveOK;
	vectorClientIndex *pVectCltIdxExclude = &i_pFMonster->m_mtVectCltIdxForExcludeMonsterMoveOK;

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티맵에서는 몬스터 정보를 계속 보낸다. 밑과 같이 수정
// 	if(0 == pFieldMapChannel->GetAdjacentCharacterIndexes(&i_pFMonster->PositionVector, i_usSendRange, pVectCltIdxSend))
// 	{
// 		return;
// 	}
	if (TRUE == IS_MAP_INFLUENCE_INFINITY(pFieldMapChannel->GetMapInfluenceTypeW())) {
		if (FALSE == pFieldMapChannel->GetAllClientIndexList(pVectCltIdxSend)) {
			return;
		}
	}
	else if (0 == pFieldMapChannel->GetAdjacentCharacterIndexes(&i_pFMonster->PositionVector, i_usSendRange, pVectCltIdxSend))
	{
		return;
	}

	if (false == pVectCltIdxExclude->empty())
	{// 주변 영역 리스트에서 제외항목을 제거한다

		for (int i = 0; i < pVectCltIdxExclude->size(); i++)
		{
			pVectCltIdxSend->erase(remove(pVectCltIdxSend->begin(), pVectCltIdxSend->end(), (*pVectCltIdxExclude)[i])
				, pVectCltIdxSend->end());
		}
	}

	if (pVectCltIdxSend->empty())
	{
		return;
	}

	int nSendCounts = 0;
	for_each(pVectCltIdxSend->begin(), pVectCltIdxSend->end(), for_each_functor_SendInRangeMonsterMoveOK(this, pData, Size, &nSendCounts));
	pFieldMapChannel->AddMapSendTraffic(nSendCounts * Size, nSendCounts);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendInRangeMessageExactAroundPosition(D3DXVECTOR3 pos, BYTE *pData, int Size, int i_fRadius, CFieldMapChannel *pFieldMapChannel, ClientIndex_t i_ExcludeCliIdx/*=0*/, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/)
/// \brief		
/// \author		dhjin
/// \date		2006-11-27 ~ 2006-11-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendInRangeMessageExactAroundPosition(D3DXVECTOR3 pos, BYTE *pData, int Size, int i_fRadius, CFieldMapChannel *pFieldMapChannel, ClientIndex_t i_ExcludeCliIdx/*=0*/, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/)
{
	if (NULL == pFieldMapChannel) { return; }


	vector<ClientIndex_t> ClientIndexVector;
	vector<ClientIndex_t>* pClientIndexVector = NULL;
	IOCPWorkerTLSDATA	*pTLSData = GetIOCPWorkerTLSDATA();

	if (NULL == pTLSData)
	{
		ClientIndexVector.reserve(100);
		pClientIndexVector = &ClientIndexVector;
	}
	else
	{
		pClientIndexVector = &pTLSData->clientIndexVector;
	}

	if (0 == pFieldMapChannel->GetExactCharacterIndexes(pClientIndexVector, &pos, i_fRadius, i_ExcludeCliIdx))
	{
		return;
	}

	int nSendCounts = this->SendInRangeClientIndexVector(pClientIndexVector, pData, Size, i_ExcludeCliIdx);
	pFieldMapChannel->AddMapSendTraffic(nSendCounts * Size, nSendCounts);
}

// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CFieldIOCP::SendInRangeMessageAroundCharacter, 인자추가(i_nStartCityMapIdx)
void CFieldIOCP::SendInRangeMessageAroundCharacter(ClientIndex_t idx, BYTE *pData, int Size
	, CFieldMapChannel *pFieldMapChannel, BOOL bIncludeMe
	, int i_nRangeSize
	, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/
	, MapIndex_t i_nStartCityMapIdx/*=VCN_CITY_MAP_INDEX*/)
{
	if (NULL == pFieldMapChannel) { return; }

	float x, z;
	vector<ClientIndex_t> ClientIndexVector;
	vector<ClientIndex_t>* pClientIndexVector = NULL;
	IOCPWorkerTLSDATA	*pTLSData = GetIOCPWorkerTLSDATA();

	if (NULL == pTLSData)
	{
		ClientIndexVector.reserve(100);
		pClientIndexVector = &ClientIndexVector;
	}
	else
	{
		pClientIndexVector = &pTLSData->clientIndexVector;
	}
	pClientIndexVector->clear();

	if (FALSE == GetUnitPlanePositonByClientIndex(pFieldMapChannel, idx, &x, &z))
	{
		return;
	}

	if (FALSE == IS_TUTORIAL_MAP_INDEX(pFieldMapChannel->GetMapChannelIndex().MapIndex))
	{
		// 2009-11-23 by cmkwon, 자신포함 데이터 전송 보내기 버그 수정 - 편대대형 스킬 버그 수정 
		// 		if (0 == pFieldMapChannel->GetAdjacentCharacterIndexes(x, z, i_nRangeSize, pClientIndexVector, (bIncludeMe==FALSE?idx:0)))
		// 		{
		// 			return;
		// 		}
				// 2009-11-23 by cmkwon, 자신포함 데이터 전송 보내기 버그 수정 - 
		if (0 == pFieldMapChannel->GetAdjacentCharacterIndexes(x, z, i_nRangeSize, pClientIndexVector, idx)
			&& FALSE == bIncludeMe)
		{
			return;
		}
		if (bIncludeMe)
		{
			pClientIndexVector->push_back(idx);			// 2009-11-23 by cmkwon, 자신포함 데이터 전송 보내기 버그 수정 - 
		}
	}
	else
	{
		if (FALSE == bIncludeMe) { return; }
		pClientIndexVector->push_back(idx);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CFieldIOCP::SendInRangeMessageAroundCharacter#, 
	int nSendCounts = this->SendInRangeClientIndexVector(pClientIndexVector, pData, Size, (bIncludeMe == FALSE ? idx : 0), i_byInfluenceMask, i_nStartCityMapIdx);
	pFieldMapChannel->AddMapSendTraffic(nSendCounts * Size, nSendCounts);
}


void CFieldIOCP::SendInRangeMessageAroundPosition(D3DXVECTOR3 pos, BYTE *pData, int Size
	, CFieldMapChannel *pFieldMapChannel
	, ClientIndex_t clientIndexToExclude/*=0*/
	, BOOL i_bCharacterFlag/*=TRUE*/)
{
	vector<ClientIndex_t> ClientIndexVector;
	vector<ClientIndex_t>* pClientIndexVector = NULL;
	IOCPWorkerTLSDATA	*pTLSData = GetIOCPWorkerTLSDATA();

	if (NULL == pTLSData)
	{
		ClientIndexVector.reserve(100);
		pClientIndexVector = &ClientIndexVector;
	}
	else
	{
		pClientIndexVector = &pTLSData->clientIndexVector;
	}
	int nRangeSize = pFieldMapChannel->GetUserVisibleDiameterW();
	if (FALSE == i_bCharacterFlag)
	{
		nRangeSize = pFieldMapChannel->GetMonsterVisibleDiameterW();
	}
	if (0 == pFieldMapChannel->GetAdjacentCharacterIndexes(pos.x, pos.z, nRangeSize, pClientIndexVector, clientIndexToExclude))
	{
		return;
	}

	int nSendCounts = this->SendInRangeClientIndexVector(pClientIndexVector, pData, Size, clientIndexToExclude);
	pFieldMapChannel->AddMapSendTraffic(nSendCounts * Size, nSendCounts);
}


//void CFieldIOCP::SendInRangeMessageAroundPositionWithIndex(D3DXVECTOR3 pos, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, ClientIndex_t idx)
//{
//	vector<ClientIndex_t> ClientIndexVector;
//	vector<ClientIndex_t>* pClientIndexVector = NULL;
//	IOCPWorkerTLSDATA	*pTLSData = GetIOCPWorkerTLSDATA();
//
//	if(NULL == pTLSData)
//	{
//		ClientIndexVector.reserve(100);
//		pClientIndexVector = &ClientIndexVector;
//	}
//	else
//	{
//		pClientIndexVector = &pTLSData->clientIndexVector;
//	}
//
//	if(0 == pFieldMapChannel->GetAdjacentCharacterIndexes(pos.x, pos.z, SIZE_VISIBLERECT_X, pClientIndexVector))
//	{
//		return;
//	}
//
//	vector<ClientIndex_t>::iterator itr(pClientIndexVector->begin());
//	while (itr != pClientIndexVector->end())
//	{
//		if(*itr != idx)
//		{
//			m_ArrayClient[*itr]->SendAddData(pData, Size);
//		}
//		itr++;
//	}
//}

// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - SendInRangeClientIndexVector, 인자추가(i_nStartCityMapIdx)
int CFieldIOCP::SendInRangeClientIndexVector(vectorClientIndex *i_pVectorCIdx
	, BYTE *i_pData
	, int i_nDataLen
	, ClientIndex_t i_excludeIdx/*=0*/
	, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/
	, MapIndex_t i_nStartCityMapIdx/*=VCN_CITY_MAP_INDEX*/)
{
	CFieldIOCPSocket	*pFISocket = NULL;
	int					nSentCounts = 0;
	int					nSize = i_pVectorCIdx->size();
	for (int i = 0; i < nSize; i++)
	{
		if ((*i_pVectorCIdx)[i] != i_excludeIdx)
		{
			pFISocket = this->GetFieldIOCPSocket((*i_pVectorCIdx)[i]);
			if (pFISocket
				&& pFISocket->IsCheckInfluenceTypeANDSocketType(i_byInfluenceMask, ST_CLIENT_TYPE, TRUE, i_nStartCityMapIdx)	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CFieldIOCP::SendInRangeClientIndexVector#, // 2005-12-07 by cmkwon, 세력체크 - 맵채팅, 지역채팅을 위한 체크
				&& pFISocket->SendAddData(i_pData, i_nDataLen))
			{
				nSentCounts++;
			}
		}
	}

	return nSentCounts;
}

int CFieldIOCP::SendInRangeClientIndexVectorAroundPosition(D3DXVECTOR3 pos
	, vectorClientIndex *i_pVectorCIdx
	, BYTE *i_pData
	, int i_nDataLen
	, CFieldMapChannel *pFieldMapChannel
	, int i_nRangeSize
	, BOOL *o_pbExistUnitInMapChannel/*=NULL*/)
{
	CFieldIOCPSocket	*pFISocket = NULL;
	int					nSentCounts = 0;
	int					nSize = i_pVectorCIdx->size();
	int					nUserVisibleDiameter = pFieldMapChannel->GetUserVisibleDiameterW();
	for (int i = 0; i < nSize; i++)
	{
		pFISocket = this->GetFieldIOCPSocket((*i_pVectorCIdx)[i]);
		if (pFISocket
			&& ST_CLIENT_TYPE == pFISocket->m_PeerSocketType
			&& pFISocket->m_character.MapChannelIndex == pFieldMapChannel->GetMapChannelIndex())
		{
			if (o_pbExistUnitInMapChannel)
			{
				*o_pbExistUnitInMapChannel = TRUE;
			}

			//////////////////////////////////////////////////////////////////////////
			// 2007-07-24 by dhjin, 튜토리얼 맵에서는 몬스터 거리를 체크하지 않는다.
			if (IS_TUTORIAL_MAP_INDEX(pFieldMapChannel->GetMapChannelIndex().MapIndex)
				|| IS_MAP_INFLUENCE_INFINITY(pFieldMapChannel->GetMapInfluenceTypeW()))		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티맵에서는 몬스터 정보를 계속 보낸다. 밑과 같이 수정
			{
				pFISocket->SendAddData(i_pData, i_nDataLen);
				nSentCounts++;
				continue;
			}

			if (nUserVisibleDiameter / 2 > D3DXVec3Length(&(pFISocket->m_character.PositionVector - pos))
				&& pFISocket->SendAddData(i_pData, i_nDataLen))
			{
				nSentCounts++;
			}
		}
	}
	return nSentCounts;
}

int CFieldIOCP::SendString128ToClientIndexVector(vectorClientIndex *i_pVectorCIdx, BYTE string128_type, char *szString)
{
	CFieldIOCPSocket	*pFISocket = NULL;
	int					nSentCounts = 0;
	int					nSize = i_pVectorCIdx->size();
	for (int i = 0; i < nSize; i++)
	{
		pFISocket = this->GetFieldIOCPSocket((*i_pVectorCIdx)[i]);
		if (pFISocket
			&& ST_CLIENT_TYPE == pFISocket->m_PeerSocketType)
		{
			pFISocket->SendString128(string128_type, szString);
			nSentCounts++;
		}
	}
	return nSentCounts;
}

// 2006-07-21 by cmkwon, 인자 추가 - BYTE i_byMinLevel, BYTE i_byMaxLevel
// 2006-07-24 by cmkwon, 인자 추가 - MAP_CHANNEL_INDEX *i_pExcludeMapChannel
// 2006-08-29 by cmkwon, 인자 추가 - BOOL i_bOnlyPCBangClient/*=FALSE*/
// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - , USHORT i_usReqUnitKind=UNITKIND_ALL_MASK
void CFieldIOCP::SendMessageToAllClients(BYTE *pData, int Size
	, BYTE i_byInfluenceMask/*= INFLUENCE_TYPE_ALL_MASK*/
	, BYTE i_byMinLevel/*=0*/
	, BYTE i_byMaxLevel/*=0*/
	, MAP_CHANNEL_INDEX *i_pExcludeMapChannel/*=NULL*/
	, BOOL i_bOnlyPCBangClient/*=FALSE*/
	, BOOL i_bArenaState/*=FALSE*/
	, USHORT i_usReqUnitKind/*=UNITKIND_ALL_MASK*/)
{
	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			&& pSock->IsCheckInfluenceTypeANDSocketType(i_byInfluenceMask, ST_CLIENT_TYPE)
			&& pSock->IsCheckLevel(i_byMinLevel, i_byMaxLevel)
			&& COMPARE_BIT_FLAG(pSock->GetCharacter()->UnitKind, i_usReqUnitKind))	// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 기어타입 인자 추가
		{
			if (i_pExcludeMapChannel
				&& *i_pExcludeMapChannel == pSock->m_character.MapChannelIndex)
			{// 2006-07-24 by cmkwon, 예외 맵의 유저 체크
				continue;
			}

			if (i_bArenaState)
			{// 2007-06-22 by dhjin, 아레나 진행중인지 체크
				if (IS_MAP_INFLUENCE_ARENA(pSock->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW()))
				{
					continue;
				}
			}

			if (FALSE == i_bOnlyPCBangClient)
//				|| pSock->m_bIsPCBangClient)
			{// 2006-08-29 by cmkwon, PCBang 유저 체크 추가
				pSock->SendAddData(pData, Size);
			}

		}
	}
}

void CFieldIOCP::SendString128ToAllClients(BYTE string128_type
	, char *szString
	, BOOL i_bWriteLog/*=FALSE*/
	, BYTE i_byInfluenceMask/*= INFLUENCE_TYPE_ALL_MASK*/)
{
	if (i_bWriteLog)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "%s\r\n", szString);
	}

	CFieldIOCPSocket *pSock = NULL;
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			&& pSock->IsCheckInfluenceTypeANDSocketType(i_byInfluenceMask, ST_CLIENT_TYPE))
		{
			pSock->SendString128(string128_type, szString);
		}
	}
}

void CFieldIOCP::SendWorldNotification(char* szString, BOOL bSpecialChat/*Write it into Leadechat or not?*/)
{
	CFieldIOCPSocket *pSock = NULL;
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE))
		{
			pSock->SendNotification(bSpecialChat, szString);
		}
	}
}

int CFieldIOCP::SendMessageToGuildMembers(BYTE *pData, int Size, UID32_t i_guildUID, UID32_t i_excludeCharUID)
{
	int nRetCounts = 0;
	CFieldIOCPSocket *pFISock = NULL;
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pFISock = GetFieldIOCPSocket(i);
		if (pFISock
			&& pFISock->IsValidCharacter(FALSE)
			&& i_guildUID == pFISock->m_character.GuildUniqueNumber
			&& i_excludeCharUID != pFISock->m_character.CharacterUniqueNumber)
		{
			pFISock->SendAddData(pData, Size);
			nRetCounts++;
		}
	}

	return nRetCounts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::SendGuildSummonMessage(BYTE *pData, int Size, UID32_t i_guildUID, UID32_t i_excludeCharUID, CFieldMapProject *i_pFMPro)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-09 ~ 2006-08-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::SendGuildSummonMessage(BYTE *pData, int Size, UID32_t i_guildUID, UID32_t i_excludeCharUID, CFieldMapProject *i_pFMPro)
{
	int nRetCounts = 0;
	CFieldIOCPSocket *pFISock = NULL;
	for (int i = CLIENT_INDEX_START_NUM; i < m_dwArrayClientSize; i++)
	{
		pFISock = GetFieldIOCPSocket(i);
		if (pFISock
			&& i_guildUID == pFISock->m_character.GuildUniqueNumber
			&& i_excludeCharUID != pFISock->m_character.CharacterUniqueNumber
			&& ERR_NO_ERROR == pFISock->IsEnableWarp(i_pFMPro->m_nMapInfluenceType, i_pFMPro->IsConflictAreaMap(), i_pFMPro->m_nQuestIndexForWarp))
		{
			pFISock->SendAddData(pData, Size);
			nRetCounts++;
		}
	}

	return nRetCounts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendMessageToCityClients(BYTE *pData, int Size, BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-19 ~ 2006-04-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendMessageToCityClients(BYTE *pData, int Size, BYTE i_byInflTy)
{
	CFieldMapChannel *pFMChann = m_pFieldMapWorkspace->GetCityFieldMapChannelPtrByInfluence(i_byInflTy);
	pFMChann->SendMessageToAllInChannel(pData, Size);
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::Send2PreServerW(BYTE *i_pData, int i_nDataLen)
/// \brief		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - CFieldIOCP::Send2PreServerW() 추가
/// \author		cmkwon
/// \date		2008-01-31 ~ 2008-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::Send2PreServerW(BYTE *i_pData, int i_nDataLen)
{
	return g_pFieldGlobal->Send2PreServer(i_pData, i_nDataLen);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::Send2IMServer(void *i_pData, int i_nDataLen)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::Send2IMServer(void *i_pData, int i_nDataLen)
{
	if (NULL == m_pIMWinSocket
		|| FALSE == m_pIMWinSocket->IsConnected())
	{
		return FALSE;
	}

	return m_pIMWinSocket->Write((BYTE*)i_pData, i_nDataLen);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::Send2NPCServerByTCP(BYTE *i_pbyData, int i_nDataLen)
/// \brief		// 2007-08-22 by cmkwon, 함수명 변경(<-Send2NPCServer)
/// \author		cmkwon
/// \date		2006-04-18 ~ 2006-04-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::Send2NPCServerByTCP(BYTE *i_pbyData, int i_nDataLen)
{
	if (NULL == m_pNPCTCPSocket
		|| FALSE == m_pNPCTCPSocket->IsUsing())
	{
		return FALSE;
	}

	return m_pNPCTCPSocket->SendAddData(i_pbyData, i_nDataLen);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::Send2PreAndIMOfAlivePacket(void)
/// \brief		// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - CFieldIOCP::Send2PreAndIMOfAlivePacket() 추가
/// \author		cmkwon
/// \date		2008-02-25 ~ 2008-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::Send2PreAndIMOfAlivePacket(void)
{
	MessageType_t msgTy;
	msgTy = T_FP_CONNECT_ALIVE;
	this->Send2PreServerW((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	msgTy = T_FI_CONNECT_ALIVE;
	this->Send2IMServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
}

BOOL CFieldIOCP::OpenUDPPortForOtherServer(void)
{
	//*
	CIOCPSocket *pIOCPSocket = NULL;
	CMapProject *pMapProject = NULL;

	for (int i = 0; i < m_pFieldMapWorkspace->GetMapProjectCounts(); i++)
	{
		pMapProject = m_pFieldMapWorkspace->GetMapProjectByIndex(i);
		if (NULL == pMapProject)
		{
			break;
		}

		// 이 맵에서 서비스되지 않으면 break
		SERVER_ID tmpID;
		tmpID.SetValue(pMapProject->m_strFieldIP, pMapProject->m_sFieldListenPort);
		if (tmpID != m_FieldServerID)
		{
			continue;
		}

		pIOCPSocket = FindEmptyIOCPSocket(i);
		if (pIOCPSocket == NULL)
		{
			char	szError[1024];
			sprintf(szError, "CFieldIOCP::OpenUDPPortForOtherServer error, i[%d]\r\n", i);
			g_pFieldGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return FALSE;
		}

		if (pIOCPSocket->OpenUDPPort(pMapProject->m_sFieldUDPPort) == FALSE)
		{
			return FALSE;
		}
		pIOCPSocket->SetPeerAddress(pMapProject->m_strNPCIP, pMapProject->m_sNPCUDPPort);
		sprintf((char*)((CFieldIOCPSocket*)pIOCPSocket)->GetCharacter()->CharacterName, "NPC_Server_%d", i);

		char	szSystemLog[256];
		sprintf(szSystemLog, "  UDP Opened for NPC_Server_%d(MapIndex[%4d] IP[%15s], Port[%d])\r\n",
			i, pMapProject->m_nMapIndex, pMapProject->m_strNPCIP, pMapProject->m_sNPCUDPPort);
		g_pFieldGlobal->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);

		HANDLE hret = CreateIoCompletionPort((HANDLE)pIOCPSocket->m_hSocket, m_hCompletionPort, (DWORD)pIOCPSocket, 0);
		if (NULL == hret)
		{
			pIOCPSocket->Close(0x14000);

			int nErr = GetLastError();
			SetLastError(0);
			char	szError[1024];
			sprintf(szError, "CNPCIOCP::OpenUDPPortForOtherServer CreateIoCompletionPort() LastError[%d] hCompletionPort[%p] pIOCPSocket[%p]\r\n"
				, nErr, m_hCompletionPort, pIOCPSocket);
			g_pFieldGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return FALSE;
		}
		pIOCPSocket->Read();
		((CFieldIOCPSocket*)pIOCPSocket)->InitializeNPCMap(pMapProject, NULL);
	}

	// 2009-03-26 by cmkwon, 시스템 로그 추가 - 
	g_pGlobal->WriteSystemLogEX(TRUE, "  [Notify] Open all UDP Socket for NPCServer!! Count(%3d)\r\n", m_pFieldMapWorkspace->GetMapProjectCounts());

	return TRUE;
}

BOOL CFieldIOCP::CloseUDPPortForOtherServer(void)
{
	// 2009-03-26 by cmkwon, 시스템 로그 추가 - 
	g_pGlobal->WriteSystemLogEX(TRUE, "  [Notify] Close all UDP Socket for NPCServer!! \r\n");

	//*
	CIOCPSocket *pIOCPSocket = NULL;
	CMapProject *pMapProject = NULL;

	for (int i = 0; i < SIZE_MAX_UDP_CHANNEL_SESSION; i++)	// 2008-08-29 by cmkwon, FieldServ<->NPCServ 가 UDP 통신 할 채널수 - // 2006-12-07 by cmkwon, 변경(SIZE_MAX_MAP_PER_A_FIELD_SERVER->SIZE_MAX_NPCSERVER_SESSION)
	{
		pMapProject = m_pFieldMapWorkspace->GetMapProjectByIndex(i);
		if (NULL == pMapProject)
		{
			break;
		}

		pIOCPSocket = GetIOCPSocket(i);
		if (pIOCPSocket == NULL)
		{
			char	szError[1024];
			sprintf(szError, "CFieldIOCP::CloseUDPPortForOtherServer error, i[%d]\r\n", i);
			g_pFieldGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			break;
		}

		if (pIOCPSocket->IsUsing())
		{
			((CFieldIOCPSocket*)pIOCPSocket)->m_pFieldMapProjectForNPC->ResetAllChannel();
			pIOCPSocket->Close(0x04000);
		}
	}
	return TRUE;
}

void CFieldIOCP::GetCurrentServerTimeInAtumDateTime(ATUM_DATE_TIME &o_AtumDateTime)
{
	/*struct tm *today;
	time_t time = GetCurrentServerTime();
	today = localtime(&time);

	o_AtumDateTime = *today;*/

	o_AtumDateTime.SetCurrentDateTime();
}

// check: MINE류 loading할 때 MAX_RANGE를 설정해야 함
void CFieldIOCP::LoadItemInfo(BOOL bReload)
{
	SIZE_MAX_MINE_REACTION_RANGE = 0.0f;

	SQLHENV		henv = SQL_NULL_HENV;
	SQLHDBC		hdbc = SQL_NULL_HDBC;
	SQLHSTMT	hstmt = SQL_NULL_HSTMT;

	RETCODE		ret;
	SQLINTEGER	cb[CB_COUNT_ITEM];
	ITEM		item;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//	ret = SQLConnect(hdbc, g_pGlobal->GetODBCDSN(), SQL_NTS, g_pGlobal->GetODBCUID(), SQL_NTS, g_pGlobal->GetODBCPASSWORD(), SQL_NTS);
	char szConnectionString[512];
	char szOutConnectionString[512]; SQLSMALLINT pcb;
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
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldIOCP::LoadItemInfo_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_DBC, hdbc, "@CFieldIOCP::LoadItemInfo Failed!\r\n", FALSE);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		return;
	}


	// prints DB Info
	char DatabaseName[40];
	char DSN[40];
	SQLSMALLINT StrLen;
	SQLGetInfo(hdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
	SQLGetInfo(hdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//ret = SQLExecDirect(hstmt, (UCHAR*)"SELECT * FROM ti_Item WITH (NOLOCK)", SQL_NTS);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	ret = SQLExecDirect(hstmt, (UCHAR*)QUERY_080702_0246, SQL_NTS);
	ret = SQLExecDirect(hstmt, (UCHAR*)PROCEDURE_080827_0246, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		// error
		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt, "CFieldIOCP::LoadItemInfo Failed!\n", TRUE);
		return;
	}

	// bind columns
	CAtumDBHelper::BindColITEM(hstmt, item, cb);

	if (!bReload)
	{
		///////////////////////////////////////////////////////////////////////
		// 최초의 loading인 경우
		///////////////////////////////////////////////////////////////////////

		// initialize to 0x00
		memset(&item, 0x00, sizeof(ITEM));
		while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
		{
			if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
			{
				// error
				CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt, "CFieldIOCP::LoadItemInfo Failed!\n", TRUE);
			}

			ITEM *pItem = new ITEM;
			memcpy(pItem, &item, sizeof(ITEM));

			if (COMPARE_BIT_FLAG(pItem->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM)
				&& FALSE == COMPARE_BIT_FLAG(pItem->ItemAttribute, ITEM_ATTR_QUEST_ITEM)
				&& COMPARE_INFLUENCE(pItem->ReqRace, INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI))
			{// 2006-02-10 by cmkwon, 킬마크 체크 - ITEM_ATTR_QUEST_ITEM에 체크가 되어있지 않아야만 항상 드랍되는 아이템이된다.
				m_mtvectItemPtrKillMarks.pushBackLock(pItem);
			}

			m_mapItemInfo.insert(pair<INT, ITEM*>(pItem->ItemNum, pItem));

			if (pItem->Kind == ITEMKIND_MINE && pItem->ReactionRange > SIZE_MAX_MINE_REACTION_RANGE)
			{
				SIZE_MAX_MINE_REACTION_RANGE = pItem->ReactionRange;
			}

			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
			pItem->pInvokingDestParamList = this->m_InvokingItemInfo.GetInvokingWearItemApplyInfoList(pItem->InvokingDestParamID);
			pItem->pInvokingDestParamByUseList = this->m_InvokingItemInfo.GetInvokingWearItemApplyInfoListByUse(pItem->InvokingDestParamIDByUse);

			// initialize to 0x00
			memset(&item, 0x00, sizeof(ITEM));
		}

		// 2009-08-11 by cmkwon, 위치 이동 - 아래의 사항은 음 로딩 시만 적용하면 됨
		// 탄알 구경 정보 loading
		ez_map<INT, ITEM*>::iterator itrBullet = m_mapItemInfo.begin();
		while (itrBullet != m_mapItemInfo.end())
		{
			ITEM *pItemBullet = itrBullet->second;
			if (pItemBullet->Kind == ITEMKIND_BULLET)
			{
				m_multiMapCaliber2Bullet.insert(pair<BYTE, ITEM*>(pItemBullet->Caliber, pItemBullet));
			}
			itrBullet++;
		}
	}
	else
	{
		///////////////////////////////////////////////////////////////////////
		// 변경 사항을 리로딩하는 경우
		///////////////////////////////////////////////////////////////////////
		memset(&item, 0x00, sizeof(ITEM));
		while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
		{
			ITEM *pExistingItem = GetItemInfo(item.ItemNum);
			if (pExistingItem != NULL)
			{
				*pExistingItem = item;
			}
			memset(&item, 0x00, sizeof(ITEM));
		}

		// 모든 client들 에게 item info를 reload하라고 보냄
		MessageType_t msg = T_FC_ITEM_RELOAD_ITEM_INFO;
		SendMessageToAllClients((BYTE*)&msg, SIZE_FIELD_TYPE_HEADER);
	}

	// 2009-08-11 by cmkwon, 위치 이동 - 아래의 사항은 위와 같이 처음 로딩 시만 적용하면 됨
	// 	// 탄알 구경 정보 loading
	// 	ez_map<INT, ITEM*>::iterator itrBullet = m_mapItemInfo.begin();
	// 	while (itrBullet != m_mapItemInfo.end())
	// 	{
	// 		ITEM *pItemBullet = itrBullet->second;
	// 		if (pItemBullet->Kind == ITEMKIND_BULLET)
	// 		{
	// 			m_multiMapCaliber2Bullet.insert(pair<BYTE, ITEM*>(pItemBullet->Caliber, pItemBullet));
	// 		}
	// 		itrBullet++;
	// 	}

	//	ez_map<INT, ITEM*>::iterator itrWeapon = m_mapItemInfo.begin();
	//	while (itrWeapon != m_mapItemInfo.end())
	//	{
	//		ITEM *pItemWeapon = itrWeapon.second;
	//		if (IS_WEAPON(pItemWeapon->Kind))
	//		{
	//			ez_map<INT, ITEM*>::iterator itrBullet = m_mapItemInfo.begin();
	//			while (itrBullet != m_mapItemInfo.end())
	//			{
	//				ITEM *pItemBullet = itrBullet.second;
	//				if (pItemBullet->Kind == ITEMKIND_BULLET)
	//				{
	//					if (pItemBullet->Caliber == pItemWeapon->Caliber)
	//					{
	//					}
	//				}
	//				itrBullet++;
	//			}
	//		}
	//
	//		itrWeapon++;
	//	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	// cleanup odbc resources
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
#ifndef _DEBUG
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
#endif
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void CFieldIOCP::ReloadIteminfo()
{
	// load item info
	CODBCStatement odbcStmt;

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//	odbcStmt.Init((char*)g_pGlobal->GetODBCDSN(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD());
	BOOL bRet = odbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
	if (FALSE == bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldIOCP::ReloadIteminfo_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

	}

	ez_map<INT, ITEM> tmpMapItemInfo;

	CAtumDBHelper::LoadItemInfo(&odbcStmt, &tmpMapItemInfo);

	ez_map<INT, ITEM>::iterator itr = tmpMapItemInfo.begin();
	while (tmpMapItemInfo.end() != itr)
	{
		ITEM &tmpNewItemInfo = itr->second;

		ITEM *pTmpOldItemInfo = GetItemInfo(tmpNewItemInfo.ItemNum);
		if (pTmpOldItemInfo != NULL)
		{
			*pTmpOldItemInfo = tmpNewItemInfo;
		}

		itr++;
	}

	odbcStmt.Clean();

	// Load Monster2Item;
	CleanMonster2Item();
	LoadMonster2Item();

	// Reload Quest
	if (!LoadQuestInfo(TRUE))
	{
		DBGOUT(STRMSG_S_F2QUEST_0001);
	}
}

RARE_ITEM_INFO* CFieldIOCP::GetRareItemInfo(INT nCodeNum)
{
	if (0 >= nCodeNum)
	{// 2008-10-21 by cmkwon, 마지막에 선택된 레어는 다시 나오지 않게 수정 - 체크 추가
		return NULL;
	}

	ez_map<INT, RARE_ITEM_INFO>::iterator itr = m_mapRareItemInfo.find(nCodeNum);
	if (itr == m_mapRareItemInfo.end())
	{
		return NULL;
	}

	return &(itr->second);
}
void CFieldIOCP::GetReqLevelApplyRare(int *o_pMinLevel, int *o_pMaxLevel, ITEM_GENERAL *i_pGItem)
{
	*o_pMinLevel = i_pGItem->ItemInfo->ReqMinLevel;
	*o_pMaxLevel = i_pGItem->ItemInfo->ReqMaxLevel;

	if (0 != i_pGItem->PrefixCodeNum)
	{
		RARE_ITEM_INFO *pRareInfo = this->GetRareItemInfo(i_pGItem->PrefixCodeNum);
		if (pRareInfo)
		{
			for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
			{
				if (DES_REQ_MIN_LEVEL == pRareInfo->DesParameter[i])
				{
					if (0 != i_pGItem->ItemInfo->ReqMinLevel)
					{
						*o_pMinLevel = max(1, *o_pMinLevel + (int)pRareInfo->ParameterValue[i]);
					}
				}
				else if (DES_REQ_MAX_LEVEL == pRareInfo->DesParameter[i])
				{
					if (0 != i_pGItem->ItemInfo->ReqMaxLevel)
					{
						*o_pMaxLevel = min(CHARACTER_MAX_LEVEL, *o_pMaxLevel + (int)pRareInfo->ParameterValue[i]);
					}
				}
				else if (0 == pRareInfo->DesParameter[i])
				{
					break;
				}
			}
		}
	}

	if (0 != i_pGItem->SuffixCodeNum)
	{
		RARE_ITEM_INFO *pRareInfo = this->GetRareItemInfo(i_pGItem->SuffixCodeNum);
		if (pRareInfo)
		{
			for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
			{
				if (DES_REQ_MIN_LEVEL == pRareInfo->DesParameter[i])
				{
					if (0 != i_pGItem->ItemInfo->ReqMinLevel)
					{
						*o_pMinLevel = max(1, *o_pMinLevel + (int)pRareInfo->ParameterValue[i]);
					}
				}
				else if (DES_REQ_MAX_LEVEL == pRareInfo->DesParameter[i])
				{
					if (0 != i_pGItem->ItemInfo->ReqMaxLevel)
					{
						*o_pMaxLevel = min(CHARACTER_MAX_LEVEL, *o_pMaxLevel + (int)pRareInfo->ParameterValue[i]);
					}
				}
				else if (0 == pRareInfo->DesParameter[i])
				{
					break;
				}
			}
		}
	}
}

BOOL CFieldIOCP::LoadQuestInfo(BOOL i_bReload /* = FALSE */)
{
	if (i_bReload)
	{
		//		if (!m_ServicePauseManager.GetQuestServiceState())
		//		{
		//			// 퀘스트 서비스 중에는 리로딩 불가
		//			return FALSE;
		//		}

				// 퀘스트 정보 cleanup
		CleanQuestInfo();

		// 몬스터 아이템도 리로딩
		CleanMonster2Item();
		LoadMonster2Item();
	}
	char	szSysLog[1024];
	sprintf(szSysLog, "\r\n\r\n  CFieldIOCP::LoadQuestInfo() =========================================================> Start\r\n");
	g_pFieldGlobal->WriteSystemLog(szSysLog);
	DbgOut(szSysLog);

	WIN32_FIND_DATA FileData;
	HANDLE hSearch;
	BOOL fFinished = FALSE;
	INT nFiles = 0;

	vector<string> vecQuestFiles;

	hSearch = FindFirstFile((CONFIG_ROOT + string("/quest/q_*.cfg")).c_str(), &FileData);
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		DBGOUT("No quest files found.\n");
		return TRUE;
	}

	while (!fFinished)
	{
		if (strnicmp(QUEST_USER_DEFINED_VARIABLES_FILE_NAME, FileData.cFileName, 128) != 0)
		{
			vecQuestFiles.push_back((CONFIG_ROOT + string("./quest/") + string(FileData.cFileName)).c_str());
		}

		// 다음 파일 검색
		if (!FindNextFile(hSearch, &FileData))
		{
			if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				fFinished = TRUE;
			}
			else
			{
				DBGOUT("Could not find next file.\n");
				return FALSE;
			}
		}
	}

	if (!QuestLoaderParser::ParseFiles((CONFIG_ROOT + string("./quest/") + string(QUEST_USER_DEFINED_VARIABLES_FILE_NAME)).c_str(), &vecQuestFiles, &m_mapQuestInfoFromScript))
	{
		return FALSE;
	}

	sprintf(szSysLog, "    Quest total count: %d\r\n\r\n", m_mapQuestInfoFromScript.size());
	g_pFieldGlobal->WriteSystemLog(szSysLog);
	DbgOut(szSysLog);

	// 퀘스트 로딩 후 처리
	ez_map<INT, CQuest*>::iterator itr = m_mapQuestInfoFromScript.begin();
	while (m_mapQuestInfoFromScript.end() != itr)
	{
		CQuest *pQuest = itr->second;
		if (FALSE == CheckAndInitQuestInfo(pQuest))
		{
			sprintf(szSysLog, "    QuestIndex(%4d) QuestName(\'%30s\') check ==> Error\r\n", pQuest->QuestIndex, pQuest->QuestName);
			g_pFieldGlobal->WriteSystemLog(szSysLog);
			DbgOut(szSysLog);
		}
		else
		{
			sprintf(szSysLog, "    QuestIndex(%4d) QuestName(\'%30s\') check ==> Success\r\n", pQuest->QuestIndex, pQuest->QuestName);
			g_pFieldGlobal->WriteSystemLog(szSysLog);
			//			DbgOut(szSysLog);
		}
		itr++;
	}
	FindClose(hSearch);		// Close the search handle.

	sprintf(szSysLog, "\r\n\r\n  CFieldIOCP::LoadQuestInfo() =========================================================> End\r\n\r\n\r\n");
	g_pFieldGlobal->WriteSystemLog(szSysLog);
	DbgOut(szSysLog);
	return TRUE;
}

void CFieldIOCP::CleanQuestInfo()
{
	map<int, CQuest*>::iterator itrQuest = m_mapQuestInfoFromScript.begin();
	while (itrQuest != m_mapQuestInfoFromScript.end())
	{
		delete(itrQuest->second);
		itrQuest++;
	}
	m_mapQuestInfoFromScript.clear();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuest *CFieldIOCP::GetQuestInfo(int i_nQuestIndex)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-02 ~ 2005-11-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuest *CFieldIOCP::GetQuestInfo(int i_nQuestIndex)
{
	return m_mapQuestInfoFromScript.findEZ(i_nQuestIndex);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::CheckAndInitQuestInfo(CQuest *i_pQuestInfo)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-02 ~ 2005-11-02
/// \warning	퀘스트 유효성 체크와 필요한 정보를 설정한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::CheckAndInitQuestInfo(CQuest *i_pQuestInfo)
{
	BOOL bRet = TRUE;

	///////////////////////////////////////////////////////////////////////////////
	// Start Requirements Check
	if (i_pQuestInfo->ReqLevel.Min > i_pQuestInfo->ReqLevel.Max)
	{
		bRet = FALSE;
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqLevel Error, MinLevel(%d) ~ MaxLevel(%d)"
			, i_pQuestInfo->ReqLevel.Min, i_pQuestInfo->ReqLevel.Max);
	}
	if (i_pQuestInfo->ReqAttackPart.Min > i_pQuestInfo->ReqAttackPart.Max)
	{
		bRet = FALSE;
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqAttackPart Error, MinLevel(%d) ~ MaxLevel(%d)"
			, i_pQuestInfo->ReqAttackPart.Min, i_pQuestInfo->ReqAttackPart.Max);
	}
	if (i_pQuestInfo->ReqDefensePart.Min > i_pQuestInfo->ReqDefensePart.Max)
	{
		bRet = FALSE;
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqDefensePart Error, MinLevel(%d) ~ MaxLevel(%d)"
			, i_pQuestInfo->ReqDefensePart.Min, i_pQuestInfo->ReqDefensePart.Max);
	}
	if (i_pQuestInfo->ReqFuelPart.Min > i_pQuestInfo->ReqFuelPart.Max)
	{
		bRet = FALSE;
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqFuelPart Error, MinLevel(%d) ~ MaxLevel(%d)"
			, i_pQuestInfo->ReqFuelPart.Min, i_pQuestInfo->ReqFuelPart.Max);
	}
	if (i_pQuestInfo->ReqSoulPart.Min > i_pQuestInfo->ReqSoulPart.Max)
	{
		bRet = FALSE;
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqSoulPart Error, MinLevel(%d) ~ MaxLevel(%d)"
			, i_pQuestInfo->ReqSoulPart.Min, i_pQuestInfo->ReqSoulPart.Max);
	}
	if (i_pQuestInfo->ReqShieldPart.Min > i_pQuestInfo->ReqShieldPart.Max)
	{
		bRet = FALSE;
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqShieldPart Error, MinLevel(%d) ~ MaxLevel(%d)"
			, i_pQuestInfo->ReqShieldPart.Min, i_pQuestInfo->ReqShieldPart.Max);
	}
	if (i_pQuestInfo->ReqDodgePart.Min > i_pQuestInfo->ReqDodgePart.Max)
	{
		bRet = FALSE;
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqDodgePart Error, MinLevel(%d) ~ MaxLevel(%d)"
			, i_pQuestInfo->ReqDodgePart.Min, i_pQuestInfo->ReqDodgePart.Max);
	}
	int i;
	for (i = 0; i < i_pQuestInfo->ReqQuestIndexVector.size(); i++)
	{
		if (NULL == this->GetQuestInfo(i_pQuestInfo->ReqQuestIndexVector[i]))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqQuestIndexVector Error, ReqQuestIndex(%d)"
				, i_pQuestInfo->ReqQuestIndexVector[i]);
		}
	}
	for (i = 0; i < i_pQuestInfo->ReqRecvQuestIndexVector.size(); i++)
	{
		if (NULL == this->GetQuestInfo(i_pQuestInfo->ReqRecvQuestIndexVector[i]))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqRecvQuestIndexVector Error, ReqRecvQuestIndexVector(%d)"
				, i_pQuestInfo->ReqRecvQuestIndexVector[i]);
		}
	}
	for (i = 0; i < i_pQuestInfo->ReqItemVector.size(); i++)
	{
		if (NULL == this->GetItemInfo(i_pQuestInfo->ReqItemVector[i].ItemNum))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo ReqItemVector Error, ItemNum(%8d)"
				, i_pQuestInfo->ReqItemVector[i].ItemNum);
		}
	}

	if (0 > i_pQuestInfo->QuestPartyMembers
		|| SIZE_MAX_PARTY_MEMBER < i_pQuestInfo->QuestPartyMembers)
	{// 2006-03-21 by cmkwon, 파티미션 체크
		bRet = FALSE;
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo QuestPartyMembers Error, QuestPartyMembers(%d)"
			, i_pQuestInfo->QuestPartyMembers);
		i_pQuestInfo->QuestPartyMembers = 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	// Start Action Check
	// 퀘스트 시작시 지급 아이템 무게 계산
	i_pQuestInfo->StartPayItemWeight = 0.0f;
	for (i = 0; i < i_pQuestInfo->StartPayItemVector.size(); i++)
	{
		ITEM* pItemInfo = GetItemInfo(i_pQuestInfo->StartPayItemVector[i].ItemNum);
		if (pItemInfo == NULL)
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo StartPayItemVector Error, ItemNum(%8d)"
				, i_pQuestInfo->StartPayItemVector[i].ItemNum);
			continue;
		}
		i_pQuestInfo->StartPayItemWeight += CAtumSJ::GetItemWeight(pItemInfo, i_pQuestInfo->StartPayItemVector[i].Count);
	}
	for (i = 0; i < i_pQuestInfo->StartDelItemVector.size(); i++)
	{
		if (NULL == this->GetItemInfo(i_pQuestInfo->StartDelItemVector[i].ItemNum))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo StartDelItemVector Error, ItemNum(%8d)"
				, i_pQuestInfo->StartDelItemVector[i].ItemNum);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// Completion Requirements
	for (i = 0; i < i_pQuestInfo->TargetItemVector.size(); i++)
	{
		if (NULL == this->GetItemInfo(i_pQuestInfo->TargetItemVector[i].ItemNum))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo TargetItemVector Error, ItemNum(%8d)"
				, i_pQuestInfo->TargetItemVector[i].ItemNum);
		}
	}
	if (0 != i_pQuestInfo->TargetDeadByMonsterUniquenumber)
	{
		if (NULL == this->GetMonsterInfo(i_pQuestInfo->TargetDeadByMonsterUniquenumber))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo TargetDeadByMonsterUniquenumber Error, MonsterUnitKind(%8d)"
				, i_pQuestInfo->TargetDeadByMonsterUniquenumber);
		}
	}
	for (i = 0; i < i_pQuestInfo->TargetMonsterVector.size(); i++)
	{
		if (NULL == this->GetMonsterInfo(i_pQuestInfo->TargetMonsterVector[i].MonsterUniqueNumber))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo TargetMonsterVector Error, MonsterUnitKind(%8d)"
				, i_pQuestInfo->TargetMonsterVector[i].MonsterUniqueNumber);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// Completion Action
	for (i = 0; i < i_pQuestInfo->EndDeleteItemVector.size(); i++)
	{
		if (NULL == this->GetItemInfo(i_pQuestInfo->EndDeleteItemVector[i].ItemNum))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo EndDeleteItemVector Error, ItemNum(%8d)"
				, i_pQuestInfo->EndDeleteItemVector[i].ItemNum);
		}
	}
	// 퀘스트 보상 아이템의 무게 계산
	i_pQuestInfo->EndPayItemWeight = 0.0f;
	for (i = 0; i < i_pQuestInfo->EndPayItemVector.size(); i++)
	{
		ITEM* pItemInfo = GetItemInfo(i_pQuestInfo->EndPayItemVector[i].ItemNum);
		if (pItemInfo == NULL)
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo EndPayItemVector Error, ItemNum(%8d)"
				, i_pQuestInfo->EndPayItemVector[i].ItemNum);
			continue;
		}
		i_pQuestInfo->EndPayItemWeight += CAtumSJ::GetItemWeight(pItemInfo, i_pQuestInfo->EndPayItemVector[i].Count);
	}
	//////////////////////////////////////////////////////////////////////////
	// 2008-12-02 by dhjin, 미션마스터 
	i_pQuestInfo->EndMissionMasterPayItemWeight = 0.0f;
	for (i = 0; i < i_pQuestInfo->EndMissionMasterPayItemVector.size(); i++)
	{
		ITEM* pItemInfo = GetItemInfo(i_pQuestInfo->EndMissionMasterPayItemVector[i].ItemNum);
		if (pItemInfo == NULL)
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo EndMissionMasterPayItemVector Error, ItemNum(%8d)"
				, i_pQuestInfo->EndMissionMasterPayItemVector[i].ItemNum);
			continue;
		}
		i_pQuestInfo->EndMissionMasterPayItemWeight += CAtumSJ::GetItemWeight(pItemInfo, i_pQuestInfo->EndMissionMasterPayItemVector[i].Count);
	}
	for (i = 0; i < i_pQuestInfo->EndRandomPayItemVector.size(); i++)
	{
		QUEST_RANDOM_PAY_ITEM *pRPayItem = &i_pQuestInfo->EndRandomPayItemVector[i];
		for (int j = 0; j < COUNT_RANDOM_PAY_ITEM; j++)
		{
			if (0 != pRPayItem->ItemWithCountProb[j].ItemNum)
			{
				if (NULL == this->GetItemInfo(pRPayItem->ItemWithCountProb[j].ItemNum))
				{
					bRet = FALSE;
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo EndRandomPayItemVector Error, ItemNum(%8d)"
						, pRPayItem->ItemWithCountProb[j].ItemNum);
				}
			}
		}
	}
	for (i = 0; i < i_pQuestInfo->EndPayMixItemVector.size(); i++)
	{
		if (NULL == this->GetMonsterInfo(i_pQuestInfo->EndPayMixItemVector[i].TargetItemNum))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo EndPayMixItemVector Error, TargetItemNum(%8d)"
				, i_pQuestInfo->EndPayMixItemVector[i].TargetItemNum);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 퀘스트용 몬스터 드랍 아이템 설정
	for (i = 0; i < i_pQuestInfo->vecQuestDropItem.size(); i++)
	{
		QUEST_DROP_ITEM *pDropItem = &i_pQuestInfo->vecQuestDropItem[i];
		if (NULL == GetMonsterInfo(pDropItem->MonsterUniqueNumber))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo QuestDropItem MonsterUnitKind Error, MonsterUnitKind(%8d)"
				, pDropItem->MonsterUniqueNumber);
			continue;
		}
		if (NULL == GetItemInfo(pDropItem->ItemNumber))
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo QuestDropItem ItemNum Error, ItemNum(%8d)"
				, pDropItem->ItemNumber);
			continue;
		}
		MONSTER2ITEM *pM2I = new MONSTER2ITEM; util::zero(pM2I, sizeof(MONSTER2ITEM));
		pM2I->MonsterUniqueNumber = pDropItem->MonsterUniqueNumber;
		pM2I->ItemNumber = pDropItem->ItemNumber;
		pM2I->MinCount = pDropItem->MinCount;
		pM2I->MaxCount = pDropItem->MaxCount;
		pM2I->Probability = pDropItem->Probability;
		pM2I->QuestIndex = i_pQuestInfo->QuestIndex;
		m_mapMonster2Item.insert(pair<INT, MONSTER2ITEM*>(pM2I->MonsterUniqueNumber, pM2I));
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-03-03 by cmkwon, 퀘스트용 캐릭터 드랍 아이템 설정 - 세력전 관련
	for (i = 0; i < i_pQuestInfo->vecQuestDropCharacter2Item.size(); i++)
	{
		QUEST_DROP_CHARACTER2ITEM *pDropItem = &i_pQuestInfo->vecQuestDropCharacter2Item[i];
		ITEM * pDropItemInfo = GetItemInfo(pDropItem->Character2Item.ItemNumber);
		if (NULL == pDropItemInfo)
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo_1 QuestDropCharacter2Item ItemNum Error, ItemNum(%8d)"
				, pDropItem->Character2Item.ItemNumber);
			continue;
		}
		if (FALSE == COMPARE_BIT_FLAG(pDropItemInfo->ItemAttribute, ITEM_ATTR_QUEST_ITEM)
			|| FALSE == COMPARE_BIT_FLAG(pDropItemInfo->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM)
			|| pDropItem->InfluenceType0 != pDropItemInfo->ReqRace)
		{
			bRet = FALSE;
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "\r\n	[Error] CFieldIOCP::CheckQuestInfo_2 QuestDropCharacter2Item ItemNum Error, ItemNum(%8d)"
				, pDropItem->Character2Item.ItemNumber);
			continue;
		}
		i_pQuestInfo->vecQuestDropCharacter2Item[i].Character2Item.QuestIndex = i_pQuestInfo->QuestIndex;
		this->InsertCharacterDropItem(i_pQuestInfo->vecQuestDropCharacter2Item[i].InfluenceType0, i_pQuestInfo->vecQuestDropCharacter2Item[i].UnitKindMask0
			, i_pQuestInfo->vecQuestDropCharacter2Item[i].Character2Item);
	}
	return bRet;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CQuest* CFieldIOCP::GetFirstScenarioQuestInfoByInfluenceType(BYTE i_byInflTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-07 ~ 2006-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CQuest* CFieldIOCP::GetFirstScenarioQuestInfoByInfluenceType(BYTE i_byInflTy)
{
	map<int, CQuest*>::iterator itrQuest = m_mapQuestInfoFromScript.begin();
	while (itrQuest != m_mapQuestInfoFromScript.end())
	{
		CQuest *pQuestInfo = itrQuest->second;
		if (pQuestInfo->IsFirstScenarioQuest(i_byInflTy))
		{
			return pQuestInfo;
		}
		itrQuest++;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \class		void CFieldIOCP::SendAllQuestList(CFieldIOCPSocket *i_pTOFISock, BYTE i_byInflTy)
///
/// \brief		
///				// 2009-10-15 by cmkwon, 퀘스트리스트 보내기시에 퀘스트명으로 검색 기능 추가 - 
/// \author		cmkwon
/// \version	
/// \date		2006-08-08 ~ 2006-08-08
/// \warning	
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendAllQuestList(CFieldIOCPSocket *i_pTOFISock, BYTE i_byInflTy, char *i_szTxt/*=NULL*/)
{
	if (FALSE == i_pTOFISock->IsValidCharacter(FALSE))
	{
		return;
	}

	ez_map<INT, CQuest*>::iterator itr = m_mapQuestInfoFromScript.begin();
	while (m_mapQuestInfoFromScript.end() != itr)
	{
		CQuest *pQuest = itr->second;
		if (COMPARE_INFLUENCE(i_byInflTy, pQuest->ReqInfluenceType)
			&& (NULL == i_szTxt || NULL != strstr(pQuest->QuestName, i_szTxt)))	// 2009-10-15 by cmkwon, 퀘스트리스트 보내기시에 퀘스트명으로 검색 기능 추가 - NULL 이 아닐 때 체크 추가
		{
			i_pTOFISock->SendString128(STRING_128_ADMIN_CMD, "QuestIndex(%4d) '%s'", pQuest->QuestIndex, pQuest->QuestName);
		}
		itr++;
	}
}


MONSTER2ITEM* CFieldIOCP::GetMonster2Item(int MonsterUniqueNumber)
{
	multimap<int, MONSTER2ITEM*>::iterator itr = m_mapMonster2Item.find(MonsterUniqueNumber);
	if (itr == m_mapMonster2Item.end())
	{
		return NULL;
	}
	else
	{
		return itr->second;
	}

	// not reachable
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::CreateDropItems(listMonsterDropItem *o_plistMonsterDropItem, CFieldMonster *i_pDeadFMon, CFieldIOCPSocket *i_pFISockTopDamage, float i_fPartyBonus)
/// \brief		
///				// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
				// 2012-02-13 by hskim, 몬스터 아이템 드랍 테스트 명령어
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	// 함수 수정시 TestDropItems() 함수도 같이 수정해주세요 - 2012-02-13 by hskim
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::CreateDropItems(listMonsterDropItem *o_plistMonsterDropItem, CFieldMonster *i_pDeadFMon, CFieldIOCPSocket *i_pFISockTopDamage, float i_fPartyBonus, DropType_t i_dropType/*=DROP_TYPE_DEAD_MONSTER*/)
{
	if (FALSE == this->m_ServicePauseManager.GetQuestServiceState())
	{
		return;
	}

	if (IS_STRATEGYPOINT_MONSTER(i_pDeadFMon->MonsterInfoPtr->Belligerence)
		|| IS_TELEPORT_MONSTER(i_pDeadFMon->MonsterInfoPtr->Belligerence))
	{// 2006-11-21 by cmkwon, 전략포인트 몬스터는 아이템을 드랍하지 않는다, // 2007-11-08 by dhjin, 텔레포트 몬스터 추가
		return;
	}

	multimap<int, MONSTER2ITEM*>::iterator	itr = m_mapMonster2Item.find(i_pDeadFMon->MonsterInfoPtr->MonsterUnitKind);
	if (itr == m_mapMonster2Item.end())
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////	
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	float fDefautlSPIRate = 1.0f;
	float fDefautlDropItemRate = 1.0f;
	float fDefautlDropRareItemRate = 1.0f;

	if (NULL == i_pFISockTopDamage
		|| FALSE == i_pFISockTopDamage->IsValidCharacter(FALSE))
	{
		i_pFISockTopDamage = NULL;
		if (DROP_TYPE_DEAD_ALL_INFINITY_KEY_MONSTER != i_dropType)
		{
			return;
		}
	}
	else
	{
		fDefautlSPIRate = i_pFISockTopDamage->GetPlusRateSPI();
		fDefautlDropItemRate = i_pFISockTopDamage->GetPlusRateDropItem();
		fDefautlDropRareItemRate = i_pFISockTopDamage->GetPlusRateDropRareItem();

		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
		fDefautlDropItemRate *= i_pDeadFMon->MonsterInfoExtend.fIncreaseDropProbabilityRatio;
		fDefautlDropRareItemRate *= i_pDeadFMon->MonsterInfoExtend.fIncreaseDropItemCountRatio;
		// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	}


	float	fDropRateOfGameRestriction = 1.0f;			// 2008-04-08 by cmkwon, 기본값
	BOOL	bNoDropAutoPickUpItem = FALSE;		// 2008-04-08 by cmkwon, 자동습득 아이템 드랍 불가 플래그

	float fPlusSPI = (fDefautlSPIRate + i_fPartyBonus) * fDropRateOfGameRestriction;
	float fPlusDropItem = (fDefautlDropItemRate - 1.0f + i_fPartyBonus) * fDropRateOfGameRestriction;
	float fPlusDropRareItem = fDefautlDropRareItemRate - 1.0f;
	do
	{
		MONSTER2ITEM	*pMonster2Item = itr->second;
		ITEM			*pItem = this->GetItemInfo(pMonster2Item->ItemNumber);

		if (pItem == NULL)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Monster DropItem Error1, MonsterUnitKind(%8d) ItemNumber(%8d)\r\n"
				, i_pDeadFMon->MonsterInfoPtr->MonsterUnitKind, pMonster2Item->ItemNumber);

			itr++;
			continue;
		}

		// 2008-03-28 by cmkwon, 이제 필요 없는 로그임(문제 해결된 상태)
		// 		// 2008-01-31 by cmkwon, 베트남 킬마크 드랍 로그 남기기
		// 		if(COMPARE_BIT_FLAG(pItem->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM|ITEM_ATTR_CASH_ITEM))
		// 		{
		// 			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Monster DropItem Error100, MonsterUnitKind(%8d) ItemNumber(%8d)\r\n"
		// 				, pMonster2Item->MonsterUniqueNumber, pMonster2Item->ItemNumber);
		// 		}

				///////////////////////////////////////////////////////////////////////////////
				// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - DropType이 같은것만 Drop 한다.
		if (i_dropType != pMonster2Item->DropType)
		{
			itr++;
			continue;
		}

		// 2008-04-08 by cmkwon, 게임 제한 시스템 수정 - 자동습득아이템 드랍 하지 않는다.
		if (bNoDropAutoPickUpItem
			&& COMPARE_BIT_FLAG(pItem->ItemAttribute, ITEM_ATTR_AUTO_PICK_UP))
		{
			itr++;
			continue;
		}

		if (0 >= pMonster2Item->Probability)
		{// 2007-12-26 by cmkwon, 드랍 확륙이 0이면 드랍 안되게 처리

			itr++;
			continue;
		}

		int pValue = RAND100K();
		if (pMonster2Item->Probability < pValue)
		{// 생성 실패

			itr++;
			continue;
		}

		if (pMonster2Item->MinCount < 0
			|| pMonster2Item->MaxCount < 0
			|| pMonster2Item->MaxCount < pMonster2Item->MinCount)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Monster DropItem Error2, MonsterUnitKind(%8d) ItemNumber(%8d) MinCount(%4d) MaxCount(%8d)\r\n"
				, i_pDeadFMon->MonsterInfoPtr->MonsterUnitKind, pMonster2Item->ItemNumber, pMonster2Item->MinCount, pMonster2Item->MaxCount);

			itr++;
			continue;
		}

		INT nAmount = RANDI(pMonster2Item->MinCount, pMonster2Item->MaxCount);

		// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 몬스터 밸런스 정보 ( 아이템 드랍개수 증가 ) 적용.
		if (i_pDeadFMon->MonsterInfoExtend.fIncreaseDropItemCountRatio > .0f)
		{
			nAmount *= i_pDeadFMon->MonsterInfoExtend.fIncreaseDropItemCountRatio;

			// 2010. 08. 12 by hsLee 보상 관련 최소 수치 보완.
			nAmount = max(nAmount, 1);
		}
		// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)

		if (0 >= nAmount)
		{
			itr++;
			continue;
		}

		// 2008-04-08 by cmkwon, 게임 제한 시스템 수정 - 퀘템의 다른 처리는 필요 없음
		// 		///////////////////////////////////////////////////////////////////////////////
		// 		// 2006-12-01 by cmkwon, 베트남 정부 정책 적용
		// 		if(bIsNoDropItem			
		// 			&& FALSE == COMPARE_BIT_FLAG(pItem->ItemAttribute, ITEM_ATTR_QUEST_ITEM)
		// 			&& ITEMKIND_QUEST != pItem->Kind)
		// 		{// 2006-12-01 by cmkwon, 퀘스트 아이템 제외
		// 			if(g_pFieldGlobal->InMasangServer())
		// 			{// 2006-01-02 by cmkwon, 테스트 코드
		// 				char szTemp[1024];
		// 				sprintf(szTemp, "	[Notify]: VTC DropItem delete !! Mon(%s:%d) DromItem ==> Item(%s:%d), Counts(%d)\r\n"
		// 					, i_pDeadFMon->MonsterInfoPtr->MonsterName, i_pDeadFMon->MonsterIndex, pItem->ItemName, pItem->ItemNum, nAmount);
		// 				DBGOUT(szTemp);
		// 				g_pFieldGlobal->WriteSystemLog(szTemp);
		// 			}
		// 			itr++;			// 2006-12-08 by cmkwon
		// 			continue;
		// 		}


		g_pFieldGlobal->WriteSystemLogEX(TRUE, "	Mon(%s:%d) DromItem ==> DropType(%d) Item(%s:%d), Counts(%d) \r\n"
			, i_pDeadFMon->MonsterInfoPtr->MonsterName, i_pDeadFMon->MonsterIndex, i_dropType, pItem->ItemName, pItem->ItemNum, nAmount);

		if (IS_COUNTABLE_ITEM(pItem->Kind))
		{// Countable Item인 경우
			ITEM_GENERAL *pNewItem = new ITEM_GENERAL(pItem);
			if (MONEY_ITEM_NUMBER == pItem->ItemNum)
			{
				// 2008-04-08 by cmkwon, 게임 제한 시스템 수정 - 
				//nAmount *= fPlusSPI;
				nAmount = max(1, (int)(nAmount * fPlusSPI));		// 2008-04-08 by cmkwon, 게임 제한 시스템 수정 - 최소 1
			}

			pNewItem->CurrentCount = nAmount;
			MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItem;
			tmpMonsterDropItem.pItemGeneral = pNewItem;
			tmpMonsterDropItem.QuestIndex = pMonster2Item->QuestIndex;
			o_plistMonsterDropItem->push_back(tmpMonsterDropItem);
		}
		else
		{// 나머지 일반 아이템인 경우(Countable Item이 아닌경우)
			for (int i = 0; i < nAmount; i++)
			{
				ITEM_GENERAL *pStoreItem = new ITEM_GENERAL(pItem);
				CFieldIOCPSocket::ApplyRareItem(i_pDeadFMon, pMonster2Item, fPlusDropRareItem
					, CFieldIOCPSocket::ms_pFieldIOCP, pStoreItem);

				//DBGOUT("%d(%#x) ", pStoreItem->ItemNum, pStoreItem);
				MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItem;
				tmpMonsterDropItem.pItemGeneral = pStoreItem;
				tmpMonsterDropItem.QuestIndex = pMonster2Item->QuestIndex;

				o_plistMonsterDropItem->push_back(tmpMonsterDropItem);
			}
		}// end

		itr++;
	} while (itr != m_mapMonster2Item.upper_bound(i_pDeadFMon->MonsterInfoPtr->MonsterUnitKind));

}

// start 2012-02-13 by hskim, 몬스터 아이템 드랍 테스트 명령어
void CFieldIOCP::TestDropItems(MONSTER_INFO *pMonsterInfo, mapMonsterDropItemTest *o_pmapMonsterDropItemTest)
{
	if (NULL == pMonsterInfo || NULL == o_pmapMonsterDropItemTest)
	{
		return;
	}

	if (IS_STRATEGYPOINT_MONSTER(pMonsterInfo->Belligerence)
		|| IS_TELEPORT_MONSTER(pMonsterInfo->Belligerence))
	{
		return;
	}

	multimap<int, MONSTER2ITEM*>::iterator	itr = m_mapMonster2Item.find(pMonsterInfo->MonsterUnitKind);
	if (itr == m_mapMonster2Item.end())
	{
		return;
	}

	float fDefautlSPIRate = 1.0f;
	float fDefautlDropItemRate = 1.0f;
	float fDefautlDropRareItemRate = 1.0f;

	float	fDropRateOfGameRestriction = 1.0f;
	BOOL	bNoDropAutoPickUpItem = FALSE;

	float fPlusSPI = (fDefautlSPIRate /*+ i_fPartyBonus*/)* fDropRateOfGameRestriction;
	float fPlusDropItem = (fDefautlDropItemRate - 1.0f /*+ i_fPartyBonus*/) * fDropRateOfGameRestriction;
	float fPlusDropRareItem = fDefautlDropRareItemRate - 1.0f;

	do
	{
		MONSTER2ITEM	*pMonster2Item = itr->second;
		ITEM			*pItem = this->GetItemInfo(pMonster2Item->ItemNumber);

		if (pItem == NULL)
		{
			itr++;
			continue;
		}

		if (DROP_TYPE_DEAD_MONSTER != pMonster2Item->DropType)
		{
			itr++;
			continue;
		}

		if (bNoDropAutoPickUpItem
			&& COMPARE_BIT_FLAG(pItem->ItemAttribute, ITEM_ATTR_AUTO_PICK_UP))
		{
			itr++;
			continue;
		}

		if (0 >= pMonster2Item->Probability)
		{

			itr++;
			continue;
		}

		int pValue = RAND100K();
		if (pMonster2Item->Probability < pValue)
		{

			itr++;
			continue;
		}

		if (pMonster2Item->MinCount < 0
			|| pMonster2Item->MaxCount < 0
			|| pMonster2Item->MaxCount < pMonster2Item->MinCount)
		{
			itr++;
			continue;
		}

		INT nAmount = RANDI(pMonster2Item->MinCount, pMonster2Item->MaxCount);

		if (0 >= nAmount)
		{
			itr++;
			continue;
		}

		if (o_pmapMonsterDropItemTest != NULL)
		{
			map<INT, MONSTER_DROP_ITEM_TEST_INFO>::iterator itrDropTest = o_pmapMonsterDropItemTest->find(pItem->ItemNum);
			if (itrDropTest == o_pmapMonsterDropItemTest->end())
			{
				MONSTER_DROP_ITEM_TEST_INFO DropItem;
				DropItem.ItemNum = pItem->ItemNum;

				if (MONEY_ITEM_NUMBER == pItem->ItemNum)
				{
					DropItem.CurrentCount = max(1, (int)(nAmount * fPlusSPI));
				}
				else
				{
					DropItem.CurrentCount = nAmount;
				}

				o_pmapMonsterDropItemTest->insert(pair<INT, MONSTER_DROP_ITEM_TEST_INFO>(DropItem.ItemNum, DropItem));
			}
			else
			{
				MONSTER_DROP_ITEM_TEST_INFO *pDropItem = &itrDropTest->second;

				if (MONEY_ITEM_NUMBER == pItem->ItemNum)
				{
					pDropItem->CurrentCount += max(1, (int)(nAmount * fPlusSPI));
				}
				else
				{
					pDropItem->CurrentCount += nAmount;
				}
			}
		}

		itr++;

	} while (itr != m_mapMonster2Item.upper_bound(pMonsterInfo->MonsterUnitKind));

}
// end 2012-02-13 by hskim, 몬스터 아이템 드랍 테스트 명령어

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::GetDropItemList(vectMONSTER2ITEMPtr *o_pVectDropItemList, int i_nMonsterUnitKind)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-21 ~ 2006-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::GetDropItemList(vectMONSTER2ITEMPtr *o_pVectDropItemList, int i_nMonsterUnitKind)
{
	o_pVectDropItemList->clear();		// 2006-11-21 by cmkwon, 초기화

	multimap<int, MONSTER2ITEM*>::iterator	itr = m_mapMonster2Item.find(i_nMonsterUnitKind);
	if (itr == m_mapMonster2Item.end())
	{
		return;
	}

	for (; itr != m_mapMonster2Item.upper_bound(i_nMonsterUnitKind); itr++)
	{
		o_pVectDropItemList->push_back(itr->second);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			MONSTER_INFO *CFieldIOCP::GetMonsterInfo(int i_nMonsterUniqueNumber)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-02 ~ 2005-11-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MONSTER_INFO *CFieldIOCP::GetMonsterInfo(int i_nMonsterUniqueNumber)
{
	ez_map<int, MONSTER_INFO>::iterator itrMon = m_mapMonsterInfo.find(i_nMonsterUniqueNumber);
	if (itrMon == m_mapMonsterInfo.end())
	{
		return NULL;
	}
	return &itrMon->second;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			MONSTER_INFO *CFieldIOCP::GetMonsterInfo(char *i_szMonsterName)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-02 ~ 2006-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MONSTER_INFO *CFieldIOCP::GetMonsterInfo(char *i_szMonsterName)
{
	ez_map<int, MONSTER_INFO>::iterator itrMon = this->m_mapMonsterInfo.begin();
	while (itrMon != this->m_mapMonsterInfo.end())
	{
		if (0 == stricmp(itrMon->second.MonsterName, i_szMonsterName))
		{
			return &itrMon->second;
		}
		itrMon++;
	}
	return NULL;
}

void CFieldIOCP::LoadMonster2Item()
{
	SQLHENV		henv = SQL_NULL_HENV;
	SQLHDBC		hdbc = SQL_NULL_HDBC;
	SQLHSTMT	hstmt = SQL_NULL_HSTMT;

	RETCODE		ret;
	SDWORD	cb1, cb2, cb3, cb4, cb5, cb6, cb7;
	SQLINTEGER	cb8 = SQL_NTS;		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	MONSTER2ITEM	m2i;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//	ret = SQLConnect(hdbc, g_pGlobal->GetODBCDSN(), SQL_NTS, g_pGlobal->GetODBCUID(), SQL_NTS, g_pGlobal->GetODBCPASSWORD(), SQL_NTS);
	char szConnectionString[512];
	char szOutConnectionString[512]; SQLSMALLINT pcb;
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
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CFieldIOCP::LoadMonster2Item_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());
		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_DBC, hdbc, "@CFieldIOCP::LoadMonster2Item Failed!\r\n", FALSE);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		return;
	}

	// prints DB Info
	char DatabaseName[40];
	char DSN[40];
	SQLSMALLINT StrLen;
	SQLGetInfo(hdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
	SQLGetInfo(hdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//ret = SQLExecDirect(hstmt, (UCHAR*)"SELECT * FROM ti_MONSTERITEM WITH (NOLOCK)", SQL_NTS);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	ret = SQLExecDirect(hstmt, (UCHAR*)QUERY_080702_0247, SQL_NTS);
	ret = SQLExecDirect(hstmt, (UCHAR*)PROCEDURE_080827_0247, SQL_NTS);		// 2008-09-08 by cmkwon, 쿼리로 남아 있던 버그 수정

	SQLBindCol(hstmt, 1, SQL_C_LONG, &m2i.MonsterUniqueNumber, 0, &cb1);
	SQLBindCol(hstmt, 2, SQL_C_LONG, &m2i.ItemNumber, 0, &cb2);
	SQLBindCol(hstmt, 3, SQL_C_LONG, &m2i.MinCount, 0, &cb3);
	SQLBindCol(hstmt, 4, SQL_C_LONG, &m2i.MaxCount, 0, &cb4);
	SQLBindCol(hstmt, 5, SQL_C_PROB1000K, &m2i.Probability, 0, &cb5);
	SQLBindCol(hstmt, 6, SQL_C_PROB100K, &m2i.PrefixProbability, 0, &cb6);
	SQLBindCol(hstmt, 7, SQL_C_PROB100K, &m2i.SuffixProbability, 0, &cb7);
	SQLBindCol(hstmt, 8, SQL_C_LONG, &m2i.DropType, 0, &cb8);	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt, "Load Monster2Item Failed!\n", TRUE);
		return;
	}

	// initialize to 0x00
	memset(&m2i, 0x00, sizeof(MONSTER2ITEM));
	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		MONSTER2ITEM *pM2I = new MONSTER2ITEM;
		memcpy(pM2I, &m2i, sizeof(MONSTER2ITEM));

		m_mapMonster2Item.insert(pair<INT, MONSTER2ITEM*>(pM2I->MonsterUniqueNumber, pM2I));

		// initialize to 0x00
		memset(&m2i, 0x00, sizeof(MONSTER2ITEM));
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	// cleanup odbc resources
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
#ifndef _DEBUG
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
#endif
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void CFieldIOCP::CleanMonster2Item()
{
	multimap<int, MONSTER2ITEM*>::iterator itr2 = m_mapMonster2Item.begin();
	while (itr2 != m_mapMonster2Item.end())
	{
		delete(itr2->second);
		itr2++;
	}
	m_mapMonster2Item.clear();
}

ITEM* CFieldIOCP::GetItemInfo(int ItemNum)
{
	ez_map<int, ITEM*>::iterator itr = m_mapItemInfo.find(ItemNum);
	if (itr == m_mapItemInfo.end())
	{
		return NULL;
	}
	else
	{
		return itr->second;
	}

	// not reachable
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM* CFieldIOCP::GetBulletItemInfoByCaliber(BYTE i_byCaliber)
/// \brief		
/// \author		cmkwon
/// \date		2005-11-02 ~ 2005-11-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM* CFieldIOCP::GetBulletItemInfoByCaliber(BYTE i_byCaliber)
{
	multimap<BYTE, ITEM*>::iterator itrBullet = this->m_multiMapCaliber2Bullet.find(i_byCaliber);
	if (itrBullet == this->m_multiMapCaliber2Bullet.end())
	{
		return NULL;
	}
	return itrBullet->second;
}

#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
void CFieldIOCP::ResetAllBuffSkill(BYTE InflType) {
	CFieldIOCPSocket *pSock = NULL;
	INT nInsertCount = 0;
	float tmBuffPenalty = 1.0f;
	ItemID_t skill;
	skill.ItemNum = 7813080;//id du pury ici celui des joueur avec le param DES_SKILL_CANCELALL(153U)
	skill.ItemUID = 0;
	ATUM_DATE_TIME time;
	GetCurrentServerTimeInAtumDateTime(time);
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (NULL == pSock
			|| FALSE == pSock->IsValidCharacter(FALSE)		// 2006-11-07 by cmkwon, 죽은 상태의 유저에게도 지급하도록 수정, // 2006-04-21 by cmkwon, 죽은 유저에게는 추가되지 않는다
			|| FALSE == COMPARE_INFLUENCE(pSock->GetCharacter()->InfluenceType, InflType))
		{
			continue;
		}

		int diff = g_diffVictoryWarByroo;
		if ((diff < 0 && TRUE == COMPARE_INFLUENCE(pSock->m_character.InfluenceType, INFLUENCE_TYPE_ANI)) ||
			(diff > 0 && TRUE == COMPARE_INFLUENCE(pSock->m_character.InfluenceType, INFLUENCE_TYPE_VCN))) {
			int levelPenality = abs(diff) / 22;
			tmBuffPenalty = 1.0 - (0.05f * levelPenality);
		}

		INIT_MSG_OF_SIZE(MSG_FC_SKILL_USE_SKILL_OK, T_FC_SKILL_USE_SKILL_OK, msgUseSkillOK, SendBuf);
		msgUseSkillOK->SkillItemID = skill;
		msgUseSkillOK->AttackIndex = 0;
		msgUseSkillOK->TargetIndex = i;
		msgUseSkillOK->UseTime = time;			// 2006-12-05 by dhjin, 스킬 사용 시간
		msgUseSkillOK->penality = tmBuffPenalty;
		pSock->SendInRangeMessageAroundMe(SendBuf, MSG_SIZE(MSG_FC_SKILL_USE_SKILL_OK), TRUE);
	}
}
#endif /* NEMERIAN_NATION_BALANCE_BUFF_PENALITY */

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::InsertItemAllCharacter(INT i_nItemNum, INT i_nItemCount, BYTE i_byInflMask/*=INFLUENCE_TYPE_ALL_MASK*/, BOOL bExceptArenaMapUser /*= FALSE*/, BOOL bPCBang /*= FALSE*/)		// 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - PC방 유저 아이템 지급 추가
/// \brief		
/// \author		cmkwon
/// \date		2006-04-17 ~ 2006-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::InsertItemAllCharacter(INT i_nItemNum, INT i_nItemCount, BYTE i_byInflMask/*=INFLUENCE_TYPE_ALL_MASK*/, BOOL bExceptArenaMapUser /*= FALSE*/, BOOL bPCBang /*= FALSE*/)		// 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - PC방 유저 아이템 지급 추가
{
	// 2008-02-20 by cmkwon, 넌카운터블 아이템 카운트 체크 추가
	ITEM *pItemInfo = this->GetItemInfo(i_nItemNum);
	if (NULL == pItemInfo)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::InsertItemAllCharacter_1 ItemNum error !!, ItemNum(%d) ItemCount(%d) InflueceType(%d)\r\n"
			, i_nItemNum, i_nItemCount, i_byInflMask);
		return 0;
	}
	if (FALSE == IS_COUNTABLE_ITEM(pItemInfo->Kind)
		&& 10 < i_nItemCount)
	{// 2008-02-20 by cmkwon, 넌카운터블 아이템 카운트는 최대 10개만 가능함
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::InsertItemAllCharacter_2 ItemCount error !!, ItemNum(%d) ItemCount(%d) InflueceType(%d)\r\n"
			, i_nItemNum, i_nItemCount, i_byInflMask);

		i_nItemCount = 10;	// 2008-02-20 by cmkwon, 강제로 10으로 설정해서 추가는 진행한다.
	}

	CFieldIOCPSocket *pSock = NULL;
	INT nInsertCount = 0;
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (NULL == pSock
			|| FALSE == pSock->IsValidCharacter(FALSE)		// 2006-11-07 by cmkwon, 죽은 상태의 유저에게도 지급하도록 수정, // 2006-04-21 by cmkwon, 죽은 유저에게는 추가되지 않는다
			|| FALSE == COMPARE_INFLUENCE(pSock->GetCharacter()->InfluenceType, i_byInflMask))
		{
			continue;
		}

		if (bExceptArenaMapUser
			&& IS_MAP_INFLUENCE_ARENA(pSock->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW()))
		{// 2007-05-18 by cmkwon, 아레나맵 유저 제외
			continue;
		}

		// 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - PC방 유저 아이템 지급 추가
		if (bPCBang)
		//	&& FALSE == pSock->m_bIsPCBangClient)
		{
			continue;
		}
		// end 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - PC방 유저 아이템 지급 추가

		if (pSock->m_ItemManager.InsertItemBaseByItemNum(i_nItemNum, i_nItemCount, IUT_BONUS_ITEM))
		{
			nInsertCount++;
		}
	}

	// 2008-02-20 by cmkwon, 수정함
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] CFieldIOCP::InsertItemAllCharacter_ ItemInfo(%d:%d) InfluenceType(%s:%d) InsertedUseCounts(%d)\r\n", i_nItemNum, i_nItemCount, CAtumSJ::GetInfluenceTypeString(i_byInflMask), i_byInflMask, nInsertCount);
	return nInsertCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::InsertTimeLimiteItemAllCharacterSameMapChannel(ITEM *i_pTimeLimiteItemInfo, MAP_CHANNEL_INDEX i_mapChanIdx, BYTE* i_pMsgData, int i_nMsgSize, BYTE i_byInflMask/*=INFLUENCE_TYPE_ALL_MASK*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-21 ~ 2006-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::InsertTimeLimiteItemAllCharacterSameMapChannel(ITEM *i_pTimeLimiteItemInfo, MAP_CHANNEL_INDEX i_mapChanIdx, BYTE* i_pMsgData, int i_nMsgSize, BYTE i_byInflMask/*=INFLUENCE_TYPE_ALL_MASK*/)
{
	CFieldIOCPSocket *pSock = NULL;
	INT nInsertCount = 0;
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter()		// 2006-04-21 by cmkwon, 죽은 상태의 유저는 추가되지 않는다
			&& i_mapChanIdx == pSock->GetCharacter()->MapChannelIndex
			&& COMPARE_INFLUENCE(pSock->GetCharacter()->InfluenceType, i_byInflMask))
		{
			if (pSock->InsertTimeLimiteItem(i_pTimeLimiteItemInfo))
			{
				pSock->SendAddData(i_pMsgData, i_nMsgSize);		// 2009-01-05 by dhjin, 미션마스터 - 편대 버프 아이템 추가
				nInsertCount++;
			}
		}
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] : CFieldIOCP::InsertTimeLimiteItemAllCharacter_ InsertedCount[%d]\r\n", nInsertCount);
	return nInsertCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::InsertTimeLimiteItemPartyMember(ITEM *i_pTimeLimiteItemInfo, CFieldIOCPSocket *i_pUseItemFISoc, BYTE* i_pMsgData, int i_nMsgSize)
/// \brief		미션마스터 - 편대 버프 아이템 추가
/// \author		dhjin
/// \date		2009-01-05 ~ 2009-01-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::InsertTimeLimiteItemPartyMember(ITEM *i_pTimeLimiteItemInfo, CFieldIOCPSocket *i_pUseItemFISoc, BYTE* i_pMsgData, int i_nMsgSize)
{
	INT nInsertCount = 0;
	vectCFieldIOCPSocket vectCFISockList;
	if (i_pUseItemFISoc->GetPartyMemberList(&vectCFISockList))
	{
		vectCFieldIOCPSocket::iterator itr(vectCFISockList.begin());
		for (; itr != vectCFISockList.end(); itr++)
		{
			CFieldIOCPSocket *pFISoc = *itr;
			if (pFISoc
				&& pFISoc->IsValidCharacter(FALSE)
				)
			{
				if (pFISoc->InsertTimeLimiteItem(i_pTimeLimiteItemInfo))
				{
					pFISoc->SendAddData(i_pMsgData, i_nMsgSize);
					nInsertCount++;
				}
			}
		}
	}

	return nInsertCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM * CFieldIOCP::CheckAllItemInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-10 ~ 2006-08-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM * CFieldIOCP::CheckAllItemInfo(void)
{
	m_vectITEMPtr4RandomBox.clear();							// 2006-08-10 by cmkwon, 랜던박스 지급가능 아이템 초기화
	m_vectITEMPtr4RandomBox.reserve(10);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-29 by cmkwon
	m_vectITEMPtrSkillSupportItems.clear();
	m_vectITEMPtrSkillSupportItems.reserve(10);

	for (auto& info : m_mapItemInfo)
	{
		auto pItemInfo = info.second;
		
		if (!CheckItemInfo(pItemInfo)) return pItemInfo;

		if (COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_BAZAAR_ITEM)
			&& MONEY_ITEM_NUMBER != pItemInfo->ItemNum		// 2006-08-11 by cmkwon, SPI는 따로 지급
			&& FALSE == IS_WEAPON(pItemInfo->Kind)			// 2006-08-11 by cmkwon, 무기 지급 안됨
			&& ITEMKIND_DEFENSE != pItemInfo->Kind			// 2006-08-11 by cmkwon, 아머 지급 안됨		
			&& ITEMKIND_SUPPORT != pItemInfo->Kind			// 2006-08-11 by cmkwon, 엔진 지급 안됨
			&& ITEMKIND_COMPUTER != pItemInfo->Kind)		// 2006-08-11 by cmkwon, 컴류터 지급 안됨
		{// 2006-08-10 by cmkwon, 랜덤박스 아이템 사용시 RANDOMBOX_RESULT_ITEM일때 지급가능한 아이템 벡터

			m_vectITEMPtr4RandomBox.push_back(pItemInfo);
		}

		if (COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_SKILL_SUPPORT_ITEM))
		{// 2006-09-29 by cmkwon, 보조스킬아이템
			m_vectITEMPtrSkillSupportItems.push_back(pItemInfo);
		}

		// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - ITEM::IsExistDesParam() 함수로 통합함
		//if(IS_EXIST_DES_PARAM(pItemInfo, DES_SKILL_CHAFF_HP))
		if (pItemInfo->IsExistDesParam(DES_SKILL_CHAFF_HP))
		{// 2006-12-04 by dhjin, 체프의 HP를 Endurance에 할당한다.
			pItemInfo->Endurance = pItemInfo->GetParameterValue(DES_SKILL_CHAFF_HP);
		}
	}

	return nullptr;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::CheckItemInfo(ITEM *i_pItemInfo)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-10 ~ 2006-08-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::CheckItemInfo(ITEM *i_pItemInfo)
{

	// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldIOCP::CheckItemInfo#, 체크 시스템 수정
	//	if(COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))		// 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 
	// 	{
	// 		///////////////////////////////////////////////////////////////////////////////
	// 		// 2006-09-29 by cmkwon, 사용 시간 제한 아이템 체크
	// 		switch(i_pItemInfo->Kind)
	// 		{
	// 		case ITEMKIND_CARD:						// 2006-09-30 by cmkwon
	// 		case ITEMKIND_SKILL_SUPPORT_ITEM:		// 2006-09-30 by cmkwon
	// 		case ITEMKIND_ACCESSORY_TIMELIMIT:		// 2006-09-30 by cmkwon
	// 			break;
	// 		default:
	// 			{
	// 				// 2009-08-07 by cmkwon, 아이템 에러 시스템 로그 추가 - 
	// 				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 11000 ItemInfo(%8d:%20s), ItemKind(%d:%s) \r\n"
	// 					, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));
	// 				return FALSE;
	// 			}
	// 		}
	// 	}
		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldIOCP::CheckItemInfo#, 아래와 같이 정리
	if (ITEMKIND_ACCESSORY_TIMELIMIT == i_pItemInfo->Kind && FALSE == COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE))
	{
		// 2009-10-27 by cmkwon, 시간 제한형 악세서리 아이템은 반드시 ITEM_ATTR_TIME_LIMITE이 설정되어 있어야 한다.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 11000 ItemInfo(%8d:%20s), ItemKind(%d:%s) invalid ItemAttribute(%I64d) !! IsITEMKIND_ACCESSORY_TIMELIMIT(%d) ITEM_ATTR_TIME_LIMITE(%d) \r\n"
			, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind), i_pItemInfo->ItemAttribute, (ITEMKIND_ACCESSORY_TIMELIMIT == i_pItemInfo->Kind), COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE));

		// 2010-03-12 by cmkwon, CFieldIOCP::CheckItemInfo#에서 릴리즈모드일때문만 FALSE를 리턴 - 
#if !defined(_DEBUG)
		return FALSE;
#endif	
	}

	constexpr auto badattr = ITEM_ATTR_TIME_LIMITE | ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED;

	if ((i_pItemInfo->ItemAttribute & badattr) == badattr) // means both bits are set
	{
		// 2009-10-27 by cmkwon, 사용 후 시간 제한형과 사용 후 기간 제한형은  동시에 설정 불가능 하다
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 11010 ItemInfo(%8d:%20s), ItemKind(%d:%s) invalid ItemAttribute(%I64d) !! ITEM_ATTR_TIME_LIMITE(%d) ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED(%d) \r\n"
			, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind), i_pItemInfo->ItemAttribute, COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE), COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED));

		// 2010-03-12 by cmkwon, CFieldIOCP::CheckItemInfo#에서 릴리즈모드일때문만 FALSE를 리턴 - 
#if !defined(_DEBUG)
		return FALSE;
#endif	
	}
	if (COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE | ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)
		&& 0 >= i_pItemInfo->Time && ITEMKIND_PET_SOCKET_ITEM != i_pItemInfo->Kind)		// 2011-09-30 by hskim, 파트너 시스템 2차 - 파트너 소켓 아이템 예외 추가
	{
		// 2009-10-27 by cmkwon
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 11100 ItemInfo(%8d:%20s), ItemKind(%d:%s) invalid using time !! Time(%d) IsITEM_ATTR_TIME_LIMITE(%d) IsITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED(%d)\r\n"
			, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind), i_pItemInfo->Time, COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE), COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED));

		// 2010-03-12 by cmkwon, CFieldIOCP::CheckItemInfo#에서 릴리즈모드일때문만 FALSE를 리턴 - 
#if !defined(_DEBUG)
		return FALSE;
#endif	
	}
	if (COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE))
	{
		switch (i_pItemInfo->Kind)
		{
		case ITEMKIND_CARD:						// 2006-09-30 by cmkwon
		case ITEMKIND_SKILL_SUPPORT_ITEM:		// 2006-09-30 by cmkwon
		case ITEMKIND_ACCESSORY_TIMELIMIT:		// 2006-09-30 by cmkwon
		case ITEMKIND_PET_SOCKET_ITEM:			// 2011-09-30 by hskim, 파트너 시스템 2차 - 파트너 소켓 아이템 예외 추가
			break;
		default:
		{
			// 2009-10-27 by cmkwon
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 11210 ItemInfo(%8d:%20s), ItemKind(%d:%s) invalid ItemKind !! Don't have to set ITEM_ATTR_TIME_LIMITE in ItemAttribute \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));

			// 2010-03-12 by cmkwon, CFieldIOCP::CheckItemInfo#에서 릴리즈모드일때문만 FALSE를 리턴 - 
#if !defined(_DEBUG)
			return FALSE;
#endif	
		}
		}
	}
	else if (COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)
		&& ITEMKIND_CARD != i_pItemInfo->Kind)
	{
		// 2009-10-27 by cmkwon
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 11300 ItemInfo(%8d:%20s), ItemKind(%d:%s) IsItemKindCard(%d) \r\n"
			, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind), (ITEMKIND_CARD == i_pItemInfo->Kind));

		// 2010-03-12 by cmkwon, CFieldIOCP::CheckItemInfo#에서 릴리즈모드일때문만 FALSE를 리턴 - 
#if !defined(_DEBUG)
		return FALSE;
#endif	
	}
	else if (COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
	{
		if (0 >= (int)(i_pItemInfo->Endurance))
		{
			// 2009-10-22 by cmkwon, 시간 제한형류의 아이템의 제한 시간을 설정 해야한다.
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 11400 ItemInfo(%8d:%20s), ItemKind(%d:%s) invalid using time !! Endurance(%d) \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind), (int)(i_pItemInfo->Endurance));

			// 2010-03-12 by cmkwon, CFieldIOCP::CheckItemInfo#에서 릴리즈모드일때문만 FALSE를 리턴 - 
#if !defined(_DEBUG)
			return FALSE;
#endif	
		}
	}


	switch (i_pItemInfo->Kind)
	{
	case ITEMKIND_RANDOMBOX:
	{
		// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
		// 			if(0.0f > i_pItemInfo->DestParameter1
		// 				|| 0.0f > i_pItemInfo->DestParameter2
		// 				|| 0.0f > i_pItemInfo->DestParameter3
		// 				|| 0.0f > i_pItemInfo->DestParameter4
		// 				|| 0.0f >= i_pItemInfo->DestParameter1+i_pItemInfo->DestParameter2+i_pItemInfo->DestParameter3+i_pItemInfo->DestParameter4)
		if (0.0f > i_pItemInfo->ArrDestParameter[0]
			|| 0.0f > i_pItemInfo->ArrDestParameter[1]
			|| 0.0f > i_pItemInfo->ArrDestParameter[2]
			|| 0.0f > i_pItemInfo->ArrDestParameter[3]
			|| 0.0f > i_pItemInfo->ArrDestParameter[4]
			|| 0.0f >= i_pItemInfo->ArrDestParameter[0] + i_pItemInfo->ArrDestParameter[1] + i_pItemInfo->ArrDestParameter[2] + i_pItemInfo->ArrDestParameter[3] + i_pItemInfo->ArrDestParameter[4])
		{
			// 2009-08-07 by cmkwon, 아이템 에러 시스템 로그 추가 - 
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 12000 ItemInfo(%8d:%20s), ItemKind(%d:%s) \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));
			return FALSE;
		}
		if (0.0f > i_pItemInfo->AbilityMin
			|| i_pItemInfo->AbilityMin > i_pItemInfo->AbilityMax)
		{
			// 2009-08-07 by cmkwon, 아이템 에러 시스템 로그 추가 - 
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 13000 ItemInfo(%8d:%20s), ItemKind(%d:%s) \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));
			return FALSE;
		}
		if (0.0f > i_pItemInfo->RangeAngle
			|| i_pItemInfo->RangeAngle > i_pItemInfo->BoosterAngle)
		{
			// 2009-08-07 by cmkwon, 아이템 에러 시스템 로그 추가 - 
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 14000 ItemInfo(%8d:%20s), ItemKind(%d:%s) \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));
			return FALSE;
		}
	}
	break;
	}

	// 2006-12-04 by dhjin
	// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - ITEM::IsExistDesParam() 함수로 통합함
	//if(IS_EXIST_DES_PARAM(i_pItemInfo, DES_SKILL_HALLUCINATION))
	if (i_pItemInfo->IsExistDesParam(DES_SKILL_HALLUCINATION))
	{
		if (FALSE == util::in_range(1, i_pItemInfo->GetParameterValue(DES_SKILL_HALLUCINATION), 20))
		{
			// 2009-08-07 by cmkwon, 아이템 에러 시스템 로그 추가 - 
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 15000 ItemInfo(%8d:%20s), ItemKind(%d:%s) \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));
			return FALSE;
		}
	}
	// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - ITEM::IsExistDesParam() 함수로 통합함
	//if(IS_EXIST_DES_PARAM(i_pItemInfo, DES_SKILL_CHAFF_HP))
	if (i_pItemInfo->IsExistDesParam(DES_SKILL_CHAFF_HP))
	{
		if (FALSE == IS_SKILL_ITEM(i_pItemInfo->Kind))
		{// 2009-10-29 by cmkwon, 체크 추가
			// 2009-08-07 by cmkwon, 아이템 에러 시스템 로그 추가 - 
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 16000 ItemInfo(%8d:%20s), ItemKind(%d:%s) Chaff not matching ItemKind and DesParameter \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));
			return FALSE;
		}
		if (FALSE == util::in_range(1, i_pItemInfo->GetParameterValue(DES_SKILL_CHAFF_HP), 3000))
		{
			// 2009-08-07 by cmkwon, 아이템 에러 시스템 로그 추가 - 
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 16100 ItemInfo(%8d:%20s), ItemKind(%d:%s), hav to ChaffHP(%d <= %d < %d) \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind), 1, i_pItemInfo->GetParameterValue(DES_SKILL_CHAFF_HP), 3000);
			return FALSE;
		}
		i_pItemInfo->Endurance = i_pItemInfo->GetParameterValue(DES_SKILL_CHAFF_HP);
	}


	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItem 체크
	if (i_pItemInfo->IsExistDesParam(DES_SHAPE_ITEM)
		|| i_pItemInfo->IsExistDesParam(DES_EFFECT_ITEM))
	{
		if (ITEMKIND_INGOT != i_pItemInfo->Kind)
		{
			// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 컬렉션 등룍용 외변킷 Kind 변경 지원
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - 컬렉션 등룍용 외변킷 Kind 변경 지원
			if (FALSE == i_pItemInfo->IsExistDesParam(DES_COLLECTION_ARMOR_INDEX))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 17000 ItemInfo(%8d:%20s), ItemKind(%d:%s) \r\n"
					, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));
				return FALSE;
			}
#else
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 17000 ItemInfo(%8d:%20s), ItemKind(%d:%s) \r\n"
				, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind));
			return FALSE;
#endif
			// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - 컬렉션 등룍용 외변킷 Kind 변경 지원
		}

		if (i_pItemInfo->IsExistDesParam(DES_SHAPE_ITEM))
		{
			if (NULL == i_pItemInfo->LinkItem)
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 17100 ItemInfo(%8d:%20s), ItemKind(%d:%s) LinkItem(%d) \r\n"
					, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind), i_pItemInfo->LinkItem);
				return FALSE;
			}

			if (NULL == this->GetItemInfo(i_pItemInfo->LinkItem))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 17200 ItemInfo(%8d:%20s), ItemKind(%d:%s) LinkItem(%d) \r\n"
					, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, CAtumSJ::GetItemKindName(i_pItemInfo->Kind), i_pItemInfo->LinkItem);
				return FALSE;
			}
		}
	}

	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	if (i_pItemInfo->Time && i_pItemInfo->InvokingDestParamID) {
		// 발동류 아이템은 현재 지원하지 않는다.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 17300 ItemInfo(%8d:%20s), ItemTime(%d) InvokingDestParamID(%d) \r\n"
			, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Time, i_pItemInfo->InvokingDestParamID);
		return FALSE;
	}

	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	if (ITEMKIND_ACCESSORY_UNLIMITED != i_pItemInfo->Kind
		&& ITEMKIND_ENCHANT != i_pItemInfo->Kind
		&& (i_pItemInfo->InvokingDestParamID
			|| i_pItemInfo->InvokingDestParamIDByUse)) {
		// 발동류 아이템은 현재 무제한 시간 제한 아이템만 지원한다.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 17400 ItemInfo(%8d:%20s), ItemKind(%d) InvokingDestParamID(%d) InvokingDestParamIDByUse(%d)\r\n"
			, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, i_pItemInfo->InvokingDestParamID, i_pItemInfo->InvokingDestParamIDByUse);
		return FALSE;
	}

	// 2010-04-05 by cmkwon, 몬스터변신관련 해제 카드 구현 - 
	if (i_pItemInfo->IsExistDesParam(DES_TRANSFORM_TO_GEAR) && COMPARE_BIT_FLAG(i_pItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::CheckItemInfo# 18000 DES_TRANSFORM_TO_GEAR can't set ITEM_ATTR_TIME_LIMITE! ItemInfo(%8d:%20s), ItemKind(%d) ItemAttribute(%I64d) \r\n"
			, i_pItemInfo->ItemNum, i_pItemInfo->ItemName, i_pItemInfo->Kind, i_pItemInfo->ItemAttribute);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM * CFieldIOCP::GetRandomItemFromRandomBoxItemList(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-10 ~ 2006-08-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM * CFieldIOCP::GetRandomItemFromRandomBoxItemList(void)
{
	return m_vectITEMPtr4RandomBox[RANDI(0, m_vectITEMPtr4RandomBox.size() - 1)];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM * CFieldIOCP::GetRandomSkillSupportItem(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-29 ~ 2006-09-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM * CFieldIOCP::GetRandomSkillSupportItem(void)
{
	return m_vectITEMPtrSkillSupportItems[RANDI(0, m_vectITEMPtrSkillSupportItems.size() - 1)];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::GetMysteryDropItem(vectMYSTERY_ITEM_DROPPtr *o_pMysteryDropItemList, CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum, BOOL i_bPackageItem/*=FALSE*/)
/// \brief		
///				// 2008-08-26 by cmkwon, ItemAttribute 추가 - 패키지(Package) 아이템, 함수 인자, 리턴값 수정
/// \author		cmkwon
/// \date		2007-05-11 ~ 2007-05-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::GetMysteryDropItem(vectMYSTERY_ITEM_DROPPtr *o_pMysteryDropItemList, CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum, BOOL i_bPackageItem/*=FALSE*/)
{
	o_pMysteryDropItemList->clear();		// 2008-08-26 by cmkwon, ItemAttribute 추가 - 패키지(Package) 아이템, 초기화

	if (NULL == i_pFISoc
		|| FALSE == i_pFISoc->IsValidCharacter(FALSE))
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, 럭키머신 multimap -> mt_multimap으로 변경
//	mmapINT2MYSTERY_ITEM_DROP::iterator	itr	= m_mmapMysteryItemDropList.find(i_nMysteryItemDropNum);
	mtmapINT2MYSTERY_ITEM_DROP::iterator	itr = m_mtmapMysteryItemDropList.find(i_nMysteryItemDropNum);
	if (itr == m_mtmapMysteryItemDropList.end())
	{
		return FALSE;
	}


	vectMYSTERY_ITEM_DROPPtr	vectMyteryDropItemPtrList;
	INT nTotalProbability = 0;

	for (; itr != m_mtmapMysteryItemDropList.upper_bound(i_nMysteryItemDropNum); itr++)
	{
		MYSTERY_ITEM_DROP	*pMysteryItemDropInfo = &itr->second;
		ITEM				*pItemInfo = this->GetItemInfo(pMysteryItemDropInfo->DropItemNum);

		if (NULL == pItemInfo)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::GetMysteryDropItem_ error !!, MysteryItemDropNum(%8d) DropItemNum(%8d)\r\n"
				, pMysteryItemDropInfo->MysteryItemDropNum, pMysteryItemDropInfo->DropItemNum);
			continue;
		}

		if (ERR_NO_ERROR != this->CheckMysteryItemDropRequirements(i_pFISoc, pMysteryItemDropInfo))
		{
			continue;
		}

		nTotalProbability += pMysteryItemDropInfo->Probability;
		vectMyteryDropItemPtrList.push_back(pMysteryItemDropInfo);

		///////////////////////////////////////////////////////////////////////////////
		// 2008-08-26 by cmkwon, ItemAttribute 추가 - 패키지(Package) 아이템, 모두 추가 한다.
		if (i_bPackageItem
			&& MAX_ITEM_COUNT_FOR_PACKAGE_ITEM > o_pMysteryDropItemList->size())
		{
			o_pMysteryDropItemList->push_back(pMysteryItemDropInfo);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-26 by cmkwon, ItemAttribute 추가 - 패키지(Package) 아이템, 처리
	if (i_bPackageItem)
	{
		if (o_pMysteryDropItemList->empty())
		{
			return FALSE;
		}

		return TRUE;
	}

	if (0 >= nTotalProbability
		|| vectMyteryDropItemPtrList.empty())
	{
		return FALSE;
	}

	int nSumProbability = 0;
	int nDropProValue = RANDI(0, nTotalProbability - 1);	// 2008-02-13 by cmkwon, 수정함
	int nCnt = vectMyteryDropItemPtrList.size();
	for (int i = 0; i < nCnt; i++)
	{
		if (nDropProValue < nSumProbability + vectMyteryDropItemPtrList[i]->Probability)
		{
			// 2008-08-26 by cmkwon, ItemAttribute 추가 - 패키지(Package) 아이템, 아래와 같이 벡터에 할당하고 TRUE를 리턴한다.
			//return vectMyteryDropItemPtrList[i];
			o_pMysteryDropItemList->push_back(vectMyteryDropItemPtrList[i]);
			return TRUE;
		}

		nSumProbability += vectMyteryDropItemPtrList[i]->Probability;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::GetMysteryDropItem_ error !!, MysteryItemDropNum(%8d) TotalProbability(%8d)\r\n"
		, i_nMysteryItemDropNum, nTotalProbability);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CFieldIOCP::CheckMysteryItemDropRequirements(CFieldIOCPSocket *i_pFISoc, MYSTERY_ITEM_DROP *i_pMysteryItemDrop)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-11 ~ 2007-05-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CFieldIOCP::CheckMysteryItemDropRequirements(CFieldIOCPSocket *i_pFISoc, MYSTERY_ITEM_DROP *i_pMysteryItemDrop)
{
	if (NULL == i_pFISoc
		|| FALSE == i_pFISoc->IsValidCharacter(FALSE))
	{
		return ERR_INVALID_CHARACTER;
	}

	const CHARACTER *pCharacter = i_pFISoc->GetCharacter();

	// 필요유닛종류 체크
	if (0 == (i_pMysteryItemDrop->ReqUnitKind & pCharacter->UnitKind))
	{
		return ERR_PROTOCOL_REQ_UNITKIND_NOT_MATCHED;
	}

	// 최소 요구 레벨 체크
	if (0 != i_pMysteryItemDrop->ReqMinLevel
		&& i_pMysteryItemDrop->ReqMinLevel > pCharacter->Level)
	{
		return ERR_PROTOCOL_REQ_MINLEVEL_NOT_MATCHED;
	}

	// 최대 요구 레벨 체크
	if (0 != i_pMysteryItemDrop->ReqMaxLevel
		&& i_pMysteryItemDrop->ReqMaxLevel < pCharacter->Level)
	{
		return ERR_PROTOCOL_REQ_MAXLEVEL_NOT_MATCHED;
	}

	return ERR_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::GetLuckyDropItem(CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum, LUCKYITEM_DROP_INFO * o_pLuckyItemInfo, MSG_FL_LOG_LUCKY_DROP * o_pLuckyDropLog)
/// \brief		// 2008-11-04 by dhjin, 럭키머신
///				// 2009-03-31 by dhjin, 럭키머신 로그 - 인자추가(, MSG_FL_LOG_LUCKY_DROP * o_pLuckyDropLog)
/// \author		dhjin
/// \date		2008-11-04 ~ 2008-11-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::GetLuckyDropItem(CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum, LUCKYITEM_DROP_INFO * o_pLuckyItemInfo, MSG_FL_LOG_LUCKY_DROP * o_pLuckyDropLog)
{
	if (NULL == i_pFISoc
		|| FALSE == i_pFISoc->IsValidCharacter(FALSE))
	{
		return FALSE;
	}

	mt_auto_lock mta(&m_mtmapMysteryItemDropList);
	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, 럭키머신 multimap -> mt_multimap으로 변경
//	mmapINT2MYSTERY_ITEM_DROP::iterator	itr	= m_mmapMysteryItemDropList.find(i_nMysteryItemDropNum);
	mtmapINT2MYSTERY_ITEM_DROP::iterator	itr = m_mtmapMysteryItemDropList.find(i_nMysteryItemDropNum);
	if (itr == m_mtmapMysteryItemDropList.end())
	{
		return FALSE;
	}


	vectMYSTERY_ITEM_DROPPtr	vectMyteryDropItemPtrList;
	INT nTotalProbability = 0;

	for (; itr != m_mtmapMysteryItemDropList.upper_bound(i_nMysteryItemDropNum); itr++)
	{
		MYSTERY_ITEM_DROP	*pMysteryItemDropInfo = &itr->second;
		ITEM				*pItemInfo = this->GetItemInfo(pMysteryItemDropInfo->DropItemNum);

		if (NULL == pItemInfo)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::GetLuckyDropItem_ error !!, MysteryItemDropNum(%8d) DropItemNum(%8d)\r\n"
				, pMysteryItemDropInfo->MysteryItemDropNum, pMysteryItemDropInfo->DropItemNum);
			continue;
		}

		if (ERR_NO_ERROR != this->CheckMysteryItemDropRequirements(i_pFISoc, pMysteryItemDropInfo))
		{
			continue;
		}

		if (0 != pMysteryItemDropInfo->CountPerPeriod
			&& pMysteryItemDropInfo->DropCount >= pMysteryItemDropInfo->CountPerPeriod)
		{
			continue;
		}
		nTotalProbability += pMysteryItemDropInfo->Probability;
		vectMyteryDropItemPtrList.push_back(pMysteryItemDropInfo);

	}

	if (0 >= nTotalProbability
		|| vectMyteryDropItemPtrList.empty())
	{
		return FALSE;
	}

	int nSumProbability = 0;
	int nDropProValue = RANDI(0, nTotalProbability - 1);	// 2008-02-13 by cmkwon, 수정함
	int nCnt = vectMyteryDropItemPtrList.size();
	for (int i = 0; i < nCnt; i++)
	{
		if (nDropProValue < nSumProbability + vectMyteryDropItemPtrList[i]->Probability)
		{
			o_pLuckyItemInfo->MysteryItemDropNum = i_nMysteryItemDropNum;
			o_pLuckyItemInfo->DropItemNum = vectMyteryDropItemPtrList[i]->DropItemNum;
			o_pLuckyItemInfo->MinCount = vectMyteryDropItemPtrList[i]->MinCount;
			o_pLuckyItemInfo->MaxCount = vectMyteryDropItemPtrList[i]->MaxCount;
			o_pLuckyItemInfo->PrefixProbability = vectMyteryDropItemPtrList[i]->PrefixProbability;
			o_pLuckyItemInfo->SuffixProbability = vectMyteryDropItemPtrList[i]->SuffixProbability;
			o_pLuckyItemInfo->LuckItemUsing = TRUE;
			o_pLuckyItemInfo->Period = vectMyteryDropItemPtrList[i]->Period;
			vectMyteryDropItemPtrList[i]->DropCount++;

			//////////////////////////////////////////////////////////////////////////
			// 2009-03-31 by dhjin, 럭키머신 로그
			o_pLuckyDropLog->MysteryItemDropNum = i_nMysteryItemDropNum;
			o_pLuckyDropLog->DropItemNum = vectMyteryDropItemPtrList[i]->DropItemNum;
			o_pLuckyDropLog->Starttime = vectMyteryDropItemPtrList[i]->Starttime;
			o_pLuckyDropLog->Endtime = o_pLuckyDropLog->Starttime;
			o_pLuckyDropLog->Endtime.AddDateTime(0, 0, vectMyteryDropItemPtrList[i]->Period, 0, 0, 0);
			o_pLuckyDropLog->CountPerPeriod = vectMyteryDropItemPtrList[i]->CountPerPeriod;
			o_pLuckyDropLog->DropCount = vectMyteryDropItemPtrList[i]->DropCount;
			return TRUE;
		}
		nSumProbability += vectMyteryDropItemPtrList[i]->Probability;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::GetLuckyDropItem_ error !!, MysteryItemDropNum(%8d) TotalProbability(%8d)\r\n"
		, i_nMysteryItemDropNum, nTotalProbability);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::CheckLuckyItemPeriod(ATUM_DATE_TIME * i_pCurrentTime)
/// \brief		기간이 끝났는지 체크하여 새로운 기간을 설정한다.
/// \author		dhjin
/// \date		2008-11-04 ~ 2008-11-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::CheckLuckyItemPeriod(ATUM_DATE_TIME * i_pCurrentTime)
{
	INT	ContinueMysteryItemDropNum = 0;	// 2008-11-04 by dhjin, 기간 업데이트가 중복 피하기 체크변수
	ATUM_DATE_TIME tmLuckyItemStartTime;

	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, 럭키머신 multimap -> mt_multimap으로 변경
//	mmapINT2MYSTERY_ITEM_DROP::iterator	itr	= m_mmapMysteryItemDropList.begin();
	mt_auto_lock mta(&m_mtmapMysteryItemDropList);
	mtmapINT2MYSTERY_ITEM_DROP::iterator	itr = m_mtmapMysteryItemDropList.begin();
	for (; itr != m_mtmapMysteryItemDropList.end(); itr++)
	{
		MYSTERY_ITEM_DROP	*pMysteryItemDropInfo = &itr->second;
		if (0 == pMysteryItemDropInfo->Period)
		{
			continue;
		}

		if (ContinueMysteryItemDropNum == pMysteryItemDropInfo->MysteryItemDropNum)
		{// 2008-11-04 by dhjin, pMysteryItemDropInfo->MysteryItemDropNum로 기간 업데이트가 되었기 때문에 또 할 필요가 없다. 체크변수
			itr->second.DropCount = 0;
			itr->second.Starttime = *i_pCurrentTime;
			continue;
		}

		tmLuckyItemStartTime = pMysteryItemDropInfo->Starttime;
		tmLuckyItemStartTime.AddDateTime(0, 0, pMysteryItemDropInfo->Period, 0, 0, 0);
		if (*i_pCurrentTime >= tmLuckyItemStartTime)
		{
			ContinueMysteryItemDropNum = pMysteryItemDropInfo->MysteryItemDropNum;
			QPARAM_UPDATE_LUCKYITEM_STARTTIME *pQParam = new QPARAM_UPDATE_LUCKYITEM_STARTTIME;
			pQParam->MysteryItemDropNum = pMysteryItemDropInfo->MysteryItemDropNum;
			pQParam->StartTime = *i_pCurrentTime;
			m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateLuckyItemStarttime, NULL, 0, pQParam);
			itr->second.DropCount = 0;
			itr->second.Starttime = *i_pCurrentTime;

		}

	}

}

CMapProject * CFieldIOCP::GetMapProjectByMapIndex(MapIndex_t MapIndex)
{
	return m_pFieldMapWorkspace->GetMapProjectByMapIndex(MapIndex);
}

CFieldMapProject *CFieldIOCP::GetFieldMapProjectByMapIndex(MapIndex_t MapIndex)
{
	return (CFieldMapProject*)m_pFieldMapWorkspace->GetMapProjectByMapIndex(MapIndex);
}

CFieldMapChannel *CFieldIOCP::GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX i_mapChannIdx)
{
	if (0 == i_mapChannIdx.MapIndex)
	{// 2008-06-20 by cmkwon, 체크 추가
		return NULL;
	}

	CFieldMapProject *pFPro = GetFieldMapProjectByMapIndex(i_mapChannIdx.MapIndex);
	if (NULL == pFPro)
	{
		return NULL;
	}

	return pFPro->GetFieldMapChannelByIndex(i_mapChannIdx.ChannelIndex);
}

// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 인자추가(MapIndex_t i_nStartCityMapIdx)
CFieldMapProject *CFieldIOCP::GetCityFieldMapProjectPtrByInfluenceW(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx/*=VCN_CITY_MAP_INDEX*/)
{
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CFieldIOCP::GetCityFieldMapProjectPtrByInfluenceW#
	return m_pFieldMapWorkspace->GetCityFieldMapProjectPtrByInfluence(i_byInfluence, i_nStartCityMapIdx);
}


void CFieldIOCP::SendLogMessageServerInfo(void)
{
	if (m_pFieldMapWorkspace && m_pFieldMapWorkspace->GetMapProjectCounts())
	{
		m_pFieldMapWorkspace->SendLogServerInfo();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::CheckPacketPriority(DWORD i_dwSendCounts, EN_NETWORK_STATE i_enMapNetworkState)
/// \brief		패킷 레벨링 관련, 전송하는 메세지 타입별로 우선순위가 결정된다.
/// \author		cmkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning
///
/// \param		i_dwSendCounts		[in] 전송하는 데이타 타입의 전송 Count
/// \param		i_enMapNetworkState	[in] 전송하는 데이타가 포함된 맵의 NetworkState
/// \return		TRUE일때는 전송해야하는 데이타임, FALSE일때는 삭제되어지는 데이타임
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::CheckPacketPriority(DWORD i_dwSendCounts, DWORD i_dwModulusNumber)
{
	if (i_dwModulusNumber < MAX_MODULUS_NUMBER_TOTAL_NETWORK_STATE
		&& i_dwSendCounts%i_dwModulusNumber == 0)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::OnDoHourlyWorkIOCP(ATUM_DATE_TIME *pDateTime)
/// \brief		
/// \author		dhjin
/// \date		2006-09-28 ~ 2006-09-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::OnDoHourlyWorkIOCP(ATUM_DATE_TIME *pDateTime)
{
	if (m_pAllGuildItemManager)
	{
		m_pAllGuildItemManager->OnDoHourlyWorkAllGuildItemManager(pDateTime);
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-08-19 by dhjin, MySQL포팅 문제로 MySQL에서 지원하지 않는 MSSQL에 Job관련 작업을 여기서 처리한다.
	if (6 == pDateTime->Hour)
	{// 매일 오전 6시에 Job을 수행한다.
		// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - DQT_DailyJob 로 변경 함.
		//m_pAtumDBManager->MakeAndEnqueueQuery(QT_DailyJob, NULL, 0, NULL);
		m_pAtumDBManager->MakeQueryWithNewThread(DQT_DailyJob, NULL, NULL);
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-11-04 by dhjin, 럭키머신
	this->CheckLuckyItemPeriod(pDateTime);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 1시간 마다 다시 로딩
	m_pAtumDBManager->MakeQueryWithNewThread(DQT_LoadInfluenceRate, NULL, NULL);


	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 자정마다 다시 설정됨
	if (0 == pDateTime->Hour
		&& FALSE == g_pFieldGlobal->IsArenaServer()
		&& FALSE == g_pFieldGlobal->GetIsJamboreeServer())
	{
		m_pAtumDBManager->MakeQueryWithNewThread(DQT_ReloadWRKServiceList, NULL, NULL);

		// 2010-01-14 by cmkwon, 월드랭킹 리스트 리로드 구조 변경 - DQT_ReloadWRKServiceList 처리 후에 자동으로 호출되게 수정
		//m_pAtumDBManager->MakeQueryWithNewThread(DQT_ReloadWRKLevel, NULL, NULL, NULL, TRUE);
		//m_pAtumDBManager->MakeQueryWithNewThread(DQT_ReloadWRKFame, NULL, NULL, NULL, TRUE);
		//m_pAtumDBManager->MakeQueryWithNewThread(DQT_ReloadWRKPVP, NULL, NULL, NULL, TRUE);
	}


	// 2013-04-18 by jhseol,bckim 이달의 아머 - 1시간 마다 체크하여 전체 유저에게 이달의 아머 이벤트 정보 보내기
#ifdef S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// 1시간 마다 체크하여 전체 유저에게 이달의 아머 이벤트 정보 보내기
	if (NULL != m_pMonthlyArmorManager)
	{
		m_pMonthlyArmorManager->SendMonthlyArmorEventListToAllClients();
	}
#endif
	// end 2013-04-18 by jhseol,bckim 이달의 아머 - 1시간 마다 체크하여 전체 유저에게 이달의 아머 이벤트 정보 보내기

#if S_BONUS_KILL_SYSTEM_RENEWAL
	this->CleanKillCountCache();
#endif // S_BONUS_KILL_SYSTEM_RENEWAL

}

void CFieldIOCP::OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-30 by cmkwon
	g_pFieldGlobal->ChecklRequestInCRTLib();

	if (FALSE == this->GetListeningFlag()) { return; }

	m_nPCBangMinTick++;

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	this->ResetInfinityByResetCycle(pDateTime);

	// 2007-11-26 by cmkwon, HappyHourEvent 버그수정 - 아래와 같이 수정함.
	//	if(FALSE == m_pHappyHourEventManager->OnDoMinutelyWorkHappyHourEventManager(pDateTime))
	//	{
	//		m_gameEventManager.OnDoMinutelyWorkGameEventTime(pDateTime);
	//	}
	m_pHappyHourEventManager->OnDoMinutelyWorkHappyHourEventManager(pDateTime);
	m_gameEventManager.OnDoMinutelyWorkGameEventTime(pDateTime);
#ifdef _AUTO_DGA
	m_pDGAEventManager->OnDoMinutelyWorkDGAEventManager(pDateTime);
#endif

	m_pFieldMapWorkspace->OnDoMinutelyWorkFieldMapWorkspace();
	// 2005-08-23 by cmkwon, 도시점령전 삭제 예정	
	//	m_cityWarManager.OnDoMinutelyWorkCityWarManager(pDateTime);

		///////////////////////////////////////////////////////////////////////////////
		// 2006-04-14 by cmkwon
	m_InflWarManager.OnDoMinutelyWorkInflWarManager(pDateTime);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-17 by cmkwon, 전광판 스트링 유지시간 체크
	this->SignBoardCheckExpireTime(pDateTime);

	// 2009-03-09 by cmkwon, 일본 Arario nProtect에 CS인증 적용하기 - 아래와 같이 주기 변수로 체크하게 수정
	//int nRemainderMinute = pDateTime->Minute%3;
	//for (int i = CLIENT_INDEX_START_NUM; i < m_dwArrayClientSize; i++)
	int nRemainNum = pDateTime->Minute%CFieldIOCPSocket::ms_nPeriodMinutesforSecurityCheck;
	for (int i = CLIENT_INDEX_START_NUM + nRemainNum; i < m_dwArrayClientSize; i = i + CFieldIOCPSocket::ms_nPeriodMinutesforSecurityCheck)
	{
		// 2008-04-10 by cmkwon, 1분에 한번씩 핵쉴드 체크를 한다.
		//if(nRemainderMinute == i%3)
		{
			CFieldIOCPSocket *pFISock = this->GetFieldIOCPSocket(i);

			// 2008-01-10 by cmkwon, IsCheckInfluenceTypeANDSocketType() 버그 수정 - RACE_GAMEMASTER 체크 필요 없음
			if (pFISock
				&& pFISock->IsValidCharacter(FALSE, TRUE)		// 2010-04-22 by cmkwon, 아레나 플레이 상태도 유효한 캐릭터로 처리 - 게임가드 처리는 
				&& pFISock->IsCheckInfluenceTypeANDSocketType(INFLUENCE_TYPE_ALL_MASK, ST_CLIENT_TYPE, FALSE))
			{
				// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 아래와 같이 수정함
				//pFISock->SecurityCheckClient_1();
				pFISock->SecurityClientCheck();		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 사용하지 않음
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-16 by dhjin, 전진기지 관련 
	m_OutPostManager.OnDoMinutelyWorkOutPostManager(pDateTime);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 1분마다 스레드 핸들 종료 체크
	m_pAtumDBManager->CheckAndCloseHandleDynamicDBThread();

	// 2009-02-11 by dhjin, 21시 10분, 22시 10분으로 변경
	///////////////////////////////////////////////////////////////////////////////



}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::OnDoSecondlyWorkFIOCP(ATUM_DATE_TIME *pDateTime)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::OnDoSecondlyWorkFIOCP(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldMapWorkspace->OnDoSecondlyWorkFieldMapWorkspace(pDateTime);
#if defined(_WARRIOR_ENABLE_SERVER)
	//start warriorsw 2015-03-03
	int nRemainNum = pDateTime->Minute%CFieldIOCPSocket::ms_nPeriodMinutesforSecurityCheck;
	for (int i = CLIENT_INDEX_START_NUM + nRemainNum; i < m_dwArrayClientSize; i = i + CFieldIOCPSocket::ms_nPeriodMinutesforSecurityCheck)
	{
		// 2008-04-10 by cmkwon, 1분에 한번씩 핵쉴드 체크를 한다.
		//if(nRemainderMinute == i%3)
		{
			CFieldIOCPSocket *pFISock = this->GetFieldIOCPSocket(i);

			// 2008-01-10 by cmkwon, IsCheckInfluenceTypeANDSocketType() 버그 수정 - RACE_GAMEMASTER 체크 필요 없음
			if (pFISock
				&& pFISock->IsValidCharacter(FALSE, TRUE)		// 2010-04-22 by cmkwon, 아레나 플레이 상태도 유효한 캐릭터로 처리 - 게임가드 처리는 
				&& pFISock->IsCheckInfluenceTypeANDSocketType(INFLUENCE_TYPE_ALL_MASK, ST_CLIENT_TYPE, FALSE))
			{
				// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 아래와 같이 수정함
				//pFISock->SecurityCheckClient_1();
				pFISock->SecurityClientCheck();		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 사용하지 않음
			}
		}
	}
#endif
}

CFieldIOCPSocket* CFieldIOCP::GetFieldIOCPSocketByCharacterUniqueNumber(UID32_t uniqueNumber, BOOL i_bPermissionArenaState/*=FALSE*/)	// 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
{
	CFieldIOCPSocket *pRetFISock = m_mapCharacterUniqueNumber.findLock(uniqueNumber);
	if (NULL == pRetFISock
		|| FALSE == pRetFISock->IsValidCharacter(FALSE, i_bPermissionArenaState)	// 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
		|| uniqueNumber != pRetFISock->GetCharacter()->CharacterUniqueNumber)
	{
		return NULL;
	}

	return pRetFISock;
}

void CFieldIOCP::ApplyBuffSkillToGuild(INT itemNum, INT guildUID)
{
	for (auto &socket : m_mapCharacterUniqueNumber)
	{
		if (socket.second->GetCharacter()->GuildUniqueNumber == guildUID)
		{
			socket.second->ApplyBuffSkill(itemNum);
		}
	}
}

CFieldIOCPSocket* CFieldIOCP::GetFieldIOCPSocketByCharacterName(char* i_szCharacterName)
{
	if (NULL == i_szCharacterName
		|| strlen(i_szCharacterName) == 0)
	{
		return NULL;
	}

	char tmCharacterName[SIZE_MAX_CHARACTER_NAME];
	CFieldIOCPSocket *pRetFISock = m_mapCharacterName.findLock(GGetLowerCase(tmCharacterName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME));
	if (NULL == pRetFISock
		|| FALSE == pRetFISock->CompareCharacterName(i_szCharacterName))
	{
		return NULL;
	}
	return pRetFISock;
}

///////////////////////////////////////////////////////////////////////////////
// 2012-10-05 by jhseol, NPlay pc방 모듈 - AccountName으로 유저 소켓 찾기
CFieldIOCPSocket* CFieldIOCP::GetFieldIOCPSocketByAccountName(char* i_szAccountName)
{
	char tmAccountName[SIZE_MAX_ACCOUNT_NAME];
	CFieldIOCPSocket *pRetFISock = m_mapAccountName.findLock(GGetLowerCase(tmAccountName, i_szAccountName, SIZE_MAX_ACCOUNT_NAME));
	if (NULL == pRetFISock)
	{
		return NULL;
	}
	return pRetFISock;
}
// end 2012-10-05 by jhseol, NPlay pc방 모듈 - AccountName으로 유저 소켓 찾기

CHARACTER* CFieldIOCP::GetCharacterByArrayIndex(int idx)
{
	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCPSocket(idx);
	if (pFieldIOCPSocket == NULL)
	{
		return NULL;
	}

	return (CHARACTER*)pFieldIOCPSocket->GetCharacter();
}

BOOL CFieldIOCP::GetUnitPlanePositonByClientIndex(CFieldMapChannel *pCurMapChannel
	, int i_nIdx
	, float *o_pX, float *o_pZ)
{
	if (IS_CHARACTER_CLIENT_INDEX(i_nIdx))
	{
		CFieldIOCPSocket *fsock = this->GetFieldIOCPSocket(i_nIdx);
		if (NULL == fsock)
		{
			return FALSE;
		}

		*o_pX = fsock->m_character.PositionVector.x;
		*o_pZ = fsock->m_character.PositionVector.z;
	}
	else if (IS_MONSTER_CLIENT_INDEX(i_nIdx))
	{
		if (NULL == pCurMapChannel)
		{
			return FALSE;
		}
		CMonster *pMon = pCurMapChannel->GetMonster(i_nIdx);
		if (NULL == pMon)
		{
			return FALSE;
		}

		*o_pX = pMon->PositionVector.x;
		*o_pZ = pMon->PositionVector.z;
	}
	else
	{
		g_pFieldGlobal->WriteSystemLog("Invalid ClientIndex @GetUnitPlanePositonByClientIndex(), %d\r\n", i_nIdx);
		return FALSE;
	}

	return TRUE;
}


// 2006-08-28 by cmkwon, i_arrItemWithCount의 자료형을 ITEM_W_COUNT에서 ITEM_W_COUNT_CHECK로 변경함
// 2008-03-17 by cmkwon, 조합 시스템 기능 추가 - 인자추가(INT i_nMixCnts)
ITEM_MIXING_INFO* CFieldIOCP::CheckAndGetItemMixingInfo(ITEM_W_COUNT_CHECK *i_arrItemWithCount, INT i_nItemWithCount, INT i_nMixCnts)
{
	int nItemMatch = 0;
	int nCountMatch = 0;

	for (int j = 0; j < m_vectorItemMixingInfo.size(); j++)
	{
		ITEM_MIXING_INFO *pItemMixingInfo = &m_vectorItemMixingInfo[j];
		if (pItemMixingInfo->NumOfSourceItems != i_nItemWithCount)
		{
			continue;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2006-08-28 by cmkwon, 다시 체크하기 위한 변수들 초기화
		nItemMatch = 0;
		nCountMatch = 0;
		for (int i = 0; i < i_nItemWithCount; i++)
		{
			i_arrItemWithCount[i].bChecked = FALSE;
		}

		// 2006-08-28 by cmkwon, 같이 아이템 두번 비교하는 버그를 아래와 같이 수정함
		// 		for (int i = 0; i < i_nItemWithCount; i++)
		// 		{
		// 			ITEM_W_COUNT &tmpSourceItemWithCount = i_arrItemWithCount[i];
		// 
		// 			for (int k = 0; k < COUNT_ITEM_MIXING_SOURCE; k++)
		// 			{
		// 				if (pItemMixingInfo->SourceItem[k].ItemNum == 0)
		// 				{
		// 					continue;
		// 				}
		// 
		// 				if (pItemMixingInfo->SourceItem[k].ItemNum == tmpSourceItemWithCount.ItemNum)
		// 				{
		// 					nItemMatch++;
		// 					if (pItemMixingInfo->SourceItem[k].Count == tmpSourceItemWithCount.Count)
		// 					{
		// 						nCountMatch++;
		// 					}
		// 					break;
		// 				}
		// 			}
		// 
		// 			if (nItemMatch == i_nItemWithCount 
		// 				&& nCountMatch == i_nItemWithCount
		// 				&& pItemMixingInfo->NumOfSourceItems == i_nItemWithCount)
		// 			{
		// 				return pItemMixingInfo;
		// 			}
		// 		}// END - 		for (int i = 0; i < i_nItemWithCount; i++)


		for (int k = 0; k < COUNT_ITEM_MIXING_SOURCE; k++)
		{
			if (pItemMixingInfo->SourceItem[k].ItemNum == 0)
			{
				continue;
			}

			for (int i = 0; i < i_nItemWithCount; i++)
			{
				ITEM_W_COUNT_CHECK &tmpSItemWCountCheck = i_arrItemWithCount[i];
				if (FALSE == tmpSItemWCountCheck.bChecked
					&& pItemMixingInfo->SourceItem[k].ItemNum == tmpSItemWCountCheck.ItemNum)
				{
					nItemMatch++;

					// 2008-03-17 by cmkwon, 조합 시스템 기능 추가 - 비교식 수정, 조합회수 만큼 체크
					//if (pItemMixingInfo->SourceItem[k].Count == tmpSItemWCountCheck.Count)
					if (pItemMixingInfo->SourceItem[k].Count * i_nMixCnts == tmpSItemWCountCheck.Count)
					{
						nCountMatch++;
						tmpSItemWCountCheck.bChecked = TRUE;		// 2006-08-28 by cmkwon, 한번 비교한 아이템은 비교하지 않기 위해서
					}
					break;
				}
			}

			if (nItemMatch == i_nItemWithCount
				&& nCountMatch == i_nItemWithCount
				&& pItemMixingInfo->NumOfSourceItems == i_nItemWithCount)
			{
				return pItemMixingInfo;
			}
		}// END - for (int k = 0; k < COUNT_ITEM_MIXING_SOURCE; k++)
	}// END - for (int j = 0; j < m_vectorItemMixingInfo.size(); j++)

	return NULL;
}

// start 2012-03-16 by hskim, DB 조합식 정리 (고정값 수식으로 변경)
ITEM_MIXING_INFO* CFieldIOCP::CheckAndGetItemFixMixingInfo(ITEM_W_COUNT_CHECK *i_arrItemWithCount, INT i_nItemWithCount, INT i_nMixCnts, ITEM_MIXING_INFO *i_pItemMixingInfo)
{
	static_assert(COUNT_ITEM_FIX_MIXING_SOURCE == 2, "Review this method before changing COUNT_ITEM_FIX_MIXING_SOURCE");

	INT iCheckSourceItemKind[COUNT_ITEM_FIX_MIXING_SOURCE] = { -1, -1 };
	INT iCheckSourceReqUnitKind[COUNT_ITEM_FIX_MIXING_SOURCE] = { -1, -1 };

	// Check preconditions
	if (i_nItemWithCount != COUNT_ITEM_FIX_MIXING_SOURCE || i_nMixCnts != 1 || !i_pItemMixingInfo) return nullptr;


	// 고정 조합식 종류
	// 무기 이펙트/무기 외형/아머 외형

	for (auto i = 0; i < i_nItemWithCount; i++)
	{
		auto& tmpSItemWCountCheck = i_arrItemWithCount[i];

		auto pItemInfo = GetItemInfo(tmpSItemWCountCheck.ItemNum);

		if (!pItemInfo || tmpSItemWCountCheck.Count != 1) return nullptr;

		if (IS_WEAPON(pItemInfo->Kind) || ITEMKIND_DEFENSE == pItemInfo->Kind || ITEMKIND_PET_ITEM == pItemInfo->Kind)		// 2012-12-03 by hskim, 파트너 기간제 외형 변경
		{
			iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT] = pItemInfo->Kind;
			iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT] = pItemInfo->ReqUnitKind;

			i_pItemMixingInfo->SourceItem[1].ItemNum = pItemInfo->ItemNum;
			i_pItemMixingInfo->SourceItem[1].Count = 1;

			continue;
		}

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		if (pItemInfo->IsExistDesParam(DES_COLLECTION_ARMOR_INDEX)) return nullptr;
#endif

		if (pItemInfo->IsExistDesParam(DES_SHAPE_ITEM) && pItemInfo->IsExistDesParam(DES_EFFECT_ITEM))
		{
			auto fParameterValue = pItemInfo->GetParameterValue(DES_SHAPE_ITEM);
			auto fParameterValue2 = pItemInfo->GetParameterValue(DES_EFFECT_ITEM);

			if (fParameterValue == fParameterValue2)
			{
				iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = fParameterValue;
				iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->ReqUnitKind;
				i_pItemMixingInfo->TargetItemNum = pItemInfo->ItemNum;
			}

			continue;
		}

		if (pItemInfo->IsExistDesParam(DES_SHAPE_ITEM))
		{
			iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->GetParameterValue(DES_SHAPE_ITEM);
			iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->ReqUnitKind;
			i_pItemMixingInfo->TargetItemNum = pItemInfo->ItemNum;

			continue;
		}

		if (pItemInfo->IsExistDesParam(DES_EFFECT_ITEM))
		{
			iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->GetParameterValue(DES_EFFECT_ITEM);
			iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->ReqUnitKind;
			i_pItemMixingInfo->TargetItemNum = pItemInfo->ItemNum;

			continue;
		}

		if (pItemInfo->IsExistDesParam(DES_INIT_EFFECT_ITEM))
		{
			iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->GetParameterValue(DES_INIT_EFFECT_ITEM);
			iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->ReqUnitKind;
			i_pItemMixingInfo->TargetItemNum = pItemInfo->ItemNum;

			continue;
		}

		if (pItemInfo->IsExistDesParam(DES_INIT_SHAPE_ITEM))
		{
			iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->GetParameterValue(DES_INIT_SHAPE_ITEM);
			iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD] = pItemInfo->ReqUnitKind;
			i_pItemMixingInfo->TargetItemNum = pItemInfo->ItemNum;

			continue;
		}
	}

	if (!IS_GENERAL_ITEM(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]) || !IS_GENERAL_ITEM(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])) return nullptr;

	i_pItemMixingInfo->MixingProbability = 10000;		// 확율 100 %
	i_pItemMixingInfo->MixingCost = 0;					// 비용 0 SPI
	i_pItemMixingInfo->NumOfSourceItems = 2;			// 조합 2개
	i_pItemMixingInfo->SourceItem[0].ItemNum = i_pItemMixingInfo->TargetItemNum;
	i_pItemMixingInfo->SourceItem[0].Count = 1;

	// 결과 값 리턴

	if (IS_PRIMARY_WEAPON(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])		// 무기 외형 - 기본 무기 A 기어
		&& IS_AGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])
		&& KIND_ITEM_FIX_MIXING_REQ_PRIMARY_WEAPON_A == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (IS_PRIMARY_WEAPON(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])		// 무기 외형 - 기본 무기 BMI 기어
		&& (IS_BGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]) || IS_MGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]) || IS_IGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]))
		&& KIND_ITEM_FIX_MIXING_REQ_PRIMARY_WEAPON_BMI == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (IS_SECONDARY_WEAPON(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])		// 무기 외형 - 고급 무기 A 기어
		&& IS_AGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])
		&& KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAPON_A == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (IS_SECONDARY_WEAPON(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])		// 무기 외형 - 고급 무기 BMI 기어
		&& (IS_BGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]) || IS_MGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]) || IS_IGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]))
		&& KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAPON_BMI == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (IS_SECONDARY_WEAPON(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])		// 무기 외형 - 고급 무기 이펙트
		&& KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAPON == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (ITEMKIND_DEFENSE == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]		// 아머 외형 - B 기어
		&& IS_BGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])
		&& KIND_ITEM_FIX_MIXING_REQ_DEFENSE_B == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (ITEMKIND_DEFENSE == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]		// 아머 외형 - M 기어
		&& IS_MGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])
		&& KIND_ITEM_FIX_MIXING_REQ_DEFENSE_M == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (ITEMKIND_DEFENSE == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]		// 아머 외형 - A 기어
		&& IS_AGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])
		&& KIND_ITEM_FIX_MIXING_REQ_DEFENSE_A == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (ITEMKIND_DEFENSE == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]		// 아머 외형 - I 기어
		&& IS_IGEAR(iCheckSourceReqUnitKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT])
		&& KIND_ITEM_FIX_MIXING_REQ_DEFENSE_I == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (ITEMKIND_DEFENSE == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]		// 아머 외형 - 초기화
		&& KIND_ITEM_FIX_MIXING_REQ_DEFENSE == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	if (IS_SECONDARY_WEAPON(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]) 		// 이펙트 외형 - 고급 무기 초기화
		&& KIND_ITEM_FIX_MIXING_REQ_SECOND_WEAP == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	// 2012-12-03 by hskim, 파트너 기간제 외형 변경
#ifdef SC_PARTNER_SHAPE_CHANGE_HSKIM
	if (ITEMKIND_PET_ITEM == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT] 		// 파트너 외형
		&& KIND_ITEM_FIX_MIXING_REQ_PET_ITEM == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}
#endif
	// end 2012-12-03 by hskim, 파트너 기간제 외형 변경

	if (IS_WEAPON(iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EQUIPMENT]) 		// 이펙트 외형 - 고급 무기 초기화
		&& KIND_ITEM_FIX_MIXING_REQ_WEAPON == iCheckSourceItemKind[INDEX_ITEM_FIX_MIXING_EFFECT_CARD])
	{
		return i_pItemMixingInfo;
	}

	return NULL;
}
// end 2012-03-16 by hskim, DB 조합식 정리 (고정값 수식으로 변경)

void CFieldIOCP::DeleteFieldParty(CFieldParty *i_pFParty)
{
	i_pFParty->m_dwPartyDeletedTick = timeGetTime();
	mt_auto_lock mtA(&m_mtvectorDeletedFieldParties);
	mt_vector<CFieldParty*>::iterator itr = find(m_mtvectorDeletedFieldParties.begin(), m_mtvectorDeletedFieldParties.end(), i_pFParty);
	if (itr == m_mtvectorDeletedFieldParties.end())
	{
		m_mtvectorDeletedFieldParties.pushBackLock(i_pFParty);
	}
	mtA.auto_unlock_cancel();

	this->AdjustDeletedFieldParties();			// 지워진 파티 정리
}

void CFieldIOCP::AdjustDeletedFieldParties(void)
{
	if (m_mtvectorDeletedFieldParties.empty()) { return; }

	m_mtvectorDeletedFieldParties.lock();
	if (m_mtvectorDeletedFieldParties.empty())
	{
		m_mtvectorDeletedFieldParties.unlock();
		return;
	}

	CFieldParty *pFParty;
	DWORD dwCurTick = timeGetTime();
	mt_vector<CFieldParty*>::iterator it(m_mtvectorDeletedFieldParties.begin());
	while (it != m_mtvectorDeletedFieldParties.end())
	{
		pFParty = *it;
		if (dwCurTick - pFParty->m_dwPartyDeletedTick < 300000)
		{// 2006-07-04 by cmkwon, 지워진지 5분이 지나지 않은 것은 삭제하지 않는다, 뒤의 것들도 체크할 필요 없음

			break;
		}
		util::del(pFParty);
		it = m_mtvectorDeletedFieldParties.erase(it);
	}
	m_mtvectorDeletedFieldParties.unlock();
}


void CFieldIOCP::OnCityWarStartFieldIOCP(CCityWar *i_pCityWar)
{
	CFieldMapChannel *pFMChann = this->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pCityWar->MapIndex, 0));
	if (NULL == pFMChann)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// NPC Server로 점령전 시작 정보 전송
	INIT_MSG_WITH_BUFFER(MSG_FN_CITYWAR_START_WAR, T_FN_CITYWAR_START_WAR, pSMsg, SendBuf);
	pSMsg->ChannelIndex = pFMChann->GetMapChannelIndex().ChannelIndex;
	pFMChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_CITYWAR_START_WAR));

	///////////////////////////////////////////////////////////////////////////////
	// 도시점령전 시작 정보를 모든 유저에게 전송한다
	INIT_MSG(MSG_FC_CITYWAR_START_WAR, T_FC_CITYWAR_START_WAR, pS2CStartWar, SendBuf);
	pS2CStartWar->CityWarMapIndex3 = pFMChann->GetMapChannelIndex().MapIndex;
	pS2CStartWar->OccupyGuildUID3 = i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1;
	util::strncpy(pS2CStartWar->szOccupyGuildName3, i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1, SIZE_MAX_GUILD_NAME);
	pS2CStartWar->atimeCityWarEndTime = i_pCityWar->m_ATimeWarEndTime;
	this->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_CITYWAR_START_WAR));

	//	///////////////////////////////////////////////////////////////////////////////
	//	// 서버군 모두에게 점령전 시작 정보 전송
	//	char szNotice[1024];
	//	sprintf(szNotice, "\"%s\" 도시점령전이 시작 되었습니다.", i_pCityWar->MapName);	
	//	this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);	
	//
	//	sprintf(szNotice, "\"%s\" 맵에 모든 유저(점령 여단 제외)는", i_pCityWar->MapName);	
	//	this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//	this->SendString128ToAllClients(STRING_128_USER_NOTICE, "    도시로 워프 됩니다.", TRUE);

		///////////////////////////////////////////////////////////////////////////////
		// 점령 여단을 제외한 모든 유저 모두 도시맵으로 귀환
	pFMChann->WarpToCityMapFieldMapChannel(m_DefaultMapIndex, i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1);

	///////////////////////////////////////////////////////////////////////////////
	// 점령 여단의 CityWarTeamType을 재설정한다.
	this->SetCityWarTeamTypeByMapIndex(i_pCityWar, pFMChann->m_pFieldMapProject);
}

void CFieldIOCP::OnCityWarEndFieldIOCP(CCityWar *i_pCityWar, vectCITYWAR_QUEST *i_pCityWarQuestResultList)
{
	CFieldMapChannel *pFMChann = this->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pCityWar->MapIndex, 0));
	if (NULL == pFMChann)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 도시점령전 퀘스트 결과 처리를 한다
	for (int i = 0; i < i_pCityWarQuestResultList->size(); i++)
	{
		SCITYWAR_QUEST &warQuest = (*i_pCityWarQuestResultList)[i];
		CFieldIOCPSocket *pFISock = this->GetFieldIOCPSocketByCharacterUniqueNumber(warQuest.nCityWarQuestCharacterUID1);
		if (NULL == pFISock
			|| FALSE == pFISock->IsValidCharacter(FALSE))
		{
			if (FALSE == warQuest.bQuesCompletion)
			{
				// 퀘스트 지우기 - DB
				QPARAM_DELETE_QUEST *pQDeleteQuest = new QPARAM_DELETE_QUEST;
				pQDeleteQuest->CharacterUniqueNumber = warQuest.nCityWarQuestCharacterUID1;
				pQDeleteQuest->QuestIndex = i_pCityWar->CityWarQuestIndex;
				m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteQuest, NULL, 0, pQDeleteQuest);
			}
			else
			{
				// Update DB
				QPARAM_UPDATE_QUEST *pQUpdateQuest = new QPARAM_UPDATE_QUEST;
				pQUpdateQuest->CharacterUniqueNumber = warQuest.nCityWarQuestCharacterUID1;
				pQUpdateQuest->QuestIndex = i_pCityWar->CityWarQuestIndex;
				pQUpdateQuest->QuestState = QUEST_STATE_COMPLETED;
				pQUpdateQuest->QuestProgressTimeInSecond = 0;	// 2006-08-31 by cmkwon, 캐릭터가 게임 중이지 않은 상태에서 퀘스트 완료는 문제 있음, 여기는 현재는 사용되지 안는 모듈임
				m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateQuestState, NULL, 0, pQUpdateQuest);
			}
		}
		else
		{
			pFISock->CityWarQuestResult(i_pCityWar->CityWarQuestIndex, warQuest.bQuesCompletion);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 점령 여단의 CityWarTeamType을 재설정한다.
	this->SetCityWarTeamTypeByMapIndex(i_pCityWar, pFMChann->m_pFieldMapProject);

	///////////////////////////////////////////////////////////////////////////////
	// 모니터로 변경 정보 전송
	INIT_MSG_WITH_BUFFER(MSG_FM_CITYWAR_CHANGED_OCCUPY_INFO, T_FM_CITYWAR_CHANGED_OCCUPY_INFO, pSMsg, SendBuf);
	pSMsg->MapChannelIndex.MapIndex = i_pCityWar->MapIndex;
	pSMsg->MapChannelIndex.ChannelIndex = 0;
	pSMsg->cityWarOccupyInfo = *i_pCityWar->GetCITY_OCCUPY_INFO();
	this->SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_FM_CITYWAR_CHANGED_OCCUPY_INFO));

	///////////////////////////////////////////////////////////////////////////////
	// NPC Server로 점령전 종료 정보 전송
	INIT_MSG(MSG_FN_CITYWAR_END_WAR, T_FN_CITYWAR_END_WAR, pSEndWar, SendBuf);
	pSEndWar->ChannelIndex = pFMChann->GetMapChannelIndex().ChannelIndex;
	pSEndWar->OccupyGuildUID4 = i_pCityWar->OccupyGuildUID;
	pFMChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_CITYWAR_END_WAR));

	///////////////////////////////////////////////////////////////////////////////
	// 도시점령전 종료 정보를 모든 유저에게 전송한다
	INIT_MSG(MSG_FC_CITYWAR_END_WAR, T_FC_CITYWAR_END_WAR, pS2CEndWar, SendBuf);
	pS2CEndWar->CityWarMapIndex3 = pFMChann->GetMapChannelIndex().MapIndex;
	pS2CEndWar->OccupyGuildUID3 = i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1;
	util::strncpy(pS2CEndWar->szOccupyGuildName3, i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1, SIZE_MAX_GUILD_NAME);
	pS2CEndWar->atimeNextCityWarDefaultTime = i_pCityWar->CityWarDefaultTime;
	this->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_CITYWAR_END_WAR));

	//	///////////////////////////////////////////////////////////////////////////////
	//	// 서버군 모두에게 점령전 종료 정보 전송
	//	char szNotice[1024];
	//	sprintf(szNotice, "\"%s\" 도시점령전이 종료 되었습니다.", i_pCityWar->MapName);
	//	this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);	
	//	if(INVALID_GUILD_UID == i_pCityWar->OccupyGuildUID)
	//	{
	//		sprintf(szNotice, "\"NPC\"가 \"%s\"을 점령하였습니다.", i_pCityWar->MapName);
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);	
	//		
	//		sprintf(szNotice, "다음 \"%s\" 도시점령전은", i_pCityWar->MapName);
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);	
	//
	//		sprintf(szNotice, "    \"%s\"에 진행됩니다."		
	//			, i_pCityWar->CityWarSettingTime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING), TRUE));
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//	}
	//	else
	//	{
	//		sprintf(szNotice, "\"%s\"여단이 \"%s\"을 점령하였습니다.", i_pCityWar->OccupyGuildName, i_pCityWar->MapName);	
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//		
	//		sprintf(szNotice, "다음 \"%s\" 도시점령전은", i_pCityWar->MapName);
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//
	//		sprintf(szNotice, "    \"%s\"에 진행됩니다."		
	//			, i_pCityWar->CityWarSettingTime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING), TRUE));
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//	}
	//
	// 2005-02-09 by cmkwon, 현재는 여단장이 시간 변경을 할 수 없으므로 위의 것으로 처리함
	//	if(INVALID_GUILD_UID == i_pCityWar->OccupyGuildUID)
	//	{
	//		sprintf(szNotice, "\"NPC\"가 \"%s\"을 점령하였습니다.", i_pCityWar->MapName);
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);	
	//		
	//		sprintf(szNotice, "다음 \"%s\" 도시점령전은", i_pCityWar->MapName);
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);	
	//
	//		sprintf(szNotice, "    \"%s\"에 진행됩니다."		
	//			, i_pCityWar->CityWarSettingTime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING), TRUE));
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//	}
	//	else
	//	{
	//		sprintf(szNotice, "\"%s\"여단이 \"%s\"을 점령하였습니다.", i_pCityWar->OccupyGuildName, i_pCityWar->MapName);	
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//		
	//		sprintf(szNotice, "다음 \"%s\" 도시점령전은", i_pCityWar->MapName);
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//
	//		ATUM_DATE_TIME tmATime = i_pCityWar->CityWarSettingTime;
	//		tmATime.AddDateTime(0, 0, 0, TIMEGAP_CITYWAR_CHANGEABLE_TERM_HOUR, 0, 0);
	//		sprintf(szNotice, "    \"%s ~ %s\"에 진행됩니다."		
	//			, i_pCityWar->CityWarSettingTime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING), TRUE)
	//			, tmATime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING), TRUE));
	//		this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
	//	}
}

void CFieldIOCP::OnCityWarInfoFieldIOCP(CCityWar *i_pCityWar, int i_nRemainMinute)
{
	// 2005-12-28 by cmkwon
	//	char szNotice[1024];
	//	switch(i_pCityWar->GetCityWarState())
	//	{
	//	case CITYWAR_STATE_READY:
	//		{
	//			sprintf(szNotice, STRMSG_S_F2CITYWAR_0006, i_nRemainMinute, i_pCityWar->MapName);
	//		}
	//		break;
	//	case CITYWAR_STATE_STARTED:
	//		{
	//			sprintf(szNotice, STRMSG_S_F2CITYWAR_0007, i_nRemainMinute, i_pCityWar->MapName);
	//		}
	//		break;	
	//	default:
	//		{
	//			return;
	//		}
	//	}
	//	this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
}

void CFieldIOCP::OnCityWarQuestMonsterCreated(MapIndex_t i_mapIdx)
{
	// 2005-12-28 by cmkwon
	//
	//	///////////////////////////////////////////////////////////////////////////////
	//	// 
	//	CCityWar *pCityWar = m_cityWarManager.OnCityWarQuestMonsterCreatedCityWarManager(i_mapIdx);
	//	if(NULL == pCityWar)
	//	{
	//		return;
	//	}
	//
	//	///////////////////////////////////////////////////////////////////////////////
	//	// 서비스 시에는 필요 없음
	//	char szNotice[512];
	//	if(0 == pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1)
	//	{
	//		sprintf(szNotice,STRMSG_S_F2CITYWAR_0008, pCityWar->MapName);
	//	}
	//	else
	//	{
	//		sprintf(szNotice,STRMSG_S_F2CITYWAR_0009, pCityWar->MapName, pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1);
	//	}
	//	this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);
}

void CFieldIOCP::OnCityWarQuestMonsterDead(CCityWar *i_pCityWar)
{
	///////////////////////////////////////////////////////////////////////////////
	// 점령 여단이 변경되어 설정
	CCityWar *pCityWar = m_cityWarManager.OnCityWarQuestMonsterDeadCityWarManager(i_pCityWar->MapIndex);
	if (NULL == pCityWar)
	{
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FC_CITYWAR_MONSTER_DEAD, T_FC_CITYWAR_MONSTER_DEAD, pSMonDead, SendBuf);
	pSMonDead->CityWarMapIndex3 = i_pCityWar->MapIndex;
	pSMonDead->OccupyGuildUID3 = i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1;
	util::strncpy(pSMonDead->szOccupyGuildName3, i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1, SIZE_MAX_GUILD_NAME);
	this->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_CITYWAR_MONSTER_DEAD));
	// 2005-05-16 by cmkwon, 위의 메시지로 대체
	//	char szNotice[512];
	//	if(0 == pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1)
	//	{
	//		sprintf(szNotice,STRMSG_S_F2CITYWAR_0010, pCityWar->MapName);
	//	}
	//	else
	//	{
	//		sprintf(szNotice,STRMSG_S_F2CITYWAR_0011, pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildName1, pCityWar->MapName);
	//	}
	//	this->SendString128ToAllClients(STRING_128_USER_NOTICE, szNotice, TRUE);

		///////////////////////////////////////////////////////////////////////////////
		// 점령 여단을 제외한 모든 유저 모두 도시맵으로 귀환
	CFieldMapChannel *pFMChann = this->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pCityWar->MapIndex, 0));
	if (pFMChann)
	{
		pFMChann->WarpToCityMapFieldMapChannel(m_DefaultMapIndex, i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 점령 여단의 CityWarTeamType을 재설정한다.
	this->SetCityWarTeamTypeByMapIndex(i_pCityWar, pFMChann->m_pFieldMapProject);

	///////////////////////////////////////////////////////////////////////////////
	// NPC 서버로 점령여단 정보 전송 - 
	INIT_MSG(MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO, T_FN_CITYWAR_CHANGE_OCCUPY_INFO, pSMsg, SendBuf);
	pSMsg->ChannelIndex = pFMChann->GetMapChannelIndex().ChannelIndex;
	pSMsg->OccupyGuildUID4 = i_pCityWar->m_OccupyCITY_WAR_DATAWhenWarStarted.GuildUID1;
	pFMChann->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO));
}

void CFieldIOCP::SetCityWarTeamTypeByMapIndex(CCityWar *i_pCityWar, CFieldMapProject *i_pFMPro)
{
	CFieldIOCPSocket *pSock = NULL;
	for (int i = CLIENT_INDEX_START_NUM; i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock->IsUsing()
			&& pSock->m_PeerSocketType == ST_CLIENT_TYPE
			&& pSock->GetCharacter()->MapChannelIndex.MapIndex == i_pCityWar->MapIndex)
		{
			pSock->m_byCityWarTeamType = i_pFMPro->GetCityWarTeamTypeByGuildUID(pSock->GetCharacter()->GuildUniqueNumber);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::GetP2PPKFamePoint(const char *szAttackCharName, const char *szDeadCharName)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-29 ~ 2005-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::GetP2PPKFamePoint(const char *szAttackCharName, const char *szDeadCharName)
{
	char	szTemp[SIZE_MAX_CHARACTER_NAME * 2 + 1];
	util::zero(szTemp, SIZE_MAX_CHARACTER_NAME * 2 + 1);
	sprintf(szTemp, "%s_%s", szAttackCharName, szDeadCharName);

	mt_auto_lock mtA(&m_mtmapUsedFameDataList);
	int *pnUsedFamePoint = m_mtmapUsedFameDataList.findNoLock_Ptr(szTemp);
	if (pnUsedFamePoint)
	{
		if (MAXIMUM_USABLE_FAME_POINT_FOR_ONE_DAY <= (*pnUsedFamePoint))
		{
			return FALSE;
		}
		(*pnUsedFamePoint)++;
		return TRUE;
	}

	m_mtmapUsedFameDataList.insertNoLock(szTemp, 1);
	return TRUE;
}

INT CFieldIOCP::CountP2PPKFamePoint(const char *szAttackCharName, const char *szDeadCharName)
{
	char	szTemp[SIZE_MAX_CHARACTER_NAME * 2 + 1];
	util::zero(szTemp, SIZE_MAX_CHARACTER_NAME * 2 + 1);
	sprintf(szTemp, "%s_%s", szAttackCharName, szDeadCharName);

	mt_auto_lock mtA(&m_mtmapUsedFameDataList);
	int *pnUsedFamePoint = m_mtmapUsedFameDataList.findNoLock_Ptr(szTemp);
	if (pnUsedFamePoint)
	{
		return *pnUsedFamePoint;
	}

	return 0;
}

void CFieldIOCP::ResetAllP2PPKFamePoint(void)
{
	mt_auto_lock mtA(&m_mtmapUsedFameDataList);

	mtmapUsedFameData::iterator		itr(m_mtmapUsedFameDataList.begin());
	for (; itr != m_mtmapUsedFameDataList.end(); itr++)
	{
		itr->second = 0;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::AddInfluenceTypeCount(BYTE i_byInflType)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-08 ~ 2006-02-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::AddInfluenceTypeCount(BYTE i_byInflType)
{
	switch (i_byInflType)
	{
	case INFLUENCE_TYPE_NORMAL:		m_nNormalInfluenceCount++;		break;
	case INFLUENCE_TYPE_VCN:		m_nVCNInfluenceCount++;			break;
	case INFLUENCE_TYPE_ANI:		m_nANIInfluenceCount++;			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::GetInfluenceTypeDistirbutionPercent(BYTE i_byInflType)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-08 ~ 2006-02-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::GetInfluenceTypeDistirbutionPercent(BYTE i_byInflType)
{
	//////////////////////////////////////////////////////////////////////////
	// 2008-03-10 by dhjin, 아레나 통합 - 아레나 서버에서는 세금 적용을 하지 않는다.
	if (g_pFieldGlobal->IsArenaServer())
	{
		return 0;
	}

	int nSelectedCount = m_nVCNInfluenceCount + m_nANIInfluenceCount;
	if (SIZE_MIN_SELECTED_INFLUENCE_TYPE_USER_COUNT_FOR_INFLUENCE_DISTRIBUTION > nSelectedCount)
	{// 2006-02-08 by cmkwon, 세력 선택 유저가 SIZE_MIN_SELECTED_INFLUENCE_TYPE_USER_COUNT_FOR_INFLUENCE_DISTRIBUTION명 보다 작으면 세력전 분포 처리 하지 않는다

		if (10 > nSelectedCount)
		{// 2006-02-24 by cmkwon, 마상 내부 테스트 서버가 아니거나 10명보다 적을 때는 0이 리턴된다.
			return 0;
		}
	}

	int nVCNInflPercent = (int)((100 * m_nVCNInfluenceCount) / nSelectedCount);		// 소숫점은 삭제한다.
	switch (i_byInflType)
	{
	case INFLUENCE_TYPE_VCN:
	{
		return nVCNInflPercent;
	}
	break;
	case INFLUENCE_TYPE_ANI:
	{
		return 100 - nVCNInflPercent;
	}
	break;
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			float CFieldIOCP::GetInfluenceTypeDistirbutionTexPercent(BYTE i_byInflType)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-08 ~ 2006-02-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CFieldIOCP::GetInfluenceTypeDistirbutionTexPercent(BYTE i_byInflType)
{
	return max(0, this->GetInfluenceTypeDistirbutionPercent(i_byInflType) - 50);
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::ChangeMapInfluenceType2AllConflictAreaW(BYTE i_byDeadMonsterBelligerence, BOOL i_bAdd)	// 2009-01-12 by dhjin, 선전 포고 - 세력 추가 비트
/// \brief		
/// \author		cmkwon
/// \date		2006-02-09 ~ 2006-02-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::ChangeMapInfluenceType2AllConflictAreaW(BYTE i_byDeadMonsterBelligerence, BOOL i_bAdd)	// 2009-01-12 by dhjin, 선전 포고 - 세력 추가 비트
{
	SHORT sMapInfluenceTy = 0;
	switch (i_byDeadMonsterBelligerence)
	{
	case BELL_INFLUENCE_VCN:		sMapInfluenceTy = MAP_INFLUENCE_ANI_START + 1;		break;
	case BELL_INFLUENCE_ANI:		sMapInfluenceTy = MAP_INFLUENCE_VCN_START + 1;		break;
	default:
	{
		return 0;
	}
	}

	return m_pFieldMapWorkspace->ChangeMapInfluenceType2AllConflictArea(sMapInfluenceTy, i_bAdd);	// 2009-01-12 by dhjin, 선전 포고 - 세력 추가 비트
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM * CFieldIOCP::GetKillMarkItemPtr(BYTE i_byCharInflTy, int i_byCharLevel)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-09 ~ 2006-02-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM * CFieldIOCP::GetKillMarkItemPtr(BYTE i_byCharInflTy, int i_byCharLevel)
{
	if (COMPARE_INFLUENCE(i_byCharInflTy, INFLUENCE_TYPE_NORMAL))
	{// 2006-02-09 by cmkwon, 일반군은 킬마크가 없다
		return NULL;
	}

	for (int i = 0; i < m_mtvectItemPtrKillMarks.size(); i++)
	{
		ITEM *pITEM = m_mtvectItemPtrKillMarks[i];
		if (COMPARE_INFLUENCE(pITEM->ReqRace, i_byCharInflTy)
			&& util::in_range(pITEM->ReqMinLevel, i_byCharLevel, pITEM->ReqMaxLevel))
		{
			return pITEM;
		}
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::InsertCharacterDropItem(BYTE i_byInflMask, USHORT i_UnitKindMask, CHARACTER2ITEM i_Character2Item)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-02 ~ 2006-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::InsertCharacterDropItem(BYTE i_byInflMask, USHORT i_UnitKindMask, CHARACTER2ITEM i_Character2Item)
{
	if (COMPARE_INFLUENCE(i_byInflMask, INFLUENCE_TYPE_NORMAL))
	{
		if (IS_BGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_NORMAL, UNITKIND_BGEAR_MASK, i_Character2Item);
		}
		if (IS_MGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_NORMAL, UNITKIND_MGEAR_MASK, i_Character2Item);
		}
		if (IS_AGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_NORMAL, UNITKIND_AGEAR_MASK, i_Character2Item);
		}
		if (IS_IGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_NORMAL, UNITKIND_IGEAR_MASK, i_Character2Item);
		}
	}
	if (COMPARE_INFLUENCE(i_byInflMask, INFLUENCE_TYPE_VCN))
	{
		if (IS_BGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_VCN, UNITKIND_BGEAR_MASK, i_Character2Item);
		}
		if (IS_MGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_VCN, UNITKIND_MGEAR_MASK, i_Character2Item);
		}
		if (IS_AGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_VCN, UNITKIND_AGEAR_MASK, i_Character2Item);
		}
		if (IS_IGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_VCN, UNITKIND_IGEAR_MASK, i_Character2Item);
		}
	}
	if (COMPARE_INFLUENCE(i_byInflMask, INFLUENCE_TYPE_ANI))
	{
		if (IS_BGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_ANI, UNITKIND_BGEAR_MASK, i_Character2Item);
		}
		if (IS_MGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_ANI, UNITKIND_MGEAR_MASK, i_Character2Item);
		}
		if (IS_AGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_ANI, UNITKIND_AGEAR_MASK, i_Character2Item);
		}
		if (IS_IGEAR(i_UnitKindMask))
		{
			this->InsertCharacter2Item(INFLUENCE_TYPE_ANI, UNITKIND_IGEAR_MASK, i_Character2Item);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::InsertCharacter2Item(BYTE i_byInflty, USHORT i_UnitKindMask, CHARACTER2ITEM i_Character2Item)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-02 ~ 2006-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::InsertCharacter2Item(BYTE i_byInflty, USHORT i_UnitKindMask, CHARACTER2ITEM i_Character2Item)
{
	CHARACTER2ITEMLIST *pChar2ItemList = this->FindCharacter2Item(i_byInflty, i_UnitKindMask);
	if (pChar2ItemList)
	{
		pChar2ItemList->vectCharacter2Item.push_back(i_Character2Item);
		return;
	}
	CHARACTER2ITEMLIST tmChar2ItemList;
	tmChar2ItemList.InfluenceType0 = i_byInflty;
	tmChar2ItemList.UnitKindMask0 = i_UnitKindMask;
	tmChar2ItemList.vectCharacter2Item.push_back(i_Character2Item);
	m_vectCharacter2ItemList.push_back(tmChar2ItemList);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CHARACTER2ITEMLIST * CFieldIOCP::FindCharacter2Item(BYTE i_byInflty, USHORT i_UnitKindMask)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-02 ~ 2006-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CHARACTER2ITEMLIST * CFieldIOCP::FindCharacter2Item(BYTE i_byInflty, USHORT i_UnitKindMask)
{
	for (int i = 0; i < m_vectCharacter2ItemList.size(); i++)
	{
		if (i_byInflty == m_vectCharacter2ItemList[i].InfluenceType0
			&& IS_SAME_UNITKIND(i_UnitKindMask, m_vectCharacter2ItemList[i].UnitKindMask0))
		{
			return &m_vectCharacter2ItemList[i];
		}
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::SignBoardInsertString(MSG_SIGN_BOARD_STRING *i_pSignBoardString)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-17 ~ 2006-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::SignBoardInsertString(MSG_SIGN_BOARD_STRING *i_pSignBoardString)
{
	mt_auto_lock mtA(&m_mtvectSignBoardStringList);
	if (100 <= m_mtvectSignBoardStringList.size())
	{
		return FALSE;
	}
	i_pSignBoardString->StringIndex = m_nSignBoardStringIndex;
	m_nSignBoardStringIndex++;
	m_mtvectSignBoardStringList.push_back(*i_pSignBoardString);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::SignBoardDeleteString(int i_nStringIdx, BOOL i_bInflLeader)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-18 ~ 2006-04-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::SignBoardDeleteString(int i_nStringIdx, BOOL i_bInflLeader)
{
	mt_auto_lock mtA(&m_mtvectSignBoardStringList);
	for (int i = 0; i < m_mtvectSignBoardStringList.size(); i++)
	{
		if (i_nStringIdx == m_mtvectSignBoardStringList[i].StringIndex
			&& i_bInflLeader == m_mtvectSignBoardStringList[i].IsInfluenceLeader)
		{
			m_mtvectSignBoardStringList.erase(m_mtvectSignBoardStringList.begin() + i);
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendSignBoardStringList(CFieldIOCPSocket *i_pFISocket, int i_nReqStringIndex/*=-1*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-17 ~ 2006-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendSignBoardStringList(CFieldIOCPSocket *i_pFISocket, int i_nReqStringIndex/*=-1*/)
{
	if (m_mtvectSignBoardStringList.empty()) return;

	mt_auto_lock mtA(&m_mtvectSignBoardStringList);

	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK, T_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK, pSStringList, SendBuf);
	
	// unnecessary because INIT_MSG_WITH_BUFFER memsets to 0x00
	// pSStringList->nStringCount = 0;

	auto nSendBytes = MSG_SIZE(MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK);
	auto pInsertString = (MSG_FC_WAR_SIGN_BOARD_INSERT_STRING*)(SendBuf + nSendBytes);

	for (auto i = 0; i < m_mtvectSignBoardStringList.size(); i++)
	{
		if (nSendBytes + sizeof(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING) >= SIZE_MAX_PACKET)
		{
			i_pFISocket->SendAddData(SendBuf, nSendBytes);

			pSStringList = _init_msg<MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK, T_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK>(SendBuf);

			nSendBytes = MSG_SIZE(MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK);
			pInsertString = (MSG_FC_WAR_SIGN_BOARD_INSERT_STRING*)(SendBuf + nSendBytes);
		}
		if (COMPARE_INFLUENCE(m_mtvectSignBoardStringList[i].InfluenceMask0, i_pFISocket->GetCharacter()->InfluenceType)
			&& i_nReqStringIndex < m_mtvectSignBoardStringList[i].StringIndex)
		{
			pInsertString->IsInfluenceLeader = m_mtvectSignBoardStringList[i].IsInfluenceLeader;
			pInsertString->StringIndex = m_mtvectSignBoardStringList[i].StringIndex;
			pInsertString->SignBoardExpiryTime = m_mtvectSignBoardStringList[i].SignBoardExprieATime;
			util::strncpy(pInsertString->SingBoardString, m_mtvectSignBoardStringList[i].SingBoardString, SIZE_MAX_SIGN_BOARD_STRING);
			pSStringList->nStringCount++;
			nSendBytes += sizeof(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING);
			pInsertString = (MSG_FC_WAR_SIGN_BOARD_INSERT_STRING*)(SendBuf + nSendBytes);
		}
	}
	if (pSStringList->nStringCount > 0)
	{
		i_pFISocket->SendAddData(SendBuf, nSendBytes);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SignBoardCheckExpireTime(ATUM_DATE_TIME *i_pCurATime)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-17 ~ 2006-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SignBoardCheckExpireTime(ATUM_DATE_TIME *i_pCurATime)
{
	mt_auto_lock mtA(&m_mtvectSignBoardStringList);
	mtvectMSG_SIGN_BOARD_STRINGList::iterator itr(m_mtvectSignBoardStringList.begin());
	while (itr != m_mtvectSignBoardStringList.end())
	{
		if (*i_pCurATime >= itr->SignBoardExprieATime)
		{
			itr = m_mtvectSignBoardStringList.erase(itr);
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::Send2LeaderANDSubLeader(BYTE i_byInflTy, BYTE *i_pData, int i_nDataLen, CFieldMapChannel *i_pExcludeFMChann)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::Send2LeaderANDSubLeader(BYTE i_byInflTy, BYTE *i_pData, int i_nDataLen, CFieldMapChannel *i_pExcludeFMChann)
{
	CFieldIOCPSocket *pLFISocket = NULL;
	CFieldIOCPSocket *pSubL1FISocket = NULL;
	CFieldIOCPSocket *pSubL2FISocket = NULL;

	this->m_InflWarManager.GetAllLeaderFISocket(i_byInflTy, &pLFISocket, &pSubL1FISocket, &pSubL2FISocket);

	if (pLFISocket
		&& i_pExcludeFMChann != pLFISocket->GetCurrentFieldMapChannel())
	{
		pLFISocket->SendAddData(i_pData, i_nDataLen);
	}
	if (pSubL1FISocket
		&& i_pExcludeFMChann != pSubL1FISocket->GetCurrentFieldMapChannel())
	{
		pSubL1FISocket->SendAddData(i_pData, i_nDataLen);
	}
	if (pSubL2FISocket
		&& i_pExcludeFMChann != pSubL2FISocket->GetCurrentFieldMapChannel())
	{
		pSubL2FISocket->SendAddData(i_pData, i_nDataLen);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::AddCallWarpEventRequest(SCALL_WARP_EVENT *i_pCallWarpEvent)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-21 ~ 2006-07-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::AddCallWarpEventRequest(SCALL_WARP_EVENT *i_pCallWarpEvent)
{
	return m_callWarpEventManager.AddCallWarpEvent(i_pCallWarpEvent);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CFieldIOCP::CheckCallWarpEventRequestAck(SCALL_WARP_EVENT *o_pCallWarpEvent, DWORD i_dwCallWarpID, BOOL i_bAddUserCount/*=FALSE*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-21 ~ 2006-07-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CFieldIOCP::CheckCallWarpEventRequestAck(SCALL_WARP_EVENT *o_pCallWarpEvent, DWORD i_dwCallWarpID, BOOL i_bAddUserCount/*=FALSE*/)
{
	return m_callWarpEventManager.CheckCallWarpEvent(o_pCallWarpEvent, i_dwCallWarpID, i_bAddUserCount);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendGameEventMessage(CFieldIOCPSocket *pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy/*=T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK*/)
/// \brief		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - CFieldIOCP::SendGameEventMessage() 인자 추가
/// \author		cmkwon
/// \date		2006-08-22 ~ 2006-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendGameEventMessage(CFieldIOCPSocket *pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy/*=T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK*/)
{
	if (NULL == pToFISock
		|| FALSE == pToFISock->IsValidCharacter(FALSE))
	{
		return;
	}

	// 2013-05-22 by jhseol, 소스정리 - 주석삭제

	///////////////////////////////////////////////////////////////////////////////
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	// 2008-05-21 by cmkwon, 1. GAME_EVENT_GROUP_HAPPYHOUR 정보 전송
	m_pHappyHourEventManager->SendHappyHourEventInfo(pToFISock, pToFISock->GetCharacter()->InfluenceType, i_msgTy);

	// 2008-05-21 by cmkwon, 2. GAME_EVENT_GROUP_MOTHERSHIP, GAME_EVENT_GROUP_ITEM 정보 전송
	m_gameEventManager.SenGameEventGroupMotherShip8ItemInfo(pToFISock, pToFISock->GetCharacter()->InfluenceType);
}

//////////////////////////////////////////////////////////////////////
// 2012-10-05 by jhseol, NPlay PC방 모듈 - 해피아워 정보 가져오기
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::LoadGameEventMessage(CFieldIOCPSocket *pToFISock, BYTE i_byInflTy, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* msg)
/// \brief		해피아워 정보 가져오기
/// \author		jhseol
/// \date		2012-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::LoadGameEventMessage(CFieldIOCPSocket *pToFISock, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* msg)
{
	if (NULL == pToFISock
		|| FALSE == pToFISock->IsValidCharacter(FALSE))
	{
		return;
	}

	m_pHappyHourEventManager->LoadHappyHourEventInfo(pToFISock, pToFISock->GetCharacter()->InfluenceType, msg);
}
// end 2012-10-05 by jhseol, NPlay PC방 모듈 - 해피아워 정보 가져오기

///////////////////////////////////////////////////////////////////////////////
/// \fn			mtmapAllGuildItem * CFieldIOCP::GetmtmapAllGuildItemPtrW(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mtmapAllGuildItem * CFieldIOCP::GetmtmapAllGuildItemPtrW(void)
{
	return m_pAllGuildItemManager->GetmtmapAllGuildItemPtr();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CGuildItemManager *CFieldIOCP::GetGuildItemMangerNoLockW(UID32_t i_GuildUID)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CGuildItemManager *CFieldIOCP::GetGuildItemMangerNoLockW(UID32_t i_GuildUID)
{
	return m_pAllGuildItemManager->GetGuildItemMangerNoLock(i_GuildUID);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::InsertGuildItemManagerNoLockW(CGuildItemManager *i_pGuildManager)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::InsertGuildItemManagerNoLockW(CGuildItemManager *i_pGuildManager)
{
	return m_pAllGuildItemManager->InsertGuildItemManagerNoLock(i_pGuildManager);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::InitStrategyPointOccupyW(BOOL i_bVCNMapInflTyForInit)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-21 ~ 2006-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::InitStrategyPointOccupyW(BOOL i_bVCNMapInflTyForInit)
{
	m_pFieldMapWorkspace->InitStrategyPointOccupy(i_bVCNMapInflTyForInit);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int* CFieldIOCP::GetUserLevelList(int i_UserLevelList[10])
/// \brief		/레벨분포 명령어 시 접속한 유저 레벨 분포 상태 값 얻기.
/// \author		dhjin
/// \date		2006-08-09 ~ 2006-08-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int* CFieldIOCP::GetUserLevelList(int i_UserLevelList[10])
{
	CFieldIOCPSocket *pSock = NULL;
	int nCharLevel = 0;

	memset(i_UserLevelList, 0, sizeof(i_UserLevelList) * 10);

	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = GetFieldIOCPSocket(i);
		if (pSock->IsValidCharacter(FALSE))
		{
			nCharLevel = pSock->GetCharacter()->Level;
			i_UserLevelList[min(9, (nCharLevel - 1) / 10)]++;
		}
	}

	return i_UserLevelList;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::GetPCBangUserCount()
/// \brief		
/// \author		dhjin
/// \date		2007-01-25 ~ 2007-01-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::GetPCBangUserCount()
{
	CFieldIOCPSocket *pSock = NULL;
	int nPCBangUserCount = 0;

	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = GetFieldIOCPSocket(i);
		if (pSock->IsValidCharacter(FALSE)) nPCBangUserCount++;
	}

	return nPCBangUserCount;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendWarPointToInflClient(BYTE i_byInflTy, int i_nWarPoint, BYTE i_byWPUpdateTy/*=WPUT_MOTHERSHIPWAR*/)
/// \brief		WarPoint를 세력전 승리 세력에게 지급한다.
///				// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldIOCP::SendWarPointToInflClient()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가
/// \author		dhjin
/// \date		2007-05-15 ~ 2007-05-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendWarPointToInflClient(BYTE i_byInflTy, int i_nWarPoint, BYTE i_byWPUpdateTy/*=WPUT_MOTHERSHIPWAR*/)
{
	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			&& i_byInflTy == pSock->m_character.InfluenceType
			&& !IS_MAP_INFLUENCE_ARENA(pSock->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW()))
		{
			// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldIOCPSocket::WPAddWarPoint()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가
			pSock->WPAddWarPoint(i_nWarPoint, FALSE, i_byWPUpdateTy);
			// 2007-08-13 by cmkwon, 위와 같이 WPAddWarPoint()함수로 변경함
			//			pSock->m_character.WarPoint += i_nWarPoint;
			//			pSock->m_character.CumulativeWarPoint += i_nWarPoint;
			//			
			//			CAtumLogSender::SendLogMessageGetWarPoint(pSock, i_nWarPoint, pSock->m_character.WarPoint, pSock->m_character.CumulativeWarPoint);
			//			// 2007-05-16 by dhjin, DB에 업데이트
			//			QPARAM_UPDATE_WARPOINT *pQparam = new QPARAM_UPDATE_WARPOINT;
			//			pQparam->CharacterUID		= pSock->m_character.CharacterUniqueNumber;
			//			pQparam->AccountUID			= pSock->m_character.AccountUniqueNumber;
			//			pQparam->TotalWarPoint		= pSock->m_character.WarPoint;
			//			pQparam->CumulativeWarPoint	= pSock->m_character.CumulativeWarPoint;
			//			m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateWarPoint, pSock, pSock->m_character.AccountUniqueNumber, pQparam);
			//
			//			// 2007-05-16 by dhjin, 클라이언트에게 전송
			//			INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_PAY_WARPOINT, T_FC_CHARACTER_PAY_WARPOINT, pMsg, SendBuf);
			//			pMsg->WarPoint				= i_nWarPoint;
			//			pMsg->TotalWarPoint			= pSock->m_character.WarPoint;
			//			pMsg->CumulativeWarPoint	= pSock->m_character.CumulativeWarPoint;
			//			pSock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_PAY_WARPOINT));
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendWarPointToInflClientByDGA(int i_nWarPoint)
/// \brief		Adds warpoints to every client
///
/// \author		killburne
/// \date		2013-12-25 ~ 2013-12-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendWarPointToInflClientByDGA(int i_nWarPoint, BYTE i_bInfl/*=INFLUENCE_TYPE_ALL_MASK*/)
{
	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			&& (pSock->GetCharacter()->InfluenceType == i_bInfl || i_bInfl == INFLUENCE_TYPE_ALL_MASK)
			&& !IS_MAP_INFLUENCE_ARENA(pSock->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW()))
		{
			// 2013-12-25 by killburne
			pSock->WPAddWarPoint(i_nWarPoint, FALSE, TRUE, WPUT_GENERAL, FALSE);

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::GetAllFileNameList(vectString *i_pVectFileNameList, const char *i_szDir)
/// \brief		// 2009-10-06 by cmkwon, 베트남 게임 가드 X-TRAP으로 변경 - CFieldIOCP::GetAllFileNameList# 수정, 인자 추가 (i_szPrefixFileName)
/// \author		cmkwon
/// \date		2007-05-28 ~ 2007-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::GetAllFileNameList(vectString *i_pVectFileNameList, const char *i_szDir, char *i_szPrefixFileName/*=NULL*/)
{
	// 2007-07-18 by cmkwon, omi.tex도 체크섬을 체크 루틴 추가 - 함수를 호출한 쪽에서 처리함
	//	i_pVectFileNameList->clear();

	char szFullPath[MAX_PATH];
	WIN32_FIND_DATA FileData = { 0 };

	_stprintf(szFullPath, "%s\\*.*", i_szDir);
	HANDLE hSearch = FindFirstFile(szFullPath, &FileData);
	if (hSearch == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	while (TRUE)
	{
		if (FALSE == (FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{//  파일 처리

			// 2009-10-06 by cmkwon, 베트남 게임 가드 X-TRAP으로 변경 - CFieldIOCP::GetAllFileNameList# 수정, 인자 추가 (i_szPrefixFileName)
			if (NULL == i_szPrefixFileName
				|| 0 == strnicmp(FileData.cFileName, i_szPrefixFileName, strlen(i_szPrefixFileName)))
			{
				i_pVectFileNameList->push_back(FileData.cFileName);	// 2007-05-28 by cmkwon, 벡터리스트에 추가
			}
		}

		if (FALSE == FindNextFile(hSearch, &FileData))
		{
			break;
		}
	}
	FindClose(hSearch);

	SetLastError(0);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::LoadResObjCheckList(BOOL i_bReloadOnlyRexTexDirectory/*=FALSE*/)
/// \brief		// 2008-09-08 by cmkwon, SCMonitor에서 ReloadVersionInfo시에 일부 체크섬파일(.\Res-Tex\*.*)도 리로드하기 - 
/// \author		cmkwon
/// \date		2007-05-28 ~ 2007-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//BOOL CFieldIOCP::LoadResObjCheckList(void)
BOOL CFieldIOCP::LoadResObjCheckList(BOOL i_bReloadOnlyRexTexDirectory/*=FALSE*/)
{
	if (FALSE == i_bReloadOnlyRexTexDirectory)
	{// 2008-09-08 by cmkwon, 리로드시에는 초기화 하면 안된다.
		m_mapResObjCheckSumList.clear();
	}

	vectString		vectFileNameList;
	char			szResDirectoryPath[MAX_PATH] = { NULL, };
	int				i = 0;
	int				nCnt = 0;


	if (FALSE == i_bReloadOnlyRexTexDirectory)
	{// 2008-09-08 by cmkwon, 리로드시에는 .\map\Res-Obj\*.* Files 체크섬 계산은 제외 한다.

		///////////////////////////////////////////////////////////////////////////////
		// 1. .\map\Res-Obj\*.* Files 체크섬 계산 
		// 2007-07-18 by cmkwon, omi.tex도 체크섬을 체크 루틴 추가
		util::strncpy(szResDirectoryPath, RESOBJ_DIRECTORY_PATH, MAX_PATH);
		if (FALSE == GetAllFileNameList(&vectFileNameList, szResDirectoryPath))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] LoadResObjCheckList_ error !!, Directory(%s)\r\n", szResDirectoryPath);
			return FALSE;
		}

		nCnt = vectFileNameList.size();
		for (i = 0; i < nCnt; i++)
		{
			SRESOBJ_CHECKSUM	resObjCheckSum;
			CGameData			tmMapGameData;
			char				szFileFullPath[MAX_PATH];
			util::zero(&resObjCheckSum, sizeof(resObjCheckSum));

			util::strncpy(resObjCheckSum.szResObjFileName, vectFileNameList[i].c_str(), SIZE_MAX_RESOBJ_FILE_NAME);
			_strupr(resObjCheckSum.szResObjFileName);	// 2008-11-27 by cmkwon, CheckSum 리스트 파일 이름을 대문자로 변경 처리 - 
			sprintf(szFileFullPath, "%s/%s", szResDirectoryPath, resObjCheckSum.szResObjFileName);

			// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
			//if(tmMapGameData.GetCheckSum(&resObjCheckSum.uiObjCheckSum, &resObjCheckSum.nFileSize, szFileFullPath))
			if (tmMapGameData.GetCheckSum(resObjCheckSum.byDigest, &resObjCheckSum.nFileSize, szFileFullPath))
			{
				m_mapResObjCheckSumList.insert(pair<string, SRESOBJ_CHECKSUM>(resObjCheckSum.szResObjFileName, resObjCheckSum));
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2. .\map\Res-Tex\*.* Files 체크섬 계산
	// 2007-07-18 by cmkwon, omi.tex도 체크섬을 체크 루틴 추가
	vectFileNameList.clear();
	util::strncpy(szResDirectoryPath, RESTEX_DIRECTORY_PATH, MAX_PATH);
	if (FALSE == GetAllFileNameList(&vectFileNameList, szResDirectoryPath))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] LoadResObjCheckList_ error !!, Directory(%s)\r\n", szResDirectoryPath);
		return FALSE;
	}

	nCnt = vectFileNameList.size();
	for (i = 0; i < nCnt; i++)
	{
		SRESOBJ_CHECKSUM	resObjCheckSum;
		CGameData			tmMapGameData;
		char				szFileFullPath[MAX_PATH];
		util::zero(&resObjCheckSum, sizeof(resObjCheckSum));

		util::strncpy(resObjCheckSum.szResObjFileName, vectFileNameList[i].c_str(), SIZE_MAX_RESOBJ_FILE_NAME);
		_strupr(resObjCheckSum.szResObjFileName);	// 2008-11-27 by cmkwon, CheckSum 리스트 파일 이름을 대문자로 변경 처리 - 
		sprintf(szFileFullPath, "%s/%s", szResDirectoryPath, resObjCheckSum.szResObjFileName);

		// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
		//if(tmMapGameData.GetCheckSum(&resObjCheckSum.uiObjCheckSum, &resObjCheckSum.nFileSize, szFileFullPath))
		if (tmMapGameData.GetCheckSum(resObjCheckSum.byDigest, &resObjCheckSum.nFileSize, szFileFullPath))
		{
			if (FALSE == i_bReloadOnlyRexTexDirectory)
			{// 2008-09-08 by cmkwon, 처음에는 무조건 추가하고
				m_mapResObjCheckSumList.insert(pair<string, SRESOBJ_CHECKSUM>(resObjCheckSum.szResObjFileName, resObjCheckSum));
			}
			else
			{// 2008-09-08 by cmkwon, 리로드 시에는 기존에 존재하던 파일만 업데이트 한다.(새로운 파일까지 업데이트하려면 동기화 문제를 해결 해야 한다)

				mapstring2SRESOBJ_CHECKSUM::iterator itr = m_mapResObjCheckSumList.find(resObjCheckSum.szResObjFileName);
				if (itr != m_mapResObjCheckSumList.end())
				{
					itr->second = resObjCheckSum;
				}
			}
		}
	}


	///////////////////////////////////////////////////////////////////////////////
	// 3. .\map\Res-EXE\*.* Files 체크섬 파일리스트 추가
	// 2008-09-17 by cmkwon, 클라이언트 실행파일도 체크섬 체크 추가 - 
	vectFileNameList.clear();
	util::strncpy(szResDirectoryPath, RESEXE_DIRECTORY_PATH, MAX_PATH);
	if (FALSE == GetAllFileNameList(&vectFileNameList, szResDirectoryPath))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] LoadResObjCheckList_ error !!, Directory(%s)\r\n", szResDirectoryPath);
		return FALSE;
	}

	nCnt = vectFileNameList.size();
	for (i = 0; i < nCnt; i++)
	{
		SRESOBJ_CHECKSUM	resObjCheckSum;
		CGameData			tmMapGameData;
		char				szFileFullPath[MAX_PATH];
		util::zero(&resObjCheckSum, sizeof(resObjCheckSum));

		util::strncpy(resObjCheckSum.szResObjFileName, vectFileNameList[i].c_str(), SIZE_MAX_RESOBJ_FILE_NAME);
		_strupr(resObjCheckSum.szResObjFileName);	// 2008-11-27 by cmkwon, CheckSum 리스트 파일 이름을 대문자로 변경 처리 - 
		sprintf(szFileFullPath, "%s/%s", szResDirectoryPath, resObjCheckSum.szResObjFileName);

		// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
		//if(tmMapGameData.GetCheckSum(&resObjCheckSum.uiObjCheckSum, &resObjCheckSum.nFileSize, szFileFullPath))
		if (tmMapGameData.GetCheckSum(resObjCheckSum.byDigest, &resObjCheckSum.nFileSize, szFileFullPath))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] %s is loaded for CheckSum\r\n", szFileFullPath);
			if (FALSE == i_bReloadOnlyRexTexDirectory)
			{// 2008-09-08 by cmkwon, 처음에는 무조건 추가하고
				m_mapResObjCheckSumList.insert(pair<string, SRESOBJ_CHECKSUM>(resObjCheckSum.szResObjFileName, resObjCheckSum));
			}
			else
			{// 2008-09-08 by cmkwon, 리로드 시에는 기존에 존재하던 파일만 업데이트 한다.(새로운 파일까지 업데이트하려면 동기화 문제를 해결 해야 한다)

				mapstring2SRESOBJ_CHECKSUM::iterator itr = m_mapResObjCheckSumList.find(resObjCheckSum.szResObjFileName);
				if (itr != m_mapResObjCheckSumList.end())
				{
					itr->second = resObjCheckSum;
				}
			}
		}
	}


	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] CFieldIOCP::LoadResObjCheckList_ success !! CheckSum File Count: %d\r\n", m_mapResObjCheckSumList.size());
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SRESOBJ_CHECKSUM * CFieldIOCP::GetResObjCheckSum(char *i_szFileName)
/// \brief		
/// \author		cmkwon
/// \date		2007-05-28 ~ 2007-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SRESOBJ_CHECKSUM * CFieldIOCP::GetResObjCheckSum(char *i_szFileName)
{
	// 2008-11-27 by cmkwon, CheckSum 리스트 파일 이름을 대문자로 변경 처리 - 
	char	szTmResObjFileName[SIZE_MAX_RESOBJ_FILE_NAME];
	util::strncpy(szTmResObjFileName, i_szFileName, SIZE_MAX_RESOBJ_FILE_NAME);
	_strupr(szTmResObjFileName);

	// 2008-11-27 by cmkwon, CheckSum 리스트 파일 이름을 대문자로 변경 처리 - 
	//mapstring2SRESOBJ_CHECKSUM::iterator itr = m_mapResObjCheckSumList.find(i_szFileName);
	mapstring2SRESOBJ_CHECKSUM::iterator itr = m_mapResObjCheckSumList.find(szTmResObjFileName);
	if (itr == m_mapResObjCheckSumList.end())
	{
		return NULL;
	}

	return &(itr->second);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			STUTORIAL_INFO *CFieldIOCP::GetTutorialInfo(INT i_nTutorialUID)
/// \brief		
/// \author		cmkwon
/// \date		2007-07-23 ~ 2007-07-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
STUTORIAL_INFO *CFieldIOCP::GetTutorialInfo(INT i_nTutorialUID)
{
	vectSTutorialInfo::iterator itr(this->m_vectTutorial.begin());
	for (; itr != this->m_vectTutorial.end(); itr++)
	{
		if (i_nTutorialUID == itr->TutorialUID)
		{
			return &*itr;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SetUnusableSpeakerItem(BOOL i_bFlag)
/// \brief		// 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가 - 함수 추가
/// \author		cmkwon
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SetUnusableSpeakerItem(BOOL i_bFlag)
{
	m_bUnusableSpeakerItem = i_bFlag;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldIOCP::GetUnusableSpeakerItem(void)
/// \brief		// 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가 - 함수 추가
/// \author		cmkwon
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::GetUnusableSpeakerItem(void)
{
	return m_bUnusableSpeakerItem;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SetPrepareShutdown(BOOL i_bPrepareShutdown)
/// \brief		// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게)
/// \author		cmkwon
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SetPrepareShutdown(BOOL i_bPrepareShutdown)
{
	server::log(true, "[Notify] CFieldIOCP::SetPrepareShutdown(%d)\r\n", i_bPrepareShutdown);

	if (S_DISABLE_PREPARE_SHUTDOWN) return;

	m_bPrepareShutDown = i_bPrepareShutdown;
	if (FALSE == i_bPrepareShutdown)
	{// 서버다운준비 해제처리시에는 처리 할 것이 없음
		return;
	}

	// 2007-08-27 by cmkwon, 필요 없다
	//this->SetUnusableSpeakerItem(TRUE);		// 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가 - 사용 불가로 설정한다

	MessageType_t msgTy = T_FC_CONNECT_NOTIFY_SERVER_SHUTDOWN;
	this->SendMessageToAllClients((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::OnOutPostProtectorDestroyW(int i_nMapIndex)
/// \brief		전진기지 보호막 파괴시 처리 
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::OnOutPostProtectorDestroyW(int i_nMapIndex)
{
	m_OutPostManager.OnOutPostProtectorDestroy(i_nMapIndex);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::OnOutPostResetDestroyW(int i_nMapIndex)
/// \brief		전진기지 중앙처리장치 파괴시 처리
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::OnOutPostResetDestroyW(int i_nMapIndex)
{
	m_OutPostManager.OnOutPostResetDestroy(i_nMapIndex);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CFieldIOCP::GetPollPointActionByLevel(BYTE i_nLevel)
/// \brief		레벨에 따른 pollpoint 얻어오기
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CFieldIOCP::GetPollPointActionByLevel(BYTE i_nLevel)
{
	vectSACTION_BY_LEVEL_DB::iterator itr = m_vectSACTION_BY_LEVEL_DB.begin();
	for (; itr != m_vectSACTION_BY_LEVEL_DB.end(); itr++)
	{
		if (i_nLevel == itr->Level)
		{
			return itr->PollPoint;
		}
	}

	return 0;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::PayBossMonsterSuccessDefend(BYTE i_byInfluence)
/// \brief		모선전 방어 성공 시 보상 지급 변경으로 남아 있는 전략포인트 수 만큼 보상 일괄 지급
/// \author		dhjin
/// \date		2007-11-07 ~ 2007-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PayBossMonsterSuccessDefend(BYTE i_byInfluence)
{
	// 2013-05-09 by hskim, 세력 포인트 개선
#ifdef S_UPGRADE_INFLUENCE_POINT_HSKIM
	m_InflWarManager.CalcContributionPoint(i_byInfluence, CONTRIBUTIONPOINT_BOSS_MONSTER_WINNER, CONTRIBUTIONPOINT_BOSS_MONSTER_LOSER);		// 2013-05-09 by hskim, 세력 포인트 개선
	m_InflWarManager.CalcConsecutiveVictoriesPoint(i_byInfluence);		// 2013-05-09 by hskim, 세력 포인트 개선
#else
	//////////////////////////////////////////////////////////////////////////
	// 2007-11-07 by dhjin, 세력포인트 증가 
	m_InflWarManager.AddContributionPoint(i_byInfluence, ADD_CONTRIBUTIONPOINT_STRATEGYPOINT_MONSTER);
#endif
	// end 2013-05-09 by hskim, 세력 포인트 개선

		//////////////////////////////////////////////////////////////////////////
		// 2007-11-07 by dhjin, WarPoint 지급
	INT tmStrateyPointSummoningCount = m_InflWarManager.GetBossWarStrateyPointSummoningCountByInfl(i_byInfluence);
	SendWarPointToInflClient(i_byInfluence, ADD_WARPOINT_STRATEGYPOINT_MONSTER * tmStrateyPointSummoningCount);

	//////////////////////////////////////////////////////////////////////////
	// 2007-11-07 by dhjin, 전략포인트 드랍 아이템 지급
	INT SrategyPointMonsterNum = 0;
	if (IS_VCN_INFLUENCE_TYPE(i_byInfluence))
	{
		SrategyPointMonsterNum = STRATEGYPOINT_MONSTER_NUM_VCN;
	}
	else if (IS_ANI_INFLUENCE_TYPE(i_byInfluence))
	{
		SrategyPointMonsterNum = STRATEGYPOINT_MONSTER_NUM_ANI;
	}
	vectMONSTER2ITEMPtr		vectDropItemList;
	GetDropItemList(&vectDropItemList, SrategyPointMonsterNum);
	for (int i = 0; i < vectDropItemList.size(); i++)
	{
		InsertItemAllCharacter(vectDropItemList[i]->ItemNumber, vectDropItemList[i]->MinCount * tmStrateyPointSummoningCount, i_byInfluence, TRUE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::GetRandomRareCode(ITEM *i_pTargetItemInfo, BOOL i_bIsPrefix, int i_nReqUseTypeMask, CFieldIOCPSocket *i_pFISoc/*=NULL*/, RARE_ITEM_INFO *i_pLastRareItemInfo/*=NULL*/)
/// \brief		// 2007-12-07 by cmkwon, 레어 시스템 수정 - CFieldIOCP::GetRandomRareCode() 추가
///				// 2008-10-21 by cmkwon, 마지막에 선택된 레어는 다시 나오지 않게 수정 - 인자추가(, RARE_ITEM_INFO *i_pLastRareItemInfo/*=NULL*/)
/// \author		cmkwon
/// \date		2007-12-07 ~ 2007-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::GetRandomRareCode(ITEM *i_pTargetItemInfo, BOOL i_bIsPrefix, int i_nReqUseTypeMask, CFieldIOCPSocket *i_pFISoc/*=NULL*/, RARE_ITEM_INFO *i_pLastRareItemInfo/*=NULL*/)
{
	vector<RARE_ITEM_INFO* > *pRareFixVector = this->m_mapLevel2RareItemInfo.findEZ_ptr(min(CHARACTER_MAX_LEVEL, i_pTargetItemInfo->ReqMinLevel));
	if (NULL == pRareFixVector)
	{
		return 0;
	}

	BOOL bPrefix = FALSE;
	BOOL bSuffix = FALSE;
	if (i_bIsPrefix)
	{
		bPrefix = TRUE;
		bSuffix = FALSE;
	}
	else
	{
		bPrefix = FALSE;
		bSuffix = TRUE;
	}

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-12-07 by cmkwon, 해당 아이템에 붙을 수 있는 레어리스트 확률의 총합을 구한다.
	int			nRareTotalCnts = 0;
	Prob100K_t	nTotalSumProb = 0;
	int i;
	for (i = 0; i < pRareFixVector->size(); i++)
	{
		RARE_ITEM_INFO *pRareItemInfo = (*pRareFixVector)[i];
		if (0 >= pRareItemInfo->Probability
			|| (bPrefix && FALSE == IS_RARE_PREFIX(pRareItemInfo->CodeNum))
			|| (bSuffix && FALSE == IS_RARE_SUFFIX(pRareItemInfo->CodeNum))
			|| FALSE == COMPARE_ITEMKIND(pRareItemInfo->ReqItemKind, i_pTargetItemInfo->Kind)
			|| FALSE == COMPARE_BIT_FLAG(pRareItemInfo->ReqUseType, i_nReqUseTypeMask)
			|| (i_pLastRareItemInfo && 0 == strncmp(pRareItemInfo->Name, i_pLastRareItemInfo->Name, SIZE_MAX_RARE_FIX_NAME))	// 2008-10-21 by cmkwon, 마지막에 선택된 레어는 다시 나오지 않게 수정 - 체크 추가, 이전에 선택된 레어와 같은 이른인지 체크
			// 중복레어 체크 구문에 문제있음. 번역 누락으로 인챈트명이 공백일 경우에 모두 중복으로 체크되는 문제가 발생함. 2010. 10. 01. by hsLee.
			)
		{
			continue;
		}

		nTotalSumProb += pRareItemInfo->Probability;
		nRareTotalCnts++;
	}

	///////////////////////////////////////////////////////////////////////////////		
	// 확률의 총합으로에 따라 RareFix를 결정
	int			nRareCurCnts = 0;
	// 2007-12-10 by cmkwon, 랜덤 시스템 수정 - 
	//Prob100K_t	prob			= RANDI(0, nTotalSumProb-1);
	Prob100K_t	prob = this->GetRandInt32(1, nTotalSumProb);	// 2007-12-10 by cmkwon, 
	Prob100K_t	nCurSumProb = 0;
	for (i = 0; i < pRareFixVector->size(); i++)
	{
		RARE_ITEM_INFO *pRareItemInfo = (*pRareFixVector)[i];

		if (0 >= pRareItemInfo->Probability
			|| (bPrefix && FALSE == IS_RARE_PREFIX(pRareItemInfo->CodeNum))
			|| (bSuffix && FALSE == IS_RARE_SUFFIX(pRareItemInfo->CodeNum))
			|| FALSE == COMPARE_ITEMKIND(pRareItemInfo->ReqItemKind, i_pTargetItemInfo->Kind)
			|| FALSE == COMPARE_BIT_FLAG(pRareItemInfo->ReqUseType, i_nReqUseTypeMask)
			|| (i_pLastRareItemInfo && 0 == strncmp(pRareItemInfo->Name, i_pLastRareItemInfo->Name, SIZE_MAX_RARE_FIX_NAME))	// 2008-10-21 by cmkwon, 마지막에 선택된 레어는 다시 나오지 않게 수정 - 체크 추가, 이전에 선택된 레어와 같은 이른인지 체크
			)
		{
			continue;
		}

		if (util::in_range(nCurSumProb, prob, nCurSumProb + pRareItemInfo->Probability))
		{// 2007-12-07 by cmkwon, 레어 선택됨

			if (i_pFISoc
				&& i_pFISoc->IsValidCharacter(FALSE))
			{
				// 2007-12-07 by cmkwon, 디버깅을 위해
				char szLog[1024];
				if (i_bIsPrefix)
				{
					sprintf(szLog, "Prefix %4d:\'%20s\' Success: %4d < %4d <= %4d, TotalProb(%5d) TotalRareCount(%d) CurCount(%d)",
						pRareItemInfo->CodeNum, pRareItemInfo->Name, nCurSumProb, prob, nCurSumProb + pRareItemInfo->Probability,
						nTotalSumProb, nRareTotalCnts, nRareCurCnts);
				}
				else
				{
					sprintf(szLog, "Suffix %4d:\'%20s\' Success: %4d < %4d <= %4d, TotalProb(%5d) TotalRareCount(%d) CurCount(%d)",
						pRareItemInfo->CodeNum, pRareItemInfo->Name, nCurSumProb, prob, nCurSumProb + pRareItemInfo->Probability,
						nTotalSumProb, nRareTotalCnts, nRareCurCnts);
				}
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "%s\r\n", szLog);
				i_pFISoc->SendString128(STRING_128_ADMIN_CMD, szLog);
			}

			return pRareItemInfo->CodeNum;
		}

		nCurSumProb += pRareItemInfo->Probability;		// 더함
		nRareCurCnts++;
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::GetRandomRareCode_ TargetItem(%d:%s) TotalProb(%d) RareTotalCount(%d)\r\n"
		, i_pTargetItemInfo->ItemNum, i_pTargetItemInfo->ItemName, nTotalSumProb, nRareTotalCnts);
	return 0;		// 2007-12-07 by cmkwon, 여기로 오면 안됨, 무조건 레어가 붙어서 나가야 함
}

// start 2011-10-20 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션
///////////////////////////////////////////////////////////////////////////////
/// \fn			CFieldIOCP::CheckFixRareCode(ITEM *i_pTargetItemInfo, BOOL i_bIsPrefix, int i_nFixedCodeNum)
/// \brief		2011-10-20 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션 검사
/// \author		hskim
/// \date		2011-10-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::CheckFixRareCode(ITEM *i_pTargetItemInfo, BOOL i_bIsPrefix, int i_nFixedCodeNum)
{
	BOOL bPrefix = FALSE;
	BOOL bSuffix = TRUE;
	int	nReqUseType = 0;

	vector<RARE_ITEM_INFO* > *pRareFixVector = this->m_mapLevel2RareItemInfo.findEZ_ptr(min(CHARACTER_MAX_LEVEL, i_pTargetItemInfo->ReqMinLevel));

	if (NULL == pRareFixVector)
	{
		return FALSE;
	}

	if (i_bIsPrefix)
	{
		bPrefix = TRUE;
		bSuffix = FALSE;
	}

	for (int i = 0; i < pRareFixVector->size(); i++)
	{
		RARE_ITEM_INFO *pRareItemInfo = (*pRareFixVector)[i];

		if (pRareItemInfo->CodeNum == i_nFixedCodeNum)
		{
			if ((bPrefix && FALSE == IS_RARE_PREFIX(pRareItemInfo->CodeNum))
				|| (bSuffix && FALSE == IS_RARE_SUFFIX(pRareItemInfo->CodeNum))
				|| FALSE == COMPARE_ITEMKIND(pRareItemInfo->ReqItemKind, i_pTargetItemInfo->Kind))
			{
				return FALSE;
			}

			return TRUE;
		}
	}

	return FALSE;
}
// end 2011-10-20 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션

///////////////////////////////////////////////////////////////////////////////
/// \fn			UINT CFieldIOCP::GetRandInt32(UINT i_uiMin, UINT i_uiMax)
/// \brief		// 2007-12-10 by cmkwon, 랜덤 시스템 수정 - CFieldIOCP::GetRandInt32() 추가
/// \author		cmkwon
/// \date		2007-12-10 ~ 2007-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UINT CFieldIOCP::GetRandInt32(UINT i_uiMin, UINT i_uiMax)
{
	if (i_uiMin >= i_uiMax) return i_uiMin;

	return RANDI(i_uiMin, i_uiMax);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			아레나 통합 - 필드 서버 정보를 추가	
/// \brief		
/// \author		dhjin
/// \date		2007-12-27 ~ 2007-12-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::InsertFieldServerInfoForArenaServer(SARENA_USE_FIELD_SERVER_INFO * i_pFieldServerInfo)
{

	if (i_pFieldServerInfo->MFS_Port == m_SARENA_USE_FIELD_SERVER_INFO.MFS_Port
		&& 0 == strcmp(i_pFieldServerInfo->MFS_IP, m_SARENA_USE_FIELD_SERVER_INFO.MFS_IP)
		&& 0 == strcmp(i_pFieldServerInfo->MFS_Name, m_SARENA_USE_FIELD_SERVER_INFO.MFS_Name))
	{
		m_SARENA_USE_FIELD_SERVER_INFO = *i_pFieldServerInfo;

		return true;
	}

	if (0 == strcmp(i_pFieldServerInfo->MFS_IP, m_SARENA_USE_FIELD_SERVER_INFO.MFS_IP) ||
		0 == strcmp(i_pFieldServerInfo->MFS_Name, m_SARENA_USE_FIELD_SERVER_INFO.MFS_Name))
		
		return false;

	m_SARENA_USE_FIELD_SERVER_INFO = *i_pFieldServerInfo;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CIOCPSocket * CFieldIOCP::GetMFSSockForArenaServer(USHORT i_MFSID)
/// \brief		아레나 통합 - 필드 서버 소켓 가져오기
/// \author		dhjin
/// \date		2007-12-28 ~ 2007-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//CIOCPSocket* CFieldIOCP::GetMFSSockForArenaServer(const char* name)
//{
//	mt_auto_lock autolock { m_mtvectSARENA_USE_FIELD_SERVER_INFO };
//	
//	for (auto& x : m_mtvectSARENA_USE_FIELD_SERVER_INFO)
//		
//		if (reinterpret_cast<CFieldIOCPSocket*>(x.Socket)->Arena) return x.Socket;
//
//	
//
//	return nullptr;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendArenaServerPay(USHORT i_MFSID, BYTE i_byInfluence, INT i_nPayInfluencePoint)
/// \brief		아레나 통합 - 필드 서버로 보상 전송 
/// \author		dhjin
/// \date		2008-01-10 ~ 2008-01-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendArenaServerPay(BYTE i_byInfluence, INT i_nPayInfluencePoint)
{
	auto mfs = GetMFSSockForArenaServer();
	
	if (!mfs) return;

	MessageData<T_FtoA_ARENA_SERVER_PAY> pSendMsg;

	pSendMsg->Influence = i_byInfluence;
	pSendMsg->PayInfluencePoint = i_nPayInfluencePoint;

	mfs->SendAddData(pSendMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::ConvertArenaRenderUserName(CHAR * i_pArenaCharacterName, CHAR * o_pSendArenaCharacterName)
/// \brief		아레나 통합 - 아레나 서버에서 유저 이름 '\xxx' 띠고 보여주기
/// \author		dhjin
/// \date		2008-02-21 ~ 2008-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::ConvertArenaRenderUserName(CHAR * i_pArenaCharacterName, CHAR * o_pSendArenaCharacterName)
{
	char chFindChar[8];
	//memset(chFindChar, 0x00, 8);
	//sprintf(chFindChar, "\\"); 
	char* pFindChChatting = strstr(i_pArenaCharacterName, chFindChar);
	int nPos = strlen(i_pArenaCharacterName) + 1;

	if (pFindChChatting)
	{
		nPos = pFindChChatting - i_pArenaCharacterName;
	}
	strncpy(o_pSendArenaCharacterName, i_pArenaCharacterName, nPos);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendAllLetterByInfluence(BYTE i_byInfluence, SLETTER_INFO * i_pAllLetterInfo)
/// \brief		EP3 편지 시스템 - 접속해 있는 같은 세력 유저에게 전체 편지 추가
/// \author		dhjin
/// \date		2008-05-09 ~ 2008-05-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendAllLetterByInfluence(BYTE i_byInfluence, SLETTER_INFO * i_pAllLetterInfo)
{
	if (!IS_VCN_INFLUENCE_TYPE(i_byInfluence)
		&& !IS_ANI_INFLUENCE_TYPE(i_byInfluence))
	{
		return;
	}

	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			&& pSock->IsCheckInfluenceTypeANDSocketType(i_byInfluence, ST_CLIENT_TYPE))
		{
			i_pAllLetterInfo->RecvCharacterUID = pSock->m_character.CharacterUniqueNumber;
			pSock->m_LetterManager.InsertAllLetter(i_pAllLetterInfo);

			INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_ALLLETTER_RECEIVE, T_FC_CHAT_ALLLETTER_RECEIVE, pSMsg, SendBuf);
			util::strncpy(pSMsg->SendCharacterName, i_pAllLetterInfo->SendCharacterName, SIZE_MAX_CHARACTER_NAME);
			pSock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHAT_ALLLETTER_RECEIVE));
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldIOCP::WarpAllInfluenceUsersToCity(BYTE i_byInflTy, BOOL i_bIncludeDiedCharacter/*=FALSE*/)
/// \brief		// 2008-10-28 by cmkwon, 대회서버 시스템 수정(MotherShip 소환시 상대 세력 도시맵으로 이동) - 
/// \author		cmkwon
/// \date		2008-10-28 ~ 2008-10-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::WarpAllInfluenceUsersToCity(BYTE i_byInflTy, BOOL i_bIncludeDiedCharacter/*=FALSE*/, BOOL bExclusionMapByMSWar/*=FALSE*/)		// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
{
	int nWarpedCnts = 0;

	CFieldIOCPSocket *pSock = NULL;
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			&& pSock->IsCheckInfluenceTypeANDSocketType(i_byInflTy, ST_CLIENT_TYPE))
		{

			// 2009-02-05 by cmkwon, 전쟁시 강제 워프 관련 처리 수정 - ClearCharacterForWarp() 함수에서 처리
			// 			if(i_bIncludeDiedCharacter
			// 				&& COMPARE_BODYCON_BIT(pSock->GetCharacter()->BodyCondition, BODYCON_DEAD_MASK))
			// 			{
			// 				pSock->CharacterDeadGameStartRoutine(TRUE, pSock->GetCharacter()->HP, pSock->GetCharacter()->DP);
			// 				pSock->m_bDeadReasonByPK		= FALSE;				
			// 			}

			Err_t errNum = pSock->ClearCharacterForWarp();	// 2009-02-05 by cmkwon, 전쟁시 강제 워프 관련 처리 수정 - CFieldIOCP::WarpAllInfluenceUsersToCity# 처리
			if (ERR_NO_ERROR != errNum)
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Can't warp in CFieldIOCP::WarpAllInfluenceUsersToCity# !! %s ErrorNum(%d) 1\r\n", GetCharacterString(pSock->GetCharacter(), string()), errNum);
				continue;
			}
			errNum = pSock->IsEnableWarpToCityMap();
			if (ERR_NO_ERROR != errNum)
			{// 2008-10-28 by cmkwon, 워프 안되는 상황임. - 알수 없음
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Can't warp in CFieldIOCP::WarpAllInfluenceUsersToCity# !! %s ErrorNum(%d) 2\r\n", GetCharacterString(pSock->GetCharacter(), string()), errNum);
				continue;
			}

			// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
			if (TRUE == bExclusionMapByMSWar)
			{
				CFieldMapChannel *pCurrentFieldMapChannel = pSock->m_pCurrentFieldMapChannel;

				if (NULL != pCurrentFieldMapChannel)
				{
					if (TRUE == IsWarpExclusionMapByMSWar(i_byInflTy, pCurrentFieldMapChannel->GetMapInfluenceTypeW()))
					{
						// 해당 맵에서는 마을로 귀환 처리하지 않는다

						continue;
					}
				}
			}
			// end 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프

			EventResult_t evRet = pSock->WarpToCityMap();
			if (EVENT_RESULT_CONTINUE != evRet)
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Error] CFieldIOCP::WarpAllInfluenceUsersToCity# WarpToCityMap error !!, %s EventResult(%d)\r\n"
					, GetCharacterString(pSock->GetCharacter(), string()), evRet);
			}
			else
			{
				nWarpedCnts++;
			}
		}
	}

	return nWarpedCnts;
}

// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
BOOL CFieldIOCP::IsWarpExclusionMapByMSWar(BYTE byInflTy, int nMapInfluenceType)
{
	if (COMPARE_INFLUENCE(byInflTy, INFLUENCE_TYPE_VCN))
	{
		return !IS_MAP_INFLUENCE_MSWAR_ANI(nMapInfluenceType);
	}
	else if (COMPARE_INFLUENCE(byInflTy, INFLUENCE_TYPE_ANI))
	{
		return !IS_MAP_INFLUENCE_MSWAR_VCN(nMapInfluenceType);
	}

	return TRUE;
}
// end 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
///				// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
//BOOL CFieldIOCP::IsEnableSelectInfluence(BYTE i_byInflTy)
BOOL CFieldIOCP::IsSelectableInfluence(BYTE i_byInflTy, BYTE i_byLv)
{
	// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
	// 	///////////////////////////////////////////////////////////////////////////////
	// 	// 2009-06-03 by cmkwon, 세력 선택시 제한 시스템 사용 여부 플래그 추가 - 
	// 	if(FALSE == g_pFieldGlobal->GetUseInflSelectionRestrictSystem())
	// 	{
	// 		return TRUE;
	// 	}
	// 
	// 	int nInflPercent = this->GetInfluenceTypeDistirbutionPercent(i_byInflTy);
	// 
	// 	if(m_bWarningStateForSelectInfluence)
	// 	{
	// 		if(50 < nInflPercent)
	// 		{
	// 			return FALSE;
	// 		}
	// 		if(util::in_range(49, nInflPercent, 51))
	// 		{
	// 			m_bWarningStateForSelectInfluence	= FALSE;
	// 		}
	// 		return TRUE;
	// 	}
	// 
	// 	if(MAX_INFLUENCE_PERCENT <= nInflPercent)
	// 	{
	// 		m_bWarningStateForSelectInfluence	= TRUE;
	// 		return FALSE;
	// 	}
	// 	return TRUE;
		///////////////////////////////////////////////////////////////////////////////
		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
	return m_InflRateMan.IsSelectableInfluence(i_byInflTy, i_byLv);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
/// \author		cmkwon
/// \date		2009-08-05 ~ 2009-08-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::ArrangePartyFormationSkill(mapINT2ItemSkillPtr *o_pPartyItemSkillList, ez_map<INT, ITEM*> *i_pItemInfoList)
{
	o_pPartyItemSkillList->clear();
	UID64_t		uid4Skill = ITEM_UID_FOR_PARTY_FLIGHT_FORMATION_SKILL_START;

	ez_map<int, ITEM*>::iterator itr = i_pItemInfoList->begin();
	for (; itr != i_pItemInfoList->end(); itr++)
	{
		ITEM *pItemInfo = itr->second;
		if (FALSE == IS_SKILL_ITEM(pItemInfo->Kind)
			|| FALSE == COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, SKILL_ATTR_PARTY_FORMATION_SKILL))
		{
			continue;
		}
		if (FALSE == IS_VALID_UID64(uid4Skill))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] PartyFormationSkill Error 11000 !! %I64d, %d \r\n", uid4Skill, o_pPartyItemSkillList->size());
			break;
		}

		ITEM_SKILL *pItemSkill = new ITEM_SKILL(pItemInfo);
		pItemSkill->UniqueNumber = uid4Skill++;
		if (FALSE == o_pPartyItemSkillList->insertEZ(pItemSkill->ItemNum, pItemSkill))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] PartyFormationSkill Error 12000 !! %d %I64d \r\n", pItemInfo->ItemNum, pItemSkill->UniqueNumber);
			util::del(pItemSkill);
		}
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] PartyFormationSkill loaded !! SkillCount(%d) \r\n", o_pPartyItemSkillList->size());
	return o_pPartyItemSkillList->size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
/// \author		cmkwon
/// \date		2009-08-05 ~ 2009-08-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_SKILL * CFieldIOCP::SearchPartyFormationSkill(INT i_nItemNum)
{
	return m_PartyFormationItemSkillList.findEZ(i_nItemNum);
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
int CFieldIOCP::ResetItemParamOverlapList(vectSITEM_PARAM_OVERLAP *i_pParamOverlapList)
{
	m_mapItemParamOverlapList.clear();

	vectSITEM_PARAM_OVERLAP::iterator vectItr(i_pParamOverlapList->begin());
	for (; vectItr != i_pParamOverlapList->end(); vectItr++)
	{
		SITEM_PARAM_OVERLAP *pOverlap = &*vectItr;

		ITEM *pItemInfo = this->GetItemInfo(pOverlap->ItemNum);
		if (NULL == pItemInfo)
		{
			continue;
		}

		vectINT *pOverlapIdxList = m_mapItemParamOverlapList.findEZ_ptr(pOverlap->ItemNum);
		if (pOverlapIdxList)
		{
			pOverlapIdxList->push_back(pOverlap->OverlapIndex);
		}
		else
		{
			vectINT tmOverlapIdxList;
			tmOverlapIdxList.push_back(pOverlap->OverlapIndex);
			if (false == m_mapItemParamOverlapList.insertEZ(pOverlap->ItemNum, tmOverlapIdxList))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::ResetItemParamOverlapList# insertEZ error !! ItemNum(%d) OverlapIdex(%d) \r\n"
					, pOverlap->ItemNum, pOverlap->OverlapIndex);
				return FALSE;
			}
		}
	}

	ezmapINT2vectINT::iterator mapItr(m_mapItemParamOverlapList.begin());
	for (; mapItr != m_mapItemParamOverlapList.end(); mapItr++)
	{
		ITEM *pItemInfo = this->GetItemInfo(mapItr->first);
		if (NULL == pItemInfo)
		{
			continue;
		}

		if (NULL != pItemInfo->pParamOverlapIdxList)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::ResetItemParamOverlapList# set error !! ItemNum(%d)\r\n"
				, pItemInfo->ItemNum);
			return FALSE;
		}

		pItemInfo->pParamOverlapIdxList = &(mapItr->second);
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-02-02 ~ 2010-02-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::LimitedEReset(void)
{
	mt_auto_lock mtA(&m_LimitedEItemList);
	m_LimitedEItemList.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-02-02 ~ 2010-02-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_W_COUNT *CFieldIOCP::LimitedEFindItemInfoNoLock(INT i_nItemNum)
{
	if (m_LimitedEItemList.empty())
	{
		return NULL;
	}

	mtvectITEM_W_COUNT::iterator itr(m_LimitedEItemList.begin());
	for (; itr != m_LimitedEItemList.end(); itr++)
	{
		ITEM_W_COUNT *pItemWCnt = &*itr;
		if (i_nItemNum == pItemWCnt->ItemNum)
		{
			return pItemWCnt;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-02-02 ~ 2010-02-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::LimitedEGetItemWCount(vectITEM_W_COUNT *o_pItemCntList)
{
	if (m_LimitedEItemList.empty())
	{
		return FALSE;
	}

	mt_auto_lock mtA(&m_LimitedEItemList);

	o_pItemCntList->insert(o_pItemCntList->begin(), m_LimitedEItemList.begin(), m_LimitedEItemList.end());
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-02-02 ~ 2010-02-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::LimitedEUpdateItemCount(INT i_nItemNum, INT i_nRemainCnt)
{
	mt_auto_lock mtA(&m_LimitedEItemList);
	ITEM_W_COUNT *pItemWCnt = this->LimitedEFindItemInfoNoLock(i_nItemNum);
	if (pItemWCnt)
	{
		pItemWCnt->Count = i_nRemainCnt;
		return;
	}

	ITEM_W_COUNT itemWCnt;
	itemWCnt.ItemNum = i_nItemNum;
	itemWCnt.Count = i_nRemainCnt;
	m_LimitedEItemList.push_back(itemWCnt);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-02-02 ~ 2010-02-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::LimitedEUpdateShopItemW(UINT i_nBuildIdx, CFieldMapProject *i_pFMapPro)
{
	vectITEM_W_COUNT tmItemWCnt;

	if (FALSE == this->LimitedEGetItemWCount(&tmItemWCnt))
	{
		return;
	}

	if (FALSE == i_pFMapPro->LimitedEUpdate(i_nBuildIdx, &tmItemWCnt))
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::LimitedEUpdateShopItemW# Update error !! BuildIdx(%d) ItemCnt(%d) \r\n"
			, i_nBuildIdx, tmItemWCnt.size());
	}
}

// 2013-03-13 by hskim, 웹 캐시 상점
BOOL CFieldIOCP::IsCashShopItemNoLock(INT nItemNum)
{
	if (m_LimitedEItemList.empty())
	{
		return NULL;
	}

	mtvectITEM_W_COUNT::iterator itr(m_LimitedEItemList.begin());
	for (; itr != m_LimitedEItemList.end(); itr++)
	{
		ITEM_W_COUNT *pItemWCnt = &*itr;
		if (nItemNum == pItemWCnt->ItemNum)
		{
			return TRUE;
		}
	}
	return NULL;
}
// end 2013-03-13 by hskim, 웹 캐시 상점

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 기존 소스 수정 - 
/// \author		cmkwon
/// \date		2010-04-20 ~ 2010-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldIOCP::GetLuckyMachineItemList(vectSLUCKY_MACHINEPtr *o_pLuckyMachineItemList, INT i_buildingIdx, INT i_machineNum, INT i_coinItemNum)
{
	vectSLUCKY_MACHINE::iterator itr = this->m_vectLuckyMachine.begin();
	for (; itr != this->m_vectLuckyMachine.end(); itr++)
	{
		SLUCKY_MACHINE *pLuckyMachineItem = &*itr;

		if (i_coinItemNum == pLuckyMachineItem->CoinItemNum
			&& i_buildingIdx == pLuckyMachineItem->BuildingIndex
			&& i_machineNum == pLuckyMachineItem->MachineNum)
		{
			o_pLuckyMachineItemList->push_back(pLuckyMachineItem);
		}
	}

	return o_pLuckyMachineItemList->size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 기존 소스 수정 - 
/// \author		cmkwon
/// \date		2010-04-20 ~ 2010-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CFieldIOCP::GetLuckyMachineKind(INT i_buildingIdx, INT i_machineNum, INT i_coinItemNum)
{
	vectSLUCKY_MACHINE::iterator itr = this->m_vectLuckyMachine.begin();
	for (; itr != this->m_vectLuckyMachine.end(); itr++)
	{
		SLUCKY_MACHINE *pLuckyMachineItem = &*itr;

		if (i_coinItemNum == pLuckyMachineItem->CoinItemNum
			&& i_buildingIdx == pLuckyMachineItem->BuildingIndex
			&& i_machineNum == pLuckyMachineItem->MachineNum)
		{
			return pLuckyMachineItem->MachineKind;
		}
	}

	return LUCKY_MACHINE_KIND_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SearchMissionMaster(int i_nQuestIdx, CFieldIOCPSocket * i_pFISoc, vector<CFieldIOCPSocket*> * o_pVecMissionMasterIOCPSocket)
/// \brief		
/// \author		dhjin
/// \date		2008-12-08 ~ 2008-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SearchMissionMaster(int i_nQuestIdx, CFieldIOCPSocket * i_pFISoc, vector<CFieldIOCPSocket*> * o_pVecMissionMasterIOCPSocket)
{
	if (NULL == i_pFISoc)
	{
		return;
	}

	CFieldIOCPSocket *pSock = NULL;
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter()
			&& pSock->IsCheckInfluenceTypeANDSocketType(i_pFISoc->m_character.InfluenceType, ST_CLIENT_TYPE)
			&& ARENA_STATE_NONE == pSock->m_ArenaInfo.State
			&& INFINITY_STATE_NONE == pSock->m_InfinityPlayingInfo.InfinityState		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다. 인피 상태 체크
			&& FALSE == pSock->m_bMissionMaster
			&& pSock->CheckQuestComplete(i_nQuestIdx)
			&& (USER_INFO_OPTION_MISSIONMASTER & pSock->m_character.SecretInfoOption)
			)
		{
			o_pVecMissionMasterIOCPSocket->push_back(pSock);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::SendHelpMissionMaster(vector<CFieldIOCPSocket*> * i_pVecMissionMasterIOCPSocket, BYTE* i_pMsgData, int i_nMsgSize)
/// \brief		
/// \author		dhjin
/// \date		2008-12-09 ~ 2008-12-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::SendHelpMissionMaster(vector<CFieldIOCPSocket*> * i_pVecMissionMasterIOCPSocket, BYTE* i_pMsgData, int i_nMsgSize)
{
	if (NULL == i_pVecMissionMasterIOCPSocket)
	{
		return;
	}

	if (SIZE_MAX_MISSIONMASTER_HELP_COUNT >= i_pVecMissionMasterIOCPSocket->size())
	{// 2008-12-08 by dhjin, 미션마스터 도움 요청 가능한 인원이 SIZE_MAX_MISSIONMASTER_HELP_COUNT보다 적으면 모두 보낸다.
		vector<CFieldIOCPSocket*>::iterator itr = i_pVecMissionMasterIOCPSocket->begin();
		while (itr != i_pVecMissionMasterIOCPSocket->end())
		{
			if (NULL != *itr
				&& (*itr)->IsValidCharacter()
				&& ARENA_STATE_NONE == (*itr)->m_ArenaInfo.State
				&& INFINITY_STATE_NONE == (*itr)->m_InfinityPlayingInfo.InfinityState		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다. 인피 상태 체크
				&& FALSE == (*itr)->m_bMissionMaster
				&& (USER_INFO_OPTION_MISSIONMASTER & (*itr)->m_character.SecretInfoOption)
				)
			{
				(*itr)->SendAddData(i_pMsgData, i_nMsgSize);
			}
			else
			{
				itr = i_pVecMissionMasterIOCPSocket->erase(itr);
				continue;
			}
			itr++;
		}
	}
	else
	{// 2008-12-08 by dhjin, 미션마스터를 랜덤으로 뽑아 도움을 요청한다.
		INT	RandomCount = this->GetRandInt32(0, i_pVecMissionMasterIOCPSocket->size());
		INT ListCount = 0;
		INT CheckRandomCount = 0;
		vector<CFieldIOCPSocket*>::iterator itr = i_pVecMissionMasterIOCPSocket->begin();
		while (itr != i_pVecMissionMasterIOCPSocket->end())
		{// 2008-12-09 by dhjin, 랜덤 위치를 찾아서 랜덤 위치부터 5명까지 보낸다.
			if (ListCount >= SIZE_MAX_MISSIONMASTER_HELP_COUNT)
			{
				break;
			}

			if (CheckRandomCount < RandomCount)
			{
				CheckRandomCount++;
				itr++;
				continue;
			}

			if (NULL != *itr
				&& (*itr)->IsValidCharacter()
				&& ARENA_STATE_NONE == (*itr)->m_ArenaInfo.State
				&& INFINITY_STATE_NONE == (*itr)->m_InfinityPlayingInfo.InfinityState		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다. 인피 상태 체크
				&& FALSE == (*itr)->m_bMissionMaster
				&& (USER_INFO_OPTION_MISSIONMASTER & (*itr)->m_character.SecretInfoOption)
				)
			{
				(*itr)->SendAddData(i_pMsgData, i_nMsgSize);
				ListCount++;
			}
			else
			{
				itr = i_pVecMissionMasterIOCPSocket->erase(itr);
				continue;
			}
			itr++;

		}

		if (ListCount < SIZE_MAX_MISSIONMASTER_HELP_COUNT)
		{// 2008-12-09 by dhjin, 만약 5명을 다 못보냈다면 나머지를 보낸다.
			itr = i_pVecMissionMasterIOCPSocket->begin();
			while (itr != i_pVecMissionMasterIOCPSocket->end())
			{
				if (ListCount >= SIZE_MAX_MISSIONMASTER_HELP_COUNT)
				{
					break;
				}

				if (NULL != *itr
					&& (*itr)->IsValidCharacter()
					&& ARENA_STATE_NONE == (*itr)->m_ArenaInfo.State
					&& INFINITY_STATE_NONE == (*itr)->m_InfinityPlayingInfo.InfinityState		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다. 인피 상태 체크
					&& FALSE == (*itr)->m_bMissionMaster
					&& (USER_INFO_OPTION_MISSIONMASTER & (*itr)->m_character.SecretInfoOption)
					)
				{
					(*itr)->SendAddData(i_pMsgData, i_nMsgSize);
					ListCount++;
				}
				else
				{
					itr = i_pVecMissionMasterIOCPSocket->erase(itr);
					continue;
				}
				itr++;
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::WarContributionCalculation(CFieldIOCPSocket * i_pFISoc, UID32_t i_nValue)
/// \brief		전쟁 보상 추가안 - 기여도 계산하여 추가한다.
/// \author		dhjin
/// \date		2008-12-23 ~ 2008-12-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::WarContributionCalculation(CFieldIOCPSocket * i_pFISoc, UID32_t i_nValue)
{
	if (NULL == i_pFISoc)
	{
		return;
	}

	UID32_t WarContribution = this->WarContributionCalculationByGear(i_pFISoc->m_character.UnitKind, i_nValue);

	// 2008-12-23 by dhjin, 모선전중인지 체크하고 계산
	if (m_InflWarManager.CheckSummonBoss())
	{
		mt_auto_lock mta(&m_mtvectCBossWarContributionCalculation);
		for (mtvectCBossWarContributionCalculation::iterator itr(m_mtvectCBossWarContributionCalculation.begin())
			; itr != m_mtvectCBossWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDContribution(i_pFISoc->m_character.CharacterUniqueNumber, WarContribution, i_pFISoc->m_character.InfluenceType, i_pFISoc->m_character.UnitKind);
		}

		if (IS_VALID_UNIQUE_NUMBER(i_pFISoc->m_character.GuildUniqueNumber))
		{
			SWarContributionCalculationGuild tmWarContributionCalculationGuild;
			util::zero(&tmWarContributionCalculationGuild, sizeof(SWarContributionCalculationGuild));
			tmWarContributionCalculationGuild.GuildUID = i_pFISoc->m_character.GuildUniqueNumber;
			util::strncpy(tmWarContributionCalculationGuild.GuildName, i_pFISoc->m_character.GuildName, SIZE_MAX_GUILD_NAME);
			tmWarContributionCalculationGuild.GuildInfl = i_pFISoc->m_character.InfluenceType;
			tmWarContributionCalculationGuild.Contribution = WarContribution;

			for (mtvectCBossWarContributionCalculation::iterator itr(m_mtvectCBossWarContributionCalculation.begin())
				; itr != m_mtvectCBossWarContributionCalculation.end(); itr++)
			{
				(*itr)->UpdateGuildUIDContribution(&tmWarContributionCalculationGuild);
			}
		}

		mta.auto_unlock_cancel();
	}

	// 2008-12-23 by dhjin, 전진기지전쟁 중인지 체크하고 계산
	if (this->m_OutPostManager.CheckALLOutPostWaring())
	{
		mt_auto_lock mta(&m_mtvectCOutPostWarContributionCalculation);
		for (mtvectCOutPostWarContributionCalculation::iterator itr(m_mtvectCOutPostWarContributionCalculation.begin())
			; itr != m_mtvectCOutPostWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDContribution(i_pFISoc->m_character.CharacterUniqueNumber, WarContribution, i_pFISoc->m_character.InfluenceType, i_pFISoc->m_character.UnitKind);
		}
		mta.auto_unlock_cancel();
	}

	// 2008-12-23 by dhjin, 전략포인트전쟁 중인지 체크하고 계산
	if (m_InflWarManager.CheckSummonOnlyStrategyPoint())
	{
		mt_auto_lock mta(&m_mtvectCStrategyPointWarContributionCalculation);
		for (mtvectCStrategyPointWarContributionCalculation::iterator itr(m_mtvectCStrategyPointWarContributionCalculation.begin())
			; itr != m_mtvectCStrategyPointWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDContribution(i_pFISoc->m_character.CharacterUniqueNumber, WarContribution, i_pFISoc->m_character.InfluenceType, i_pFISoc->m_character.UnitKind);
		}
		mta.auto_unlock_cancel();
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID64_t CFieldIOCP::WarContributionCalculationByGear(USHORT i_sUnitKind, UID32_t i_nValue)
/// \brief		전쟁 보상 추가안 - 각 기어별 기여도 계산
/// \author		dhjin
/// \date		2008-12-23 ~ 2008-12-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t CFieldIOCP::WarContributionCalculationByGear(USHORT i_sUnitKind, UID32_t i_nValue)
{
	if (IS_BGEAR(i_sUnitKind))
	{
		return i_nValue * BGEAR_WAR_CONTRIBUTION;
	}
	else if (IS_IGEAR(i_sUnitKind))
	{
		return i_nValue * IGEAR_WAR_CONTRIBUTION;
	}
	else if (IS_MGEAR(i_sUnitKind))
	{
		return i_nValue * MGEAR_WAR_CONTRIBUTION;
	}
	else if (IS_AGEAR(i_sUnitKind))
	{
		return i_nValue * AGEAR_WAR_CONTRIBUTION;
	}

	return i_nValue;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::WarDeathContributionCalculation(CFieldIOCPSocket * i_pFISoc)
/// \brief		전쟁 보상 추가안 - 유저 죽은 카운트 추가
/// \author		dhjin
/// \date		2009-01-06 ~ 2009-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::WarDeathContributionCalculation(CFieldIOCPSocket * i_pFISoc)
{
	if (NULL == i_pFISoc)
	{
		return;
	}

	// 2008-12-23 by dhjin, 모선전중인지 체크하고 계산
	if (m_InflWarManager.CheckSummonBoss())
	{
		mt_auto_lock mta(&m_mtvectCBossWarContributionCalculation);
		for (mtvectCBossWarContributionCalculation::iterator itr(m_mtvectCBossWarContributionCalculation.begin())
			; itr != m_mtvectCBossWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDInflDeathCount(i_pFISoc->m_character.CharacterUniqueNumber, i_pFISoc->m_character.InfluenceType);
		}
		mta.auto_unlock_cancel();
	}

	// 2008-12-23 by dhjin, 전진기지전쟁 중인지 체크하고 계산
	if (this->m_OutPostManager.CheckALLOutPostWaring())
	{
		mt_auto_lock mta(&m_mtvectCOutPostWarContributionCalculation);
		for (mtvectCOutPostWarContributionCalculation::iterator itr(m_mtvectCOutPostWarContributionCalculation.begin())
			; itr != m_mtvectCOutPostWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDInflDeathCount(i_pFISoc->m_character.CharacterUniqueNumber, i_pFISoc->m_character.InfluenceType);
		}
		mta.auto_unlock_cancel();
	}

	// 2008-12-23 by dhjin, 전략포인트전쟁 중인지 체크하고 계산
	if (m_InflWarManager.CheckSummonOnlyStrategyPoint())
	{
		mt_auto_lock mta(&m_mtvectCStrategyPointWarContributionCalculation);
		for (mtvectCStrategyPointWarContributionCalculation::iterator itr(m_mtvectCStrategyPointWarContributionCalculation.begin())
			; itr != m_mtvectCStrategyPointWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDInflDeathCount(i_pFISoc->m_character.CharacterUniqueNumber, i_pFISoc->m_character.InfluenceType);
		}
		mta.auto_unlock_cancel();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::WarOtherInflStayTime(CFieldIOCPSocket * i_pFISoc, BOOL i_bCheckStart)
/// \brief		전쟁 보상 추가안 - 상대세력에 머문시간
/// \author		dhjin
/// \date		2009-01-06 ~ 2009-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::WarOtherInflStayTime(CFieldIOCPSocket * i_pFISoc, BOOL i_bCheckStart)
{
	if (NULL == i_pFISoc)
	{
		return;
	}

	// 2008-12-23 by dhjin, 모선전중인지 체크하고 계산
	if (m_InflWarManager.CheckSummonBoss())
	{
		mt_auto_lock mta(&m_mtvectCBossWarContributionCalculation);
		for (mtvectCBossWarContributionCalculation::iterator itr(m_mtvectCBossWarContributionCalculation.begin())
			; itr != m_mtvectCBossWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDOtherInflStayTime(i_pFISoc->m_character.CharacterUniqueNumber, i_bCheckStart);
		}
		mta.auto_unlock_cancel();
	}

	// 2008-12-23 by dhjin, 전진기지전쟁 중인지 체크하고 계산
	if (this->m_OutPostManager.CheckALLOutPostWaring())
	{
		mt_auto_lock mta(&m_mtvectCOutPostWarContributionCalculation);
		for (mtvectCOutPostWarContributionCalculation::iterator itr(m_mtvectCOutPostWarContributionCalculation.begin())
			; itr != m_mtvectCOutPostWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDOtherInflStayTime(i_pFISoc->m_character.CharacterUniqueNumber, i_bCheckStart);
		}
		mta.auto_unlock_cancel();
	}

	// 2008-12-23 by dhjin, 전략포인트전쟁 중인지 체크하고 계산
	if (m_InflWarManager.CheckSummonOnlyStrategyPoint())
	{
		mt_auto_lock mta(&m_mtvectCStrategyPointWarContributionCalculation);
		for (mtvectCStrategyPointWarContributionCalculation::iterator itr(m_mtvectCStrategyPointWarContributionCalculation.begin())
			; itr != m_mtvectCStrategyPointWarContributionCalculation.end(); itr++)
		{
			(*itr)->UpdateCharacterUIDOtherInflStayTime(i_pFISoc->m_character.CharacterUniqueNumber, i_bCheckStart);
		}
		mta.auto_unlock_cancel();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::WarStartOtherInflStayTimeForAllUser()
/// \brief		전쟁 보상 추가안 - 전쟁 시작시 상대세력에 이미 있던 유저들 머문시간 설정
/// \author		dhjin
/// \date		2009-01-06 ~ 2009-01-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::WarStartOtherInflStayTimeForAllUser()
{
	CFieldIOCPSocket *pSock = NULL;
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CFieldIOCP::WarStartOtherInflStayTimeForAllUser#, 아래와 같이 수정
			//			&& IS_OTHER_INFLUENCE_MAP(pSock->m_character.InfluenceType, pSock->GetCurrentFieldMapChannel()->GetMapInfluenceTypeW())
			&& COMPARE_INFLUENCE(pSock->m_character.InfluenceType, INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI)		// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
			&& CAtumSJ::IsOtherInfluenceMap(pSock->m_character.InfluenceType, pSock->m_character.GetStartCityMapIndex(), pSock->GetCurrentFieldMapChannel()->GetMapInfluenceTypeW())	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
			&& (0 == pSock->GetCurrentFieldMapChannel()->GetMapChannelIndex().ChannelIndex))
		{
			this->WarOtherInflStayTime(pSock, TRUE);
		}
	}
}

void CFieldIOCP::InsertCBossWarContributionCalculation(MapIndex_t i_nMapIndex, int i_nStartWarContributionPoint)
{
	mt_auto_lock mta(&m_mtvectCBossWarContributionCalculation);
	for (mtvectCBossWarContributionCalculation::iterator itr(m_mtvectCBossWarContributionCalculation.begin())
		; itr != m_mtvectCBossWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex())
		{
			return;
		}
	}

	CBossWarContributionCalculation * NewBossWar = new CBossWarContributionCalculation(i_nMapIndex, i_nStartWarContributionPoint);
	m_mtvectCBossWarContributionCalculation.push_back(NewBossWar);

}

void CFieldIOCP::InsertCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectCOutPostWarContributionCalculation);
	for (mtvectCOutPostWarContributionCalculation::iterator itr(m_mtvectCOutPostWarContributionCalculation.begin())
		; itr != m_mtvectCOutPostWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex())
		{
			return;
		}
	}

	CWarContributionCalculation * NewWar = new CWarContributionCalculation();
	NewWar->SetMapIndex(i_nMapIndex);
	m_mtvectCOutPostWarContributionCalculation.push_back(NewWar);

}

void CFieldIOCP::InsertCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectCStrategyPointWarContributionCalculation);
	for (mtvectCStrategyPointWarContributionCalculation::iterator itr(m_mtvectCStrategyPointWarContributionCalculation.begin())
		; itr != m_mtvectCStrategyPointWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex())
		{
			return;
		}
	}

	CWarContributionCalculation * NewWar = new CWarContributionCalculation();
	NewWar->SetMapIndex(i_nMapIndex);
	m_mtvectCStrategyPointWarContributionCalculation.push_back(NewWar);
}

void CFieldIOCP::DeleteCBossWarContributionCalculation(MapIndex_t i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectCBossWarContributionCalculation);
	for (mtvectCBossWarContributionCalculation::iterator itr(m_mtvectCBossWarContributionCalculation.begin())
		; itr != m_mtvectCBossWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex())
		{
			util::del(*itr);
			m_mtvectCBossWarContributionCalculation.erase(itr);
			return;
		}
	}
}

void CFieldIOCP::DeleteCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectCOutPostWarContributionCalculation);
	for (mtvectCOutPostWarContributionCalculation::iterator itr(m_mtvectCOutPostWarContributionCalculation.begin())
		; itr != m_mtvectCOutPostWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex())
		{
			util::del(*itr);
			m_mtvectCOutPostWarContributionCalculation.erase(itr);
			return;
		}
	}
}

void CFieldIOCP::DeleteCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex)
{
	mt_auto_lock mta(&m_mtvectCStrategyPointWarContributionCalculation);
	for (mtvectCStrategyPointWarContributionCalculation::iterator itr(m_mtvectCStrategyPointWarContributionCalculation.begin())
		; itr != m_mtvectCStrategyPointWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex())
		{
			util::del(*itr);
			m_mtvectCStrategyPointWarContributionCalculation.erase(itr);
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::PayCBossWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl)
/// \brief		전쟁 보상 추가안 - 승리세력에게 보상 지급
/// \author		dhjin
/// \date		2008-12-23 ~ 2008-12-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PayCBossWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl)
{
	mt_auto_lock mta(&m_mtvectCBossWarContributionCalculation);
	for (mtvectCBossWarContributionCalculation::iterator itr(m_mtvectCBossWarContributionCalculation.begin())
		; itr != m_mtvectCBossWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex()
			&& PAY_MINIMUN_COUNT <= (*itr)->GetSizeCharacterUIDContribution())
		{
			// 2008-12-23 by dhjin, 개인 지급
			this->PaySendBossWarContribution(*itr, i_byWinInfl);

			// 2008-12-23 by dhjin, 지도자 지급
			this->PaySendBossWarContributionLeader(*itr, i_byWinInfl);

			// 2008-12-23 by dhjin, 각 여단 지급
			this->PaySendBossWarContributionGuild(*itr, i_byWinInfl);

			MSG_FL_LOG_WAR_CONTRIBUTION_GEAR tmContributionGaer;
			util::zero(&tmContributionGaer, sizeof(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR));
			(*itr)->GetGearContributioninfo(&tmContributionGaer);
			CAtumLogSender::SendLogMessageWarContributionGear(&tmContributionGaer);

			// 2010-06-01 by shcho, GLogDB 관련 -
			this->SendBossGLogEventParticipationRate(i_nMapIndex, (*itr)->GetSizeCharacterUIDContribution(), (*itr)->GetWarStartTime());
			return;
		}
	}
}

// 2010-06-01 by shcho, GLogDB 관련 -
void CFieldIOCP::SendBossGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime)
{
#ifdef S_GLOG_HSSON
	char szDes[512];
	sprintf(szDes, "BOSS EVENT!! MapName[%s]", CAtumSJ::GetMapName(i_nMapIndex));
	MSG_FL_LOG_EVENT_PARTICIPATION_RATE EventMsg;
	util::zero(&EventMsg, sizeof(MSG_FL_LOG_EVENT_PARTICIPATION_RATE));
	util::strncpy(EventMsg.Description, szDes, SIZE_MAX_GLOG_EVENT_DESCRIPTION);
	EventMsg.EndTime.SetCurrentDateTime();
	EventMsg.ParticipationCount = i_nCnt;
	INT TotalCnt = this->GetMapWorkspace()->GetTotalUserCurrentCnt();

	// start 2011-12-30 by hskim, EP4 - 버그 수정 (서버 죽는 문제 해결)
	////////////////////////////////
	// 기존
	// EventMsg.ParticipationRate	= (USHORT)( (i_nCnt * 100)  / TotalCnt );

	// 수정
	//
	if (0 != TotalCnt)
	{
		EventMsg.ParticipationRate = (USHORT)((i_nCnt * 100) / TotalCnt);
	}
	////////////////////////////////
	// end 2011-12-30 by hskim, EP4 - 버그 수정 (서버 죽는 문제 해결)

	EventMsg.StartTime = *i_pWarStartTime;

	CAtumLogSender::SendGLogEventParticipationRate(&EventMsg);
#endif
}

void CFieldIOCP::SendStrategyPointGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime)
{
#ifdef S_GLOG_HSSON
	char szDes[512];
	sprintf(szDes, "StrategyPoint EVENT!! MapName[%s]", CAtumSJ::GetMapName(i_nMapIndex));
	MSG_FL_LOG_EVENT_PARTICIPATION_RATE EventMsg;
	util::zero(&EventMsg, sizeof(MSG_FL_LOG_EVENT_PARTICIPATION_RATE));
	util::strncpy(EventMsg.Description, szDes, SIZE_MAX_GLOG_EVENT_DESCRIPTION);
	EventMsg.EndTime.SetCurrentDateTime();
	EventMsg.ParticipationCount = i_nCnt;
	INT TotalCnt = this->GetMapWorkspace()->GetTotalUserCurrentCnt();

	// start 2011-12-30 by hskim, EP4 - 버그 수정 (서버 죽는 문제 해결)
	////////////////////////////////
	// 기존
	//EventMsg.ParticipationRate	= (USHORT)( (i_nCnt * 100)  / TotalCnt );

	// 수정
	//
	if (0 != TotalCnt)
	{
		EventMsg.ParticipationRate = (USHORT)((i_nCnt * 100) / TotalCnt);
	}
	////////////////////////////////
	// end 2011-12-30 by hskim, EP4 - 버그 수정 (서버 죽는 문제 해결)

	EventMsg.ParticipationRate = (USHORT)((i_nCnt * 100) / TotalCnt);
	EventMsg.StartTime = *i_pWarStartTime;

	CAtumLogSender::SendGLogEventParticipationRate(&EventMsg);
#endif
}

void CFieldIOCP::SendOutPostGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime)
{
#ifdef S_GLOG_HSSON
	char szDes[512];
	sprintf(szDes, "OutPost EVENT!! MapName[%s]", CAtumSJ::GetMapName(i_nMapIndex));
	MSG_FL_LOG_EVENT_PARTICIPATION_RATE EventMsg;
	util::zero(&EventMsg, sizeof(MSG_FL_LOG_EVENT_PARTICIPATION_RATE));
	util::strncpy(EventMsg.Description, szDes, SIZE_MAX_GLOG_EVENT_DESCRIPTION);
	EventMsg.EndTime.SetCurrentDateTime();
	EventMsg.ParticipationCount = i_nCnt;
	INT TotalCnt = this->GetMapWorkspace()->GetTotalUserCurrentCnt();

	// start 2011-12-30 by hskim, EP4 - 버그 수정 (서버 죽는 문제 해결)
	////////////////////////////////
	// 기존
	//EventMsg.ParticipationRate	= (USHORT)( (i_nCnt * 100)  / TotalCnt );

	// 수정
	//
	if (0 != TotalCnt)
	{
		EventMsg.ParticipationRate = (USHORT)((i_nCnt * 100) / TotalCnt);
	}
	////////////////////////////////
	// end 2011-12-30 by hskim, EP4 - 버그 수정 (서버 죽는 문제 해결)

	EventMsg.StartTime = *i_pWarStartTime;

	CAtumLogSender::SendGLogEventParticipationRate(&EventMsg);
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::PaySendBossWarContribution(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl)
/// \brief		전쟁 보상 추가안 - 개인 보상
/// \author		dhjin
/// \date		2008-12-23 ~ 2008-12-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PaySendBossWarContribution(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl)
{
	if (NULL == i_pBossWarContribution)
	{
		return;
	}

	UID64_t TotalContribution = max(1, i_pBossWarContribution->GetTotalContribution());		// 2009-01-30 by dhjin, 전쟁 보상 추가안 0으로 나누는 버그 수정.

	mtMapCharacterUIDContribution * tmMapCharacterUIDContribution = i_pBossWarContribution->GetmtMapCharacterUIDContribution();

	mt_auto_lock mta(tmMapCharacterUIDContribution);		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - lock 잡아야 한다.

	for (mtMapCharacterUIDContribution::iterator itrC(tmMapCharacterUIDContribution->begin())
		; itrC != tmMapCharacterUIDContribution->end(); itrC++)
	{// 2008-12-23 by dhjin, 개인 지급 
		CFieldIOCPSocket * PayFISoc = this->GetFieldIOCPSocketByCharacterUniqueNumber(itrC->first);
		if (NULL != PayFISoc
			&& PayFISoc->IsValidCharacter(FALSE)		// 2009-03-03 by cmkwon, 전쟁 보상시 죽은 상태의 캐릭터에도 SPI 지급 - 모선전 보상
//			&& i_byWinInfl == PayFISoc->m_character.InfluenceType		// 2009-01-21 by dhjin, 전쟁 보상 추가안 수정 모든 세력 개인 보상 지급
)
		{

			if (OTHER_INFL_STAY == itrC->second.CheckOtherInflStay)
			{// 상대세력맵에 머문 시간 최종 업데이트
				ATUM_DATE_TIME	CurrentTime;
				CurrentTime.SetCurrentDateTime();
				itrC->second.OtherInflStayTime += CurrentTime.GetTimeDiffTimeInMinutes(itrC->second.OtherInflStayStartTime);
				itrC->second.OtherInflStayStartTime.Reset();
			}
			UID32_t Pay;
			int TotalWarTime = max(1, i_pBossWarContribution->GetTotalWarTime());	// 2009-01-30 by dhjin, 전쟁 보상 추가안 0으로 나누는 버그 수정.
			UID64_t tmPay = ((itrC->second.Contribution * PAY_WAR_CONTRIBUTION_BY_BOSS * i_pBossWarContribution->GetInflUserCount(i_byWinInfl)) / TotalContribution)
				+ (((itrC->second.OtherInflStayTime + itrC->second.DeathCount) * PAY_WAR_CONTRIBUTION_BY_BOSS) / (TotalWarTime + i_pBossWarContribution->GetInflDeathCount(i_byWinInfl)));

			// 2009-03-18 by cmkwon, 전쟁 보상 추가안 수정(개인 보상 최대값 설정) - 아래와 같이 수정함.
			// 			if(0 == tmPay)
			// 			{
			// 				tmPay = PAY_MINIMUN_BY_BOSS;
			// 			}
			// 			else if(MAX_ITEM_COUNTS <= tmPay)
			// 			{
			// 				tmPay = MAX_ITEM_COUNTS;
			// 			}
						// 2009-03-18 by cmkwon, 전쟁 보상 추가안 수정(개인 보상 최대값 설정) - 아래와 같이 수정함, 모선전
			tmPay = max(PAY_MINIMUN_BY_BOSS, tmPay);
			tmPay = min(PAY_MAXMUN_BY_BOSS, tmPay);

			Pay = tmPay;

			PayFISoc->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, Pay, IUT_WAR_CONTRIBUTION);
			CAtumLogSender::SendLogMessageWarContribution(PayFISoc->m_character.CharacterUniqueNumber, i_pBossWarContribution->GetMapIndex(), itrC->second.Contribution, Pay, PAY_TYPE_BOSS);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::PaySendBossWarContributionLeader(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl)
/// \brief		전쟁 보상 추가안 - 지도자 보상
/// \author		dhjin
/// \date		2008-12-23 ~ 2008-12-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PaySendBossWarContributionLeader(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl)
{
	if (NULL == i_pBossWarContribution)
	{
		return;
	}

	CInflWarData *pInflWarData = this->m_InflWarManager.GetInflWarDataByInflType(i_byWinInfl);
	if (NULL != pInflWarData)
	{
		CFieldIOCPSocket * PayLeaderFISoc = GetFieldIOCPSocketByCharacterUniqueNumber(pInflWarData->InflLeaderCharacterUID);
		if (NULL != PayLeaderFISoc
			&& PayLeaderFISoc->IsValidCharacter(FALSE)		// 2009-03-03 by cmkwon, 전쟁 보상시 죽은 상태의 캐릭터에도 SPI 지급 - 모선전 지도자 보상
			)
		{

			// 2009-03-18 by cmkwon, 전쟁 보상 추가안 수정(개인 보상 최대값 설정) - 아래와 같이 수정
			//			UID32_t Pay = i_pBossWarContribution->GetStartWarContributionPoint() * PAY_WAR_CONTRIBUTION_LEADER_BY_BOSS;
			//			if(0 == Pay)
			//			{
			//				Pay = PAY_MINIMUN_BY_BOSS;
			//			}
						// 2009-03-18 by cmkwon, 전쟁 보상 추가안 수정(개인 보상 최대값 설정) - 아래와 같이 수정, 여기는 지도자 보상임(개인 보상이 아니다)
			UID64_t Pay = i_pBossWarContribution->GetStartWarContributionPoint() * PAY_WAR_CONTRIBUTION_LEADER_BY_BOSS;
			Pay = max(PAY_MINIMUN_BY_BOSS, Pay);
			Pay = min(MAX_ITEM_COUNTS, Pay);

			PayLeaderFISoc->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, (int)Pay, IUT_WAR_CONTRIBUTION_LEADER);
			CAtumLogSender::SendLogMessageWarContribution(PayLeaderFISoc->m_character.CharacterUniqueNumber, i_pBossWarContribution->GetMapIndex(), i_pBossWarContribution->GetStartWarContributionPoint(), Pay, PAY_TYPE_BOSS_LEADER);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::PaySendBossWarContributionGuild(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl)
/// \brief		전쟁 보상 추가안 - 길드 보상
/// \author		dhjin
/// \date		2008-12-24 ~ 2008-12-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PaySendBossWarContributionGuild(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl)
{
	if (NULL == i_pBossWarContribution)
	{
		return;
	}

	i_pBossWarContribution->SortmtVectWarContributionCalculationGuild();
	BYTE GuildCount = 1;
	INIT_MSG_WITH_BUFFER(MSG_FC_WAR_BOSS_CONTRIBUTION_GUILD, T_FC_WAR_BOSS_CONTRIBUTION_GUILD, pMsg, SendBuf);
	mtVectWarContributionCalculationGuild * tmVectWarContributionCalculationGuild = i_pBossWarContribution->GetmtVectWarContributionCalculationGuild();
	for (mtVectWarContributionCalculationGuild::iterator itrG(tmVectWarContributionCalculationGuild->begin())
		; itrG != tmVectWarContributionCalculationGuild->end(); itrG++)
	{
		if (i_byWinInfl != itrG->GuildInfl)
		{
			continue;
		}

		if (1 == GuildCount)
		{
			UID32_t Pay = (i_pBossWarContribution->GetStartWarContributionPoint() * PAY_WAR_CONTRIBUTION_1_GUILD_BY_BOSS) + PAY_WAR_CONTRIBUTION_1_GUILD_BY_BOSS_PLUS;
			if (0 == Pay)
			{
				Pay = PAY_MINIMUN_BY_BOSS;
			}

			this->PaySendBossWarContributionGuildProcess(itrG->GuildUID, Pay, itrG->Contribution, i_pBossWarContribution->GetMapIndex(), PAY_TYPE_BOSS_1_GUILD);
			pMsg->ContributionGuldInfo[0].order = 1;
			pMsg->ContributionGuldInfo[0].GuildUID = itrG->GuildUID;
			util::strncpy(pMsg->ContributionGuldInfo[0].GuildName, itrG->GuildName, SIZE_MAX_GUILD_NAME);
			pMsg->ContributionGuldInfo[0].GuildUID = itrG->GuildUID;
			pMsg->ContributionGuldInfo[0].GuildPay = Pay;
			GuildCount++;
		}
		else if (2 == GuildCount)
		{
			UID32_t Pay = (i_pBossWarContribution->GetStartWarContributionPoint() * PAY_WAR_CONTRIBUTION_2_GUILD_BY_BOSS) + PAY_WAR_CONTRIBUTION_2_GUILD_BY_BOSS_PLUS;
			if (0 == Pay)
			{
				Pay = PAY_MINIMUN_BY_BOSS;
			}

			this->PaySendBossWarContributionGuildProcess(itrG->GuildUID, Pay, itrG->Contribution, i_pBossWarContribution->GetMapIndex(), PAY_TYPE_BOSS_2_GUILD);
			pMsg->ContributionGuldInfo[1].order = 2;
			pMsg->ContributionGuldInfo[1].GuildUID = itrG->GuildUID;
			util::strncpy(pMsg->ContributionGuldInfo[1].GuildName, itrG->GuildName, SIZE_MAX_GUILD_NAME);
			pMsg->ContributionGuldInfo[1].GuildUID = itrG->GuildUID;
			pMsg->ContributionGuldInfo[1].GuildPay = Pay;
			GuildCount++;
		}
		else if (3 == GuildCount)
		{
			UID32_t Pay = (i_pBossWarContribution->GetStartWarContributionPoint() * PAY_WAR_CONTRIBUTION_3_GUILD_BY_BOSS) + PAY_WAR_CONTRIBUTION_3_GUILD_BY_BOSS_PLUS;
			if (0 == Pay)
			{
				Pay = PAY_MINIMUN_BY_BOSS;
			}

			this->PaySendBossWarContributionGuildProcess(itrG->GuildUID, Pay, itrG->Contribution, i_pBossWarContribution->GetMapIndex(), PAY_TYPE_BOSS_3_GUILD);
			pMsg->ContributionGuldInfo[2].order = 3;
			pMsg->ContributionGuldInfo[2].GuildUID = itrG->GuildUID;
			util::strncpy(pMsg->ContributionGuldInfo[2].GuildName, itrG->GuildName, SIZE_MAX_GUILD_NAME);
			pMsg->ContributionGuldInfo[2].GuildUID = itrG->GuildUID;
			pMsg->ContributionGuldInfo[2].GuildPay = Pay;
			GuildCount++;
		}
		else
		{
			break;
		}
	}
	this->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_WAR_BOSS_CONTRIBUTION_GUILD), i_byWinInfl);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		전쟁 보상 추가안 - 길드 보상 처리
/// \author		dhjin
/// \date		2008-12-24 ~ 2008-12-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PaySendBossWarContributionGuildProcess(UID32_t i_nGuildUID, UID32_t i_nPay, UID64_t i_nContribution, MapIndex_t i_nMapIndex, BYTE i_byPayGrade)
{
	mt_auto_lock mtaAGM(this->GetmtmapAllGuildItemPtrW());
	CGuildItemManager *pGuildItemMan = this->GetGuildItemMangerNoLockW(i_nGuildUID);
	if (NULL == pGuildItemMan
		|| FALSE == pGuildItemMan->GetIsLoaded()
		)
	{
		ITEM* pItemInfo = this->GetItemInfo(MONEY_ITEM_NUMBER);
		if (pItemInfo == NULL)
		{
			return;
		}

		ITEM_GENERAL *pItemGeneral = new ITEM_GENERAL(pItemInfo);
		pItemGeneral->Kind = pItemInfo->Kind;				// 2006-09-26 by cmkwon
		pItemGeneral->UniqueNumber = 0;
		pItemGeneral->AccountUniqueNumber = i_nGuildUID;
		pItemGeneral->Possess = i_nGuildUID;
		pItemGeneral->ItemStorage = ITEM_IN_GUILD_STORE;
		pItemGeneral->Wear = WEAR_NOT_ATTACHED;
		pItemGeneral->CurrentCount = (IS_CHARGABLE_ITEM(pItemInfo->Kind) ? pItemInfo->Charging : i_nPay);
		// 2009-08-25 by cmkwon, 사용하지 않는 필드 제거(td_Store.ScarcityNumber) - 
		//		pItemGeneral->ScarcityNumber		= 0;
		pItemGeneral->ItemWindowIndex = POS_INVALID_POSITION;
		pItemGeneral->NumOfEnchants = 0;

		this->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertGuildStoreItem, NULL
			, NULL, pItemGeneral, NULL);

		CAtumLogSender::SendLogMessageWarContribution(i_nGuildUID, i_nMapIndex, i_nContribution, i_nPay, i_byPayGrade);
		return;
	}

	ITEM_GENERAL *pItemGeneralInGuildStore = pGuildItemMan->GetItemGeneralByItemNum(MONEY_ITEM_NUMBER);
	if (NULL == pItemGeneralInGuildStore)
	{
		ITEM* pItemInfo = this->GetItemInfo(MONEY_ITEM_NUMBER);
		if (pItemInfo == NULL)
		{
			return;
		}

		ITEM_GENERAL *pItemGeneral = new ITEM_GENERAL(pItemInfo);
		pItemGeneral->Kind = pItemInfo->Kind;				// 2006-09-26 by cmkwon
		pItemGeneral->UniqueNumber = 0;
		pItemGeneral->AccountUniqueNumber = i_nGuildUID;
		pItemGeneral->Possess = i_nGuildUID;
		pItemGeneral->ItemStorage = ITEM_IN_GUILD_STORE;
		pItemGeneral->Wear = WEAR_NOT_ATTACHED;
		pItemGeneral->CurrentCount = (IS_CHARGABLE_ITEM(pItemInfo->Kind) ? pItemInfo->Charging : i_nPay);
		// 2009-08-25 by cmkwon, 사용하지 않는 필드 제거(td_Store.ScarcityNumber) - 
		//		pItemGeneral->ScarcityNumber		= 0;
		pItemGeneral->ItemWindowIndex = POS_INVALID_POSITION;
		pItemGeneral->NumOfEnchants = 0;

		this->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertGuildStoreItem, NULL
			, NULL, pItemGeneral, NULL);

		CAtumLogSender::SendLogMessageWarContribution(i_nGuildUID, i_nMapIndex, i_nContribution, i_nPay, i_byPayGrade);
		return;
	}

	INT64 n64CurCnt = pItemGeneralInGuildStore->CurrentCount;
	if (MAX_ITEM_COUNTS < n64CurCnt + i_nPay)
	{// 2007-10-05 by cmkwon, 최대 개수 체크
		i_nPay = MAX_ITEM_COUNTS - pItemGeneralInGuildStore->CurrentCount;
	}
	pItemGeneralInGuildStore->CurrentCount += i_nPay;
	QPARAM_UPDATE_GUILDSTORE *qQParamStoreUpdate = new QPARAM_UPDATE_GUILDSTORE;
	qQParamStoreUpdate->ItemUID = pItemGeneralInGuildStore->UniqueNumber;
	qQParamStoreUpdate->AccountUID = pItemGeneralInGuildStore->AccountUniqueNumber;
	qQParamStoreUpdate->Possess = pItemGeneralInGuildStore->Possess;
	qQParamStoreUpdate->ItemStorage = ITEM_IN_GUILD_STORE;
	qQParamStoreUpdate->Count = pItemGeneralInGuildStore->CurrentCount;
	this->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateGuildStoreItem, NULL, pItemGeneralInGuildStore->AccountUniqueNumber, qQParamStoreUpdate);

	CAtumLogSender::SendLogMessageWarContribution(i_nGuildUID, i_nMapIndex, i_nContribution, i_nPay, i_byPayGrade);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::PayCWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl)
/// \brief		전쟁 보상 추가안 - 전진기지 승리세력에게 보상 지급
/// \author		dhjin
/// \date		2008-12-24 ~ 2008-12-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PayCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl)
{
	mt_auto_lock mta(&m_mtvectCOutPostWarContributionCalculation);
	for (mtvectCOutPostWarContributionCalculation::iterator itr(m_mtvectCOutPostWarContributionCalculation.begin())
		; itr != m_mtvectCOutPostWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex()
			&& PAY_MINIMUN_COUNT <= (*itr)->GetSizeCharacterUIDContribution())
		{
			// 2008-12-23 by dhjin, 개인 지급
			this->PaySendWarContribution(*itr, i_byWinInfl, PAY_WAR_CONTRIBUTION_BY_OUTPOST, PAY_TYPE_OUTPOST);

			MSG_FL_LOG_WAR_CONTRIBUTION_GEAR tmContributionGaer;
			util::zero(&tmContributionGaer, sizeof(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR));
			(*itr)->GetGearContributioninfo(&tmContributionGaer);
			CAtumLogSender::SendLogMessageWarContributionGear(&tmContributionGaer);

			// 2010-06-01 by shcho, GLogDB 관련 -
			this->SendOutPostGLogEventParticipationRate(i_nMapIndex, (*itr)->GetSizeCharacterUIDContribution(), (*itr)->GetWarStartTime());

			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::PayCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl)
/// \brief		전쟁 보상 추가안 - 전략포인트 승리세력에게 보상 지급
/// \author		dhjin
/// \date		2008-12-24 ~ 2008-12-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PayCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl)
{
	mt_auto_lock mta(&m_mtvectCStrategyPointWarContributionCalculation);
	for (mtvectCStrategyPointWarContributionCalculation::iterator itr(m_mtvectCStrategyPointWarContributionCalculation.begin())
		; itr != m_mtvectCStrategyPointWarContributionCalculation.end(); itr++)
	{
		if (i_nMapIndex == (*itr)->GetMapIndex()
			&& PAY_MINIMUN_COUNT <= (*itr)->GetSizeCharacterUIDContribution())
		{
			// 2008-12-23 by dhjin, 개인 지급
			this->PaySendWarContribution(*itr, i_byWinInfl, PAY_WAR_CONTRIBUTION_BY_STRATEGYPOINT, PAY_TYPE_STRATEGYPOINT);

			MSG_FL_LOG_WAR_CONTRIBUTION_GEAR tmContributionGaer;
			util::zero(&tmContributionGaer, sizeof(MSG_FL_LOG_WAR_CONTRIBUTION_GEAR));
			(*itr)->GetGearContributioninfo(&tmContributionGaer);
			CAtumLogSender::SendLogMessageWarContributionGear(&tmContributionGaer);

			// 2010-06-01 by shcho, GLogDB 관련 -
			this->SendStrategyPointGLogEventParticipationRate(i_nMapIndex, (*itr)->GetSizeCharacterUIDContribution(), (*itr)->GetWarStartTime());

			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::PaySendWarContribution(CWarContributionCalculation * i_pWarContribution, BYTE i_byWinInfl, int i_nDefinePayWarType, BYTE i_byWarType)
/// \brief		전쟁 보상 추가안 - 개인 보상
/// \author		dhjin
/// \date		2008-12-24 ~ 2008-12-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldIOCP::PaySendWarContribution(CWarContributionCalculation * i_pWarContribution, BYTE i_byWinInfl, int i_nDefinePayWarType, BYTE i_byWarType)
{
	if (NULL == i_pWarContribution)
	{
		return;
	}

	UID64_t TotalContribution = max(1, i_pWarContribution->GetTotalContribution());		// 2009-01-30 by dhjin, 전쟁 보상 추가안 0으로 나누는 버그 수정.

	CFieldMapChannel *pFMChann = this->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(i_pWarContribution->GetMapIndex(), 0));
	CFieldMapChannel *pBackFMChann = this->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pFMChann->m_pFieldMapProject->m_nBeforeMapIndex, 0));
	CFieldMapChannel *pBackFMChann2 = this->GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX(pFMChann->m_pFieldMapProject->m_nBeforeMapIndex2, 0));

	mtMapCharacterUIDContribution * tmMapCharacterUIDContribution = i_pWarContribution->GetmtMapCharacterUIDContribution();

	mt_auto_lock mta(tmMapCharacterUIDContribution);		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - lock 잡아야 한다.

	for (mtMapCharacterUIDContribution::iterator itrC(tmMapCharacterUIDContribution->begin())
		; itrC != tmMapCharacterUIDContribution->end(); itrC++)
	{// 2008-12-23 by dhjin, 개인 지급 

		CFieldIOCPSocket * PayFISoc = this->GetFieldIOCPSocketByCharacterUniqueNumber(itrC->first);
		if (NULL != PayFISoc
			&& PayFISoc->IsValidCharacter(FALSE)		// 2009-03-03 by cmkwon, 전쟁 보상시 죽은 상태의 캐릭터에도 SPI 지급 - 전략포인트,전진기지전 보상
//			&& i_byWinInfl == PayFISoc->m_character.InfluenceType					// 2009-01-21 by dhjin, 전쟁 보상 추가안 수정 모든 세력 개인 보상 지급
&& (pFMChann == PayFISoc->GetCurrentFieldMapChannel()
	|| pBackFMChann == PayFISoc->GetCurrentFieldMapChannel()
	|| pBackFMChann2 == PayFISoc->GetCurrentFieldMapChannel())
			)
		{
			if (OTHER_INFL_STAY == itrC->second.CheckOtherInflStay)
			{// 상대세력맵에 머문 시간 최종 업데이트
				ATUM_DATE_TIME	CurrentTime;
				CurrentTime.SetCurrentDateTime();
				itrC->second.OtherInflStayTime += CurrentTime.GetTimeDiffTimeInMinutes(itrC->second.OtherInflStayStartTime);
				itrC->second.OtherInflStayStartTime.Reset();
			}

			UID32_t Pay;
			int TotalWarTime = max(1, i_pWarContribution->GetTotalWarTime());		// 2009-01-30 by dhjin, 전쟁 보상 추가안 0으로 나누는 버그 수정.
// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 아래와 같이 자신 세력의 유저수로 수정(기존에는 승리세력의 유저수로 되어 있었음)
//			UID64_t tmPay = ( (itrC->second.Contribution * i_nDefinePayWarType * i_pWarContribution->GetInflUserCount(i_byWinInfl) ) / TotalContribution )
//							+ ( ((itrC->second.OtherInflStayTime + itrC->second.DeathCount) * i_nDefinePayWarType) / (TotalWarTime + i_pWarContribution->GetInflDeathCount(i_byWinInfl)) );
			///////////////////////////////////////////////////////////////////////////////
			// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 아래와 같이 자신 세력의 유저수로 수정
			UID64_t tmPay = ((itrC->second.Contribution * i_nDefinePayWarType * i_pWarContribution->GetInflUserCount(PayFISoc->m_character.InfluenceType)) / TotalContribution)
				+ (((itrC->second.OtherInflStayTime + itrC->second.DeathCount) * i_nDefinePayWarType) / (TotalWarTime + i_pWarContribution->GetInflDeathCount(PayFISoc->m_character.InfluenceType)));

			// 2009-03-18 by cmkwon, 전쟁 보상 추가안 수정(개인 보상 최대값 설정) - 아래와 같이 수정, 전진기지전, 거점전
			if (PAY_TYPE_STRATEGYPOINT == i_byWarType)
			{
				tmPay = max(PAY_MINIMUN_BY_STRATEGYPOINT, tmPay);
				tmPay = min(PAY_MAXMUN_BY_STRATEGYPOINT, tmPay);
			}
			else
			{
				tmPay = max(PAY_MINIMUN_BY_OUTPOST, tmPay);
				tmPay = min(PAY_MAXMUN_BY_OUTPOST, tmPay);
			}

			Pay = tmPay;

			PayFISoc->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, Pay, IUT_WAR_CONTRIBUTION);
			CAtumLogSender::SendLogMessageWarContribution(PayFISoc->m_character.CharacterUniqueNumber, i_pWarContribution->GetMapIndex(), itrC->second.Contribution, Pay, i_byWarType);
		}
	}
}

void CFieldIOCP::ResetInfinityByResetCycle(ATUM_DATE_TIME *i_pCurDateTime) {
	// 인피 귀속 리셋 
	vectorInfinityModeInfo tmInfinityModeList;
	m_InfinityManager.GetInfinityModeInfo(&tmInfinityModeList);

	if (0 >= tmInfinityModeList.size()) {
		return;
	}

	vectorInfinityModeInfo::iterator itr = tmInfinityModeList.begin();
	for (; itr != tmInfinityModeList.end(); itr++) {
		if (i_pCurDateTime->Hour == itr->ResetCycle
			&& 0 == i_pCurDateTime->Minute) {
			if (FALSE == g_pFieldGlobal->IsArenaServer()) {
				// DB 리셋, 주의! 아레나 서버는 유저만 업뎃!!!
				m_pAtumDBManager->MakeAndEnqueueQuery(QT_ResetInfinityImpute, NULL, NULL, NULL, NULL, itr->InfinityModeUID);
			} // if(FALSE == g_pFieldGlobal->IsArenaServer()) {
			// 유저 리셋
			CFieldIOCPSocket *pSock = NULL;
			for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++) {
				pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
				if (pSock
					&& pSock->IsValidCharacter(FALSE)) {
					pSock->InfinityResetImpute(itr->InfinityModeUID);	// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 함수명 변경
				} // if (pSock
			} // for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++) {
		} // if(i_pCurDateTime->Hour == itr->ResetCycle
	} // vectorInfinityModeInfo::iterator itr = tmInfinityModeList.begin();
}

void CFieldIOCP::ResetInfinityByResetCycleByServerStart() {
	// 서버 시작시 모든 인피 귀속 정보 리셋 체크
	if (TRUE == g_pFieldGlobal->IsArenaServer()) {
		return;
	}

	vectorInfinityModeInfo tmInfinityModeList;
	m_InfinityManager.GetInfinityModeInfo(&tmInfinityModeList);
	if (0 >= tmInfinityModeList.size()) {
		return;
	}

	ATUM_DATE_TIME	CurTime;
	CurTime.SetCurrentDateTime();
	vectorInfinityModeInfo::iterator itr = tmInfinityModeList.begin();
	for (; itr != tmInfinityModeList.end(); itr++) {
		QPARAM_INFINITY_RESET_IMPUTE_SERVERSTART *pQParam = new QPARAM_INFINITY_RESET_IMPUTE_SERVERSTART;
		pQParam->InfinityModeUID = itr->InfinityModeUID;
		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 귀속 정보 리셋, 밑과 같이 수정
//		if(itr->ResetCycle > CurTime.Hour) {
//			pQParam->StartTime.SetDateTime(CurTime.Year, CurTime.Month, CurTime.Day, itr->ResetCycle, 0,0);
//		}
//		else {
//			pQParam->StartTime.SetDateTime(CurTime.Year, CurTime.Month, CurTime.Day, CurTime.Hour, 0,0);
//		}
//		pQParam->StartTime.Day -= 1;		// 하루를 빼서 하루 지난 귀속 정보를 초기화 한다.
		if (itr->ResetCycle < CurTime.Hour) {
			// 오늘 리셋 시간 전은 모두 초기화한다.
			pQParam->StartTime.SetDateTime(CurTime.Year, CurTime.Month, CurTime.Day, itr->ResetCycle, 0, 0);
		}
		else {
			// 2009-09-09 ~ 2010-02-01 by dhjin, 인피니티 - 인피니티 리셋 설정 시간 값 연산 오류 수정, 밑과 같이 수정
			// 전날 리셋 시간 전은 모두 초기화한다.
			// pQParam->StartTime.SetDateTime(CurTime.Year, CurTime.Month, (CurTime.Day-1), itr->ResetCycle, 0,0);
			pQParam->StartTime.SetDateTime(CurTime.Year, CurTime.Month, CurTime.Day, itr->ResetCycle, 0, 0);
			pQParam->StartTime.AddDateTime(0, 0, -1, 0, 0, 0);
		}
		m_pAtumDBManager->MakeAndEnqueueQuery(QT_ResetInfinityImputeByServerStart, NULL, 0, pQParam);
	} // for(; itr != tmInfinityModeList.end(); itr++) {
}

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
// BOOL CFieldIOCP::CheckTenderItem(ItemNum_t i_CheckItemNum) {
// 	// Tender Item인지 체크
// 	vectorTenderInfo::iterator itrTender = m_TenderInfoList.begin();
// 	for(; itrTender != m_TenderInfoList.end(); itrTender++){
// 		if(i_CheckItemNum == itrTender->TenderItemNum) {
// 			return TRUE;
// 		}
// 	}
// 
// 	return FALSE;
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldIOCP::SetTenderItemList(vectorTenderInfo *i_pTenderItemList)
{
	vectorTenderInfo::iterator itr(i_pTenderItemList->begin());
	for (; itr != i_pTenderItemList->end(); itr++)
	{
		TENDER_INFO	*pTenderInfo = &*itr;
		ITEM		*pItemInfo = this->GetItemInfo(pTenderInfo->TenderItemNum);
		if (NULL == pItemInfo)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldIOCP::SetTenderItemList# no such itemInfo !!, TenderUID(%d) TenderItemNum(%d) \r\n", pTenderInfo->TenderUID, pTenderInfo->TenderItemNum);
			continue;
		}

		pItemInfo->IsTenderDropItem = TRUE;
	}

	return TRUE;
}

// 2010-08-05 by dhjin, 버닝맵 - 
BURNING_MAP_INFO * CFieldIOCP::GetBurningMapInfo(MapIndex_t i_nMapIdx)
{
	BurningMapInfoList::iterator itr(m_BurningMapList.begin());
	for (; itr != m_BurningMapList.end(); itr++)
	{
		if (i_nMapIdx == itr->MapIndex)
		{
			return &*itr;
		}
	}

	return NULL;
}
// START 2011-11-03 by shcho, yedang 셧다운제 구현 - 체크 함수 추가
void CFieldIOCP::FindShutdownUserAndStartShutDownRoutin(ATUM_DATE_TIME *i_pCurDateTime)	// 현재 시간을 넣는다.
{
	// 11시 이후부터는 알림을 알리다가 12시가 되면 종료시키고 6시 이후부터는 정상 접속 되도록 한다.
	if (23 == i_pCurDateTime->Hour) // 12시가 아니다.
	{// 10분마다 알림처리를 한다. 모든유저 성인 미성년 상관없이...
		if (i_pCurDateTime->Minute == 10
			|| i_pCurDateTime->Minute == 20
			|| i_pCurDateTime->Minute == 30
			|| i_pCurDateTime->Minute == 40
			|| i_pCurDateTime->Minute == 50
			)
		{
			MessageType_t msgTy = T_FC_SHUTDOWNMINS_USER_ALTER;
			this->SendMessageToAllClients((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER, INFLUENCE_TYPE_ALL_MASK); // 종료전 10분마다 종료라고 보낸다.
		}
	}

	vectShutDownUserData::iterator itr(m_ShutDownData.begin());
	for (; itr != m_ShutDownData.end(); itr++)
	{
		// 		if(TRUE == g_pFieldGlobal->IsArenaServer()) // 아레나 서버이면 구분한다.
		// 		{ // 아레나 서버는 케릭터가 하나다. 한번만 보내면 된다.
		CFieldIOCPSocket* pfIocpsock = GetFieldIOCPSocketByCharacterUniqueNumber(itr->CharacterUniqueNumber[0]); // 소켓 못뱉음
		if (NULL != pfIocpsock)
		{
			// 성공 하면 주석 처리 
			//DbgOut("SHUTDOWNMINS[1]:ArenaUser ShutDownMins Check OK!!AcName[%s],ChrName[%s]\r\n", pfIocpsock->m_character.AccountName, pfIocpsock->m_character.CharacterName);
			DbgOut("SHUTDOWNMINS[1]:User ShutDownMins Check OK!!AcName[%s],ChrName[%s]\r\n", pfIocpsock->m_character.AccountName, pfIocpsock->m_character.CharacterName);
			if (23 == i_pCurDateTime->Hour) // 12시가 아니다.
			{// 10분마다 알림처리를 한다.
				if (i_pCurDateTime->Minute == 59)
				{
					pfIocpsock->SendAddMessageType(T_FC_SHUTDOWNMINS_USER_ALTER); // 종료전 1분전 종료라고 보낸다.
				}
			}


			if (6 > i_pCurDateTime->Hour) // 새벽 0시부터 새벽 6시 이전이다.
			{
				pfIocpsock->SendAddMessageType(T_FC_SHUTDOWNMINS_USER_ENDGAME);	// 이것을 보내면 게임 종료루틴을 서버 쪽으로 전송 하도록 한다.
			}
		}
		//	}
	// 		else
	// 		{	// 아레나 서버가 아닐떄는 세번 보내도록 한다.		
	// 			for(int i=0 ;  i<3 ; ++i)
	// 			{
	// 				CFieldIOCPSocket* pfIocpsock = GetFieldIOCPSocketByCharacterUniqueNumber(itr->CharacterUniqueNumber[i]); // 소켓 못뱉음
	// 
	// 				if(NULL!=pfIocpsock)
	// 				{
	// 					// 성공 하면 주석 처리 
	// 					DbgOut("SHUTDOWNMINS[2]:FieldServer User ShutDownMins Check OK!!AcName[%s],ChrName[%s]\r\n", pfIocpsock->m_character.AccountName, pfIocpsock->m_character.CharacterName);
	// 
	// 					if( 23 == i_pCurDateTime->Hour) // 12시가 아니다.
	// 					{// 10분마다 알림처리를 한다.
	// 						if(i_pCurDateTime->Minute==59)
	// 						{
	// 							pfIocpsock->SendAddMessageType(T_FC_SHUTDOWNMINS_USER_ALTER); // 종료전 1분전 종료라고 보낸다.
	// 						}	
	// 					}
	// 					
	// 					
	// 					if(6 > i_pCurDateTime->Hour) // 새벽 0시부터 새벽 6시 이전이다.
	// 					{
	// 						pfIocpsock->SendAddMessageType(T_FC_SHUTDOWNMINS_USER_ENDGAME);	// 이것을 보내면 게임 종료루틴을 서버 쪽으로 전송 하도록 한다.
	// 					}
	// 				}
	// 			}
	// 		}
	}
	if (6 == i_pCurDateTime->Hour) // 예당에서 웹페이지에서 12시 이후 미성년자 막는것이 실패하는 것을 대비하기 위한 코드
	{	// 새벽 6시가 되면   미성년자 정보를 모두 지운다.
		if (0 != m_ShutDownData.size())
		{ // 데이터가 있을 경우에만 처리해야 한다.
			m_ShutDownData.clear();
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify]ShutDOWNMINsDataClear!!\r\n");
		}
	}
}

// END 2011-11-03 by shcho, yedang 셧다운제 구현 - 체크 함수 추가


// 2012-11-29 by jhseol, OX이벤트 맵 워프제한
BYTE CFieldIOCP::ReversalMapWarpPossible(int i_nMapindex)
{
	if (0 < i_nMapindex && i_nMapindex < IS_MAP_WARP_POSSIBLE_INDEX)
	{
		m_bIsMapWarpPossible[i_nMapindex] = !m_bIsMapWarpPossible[i_nMapindex];
		return m_bIsMapWarpPossible[i_nMapindex];
	}
	return 2;
}

BOOL CFieldIOCP::IsMapWarpPossible(int i_nMapindex)
{
	if (0 < i_nMapindex && i_nMapindex < IS_MAP_WARP_POSSIBLE_INDEX)
	{
		return m_bIsMapWarpPossible[i_nMapindex];
	}
	return FALSE;
}
// end 2012-11-29 by jhseol, OX이벤트 맵 워프제한

///////////////////////////////////////////////////////////////////////////////
// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 워프제한
void CFieldIOCP::SetNextMapWarpPossible(MapIndex_t i_nMapIndex, BOOL i_bWarpPossible)
{
	if (0 < i_nMapIndex && i_nMapIndex < IS_MAP_WARP_POSSIBLE_INDEX)
	{
		m_bTriggerNextMapWarpPossible[i_nMapIndex] = i_bWarpPossible;
	}
}

BOOL CFieldIOCP::GetNextMapWarpPossible(MapIndex_t i_nMapIndex)
{
	return m_bTriggerNextMapWarpPossible[i_nMapIndex];
}
// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 워프제한

// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
BOOL CFieldIOCP::ItemEventSubTypeCheck(UID32_t i_nAUID, UID32_t i_nCUID, SITEM_EVENT_INFO i_SITEM_EVENT_INFO, DestParam_t i_SkipSubEventType /*= ITEM_EVENT_DES_EMPTY*/)
{
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	if (FALSE == i_SITEM_EVENT_INFO.SubEventType)
	{
		return TRUE;
	}
	// 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
	if (TRUE == g_pFieldGlobal->IsArenaServer())	// 아레나 서버에선 이용불가
	{
		return FALSE;
	}
	// end 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] ItemEventSubTypeCheck Step.Start : AUID(%8d) CUID(%8d) EventInfo(%3d|%d)\r\n",i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType);

	int FindSkipIndex = 0;
	for (int SkipIndex = 0; SkipIndex < SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT; SkipIndex++)		// 우선 스킵할 지점부터 찾는다.
	{
		if (ITEM_EVENT_DES_EMPTY != i_SkipSubEventType && i_SkipSubEventType == i_SITEM_EVENT_INFO.ArrDestParameter[SkipIndex])
		{
			FindSkipIndex = SkipIndex + 1;	// 스킵된 다음 녀석부터 찾아야 하니 인덱스에 +1
		}
	}
	if (FindSkipIndex == SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT)	// 앞에서 찾은 스킵인덱스 값이 MAX SIZE와 같다면 더이상 체크할 필요가 없음으로 무조건 지급
	{
		//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] ItemEventSubTypeCheck Step.End_1 : AUID(%8d) CUID(%8d) EventInfo(%3d|%d)\r\n",i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType);
		return TRUE;
	}

	int bBeforeStepPass = 0;	// 우선순위에 따라 지급조건에 문제가 없는지 확인할 변수
	for (int index = FindSkipIndex; index < SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT; index++)
	{
		if (ITEM_EVENT_DES_EMPTY == i_SITEM_EVENT_INFO.ArrDestParameter[index])
		{
			if (FindSkipIndex == index)	// 스킵된 다음부터 설정된 이벤트 타입이 없으면 체크가 완료 된 것임으로 선물지급
			{
				//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] ItemEventSubTypeCheck Step.End_2 : AUID(%8d) CUID(%8d) EventInfo(%3d|%d)\r\n",i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType);
				return TRUE;
			}
			else
			{
				break;
			}
		}
		else if (ITEM_EVENT_DES_PLAY_TIME == i_SITEM_EVENT_INFO.ArrDestParameter[index])
		{
			if (0 == index)	// 우선순위가 가장 높다는걸 의미, 그럼으로 바로 틱에 추가
			{
				InsertTickEvent(SM_TICK_EVENT_SUB_ITEM_EVENT, i_SITEM_EVENT_INFO.ArrParameterValue[index] * 60, i_SITEM_EVENT_INFO.ItemEventUID, i_nCUID, (void*)(i_SITEM_EVENT_INFO.ArrParameterValue[index] * 60));
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] InsertItemEventTick, AUID(%8d) CUID(%8d) EventInfo(%3d|%d) SubEventPlayTime(%3d)!\r\n"
					, i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType, i_SITEM_EVENT_INFO.ArrParameterValue[index]);
			}
			else				// 아니라면
			{					// 다른 우선순위의 체크가 정상인지 확인하여 틱에 추가
				if (FALSE == ITEM_EVENT_MASK_CHECK(bBeforeStepPass, ITEM_EVENT_STEP_PASS_MASK_FAIL))
				{
					InsertTickEvent(SM_TICK_EVENT_SUB_ITEM_EVENT, i_SITEM_EVENT_INFO.ArrParameterValue[index] * 60, i_SITEM_EVENT_INFO.ItemEventUID, i_nCUID, (void*)(i_SITEM_EVENT_INFO.ArrParameterValue[index] * 60));
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] InsertItemEventTick, AUID(%8d) CUID(%8d) EventInfo(%3d|%d) SubEventPlayTime(%3d)!\r\n"
						, i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType, i_SITEM_EVENT_INFO.ArrParameterValue[index]);
				}
			}
			//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] ItemEventSubTypeCheck Step.End_3 : AUID(%8d) CUID(%8d) EventInfo(%3d|%d)\r\n",i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType);
			return FALSE; // 틱에 추가가 됬�?안됬�?상관없이 플레이타임 이벤트 타입이 걸리면 아직 지급해 주면 안됨으로 무조건 FALSE 리턴
		}
		else if (ITEM_EVENT_DES_PLAY_REQUIRED_ITEM == i_SITEM_EVENT_INFO.ArrDestParameter[index])
		{
			int tmItemCount = 0;		// 보유한 수량을 누적시킬 변수
			int tmTermsCount = 0;		// 지급 기준이 되는 수량
			for (int i = 0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT; i++)
			{		// ITEM_EVENT_DES_PLAY_REQUIRED_ITEM 와 쌍을 이루는 조건 아이템의 수량정보 찾기
				if (ITEM_EVENT_DES_PLAY_REQUIRED_COUNT == i_SITEM_EVENT_INFO.ArrDestParameter[i])
				{
					tmTermsCount = i_SITEM_EVENT_INFO.ArrParameterValue[i];
				}
			}

			if (0 >= tmTermsCount)
			{
				bBeforeStepPass |= ITEM_EVENT_STEP_PASS_MASK_FAIL;		// 수량이 잘못됨 통과 하지 못함
// 				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error1] InsertItemEventTick, AUID(%8d) CUID(%8d) EventInfo(%3d|%d) REQUIRED_COUNT_ERROR SubEventInfo(%s|%d|%d)!\r\n"
// 					, i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType
// 					, i_SITEM_EVENT_INFO.GetDestParameterStr(i_SITEM_EVENT_INFO.ArrDestParameter[index]), i_SITEM_EVENT_INFO.ArrParameterValue[index], tmTermsCount);
			}
			else
			{
				CFieldIOCPSocket *pSock = GetFieldIOCPSocketByCharacterUniqueNumber(i_nCUID, TRUE);		// 소켓 검사	// 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
				if (NULL != pSock)
				{
					mt_auto_lock igLock(&(pSock->m_ItemManager.m_mapItemGeneral));
					mtmapUID2ItemGeneral::iterator itr = pSock->m_ItemManager.m_mapItemGeneral.begin();
					for (; pSock->m_ItemManager.m_mapItemGeneral.end() != itr; itr++)	// 보유한 아이템 조회
					{
						ITEM_GENERAL *pItemG = itr->second;
						if (NULL != pItemG)		// NULL 이 아니고
						{
							if (ITEM_IN_CHARACTER == pItemG->ItemStorage)		// 케릭터 인벤이 있어야 하며
							{
								if (i_SITEM_EVENT_INFO.ArrParameterValue[index] == pItemG->ItemNum)	// 체크할 아이템 번호와 같으면
								{
									if (IS_COUNTABLE_ITEM(pItemG->Kind))
									{
										tmItemCount += pItemG->CurrentCount;		// 카운터블일경우 쌓인 수량을 더하고
									}
									else
									{
										tmItemCount++;								// 그외의 아아템이면 +1
									}
								}
							}
						}
					}

					if (tmTermsCount > tmItemCount)		// 조회한 아이템의 수가 이벤트 조건 수량보다 작으면
					{
						bBeforeStepPass |= ITEM_EVENT_STEP_PASS_MASK_FAIL;			// 통과 하지 못함
// 						g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error2] InsertItemEventTick, AUID(%8d) CUID(%8d) EventInfo(%3d|%d) SubEventInfo(%s|%d|%d) GetCount(%d)!\r\n"
// 							, i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType
// 							, i_SITEM_EVENT_INFO.GetDestParameterStr(i_SITEM_EVENT_INFO.ArrDestParameter[index]), i_SITEM_EVENT_INFO.ArrParameterValue[index], tmTermsCount, tmItemCount);
					}
					else
					{
						bBeforeStepPass |= ITEM_EVENT_STEP_PASS_MASK_SUCCESS;
						// 						g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] InsertItemEventTick, AUID(%8d) CUID(%8d) EventInfo(%3d|%d) SubEventInfo(%s|%d|%d) GetCount(%d)!\r\n"
						// 							, i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType
						// 							, i_SITEM_EVENT_INFO.GetDestParameterStr(i_SITEM_EVENT_INFO.ArrDestParameter[index]), i_SITEM_EVENT_INFO.ArrParameterValue[index], tmTermsCount, tmItemCount);
					}
				}
				else
				{
					bBeforeStepPass |= ITEM_EVENT_STEP_PASS_MASK_FAIL;			// 소켓 오류 통과 하지 못함
// 					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error3] InsertItemEventTick, AUID(%8d) CUID(%8d) EventInfo(%3d|%d) Sock Is NULL!\r\n"
// 						, i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType);
				}
			}
		}
	}
	{
		//////////////////////////////////////////////////////////////////////////
		// 개발 참고 노트. #001
		// 나중에 서브타입이 추가되면 {}문 위에 이어서 else if {}문을 추가한 후에 지급 조건 채크하고
		// 조건 검사에 따라 bBeforeStepPass 여기에 성공과 실패를 or연산 하면 된다
		//////////////////////////////////////////////////////////////////////////
	}

	if (TRUE == ITEM_EVENT_MASK_CHECK(bBeforeStepPass, ITEM_EVENT_STEP_PASS_MASK_FAIL))		// 실패한 흔적이 있으면 무조건 아이템 지급 안함
	{
		//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] ItemEventSubTypeCheck Step.End_4 : AUID(%8d) CUID(%8d) EventInfo(%3d|%d)\r\n",i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType);
		return FALSE;
	}
	else if (TRUE == ITEM_EVENT_MASK_CHECK(bBeforeStepPass, ITEM_EVENT_STEP_PASS_MASK_SUCCESS))		// 실패한 흔적없이 없고 성공한 기록이 있으면 아이템 지급
	{
		//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] ItemEventSubTypeCheck Step.End_5 : AUID(%8d) CUID(%8d) EventInfo(%3d|%d)\r\n",i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType);
		return TRUE;
	}
	else			// 이도저도 아니면 그냥 지급 안함.
	{
		//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] ItemEventSubTypeCheck Step.End_6 : AUID(%8d) CUID(%8d) EventInfo(%3d|%d)\r\n",i_nAUID, i_nCUID, i_SITEM_EVENT_INFO.ItemEventUID, i_SITEM_EVENT_INFO.ItemEventType);
		return FALSE;
	}
#else
	return TRUE;
#endif
}
// end 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경

// 2013-03-29 by jhseol, 아이템 이벤트 - 플레이 시간별 선물지급
void CFieldIOCP::InsertTickEvent(INT i_TickEventType, INT i_nTimeIntervalInSeconds, INT i_nTickEventParam1 /* = 0 */, INT i_nTickEventParam2 /* = 0 */, void *i_vpTickEventParam1 /* = NULL */, void *i_vpTickEventParam2 /* = NULL */)
{
	if (NULL != m_pFieldTickManager)
	{
		m_pFieldTickManager->InsertTickEvent(i_TickEventType, i_nTimeIntervalInSeconds, i_nTickEventParam1, i_nTickEventParam2, i_vpTickEventParam1, i_vpTickEventParam2);
	}
}
// end 2013-03-29 by jhseol, 아이템 이벤트 - 플레이 시간별 선물지급


// 2013-05-09 by hskim, 세력 포인트 개선	
BOOL CFieldIOCP::DeleteBuffSkillAllCharacter(BYTE i_byInfluenceMask, DestParam_t desParam)
{
	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < GetArrayClientSize(); i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			&& pSock->IsCheckInfluenceTypeANDSocketType(i_byInfluenceMask, ST_CLIENT_TYPE))
		{
			if (IS_MAP_INFLUENCE_ARENA(pSock->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW()))
			{
				continue;
			}

			pSock->ReleaseBuffSkillByDesParam(desParam);
		}
	}

	return TRUE;
}

BOOL CFieldIOCP::InsertBuffSkillAllCharacter(BYTE i_byInfluenceMask, INT nItemNum)
{
	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < GetArrayClientSize(); i++)
	{
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock
			&& pSock->IsValidCharacter(FALSE)
			&& pSock->IsCheckInfluenceTypeANDSocketType(i_byInfluenceMask, ST_CLIENT_TYPE))
		{
			if (IS_MAP_INFLUENCE_ARENA(pSock->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW()))
			{
				continue;
			}
			// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - 버프적용시 세력포인트를 미리 보내주기
			if (TURN_AROUND_BUFF_SKILL_1ST == nItemNum || TURN_AROUND_BUFF_SKILL_2ND == nItemNum)
			{
				INIT_MSG_WITH_BUFFER(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_POINT, T_FC_WAR_INFLUENCE_CONSECUTIVE_POINT, pInflPoint, pInflPointSendBuf);
				int CurrentDiffContributionPoint = m_InflWarManager.GetTurnAroundPoint();
				if ((CurrentDiffContributionPoint < 0 && TRUE == COMPARE_INFLUENCE(pSock->m_character.InfluenceType, INFLUENCE_TYPE_ANI)) ||
					(CurrentDiffContributionPoint > 0 && TRUE == COMPARE_INFLUENCE(pSock->m_character.InfluenceType, INFLUENCE_TYPE_VCN)))
				{
					pInflPoint->PVPBuffPercent = m_InflWarManager.GetPVPBuffPercent(CurrentDiffContributionPoint);
				}
				pSock->SendAddData(pInflPointSendBuf, MSG_SIZE(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_POINT));
			}
			// end 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - 버프적용시 세력포인트를 미리 보내주기
			pSock->ApplyBuffSkill(nItemNum);
		}
	}

	return TRUE;
}
// end 2013-05-09 by hskim, 세력 포인트 개선	

// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
void CFieldIOCP::LoadItemSecurityMaximunUID()
{
	QPARAM_TEMPORARY_SYSTEM_INFOMATION *pTempSystem = new QPARAM_TEMPORARY_SYSTEM_INFOMATION;
	util::zero(pTempSystem, sizeof(QPARAM_TEMPORARY_SYSTEM_INFOMATION));
	pTempSystem->Type = TEMPORARY_SYSTEM_INFOMATION_INDEX_ITEMSECURITY;
	m_pAtumDBManager->MakeAndEnqueueQuery(QT_GetTemporarySystemInfomation, NULL, 0, pTempSystem);
}

UID64_t CFieldIOCP::GeneratorItemSecurityCheckSum(UID64_t nUniqueNumber, INT ItemCount)
{
	return ((~(((GetItemSecurityMaximunUID() + nUniqueNumber + (nUniqueNumber * (nUniqueNumber << 4) * (nUniqueNumber << 8) * (nUniqueNumber << 16) * (nUniqueNumber << 32) * (nUniqueNumber << 64)) + 0X0f2233FFf2233cc * 0x0f8212ab1026) ^ ItemCount)) ^ 0X1f22bbcc1f22bbcc) & 0x7fffffffffffffff);
}
// end 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지

void CFieldIOCP::GetUserCountByInfluence(int &aniCount, int &bcuCount) {
	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++) {
		pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
		if (pSock && pSock->IsValidCharacter(FALSE)) {
			if (pSock->IsCheckInfluenceTypeANDSocketType(INFLUENCE_TYPE_VCN, ST_CLIENT_TYPE)) {
				bcuCount++;
			}
			else if (pSock->IsCheckInfluenceTypeANDSocketType(INFLUENCE_TYPE_ANI, ST_CLIENT_TYPE)) {
				aniCount++;
			}
		}
	}
}

// 2015-06-13 Future, Cashing of KillCount in case of User disconnect
#if S_BONUS_KILL_SYSTEM_RENEWAL
void CFieldIOCP::CacheCharacterKillCount(UID32_t characterUID, INT count)
{
	// Create Cache Object
	KILLCOUNT_CACHE_DATA cacheData = { characterUID, count, ATUM_DATE_TIME(true) };

	// Add or alter the map
	m_mtmapCharacterKillCounts.lock();
	m_mtmapCharacterKillCounts[characterUID] = cacheData;
	m_mtmapCharacterKillCounts.unlock();
}

INT CFieldIOCP::GetCachedKillCount(UID32_t characterUID)
{
	// Get Time to compare with
	ATUM_DATE_TIME allowedCacheTime(true);
	allowedCacheTime.AddDateTime(0, 0, 0, 0, -KILLCOUNT_CACHE_TIME_IN_MINUTES, 0);

	// Find and return Cached Killcount
	KILLCOUNT_CACHE_DATA* cacheData = m_mtmapCharacterKillCounts.findLock_Ptr(characterUID);
	if (!cacheData || cacheData->CacheTime < allowedCacheTime)
		return 0;
	else
		return cacheData->KillCount;
}

void CFieldIOCP::CleanKillCountCache()
{
	// Get Time to compare with
	ATUM_DATE_TIME allowedCacheTime(true);
	allowedCacheTime.AddDateTime(0, 0, 0, 0, -KILLCOUNT_CACHE_TIME_IN_MINUTES, 0);

	// Clean all expired KillCount Cache Data
	m_mtmapCharacterKillCounts.lock();
	mt_map<UID32_t, KILLCOUNT_CACHE_DATA>::iterator itr = m_mtmapCharacterKillCounts.begin();
	while (itr != m_mtmapCharacterKillCounts.end())
	{
		if (itr->second.CacheTime < allowedCacheTime)
		{
			// Cache Time expired, delete the Cache Entry
			itr = m_mtmapCharacterKillCounts.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	m_mtmapCharacterKillCounts.unlock();
}

#endif // S_BONUS_KILL_SYSTEM_RENEWAL
// end 2015-06-13 Future, Cashing of KillCount in case of User disconnect

// 2015-11-24 Future, checks for IP availability
bool CFieldIOCP::PeerIPAvailable(CFieldIOCPSocket* pCallingSocket)
{
	// Get caller IP
	const char* callerIP = pCallingSocket->GetPeerIP();
	if (!callerIP || strlen(callerIP) < 0)
		return false;

	// For all clients with an IP Address available
	for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
	{
		const char* destStrIp = NULL;
		CFieldIOCPSocket* pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);

		if (pSock											// Socket found
			&& pSock != pCallingSocket						// Socket is not the caller
			&& pSock->m_PeerSocketType == ST_CLIENT_TYPE	// Socket is a game client
			&& !COMPARE_RACE(pSock->GetCharacter()->Race, RACE_ACCOUNT_TYPE_MASK)	// Ignore Staff Member IPs
			&& (destStrIp = pSock->GetPeerIP())				// Socket has a valid peer IP
			&& strlen(destStrIp) > 0)						// Socket has a valid peer IP
		{
			if (strcmp(destStrIp, callerIP) == 0)
			{
				// IP duplicate found, return not available == false
				return false;
			}

		}
	}

	// Nothing was found, return available == true
	return true;
}

#ifdef S_IP_UNIQUE_CONNECTION
// Checks whether multiple IPs are restricted (true) or allowed (false)
BOOL CFieldIOCP::MultipleIPsRestricted()
{
	return m_bMultipleIPRestriction;
}

// Enables Multiple Restriction on the server
// Returns: Whether the setting was successful or not
BOOL CFieldIOCP::EnableMultipleIPRestriction(CFieldIOCPSocket* pCaller, BOOL enable /*= TRUE*/)
{
	if (enable == m_bMultipleIPRestriction)
	{
		// Same value, nothing needs to be changed
		pCaller->SendString128(STRING_128_ADMIN_CMD, "The Server is currently in this Mode already");
		return FALSE;
	}

	if (enable)
	{
		// Kick users with duplicate IPs which are not allowed to connect multiple times
		for (int i = (CLIENT_INDEX_START_NUM); i < m_dwArrayClientSize; i++)
		{
			CFieldIOCPSocket* pSock = (CFieldIOCPSocket*)GetIOCPSocket(i);
			if (pSock											// Socket found
				&& pSock->m_PeerSocketType == ST_CLIENT_TYPE	// Socket is a game client
				&& !COMPARE_RACE(pSock->GetCharacter()->Race, RACE_ACCOUNT_TYPE_MASK)	// Never kick Staff Members!!!
				&& pSock->GetPeerIP()							// Socket has a valid peer IP
				&& strlen(pSock->GetPeerIP()) > 0)				// Socket has a valid peer IP
			{
				if (!PeerIPAvailable(pSock))
				{
					// Socket has a duplicate IP with someone else
					// Request multiple IP status and kick him if he is not allowed to be connected with multiple IPs
					QPARAM_GET_CHARACTER_MULTIPLE_IP_STATUS* pQParam = new QPARAM_GET_CHARACTER_MULTIPLE_IP_STATUS;
					pQParam->AdminIssuerCharacterUID = pCaller->GetCharacter()->CharacterUniqueNumber;	// Issuing Character is the caller
					util::strncpy(pQParam->AccountName, pSock->GetCharacter()->AccountName, SIZE_MAX_ACCOUNT_NAME);
					m_pAtumDBManager->MakeAndEnqueueQuery(QT_GetCharacterMultipleIPStatus, pSock, pSock->GetCharacter()->AccountUniqueNumber, pQParam);
				}
			}
		}

		// Enable the Multiple IP Restriction System for future connections
		m_bMultipleIPRestriction = TRUE;
		pCaller->SendString128(STRING_128_ADMIN_CMD, "Multiple IP Restriction Mode Changed to: \\gENABLED\\g");
	}
	else
	{
		// Disable the Multiple IP Restriction System for future connections
		m_bMultipleIPRestriction = FALSE;
		pCaller->SendString128(STRING_128_ADMIN_CMD, "Multiple IP Restriction Mode Changed to: \\rDISABLED\\r");
	}

	return TRUE;
}
#endif // S_IP_UNIQUE_CONNECTION

#if S_BONUS_KILL_SYSTEM_RENEWAL
void CFieldIOCP::CleanKillCountCacheFromCharacter(UID32_t fromCharacterUID)
{
	m_mtmapCharacterKillCounts.deleteLock(fromCharacterUID);
}
#endif


