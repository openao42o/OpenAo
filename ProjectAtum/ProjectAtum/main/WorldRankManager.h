// WorldRankManager.h: interface for the CWorldRankManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDRANKMANAGER_H__DCE17F75_BAC9_420C_B0CA_0A56FF92D8EA__INCLUDED_)
#define AFX_WORLDRANKMANAGER_H__DCE17F75_BAC9_420C_B0CA_0A56FF92D8EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 월드 랭킹 탭
#define		WORLDRANK_SERVICE_LOCAL		0		// 지역랭킹
#define		WORLDRANK_SERVICE_WORLD		1		// 월드 랭킹
#define		MAX_WORLDRANK_SERVICE		2



// 월드 랭킹 항목들
#define		WORLDRANK_INFO_LV		0		// 레벨
#define		WORLDRANK_INFO_HONOR	1		// 명성치
#define		WORLDRANK_INFO_PVP		2		// Pvp
#define		MAX_WORLDRANK_INFO		3

// 각 기어들 
#define		WORLDRANK_GEAR_ALL		0		// 전체 기어
#define		WORLDRANK_GEAR_B		1		// B
#define		WORLDRANK_GEAR_M		2		// M
#define		WORLDRANK_GEAR_I		3		// I
#define		WORLDRANK_GEAR_A		4		// A
#define		MAX_WORLDRANK_GEAR		5		// 전체 기어,B,M,I,A (총 5개이다.)


#define		MAX_WORLDRANK_ONEPAGE	10		// 한페이지 최대 항목수
#define		MAX_WORLDRANK_PAGE		10		// 최대 페이지 수

struct structRankInfo
{
	int				nRank;				// 랭킹
	int				nLevel;				// 레벨
	int				nScore;				// 스코어
	int				nHonor;				// 개인명성
	int				nWinNum;			// 승수
	int				nLoseNum;			// 패배
	
	char			chNickName[SIZE_MAX_ARENA_FULL_NAME];		// 닉네임	
	char			chGearType[SIZE_MAX_ARENA_FULL_NAME];		// 기어 타입
	char			chInfluence[SIZE_MAX_ARENA_FULL_NAME];		// 세력명
	char			chServiceName[MAX_PATH];		// 서비스사
	char			chServerName[MAX_PATH];			// 서버명	
	
};

struct structServiceInfo
{	
	int	nDataPos;
	BOOL bImageLoadDone;
	CINFImageEx* pImage;	        // 2011. 10. 10 by jskim UI시스템 변경
	char*		pData;
	BOOL	bRestored;
};

typedef map<UINT, structServiceInfo*>		CMapServiceInfo;				// 길드 마크
typedef map<UINT, structServiceInfo*>::iterator CMapServiceInfoIterator;

class CWorldRankManager  
{
public:
	CWorldRankManager();
	virtual ~CWorldRankManager();

	
	void AddWorldRank(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
						int i_nInfoIdx,			// 레벨, 명성치, PVP
						int i_nGearIdx,			// 기어 인덱스
						structRankInfo*		pstruRankInfo);

	int GetWorldRankSize(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
							int i_nInfoIdx,			// 레벨, 명성치, PVP
							int i_nGearIdx,			// 기어 인덱스
							int i_nPage);			// 페이지 위치

	structRankInfo* GetWorldRankInfo(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
										int i_nInfoIdx,			// 레벨, 명성치, PVP
										int i_nGearIdx,			// 기어 인덱스
										int i_nSelectPage,		// 선택한 페이지
										int i_nIdx);			// 선택 인덱스	
	BOOL	RqGetServiceInfo();	// 서버 리스트를 얻어온다.

	void	AddServiceInfo(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK* pMsg);
	MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK		GetServiceInfo();
	BOOL	GetServiceStringInfo(char* o_pServer);

	void	AddServiceInfoImage(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE* pMsg);
	CINFImage* GetServiceInfoImage();
	void	DeleteServerImageInfo();

	BOOL	RqWorldRankInfo(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
						int i_nInfoIdx,			// 레벨, 명성치, PVP
						int i_nGearIdx,			// 기어 인덱스
						int i_nIdx,
						int i_nRqItemCnt,
						BOOL bCtrlRq);

	// 나의 랭킹 정보 요청
	int GetMyRankInfo(int i_nInfoIdx);// 레벨, 명성치, PVP)
	BOOL RqGetMyRankInfo();

	void AddWorldRank_Lv(int nStartRank, SWRK_LEVEL* pSwrk);
	void AddWorldRank_Fame(int nStartRank, SWRK_FAME* pSwrk);
	void AddWorldRank_PVP(int nStartRank, SWRK_PVP* pSwrk);

	BOOL	IsRqWorldRankInfo(int i_nLocalWorldIdx,			// 지역이냐? 월드냐?
								int i_nInfoIdx,			// 레벨, 명성치, PVP
								int i_nGearIdx,			// 기어 인덱스
								int i_nStartIdx);		// 시작 페이지

	void	RqCreateMyNickName(char* i_pNickName, UID32_t	i_myCharacterUniqueNumber);
	BOOL	IsValidNickName(char* i_pCharater);

	void	SetMyRankInfo(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK	*pstructMyRankInfo);

	void	InitDeviceObjects_ServiceInfoImage();
	void	RestoreDeviceObjects_ServiceInfoImage();
	void	InvalidateDeviceObjects_ServiceInfoImage();
	void	DeleteDeviceObjects_ServiceInfoImage();

	void	RqInitWorldRank();

	void	SetUseWorldRanking(BOOL bUseWorldRanking);
	BOOL	GetUseWorldRanking();

private:	
	vector<structRankInfo>		m_vecLocalRank_LV[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// 로컬 랭킹_레벨
	vector<structRankInfo>		m_vecLocalRank_Fame[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// 로컬 랭킹_명성치
	vector<structRankInfo>		m_vecLocalRank_PVP[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// 로컬 랭킹_PVP

	vector<structRankInfo>		m_vecWorldRank_LV[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// 월드 랭킹_레벨
	vector<structRankInfo>		m_vecWorldRank_Fame[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// 월드 랭킹_명성치
	vector<structRankInfo>		m_vecWorldRank_PVP[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// 월드 랭킹_PVP

	MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK		m_ServiceInfo;

	structServiceInfo*			m_ServiceInfoStruct;				// 서비스 마크 마크

	// 요청할 기어 인덱스
	int							m_nRqGearIdx;

	MSG_FC_INFO_WRK_GET_SELF_RANKING_OK	m_structMyRankInfo;

	BOOL					m_bUseWorldRanking;		// 월드 랭킹 시스템 사용 유무

};

#endif // !defined(AFX_WORLDRANKMANAGER_H__DCE17F75_BAC9_420C_B0CA_0A56FF92D8EA__INCLUDED_)
