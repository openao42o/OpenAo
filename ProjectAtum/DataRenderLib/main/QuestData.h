// QuestData.h: interface for the CQuestData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTDATA_H__543FA2F7_50DE_440D_BDBA_D6127C6BA923__INCLUDED_)
#define AFX_QUESTDATA_H__543FA2F7_50DE_440D_BDBA_D6127C6BA923__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QUEST_NAME_SIZE			60

typedef struct
{
	int			nIndex;
	char		szNPCName[QUEST_NAME_SIZE];
	char		szMAPName[QUEST_NAME_SIZE];
	char		szMissionName[QUEST_NAME_SIZE];
	char		szShopName[QUEST_NAME_SIZE];
	int			nNPCIndex;
} Quest_Data;

typedef struct
{
	int			nQuestIndex;
	int			nMonsterCount;
	int			nMonsterUnitkind;
} Quest_MonsterInfo;

// 2008-09-22 by bhsohn EP3 캐릭터 창
//class CINFCharacterInfo;
class CINFCharacterInfoExtend;

class CINFMission;
class CINFQuest;
///////////////////////////////////////////////////////////////////////////////
/// \class		CQuestData
///
/// \brief		퀘스트 정보 진행중,완료된 퀘스트 관리, UI관련 정보 추출 등
/// \author		dhkwon
/// \version	
/// \date		2004-04-28 ~ 2004-04-28
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CQuestData  
{
	friend class CShuttleChild;// 후에 지울것
	// 2008-09-22 by bhsohn EP3 캐릭터 창
	//friend class CINFCharacterInfo;
	friend class CINFCharacterInfoExtend;
	
	friend class CINFMission;
	friend class CINFGameMainMiniMap;
	friend class CStoreData;
public:
	CQuestData();
	virtual ~CQuestData();

	void Init();
	void AddCharacterQuest(int nQuestIndex, MEX_QUEST_INFO* pCharacterQuest);
	void DeleteCharacterQuest(int nQuestIndex);
	CQuest* FindQuest(int nQuestIndex);
	CQuest* FindMapHideQuest(int nMapIndex);
	CQuest* FindPosCompleteQuest(int nMapIndex);
	CQuest* FindFirstScenarioQuest(int nInfluence);


	MEX_QUEST_INFO* FindCharacterQuest(int nQuestIndex);
	void GetQuestList(int nNPCIndex);
	void MakeCompleteProgressMission(int nNPCIndex);

	void CheckClientQuestList();
	void CheckBeginnerQuest();

	BOOL IsExistStartQuest(int nNPCIndex);
	BOOL IsQuestProgress(INT nQuestIndex);
	BOOL IsQuestCompleted(INT nQuestIndex);
	BOOL IsQuestNotStart(INT nQuestIndex);
	void SendFieldSocketQuestRequestStart(int nQuestIndex, int nQuestStartType, BOOL bPassQuest=FALSE, char* strSerialKey=NULL);
	void SendFieldSocketQuestRequestSuccess(int nQuestIndex, int nQuestResult=0);
	void SendFieldSocketQuestRequestSuccessCheck(int nQuestIndex, int nQuestResult = 0);
	BOOL CheckEndQuestCondition(CQuest* pQuest, int nNPCIndex);
	void SetQuestEndNPCTalk( BOOL bSuccessful, int nQuestIndex, INT	nExpOfCompensation);
	
	void NotifyStartQuest();
	int  GetProgressQuestNum() { return m_nProgressCharacterQuestNum; }
	void NoticeQuestTargetItem( int nItemNum, int nCount );
	vector<int>  GetRaceQuestIndex(int nfluenceType);	
	CQuest* GetProgressMission();
	vector<Quest_MonsterInfo>* GetQuestMonster(){return &m_vecQuestMonsterCount;}
	Quest_MonsterInfo* FindQuestMonsterCountInfo(int i_nMonUnitKind);

	// 2007-07-09 by bhsohn 바자 상점과 상점 동시 오픈시에 대한 처리
	vector<int> GetRaceQuestIndex_ToVector(int nfluenceType);

	// 2008-11-11 by bhsohn 퀘스트 CheckSum추가
	void RefreshQuestCheckSum();
	
	// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
	//BOOL GetQuestCheckSum(UINT* o_uiCheckSum,int *o_nFileSize, char* o_chMapIdx);
	BOOL GetQuestCheckSum(BYTE o_byCheckSum[32],int *o_nFileSize, char* o_chMapIdx);
	// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템

	// 2008-12-16 by dgwoo 미션 마스터 퀘스트를 완료한것이 있는가.
	DWORD GetMissionMasterQuestCompletedCount();
	
public:
	vector<Quest_Data*>  m_vecQuest;
	vector<Quest_MonsterInfo> m_vecQuestMonsterCount;		// 퀘스트 몬스터 카운트

protected:
	BOOL IsSatisfyStartCondition(CQuest* pQuest);
	BOOL IsShowQuest(CQuest* pQuest);
protected:
	CMapQuest			m_mapQuest;							// 로딩하는 전체 퀘스트
	CMapCharacterQuest	m_mapCharacterQuest;				// 현재 완료, 진행되고 있는 퀘스트	

	int					m_nProgressCharacterQuestNum;		// 진행중인 퀘스트의 개수

	// 2007-07-09 by bhsohn 퀘스트 정보 리드 방식 변경
	vectCQuestPtr		m_vectCQuestInfoList;

	// 2008-11-11 by bhsohn 퀘스트 CheckSum추가
	BOOL				m_bGetCheckSum;
	
	// 2009. 05. 29 by ckPark SHA256 체크섬 시스템
	//UINT				m_uiCheckSum;
	BYTE				m_uiCheckSum[32];
	// end 2009. 05. 29 by ckPark SHA256 체크섬 시스템

	int					m_nFileSize;
	char				m_chMapIdx[64];
	
	
};

#endif // !defined(AFX_QUESTDATA_H__543FA2F7_50DE_440D_BDBA_D6127C6BA923__INCLUDED_)
