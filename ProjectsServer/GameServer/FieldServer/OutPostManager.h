// OutPostManager.h: interface for the COutPostManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPOSTMANAGER_H__072CE381_6268_4DD7_BF2E_550A12FC6894__INCLUDED_)
#define AFX_OUTPOSTMANAGER_H__072CE381_6268_4DD7_BF2E_550A12FC6894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutPost;
class CFieldIOCP;

///////////////////////////////////////////////////////////////////////////////
// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
class CFieldMapChannel;
struct SOUTPOSTWAR_EV_RESET_SUCCESS		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
{
	BYTE				bySuccessInfluenceTy;
	CFieldMapChannel*	pFMChanOfOutPostWarMap;		// 전진기지전이 발생한 CFieldMapChannel
};
typedef vector<SOUTPOSTWAR_EV_RESET_SUCCESS>		vectSOUTPOSTWAR_EV_RESET_SUCCESS;

struct SOUTPOSTWAR_EV_END		// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 
{
	BYTE				byWinInfluenceTy;
	BYTE				byLossInfluenceTy;
	CFieldMapChannel*	pFMChanOfOutPostWarMap;		// 전진기지전이 발생한 CFieldMapChannel
	UID32_t				GuildUIDPossessOutPost;		// 전진기지를 소유한 GuildUID
	int					nCityMapIndexForWarp;		// 전진기지를 소유한 여단을 전진기지 도시맵으로 워프 시키기 위한 MapIndex
};
typedef vector<SOUTPOSTWAR_EV_END>					vectSOUTPOSTWAR_EV_END;

typedef mt_vector<COutPost*>		mtvectOutPost;
typedef	vector<ATUM_DATE_TIME>		vectATUM_DATE_TIME;	

class COutPostManager  
{
public:
	COutPostManager();
	virtual ~COutPostManager();

	void	InitOutPostManager(CFieldIOCP * i_pFIOCP);		
	COutPost * GetOutPost(int i_nMapIndex);
	COutPost * GetOutPostCityMap(int i_nCityMapIndex);
	void	SetOutPostManagerByDBLoad(vectSOutPostInfo * i_pVectOutPostInfo);	// 전진기지 관련 정보를 DB로 부터 읽어 전진기지 정보 수 만큼 Class OutPost를 생성하여 m_mtvectSOutPost에 추가한다.
	void	SetOutPostNextWarTimeInfoByDBLoad(vectATUM_DATE_TIME * i_pvectOutPostNextWarTimeInfo);	// 전진기지 전쟁 시간 관련 정보를 DB에서 읽어온다.

	void	OnDoMinutelyWorkOutPostManager(ATUM_DATE_TIME *i_pCurDateTime);	// 분 마다 전진기지 관련 이벤트 체크
	
	void	SendOutPostWarRemainTime(int i_nRemainTime, BYTE i_byInfluence, int i_nMapIndex);			// 전진기기 전투 시작 남은 시간을 클라이언트에게 전송한다.
	void	OutPostWarStart(COutPost * i_pOutPost);								// 전진기지 전 시작

	void	OnOutPostProtectorDestroy(int i_nMapIndex);		// 전진기지 보호막 파괴시 처리 
	void	OnOutPostResetDestroy(int i_nMapIndex);			// 전진기지 중앙처리 파괴시 처리 

	BOOL	CheckValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID);				// 유효한 여단인지 체크
	INT		GetOutPostCityMapIndexByGuildUID(UID32_t i_nGuildUID);					// 유효한 여단이면 전진기지 도시 맵 번호를 리턴한다.
	INT		GetOutPostMapIndexByGuildUID(UID32_t i_nGuildUID);						// 유효한 여단이면 전진기지 맵 번호를 리턴한다.
	BOOL	CheckResetValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID);				// 각인이 유효한 여단인지 체크
	BYTE	GetOutPostInfluenceByMapIndex(int i_nMapIdx);							// 2007-09-28 by cmkwon, 전진기지맵 워프 체크 수정 - 해당 전진기지의 소유 세력을 리턴한다.

	BOOL	SendOutPostResetStartByGuildCommander(int i_nMapIndex, UID32_t i_guildUID, UID32_t i_charUID);				// 2007-10-16 by cmkwon, 로그 추가 - 인자추가(, UID32_t i_guildUID, UID32_t i_charUID), // 여단장이 각인을 시작 함

	// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 인자추가(, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList)
	void	OutPostResetSuccess(COutPost * i_pOutPost, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList);		// 전진기지 각인 성공

	// 2009-07-08 by cmkwon, 전진 기지전 버그 수정 - 인자추가(, vectSOUTPOSTWAR_EV_END *io_pEvEndList)
	void 	OutPostWarEnd(COutPost * i_pOutPost, vectSOUTPOSTWAR_EV_END *io_pEvEndList);		// 전진기지 전 종료

	void	SetExpediencyFundByOutPostWarEnd(int i_nMapindex, int i_nCityMapindex, byte i_byInfluence, UID32_t i_nGuildUID);		// 판공비 설정
	void	UpdateExpediencyfundRateLeader(int i_nMapIndx, byte i_byInfluence, int i_nChangeExpediencyFundRate);	// 2008-08-20 by dhjin, 판공비율이 INT형으로 계산중이다. float -> INT로 수정 // 지도자 판공비율 설정을 증감한다.

	void	SetInitOutPostMonsterSummonByBell(byte i_byInfl, int * o_byBell1, int * o_byBell2, int * o_byBell3);	// 전진기지 초기화 할 때 몬스터를 생성하기 위한 Bell값 설정
	void	AddDamageOfProtectorW(int i_nMapIndex, BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage);		// 전진기지 보호막에 데미지를 준 여단과 데미지 값을 저장한다.

	void	MakeMsgOutPostNextWarInfoRequest(int i_nMapIndex, BOOL i_bCheckGuildCommander, BYTE * o_pData, int * o_nSize);		// 전진기지 시간 설정 정보 전송
	INT		MakeMSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK(ATUM_DATE_TIME * o_pOutPostNextWarTimeListInfo, int * o_nSelectTimeChoice, ATUM_DATE_TIME * i_OutPostNextWarTime);	// 전진기지 시간 설정 메세지 만들기
	Err_t	OutPostNextWarTimeSetByChoiceUserW(int i_nMapIndex, int i_nSelectTimeChoice, UID32_t i_guildUID, UID32_t i_charUID);	// 2007-10-16 by cmkwon, 로그 추가 - 인자추가(, UID32_t i_guildUID, UID32_t i_charUID), // 전진기지 다음 전투 시간 설정

	BOOL	CheckOutPostWarTimeStrategyPoint(ATUM_DATE_TIME *i_pStrategyPointSummonTime);	// 전략포인트 소환 시 전진기지 전쟁 시간에 걸리는지 체크

	ATUM_DATE_TIME * GetOutPostNextWarTime(int i_nMapIndex);				// 전진기지 다음 전투 시간 얻어오기
	ATUM_DATE_TIME * GetOutPostNextWarTimeByCityMapIndex(int i_nCityMapIndex);				// 도시 맵으로 전진기지 다음 전투 시간 얻어오기
	BOOL	CheckOutPostWaring(int i_nMapIndex);					// 전진기지 전쟁이 진행중인지 체크
	BOOL	CheckALLOutPostWaring();								// 전진기지 전쟁이 진행중인지 체크 모든 맵
	BOOL	CheckOutPostPossessByGuildUID(UID32_t i_nGuildUID);		// 전진기지를 소유한 여단인지 체크
	BOOL	CheckOutPostResetByGuildUID(UID32_t i_nGuildUID);		// 전진기지를 리셋중인 여단인지 체크
	
	INT		CheckAllOutPostWaringAndSendClient(SOUTPOST_WAR_INFO *o_pOutPostWarInfo);			// 전진기지 전쟁이 발발했는지 체크하여 클라이언트에게 정보를 전송한다.

	void	OutPostPossessNPCByDeleteGuild(UID32_t i_nGuildUID);				// 2007-11-09 by dhjin, 여단 삭제 요청으로 전진기지 소유 하고 있던 여단이었을 경우 NPC소유로 변경

	INT		MakeAllOutPostInfoMsg(SCITY_WARINFO_OUTPOST *o_pOutPostWarInfo);						// 2007-12-03 by dhjin, 전진기지 모든 정보 리스트 만들기

protected:
	
	CFieldIOCP					*m_pFieldIOCP18;
	mtvectOutPost		 		m_mtvectOutPostManager;	// 전진기지
	vectATUM_DATE_TIME			m_vectOutPostNextWarTimeInfo;	// 전진기지 시간 설정 정보

};

#endif // !defined(AFX_OUTPOSTMANAGER_H__072CE381_6268_4DD7_BF2E_550A12FC6894__INCLUDED_)
