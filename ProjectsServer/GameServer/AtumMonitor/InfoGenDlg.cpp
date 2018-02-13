// InfoGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "InfoGenDlg.h"
#include "AtumDBManager.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "atumparam.h"
#include "ODBCStatement.h"
#include "selectserverdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoGenDlg dialog


CInfoGenDlg::CInfoGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoGenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoGenDlg)
	m_szInfoGenOutPath = _T("");
	//}}AFX_DATA_INIT
}


void CInfoGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoGenDlg)
	DDX_Text(pDX, IDC_EDIT_INFO_OUTPUT_PATH, m_szInfoGenOutPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoGenDlg, CDialog)
	//{{AFX_MSG_MAP(CInfoGenDlg)
	ON_BN_CLICKED(IDC_BUTTON_IG_ITEM, OnButtonIgItem)
	ON_BN_CLICKED(IDC_BUTTON_IG_MONSTER, OnButtonIgMonster)
	ON_BN_CLICKED(IDC_BUTTON_IG_MAPOBJECT, OnButtonIgMapobject)
	ON_BN_CLICKED(IDC_BUTTON_IG_ALL, OnButtonIgAll)
//	ON_BN_CLICKED(IDC_BUTTON_IG_QUEST, OnButtonIgQuest)
//	ON_BN_CLICKED(IDC_BUTTON_IG_QUEST_NPC_TALK, OnButtonIgQuestNpcTalk)
	ON_BN_CLICKED(IDC_BUTTON_IG_RARE_FIX, OnButtonIgRareFix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoGenDlg message handlers

void CInfoGenDlg::OnButtonIgMonster()
{
	// OnButtonCheckAll()로 통합
}

void CInfoGenDlg::OnButtonIgItem()
{
	// OnButtonCheckAll()로 통합
}

void CInfoGenDlg::OnButtonIgMapobject()
{
	// OnButtonCheckAll()로 통합
}

// start 2011-05-13 by hskim, 인피니티 3차
//typedef enum 
//{
//	DB_ITEM,
//	DB_MONSTER_INFO,
//	DB_MAPOBJECT,
//	DB_RARE_ITEM,
//	DB_BUILDINGNPC,
//	DB_GUILD_MARK,
//	DB_MAP_INFO,
//	DB_MIXING_INFO,
//	DB_MYSTERY_ITEM_DROP,
//	DB_LUCKYMACHINE,		// 2009-03-03 by dhjin, 럭키머신 수정안
//	DB_INVOKINGWEARITEM_DPNUM,				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
//	DB_INVOKINGWEARITEM_DPNUM_BY_USE,		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
//	DB_BURNING_MAP,			// 2010-08-05 by dhjin, 버닝맵 - 
//
//	DB_PET_BASEDATA,						// 2010-06-15 by shcho&hslee 펫시스템 - 펫 기본 정보.
//	DB_PET_LEVELDATA,						// 2010-06-15 by shcho&hslee 펫시스템 - 펫 레벨업 관련정보.
//	DB_PET_OPERATOR,						// 2010-06-15 by shcho&hslee 펫시스템 - 펫 오퍼레이터 관련정보.
//	DB_DISSOLUTIONITEM,						// 2010-08-31 by shcho&jskim 아이템용해 시스템 -					
//} DB_TYPE;
// end 2011-05-13 by hskim, 인피니티 3차

void CInfoGenDlg::OnButtonIgAll()
{
	// TODO: Add your control notification handler code here
	UpdateData();

//	OnButtonIgItem(); 
//	OnButtonIgMonster(); 
//	OnButtonIgMapobject();

	// open file
	HANDLE	hFile;
	DWORD	dwBytesWritten = 0;
	DWORD	dwPtr= 0;
	INT		nObjects = 0;
	DB_TYPE	dbType;

	hFile = CreateFile(m_szInfoGenOutPath+"\\omi.tex", GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
							CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile == INVALID_HANDLE_VALUE)
	{
		// can not open file
		MessageBox("Can not open file!!");
		return;
	}

	CAtumMonitorApp *pMonitorApp = (CAtumMonitorApp*)(AfxGetApp());

	CSelectServerDlg dlg(pMonitorApp->m_strGameServerName);
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	pMonitorApp->WriteProfile();

	GAME_SERVER_INFO_FOR_ADMIN serverInfo = dlg.m_GameServerInfoForAdmin;

	const char *szDBIP = serverInfo.DBIP;
	const int nDBPort = serverInfo.DBPort;
	const char *szDatabaseName = serverInfo.DBName;
	const char *szDBUID = serverInfo.DBUID;
	const char *szDBPassword = serverInfo.DBPWD;

	CODBCStatement odbcStmt;
	BOOL bRet = odbcStmt.Init(szDBIP, nDBPort, szDatabaseName, szDBUID, szDBPassword, GetSafeHwnd());
	if (!bRet) {
	// todo : fix this hardcoding
	//if (!odbcStmt.Init("127.0.0.1", 1433, "atum2_db_1", "atum", "DB:atum", GetSafeHwnd())) {
		MessageBox("Can not connect to DB Server!!");
		return;
	}

	// item info
	ez_map<INT, ITEM> mapItemInfo;
	nObjects = CAtumDBHelper::LoadItemInfo(&odbcStmt, &mapItemInfo);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadItemInfo error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = mapItemInfo.size();
	dbType = DB_ITEM;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	ez_map<INT, ITEM>::iterator itrItemInfo = mapItemInfo.begin();
	while (itrItemInfo != mapItemInfo.end())
	{
		ITEM *pItem = &itrItemInfo->second;
		WriteFile(hFile, (LPCVOID)pItem, sizeof(ITEM), &dwBytesWritten, NULL);
		itrItemInfo++;
	}

	// monster info
	vector<MEX_MONSTER_INFO> vecMexMonsterInfo;
	nObjects = CAtumDBHelper::Load_MEX_MONSTER_INFO(&odbcStmt, &vecMexMonsterInfo);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("Load_MEX_MONSTER_INFO error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = vecMexMonsterInfo.size();
	dbType = DB_MONSTER_INFO;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	vector<MEX_MONSTER_INFO>::iterator itrMexMonsterInfo = vecMexMonsterInfo.begin();
	while (itrMexMonsterInfo != vecMexMonsterInfo.end())
	{
		MEX_MONSTER_INFO *pMonsterInfo = &(*itrMexMonsterInfo);
		WriteFile(hFile, (LPCVOID)pMonsterInfo, sizeof(MEX_MONSTER_INFO), &dwBytesWritten, NULL);
		itrMexMonsterInfo++;
	}

	// mapobjectinfo
	ez_map<INT, MAPOBJECTINFO> mapMapObjectInfo;
	nObjects = CAtumDBHelper::LoadMapObjectInfo(&odbcStmt, &mapMapObjectInfo);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadMapObjectInfo error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = mapMapObjectInfo.size();
	dbType = DB_MAPOBJECT;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	ez_map<INT, MAPOBJECTINFO>::iterator itrMapObject = mapMapObjectInfo.begin();
	while (itrMapObject != mapMapObjectInfo.end())
	{
		MAPOBJECTINFO *pMapObjectInfo = &itrMapObject->second;
		WriteFile(hFile, (LPCVOID)pMapObjectInfo, sizeof(MAPOBJECTINFO), &dwBytesWritten, NULL);
		itrMapObject++;
	}

	// rare item info
	ez_map<INT, RARE_ITEM_INFO> mapRareItemInfo;
	nObjects = CAtumDBHelper::LoadRareItemInfo(&odbcStmt, &mapRareItemInfo);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadRareItemInfo error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = mapRareItemInfo.size();
	dbType = DB_RARE_ITEM;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	ez_map<INT, RARE_ITEM_INFO>::iterator itrRareItemInfo = mapRareItemInfo.begin();
	while (itrRareItemInfo != mapRareItemInfo.end())
	{
		RARE_ITEM_INFO *pRareItemInfo = &itrRareItemInfo->second;
		WriteFile(hFile, (LPCVOID)pRareItemInfo, sizeof(RARE_ITEM_INFO), &dwBytesWritten, NULL);
		itrRareItemInfo++;
	}

	// buildingnpc
	vector<BUILDINGNPC> vecBuildingNPC;
	nObjects = CAtumDBHelper::LoadAllBUILDINGNPC(&odbcStmt, &vecBuildingNPC);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadAllBUILDINGNPC error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = vecBuildingNPC.size();
	dbType = DB_BUILDINGNPC;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	vector<BUILDINGNPC>::iterator itrBuildingNPC = vecBuildingNPC.begin();
	while (itrBuildingNPC != vecBuildingNPC.end())
	{
		BUILDINGNPC *pBuildingNPC = &(*itrBuildingNPC);
		WriteFile(hFile, (LPCVOID)pBuildingNPC, sizeof(BUILDINGNPC), &dwBytesWritten, NULL);
		itrBuildingNPC++;
	}

	// map info
	vectorMAP_INFO mapMapInfo;
	nObjects = CAtumDBHelper::LoadMapInfo(&odbcStmt, &mapMapInfo);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadMapInfo error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = mapMapInfo.size();
	dbType = DB_MAP_INFO;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
		
	for(int i=0; i < mapMapInfo.size(); i++)
	{		
		WriteFile(hFile, (LPCVOID)&mapMapInfo[i], sizeof(MAP_INFO), &dwBytesWritten, NULL);		
	}

	// mixing info
	vector<ITEM_MIXING_INFO> vectorMixingInfo;
	nObjects = CAtumDBHelper::LoadItemMixingInfo(&odbcStmt, &vectorMixingInfo);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadItemMixingInfo error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = vectorMixingInfo.size();
	dbType = DB_MIXING_INFO;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	vector<ITEM_MIXING_INFO>::iterator itrMixingInfo = vectorMixingInfo.begin();
	while (itrMixingInfo != vectorMixingInfo.end())
	{
		ITEM_MIXING_INFO *pMixingInfo = &(*itrMixingInfo);
		WriteFile(hFile, (LPCVOID)pMixingInfo, sizeof(ITEM_MIXING_INFO), &dwBytesWritten, NULL);
		itrMixingInfo++;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-11-04 by dhjin, 럭키머신
	mmapINT2MYSTERY_ITEM_DROP	m_mapMysteryItemDropList;
	nObjects = CAtumDBHelper::LoadMysteryItemDropInfoByOmi(&m_mapMysteryItemDropList, &odbcStmt, &mapItemInfo);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadMysteryItemDropInfoByOmi error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = m_mapMysteryItemDropList.size();
	dbType = DB_MYSTERY_ITEM_DROP;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	mmapINT2MYSTERY_ITEM_DROP::iterator itrMysteryItemDropList = m_mapMysteryItemDropList.begin();
	while (itrMysteryItemDropList != m_mapMysteryItemDropList.end())
	{
		MYSTERY_ITEM_DROP	*pMysteryItemDropInfo	= &itrMysteryItemDropList->second;
		WriteFile(hFile, (LPCVOID)pMysteryItemDropInfo, sizeof(MYSTERY_ITEM_DROP), &dwBytesWritten, NULL);
		itrMysteryItemDropList++;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2009-03-03 by dhjin, 럭키머신 수정안
	vector<LUCKY_MACHINE_OMI> vectLuckyMachine;
	nObjects = CAtumDBHelper::LoadLuckyMachineOmi(&odbcStmt, &vectLuckyMachine);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadLuckyMachineOmi error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = vectLuckyMachine.size();
	dbType = DB_LUCKYMACHINE;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	vector<LUCKY_MACHINE_OMI>::iterator itrLuckyMachine = vectLuckyMachine.begin();
	while (itrLuckyMachine != vectLuckyMachine.end())
	{
		LUCKY_MACHINE_OMI *pLuckyMachine = &(*itrLuckyMachine);
		WriteFile(hFile, (LPCVOID)pLuckyMachine, sizeof(LUCKY_MACHINE_OMI), &dwBytesWritten, NULL);
		itrLuckyMachine++;
	}	

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	InvokingWearItemDestParamList InvokingWearItemDestParamNumList;
	nObjects = CAtumDBHelper::LoadInvokingWearItemDestParamNum(&odbcStmt, &InvokingWearItemDestParamNumList);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadInvokingWearItemDestParamNum error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = InvokingWearItemDestParamNumList.size();
	dbType = DB_INVOKINGWEARITEM_DPNUM;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	InvokingWearItemDestParamList::iterator itrInvokingWearItemDestParamNum = InvokingWearItemDestParamNumList.begin();
	while (itrInvokingWearItemDestParamNum != InvokingWearItemDestParamNumList.end())
	{
		INVOKING_WEAR_ITEM_DESTPARAM *pInvokingWearItemDestParamNum = &(*itrInvokingWearItemDestParamNum);
		WriteFile(hFile, (LPCVOID)pInvokingWearItemDestParamNum, sizeof(INVOKING_WEAR_ITEM_DESTPARAM), &dwBytesWritten, NULL);
		itrInvokingWearItemDestParamNum++;
	}

	InvokingWearItemDestParamList InvokingWearItemDestParamNumByUseList;
	nObjects = CAtumDBHelper::LoadInvokingWearItemDestParamNumByUse(&odbcStmt, &InvokingWearItemDestParamNumByUseList);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadInvokingWearItemDestParamNumByUse error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = InvokingWearItemDestParamNumByUseList.size();
	dbType = DB_INVOKINGWEARITEM_DPNUM_BY_USE;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	InvokingWearItemDestParamList::iterator itrInvokingWearItemDestParamNumByUse = InvokingWearItemDestParamNumByUseList.begin();
	while (itrInvokingWearItemDestParamNumByUse != InvokingWearItemDestParamNumByUseList.end())
	{
		INVOKING_WEAR_ITEM_DESTPARAM *pInvokingWearItemDestParamNumByUse = &(*itrInvokingWearItemDestParamNumByUse);
		WriteFile(hFile, (LPCVOID)pInvokingWearItemDestParamNumByUse, sizeof(INVOKING_WEAR_ITEM_DESTPARAM), &dwBytesWritten, NULL);
		itrInvokingWearItemDestParamNumByUse++;
	}

	// 2010-08-05 by dhjin, 버닝맵 - 
	BurningMapInfoList		BurningMapInfo;
	nObjects = CAtumDBHelper::LoadBurningMap(&odbcStmt, &BurningMapInfo);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadBurningMap error from DB!!, Please check DB Schema.");
		return;
	}
	nObjects = BurningMapInfo.size();
	dbType = DB_BURNING_MAP;
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
	BurningMapInfoList::iterator itrBurningMapInfo = BurningMapInfo.begin();
	while (itrBurningMapInfo != BurningMapInfo.end())
	{
		BURNING_MAP_INFO *pBurningMapInfo = &(*itrBurningMapInfo);
		WriteFile(hFile, (LPCVOID)pBurningMapInfo, sizeof(BURNING_MAP_INFO), &dwBytesWritten, NULL);
		itrBurningMapInfo++;
	}
	// end, 2010-08-05 by dhjin, 버닝맵 - 

	// 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업. - 펫 기본 정보 기록.
	vectorPetDataInfo temp_vecPetBaseDataList;
	
	dbType = DB_PET_BASEDATA;
	nObjects = CAtumDBHelper::DBLoadPetBaseDataInfo( &odbcStmt,  &temp_vecPetBaseDataList );
	
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(DB_TYPE), &dwBytesWritten, NULL);

	vectorPetDataInfo::iterator PetData_iter = temp_vecPetBaseDataList.begin();
	tPET_BASEDATA *pPetBaseData = NULL;

	for( ; PetData_iter != temp_vecPetBaseDataList.end() ; ++PetData_iter )
	{
		pPetBaseData = &PetData_iter->BaseData;
		
		WriteFile(hFile, (LPCVOID)pPetBaseData, sizeof(tPET_BASEDATA), &dwBytesWritten, NULL);
	}
	//..

	// 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업. - 펫 레벨 정보 기록. ( 기본 정보(x) * 레벨(n)만큼 )
	dbType = DB_PET_LEVELDATA;
	nObjects = CAtumDBHelper::DBLoadPetLevelDataInfo( &odbcStmt, &temp_vecPetBaseDataList );
	
	vectorPetLevelData::iterator PetLevel_iter;
	tPET_LEVEL_DATA *pPetLevelData = NULL;

	PetData_iter = temp_vecPetBaseDataList.begin();

	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	
	for ( ; PetData_iter != temp_vecPetBaseDataList.end(); ++PetData_iter )
	{		
		for(PetLevel_iter = PetData_iter->vectPetLevelDataInfo.begin(); PetLevel_iter != PetData_iter->vectPetLevelDataInfo.end(); ++PetLevel_iter )
		{	
			pPetLevelData = &(*PetLevel_iter);
			WriteFile(hFile, (LPCVOID)pPetLevelData, sizeof(tPET_LEVEL_DATA), &dwBytesWritten, NULL);
		}
	}
	//..

// 	nObjects = CAtumDBHelper::DBLoadPetBaseDataInfo ( &odbcStmt , &vecPetBaseDataList );
// 	dbType = DB_PET_BASEDATA;
// 	
// 	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
// 	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
// 
// 	vectorPetBaseData::iterator itrPetBaseDataList = vecPetBaseDataList.begin();
// 
// 	while ( itrPetBaseDataList != vecPetBaseDataList.end() )
// 	{
// 		tPET_BASEDATA *pPetBaseData = &(*itrPetBaseDataList);
// 		WriteFile ( hFile , (LPCVOID)pPetBaseData , sizeof( tPET_BASEDATA ) , &dwBytesWritten , NULL );
// 		itrPetBaseDataList++;
// 	}
// 
// 
// 	vectorPetLevelData vecPetLevelDataList;
// 	
// 	nObjects = CAtumDBHelper::DBLoadPetLevelDataInfo ( &odbcStmt , &vecPetLevelDataList );
// 	dbType = DB_PET_LEVELDATA;
// 	
// 	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
// 	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(int), &dwBytesWritten, NULL);
// 	
// 	vectorPetLevelData::iterator itrPetLevelDataList = vecPetLevelDataList.begin();
// 	
// 	while ( itrPetLevelDataList != vecPetLevelDataList.end() )
// 	{
// 		tPET_LEVEL_DATA *pPetBaseData = &(*itrPetLevelDataList);
// 		WriteFile ( hFile , (LPCVOID)itrPetLevelDataList , sizeof( tPET_LEVEL_DATA ) , &dwBytesWritten , NULL );
// 		itrPetLevelDataList++;
// 	}

	// End 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업.

	// 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업. - 펫 오퍼레이터 정보.
	vectorOperatorInfo temp_vecOperatorDataList;
	
	dbType = DB_PET_OPERATOR;

	nObjects = CAtumDBHelper::DBLoadOperatorDataInfo( &odbcStmt,  &temp_vecOperatorDataList);
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(DB_TYPE), &dwBytesWritten, NULL);

	vectorOperatorInfo::iterator Operator_iter = temp_vecOperatorDataList.begin();
	OperatorInfo *pOperatorData = NULL;

	for( ; Operator_iter != temp_vecOperatorDataList.end() ; ++Operator_iter )
	{
		pOperatorData = &*Operator_iter;
		WriteFile(hFile, (LPCVOID)pOperatorData, sizeof(OperatorInfo), &dwBytesWritten, NULL);
	}
	//..

	// END 2010-06-15 by shcho&hslee 펫시스템 - omi 추가 작업. - 펫 오퍼레이터 정보.

	// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.		
	vectorDissolutionItemInfo temp_DisitemDataList;
	
	dbType = DB_DISSOLUTIONITEM;
	
	nObjects = CAtumDBHelper::DBLoadDissolutionItemDataInfo( &odbcStmt,  &temp_DisitemDataList);
	if(0 >= nObjects)
	{// 2008-01-11 by cmkwon, 가져온 정보 개수 체크
		MessageBox("LoadDissolutionItemData error from DB!!, Please check DB Schema.");
		return;
	}
	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	
	vectorDissolutionItemInfo::iterator DisItem_iter = temp_DisitemDataList.begin();
	tDissolutionItemInfo* pDisItemData = NULL;
	
	for( ; DisItem_iter != temp_DisitemDataList.end() ; ++DisItem_iter )
	{
		pDisItemData = &*DisItem_iter;
		WriteFile(hFile, (LPCVOID)pDisItemData, sizeof(tDissolutionItemInfo), &dwBytesWritten, NULL);
	}
	//..
	// END 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.		

	// start 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가		
	vectorMonsterMultiTarget MonsterMultiTargetList;

	dbType = DB_MONSTER_MULTI_TARGET;

	nObjects = CAtumDBHelper::DBLoadMonsterMultiTargetData(&odbcStmt,  &MonsterMultiTargetList);
	if(0 >= nObjects)
	{
		MessageBox("LoadMonsterMultiTargetData error from DB!!, Please check DB Schema.");
		return;
	}

	WriteFile(hFile, (LPCVOID)&dbType, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	WriteFile(hFile, (LPCVOID)&nObjects, sizeof(DB_TYPE), &dwBytesWritten, NULL);
	
	vectorMonsterMultiTarget::iterator TargetItem_iter = MonsterMultiTargetList.begin();
	MONSTER_MULTI_TARGET *pTargetItemData = NULL;
	
	for( ; TargetItem_iter != MonsterMultiTargetList.end() ; ++TargetItem_iter )
	{
		pTargetItemData = &*TargetItem_iter;
		WriteFile(hFile, (LPCVOID)pTargetItemData, sizeof(MONSTER_MULTI_TARGET), &dwBytesWritten, NULL);
	}
	// end 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

	odbcStmt.Clean();

	// close file
	CString tmp;
	tmp.Format("File was created successfully. \r\n\r\nDB Info: %s(%d), %s", szDBIP, nDBPort, szDatabaseName);
	MessageBox(tmp);
	CloseHandle(hFile);

	// write profile
	pMonitorApp->m_szInfoGenOutPath = m_szInfoGenOutPath;
	pMonitorApp->m_strGameServerName = dlg.m_strServerName;
	pMonitorApp->WriteProfile();
}

BOOL CInfoGenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	GetDlgItem(IDC_BUTTON_IG_ALL)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInfoGenDlg::OnButtonIgRareFix()
{
	// OnButtonCheckAll()로 통합
}
