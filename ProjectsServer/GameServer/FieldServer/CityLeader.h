// CityLeader.h: interface for the CCityLeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CITYLEADER_H__828585AD_B4A6_45DB_AA50_91756BA520E8__INCLUDED_)
#define AFX_CITYLEADER_H__828585AD_B4A6_45DB_AA50_91756BA520E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCP;

typedef 	mt_vector<SCITYLEADER_INFO>		mtvectSCityLeader;

class CCityLeader  
{
public:
	CCityLeader();
	virtual ~CCityLeader();

	void	InitCityLeader(CFieldIOCP * i_pFIOCP);
	void	SetCityLeaderByLoadDB(vectSCityLeaderInfo * i_pSCityLeaderInfo);				// 도시지휘자 정보를 DB로 부터 읽어 드린다.

	BOOL	CheckIsValidCharacter(int i_nMapIndex, UID32_t i_nCharacterUID);				// 유효한 유저인지 체크
	CHAR * 	GetNotice(int i_nMapIndex);		// 공지사항 가져오기
	void	SetNotice(int i_nMapIndex , UID32_t i_nCharacterUID, char * i_pNotice);	// 공지사항 설정
	
	void	GetExpediencyfund(int i_nMapIndex, SCITYLEADER_INFO * o_pSCityLeaderInfo);	// 판공비 정보 얻어오기
	void	UpdateExpediencyfundRateATLeader(int i_nMapIndex, int i_nChangeExpediencyFundRate, SCITYLEADER_INFO * o_pSCityLeaderInfo);  // 2008-08-20 by dhjin, 판공비율이 INT형으로 계산중이다. float -> INT로 수정	// 지도자 판공비율 수정후 판공비 정보 얻어오기

	Err_t	PayBackExpediencyfund(int i_nMapIndex , UID32_t i_nCharacterUID, int i_nPayBackCount, SCITYLEADER_INFO * o_pSCityLeaderInfo);	// 판공비 요청 

	void	SetExpediencyfund(byte i_byInfluence, int i_nSPICount);	// 판공비 설정
	void	InitExpediencyfund(int i_nMapindex, byte i_byInfluence, UID32_t i_nCharacterUID, float i_fExpediencyRate);	// 판공비 초기화

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	
	void	SetExpediencyFundRate(int i_nMapIndex , UID32_t i_nCharacterUID, int i_nExpediencyFundRate); // 판공비율 수정

protected:

	CFieldIOCP						*m_pFieldIOCP19;
	mtvectSCityLeader 				m_mtvectCityLeader;	// 도시지휘자 관련

};

#endif // !defined(AFX_CITYLEADER_H__828585AD_B4A6_45DB_AA50_91756BA520E8__INCLUDED_)
